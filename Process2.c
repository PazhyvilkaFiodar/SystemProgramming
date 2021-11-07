#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    LPCTSTR pipename = "\\\\.\\pipe\\myPipe";
    HANDLE hNamedPipe;
    DWORD szAddress;
    DWORD cbRead, cbWritten;
    OVERLAPPED overlapped;
    char* msg;

    printf("Creating pipe...\n");
    hNamedPipe = CreateNamedPipe(pipename, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE |
        PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, sizeof(DWORD), sizeof(DWORD), 100, NULL);
    if (hNamedPipe == INVALID_HANDLE_VALUE) {
        printf("Unable to create pipe!\n");
        return 1;
    }

    printf("Success!\nConnecting to pipe...\n");
    ConnectNamedPipe(hNamedPipe, &overlapped);

    printf("Success!\nReading from pipe...\n");
    if (!ReadFile(hNamedPipe, &szAddress, sizeof(szAddress), &cbRead, NULL)) {
        printf("Unable to read string from the pipe!\n");
        return 2;
    }

    printf("Success!\nYour string: ");
    msg = (char*)szAddress;
    printf("%s\n", msg);

    printf("Sending answer to Process1...\n");
    if (!WriteFile(hNamedPipe, &szAddress, sizeof(szAddress), &cbWritten, NULL)) {
        printf("Unable to send answer to the first process!\n");
        return 3;
    }

    printf("Success!\nDisconnecting from named pipe...\n");
    if (!DisconnectNamedPipe(hNamedPipe)) {
        printf("Unable to disconnect from pipe!\n");
        return 4;
    }

    printf("Success!\nClosing named pipe...\n");
    if (!CloseHandle(hNamedPipe)) {
        printf("Unable to close the pipe!\n");
        return 5;
    }

    printf("Success!\nProcess2 was finished...\n");
    return 0;
}