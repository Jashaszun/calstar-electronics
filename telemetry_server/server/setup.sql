/*
 * Access control: CalSTAR MA
 *
 * Contains statements to set up database configuration and schema. Should always be run
 * before starting the server for the first time.
 */

CREATE USER IF NOT EXISTS 'telemetry'@'localhost'; -- add password later when in prod

CREATE DATABASE IF NOT EXISTS telemetry;

USE telemetry;

GRANT ALL PRIVILEGES ON telemetry.* TO 'telemetry'@'localhost'; -- reduce privileges once schema is set

-- TODO initialize tables

