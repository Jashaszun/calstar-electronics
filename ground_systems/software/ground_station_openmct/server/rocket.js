const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

function Rocket() {
    this.state = {
        "fc.state": 0,
        "accel.x": 0,
        "accel.y": 0,
        "accel.z": 1,
        "alt": 0,
        "transmission": "",
        "comms.recd": 0,
        "comms.sent": 0
    };
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    setInterval( () => {
        this.generateTelemetry();
    }, 500);

    const port = new SerialPort('/dev/ttyS7', {baudRate: 115200});
    const parser = port.pipe(new Readline());

    parser.on('data', (line) => {
        const words = String(line).split(' ');
        if (words.length == 2) {
            this.state[words[0]] = String(words[1]);
            this.generateTelemetry();
        }
    });

    process.stdin.on('data', (line) => {
        const words = String(line).split(' ');
        this.state[words[0]] = Number(words[1]); 
        this.generateTelemetry();
    });
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies 
 * listeners.
 */
Rocket.prototype.generateTelemetry = function () {
    var timestamp = Date.now(), sent = 0;
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id};
        this.notify(state);
        this.history[id].push(state);
        this.state["comms.sent"] += JSON.stringify(state).length;
        this.state["transmission"] += ", " + id + ": " + String(this.state[id]);
    }, this);
    this.state["transmission"] = "";
};

Rocket.prototype.notify = function (point) {
    this.listeners.forEach(function (l) {
        l(point);
    });
};

Rocket.prototype.listen = function (listener) {
    this.listeners.push(listener);
    return () => {
        this.listeners = this.listeners.filter(function (l) {
            return l !== listener;
        });
    };
};

module.exports = function () {
    return new Rocket()
};
