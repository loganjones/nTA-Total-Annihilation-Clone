// gfx_OpenGL_Win32.cpp //                 Author: Logan "Burn" Jones
/////////////////////////                             Date: 9/21/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"


// Instansiate the external gfx pointers
gfx_Interface*		pGfxSystem = NULL;
gfx_Interface**		ppGfx = NULL;


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
	// TODO: Implement gfx_EnumDisplayMode!
	return 0;
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
BOOL gfx_OpenGL::Create( const gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen )
{
    GLFWwindow* window = this->m_pPlatformParams->mainWindow;
    
    // If we are supposed to be full screen, change to the desired display mode
    if( bFullscreen )
    {
        // TODO: Implement fullscreen window support.
        
    } // end if( Full Screen )
    
    if( bFAILED(LoadExtensions()) )
    {
        ErrorComment( "One or more required extensions not found." );
        return FALSE;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
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
	// TODO: Implement gfx_OpenGL::Destroy!
	return FALSE;
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
        glColorTableEXT = NULL;//(PFNGLCOLORTABLEEXTPROC)wglGetProcAddress("glColorTableEXT");
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


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::SaveScreenShot() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 5/14/2002
//
//====================================================================
// Parameters:
//  LPCTSTR strScreenShotPath -
//
void gfx_OpenGL::SaveScreenShot( LPCTSTR strScreenShotPath )
{
    // TODO: Implement gfx_OpenGL::SaveScreenShot!
}
// End gfx_OpenGL::SaveScreenShot()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::TEMP_DoBlueScreen() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 9/21/2001
//
//====================================================================
//
void gfx_OpenGL::TEMP_DoBlueScreen()
{
    glClearColor( 0,0,1,1 );
    glClear( GL_COLOR_BUFFER_BIT );
    m_TriangleCount = 0;
    //SwapBuffers(m_hDC);
}
// End gfx_OpenGL::TEMP_DoBlueScreen()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_OpenGL_Win32.cpp //
///////////////////////////////
