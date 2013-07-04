#ifndef __LOADER_H
#define __LOADER_H
#endif

#include <windows.h>

#ifdef __cplusplus
	extern "C" {
#endif

#ifdef _DEBUG
#define Output(fmt, ...) _ASSERTE(WriteToLog((LPCTSTR)fmt, __VA_ARGS__))
#define Output(fmt) _ASSERTE(WriteToLog((LPCTSTR)fmt))
#else
#define Output(fmt, ...) _ASSERTE(NULL)
#define Output(fmt) _ASSERTE(NULL)
#endif

#if defined(LOADER_DECLARE_STATIC)
#define LOADER_DECLARE(type)			type __stdcall
#define LOADER_DECLARE_NONSTD(type)	type __cdecl
#define LOADER_DECLARE_DATA
#elif defined(LOADER_EXPORTS)
#define LOADER_DECLARE(type)			__declspec(dllexport) type __stdcall
#define LOADER_DECLARE_NONSTD(type)	__declspec(dllexport) type __cdecl
#define LOADER_DECLARE_DATA			__declspec(dllexport)
#else
#define LOADER_DECLARE(type)			__declspec(dllimport) type __stdcall
#define LOADER_DECLARE_NONSTD(type)	__declspec(dllimport) type __cdecl
#define LOADER_DECLARE_DATA			__declspec(dllimport)
#endif

LOADER_DECLARE(int) WriteToLog(LPCTSTR fmt, ...);
LOADER_DECLARE(HWND) JiggleIt();
LOADER_DECLARE(void) JiggleIt2();



LOADER_DECLARE(void) CreateAutoAnswerService();

/**
 * Polls routines that take no input argument and return a pointer to a type
 */
LOADER_DECLARE(void *)pollRoutine(void *(__stdcall *routine)(void), unsigned int max_time, HANDLE hEvent);


#ifdef __cplusplus
	}
#endif

