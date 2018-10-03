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

var insertData = function (dataIndex, runId, dataTypeId, value, callback) {
  db.pool.execute(
    'INSERT INTO DataPoint ( dataIndex, runId, dataTypeId, value ) VALUES ( ?, ?, ?, ?)',
    [dataIndex, runId, dataTypeId, value],
    callback
  )
}

// YOUR CODE HERE
var postUpload = function (req, res, next) {
  const rundata = parse(req.files.runfile.data, {
    columns: true,
    skip_empty_lines: true
  })

  // TODO: Read from a form to see if new run should be created
  newRun = req.body.newrun
  runId = 1 // otherwise get from form
  dataTypeId = req.body.datatype
  count = 0
  if (newRun) {
    // Create a new run
    db.pool.execute(
      'INSERT INTO Runs (runId) VALUES (DEFAULT)', [],
      function (err, results, fields) {
        if (err) {
          console.log('ERROR INSERTING INTO Runs: (upload.js line 37)')
          console.log(err)
          res.redirect('/uploadfail')
        }
        runId = results.insertId
        console.log('Inserted run with runId = ' + runId)
        // yucky code duplication
        rundata.forEach(data => {
          // insert data points into database
          Object.keys(data).forEach(key => {
            if (key != 'ID' && key != 'Ignore') {
              count += 1
              insertData(data['ID'], runId, db.dataTypeId[key], data[key], function (err, results, fields) {
                if (err) {
                  console.log(err)
                  count = -1000
                  res.redirect('/uploadfail')
                } else {
                  count -= 1
                  if (count == 0) {
                    // we're done
                    console.log('Redirecting to upload success page.')
                    res.redirect('/uploadsuccess')
                    next()
                  }
                }
              })
            }
          })
        })
      }
    )
  } else {
    rundata.forEach(data => {
      // insert data points into database
      Object.keys(data).forEach(key => {
        if (key != 'ID' && key != 'Ignore') {
          count += 1
          insertData(data['ID'], runId, db.dataTypeId[key], data[key], function (err, results, fields) {
            if (err) {
              console.log(err)
              count = -1000
              res.redirect('/uploadfail')
            } else {
              count -= 1
              if (count == 0) {
                // we're done
                console.log('Redirecting to upload success page.')
                res.redirect('/uploadsuccess')
                next()
              }
            }
          })
        }
      })
    })
  }
}

module.exports = postUpload // rename if you want
