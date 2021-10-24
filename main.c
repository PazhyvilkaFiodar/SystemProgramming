#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <string.h>

BOOL ListProcessModules(DWORD dwPID, int* count_modules){
    HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
    MODULEENTRY32 me32;
    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
    if(hModuleSnap == INVALID_HANDLE_VALUE) {
        *count_modules = -1;
        return FALSE;
    }

    me32.dwSize = sizeof(MODULEENTRY32);

    if(!Module32First(hModuleSnap, &me32))
    {
        *count_modules = -1;
        CloseHandle(hModuleSnap);
        return FALSE;
    }

    *count_modules = 0;
    do
    {
        ++(*count_modules);
        printf("\n\n     MODULE NAME:     %s", me32.szModule);
        printf("\n     Executable     = %s", me32.szExePath);
        printf("\n     Base size      = %lu", me32.modBaseSize);

    } while (Module32Next(hModuleSnap, &me32));
    CloseHandle(hModuleSnap);
    return TRUE;
}

BOOL ListProcessThreads( DWORD dwOwnerPID )
{
    HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
    THREADENTRY32 te32;

    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if(hThreadSnap == INVALID_HANDLE_VALUE)
        return FALSE;
    te32.dwSize = sizeof(THREADENTRY32);

    if(!Thread32First( hThreadSnap, &te32))
    {
        CloseHandle(hThreadSnap);
        return FALSE;
    }

    do
    {
        if(te32.th32OwnerProcessID == dwOwnerPID)
        {
            printf("\n\n     THREAD ID      = 0x%08lX", te32.th32ThreadID);
            printf("\n     Base priority  = %ld", te32.tpBasePri);
        }
    } while(Thread32Next(hThreadSnap, &te32));

    CloseHandle(hThreadSnap);
    return TRUE;
}

BOOL GetProcessList( )
{
    HANDLE hProcessSnap;


    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return FALSE;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if(!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return FALSE;
    }

    DWORD dwPriorityClass;
    HANDLE hProcess;

    do
    {
        printf("\n\n=====================================================");
        printf("\nPROCESS NAME:  %s", pe32.szExeFile);
        printf("\n-------------------------------------------------------");

        dwPriorityClass = 0;
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
        if( hProcess != NULL )
        {
            dwPriorityClass = GetPriorityClass(hProcess);
            CloseHandle(hProcess);
        }

        printf("\n  Process ID        = 0x%08lX", pe32.th32ProcessID);
        printf("\n  Thread count      = %lu",   pe32.cntThreads);
        printf("\n  Parent process ID = 0x%08lX", pe32.th32ParentProcessID);
        printf("\n  Base priority     = %lu", pe32.pcPriClassBase);
        if(dwPriorityClass)
            printf("\n  Priority class    = %lu", dwPriorityClass);
        int count_modules = 0;
        ListProcessModules(pe32.th32ProcessID, &count_modules);
        printf("\n\n  Module count      = %d", count_modules);
        ListProcessThreads(pe32.th32ProcessID);

    } while(Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return TRUE;
}

int main(int argc, char* argv[]){
    GetProcessList();
    return 0;
}