#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include <getopt.h>
#include <stdlib.h>

#ifndef VERSION
    #define VERSION "1.3"
#endif // VERSION

void extractAuths(char dbFile[], char destFile[]) {
	// Declare sql objects
	sqlite3 *db;
    sqlite3_open(dbFile, &db);
    sqlite3_stmt *statement;
	
	int export = (destFile != NULL);
	FILE *fp;
	
	if (export) {
		fp = fopen(destFile,"a+");
	}

	// Loop through 'accounts'
	if (sqlite3_prepare_v2(db, "SELECT type, email, secret FROM accounts",-1,&statement,NULL) == SQLITE_OK) {
		int res;
		while ( (res = sqlite3_step(statement)) != SQLITE_DONE) {
			if (export) {
				char buffer[256];
				sprintf(buffer,"otpauth://%s/%s?secret=%s\n",
						atoi(sqlite3_column_blob(statement,0)) == 0 ? "totp" : "hotp",
						sqlite3_column_blob(statement,1),
						sqlite3_column_blob(statement,2));
				fputs(buffer, fp);
			}
			else {
				printf("\n\nType: %s",atoi(sqlite3_column_blob(statement,0)) == 0 ? "totp" : "hotp");
				printf("\nEmail: %s",sqlite3_column_blob(statement,1));
				printf("\nSecret: %s",sqlite3_column_blob(statement,2));
			}
		}
	}
	
	// Gracefully close file and free pointer
	if (export) {
		fclose(fp);
		free(fp);
		printf("\nExported to: %s", destFile);
	}
	
	sqlite3_close(db);
}

void main(int argc, char *argv[]) {
    printf("GAuthSaviour %s by Michael Cowell (github/compl3x) (@Complex360)",VERSION);
    char * fileName = NULL;
    char * destFileName = NULL;
    int arg;
	
    while ((arg = getopt(argc, argv, "df:w:d")) != -1) {
        switch(arg) {
        case 'f':
            fileName = optarg;
            break;
        case 'w':
            destFileName = optarg;
            break;
        case 'd':
            // TODO: This currently doesn't check if the file exists
            printf("\n\n");
            system("adb pull /data/data/com.google.android.apps.authenticator2/databases/databases");
            fileName = "databases";
            break;
        }
    }

    if (fileName != NULL) {
		extractAuths(fileName, destFileName);
	}
    else {
        printf("\n\nUsage:\t%s [-d] [-f <file>] [-w <file>]",argv[0]);
        printf("\n\n\t-d\t\tDump database via adb (must have adb in directory of PATH)");
        printf("\n\t-f <file>\tRead database from <file>");
        printf("\n\t-w <file>\tConvert database to Winauth/plain text <file>\n");
    }

}
