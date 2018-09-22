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

// API endpoints
const getExport = require('./export')
const getReadData = require('./read-data')
const getRuns = require('./runs')
const postUpload = require('./upload')

const PORT = 8000
const BASE_DIR = path.join(__dirname, '..') // parent directory

// fixes weird path resolution stuff on localhost
// https://stackoverflow.com/questions/40574159/
app.use(express.static(BASE_DIR))

app.get('/', function (req, res) {
  res.sendFile(path.join(BASE_DIR, 'html', 'index.html'))
})

app.get('/export', getExport)

app.get('/readData', getReadData)

app.get('/runs', getRuns)

app.post('/upload', postUpload)

server.listen(PORT, function (err) {
  if (err) {
    logger.error('Error starting server')
    logger.error(err)
  } else {
    logger.log(`Started web server; listening on *:${PORT}`)
  }
})
