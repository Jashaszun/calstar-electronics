[Design doc](https://docs.google.com/document/d/119mO6KgK3HY6Tx_hRBTXcs0BSQwWCaGpI0whlhqaW74/edit)
(server to be temporarily hosted here until the calstar org gets private repos)

The instructions below for installing assume you have some familiarity with nodejs: if you don't, talk to Jonathan and he'll get you up to speed.

## Setting up
1. Install nodejs and npm (look this up if you don't know how)
2. Run `npm install`
3. Install [MariaDB](https://mariadb.org/download/) (this is the SQL server the OCF uses, and is compatible with MySQL libraries)
4. Run `mysql -uroot < setup.sql`; this initializes the database with a user and stuff.

## Running
1. <b>Start the SQL server</b> run `mysql.server start`, or whatever the docs tell you to do.
2. <b>Start the webserver</b> To start the webserver locally, run `npm start`. If you make any changes to server files, you'll need to ctrl-C and restart the server for the changes to take place. Make sure the SQL server is running.
3. To have client-side files update as you make changes, run `npm run watch`.
4. Visit `localhost:<port number>` in your web browser. The port number is 8000 for now.

## Style
### general
Make sure that you'll be able to understand what your code does 3 weeks from now, and make sure other people will be able to understand your code. To that end, please comment things as much as possible.
### js
Please adhere to the [standardjs](https://standardjs.com/) conventions: (tl;dr 2 space indents, single quoted strings, no semicolons—if you feel really strongly about anything, please inform me as such and we can compromise). This is just for consistency and readability, and because I don't want to configure eslint.
Imports (from `require`) should be declared as `const`.
Feel free to rename the variables I gave in the skeleton code. Just make sure the variable names make sense are at least somewhat descriptive.
### sql
Capitalize keywords and lowercase names, e.g. `SELECT points FROM data`. For longer statements, try to put clauses like `FROM` and `WHERE` on their own lines for readability.

If the schema of a table exists and you want to change it, remember to change it in `setup.sql` with `ALTER TABLE IF EXISTS`.

USE PREPARED STATEMENTS (as opposed to string concatenating parameters into a query). They help avoid SQL injection attacks, and also undefined behavior.

### html
Please 2 space indent. I don't really have much else to say.

## Library docs
The following documentation pages may be useful:
- [loggy](https://www.npmjs.com/package/loggy)
- [express](https://expressjs.com/en/4x/api.html)
- [mysql2](https://www.npmjs.com/package/mysql2)
If there's any other libraries I forgot to put down here, Google is your friend.

## TODO
(jonathan will assign these in a bit, stay tuned)
