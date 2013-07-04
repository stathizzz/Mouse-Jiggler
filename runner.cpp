#include "stdafx.h"

HANDLE hServiceThread;
HANDLE ghAnswerEvent;

DWORD WINAPI AutoAnswerServiceWorker( LPVOID lpParam );

LOADER_DECLARE(int) WriteToLog(LPCTSTR fmt, ...)
{
	FILE* log;
	LPCTSTR path;
	WCHAR res[1000];
	LPWSTR *pa[100];
	va_list args;

	GetEnvironmentVariable(HOME,(LPWSTR)pa,100);

	wcsncat((LPWSTR)pa, LOGFILE, wcslen(LOGFILE));
	log = _wfopen((LPWSTR)pa, L"a+");
	if (log == NULL)
		return -1;

	va_start(args, fmt);
	_vswprintf(res, fmt, args);
	_ftprintf(log, res);
	va_end(args);

	fclose(log);
	return 0;
};

LOADER_DECLARE(void) CreateAutoAnswerService()
{
	DWORD dwThreadID;

	// Create a thread
	hServiceThread = CreateThread( 
		NULL,         // default security attributes
		0,            // default stack size
		(LPTHREAD_START_ROUTINE) AutoAnswerServiceWorker, 
		NULL, // pass the argument
		0,            // default creation flags
		&dwThreadID); // receive thread identifier

	if( hServiceThread == NULL )
	{
		Output(L"CreateThread error: %d\n", GetLastError());
		return;
	}
}

DWORD WINAPI AutoAnswerServiceWorker( LPVOID lpParam )
{
	UNREFERENCED_PARAMETER(lpParam);

	CreateMyEvent(&ghAnswerEvent);

	pollRoutine( (void *(__stdcall *)(void))JiggleIt2, INFINITE, ghAnswerEvent);

	WaitMyEvent(ghAnswerEvent, INFINITE, NULL);

	CloseMyEvent(ghAnswerEvent);

	return 0;
}



