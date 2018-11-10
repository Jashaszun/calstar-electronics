USE telemetry;

-- Replace email with the email address of the user you want to authorize
UPDATE Users SET authorized=1 WHERE email='rahulkumar@berkeley.edu';