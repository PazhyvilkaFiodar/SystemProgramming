#include "dll.h"
#include <stdio.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 20, "Fiodar Pazhyvilka");
	*pdwBytesWritten = result;
	return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD *pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 40, "Shows Madagascar penguins theme");
	*pdwBytesWritten = result;
	return result;
}

VOID Execute()
{
	ShellExecute(NULL, "open", "https://www.youtube.com/watch?v=mfHDSxqW1mU", NULL, NULL, SW_SHOWNORMAL);
	printf("\n");
}