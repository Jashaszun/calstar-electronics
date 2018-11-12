/**
 * Access control: CalSTAR MA
 *
 * Provides a single unified interface into the SQL server, allowing us to use a
 * single connection pool to speed up queries.
 * This file exports a connection pool object from the mysql2 library.
 * For details on usage, see the package docs:
 * https://www.npmjs.com/package/mysql2#using-connection-pools
 *
 * If you need to change the export of this file, be VERY CAREFUL so as to prevent
 * changes in the interface of the exported object from affecting other files.
 */

const sql = require('mysql2/promise')

// TODO store settings in a json file, or in .env
const pool = sql.createPool({
  host: 'localhost',
  user: 'telemetry',
  database: 'telemetry',
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0
})

module.exports.pool = pool
