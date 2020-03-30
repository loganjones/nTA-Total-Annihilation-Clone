//
//  app_nTA_glfw.cpp
// app_nTA_glfw.cpp //                            \author Logan Jones
/////////////////////                                  \date 6/2/2016
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#if SYS_GLFW

#include "app.h"
#include "game.h"
#include "ogl.h"

extern LRESULT WINAPI RetrieveGfxInterface( gfx_Interface** ppGfxInterface );


//////////////////////////////////////////////////////////////////////
// app_nTA::RetrieveGfxInterface() //              \author Logan Jones
/////////////////////////////////////                  \date 9/22/2001
//
//====================================================================
// Return: BOOL -
//
BOOL app_nTA::RetrieveGfxInterface()
{
    ::RetrieveGfxInterface( &gfx );
    
    // Initialize the interface
    return gfx->Initialize( &m_PlatformParameters, GfxLoadProc, GfxErrorProc );
}
// End app_nTA::RetrieveGfxInterface()
//////////////////////////////////////////////////////////////////////

#endif // SYS_GLFW
/////////////////////////////////////////////////////////////////////
// End - app_nTA_glfw.cpp //
///////////////////////////
