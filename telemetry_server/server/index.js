/*
 * Access control: CalSTAR MA
 *
 * The entry point of the server. Responds to requests and stuff.
 *
 * To add a new API endpoint, please put the code in a separate file, import
 * it here, and mount it as appropriate.
 * Jonathan doesn't really know how routing works, so if you want to do that
 * to improve readability then feel free to do so.
 */

// dependencies
const express = require('express')
const app = express()
const server = require('http').Server(app)
const io = require('socket.io')(server)
const logger = require('loggy')
const path = require('path')
const fileUpload = require('express-fileupload')
const { sanitizeBody } = require('express-validator/filter')
const { check } = require('express-validator/check')
const mustacheExpress = require('mustache-express')
const session = require('express-session')

// API endpoints
const getExport = require('./export')
const getReadData = require('./read-data')
const { getRuns, getRun, removeRun } = require('./runs')
const { postLogin, createUser } = require('./users')
const postUpload = require('./upload')

const PORT = 8000
const BASE_DIR = path.join(__dirname, '..') // parent directory

const requireAuthentication = function (req, res, next) {
  if (req.session && req.session.userId && req.session.authorized == 1) {
    return next()
  } else {
    return res.redirect('/login')
  }
}

app.use(fileUpload())

app.use(session({
  secret: '7O72<8Z&<0M:l.rc%3m3+T+*/.F2s.Z',
  resave: true,
  saveUninitialized: false,
  cookie: {
    maxAge: 600000 // 600000 milliseconds = 10 minutes
  }
}))

// fixes weird path resolution stuff on localhost
// https://stackoverflow.com/questions/40574159/
app.use(express.static(path.join(BASE_DIR, 'html')))

app.engine('mustache', mustacheExpress())

app.set('view engine', 'mustache')
app.set('views', path.join(__dirname, '/views'))

app.get('/', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'index.html'))
})

app.get('/upload', requireAuthentication, function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_form.html'))
})

app.get('/uploadfail', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_fail.html'))
})

app.get('/uploadsuccess', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_success.html'))
})

app.get('/newuser', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'newuser.html'))
})

app.get('/login', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'login.html'))
})

app.post('/createuser', [sanitizeBody('email').normalizeEmail(), sanitizeBody('password')], createUser)

app.post('/login', [sanitizeBody('email').normalizeEmail(), sanitizeBody('password')], postLogin)

app.get('/export/:id', requireAuthentication, getExport)

// Oops.. export is doing this
// app.get('/readData', getReadData)

app.get('/runs', requireAuthentication, getRuns)

app.get('/runs/:id', requireAuthentication, getRun)

app.post('/deleteRuns/:id', requireAuthentication, removeRun)

app.post('/upload', requireAuthentication, sanitizeBody('newrun').toBoolean(), postUpload)

app.get('/logout', function (req, res, next) {
  if (req.session && req.session.userId) {
    // delete session object
    logger.info('Attempting to log user out with email ' + req.session.email)
    req.session.destroy(function (err) {
      if (err) {
        logger.error(err)
        return next(err)
      } else {
        logger.info('Successfully logged user out.')
        return res.redirect('/')
      }
    })
  } else {
    return res.redirect('/')
  }
})

server.listen(PORT, function (err) {
  if (err) {
    logger.error('Error starting server')
    logger.error(err)
  } else {
    logger.log(`Started web server; listening on *:${PORT}`)
  }
})
