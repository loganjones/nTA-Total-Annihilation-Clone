// sys_Win32.cpp //                                \author Logan Jones
//////////////////                                     \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#define INSTANTIATE_SYS_INTERFACE_POINTER
#include "sys.h"
#include "..\res\resource.h"

#include "input.h"


// Foward declare WndProc so it can be assigned in WinMain()
LRESULT CALLBACK WndProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam );


//////////////////////////////////////////////////////////////////////
// WinMain() //                                    \author Logan Jones
//////////////                                          \date 9/9/2001
//               
//====================================================================
// Parameters:
//  HINSTANCE hInstance     - This app's HINSTANCE
//  HINSTANCE hPrevInstance - Leagacy crap
//  LPSTR lpCmdLine         - The given command line
//  int nCmdShow            - Initial visibility instructions
//
// Return: int WINAPI - Exit code
//
int WINAPI WinMain(	HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	assertEx( sys!=NULL, "sys hasn't been assigned yet! BAD BAD BAD" );

	// Save the HINSTANCE and WndProc values to the platform paramaters
	sys->PlatformParameters()->hInstance = hInstance;
	sys->PlatformParameters()->pWndProc = WndProc;

	sys->Run();

	return sys->GetExitCode();
}
// End WinMain()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// WndProc() //                                    \author Logan Jones
//////////////                                          \date 9/9/2001
// Description:  WinMain is our entry point for this Win32 app.
//               Assign some platform specific stuff here and move on.
//====================================================================
// Parameters:
//  HWND hWnd     - HANDLE to the calling window
//  UINT uiMsg    - The message to process
//  WPARAM wParam - Message data
//  LPARAM lParam - Message data
//
// Return: LRESULT CALLBACK - Return 0 if message was handled, pass if on to DefWindowProc() otherwise
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	// Intercept a few messages of interest
	switch( uiMsg )
	{
		case WM_ACTIVATE:
		{
			if( LOWORD(wParam)==WA_INACTIVE )
			{
				// The App is being deactivated
				sys->ChangeState( sys_App::State_Paused );
			}
			else
			{
				// The App is being activated
				sys->ChangeState( sys_App::State_Active );
			}
			return 0;

		} // end case WM_ACTIVATE

		case WM_ACTIVATEAPP:
		{
			if( ((BOOL)wParam)==TRUE )
			{
				// The App is being activated
				sys->ChangeState( sys_App::State_Active );
			}
			else
			{
				// The App is being deactivated
				sys->ChangeState( sys_App::State_Paused );
			}
			return 0;

		} // end case WM_ACTIVATEAPP

// FIX:		This is a temporary case. Used only for emergency shutdown of the app. REMOVE before release.
		case WM_KEYDOWN:
		{
            // Check for the PAUSE-BREAK key
			if( wParam==VK_PAUSE )
			{
                // Ask the app to quit
                sys->Quit();
                return 0;
			}
			else break;
		}
// END NOTE

		case WM_CLOSE:
		{
			sys->Quit();
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				// We don't want these to happen
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
				// Prevent it :)
			}
			break;
		}

	} // End switch( message )

	// Let the input system peek in
	if( input.CheckWindowsMessage(uiMsg,wParam,lParam) )
		return 0;

	// Send it on
	return DefWindowProc( hWnd, uiMsg, wParam, lParam );
}
// End WndProc()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_Assert() //                                 \author Logan Jones
/////////////////                                      \date 9/10/2001
// Description:  Temporary home of the assert function. Just calls
//               the Win32 message box with the Abort Retry Fail option
//====================================================================
// Parameters:
//  LPTSTR strAssertion - The condition code that failed the assertion (in string form of course)
//  LPTSTR strExtraInfo - A string containing any extra info given by the coder, or NULL for nothing
//  LPTSTR strFileName  - String bufer containing the file name in which the assertion occured
//  int iLine           - Line number in the file where the assertion occured
//
// Return: BOOL - Returns TRUE to break into the debugger, FALSE to continue
//
BOOL std_Assert( LPTSTR strAssertion, LPTSTR strExtraInfo, LPTSTR strFileName, int iLine )
{
	static BOOL stdAssert_bIgnore = FALSE;
	int			iResult;
	char		strAssertTitle[ 256 ];
	char		strAssertBox[ 1024 ];

	// Just continue if the ignore flag has been set
	if( stdAssert_bIgnore==TRUE ) return FALSE;

	// Come up with a nice message foe the user ;)
	sprintf( strAssertBox,
			"Program: %s\n"
			"File: %s\n"
			"Line: %d\n"
			"\n"
			"assert( %s ); Failed\n",
			__argv[0], strFileName, iLine,
			strAssertion );
	if( strExtraInfo ) sprintf( strAssertBox, "%s\n%s\n", strAssertBox, strExtraInfo );
	strcat( strAssertBox, 
			"\n"
			"\n"
			"* Press Abort to break into the debugger.\n"
			"* Press Retry to continue program execution.\n"
			"* Press Ignore to disable any further assertion failures." );

	// Generic title
	sprintf( strAssertTitle, "Assertion Failure" );

	// Call the message box
	iResult = MessageBox(
		NULL,
		strAssertBox,
		strAssertTitle,
		MB_ABORTRETRYIGNORE | MB_ICONERROR | MB_TOPMOST );

	// Check the result
	switch( iResult )
	{
	case IDABORT: return TRUE;
	case IDRETRY: return FALSE;
	case IDIGNORE:
		stdAssert_bIgnore = TRUE;
		break;
	}

	// Continue execution
	return FALSE;
}
// End sys_Assert()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Create() //                            \author Logan Jones
//////////////////////                                 \date 9/24/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSplashPath - 
//
// Return: BOOL - 
//
BOOL sys_App::Create( LPCTSTR strSplashPath )
{
	DWORD					Error;
	WNDCLASS				wc;
	DWORD					WndStyle;
	DWORD					WndExStyle;
	RECT					WindowRect;
	SIZE					WindowSize;
	HBITMAP					hBitmap;
	HDC						hDC;
	HDC						hMemDC;

	// Fill out the window class structure
	ZeroMemory( &wc, sizeof(WNDCLASS) );
	wc.style			= CS_OWNDC;
	wc.lpfnWndProc		= (WNDPROC)m_PlatformParameters.pWndProc;
	wc.hInstance		= m_PlatformParameters.hInstance;
	wc.hIcon			= LoadIcon( m_PlatformParameters.hInstance, MAKEINTRESOURCE(IDI_nTA_ICON) );
	wc.hCursor			= LoadCursor(NULL, IDC_WAIT);
	wc.lpszClassName	= m_PlatformParameters.WindowClass;

	// Register the window class
	if( RegisterClass(&wc)==0 )
	{
		Error = GetLastError();
		ErrorBox( "Error -> sys_App::Create()", "Failed to register window class: %d", Error );
		return FALSE;
	}

	// Setup the window styles
	WndStyle = WS_POPUPWINDOW;
	WndExStyle=WS_EX_APPWINDOW;

	// Try to load the splash image if we are supposed to
	if( strSplashPath )
	{
		hBitmap = (HBITMAP)LoadImage( m_PlatformParameters.hInstance, strSplashPath, IMAGE_BITMAP, 0,0, LR_DEFAULTCOLOR | LR_LOADFROMFILE );
		if( hBitmap )
		{
			BITMAP	bm;
			GetObject( hBitmap, sizeof(BITMAP), (LPVOID)&bm );
			WindowSize.cx = bm.bmWidth;
			WindowSize.cy = bm.bmHeight;
			WndStyle |= WS_VISIBLE;
		}
		else strSplashPath = NULL;
	}

	// If there is no splash screen or we failed to load the image, set a default window size
	if( strSplashPath==NULL )
	{
		WindowSize.cx = 100;
		WindowSize.cy = 100;
	}

	// Get the window's border rect
	GetWindowRect( GetDesktopWindow(), &WindowRect );
	WindowRect.left = (WindowRect.right / 2) - (WindowSize.cx / 2);
	WindowRect.right= WindowRect.left + WindowSize.cx;
	WindowRect.top = (WindowRect.bottom / 2) - (WindowSize.cy / 2);
	WindowRect.bottom= WindowRect.top + WindowSize.cy;
	AdjustWindowRectEx( &WindowRect, WndStyle, FALSE, WndExStyle );

	// Create the main window
	if(!(m_PlatformParameters.hMainWindow=CreateWindowEx(
		WndExStyle,
		m_PlatformParameters.WindowClass,
		m_PlatformParameters.MainWindowName,
		WndStyle,
		WindowRect.left, WindowRect.top,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		m_PlatformParameters.hInstance,
		NULL )))
	{
		Error = GetLastError();
		ErrorBox( "Error -> sys_App::Create()", "Failed to create window: %d", Error );
		return FALSE;
	}

	// Make the window visible and display the splash screen if desired
	if( strSplashPath )
	{
		// Put up our pretty splash bitmap.
		// Get ready for some windows garbage.
		hDC = GetDC( m_PlatformParameters.hMainWindow );
		hMemDC = CreateCompatibleDC( hDC );
		hBitmap = (HBITMAP)SelectObject( hMemDC, hBitmap );
		BitBlt( hDC, 0, 0, (WORD)WindowSize.cx, (WORD)WindowSize.cy, hMemDC, 0, 0, SRCCOPY );
		DeleteObject( SelectObject(hMemDC,hBitmap) );
		DeleteDC( hMemDC );
		ReleaseDC( m_PlatformParameters.hMainWindow, hDC );

		// Make sure this window is visible
		SetForegroundWindow( m_PlatformParameters.hMainWindow );
		SetFocus( m_PlatformParameters.hMainWindow );
	}

	return TRUE;
}
// End sys_App::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Destroy() //                           \author Logan Jones
///////////////////////                                \date 9/24/2001
//               
//====================================================================
//
void sys_App::Destroy()
{
	if( m_PlatformParameters.hMainWindow )
	{
		DestroyWindow( m_PlatformParameters.hMainWindow );
		m_PlatformParameters.hMainWindow = NULL;
	}

	if( m_PlatformParameters.hInstance )
	{
		UnregisterClass( m_PlatformParameters.WindowClass, m_PlatformParameters.hInstance );
	}
}
// End sys_App::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Quit() //                              \author Logan Jones
////////////////////                                   \date 9/10/2001
// Description:  Quit is called to safely terminate this run of the app.
//               
//====================================================================
//
void sys_App::Quit()
{
	// Just post the quit message, and the message pump should
	// change the app state.
    PostQuitMessage(1);
}
// End sys_App::Quit()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::ErrorBox() //                          \author Logan Jones
////////////////////////                               \date 9/29/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strTitle    - 
//  LPCTSTR strErrorMsg - 
//  ...                 - 
//
// Return: BOOL - 
//
BOOL sys_App::ErrorBox( LPCTSTR strTitle, LPCTSTR strErrorMsg, ... )
{
	va_list 	va_ArgList;
	char		Message[ 256 ];

	// Access the argument list
	va_start( va_ArgList, strErrorMsg );

	// Get the message string
	vsprintf( Message, strErrorMsg, va_ArgList );

	MessageBox( m_PlatformParameters.hMainWindow, Message, strTitle, MB_OK | MB_ICONERROR );

	// Close the argument list
	va_end( va_ArgList );

	return TRUE;
}
// End sys_App::ErrorBox()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::CheckForSysMessage() //                \author Logan Jones
//////////////////////////////////                     \date 9/10/2001
// Description:  Checks the windows message queue.
//               
//====================================================================
// Return: BOOL - Returns TRUE if a message was found and handled, FALSE otherwise
//
BOOL sys_App::CheckForSysMessage()
{
	MSG msg;

	// Check the windows message queue
	if( PeekMessage(&msg,NULL,0,0,PM_NOREMOVE)==TRUE )
	{
		// Try to get the message from the queue.
		if( GetMessage(&msg,NULL,0,0)==FALSE )
        {
            // We probably got a WM_QUIT meassage.
            // Tell the app to close
			m_ExitCode = (int)msg.wParam;
            ChangeState( State_ShutDown );
        }
		else // Handle normaly
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// A message was found, return TRUE
		return TRUE;
	}

	// No message was found, return FALSE
	return FALSE;
}
// End sys_App::CheckForSysMessage()
//////////////////////////////////////////////////////////////////////


#endif // defined( _WIN32 )
/////////////////////////////////////////////////////////////////////
// End - sys_Win32.cpp //
////////////////////////
