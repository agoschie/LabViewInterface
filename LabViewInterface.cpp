//AUTHOR: Aaron J. Goschie
//DATE: May 10, 2019
// LabViewInterface.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "LabViewInterface.h"
#include <string>
#include <iostream>
#include <exception>
//define global variables to store handles to pipes.
HANDLE hPipeRead;
HANDLE hPipeWrite;

union ByteUnsignedInt
{
	unsigned int integer;
	unsigned char bytes[4];
};

//initialize the pipes
__declspec(dllexport) int CreateInterface(bool blocking)
{
	hPipeRead = CreateNamedPipe(TEXT("\\\\.\\pipe\\LABVIEW_IN"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);

	hPipeWrite = CreateNamedPipe(TEXT("\\\\.\\pipe\\LABVIEW_OUT"),
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   // FILE_FLAG_FIRST_PIPE_INSTANCE is not needed but forces CreateNamedPipe(..) to fail if the pipe already exists...
		1,
		1024 * 16,
		1024 * 16,
		NMPWAIT_USE_DEFAULT_WAIT,
		NULL);
	return(1);
}

__declspec(dllexport) int WaitForInterface()
{
	if (hPipeWrite != INVALID_HANDLE_VALUE && hPipeRead != INVALID_HANDLE_VALUE)
	{
		if (ConnectNamedPipe(hPipeRead, NULL) != FALSE && ConnectNamedPipe(hPipeWrite, NULL) != FALSE)
		{
			return (1);
		}
	}
	return(0);
}
__declspec(dllexport) int SendData(unsigned char bytes[], unsigned int sizeBytes)
{
	DWORD dwWrite;
	ByteUnsignedInt size;
	unsigned char * allBytes;
	unsigned int i;

	if (sizeBytes > 0)
	{
		size.integer = sizeBytes;
		allBytes = new unsigned char[size.integer + 4];
		i = 0;
		while (i < 4)
		{
			allBytes[i] = size.bytes[i];
			i++;
		}
		while (i < (size.integer + 4))
		{
			allBytes[i] = bytes[i - 4];
			i++;
		}
		if (WriteFile(hPipeWrite,
			allBytes,
			size.integer + 4,
			&dwWrite,
			NULL))
		{
			delete[] allBytes;
			return (1);
		}
		delete[] allBytes;
	}

	return(0);
}
__declspec(dllexport) unsigned int ReceiveSize()
{
	unsigned char buffer[4];
	DWORD dwRead;
	if (ReadFile(hPipeRead, buffer, 4, &dwRead, NULL) != FALSE)
	{
		return (*((unsigned int*)(buffer)));
	}
	return(0);
}
__declspec(dllexport) unsigned int ReceiveData(unsigned char bytes[], unsigned int size)
{
	DWORD dwRead;
	if (ReadFile(hPipeRead, bytes, size, &dwRead, NULL) != FALSE)
	{
		return (dwRead);
	}
	return(0);
}

__declspec(dllexport) int DisconnectInterface()
{
	int res1, res2;

	res1 = DisconnectNamedPipe(hPipeRead);
	res2 = DisconnectNamedPipe(hPipeWrite);
	return (res1 && res2);
}

__declspec(dllexport) int CloseInterface()
{
	int res1, res2;

	res1 = CloseHandle(hPipeRead);
	res2 = CloseHandle(hPipeWrite);
	return (res1 && res2);
}