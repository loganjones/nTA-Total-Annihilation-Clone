// igui_ControlBar.h //                            \author Logan Jones
//////////////////////                                 \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_CONTROLBAR_H_
#define _IGUI_CONTROLBAR_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// igui_ControlBar //////////////////////////
//
class igui_ControlBar : public wnd_Window
{
	friend class game_UserInterface;

	typedef list< string >		MenuModsToLoad_t;

	struct Surface_t { gfx_Surface* Surfaces[6]; };
	typedef unordered_map< string, Surface_t >		SurfaceMap_t;

	typedef map< string, igui_Menu >		MenuMap_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	void ResetOrderSelection();

	void Destroy();


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();
	void OnDestroy();

	void OnRender();

	void OnCursorMove( std_Point& ptCursor, DWORD dwFlags );

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );

	DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB );


/////////////////////////////////////////////////
// Utility
public:

	BOOL SetSelection( class unit_Object* pUnit );
	BOOL SetMenu( DWORD dwMenu );

	void ResetPressed();

	// Called by the menus to load their background panel
	BOOL LoadPanel( LPCTSTR strPanel, gfx_Surface** ppPanel );

	BOOL CreateButton( gadget_Button** ppButton, ta_gui_Common_t& Common, ta_gui_Button& Info, igui_Menu* pParent );
	BOOL LoadOrderButton( igui_OrderButton* pButton, ta_gui_Common_t& Common, ta_gui_Button& Info );
	BOOL LoadBuildButton( igui_BuildButton* pButton, ta_gui_Common_t& Common, ta_gui_Button& Info );

	INLINE void SetButtonImages( gadget_Button* pButton );

	void SetBuildMenuCommands( igui_Menu* pMenu, const struct unit_Type& UnitType );
	void SetOrderMenuCommands( igui_Menu* pMenu, const struct unit_Type& UnitType );


/////////////////////////////////////////////////
// Data members
public:

	MenuModsToLoad_t		m_MenuModsToLoad;
	BYTE*					m_pFileBuffer;
	LPTSTR					m_KeyPrefix;

	SurfaceMap_t			m_Surfaces;
	MenuMap_t				m_UnitMenus;

	unit_Object*			m_CurrentSelection;

	igui_Menu				m_OrdersMenu;
	igui_Menu*				m_CurrentMenu;
	DWORD					m_CurrentPage;
	igui_OrderButton*		m_Pressed;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_ControlBar();
	virtual ~igui_ControlBar();
/////////////////////////////////////////////////

}; // End class - igui_ControlBar
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "igui_ControlBar.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_CONTROLBAR_H_)
