/**
 * Access control: CalSTAR MA
 *
 * Handles GET requests to the /runs and /runs/{id} endpoints.
 * The former returns a list of all runs stored on the server (probably along
 * with some information about each, such as date, description, name, and ID).
 * The latter returns more specific information/metadata for a given run.
 *
 * For further details, refer to the design doc (linked in the README).
 */

const fs = require('fs')
const path = require('path')
const db = require('./db-interface')
const logger = require('loggy')

var getRuns = function (req, res) {
  db.pool.query(
    'SELECT * FROM Runs',
    function (err, results, fields) {
      if (err) {
        res.end()
        logger.error(err)
      }
      res.render('runs', {runs: results})
      logger.info(results) // results contains rows returned by server
      logger.info(fields) // fields contains extra meta data about results, if available
    }
  )
}

var getRun = function (req, res) {
  id = req.params["id"]
  db.pool.query(
    'SELECT * FROM DataPoint WHERE runId = ?',
    [id],
    function (err, results, fields) {
      if (err) {
        res.end()
        console.log(err)
      }
      res.render('runview', {data: results})
    }
  )
}

var removeRun = function (req, res) {
  console.log("Ya got here!")
  /*db.pool.execute(
    'DELETE FROM Runs WHERE runId = ?', [id],
    function (err, results, fields) {
        if (err) {
          logger.error('Error removing from runs (from upload.js)')
          logger.error(err)
          res.redirect('/uploadfail')
        } else {
          runId = results.insertId
          logger.log(`Removed run with runId = ${runId}`)
        }
      }
  )*/
  logger.info("The params: "+Object.keys(req))
}

module.exports.getRuns = getRuns
module.exports.getRun = getRun
module.exports.removeRun = removeRun
