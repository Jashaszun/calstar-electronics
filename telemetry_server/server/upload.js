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

const db = require('./db-interface.js')

// YOUR CODE HERE
var postUpload = function (req, res) {

}

module.exports = postUpload // rename if you want
