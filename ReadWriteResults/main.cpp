#include "main.h"
#include <windows.h>
#include <stdio.h>
#include <time.h>

HANDLE file;
BOOL rez;
DWORD b_written, b_read;

void DLL_EXPORT ReadData(HWND hwnd, char* buffer, DWORD bufferSize){
    HANDLE file = CreateFile("results.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE){
        DWORD b_read = 0;
        BOOL rez = ReadFile(file, buffer, bufferSize - 1, &b_read, NULL);
        if (rez){
            buffer[b_read] = '\0';
        } else {
            MessageBox(hwnd, "Text wasn't read", "Read", MB_OK);
        }
        CloseHandle(file);
    } else {
        MessageBox(hwnd, "Failed to open file", "Read", MB_OK);
    }
}

void DLL_EXPORT WriteDataInNewLine(HWND hwnd, char* data) {
    HANDLE file = CreateFile("results.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file != INVALID_HANDLE_VALUE) {
        DWORD b_written;
        BOOL rez;

        DWORD dwPos = SetFilePointer(file, 0, NULL, FILE_END);

        if (dwPos > 0) {
            rez = WriteFile(file, "\r\n", 2, &b_written, NULL);
            if (!rez) {
                MessageBox(hwnd, "Failed to write newline to file", "Error", MB_OK);
            }
        }

        rez = WriteFile(file, data, strlen(data), &b_written, NULL);
        if (!rez) {
            MessageBox(hwnd, "Failed to write data to file", "Write", MB_OK);
        }

        CloseHandle(file);
    } else {
        MessageBox(hwnd, "Failed to open file", "Error", MB_OK);
    }
}






extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            // detach from process
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
