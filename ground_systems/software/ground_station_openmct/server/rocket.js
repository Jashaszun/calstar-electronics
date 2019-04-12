const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const rl = require('readline');

function Rocket(comPort) {
    this.state = {
        "fc.pwr": 0,
        "fc.state": 0,
        "fc.alt": 0,
        "tpc.gps": "",
        "transmission": "",
        "comms.recd": 0,
        "comms.sent": 0,
        "gs.rssi": 0,
        "tpc.bat_v": "",
        "tpc.state": ""
    };
    this.total_comms_recd = 0;
    this.history = {};
    this.listeners = [];
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    // setInterval(() => {
    //     this.generateTelemetry();
    // }, 500);

    const port = new SerialPort(comPort, { baudRate: 115200 });
    const parser = port.pipe(new Readline());

    parser.on('data', (line) => {
        // console.log(line);
        try {
            // console.log(JSON.parse("{\"timestamp\": 1, \"id\": \"gs.rssi\", \"value\": -30}"));
            obj = JSON.parse(line);
            obj["timestamp"] = Date.now();
            // obj["timestamp"] = Math.round(obj["timestamp"] / 1000000);
            if (obj["id"] === "comms.recd") {
                this.total_comms_recd += obj["value"];
                obj["value"] = this.total_comms_recd;
            }
            console.log(obj);
            this.notify(obj);
            this.history[obj["id"]].push(obj);
        } catch (e) {
            if (e instanceof SyntaxError) {
                console.log("syntax error");
            }
            console.log("error");
        }

        this.updateConsole(line);
    });

    process.stdin.on('data', (line) => {
        // const words = String(line).split(' ');
        // this.state[words[0]] = Number(words[1]);
        // this.generateTelemetry();
    });

    // Clear console
    process.stdout.write('\033c');
};

/**
 * Takes a measurement of spacecraft state, stores in history, and notifies 
 * listeners.
 */
Rocket.prototype.generateTelemetry = function () {
    var timestamp = Date.now(), sent = 0;
    Object.keys(this.state).forEach(function (id) {
        var state = { timestamp: timestamp, value: this.state[id], id: id };
        this.notify(state);
        console.log(state);
        this.history[id].push(state);
        this.state["comms.sent"] += JSON.stringify(state).length;
        this.state["transmission"] += ", " + id + ": " + String(this.state[id]);
    }, this);
    this.state["transmission"] = "";

};

Rocket.prototype.updateConsole = function (line) {
    // TODO: Make this look nice and separate data cleanly
    rl.cursorTo(process.stdout, 0, 20);
    rl.clearLine(process.stdout);
    process.stdout.write(line);
}

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

module.exports = function (comPort) {
    return new Rocket(comPort)
};
