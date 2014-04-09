// gadget_Pic.cpp //                               \author Logan Jones
///////////////////                                  \date 04-30-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_Pic.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_Pic::gadget_Pic():gui_Gadget(),
	m_Animating(false),
	m_Loop(false),
	m_Resize(false),
	m_FrameCount(0),
	m_Frame(0),
	m_Frames(NULL),
	m_ButtonStates(0)
{}
gadget_Pic::~gadget_Pic()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::Create() //                         \author Logan Jones
/////////////////////////                            \date 04-30-2003
//
//===================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData - 
//  ta_gui_Pic* pPicInfo         - 
//  wnd_Window* pParent          - 
//
// Return: BOOL - 
//
BOOL gadget_Pic::Create( ta_gui_Common_t* pCommonData, ta_gui_Pic* pPicInfo, wnd_Window* pParent )
{
	// Sanity check
	assert( pPicInfo );

	// Copy the common data
	memcpy( &m_PicInfo, pPicInfo, sizeof(m_PicInfo) );

	// Call the default Create() method
	if( bFAILED(gui_Gadget::Create(pCommonData,pParent)) )
	{
		return FALSE;
	}

	return TRUE;
}
// End gadget_Pic::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::Create() //                         \author Logan Jones
/////////////////////////                            \date 04-30-2003
//
//===================================================================
// Parameters:
//  const std_Point_t& ptPos - 
//  const std_Size_t& szDim  - 
//  wnd_Window* pParent      - 
//
// Return: BOOL - 
//
BOOL gadget_Pic::Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent )
{
	m_PicInfo.HotOrNot = 0;

	m_CommonData.ID = GADGET_Pic;
	m_CommonData.XPos = ptPos.x;
	m_CommonData.YPos = ptPos.y;
	m_CommonData.Width = szDim.width;
	m_CommonData.Height= szDim.height;
	m_CommonData.Active = 1;
	m_CommonData.Name[0] = '\0';
	m_CommonData.Attribs = 0;

	return wnd_Window::Create( ptPos, szDim, pParent );
}
// End gadget_Pic::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::SetFrames() //                      \author Logan Jones
////////////////////////////                         \date 04-30-2003
//
//===================================================================
// Parameters:
//  gfx_Surface** pFrames - 
//  DWORD dwFrameCount    - 
//
void gadget_Pic::SetFrames( gfx_Surface** pFrames, DWORD dwFrameCount )
{
	m_Frames = pFrames;
	m_FrameCount = dwFrameCount;
	m_Frame = 0;

	if( m_Frames && m_Resize && m_Frames[0] )
		SetWndSize( m_Frames[0]->GetSize() );
}
// End gadget_Pic::SetFrames()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::FrameSize() //                      \author Logan Jones
////////////////////////////                         \date 04-30-2003
//
//===================================================================
// Parameters:
//  bool bFrameSize - 
//
void gadget_Pic::FrameSize( bool bFrameSize )
{
	m_Resize = bFrameSize;
	if( m_Frames && m_Resize && m_Frames[m_Frame] )
		SetWndSize( m_Frames[m_Frame]->GetSize() );
}
// End gadget_Pic::FrameSize()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::OnCreate() //                       \author Logan Jones
///////////////////////////                          \date 04-30-2003
//
//===================================================================
// Return: BOOL - 
//
BOOL gadget_Pic::OnCreate()
{
	// Set activation and visibility
	SetActive( m_CommonData.Active );
	SetVisible( m_CommonData.Active );

	// Initialize
	m_ButtonStates = 0;

	// Return the default
	return gui_Gadget::OnCreate();
}
// End gadget_Pic::OnCreate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::OnDestroy() //                      \author Logan Jones
////////////////////////////                         \date 04-30-2003
//
//===================================================================
//
void gadget_Pic::OnDestroy()
{
	gui_Gadget::OnDestroy();
}
// End gadget_Pic::OnDestroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::OnUpdate() //                       \author Logan Jones
///////////////////////////                          \date 04-30-2003
//
//===================================================================
//
void gadget_Pic::OnUpdate()
{
	if( m_Animating )
		if( ++m_Frame>=m_FrameCount )
			m_Frame = m_Loop ? 0:m_FrameCount-1;
}
// End gadget_Pic::OnUpdate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::OnRender() //                       \author Logan Jones
///////////////////////////                          \date 04-30-2003
//
//===================================================================
//
void gadget_Pic::OnRender()
{
	std_Point_t	Pos;

	// Render the frame
	if( m_Frames && m_Frames[m_Frame] )
		m_Frames[m_Frame]->Render(m_ScreenPosition,m_Size);
	else gfx->DrawRect( std_Rect(m_ScreenPosition,m_Size), (DWORD)0xFF0000FF );

	// Render the text
	if( !m_Text.empty() )
		gfx->SetCurrentFont( guiResources.Fonts.StandardSmall ),
		Pos = m_ScreenPosition + std_Point_t( m_Size.width / 2, (m_Size.height / 2) - 1 ),
		gfx->RenderStringCenteredAt( Pos, m_Text.c_str() );

	gui_Gadget::OnRender();
}
// End gadget_Pic::OnRender()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Pic::OnMouseButton() //                  \author Logan Jones
////////////////////////////////                     \date 04-30-2003
//
//===================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void gadget_Pic::OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// If a primary button was pressed or released then do our stuff
	if( dwButton==1 || dwButton==2 )

		// Has it been pressed or released
		if( bDown )

			// Set the mouse capture and indicate that the button is pressed
			SetCapture(),

			// Set the button state for the one that went down
			m_ButtonStates |= dwButton;

		else // button was released
		{
			// Unset the button state for the one that was released
			m_ButtonStates &= ~dwButton;

			// If no button states are now set, then the button has been pressed
			if( !m_ButtonStates )

				// Release capture and check where the button was released
				ReleaseCapture(),
				std_Rect(0,0,m_Size).PointInRect(ptCursor) &&
				SendMessage( m_pParent, gui_msg_ButtonPressed, (DWORD)(LPTSTR(m_CommonData.Name)), dwButton );

		} // end button was released

	// Call the default
	else gui_Gadget::OnMouseButton(ptCursor,dwButton,bDown,dwFlags);
}
// End gadget_Pic::OnMouseButton()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_Pic.cpp //
/////////////////////////