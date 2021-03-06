// igui_Menu.h //                                  \author Logan Jones
////////////////                                      \date 4/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_MENU_H_
#define _IGUI_MENU_H_
/////////////////////////////////////////////////////////////////////


////////////////////////////// igui_Menu //////////////////////////////
//
class igui_Menu : public gui_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL LoadPanel( LPCTSTR strPanel );

	void Render();


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();


/////////////////////////////////////////////////
// Utility

	void CreateButton( ta_gui_Common_t& Common, ta_gui_Button& Info );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_Menu();
	virtual ~igui_Menu();
/////////////////////////////////////////////////

}; // End class - igui_Menu
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_MENU_H_)
