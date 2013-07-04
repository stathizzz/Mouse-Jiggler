#include "stdafx.h"
#include "loader.h"

BOOL t_pressed = FALSE;


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HANDLE hEvent;
	CreateMyEvent(&hEvent);
	CreateAutoAnswerService();
	/* it runs forever until we abruptly end the process */
	WaitMyEvent(hEvent, INFINITE, NULL);

	/* never here */
	CloseMyEvent(hEvent);

	return 0;
}

inline int pressAtMiddle_lParam(HWND handle)
{
	RECT Rect;
	LONG width;
	LONG height;
	/* get the window coordinates, any click event on it will accept the call */
	GetWindowRect(handle, &Rect);
	width = Rect.right- Rect.left;
	height = Rect.bottom - Rect.top;
													
	/* press it at the middle */
	return MAKELPARAM(Rect.right - Rect.left - width/2, Rect.bottom - Rect.top - height/2);
}

inline void getCoordsAtMiddle(HWND handle, int *dx, int *dy)
{
	RECT Rect;
	LONG width;
	LONG height;
	/* get the window coordinates, any click event on it will accept the call */
	GetWindowRect(handle, &Rect);
	width = Rect.right- Rect.left;
	height = Rect.bottom - Rect.top;
													
	/* press it at the middle */
	*dx = Rect.left + width/2;
	*dy = Rect.top + height/2;
}

LOADER_DECLARE(void) JiggleIt2()
{
	__try
	{
		LPCTSTR lpClassName = L"SysListView32";
		LPCTSTR lpWindowName = NULL;
		HWND hhandle;
		int dx = 0, dy = 0;

		setlocale(LC_ALL, "POSIX");

		/* try to grab it with class name only */
		if ((hhandle = FindWindow(lpClassName, lpWindowName)) == NULL)
			return;
		
		
		lpWindowName = L"Start";
		lpClassName = L"Button";
		if ((hhandle = FindWindow(lpClassName, lpWindowName)) == NULL)
			return;

		SetActiveWindow(hhandle);

		getCoordsAtMiddle(hhandle, &dx,&dy);

		/* Implement  left click */
		INPUT    Input={0};
		// left down
		Input.type      = INPUT_MOUSE;
		Input.mi.dwFlags  = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		Input.mi.dx =(dx * 65536) / GetSystemMetrics(SM_CXSCREEN);
		Input.mi.dy =(dy * 65536) / GetSystemMetrics(SM_CYSCREEN);
		Input.mi.time = 0;
		Input.mi.dwExtraInfo = NULL;
		Input.mi.mouseData = 0;
		SendInput(1,&Input,sizeof(INPUT));

		ZeroMemory(&Input,sizeof(INPUT));
		Input.type      = INPUT_MOUSE;
		Input.mi.dwFlags  = MOUSEEVENTF_LEFTDOWN;
		Input.mi.dx = 0;
		Input.mi.dy = 0;
		Input.mi.time = 0;
		Input.mi.dwExtraInfo = NULL;
		Input.mi.mouseData = 0;
		SendInput(1,&Input,sizeof(INPUT));

		
		/* left up - not neeeded if we dont to get the response from the system */
		ZeroMemory(&Input,sizeof(INPUT));
		Input.type      = INPUT_MOUSE;
		Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;
		Input.mi.dx = 0;
		Input.mi.dy = 0;
		Input.mi.time = 0;
		Input.mi.dwExtraInfo = NULL;
		Input.mi.mouseData = 0;
		::SendInput(1,&Input,sizeof(INPUT));
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		Output(L"Exception thrown! %s\n", strerror(GetLastError()));
	}
	
}

