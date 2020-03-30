// igui_BuildButton.cpp //                         \author Logan Jones
/////////////////////////                             \date 4/25/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_BuildButton.h"
#include "game.h"
#include "snd.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_BuildButton::igui_BuildButton():gadget_Button()
{}
igui_BuildButton::~igui_BuildButton()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_BuildButton::OnInitButtonImages() //       \author Logan Jones
///////////////////////////////////////////           \date 11/5/2002
//
//===================================================================
//
void igui_BuildButton::OnInitButtonImages()
{
	theInterface->ControlBar().LoadBuildButton( this, m_CommonData, m_ButtonInfo );
}
// End igui_BuildButton::OnInitButtonImages()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_BuildButton::OnRender() //                 \author Logan Jones
/////////////////////////////////                    \date 02-02-2003
//
//===================================================================
//
void igui_BuildButton::OnRender()
{
	const igui_BuildOrders*	pOrders = theInterface->ControlBar().m_CurrentSelection->BuildOrders();
	int						Amount;

	// Render the button first
	gadget_Button::OnRender();

	// If the selected unit has build orders and those oreders include this item then render the amount info
	if( pOrders && (Amount=pOrders->Amount(m_CommonData.Name))!=0 ) {

		// Setup the info string depending on the amount (less than 0 indicates infinite)
        if( Amount>0 ) {
            m_BuildText[0] = '+';
            itoa( Amount, m_BuildText+1, 10 );
        }
        else {
            m_BuildText[0] = '~';
            m_BuildText[1] = '\0';
        }

		// Render the info
		gfx->SetCurrentFont( guiResources.Fonts.Standard );
		gfx->RenderStringCenteredAt(
			m_ScreenPosition + std_Point(m_Size.width/2, m_Size.height-6) + (m_Pressed ? std_Point(1,1) : std_Point(0,0)),
			m_BuildText,
			TRUE, FALSE );
	}
}
// End igui_BuildButton::OnRender()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_BuildButton::OnCursorMove() //             \author Logan Jones
/////////////////////////////////////                   \date 5/1/2002
//               
//====================================================================
// Parameters:
//  std_Point& ptCursor - 
//  DWORD dwFlags       - 
//
void igui_BuildButton::OnCursorMove( std_Point& ptCursor, DWORD dwFlags )
{
	// Call the default
	gadget_Button::OnCursorMove( ptCursor, dwFlags );

	// Display the type information for the unit represented by this button
	theInterface->InfoBar().DisplayBuildInfo( theGame.Units.GetUnitType(m_CommonData.Name) );
}
// End igui_BuildButton::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_BuildButton::OnPressed() //                \author Logan Jones
//////////////////////////////////                     \date 4/27/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - Button that completed the press
//
void igui_BuildButton::OnPressed( DWORD dwButton )
{
	// Send the pressed message
	//SendMessage( m_pParent, igui_msg_BuildButtonPressed, (DWORD)(LPTSTR(m_CommonData.Name)), dwButton );
	sound.PlaySound( (dwButton==1) ? guiResources.Sounds.AddBuild : guiResources.Sounds.SubBuild );
	theInterface->BuildButtonPressed( m_CommonData.Name, (m_CommonData.CommonAttribs&8)!=0, dwButton==1 );
}
// End igui_BuildButton::OnPressed()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_BuildButton.cpp //
///////////////////////////////
