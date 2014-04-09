// gfx_OpenGL_Win32.cpp //                 Author: Logan "Burn" Jones
/////////////////////////                             Date: 9/21/2001
//
/////////////////////////////////////////////////////////////////////
#ifdef _WIN32

#include "ogl.h"


//////////////////////////////////////////////////////////////////////
// gfx_EnumDisplayMode() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 8/8/2002
//               
//====================================================================
// Parameters:
//  DWORD dwIndex         - 
//  std_Size* pResolution - 
//  long* pDepth          - 
//
// Return: DWORD - 
//
DWORD CALLBACK gfx_EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth )
{
	DEVMODE				Mode;	
	std_Size			LastRes = *pResolution;
	long				LastDepth = *pDepth;

	// Continue enumerating until it fails or produces a different acceptable result
	do if( EnumDisplaySettings(NULL,dwIndex++,&Mode)==FALSE ) return 0;
	while( (LastRes.width==Mode.dmPelsWidth && LastRes.height==Mode.dmPelsHeight && LastDepth==Mode.dmBitsPerPel) ||
		   (Mode.dmBitsPerPel!=16 && Mode.dmBitsPerPel!=32) );

	// Set the return data and return
	pResolution->Set( Mode.dmPelsWidth, Mode.dmPelsHeight );
	*pDepth = Mode.dmBitsPerPel;
	return dwIndex;
}
// End gfx_EnumDisplayMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Create() //                  Author: Logan "Burn" Jones
/////////////////////////                              Date: 9/21/2001
//               
//====================================================================
// Parameters:
//  gfx_Resolution_t& resScreen - 
//  int iColorDepth             - 
//  BOOL bFullscreen            - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::Create( gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen )
{
	DEVMODE					DeviceMode;
	DWORD					WndStyle;
	DWORD					WndExStyle;
	RECT					WindowRect;
    PIXELFORMATDESCRIPTOR	pfd;
	GLuint					ChosenFormat;

	// If we are supposed to be full screen, change to the desired display mode
	if( bFullscreen )
	{
		// Fill out the device mode structure
		ZeroMemory( &DeviceMode, sizeof(DEVMODE) );
		DeviceMode.dmSize = sizeof(DEVMODE);
		DeviceMode.dmPelsWidth = resScreen.width;
		DeviceMode.dmPelsHeight= resScreen.height;
		DeviceMode.dmBitsPerPel = iColorDepth;
		DeviceMode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Change to full screen
		if( ChangeDisplaySettings(&DeviceMode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL )
		{
			ErrorComment( "Failed to change the display mode." );
            return FALSE;
		}

    } // end if( Full Screen )

	// Setup the window styles
	WndStyle = (bFullscreen) ? (WS_POPUP) : (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	WndExStyle=(bFullscreen) ? (WS_EX_APPWINDOW) : (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	// Get the window's border rect
	SetRect( &WindowRect, 0, 0, resScreen.width, resScreen.height );
	AdjustWindowRectEx( &WindowRect, WndStyle, FALSE, WndExStyle );

	// Reset the window styles
	SetWindowLong( m_pPlatformParams->hMainWindow, GWL_EXSTYLE, WndExStyle );
	SetWindowLong( m_pPlatformParams->hMainWindow, GWL_STYLE, WndStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN );
	SetWindowPos( m_pPlatformParams->hMainWindow, NULL,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		SWP_NOZORDER );

    // Fill out the pixel format
    ZeroMemory( &pfd, sizeof(PIXELFORMATDESCRIPTOR) );
    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = iColorDepth;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

	// Get the device context from the window
	if( (m_hDC=GetDC(m_pPlatformParams->hMainWindow))==NULL )
	{
		ErrorComment( "Failed to get the device context from the main window." );
		return FALSE;
	}

	// Get a format from the device context
	if( !(ChosenFormat=ChoosePixelFormat(m_hDC,&pfd)) )
	{
		ErrorComment( "Failed to match an appropriate pixel format." );
		return FALSE;
	}

	// Attempt to set the format
	if( SetPixelFormat(m_hDC,ChosenFormat,&pfd)==FALSE )
	{
		ErrorComment( "Failed to set the desired pixel format." );
		return FALSE;
	}

	// Create a rendering context
	if( (m_hRC=wglCreateContext(m_hDC))==NULL )
	{
		ErrorComment( "Failed to create the rendering context." );
		return FALSE;
	}

	// Make the rendering context current
	if( wglMakeCurrent(m_hDC,m_hRC)==FALSE )
	{
		ErrorComment( "Failed to make the rendering context current." );
		return FALSE;
	}

	if( bFAILED(LoadExtensions()) )
	{
		ErrorComment( "One or more required extensions not found." );
		return FALSE;
	}

	// Hide the windows mouse curosr for this window
	ShowCursor(FALSE);
	SetCursorPos( 0, 0 );

	// Make sure this window is visible
	ShowWindow( m_pPlatformParams->hMainWindow, SW_SHOW );
	SetForegroundWindow( m_pPlatformParams->hMainWindow );
	SetFocus( m_pPlatformParams->hMainWindow );

	m_ScreenResolution = resScreen;

	return PostCreate();
}
// End gfx_OpenGL::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Destroy() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 9/21/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_OpenGL::Destroy()
{
	// Check if the rendering context is valid
	if( m_hRC!=NULL )
	{
		// Make the context no longer current
		wglMakeCurrent( NULL ,NULL );

		// Delete the context
		wglDeleteContext( m_hRC );

		// Set the context to NULL for safety
		m_hRC = NULL;
	}

	// Make sure the device context is valid and release it
	if( m_hDC!=NULL )
	{
		ReleaseDC( m_pPlatformParams->hMainWindow, m_hDC );

		// Set the context to NULL for safety
		m_hDC = NULL;
	}

	// Reset the display mode to the default
    ChangeDisplaySettings( NULL, 0 );

	return TRUE;
}
// End gfx_OpenGL::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::LoadExtensions() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 10/8/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_OpenGL::LoadExtensions()
{
	// Initialize the extension flags
	ZeroMemory( &m_Extensions, sizeof(Extensions_t) );

	// Check for the edge clamp texture op
	if( CheckForExtensionString("GL_SGIS_texture_edge_clamp")||
        CheckForExtensionString("GL_EXT_texture_edge_clamp") )
		m_Extensions.Texture_Edge_Clamp = TRUE;
    else ErrorComment( "GL_SGIS_texture_edge_clamp not supported." );

    // Check for paletted texture support
	if( CheckForExtensionString("GL_EXT_paletted_texture") )
    {
        glColorTableEXT = (PFNGLCOLORTABLEEXTPROC)wglGetProcAddress("glColorTableEXT");
        m_Extensions.Paletted_Texture = (glColorTableEXT!=NULL);
        if( glColorTableEXT==NULL )
            ErrorComment( "GL_EXT_paletted_texture not supported." );
    }
	else ErrorComment( "GL_EXT_paletted_texture not supported." );

    // Check for shared texture palette support
	if( CheckForExtensionString("GL_EXT_shared_texture_palette") )
		m_Extensions.Shared_Texture_Palette = TRUE;
    else ErrorComment( "GL_EXT_shared_texture_palette not supported." );

    // Check for vertex array support
	if( CheckForExtensionString("GL_EXT_vertex_array")/*||(m_Version.Minor>=1)*/ )
		m_Extensions.Vertex_Array = TRUE;
    else ErrorComment( "GL_EXT_vertex_array not supported." );

	return TRUE;
}
// End gfx_OpenGL::LoadExtensions()
//////////////////////////////////////////////////////////////////////


#endif // defined( _WIN32 )
/////////////////////////////////////////////////////////////////////
// End - gfx_OpenGL_Win32.cpp //
///////////////////////////////
