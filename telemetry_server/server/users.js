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
const { OAuth2Client } = require('google-auth-library');

const CLIENT_ID = "319896366608-t41p578v6ocoolm3ledr0ikcp0dh11hq.apps.googleusercontent.com"
const client = new OAuth2Client(CLIENT_ID);

var verifyOAuth = async function (token, expectedEmail) {
  // https://developers.google.com/identity/sign-in/web/backend-auth
  const ticket = await client.verifyIdToken({
      idToken: token,
      audience: CLIENT_ID
  })
  const payload = ticket.getPayload()
  const userid = payload['sub']
  logger.log(payload)
  return 'email' === expectedEmail
}

var postLogin = function (req, res) {
  db.pool.query(
    'SELECT * FROM Users WHERE email = ?', // all users in this table are considered authorized
    [req.body.email],
    function (err, results, fields) {
      if (err) {
        res.end()
        logger.error(err)
      } else {
        if (results.length < 1) {
          // no authorized user exists for that email address
          res.redirect('/login')
          return
        } else if (results.length > 1) {
          logger.warn('Multiple users found for email address ' + req.body.email + '. Aborting login request...')
          res.redirect('/login')
          return
        }
        var verified = verifyOAuth(req.idtoken, req.body.email)
        if (verified) {
          req.session.userId = results[0].userId
          req.session.email = results[0].email
          req.session.authorized = 1
          logger.info('Successfully logged in authorized user with email ' + req.body.email)
          res.redirect('/runs')
        } else {
          req.session.authorized = 0
          logger.info('Failed to log in user with email ' + req.body.email)
          res.redirect('/login')
        }
      }
    }
  )
}

module.exports.postLogin = postLogin
