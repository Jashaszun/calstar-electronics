/**
 * Basic implementation of a history and realtime server.
 */

var comPort;
if (process.argv.length != 3) {
    console.log("Must provide the COM port on which to listen to the ground station.");
    process.exit(1);
} else {
    comPort = process.argv[2];
}

var Rocket = require('./rocket');
var RealtimeServer = require('./realtime-server');
var HistoryServer = require('./history-server');
var StaticServer = require('./static-server');

var expressWs = require('express-ws');
var app = require('express')();
expressWs(app);

var rocket = new Rocket(comPort);
var realtimeServer = new RealtimeServer(rocket);
var historyServer = new HistoryServer(rocket);
var staticServer = new StaticServer();

app.use('/realtime', realtimeServer);
app.use('/history', historyServer);
app.use('/', staticServer);

var port = process.env.PORT || 8080

app.listen(port, function () {
    console.log('Open MCT hosted at http://localhost:' + port);
    console.log('History hosted at http://localhost:' + port + '/history');
    console.log('Realtime hosted at ws://localhost:' + port + '/realtime');
});
