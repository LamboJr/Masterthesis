#include <sqlite3.h>

void DatabaseInit();
int DataBaseInsertBuffer(const char *Tablename,u16* buffer);
int DataBaseReadBuffer(u16 *buffer, const char *Tablename);
int create_table(sqlite3 *db, const char *Tablename);
int print_table_names() ;
void*  DataBaseInsertThread(void * arg);
int validDataBaseEntry(char *CompareString);
int DataBaseDropTable(char* Tablename);
