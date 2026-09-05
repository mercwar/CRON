#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "cdata_loader.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

CmdObj database[MAX_ITEMS];
int db_count = 0;

void load_database(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[CRON] ERROR: Cannot open database metadata target file: %s\n", filename);
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "[objects]", 9) == 0) continue;
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;

        char *eq = strchr(line, '=');
        if (eq) {
            *eq = '\0';
            char *pipe = strchr(eq + 1, '|');
            if (pipe) {
                *pipe = '\0';
                strncpy_s(database[db_count].key, sizeof(database[db_count].key), line, _TRUNCATE);
                strncpy_s(database[db_count].val, sizeof(database[db_count].val), eq + 1, _TRUNCATE);
                strncpy_s(database[db_count].usage, sizeof(database[db_count].usage), pipe + 1, _TRUNCATE);
            } else {
                strncpy_s(database[db_count].key, sizeof(database[db_count].key), line, _TRUNCATE);
                strncpy_s(database[db_count].val, sizeof(database[db_count].val), eq + 1, _TRUNCATE);
                database[db_count].usage[0] = '\0';
            }
        } else {
            strncpy_s(database[db_count].key, sizeof(database[db_count].key), line, _TRUNCATE);
            database[db_count].val[0] = '\0';
            database[db_count].usage[0] = '\0';
        }
        db_count++;
        if (db_count >= MAX_ITEMS) break;
    }
    fclose(fp);
}
