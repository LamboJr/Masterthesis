#include <stdio.h>
#include <pthread.h>

//#include <sqlite3.h>
/*
int main_Database() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Open database, create it if it doesn't exist
    rc = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // SQL statement to create a table
    const char *sql = "CREATE TABLE IF NOT EXISTS Pokemon ("
                      "id INTEGER PRIMARY KEY,"
                      "name TEXT NOT NULL,"
                      "Level INTEGER);";

    // Execute SQL statement
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }

    // Close database
    sqlite3_close(db);

    return 0;
}
*/