/**
 * Access control: CalSTAR MA
 *
 * Handles POST requests to the /upload endpoint.
 * Exports a function (request, response) that returns void, to be used
 * as express middleware.
 *
 * This API endpoint handles uploads of raw test files; for now, these
 * files can be .wbpz (ANSYS files used by sims) or .csv; we only need
 * to worry about parsing .csv files. The .csv files should be parsed,
 * and their data uploaded into the SQL database as appropriate.
 *
 * For further details, refer to the design doc (linked in the README).
 */

const db = require('./db-interface')
const parse = require('csv-parse/lib/sync')

// YOUR CODE HERE
var postUpload = function (req, res) {
    const rundata = parse(req.files.runfile.data, {
        columns: true,
        skip_empty_lines: true
    })
    res.send(rundata);

    // TODO: Read from a form to see if new run should be created
    newRun = true;
    runId = 1;
    dataTypeId = 0;

    if (newRun) {
        // Create a new run
        db.pool.execute(
            'INSERT INTO Runs (runId) VALUES (DEFAULT)', [],
            function (err, results, fields) {
                if (err) {
                    console.log(err);
                }

                console.log(results);
                console.log(fields);

                runId = results.insertId;

                // yucky code duplication
                rundata.forEach(data => {
                    // insert data points into database
                    db.pool.execute(
                        'INSERT INTO DataPoint ( dataIndex, runId, value ) VALUES ( 0, ?, ?)',
                        [runId, data["Time"]],      // Time is hardcoded for now - need to check if CSVs will have headers
                        function (err, results, fields) {
                            if (err) {
                                console.log(err);
                            }

                            console.log(results);
                            console.log(fields);
                        }
                    );
                });
            }
        )
    } else {
        rundata.forEach(data => {
            // insert data point into database
            db.pool.execute(
                'INSERT INTO DataPoint ( dataIndex, runId, value ) VALUES ( 0, ?, ?)',
                [runId, data["Time"]],
                function (err, results, fields) {
                    if (err) {
                        console.log(err);
                    }

                    console.log(results);
                    console.log(fields);
                }
            );
        });
    }

}

module.exports = postUpload // rename if you want
