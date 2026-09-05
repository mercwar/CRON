#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "avis_window.h"
#include "cdata_loader.h"

// Forward declaration
int RunConsole(int guiFlag);

int main(int argc, char *argv[]) {
    // Dynamically fetch and unpack key-value structures from your parsed compilation phase
    load_database("WIN11_OBJECTS.CDATA");

    // FIX: Safely parse array index address pointer boundaries 
    if (argc > 1 && strcmp(argv[1], "--avis-gui") == 0) {
        ShowAvisWindow(GetModuleHandle(NULL));
        return 0;
    }

    return RunConsole(0);
}

int RunConsole(int guiFlag) {
    // FIX: Clear unreferenced parameter compile warning C4100 under strict /W4 rule optimization
    (void)guiFlag;

    char input[256];
    printf("=== AVIS NEON CORE :: COMMAND LINE EDITOR ===\n");
    printf("Loaded %d system command objects.\n\n", db_count);

    while (1) {
        printf("AVIS_CRON > ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\r\n")] = 0;

        if (strcmp(input, "exit") == 0) break;

        // Route directly to GUI view if the standalone 'avis' keyword token is issued
        if (_stricmp(input, "avis") == 0) {
            ShowAvisWindow(GetModuleHandle(NULL));
            continue;
        }

        int found = 0;
        for (int i = 0; i < db_count; i++) {
            if (_stricmp(input, database[i].key) == 0) {
                printf("Executing: [%s]\n", database[i].key);
                
                // Map execution to command keys directly, formatting batch instructions safely
                if (_stricmp(database[i].key, "CALL") == 0 || _stricmp(database[i].key, "GOTO") == 0 || _stricmp(database[i].key, "SET") == 0) {
                    system("call /?");
                } else if (strstr(database[i].val, ".exe") != NULL || strstr(database[i].val, ".EXE") != NULL) {
                    system(database[i].key);
                } else {
                    char helpCmd[128];
                    sprintf_s(helpCmd, sizeof(helpCmd), "%s /?", database[i].key);
                    system(helpCmd);
                }
                
                printf("\n");
                found = 1;
                break;
            }
        }
        
        // Pass-through processing lane for unstructured inputs
        if (!found && strlen(input) > 0) {
            printf("Unknown Meta Core instruction. Pass-through execution...\n");
            system(input);
        }
    }
    return 0;
}
