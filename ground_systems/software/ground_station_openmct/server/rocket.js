const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');
const rl = require('readline');
const chalk = require('chalk');
const nmea = require('@drivetech/node-nmea')

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
        "tpc.state": "",
        "test.telemetry": ""
    };
    this.total_comms_recd = 0;
    this.history = {};
    this.listeners = [];
    this.t0 = Date.now();
    this.set_t0 = false;
    this.timestamp0 = 0;
    Object.keys(this.state).forEach(function (k) {
        this.history[k] = [];
    }, this);

    // setInterval(() => {
    //     // this.generateTelemetry();
    //     obj = {
    //         "timestamp": this.t0 + this.tlast,
    //         "id": "test.telemetry",
    //         "value": Math.sin(this.tlast * 0.05)
    //     };
    //     this.tlast = this.tlast + 500;
    //     this.notify(obj);
    //     this.history[obj["id"]].push(obj);
    // }, 500);

    const port = new SerialPort(comPort, { baudRate: 115200 });
    const parser = port.pipe(new Readline());
    // port.on('open', (data) => {
    //     console.log("Port opened");
    // });

    parser.on('data', (line) => {
        try {
            obj = JSON.parse(line);

            if (this.set_t0 === false) {
                this.t0 = Date.now();
                this.timestamp0 = obj["timestamp"];
            }
            obj["timestamp"] = this.t0 + Math.round((obj["timestamp"] - this.timestamp0) / 1000);

            if (obj["id"] === "comms.recd") {
                this.total_comms_recd += obj["value"];
                obj["value"] = this.total_comms_recd;
            } else if (obj["id"] === "tpc.gps") {
                // const gps_data = nmea.parse(obj["value"]);
                // if (data.valid) {
                // obj["value"] = data.loc.coordinates;
                // }
            }
            // console.log(obj);
            this.notify(obj);
            this.history[obj["id"]].push(obj);
            this.updateConsole(obj);
        } catch (e) {
            rl.cursorTo(process.stdout, 0, 35);
            if (e instanceof SyntaxError) {
                console.log("syntax error");
            }
            console.log("error");
            console.log(e.message);
            console.log(line);
        }
    });

    rl.emitKeypressEvents(process.stdin);
    process.stdin.setRawMode(true);
    process.stdin.on('keypress', (str, key) => {
        // console.log("Str: " + str);
        // console.log("Key: ");
        // console.log(key);
        if (key && ((key.ctrl && key.name == 'c') || key.name == 'q')) {
            // Raw mode so we have to do our own Ctrl-C
            // console.log("Exiting...");
            process.exit();
        } else if (key && key.name == 'o') {
            port.write("o\n", function (err) {
                if (err) {
                    return console.log('Error on write: ', err.message)
                }
                // console.log('Turned off FC')
            });
        } else if (key && key.name == 'n') {
            port.write("n\n", function (err) {
                if (err) {
                    return console.log('Error on write: ', err.message)
                }
                // console.log('Turned on FC')
            });
        }
    });

    // process.stdin.on('data', (line) => {
    //     // const words = String(line).split(' ');
    //     // this.state[words[0]] = Number(words[1]);
    //     // this.generateTelemetry();
    // });

    this.drawTable();
    console.log("Transceiving on port " + comPort);
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

