#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "avis_window.h"
#include "cdata_loader.h"

// Forward Declarations
int RunConsole(int guiFlag);
void DispatchCommandNonBlocking(const char* cmd);

// Global tracking Job Object shared handle
HANDLE hGlobalJobObject = NULL;

// Subsystem Windows Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    // 1. Initialize Global Job Object constraints configuration up-front
    hGlobalJobObject = CreateJobObjectA(NULL, NULL);
    if (hGlobalJobObject != NULL) {
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        SetInformationJobObject(hGlobalJobObject, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
    }

    // 2. Load initialized CDATA cache table node
    load_database("WIN11_OBJECTS.CDATA");

    // 3. Evaluate runtime startup flag arguments natively using Win32 API
    int argc;
    LPWSTR* argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    BOOL launchGui = FALSE;

    if (argvW != NULL) {
        for (int i = 1; i < argc; i++) {
            if (wcscmp(argvW[i], L"--avis-gui") == 0) {
                launchGui = TRUE;
                break;
            }
        }
        LocalFree(argvW);
    }

    if (launchGui) {
        // Pure Windowed Mode: Show the UI window directly without initializing a console channel
        ShowAvisWindow(hInstance);
        
        // Clean up tracking tables right before app teardown finishes
        if (hGlobalJobObject != NULL) CloseHandle(hGlobalJobObject);
        return 0;
    }

    // 4. Console Mode: Manually provision a console context block for the session
    if (AllocConsole()) {
        // Redirect standard I/O streams back to the newly attached console buffer safely
        FILE* fpDummy;
        freopen_s(&fpDummy, "CONIN$", "r", stdin);
        freopen_s(&fpDummy, "CONOUT$", "w", stdout);
        freopen_s(&fpDummy, "CONOUT$", "w", stderr);
    }

    int exitCode = RunConsole(0);

    if (hGlobalJobObject != NULL) CloseHandle(hGlobalJobObject);
    return exitCode;
}

// Global safe asynchronous execution wrapper function
void DispatchCommandNonBlocking(const char* cmd) {
    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFOA));
    si.cb = sizeof(STARTUPINFOA);
    si.dwFlags |= STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_SHOWNORMAL;

    char commandLine[1024];
    DWORD creationFlags = CREATE_NEW_CONSOLE;

    // FIX: Check if the string is executing a raw executable direct path or tool name
    // If it maps straight to a file allocation context, we drop cmd.exe entirely and suppress windows
    if (strstr(cmd, ".exe") != NULL || strstr(cmd, ".EXE") != NULL || strchr(cmd, ' ') == NULL) {
        sprintf_s(commandLine, sizeof(commandLine), "%s", cmd);
        creationFlags = CREATE_NO_WINDOW; // Completely blocks window frame rendering context
    } else {
        // Fallback for custom script macros, internal pipelines, or raw compound commands
        sprintf_s(commandLine, sizeof(commandLine), "cmd.exe /c %s", cmd);
        creationFlags = CREATE_NEW_CONSOLE;
    }

    // Spawn detached process allocation lane
    BOOL bSuccess = CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, 
                                   creationFlags, NULL, NULL, &si, &pi);
    
    if (bSuccess) {
        if (hGlobalJobObject != NULL) {
            AssignProcessToJobObject(hGlobalJobObject, pi.hProcess);
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

int RunConsole(int guiFlag) {
    (void)guiFlag;
    char input[256];

    printf("=== AVIS NEON CORE :: COMMAND LINE EDITOR ===\n");
    printf("Loaded %d system command objects.\n\n", db_count);

    while (1) {
        printf("AVIS_CRON > ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\r\n")] = 0;

        if (strcmp(input, "exit") == 0) break;

        if (_stricmp(input, "avis") == 0) {
            ShowAvisWindow(GetModuleHandle(NULL));
            continue;
        }

        int found = 0;
        for (int i = 0; i < db_count; i++) {
            if (_stricmp(input, database[i].key) == 0) {
                printf("Executing non-blocking task track: [%s]\n", database[i].key);
                
                if (_stricmp(database[i].key, "CALL") == 0 || _stricmp(database[i].key, "GOTO") == 0 || _stricmp(database[i].key, "SET") == 0) {
                    DispatchCommandNonBlocking("call /?");
                } else if (strstr(database[i].val, ".exe") != NULL || strstr(database[i].val, ".EXE") != NULL) {
                    DispatchCommandNonBlocking(database[i].key);
                } else {
                    char helpCmd[256];
                    sprintf_s(helpCmd, sizeof(helpCmd), "%s /?", database[i].key);
                    DispatchCommandNonBlocking(helpCmd);
                }
                
                printf("\n");
                found = 1;
                break;
            }
        }
        
        if (!found && strlen(input) > 0) {
            printf("Unknown Meta Core instruction. Pass-through execution...\n");
            DispatchCommandNonBlocking(input);
        }
    }
    return 0;
}
