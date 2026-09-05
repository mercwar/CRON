#include "avis_window.h"
#include "cdata_loader.h"
#include <windows.h>
#include <stdio.h>
#include <ctype.h>

static HWND hList, hEdit, hButton, hUsageBox;

// Helper function to extract ONLY the structural syntax/prototype line from raw help output
void ParsePrototypeFromOutput(const char* pBuffer, const char* cmdKey, HWND hTargetEdit) {
    char line[1024];
    int bufIdx = 0;
    int lineIdx = 0;

    if (_stricmp(cmdKey, "DISKPART") == 0) {
        SetWindowTextW(hTargetEdit, L"DISKPART [/S script] [/?]");
        return;
    }

    while (pBuffer[bufIdx] != '\0') {
        char c = pBuffer[bufIdx++];
        if (c == '\n' || c == '\r' || lineIdx >= 1023) {
            line[lineIdx] = '\0';
            
            if (lineIdx > 0) {
                char* pStart = line;
                while (*pStart == ' ' || *pStart == '\t') pStart++;

                int keyLen = (int)strlen(cmdKey);
                if (_strnicmp(pStart, cmdKey, keyLen) == 0) {
                    char nextChar = pStart[keyLen];
                    if (nextChar == ' ' || nextChar == '\0' || nextChar == '[' || nextChar == '/') {
                        int wLen = MultiByteToWideChar(CP_OEMCP, 0, pStart, -1, NULL, 0);
                        wchar_t* wBuffer = (wchar_t*)malloc(wLen * sizeof(wchar_t));
                        if (wBuffer) {
                            MultiByteToWideChar(CP_OEMCP, 0, pStart, -1, wBuffer, wLen);
                            SetWindowTextW(hTargetEdit, wBuffer);
                            free(wBuffer);
                            return;
                        }
                    }
                }
            }
            lineIdx = 0;
            if (c == '\r' && pBuffer[bufIdx] == '\n') bufIdx++;
        } else {
            line[lineIdx++] = c;
        }
    }

    wchar_t wFallback[256];
    swprintf_s(wFallback, 256, L"%hs ", cmdKey);
    SetWindowTextW(hTargetEdit, wFallback);
}

// Executes a command invisibly and captures output without hanging on interactive consoles
void CaptureCommandOutput(const char* cmd, const char* cmdKey, HWND hTargetEdit, BOOL extractPrototypeOnly) {
    HANDLE hChildStd_OUT_Rd = NULL;
    HANDLE hChildStd_OUT_Wr = NULL;
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0)) return;
    if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0)) return;

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOA siStartInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&siStartInfo, sizeof(STARTUPINFOA));

    siStartInfo.cb = sizeof(STARTUPINFOA);
    siStartInfo.hStdError = hChildStd_OUT_Wr;
    siStartInfo.hStdOutput = hChildStd_OUT_Wr;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    siStartInfo.wShowWindow = SW_HIDE;

    char szCmdLine[512];
    
    if (_stricmp(cmdKey, "DISKPART") == 0) {
        sprintf_s(szCmdLine, sizeof(szCmdLine), "cmd.exe /c echo help | diskpart");
    } else {
        sprintf_s(szCmdLine, sizeof(szCmdLine), "cmd.exe /c %s", cmd);
    }

    BOOL bSuccess = CreateProcessA(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo);
    CloseHandle(hChildStd_OUT_Wr);

    if (bSuccess) {
        DWORD dwRead;
        char* pBuffer = (char*)malloc(131072);
        DWORD dwTotalBytes = 0;

        if (pBuffer) {
            pBuffer[0] = '\0';
            while (ReadFile(hChildStd_OUT_Rd, pBuffer + dwTotalBytes, 131071 - dwTotalBytes, &dwRead, NULL) && dwRead > 0) {
                dwTotalBytes += dwRead;
            }
            pBuffer[dwTotalBytes] = '\0';

            if (extractPrototypeOnly) {
                ParsePrototypeFromOutput(pBuffer, cmdKey, hTargetEdit);
            } else {
                int wLen = MultiByteToWideChar(CP_OEMCP, 0, pBuffer, -1, NULL, 0);
                wchar_t* wBuffer = (wchar_t*)malloc(wLen * sizeof(wchar_t));
                if (wBuffer) {
                    MultiByteToWideChar(CP_OEMCP, 0, pBuffer, -1, wBuffer, wLen);
                    SetWindowTextW(hTargetEdit, wBuffer);
                    free(wBuffer);
                }
            }
            free(pBuffer);
        }
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
    }
    CloseHandle(hChildStd_OUT_Rd);
}

LRESULT CALLBACK AvisWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hList = CreateWindowW(L"LISTBOX", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            10, 10, 200, 425, hWnd, (HMENU)1, NULL, NULL);

        hUsageBox = CreateWindowW(L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_READONLY,
            220, 10, 400, 350, hWnd, (HMENU)4, NULL, NULL);

        hEdit = CreateWindowW(L"EDIT", NULL,
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            220, 375, 310, 25, hWnd, (HMENU)2, NULL, NULL);

        hButton = CreateWindowW(L"BUTTON", L"Send",
            WS_CHILD | WS_VISIBLE,
            540, 375, 80, 25, hWnd, (HMENU)3, NULL, NULL);

        for (int i = 0; i < db_count; i++) {
            wchar_t wbuf[256];
            MultiByteToWideChar(CP_UTF8, 0, database[i].key, -1, wbuf, 256);
            SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)wbuf);
        }
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1 && HIWORD(wParam) == LBN_SELCHANGE) {
            int sel = (int)SendMessageW(hList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) {
                char cmdRun[512];
                sprintf_s(cmdRun, sizeof(cmdRun), "%s /?", database[sel].key);
                
                CaptureCommandOutput(cmdRun, database[sel].key, hEdit, TRUE);
                
                SetFocus(hEdit);
                SendMessageW(hEdit, EM_SETSEL, 0, 0);
                SendMessageW(hEdit, EM_SCROLLCARET, 0, 0);
            }
        }

        if (LOWORD(wParam) == 3) {
            wchar_t buf[256];
            char mb[256];
            GetWindowTextW(hEdit, buf, 256);
            
            WideCharToMultiByte(CP_ACP, 0, buf, -1, mb, sizeof(mb), NULL, NULL);
            system(mb);
        }
        break;

    case WM_CONTEXTMENU:
        if ((HWND)wParam == hList) {
            POINT pt;
            pt.x = (int)(short)LOWORD(lParam);
            pt.y = (int)(short)HIWORD(lParam);
            
            ScreenToClient(hList, &pt);
            int sel = (int)SendMessageW(hList, LB_ITEMFROMPOINT, 0, MAKELPARAM(pt.x, pt.y));
            
            if (HIWORD(sel) == 0) { 
                int idx = LOWORD(sel);
                SendMessageW(hList, LB_SETCURSEL, idx, 0);
                
                char cmdRun[512];
                sprintf_s(cmdRun, sizeof(cmdRun), "%s /?", database[idx].key);
                
                CaptureCommandOutput(cmdRun, database[idx].key, hUsageBox, FALSE);
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

void ShowAvisWindow(HINSTANCE hInst) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = AvisWndProc;
    wc.hInstance = hInst;
    // UPDATED: Dynamically bind 'favi-con.ico' using precompiled resource ID 1
    wc.hIcon = LoadIconW(hInst, MAKEINTRESOURCEW(1));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"CRON_Window";
    RegisterClassW(&wc);

    // UPDATED: Standardized window title identifier name to CRON
    HWND hWnd = CreateWindowW(L"CRON_Window", L"CRON",
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 645, 475,
        NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}
