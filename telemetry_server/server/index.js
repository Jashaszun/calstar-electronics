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

app.use(express.static(BASE_DIR)) // https://stackoverflow.com/questions/40574159/

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
