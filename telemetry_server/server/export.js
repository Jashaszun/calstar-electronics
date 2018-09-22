/**
 * Access control: CalSTAR MA
 *
 * Handles GET requests to the /export endpoint.
 * Exports a function (request, response) that returns void, to be used
 * as express middleware.
 *
 * This API endpoint gives the file contents to the querier.
 *
 * For further details, refer to the design doc (linked in the README).
 */

const fs = require('fs')
const path = require('path')

// YOUR CODE HERE
var getExport = function (req, res) {

}

module.exports = getExport // rename however you want
