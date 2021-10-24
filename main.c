#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <psapi.h>

#define _CRT_SECURE_NO_WARNINGS

DWORD* listProcesses(DWORD* cbNeeded) {
    DWORD* processes = (DWORD*)malloc(1024 * sizeof(DWORD));
    if (!EnumProcesses(processes, 1024 * sizeof(DWORD), cbNeeded))
        return NULL;

    return processes;
}

void printProcessNameAndID(DWORD* processID) {
    char processName[MAX_PATH];
    strcpy(processName, "<unknown>");
    HANDLE processHandle = OpenProcess( PROCESS_QUERY_INFORMATION |
                                        PROCESS_VM_READ,
                                        FALSE, *processID);
    if (processHandle != NULL){
        HMODULE hModule;
        DWORD cbNeeded;
        if (EnumProcessModules(processHandle, &hModule, sizeof(hModule), &cbNeeded)) {
            GetModuleBaseName(processHandle, hModule, processName, sizeof(processName) / sizeof(char));
        }
    }

    printf("%s  (PID: %lu)\n", processName, *processID);
    CloseHandle(processHandle);
}

int main() {
    DWORD cbNeeded;
    DWORD* processes = listProcesses(&cbNeeded);
    if (processes == NULL)
        return 1;

    DWORD numberProcesses = cbNeeded / sizeof(DWORD);
    for (int i = 0; i < numberProcesses; ++i)
        if (*(processes + i) != 0)
            printProcessNameAndID(processes + i);
    return 0;
}