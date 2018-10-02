/*
 * Access control: CalSTAR MA
 *
 * Contains statements to set up database configuration and schema. Should always be run
 * before starting the server for the first time.
 */

CREATE USER IF NOT EXISTS 'telemetry'@'localhost'; -- add password later when in prod

DROP DATABASE IF EXISTS telemetry;

CREATE DATABASE IF NOT EXISTS telemetry;

USE telemetry;

GRANT ALL PRIVILEGES ON telemetry.* TO 'telemetry'@'localhost'; -- reduce privileges once schema is set

-- TODO initialize tables
CREATE TABLE Runs (
	runId SERIAL,
	PRIMARY KEY (runId)
);

CREATE TABLE DataType (
	dataTypeId SERIAL,
	type VARCHAR(255),
	name VARCHAR(255),
	units VARCHAR(255),
	PRIMARY KEY (dataTypeId)
);

CREATE TABLE DataPoint (
	dataIndex int,
	runId BIGINT UNSIGNED NOT NULL,
	value double,
	PRIMARY KEY (dataIndex),
	FOREIGN KEY (runId) REFERENCES Runs(runId)
);

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'Time', 'Time', 'seconds' );

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'Altitude', 'Altitude', 'feet' );

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'Temperature', 'Temperature', 'F' );

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'GyroX', 'GyroX', 'deg/sec' );

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'GyroY', 'GyroY', 'deg/sec' );

INSERT INTO DataType ( type, name, units ) 
VALUES ( 'GyroZ', 'GyroZ', 'deg/sec' );

INSERT INTO Runs (runId) VALUES (DEFAULT);