LOADER_DECLARE(HWND) JiggleIt()
{
	__try
	{
		HWND hhandle = NULL;
		int  res, width, height;
		RECT Rect;
		
		LPCTSTR lpClassName = L"SysListView32";
		LPCTSTR lpWindowName = NULL;
		
		setlocale(LC_ALL, "POSIX");

		/* try to grab it with class name only */
		if ((hhandle = FindWindow(lpClassName, lpWindowName)) == NULL)
		{	
				return NULL;
		}
		
		lpWindowName = L"Start";
		lpClassName = L"Button";
		if ((hhandle = FindWindow(lpClassName, lpWindowName)) == NULL)
		{
			return NULL;
		}
		/*if (lpWindowName != NULL)
			Output(L"Handle 0x%02X grabbed!\nWindow Name: %s\nClass Name: %s\n", hhandle, lpWindowName, lpClassName);
		else
			Output(L"Handle 0x%06X grabbed!No Window Name!\nClass Name: %s\n", hhandle, lpClassName);
		*/
		
		if (SendMessage(hhandle, WM_MOUSEACTIVATE, (WPARAM)hhandle, MAKELPARAM(MK_LBUTTON, WM_LBUTTONDOWN)) ==  MA_ACTIVATE)
		{
			/* success on first message, try a down click event */
			res = pressAtMiddle_lParam(hhandle);
							
			if ( SendMessage(hhandle, WM_LBUTTONDOWN, MK_LBUTTON, res) == 0)
			{
				res = pressAtMiddle_lParam(hhandle);
				/* success on click down event, press the click up event */
				if ( SendMessage(hhandle, WM_LBUTTONUP, 0, res) == 0)
				{
					Output(L"Button pressed!\n");
					return hhandle;
				}
			}
		}
		_sleep(1000);
		SetForegroundWindow(hhandle);
		res = SendMessage(hhandle, WM_MOUSEACTIVATE, (WPARAM)hhandle, MAKELPARAM(MK_LBUTTON, WM_LBUTTONDOWN));
		
			/* success on first message, try a down click event */
			res = pressAtMiddle_lParam(hhandle);
							
			if ( SendMessage(hhandle, WM_LBUTTONDOWN, MK_LBUTTON, res) == 0)
			{
				res = pressAtMiddle_lParam(hhandle);
				/* success on click down event, press the click up event */
				if ( SendMessage(hhandle, WM_LBUTTONUP, 0, res) == 0)
				{
					Output(L"Button pressed!\n");
					return hhandle;
				}
			}
		

		//if (SendMessage(hhandle, WM_MOUSEACTIVATE, (WPARAM)hhandle, MAKELPARAM(MK_LBUTTON, WM_MOUSEMOVE)) ==  MA_ACTIVATE)
		//{
		//	//if ( SendMessage(hhandle, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0)) == 0)
		//	{				
		//		/* success on click down event, press the click up event */
		//		//if ( SendMessage(hhandle, WM_LBUTTONUP, 0, MAKELPARAM(0, 0)) == 0)
		//		{
		//			SetForegroundWindow(hhandle);
		//			if (SendMessage(hhandle,WM_KEYDOWN, 0x00000011, 0x001D0001) == 0) // CTRL keydown
		//			{
		//				if (SendMessage(hhandle,WM_KEYUP, 0x00000011, 0x001D0001) == 0) // CTRL keyup
		//					return hhandle;
		//			}
		//		}
		//	}
		//	

		//	///* success on first message, try a down click event */
		//	//if ( SendMessage(hhandle, WM_MOUSEMOVE, MK_LBUTTON, res) == 0)
		//	//{
		//	//	/* success on click down event, press the click up event */
		//	//	if ( SendMessage(hhandle, WM_MOUSEMOVE, 0, res) == 0)
		//	//	{
		//	//		Output(L"Auto-answer is implemented!\n");
		//	//		return hhandle;
		//	//	}
		//	//}
		//}
		return NULL;
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		Output(L"Exception thrown! %s\n", strerror(GetLastError()));
		return NULL;
	}
	
}


LOADER_DECLARE(void *) pollRoutine(void *(__stdcall *routine)(void), unsigned int max_time, HANDLE hEvent)
{
	void *ret;
	int timer = 0;
	for(;;)
	{
		if (t_pressed == TRUE)
		{
			if (hEvent != NULL && !SetEvent(hEvent) ) 
			{
				Output(L"SetEvent failed (%d)\n", GetLastError());
			}
			t_pressed = FALSE;
			break;
		}

		if (timer > max_time)
		{
						
			if (!hEvent)
			{
				Output(L"No event to trigger!GLE: (%s)\n", _wcserror(GetLastError()));
				return NULL;
			}
			/* handle created! */
			if ( !SetEvent(hEvent) ) 
			{
				Output(L"SetEvent failed (%s)\n", _wcserror(GetLastError()));
				return NULL;
			}

			return ret;
		}

		ret = routine();
		
		timer += SLEEP_TIME;
		_sleep(SLEEP_TIME);
		continue;
	}

	return NULL;
}

void CreateMyEvent(HANDLE *ghEvent)
{
	*ghEvent = CreateEvent( 
        NULL,   // default security attributes
        FALSE,  // auto-reset event object
        FALSE,  // initial state is nonsignaled
        NULL);  // unnamed object

    if (*ghEvent == NULL) 
    { 
        Output(L"CreateEvent error: %d\n", GetLastError()); 
        ExitProcess(0); 
    } 
}

void *WaitMyEvent(HANDLE ghEvent, unsigned int time, void *(__stdcall *callback)(void))
{
	void *ret;
	DWORD dwEvent = WaitForSingleObjectEx( 
			ghEvent,    // array of objects
			time,		// infinite wait
			FALSE);  

	switch (dwEvent) 
	{ 
		// ghEvent was signaled
		case WAIT_OBJECT_0: 
			if (t_pressed == TRUE)
			{
				return NULL;
			}

			if (callback == NULL)
				return NULL;
			
			ret = callback();
			return ret;
			case WAIT_FAILED:
			case WAIT_TIMEOUT:
			Output(L"WaitForSingleObject() failed, error %u\n", GetLastError());
			break;
		// Return value is invalid.
		default: 
			Output(L"Wait error: %d\n", GetLastError()); 
			break;
	}
	return NULL;
}

BOOL CloseMyEvent(HANDLE ghEvent)
{
	return CloseHandle(ghEvent);
}


int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep) {

   Output(L"In filter.", NULL);

   if (code == EXCEPTION_ACCESS_VIOLATION) 
   {
      Output(L"Caught AV as expected.", NULL);
      return EXCEPTION_EXECUTE_HANDLER;
   }
   else 
   {
      Output(L"Didn't catch AV, unexpected.", NULL);
      return EXCEPTION_CONTINUE_SEARCH;
   };

}