/**
 * Access control: CalSTAR MA
 *
 * Handles POST requests to the /upload endpoint.
 * Exports a function (request, response) that returns void, to be used
 * as express middleware.
 *
 * This API endpoint handles uploads of raw test files; for now, these
 * files can be .wbpz (ANSYS files used by sims) or .csv; we only need
 * to worry about parsing .csv files. The .csv files should be parsed,
 * and their data uploaded into the SQL database as appropriate.
 *
 * For further details, refer to the design doc (linked in the README).
 */

const db = require('./db-interface')
const parse = require('csv-parse/lib/sync')
const logger = require('loggy')

var insertData = function (dataIndex, runId, dataTypeId, value, callback) {
  db.pool.execute(
    'INSERT INTO DataPoint ( dataIndex, runId, dataTypeId, value ) VALUES ( ?, ?, ?, ?)',
    [dataIndex, runId, dataTypeId, value],
    callback
  )
}

// YOUR CODE HERE
var postUpload = function (req, res, next) {
  const runData = parse(req.files.runfile.data, {
    columns: true,
    skip_empty_lines: true
  })

  // TODO: Read from a form to see if new run should be created
  var newRun = req.body.newrun
  var runId = 1 // otherwise get from form
  var dataTypeId = req.body.datatype
  var count = 0
  var insertRun = function (data) {
    // insert data points into database
    Object.keys(data).forEach(key => {
      if (key !== 'ID' && key !== 'Ignore') {
        count += 1
        insertData(data['ID'], runId, db.dataTypeId[key], data[key], function (err, results, fields) {
         if (err) {
            logger.error(err)
            count = -1000
            res.redirect('/uploadfail')
          } else {
            count -= 1
            if (count === 0) {
              // we're done
              logger.info('Redirecting to upload success page.')
              res.redirect('/uploadsuccess')
              next()
            }
          }
        })
      }
    })
  }
  if (newRun) {
    // Create a new run
    db.pool.execute(
      'INSERT INTO Runs (runId) VALUES (DEFAULT)', [],
      function (err, results, fields) {
        if (err) {
          logger.error('Error inserting into runs (from upload.js)')
          logger.error(err)
          res.redirect('/uploadfail')
        } else {
          runId = results.insertId
          logger.log(`Inserted run with runId = ${runId}`)
          runData.forEach(insertRun)
        }
      }
    )
  } else {
    runData.forEach(insertRun)
  }
}

module.exports = postUpload // rename if you want
