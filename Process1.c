#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define MAXLENGTH 128

DWORD GetProcessID(char* process) {
    PROCESSENTRY32 processEntry;
    DWORD processID = 0;
    HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Snapshot != INVALID_HANDLE_VALUE) {
        processEntry.dwSize = sizeof(processEntry);
        Process32First(Snapshot, &processEntry);

        do {
            if (strcmp(processEntry.szExeFile, process) == 0) {
                processID = processEntry.th32ProcessID;
                break;
            }
        } while (Process32Next(Snapshot, &processEntry));

        CloseHandle(Snapshot);
    }

    return processID;
}

int main(int argc, char* argv[])
{
    LPCTSTR pipename = "\\\\.\\pipe\\myPipe";
    HANDLE hProcess2;
    LPVOID szAddress;
    DWORD cbWritten, cbRead, dBuf;
    DWORD string_size = sizeof(char) * MAXLENGTH;
    char string[MAXLENGTH];
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    printf("Enter your string: \n");
    fgets(string, sizeof(string), stdin);

    printf("Opening Process2...\n");
    hProcess2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessID("Process2.exe"));
    if (hProcess2 == INVALID_HANDLE_VALUE) {
        printf("Invalid handle of second process\n");
        return 1;
    }

    printf("Success!\nAllocating memory for Process2...\n");
    szAddress = VirtualAllocEx(hProcess2, 0, string_size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    printf("Success!\nWriting your string in Process2 memory...\n");
    if (!WriteProcessMemory(hProcess2, szAddress, string, string_size, 0)) {
        printf("Unable to write string to the second process\n");
        return 2;
    }

    printf("Success!\nConnecting to named pipe...\n");
    while (hPipe == INVALID_HANDLE_VALUE)
        hPipe = CreateFile(pipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    printf("Success!\nWriting memory address...\n");
    dBuf = (DWORD)szAddress;
    if (!WriteFile(hPipe, &dBuf, sizeof(dBuf), &cbWritten, NULL)) {
        printf("Unable to write to the memory of the second process!\n");
        return 3;
    }

    printf("Success!\nWaiting answer from Process2...\n");
    if (!ReadFile(hPipe, &szAddress, sizeof(szAddress), &cbRead, NULL)) {
        printf("Unable to get answer from the second process!\n");
        return 4;
    }

    printf("Success!\nFreeing Process2 memory...\n");
    if (!VirtualFreeEx(hProcess2, szAddress, 0, MEM_RELEASE)) {
        printf("Unable to free memory !\n");
        return 5;
    }

    printf("Success!\nClosing named pipe...\n");
    if (!CloseHandle(hPipe)) {
        printf("Unable to close pipe!\n");
        return 6;
    }

    printf("Success!\nClosing Process2...\n");
    if (!CloseHandle(hProcess2)) {
        printf("Unable to close the second process!!!\n");
        return 7;
    }

    printf("Success!\nProcess1 was finished...\n");
    return 0;
}