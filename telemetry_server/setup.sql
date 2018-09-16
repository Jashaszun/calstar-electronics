CREATE USER IF NOT EXISTS 'telemetry'@'localhost'; -- add password later when in prod

CREATE DATABASE IF NOT EXISTS telemetry;

USE telemetry;

GRANT ALL PRIVILEGES ON telemetry.* TO 'telemetry'@'localhost'; -- reduce privileges once schema is set

-- TODO initialize tables

