// igui_Menu.cpp //                                \author Logan Jones
//////////////////                                    \date 4/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_Menu.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_Menu::igui_Menu():gui_Window()
{}
igui_Menu::~igui_Menu()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Menu::Render() //                          \author Logan Jones
////////////////////////                               \date 4/23/2002
//               
//====================================================================
//
void igui_Menu::Render()
{
	// Assume this window is created
	assert( m_bCreated );

	// Window must be visible to render
	if( m_bVisible )
	{
		// Call the render event
		OnRender();

		// Render the children
		for_each( m_Children.begin(), m_Children.end(), RenderChild );
	}
}
// End igui_Menu::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Menu::LoadPanel() //                       \author Logan Jones
///////////////////////////                            \date 4/23/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strPanel - 
//
// Return: BOOL - 
//
BOOL igui_Menu::LoadPanel( LPCTSTR strPanel )
{
	return ((igui_ControlBar*)m_pParent)->LoadPanel( strPanel, &m_Panel );
}
// End igui_Menu::LoadPanel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Menu::OnCreate() //                        \author Logan Jones
//////////////////////////                             \date 4/27/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL igui_Menu::OnCreate()
{
	// Remove the menu from the parent.
	Detach();

	return gui_Window::OnCreate();
}
// End igui_Menu::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_Menu::CreateButton() //                    \author Logan Jones
//////////////////////////////                         \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Button& Info     - 
//
void igui_Menu::CreateButton( ta_gui_Common_t& Common, ta_gui_Button& Info )
{
	gadget_Button* pButton;

	// Could this be a build button
    if( Common.Attribs & GADGET_IS_BUILD_BUTTON ) {

		// It could still be a empty spot, i.e. a patch
		if( stricmp(Common.Name,"IGPATCH")==0 )
			pButton = new igui_OrderButton;
		else // Probably a build button
			pButton = new igui_BuildButton;
    }
	// How about an order button
    else if( Common.Attribs & GADGET_IS_ORDER_BUTTON ) {
		pButton = new igui_OrderButton;
    }
	// No... It could be a prev/next or stop button
	else if( stricmp("Prev",Common.Name+3)==0 ||
			 stricmp("Next",Common.Name+3)==0 ||
             stricmp("Stop",Common.Name+3)==0 ) {
        pButton = new gadget_Button;
		strcpy( Common.Name, Common.Name + 3 );
    }
    else {// I don't know what it is
			pButton = new gadget_Button;
    }

	// Create the button and add it to the list
	if( pButton->Create(&Common,&Info,this) )
		m_Gadgets.push_back( pButton );
}
// End igui_Menu::CreateButton()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_Menu.cpp //
////////////////////////
