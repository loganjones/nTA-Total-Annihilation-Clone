// gadget_String.cpp //                            \author Logan Jones
/////////////////////////                            \date 10/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_String.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_String::gadget_String()
{}
gadget_String::~gadget_String()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_String::Create() //                      \author Logan Jones
///////////////////////////////                       \date 10/14/2001
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData - 
//  ta_gui_String* pStringInfo   - 
//  wnd_Window* pParent          - 
//
// Return: BOOL - 
//
BOOL gadget_String::Create( ta_gui_Common_t* pCommonData, ta_gui_String* pStringInfo, wnd_Window* pParent )
{
	// Sanity check
	assert( pStringInfo );

	// Call the default Create() method
	if( bFAILED(gui_Gadget::Create(pCommonData,pParent)) )
	{
		return FALSE;
	}

	// Copy the common data
	memcpy( &m_StringInfo, pStringInfo, sizeof(m_StringInfo) );

	return TRUE;
}
// End gadget_String::Create()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_String::Create() //                      \author Logan Jones
////////////////////////////                         \date 04-25-2003
//
//===================================================================
// Parameters:
//  const std_Point_t& ptPos - 
//  const std_Size_t& szDim  - 
//  wnd_Window* pParent      - 
//
// Return: BOOL - 
//
BOOL gadget_String::Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent )
{
	m_StringInfo.Text[0] = '\0';

	m_CommonData.ID = GADGET_String;
	m_CommonData.XPos = ptPos.x;
	m_CommonData.YPos = ptPos.y;
	m_CommonData.Width = szDim.width;
	m_CommonData.Height= szDim.height;
	m_CommonData.Active = 1;
	m_CommonData.Name[0] = '\0';
	m_CommonData.Attribs = 2;

	return wnd_Window::Create( ptPos, szDim, pParent );
}
// End gadget_String::Create()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_String::OnRender() //                    \author Logan Jones
/////////////////////////////////                     \date 10/14/2001
//               
//====================================================================
//
void gadget_String::OnRender()
{
	std_Point_t		Pos( m_ScreenPosition );
	BOOL			bCenterHoriz = FALSE;
	BOOL			bCenterVert = FALSE;
/*
	PALETTEENTRY	pe;
	pe.peRed = 0x00; pe.peGreen = 0x00; pe.peBlue = 0xFF; pe.peFlags = 0xFF;
	if( *m_Size )
		gfx->DrawRect( std_Rect_t(m_ScreenPosition,m_Size), pe );
	else gfx->DrawRect( std_Rect_t(m_ScreenPosition,std_Size_t(m_Size.width,1)), pe );
*/
	gfx->SetCurrentFont( guiResources.Fonts.StandardSmall );
	if( m_CommonData.Attribs & 0x02 )
	{
		Pos += std_Point_t( m_Size.width / 2 , m_Size.height / 2 );
		bCenterHoriz = TRUE;
		bCenterVert = TRUE;
	}
	else Pos.y += m_Size.height;

	gfx->RenderStringCenteredAt(
		Pos,
		m_StringInfo.Text,
		bCenterHoriz,
		bCenterVert );

	gui_Gadget::OnRender();
}
// End gadget_String::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_String::SetText() //                     \author Logan Jones
////////////////////////////////                      \date 10/16/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strText - 
//
void gadget_String::SetText( LPCTSTR strText )
{
	strcpy( m_StringInfo.Text, strText );
}
// End gadget_String::SetText()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_String.cpp //
///////////////////////////////
