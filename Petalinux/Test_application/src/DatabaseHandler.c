#include <stdio.h>
#include "Includes/DataTypes.h"
#include <string.h>
#include <sqlite3.h>
#include "DatabaseHandler.h"
#include <pthread.h>
#include "Includes/Link.h"
#include "Includes/structs.h"
#include "Includes/decode_Pokemon_structure.h"

#define BUFFER_SIZE 50

void DatabaseInit(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int ret;

    // Open database, create it if it doesn't exist
    ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);

    if (ret) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    } else {
        fprintf(stderr, "Opened database successfully\n");
    }
    const char *PokemonName = "Brutalanda";
    extern u16 Salamence_pokemonbuffer[50];

    create_table(db, PokemonName);

    DataBaseInsertBuffer(PokemonName,Salamence_pokemonbuffer);

    u16 ReadDBBuffer[50];

    DataBaseReadBuffer(ReadDBBuffer, PokemonName);

    // Close database
    sqlite3_close(db);

    return;
}



int DataBaseInsertBuffer(const char *Tablename,u16* buffer){

    // Open SQLite database or create if not exists
	sqlite3 *db;
	int ret;
	// Open database, create it if it doesn't exist
	ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);
	if (ret) {
	   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	   return 1;
	} else {
	   fprintf(stderr, "Opened database successfully\n");
	}
	create_table(db, Tablename);
    int rc;
    char sql_insert[100];

    snprintf(sql_insert, sizeof(sql_insert), "INSERT INTO %s (value) VALUES (?)", Tablename);
    // Insert buffer data into SQLite database
    //char *sql_insert = "INSERT INTO buffer_data (value) VALUES (?)";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Bind and insert data into the database
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        sqlite3_bind_int(stmt, 1, buffer[i]);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return 1;
        }
        sqlite3_reset(stmt);
    }
    // Finalize and close database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    printf("Data inserted successfully\n");
    return 0;
}

void*  DataBaseInsertThread(void * arg){
	ThreadData *Tdata= (ThreadData *)arg;
	u16 Index = 1;
	printf("The following Pokemon is stored in the Database :\n");
	struct Pokemon StoredPokemon = PrintDecodedPokemonData(Tdata->ArgBufferPtr);
	char TableName[100];
	snprintf(TableName, sizeof(TableName), "%.10s_%d",StoredPokemon.nickname,StoredPokemon.personality);
	while ( validDataBaseEntry(TableName) == 0){
		snprintf(TableName, sizeof(TableName), "%.10s_%d_%d",StoredPokemon.nickname,StoredPokemon.personality,Index++);
	}
	DataBaseInsertBuffer(TableName,Tdata->ArgBufferPtr);
	char DroppedPokemon[100];
	printf("The follwing Pokemon is removed from the Database \n");
	struct Pokemon DroppedPokemonStruct = PrintDecodedPokemonData(Tdata->ArgBufferPtr2);
	snprintf(DroppedPokemon,sizeof(DroppedPokemon),"%.10s_%d",DroppedPokemonStruct.nickname,DroppedPokemonStruct.personality);

	DataBaseDropTable(DroppedPokemon);
	return NULL;

}

int DataBaseReadBuffer(u16 *buffer, const char *Tablename){
	sqlite3 *db;
	int ret;
	// Open database, create it if it doesn't exist
	ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);
	if (ret) {
	   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	   return 1;
	} else {
	   fprintf(stderr, "Opened database successfully\n");
	}
	create_table(db, Tablename);
    // Prepare SQL statement to select data
    char sql_select[100];
    snprintf(sql_select, sizeof(sql_select), "SELECT value FROM %s", Tablename);
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    int index = 0;

    // Retrieve data and store in buffer
    while (sqlite3_step(stmt) == SQLITE_ROW && index < BUFFER_SIZE) {
        buffer[index++] = sqlite3_column_int(stmt, 0);
    }
    // Finalize statement and close database connection
    sqlite3_finalize(stmt);

    // Display retrieved data
    printf("Retrieved data from database:\n");
//    for (int i = 0; i < index; ++i) {
//        printf("%04x ", buffer[i]);
//    }
    printf("\n");
    sqlite3_close(db);
    return 0;
}

int DataBaseDropTable(char* Tablename){
	char sql_drop_table[100];
	 char *err_msg = 0;
	snprintf(sql_drop_table,sizeof(sql_drop_table),"DROP TABLE %s",Tablename);
	sqlite3 *db;
	int ret;
	// Open database, create it if it doesn't exist
	ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);
	if (ret) {
	   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	   return 1;
	} else {
	   fprintf(stderr, "Opened database successfully\n");
	}
	int rc = sqlite3_exec(db, sql_drop_table, 0, 0, &err_msg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", err_msg);
		sqlite3_free(err_msg);
		return 1;
	}

	printf("Table '%s' dropped successfully\n", Tablename);
	return 0;

}

int create_table(sqlite3 *db, const char *Tablename) {
    char *err_msg = 0;

    // Prepare SQL statement to create a new table
    char sql_create_table[100];
    snprintf(sql_create_table, sizeof(sql_create_table), "CREATE TABLE IF NOT EXISTS %s (id INTEGER PRIMARY KEY, value INTEGER)", Tablename);
    int rc = sqlite3_exec(db, sql_create_table, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 1;
    }

    printf("Table '%s' created successfully\n", Tablename);
    return 0;
}

int print_table_names(){
	sqlite3 *db;
		int ret;
		// Open database, create it if it doesn't exist
		ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);
		if (ret) {
		   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		   return 1;
		} else {
		   fprintf(stderr, "Opened database successfully\n");
		}
    sqlite3_stmt *stmt;
    const char *sql = "SELECT name FROM sqlite_master WHERE type='table'";

    // Prepare SQL statement
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Execute the statement and print table names
    printf("Pokemon BackUps:\n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *table_name = sqlite3_column_text(stmt, 0);
        printf("%s\n", table_name);
    }

    // Finalize statement
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}

int validDataBaseEntry(char *CompareString){
	sqlite3 *db;
			int ret;
			// Open database, create it if it doesn't exist
			ret = sqlite3_open("/home/petalinux/database/Pokemon.db", &db);
			if (ret) {
			   fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			   return 1;
			} else {
			   fprintf(stderr, "Opened database successfully\n");
			}
	    sqlite3_stmt *stmt;
	    const char *sql = "SELECT name FROM sqlite_master WHERE type='table'";

	    // Prepare SQL statement
	    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
	    if (rc != SQLITE_OK) {
	        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
	        return 1;
	    }
	    int Check = 0;
	    // Execute the statement and print table names
	    //printf("Table Names:\n");
	    while (sqlite3_step(stmt) == SQLITE_ROW) {
	        char *table_name = sqlite3_column_text(stmt, 0);
	        if(strcmp(table_name,CompareString) == 0){
	        	printf("Matching Name -> %s\n",table_name);
	        	Check = 1;
	        }
	    }

	    // Finalize statement
	    sqlite3_finalize(stmt);
	    sqlite3_close(db);
	    if (Check == 1){

	    	return 0;
	    }else{
	    	printf("No matching Name\n");
	    	return 1;
	    }

}

