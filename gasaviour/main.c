#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include <getopt.h>


#ifndef VERSION
    #define VERSION 1.2
#endif // VERSION

/** String comparison
 *
 * \param first     The first string
 * \param second    The...second string? Why am I even commenting this?
 * \return int      1 if equal, otherwise 0
 */
int isEqual(char first[], char second[]) {
    return (strcmp(first,second) == 0);
}

// Function for exporting authentication data directly to a file
/** Export auths to WinAuth format
 *
 * \param dbFile    The database file to read auths from
 * \param destFile  The WinAuth file to save to
 */
void exportAuthsToWinAuth(char dbFile[], char destFile[]) {
    // Open database
    sqlite3 *db;
    sqlite3_open(dbFile, &db);
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(db, "SELECT type, email, secret FROM accounts",-1,&statement,NULL) == SQLITE_OK) {
        int res;
        FILE *fp;
        fp = fopen(destFile,"a");
        // Loop through each authentication
        while ( (res = sqlite3_step(statement)) != SQLITE_DONE) {
            char buff[1024];
            // Format and write to file
            sprintf(buff,"otpauth://%s/%s?secret=%s\n",
                    isEqual(sqlite3_column_blob(statement,0),"0") ? "totp" : "hotp",
                    sqlite3_column_blob(statement,1),
                    sqlite3_column_blob(statement,2));
            fputs(buff,fp);
        }
    }
    sqlite3_close(db);
}

// Print all auths from the provided file
/** Print all auths from dbFile
 *
 * \param dbFile The location of the database to read.
 */
void printAllAuths(char dbFile[]) {
    int res;
    sqlite3 *db;
    sqlite3_open(dbFile, &db);
    sqlite3_stmt *statement;

    if (sqlite3_prepare_v2(db,"SELECT email, secret,type FROM accounts",-1, &statement, NULL) == SQLITE_OK) {
        while ( (res = sqlite3_step(statement)) != SQLITE_DONE) {
            printf("\n\nEmail: %s",sqlite3_column_blob(statement,0));
            printf("\nSecret: %s",sqlite3_column_blob(statement,1));
            printf("\nType: %s",isEqual(sqlite3_column_blob(statement,2),"0") ? "totp" : "hotp");
        }
    }
    sqlite3_close(db);
}

void main(int argc, char *argv[]) {
    printf("GAuthSaviour %g by Michael Cowell (github/compl3x) (@Complex360)",VERSION);
    char * fileName = NULL;
    char * destFileName = NULL;

    int arg;
    while ((arg = getopt(argc, argv, "df:w:")) != -1) {
        switch(arg) {
        case 'f':
            fileName = optarg;
            break;
        case 'w':
            destFileName = optarg;
            break;
        }
    }

    if (destFileName != NULL) {
        printf("\n Converting %s to %s",fileName,destFileName);
        exportAuthsToWinAuth(fileName,destFileName);
    }
    else if (fileName != NULL) {
        printf("\n Printing auths from %s",fileName);
        printAllAuths(fileName);
    }
    else {
        printf("\n\nUsage:\t%s [-f <file>] [-w <file>]",argv[0]);
        printf("\n\n\t-f <file>\tRead database from <file>");
        printf("\n\t-w <file>\tConvert database to Winauth <file>\n");
    }

}
