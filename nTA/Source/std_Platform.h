// std_Platform.h //                               \author Logan Jones
///////////////////                                   \date 1/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _STD_PLATFORM_H_
#define _STD_PLATFORM_H_
/////////////////////////////////////////////////////////////////////


#ifdef _WIN32

	#define WIN32_LEAN_AND_MEAN			1
	#include <winsock2.h>
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

	typedef int ssize_t;
	typedef DWORD Param_t;

#define __unused	


#elif __APPLE__
#include "TargetConditionals.h"
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <objc/objc.h>
#include <algorithm>

#if SYS_GLFW
#include <GLFW/glfw3.h>
#endif

typedef void * LPVOID;
typedef const char * LPCTSTR;
typedef char * LPTSTR;
typedef char * LPSTR;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef uint32_t UINT32;
typedef int32_t INT32;
typedef uint16_t UINT16;
typedef int16_t INT16;
typedef uint8_t UINT8;
typedef int8_t INT8;
typedef uint32_t UINT;
typedef long LRESULT;
#define stricmp strcasecmp
#define strnicmp strncasecmp
#define ZeroMemory(P,SZ) memset(P,0,SZ)
#define CALLBACK
#define WINAPI
typedef int SOCKET;

typedef size_t Param_t;

#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

#define __min(a,b)  std::min(a,b)
#define __max(a,b)  std::max(a,b)

#define MAX_PATH 255

typedef struct PALETTEENTRY {
    BYTE peRed;
    BYTE peGreen;
    BYTE peBlue;
    BYTE peFlags;
} PALETTEENTRY, *LPPALETTEENTRY;

#define INVALID_SOCKET (int)(~0)

#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#  error "Unknown Apple platform"
#endif

// Platform specific stuff used everywhere
struct std_PlatformParameters
{
    // Name given for the main window
    char				MainWindowName[ 128 ];
    
#if SYS_GLFW
    GLFWwindow          *mainWindow;
#endif
    
};

extern char * itoa(  int value, char * buffer, int radix );
extern char * ltoa( long value, char * buffer, int radix );


#else
#  error "Unknown Platform"
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_STD_PLATFORM_H_)