var tableRows = {};
var baseLine = 10;
var updateTimeouts = {};
var ageDataTimeout = 3000; // in milliseconds
var tableWidth;
var tableValueCol;
var tableValueMaxLen;
var fcStateDrawInfo;
Rocket.prototype.drawTable = function() {
    // Clear console
    process.stdout.write('\033c');

    rl.cursorTo(process.stdout, 0, baseLine);
    tableWidth =         "-----------------------------------------------------------".length; // same length as below lines
    tableValueCol =      "---------------".length; // length up to start of "Value" table
    tableValueMaxLen =                  "-------------------------------------------".length;
    process.stdout.write("╔═══════════╤═════════════════════════════════════════════╗\r\n");
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║  ID (*)   │  Value                                      ║\r\n");
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("╠═══════════╪═════════════════════════════════════════════╣\r\n");
    process.stdout.write("║gs.rssi    │                                             ║\r\n"); tableRows["gs.rssi"] = 5;
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║comms.recd │                                             ║\r\n"); tableRows["comms.recd"] = 7;
    process.stdout.write("║comms.sent │                                             ║\r\n"); tableRows["comms.sent"] = 8;
    process.stdout.write("╟───────────┼─────────────────────────────────────────────║\r\n");
    process.stdout.write("║tpc.state  │                                             ║\r\n"); tableRows["tpc.state"] = 10;
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║tpc.bat_v  │                                             ║\r\n"); tableRows["tpc.bat_v"] = 12;
    process.stdout.write("║tpc.bat_v *│                                             ║\r\n"); tableRows["tpc.bat_v*"] = 13;
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║tpc.gps    │                                             ║\r\n"); tableRows["tpc.gps"] = 15;
    process.stdout.write("╟───────────┼─────────────────────────────────────────────║\r\n");
    process.stdout.write("║fc.pwr     │                                             ║\r\n"); tableRows["fc.pwr"] = 17;
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║fc.state   │                                             ║\r\n"); tableRows["fc.state"] = 19;
    process.stdout.write("║           │                                             ║\r\n");
    process.stdout.write("║fc.alt     │                                             ║\r\n"); tableRows["fc.alt"] = 21;
    process.stdout.write("╚═══════════╧═════════════════════════════════════════════╝\r\n");
    process.stdout.write("\r\nTurn on FC: n\tTurn off FC: o");
    process.stdout.write("                                 ");
    process.stdout.write(chalk.green("Green: Complete") + "   " + chalk.blueBright("Blue: Current") + "\r\n");
    process.stdout.write("Quit: q\r\n");
    tableRows["$END"] = 26;

    // Important: These states should match the states in //firmware-launch/general/msg_fc_update.fbs
    // They must also be in the same order
    // E.g. if msg_fc_update.fbs contains "enum FCState : byte { pad = 0, flight = 1 }"
    // then this should be:
    // fcStateDrawInfo = [
    //     { state: "pad",    line: x },
    //     { state: "flight", line: y }
    // ];
    // where "pad" comes directly before "flight" and x < y
    fcStateDrawInfo = [
        { state: "setup",           line: 1 },
        { state: "pad",             line: 3 },
        { state: "flight",          line: 5 },
        { state: "drogue_deployed", line: 9 },
        { state: "drogue_coast",    line: 11 },
        { state: "main_deployed",   line: 15 },
        { state: "main_coast",      line: 17 },
        { state: "landed",          line: 21 }
    ];

    this.drawFCState(undefined);

    // Move back to top left for the "xxx hosted at yyy" messages
    rl.cursorTo(process.stdout, 0, 0);
};
Rocket.prototype.updateConsole = function(obj) {
    if (obj.id === "tpc.gps" && obj.value === "") {
        // We sometimes don't get sent GPS strings. Just ignore it in that case.
        return;
    }

    if (updateTimeouts[obj.id]) {
        clearTimeout(updateTimeouts[obj.id]);
    }

    var value;
    var suffix = "";
    if (obj.id === "comms.recd" || obj.id === "comms.sent") {
        suffix = " bytes";
    } else if (obj.id === "tpc.bat_v") {
        value = obj.value.toFixed(3);
        suffix = " V";
    } else if (obj.id === "fc.alt") {
        value = obj.value.toFixed();
        suffix = " feet";
    }

    if (value === undefined) {
        value = obj.value.toString() + suffix;
    } else {
        value = value.toString() + suffix;
    }

    this.updateRow(obj.id, value, false);
    // Age data
    updateTimeouts[obj.id] = setTimeout(this.updateRow, ageDataTimeout, obj.id, value, true);

    if (obj.id === "tpc.bat_v") {
        if (updateTimeouts["tpc.bat_v*"]) {
            clearTimeout(updateTimeouts["tpc.bat_v*"]);
        }
        if (this.tpcBatHistory === undefined) {
            this.tpcBatHistory = [];
        }
        this.tpcBatHistory.push(obj.value);
        if (this.tpcBatHistory.length > 30) {
            this.tpcBatHistory = this.tpcBatHistory.slice(1);
        }

        value = (this.tpcBatHistory.reduce((acc, v) => acc + v, 0) / this.tpcBatHistory.length).toFixed(3).toString() + " V";
        this.updateRow("tpc.bat_v*", value, false);
        // Age data
        updateTimeouts["tpc.bat_v*"] = setTimeout(this.updateRow, ageDataTimeout, "tpc.bat_v*", value, true);
    } else if (obj.id === "fc.state") {
        // TODO: convert state into a string and then call this.drawFCState(stateStr)
    }
};
Rocket.prototype.updateRow = function(id, valueStr, aged) {
    valueStr = valueStr.padEnd(tableValueMaxLen);
    if (aged) {
        valueStr = chalk.red.strikethrough(valueStr);
    }

    rl.cursorTo(process.stdout, tableValueCol, baseLine + tableRows[id]);
    process.stdout.write(valueStr);
    rl.cursorTo(process.stdout, 0, baseLine + tableRows["$END"]);
};
Rocket.prototype.drawFCState = function(currentState) {
    var currentStateIndex = -1;
    for (var i = 0; i < fcStateDrawInfo.length; i++) {
        if (currentState === fcStateDrawInfo[i].state) {
            currentStateIndex = i;
            break;
        }
    }

    var completedLineText = chalk.green("|");
    var incompleteLineText = "|";
    for (var i = 0; i < fcStateDrawInfo.length; i++) {
        var stateText = "(X) - " + fcStateDrawInfo[i].state;
        if (i < currentStateIndex) {
            stateText = chalk.green(stateText);
        } else if (i == currentStateIndex) {
            stateText = chalk.blueBright(stateText);
        }

        rl.cursorTo(process.stdout, tableWidth + 5, baseLine + fcStateDrawInfo[i].line);
        process.stdout.write(stateText);

        if (i < fcStateDrawInfo.length - 1) {
            // Draw the line in between
            for (var line = fcStateDrawInfo[i].line + 1; line <= fcStateDrawInfo[i+1].line - 1; line++) {
                rl.cursorTo(process.stdout, tableWidth + 6, baseLine + line);
                // Drawing line between i and i+1
                process.stdout.write(i < currentStateIndex ? completedLineText : incompleteLineText);
            }
        }
    }
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

module.exports = function (comPort) {
    return new Rocket(comPort)
};