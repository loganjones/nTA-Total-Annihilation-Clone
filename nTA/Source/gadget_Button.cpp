// gadget_Button.cpp //                            \author Logan Jones
//////////////////////                                \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_Button.h"

#define SetStandardImages( X, Y ) \
	m_StageImage = guiResources.StandardButton_##X##x##Y.Stages; \
	m_PressedImage = guiResources.StandardButton_##X##x##Y.Pressed; \
	m_DisabledImage = guiResources.StandardButton_##X##x##Y.Disabled
#define SetStageImages( STAGES ) \
	m_StageImage = guiResources.StageButton_##STAGES.Stages; \
	m_PressedImage = guiResources.StageButton_##STAGES.Pressed; \
	m_DisabledImage = guiResources.StageButton_##STAGES.Disabled

enum ta_gui_Flags_t
{
	// Attribs
	GUI_STAGE_BUTTON			= 1,
	GUI_STANDARD_BUTTON			= 2,
	GUI_CUSTOM_IMAGE			= 1024,
};

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_Button::gadget_Button():gui_Gadget(),
	m_Pressed( false ),
	m_ButtonStates(0),
	m_StageImage( NULL ),
	m_PressedImage( NULL ),
	m_DisabledImage( NULL )
{}
gadget_Button::~gadget_Button()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::Create() //                      \author Logan Jones
////////////////////////////                          \date 10/14/2001
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData - 
//  ta_gui_Button* pButtonInfo   - 
//  wnd_Window* pParent          - 
//
// Return: BOOL - 
//
BOOL gadget_Button::Create( ta_gui_Common_t* pCommonData, ta_gui_Button* pButtonInfo, wnd_Window* pParent )
{
	// Sanity check
	assert( pButtonInfo );

	// Copy the common data
	memcpy( &m_ButtonInfo, pButtonInfo, sizeof(m_ButtonInfo) );

	// Call the default Create() method
	if( bFAILED(gui_Gadget::Create(pCommonData,pParent)) )
	{
		return FALSE;
	}

	return TRUE;
}
// End gadget_Button::Create()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Button::Create() //                      \author Logan Jones
////////////////////////////                         \date 04-30-2003
//
//===================================================================
// Parameters:
//  const std_Point_t& ptPos - 
//  const std_Size_t& szDim  - 
//  wnd_Window* pParent      - 
//
// Return: BOOL - 
//
BOOL gadget_Button::Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent )
{
	m_ButtonInfo.Text[0] = '\0';
	m_ButtonInfo.Stages = 0;
	m_ButtonInfo.Grayed = 0;
	m_ButtonInfo.Status = 0;
	m_ButtonInfo.QuickKey=0;

	m_CommonData.ID = GADGET_Button;
	m_CommonData.XPos = (INT32)ptPos.x;
	m_CommonData.YPos = (INT32)ptPos.y;
	m_CommonData.Width = (UINT32)szDim.width;
	m_CommonData.Height= (UINT32)szDim.height;
	m_CommonData.Active = 1;
	m_CommonData.Name[0] = '\0';
	m_CommonData.Attribs = GUI_STANDARD_BUTTON;

	return wnd_Window::Create( ptPos, szDim, pParent );
}
// End gadget_Button::Create()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::SetButtonImages() //             \author Logan Jones
/////////////////////////////////////                  \date 4/22/2002
//               
//====================================================================
// Parameters:
//  gfx_Surface** pStages  - 
//  gfx_Surface* pPressed  - 
//  gfx_Surface* pDisabled - 
//  DWORD dwStages         - 
//
// Return: BOOL - 
//
BOOL gadget_Button::SetButtonImages( gfx_Surface** pStages, gfx_Surface* pPressed, gfx_Surface* pDisabled, DWORD dwStages )
{
	m_StageImage = pStages;
	m_PressedImage = pPressed;
	m_DisabledImage = pDisabled;
	if( pStages && pStages[0] ) SetWndSize( pStages[0]->GetSize() );
	if( dwStages!=~0 ){
        m_StageText.clear();
		m_ButtonInfo.Stages = dwStages;
		if(m_Stage>=dwStages) m_Stage=dwStages-1;
	}
	return TRUE;
}
// End gadget_Button::SetButtonImages()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::SetStage() //                    \author Logan Jones
/////////////////////////////////                     \date 10/27/2001
//               
//====================================================================
// Parameters:
//  DWORD dwStage - 
//
void gadget_Button::SetStage( DWORD dwStage )
{
	if( dwStage<=m_ButtonInfo.Stages )
		m_Stage = dwStage;
}
// End gadget_Button::SetStage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::SetText() //                     \author Logan Jones
/////////////////////////////                          \date 9/12/2002
//
//====================================================================
// Parameters:
//  LPCTSTR strText - 
//  long lStage     - 
//
void gadget_Button::SetText( LPCTSTR strText, long lStage )
{
	// Check for the default parameter
	if( lStage<0 ) lStage = m_Stage;

	// If the given stage is in range, set the text
	if( lStage<=m_ButtonInfo.Stages )
    {
		if( m_StageText.empty() ) m_StageText.push_back( strText );
		else m_StageText[lStage] = strText;
    }
}
// End gadget_Button::SetText()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::GetText() //                     \author Logan Jones
/////////////////////////////                          \date 9/12/2002
//
//====================================================================
// Parameters:
//  long lStage - 
//
// Return: LPCTSTR - 
//
LPCTSTR gadget_Button::GetText( long lStage ) const
{
	// Check for the default parameter
	if( lStage<0 ) lStage = m_Stage;

	// If the given stage is in range, return the text
	if( lStage<=m_ButtonInfo.Stages )
		return m_StageText[lStage].c_str();
	else return NULL;
}
// End gadget_Button::GetText()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnCreate() //                    \author Logan Jones
/////////////////////////////////                     \date 10/15/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gadget_Button::OnCreate()
{
	LPTSTR		Text;
	LPTSTR		Divider;

	// Get the default images for the button
	OnInitButtonImages();
	if( m_StageImage && m_StageImage[0] ) SetWndSize( m_StageImage[0]->GetSize() );

	// Check if this is an on/off button
	if( m_ButtonInfo.Stages==1 )
		m_ButtonInfo.Stages = 2;

	// Default to the first stage
	m_Stage = 0;

	if( m_ButtonInfo.Stages )
	{
		Text = m_ButtonInfo.Text;
		for( UINT32 n=0; n<m_ButtonInfo.Stages; ++n)
		{
			Divider = strchr( Text, '|' );
			if( Divider )
			{
				Divider[0] = '\0';
				m_StageText.push_back( Text );
				Divider[0] = '|';
				Text = Divider + 1;
			}
			else m_StageText.push_back( Text );
		}
	}
	else if( m_ButtonInfo.Text[0]!='\0' )
		m_StageText.push_back( m_ButtonInfo.Text );
	else
		m_StageText.reserve( 0 );

	// Set activation and visibility
	SetActive( m_CommonData.Active && !m_ButtonInfo.Grayed );
	SetVisible( m_CommonData.Active );

	// Return the default
	return gui_Gadget::OnCreate();
}
// End gadget_Button::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnDestroy() //                   \author Logan Jones
//////////////////////////////////                    \date 10/15/2001
//               
//====================================================================
//
void gadget_Button::OnDestroy()
{
	m_StageText.clear();

	gui_Gadget::OnDestroy();
}
// End gadget_Button::OnDestroy()
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// gadget_Button::OnRender() //                    \author Logan Jones
//////////////////////////////                         \date 4/20/2002
//               
//====================================================================
//
void gadget_Button::OnRender()
{
	std_Point_t Pos, Depression( m_Pressed ? std_Point_t(1,1) : std_Point_t(0,0) );

	//  Render the proper button image
	if( m_bActive )
	{
		if( m_Pressed )
		{
			if( m_PressedImage ) gfx->RenderSurfaceEx( m_ScreenPosition, m_PressedImage );
			else gfx->DrawRect( std_Rect(m_ScreenPosition,m_Size), (DWORD)0xEEEEEEFF );
		}
		else // not pressed
		{
			if( m_StageImage && m_StageImage[m_Stage] )
				gfx->RenderSurfaceEx( m_ScreenPosition, m_StageImage[m_Stage] );
			else gfx->DrawRect( std_Rect(m_ScreenPosition,m_Size), (DWORD)0xAAAAAAFF );
		}
	}
	else // Disabled
	{
		if( m_DisabledImage ) gfx->RenderSurfaceEx( m_ScreenPosition, m_DisabledImage );
		else gfx->DrawRect( std_Rect(m_ScreenPosition,m_Size), (DWORD)0x666666FF );
	}

	// Render the text
	if( !m_StageText.empty() )
	{
		gfx->SetCurrentFont( guiResources.Fonts.Standard );
		if( m_ButtonInfo.Stages )
		{
			Pos = m_ScreenPosition + std_Point_t( 3, (m_Size.height / 2) - 1 ) + Depression;
			gfx->RenderStringCenteredAt( Pos, m_StageText[m_Stage].c_str(), FALSE, TRUE );
		}
		else
		{
			Pos = m_ScreenPosition + std_Point_t( m_Size.width / 2, (m_Size.height / 2) - 1 ) + Depression;
			gfx->RenderStringCenteredAt( Pos, m_StageText[m_Stage].c_str() );
		}
	}

	gui_Gadget::OnRender();
}
// End gadget_Button::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnCursorMove() //                \author Logan Jones
//////////////////////////////////                     \date 4/20/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwFlags         - 
//
void gadget_Button::OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags )
{
	// Check if we have capture
	if( m_pWndManager->GetCapture()==this )
	{
		// If the cursor is in the button window set the pressed state
		if( std_Rect(0,0,m_Size).PointInRect(ptCursor) )
		{
			// If the button wasn't already pressed, send the event
			if( !m_Pressed ) OnButtonDown();
			m_Pressed = true;
		}
		else
		{
			// If the button was pressed, send the event
			if( m_Pressed ) OnButtonUp();
			m_Pressed = false;
		}
	}

	// Call the default
	gui_Gadget::OnCursorMove(ptCursor,dwFlags);
}
// End gadget_Button::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnMouseButton() //               \author Logan Jones
///////////////////////////////////                    \date 4/20/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void gadget_Button::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// If a primary button was pressed or released then do our stuff
	if( dwButton==1 || dwButton==2 )
	{
		// Has it been pressed or released
		if( bDown )
		{
			// Set the mouse capture and indicate that the button is pressed
			SetCapture();
			m_Pressed = true;

			// Set the button state for the one that went down
			m_ButtonStates |= dwButton;

			// Handle the event
			OnButtonDown();
		}
		else // button was released
		{
			// Unset the button state for the one that was released
			m_ButtonStates &= ~dwButton;

			// If no button states are now set, then the button has been pressed
			if( !m_ButtonStates ) {
				// The button is no longer pressed
				m_Pressed = false;
				OnButtonUp();

				// Release capture and check where the button was released
				ReleaseCapture();
				if( std_Rect(0,0,m_Size).PointInRect(ptCursor) )
					OnPressed( dwButton );
			}

		} // end button was released

	} // end if( dwButton )

	// Call the default
	else gui_Gadget::OnMouseButton(ptCursor,dwButton,bDown,dwFlags);
}
// End gadget_Button::OnMouseButton()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gadget_Button::OnInitButtonImages() //          \author Logan Jones
////////////////////////////////////////              \date 11/5/2002
//
//===================================================================
//
void gadget_Button::OnInitButtonImages()
{
	gui_Resources::ButtonImages_t*	pImages;

    if( m_CommonData.Attribs & GUI_STANDARD_BUTTON ) {
        pImages = guiResources.GetClosestStandardButton( std_Size(m_CommonData.Width,m_CommonData.Height) );
        m_StageImage = pImages->Stages;
        m_PressedImage = pImages->Pressed;
		m_DisabledImage = pImages->Disabled;
    }

	else if( m_CommonData.Attribs & GUI_STAGE_BUTTON )
		switch( m_ButtonInfo.Stages )
		{
			case 1: SetStageImages( 1 ); break;
			case 2: SetStageImages( 2 ); break;
			case 3: SetStageImages( 3 ); break;
			case 4: SetStageImages( 4 ); break;
			case 5: SetStageImages( 4 ); break;
			default: // assertEx(0,"Invalid stage count");
				SetButtonImages( NULL, NULL, NULL );
		}
	else m_pParent->OnWndMessage( this, gui_msg_InitButtonImages,0,0);
}
// End gadget_Button::OnInitButtonImages()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnButtonDown() //                \author Logan Jones
//////////////////////////////////                     \date 4/21/2002
//               
//====================================================================
//
void gadget_Button::OnButtonDown()
{
	// Send down message
	SendMessage( m_pParent, gui_msg_ButtonDown, (Param_t)(LPTSTR(m_CommonData.Name)), 1 );
}
// End gadget_Button::OnButtonDown()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnButtonUp() //                  \author Logan Jones
////////////////////////////////                       \date 4/21/2002
//               
//====================================================================
//
void gadget_Button::OnButtonUp()
{
	// Send up message
	SendMessage( m_pParent, gui_msg_ButtonUp, (Param_t)(LPTSTR(m_CommonData.Name)), 1 );
}
// End gadget_Button::OnButtonUp()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Button::OnPressed() //                   \author Logan Jones
///////////////////////////////                        \date 4/20/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - Button that completed the press
//
void gadget_Button::OnPressed( DWORD dwButton )
{
	// Check if any stage changing needs to happen
	if( m_ButtonInfo.Stages )
    {
		if( dwButton==1 ) m_Stage = ((m_Stage+1) >= m_ButtonInfo.Stages) ? 0: m_Stage+1;
		else m_Stage = (m_Stage == 0) ? m_ButtonInfo.Stages-1: m_Stage-1;
    }

	// Send the pressed message
	SendMessage( m_pParent, gui_msg_ButtonPressed, (Param_t)(LPTSTR(m_CommonData.Name)), dwButton );
}
// End gadget_Button::OnPressed()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_Button.cpp //
////////////////////////////
