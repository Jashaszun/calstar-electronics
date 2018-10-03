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

// API endpoints
const getExport = require('./export')
const getReadData = require('./read-data')
const getRuns = require('./runs')
const postUpload = require('./upload')

const PORT = 8000
const BASE_DIR = path.join(__dirname, '..') // parent directory

app.use(fileUpload())

// fixes weird path resolution stuff on localhost
// https://stackoverflow.com/questions/40574159/
app.use(express.static(path.join(BASE_DIR, 'html')))

app.get('/', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'index.html'))
})

app.get('/upload', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_form.html'))
})

app.get('/uploadfail', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_fail.html'))
})

app.get('/uploadsuccess', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'upload_success.html'))
})

app.get('/export', getExport)

app.get('/readData', getReadData)

app.get('/runs', getRuns)

app.post('/upload', sanitizeBody('newrun').toBoolean(), postUpload)

server.listen(PORT, function (err) {
  if (err) {
    logger.error('Error starting server')
    logger.error(err)
  } else {
    logger.log(`Started web server; listening on *:${PORT}`)
  }
})
