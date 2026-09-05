#ifndef CDATA_LOADER_H
#define CDATA_LOADER_H

#define MAX_ITEMS 5000

typedef struct {
    char key[128];      // command name
    char val[512];      // description / help text
    char usage[512];    // full usage example / prototype
} CmdObj;

extern CmdObj database[MAX_ITEMS];
extern int db_count;

void load_database(const char *filename);

#endif
