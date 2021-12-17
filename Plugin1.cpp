#include "dll.h"
#include <stdio.h>
#include <TlHelp32.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten) 
{
	int result = _snprintf_s(buffer, dwBufferSize, 20, "Fiodar Pazhyvilka");
	*pdwBytesWritten = result;
	return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 40, "Shows info about dll");
	*pdwBytesWritten = result;
	return result;
}

VOID Execute() 
{
    ShellExecute(NULL, "open", "https://drive.google.com/file/d/1USp7ZCnlEWEVAvE3mF0BP3m2i_IrgSJ2/view?usp=sharing", NULL, NULL, SW_SHOWNORMAL);
}