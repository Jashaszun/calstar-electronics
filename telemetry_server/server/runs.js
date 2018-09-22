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

// YOUR CODE HERE
var getRuns = function (req, res) {

}

module.exports = getRuns // rename however you want
