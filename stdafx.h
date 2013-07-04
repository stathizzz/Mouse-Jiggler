#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include <assert.h>
#include <conio.h>
#include "loader.h"
#include <excpt.h>
#include <string.h>
#include <locale.h>

#define HOME L"WINDIR"
#define LOGFILE L"\\jiggle.log"

#define SLEEP_TIME 2000//140000
#define MAX_WAIT_TIME 10000

#ifdef _CONSOLE
#undef _CONSOLE;
#endif

void CreateMyEvent(HANDLE *ghEvent);
void *WaitMyEvent(HANDLE ghEvent, unsigned int time, void *(__stdcall *callback)(void));
BOOL CloseMyEvent(HANDLE ghEvent);
int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep);


struct threadData
{
	HANDLE *hThread;
	HANDLE hEvent;
};
typedef struct threadData threadData;