#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <sqlite3.h>
#define SIZE 1000000

struct Rec {
  int id;         /* unikalny identyfikator, klucz główny */
  char name[20];  /* nazwa */
  char desc[90];  /* opis */
};

typedef struct Rec Rec;

void fillRec(sqlite3* db) {
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
	int i=0;
    for(i=0; i<SIZE; i++) {
		char* sql = sqlite3_mprintf("INSERT INTO inv VALUES(%d,'Name1234','Description12345');",i);
		int rc = sqlite3_exec(db,sql,NULL,NULL,NULL);
		if (rc) {
		    fprintf(stderr, "Insert error: %s\n", sqlite3_errmsg(db));
		}
	}
	int rc = sqlite3_exec(db, "COMMIT TRANSACTION", NULL, NULL, NULL);
	if (rc) {
		fprintf(stderr, "Commit error: %s\n", sqlite3_errmsg(db));
	}
}

void find(sqlite3* db, int id) {
	char * sql;
    sql = sqlite3_mprintf("SELECT * FROM inv WHERE id=%d;",id);
	sqlite3_stmt *stmt;
	int rc=sqlite3_prepare_v2(db,sql,strlen(sql), &stmt, NULL);
	if (rc) {
	fprintf(stderr, "Database prepare statement error: %s\n", sqlite3_errmsg(db));
	}
	do {
		rc=sqlite3_step(stmt);
		if (rc==SQLITE_ROW) {
		  printf("I found elements and id: %s, %s, %d\n",
					 (char *)sqlite3_column_text(stmt,1),
					 (char *)sqlite3_column_text(stmt,2),
					 sqlite3_column_int(stmt,0));
		}
	} while (rc == SQLITE_ROW);
	sqlite3_finalize(stmt);
}
/*
void findAll(sqlite3* db) {
	char * sql;
    sql = sqlite3_mprintf("SELECT * FROM inv");
	sqlite3_stmt *stmt;
	int rc=sqlite3_prepare_v2(db,sql,strlen(sql), &stmt, NULL);
	if (rc) {
	fprintf(stderr, "Database prepare statement error: %s\n", sqlite3_errmsg(db));
	}
	do {
		rc=sqlite3_step(stmt);
		if (rc==SQLITE_ROW) {
		  printf("I found elements and id: %s, %s, %d\n",
					 (char *)sqlite3_column_text(stmt,1),
					 (char *)sqlite3_column_text(stmt,2),
					 sqlite3_column_int(stmt,0));
		}
	} while (rc == SQLITE_ROW);
	sqlite3_finalize(stmt);
}
*/

int main(int argc, char **argv) {

    int rc;
	struct rusage mem;
	clock_t c1, c2, c3, c4;

    sqlite3 *db;
	// char loc[]=":memory:"
    char loc[]="./db.sqlite";
    
    rc=sqlite3_open(loc,&db);
    if (rc){
        fprintf(stderr, "Database create table error: %s\n", sqlite3_errmsg(db));
    return 1;
    }

	rc=sqlite3_exec(db,"CREATE TABLE inv (id integer PRIMARY KEY, name varchar(20), desc varchar(90));",NULL,NULL,NULL);
    if (rc){
         fprintf(stderr, "Database create table error: %s\n", sqlite3_errmsg(db));
    return 1;
    }

	c1 = clock();
	fillRec(db);
	c2 = clock();

    c3 = clock();
	find(db, 999999);
	c4 = clock();


	printf("Inserting time[s]: %f\n", ((float)c2-(float)c1)/CLOCKS_PER_SEC);
    printf("Finding time[s]: %f\n", ((float)c4-(float)c3)/CLOCKS_PER_SEC);

	getrusage(RUSAGE_SELF,&mem);
	printf("Max mem usage[kB]: %ld\n", mem.ru_maxrss);

	sqlite3_close(db);

    return 0;
}
