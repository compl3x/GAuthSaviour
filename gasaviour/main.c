#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include "sqlite3.h"

// Wrapper for string comparisons
int isEqual(char first[], char second[]) {
    return (strcmp(first,second) == 0);
}

// Function for exporting authentication data directly to a file
void exportAuthsToWinAuth(char dbFile[], char destFile[]) {
    sqlite3 *db;
    sqlite3_open(dbFile, &db);
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT type, email, secret FROM accounts",-1,&statement,NULL) == SQLITE_OK) {
        int res;
        FILE *fp;
        fp = fopen(destFile,"a");
        // Loop through each row
        while ( (res = sqlite3_step(statement)) != SQLITE_DONE) {
            char buff[1024];
            sprintf(buff,"otpauth://%s/%s?secret=%s\n",
                    isEqual(sqlite3_column_blob(statement,0),"0") ? "totp" : "hotp",
                    sqlite3_column_blob(statement,1),
                    sqlite3_column_blob(statement,2));
            fputs(buff,fp);
        }
    }
    sqlite3_close(db);
}

// Default callback from SQLite documentation - prints all columns of all rows
static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Print all auths from the provided file
/** Print all auths from dbFile
 *
 * \param dbFile The location of the database to read.
 * \return null
 * Will read the email, secret, and type fields from the accounts table,
 * which is all you need in most cases.
 */
void printAllAuths(char dbFile[]) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    sqlite3_open(dbFile, &db);
    rc = sqlite3_exec(db,"SELECT email, secret,type FROM accounts" , callback, 0, &zErrMsg);
    sqlite3_close(db);
}

void main(int argc, char *argv[]) {
    printf("GAuthSaviour 1.0 by Michael Cowell (github:compl3x)");
    char * fileName = NULL;
    char * destFileName = NULL;
    int isExport = 0;
    // Default behaviour
    if (argc == 1) {
        printf("\nRunning in default mode (reading from local file \"databases\")\n\n");
        printAllAuths("databases");
    }
    else {
        // Loop through all arguments
        int counter;
        for (counter = 0; counter < argc; counter++) {
            if (isEqual(argv[counter],"--help")) {
                // Usage information
                printf("\nUsage: %s [-d] [-w filename] [-f filename]",argv[0]);
                printf("\n\n\t-d\t\tDump database via adb");
                printf("\n\t-w <filename>\tConvert database to WinAuth file");
                printf("\n\t-f <filename>\tLoad database from <filename>");
                break;
            }
            else if (isEqual(argv[counter],"-d")) {
                system("adb pull /data/data/com.google.android.apps.authenticator2/databases/databases");
            }
            else if (isEqual(argv[counter],"-w") && (counter+1 <= argc)) {
                isExport = 1;
                destFileName = argv[counter+1];
            }
            else if (isEqual(argv[counter],"-f") && (counter+1 <= argc) ) {
                fileName = argv[counter+1];
            }
        }
    }

    if (isExport) {
        exportAuthsToWinAuth(fileName,destFileName);
    }
    else if (fileName != NULL) {
        printAllAuths(fileName);
    }

}
