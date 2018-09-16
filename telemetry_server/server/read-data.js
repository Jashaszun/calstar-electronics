const db = require('mysql2')

// TODO store settings in a json file, or in .env
const pool = db.createPool({
  host: 'localhost',
  user: 'telemetry',
  database: 'telemetry',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0
})

// YOUR CODE HERE
var getReadData = function (req, res) {

}

module.exports = getReadData // rename however you want
