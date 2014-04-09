// fe_Wnd.cpp //                                   \author Logan Jones
///////////////                                       \date 10/3/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_Wnd.h"


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_Wnd::fe_Wnd():gui_Window()
{}
fe_Wnd::~fe_Wnd()
{
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Wnd::LoadPanel() //                          \author Logan Jones
////////////////////////                              \date 10/14/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strPanel - 
//
// Return: BOOL - 
//
BOOL fe_Wnd::LoadPanel( LPCTSTR strPanel )
{
	char		PanelPath[ MAX_PATH ];
	BYTE*		pPanelBuffer = NULL;
	DWORD		BufferSize;

	// Configure the path
	sprintf( PanelPath, "bitmaps/%s.pcx", strPanel );

	// Retrieve the buffer
	BufferSize = theFileTree.DumpFileToBuffer( PanelPath, &pPanelBuffer );
	if( (pPanelBuffer==NULL) || (BufferSize==0) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to dump \'%s\' to a buffer.", PanelPath );
		return FALSE;
	}

	// Create the surface
	if( bFAILED(gfx->CreateSurfaceFromPCX(
		pPanelBuffer,
		BufferSize,
		GFX_FORCE_CURRENT_PALETTE,
		m_Size,
		&m_Panel )) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create a surface from %s.", strPanel );
		delete [] pPanelBuffer;
		return FALSE;
	}

	// Delete the buffer
	delete [] pPanelBuffer;

	// Return success
	return TRUE;
}
// End fe_Wnd::LoadPanel()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_Wnd.cpp //
/////////////////////
