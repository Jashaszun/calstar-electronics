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
const bcrypt = require('bcrypt')
const saltRounds = 10

var createUser = function (req, res) {
  if (req.body.email && req.body.password) {
    var passHash = bcrypt.hashSync(req.body.password, saltRounds)
    db.pool.execute(
      'INSERT INTO Users ( email, password ) VALUES ( ?, ? )',
      [req.body.email, passHash],
      function (err, results, fields) {
        if (err) {
          logger.error(err)
          res.redirect('/login')
        } else {
          logger.log('Created user with email ' + req.body.email + ' and a hashed password. The user will require authorization before access is granted.')
          res.redirect('/runs')
        }
      }
    )
  }
}

var postLogin = function (req, res) {
  db.pool.query(
    'SELECT * FROM Users WHERE email = ?',
    [req.body.email],
    function (err, results, fields) {
      if (err) {
        res.end()
        logger.error(err)
      } else {
        if (results.length < 1) {
          // no user exists for that email address
          res.redirect('/login')
          return
        } else if (results.length > 1) {
          logger.warn('Multiple users found for email address ' + req.body.email + '. Aborting login request...')
          res.redirect('/login')
          return
        }
        if (bcrypt.compareSync(req.body.password, results[0].password)) {
          req.session.userId = results[0].userId
          req.session.email = results[0].email
          req.session.authorized = results[0].authorized
          logger.info('Successfully logged in user with email ' + req.body.email)
          res.redirect('/runs')
        } else {
          logger.warn('Unsuccessful login attempt for user with email ' + req.body.email)
          res.redirect('/login')
        }
      }
    }
  )
}

module.exports.postLogin = postLogin
module.exports.createUser = createUser
