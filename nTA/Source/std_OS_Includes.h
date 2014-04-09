// std_OS_Includes.h //                            \author Logan Jones
//////////////////////                                 \date 9/8/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_OS_INCLUDES_H_
#define _STD_OS_INCLUDES_H_
/////////////////////////////////////////////////////////////////////


#ifdef _WIN32

	#define WIN32_LEAN_AND_MEAN			1
	#include <windows.h>
	#include <zmouse.h>
	#include <mmsystem.h>

	// Platform specific stuff used everywhere
	struct std_PlatformParameters
	{
		// The current instance for the app.
		// Should be assigned from WinMain at startup.
		HINSTANCE			hInstance;

		// The handle of the main window for the app.
		// The window is created by the gfx system, so this
		// handle should be assigned there.
		HWND				hMainWindow;

		// Address of the app's window message proc.
		// This should also assigned from WinMain at startup.
		// Whenever the app's window class is created, this pointer will
		// be given for the lpfnWndProc parameter
		WNDPROC				pWndProc;

		// The name of the window class used for the app
		char				WindowClass[ 32 ];

		// Name given for the main window
		char				MainWindowName[ 128 ];
	};

#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_OS_INCLUDES_H_)
