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
const fs = require('fs')
const path = require('path')

const csvSaveDir = path.join(__dirname, '..', 'data')

var saveCSV = function (runId, runFile) {
  fs.writeFile(path.join(csvSaveDir, 'run-' + runId + '.csv'), runFile.data, function (err) {
    if (err) {
      logger.error('Error writing CSV for run-' + runId)
      logger.error(err)
    } else {
      logger.log('Wrote CSV for run-' + runId)
    }
  })
}

var postUpload = async function (req, res, next) {
  var runfileText = req.files.runfile.data.toString()
  // WARNING: Do not mix \r\n and \n
  const runData = parse(runfileText, {
    columns: true,
    trim: true,
    skip_empty_lines: true
  })

  var count = runData.length * Object.keys(runData[0]).length
  var insertDataPoints = async function (runId, dataTypeId, key) {
    try {
      logger.info(runData)
      for (let [index, row] of Object.entries(runData)) {
        await db.pool.execute(
          'INSERT INTO DataPoint (dataIndex, runId, dataTypeId, value) VALUES (?, ?, ?, ?)',
          [index, runId, dataTypeId[key], row[key]]
        )
        count -= 1 // race condition
        if (count === 0) {
          return res.redirect('/uploadsuccess')
        }
      }
    } catch (err) {
      logger.error('Error inserting into runs (from upload.js)')
      logger.error(err)
      res.redirect('/uploadfail')
    }
    return false
  }

  // Create a new run
  db.pool.execute('INSERT INTO Runs (runName) VALUES (?)', [req.files.runfile.name])
    .then(function ([results, _]) {
      var runId = results.insertId
      logger.log(`Inserted run with runId = ${runId}`)

      saveCSV(runId, req.files.runfile)
      db.pool.query('SELECT * FROM DataType', [])
        .then(async function ([datatypes, _]) {
          var dataTypeId = {}
          datatypes.forEach(function (element) {
            dataTypeId[element.name] = element.dataTypeId
          })
          for (let key of Object.keys(runData[0])) {
            if (!(key in dataTypeId)) {
              dataTypeId[key] = -1
              var results = (await db.pool.execute( // second param is unused
                'INSERT INTO DataType (type, name, units) VALUES (\'Unknown\', ?, \'Unknown\')',
                [key]
              ))[0]
              dataTypeId[key] = results.insertId
            }
            // insert all data with this key
            // MUST be await bc this function is stateful
            var errored = await insertDataPoints(runId, dataTypeId, key)
            if (errored) {
              return
            }
          }
        })
        .catch(err => logger.error(err))
    })
    .catch(err => {
      logger.error('Error inserting into runs (from upload.js)')
      logger.error(err)
      res.redirect('/uploadfail')
    })
}

module.exports = postUpload
