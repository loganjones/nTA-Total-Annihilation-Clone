// igui_GameWindow.h //                            \author Logan Jones
//////////////////////                                 \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_GAMEWINDOW_H_
#define _IGUI_GAMEWINDOW_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// igui_GameWindow //////////////////////////
//
class igui_GameWindow : public wnd_Window
{
	friend class game_UserInterface;

/////////////////////////////////////////////////
// Types
public:

	enum InterafceModes_t : short;
	enum InterafceOrder_t : short;


/////////////////////////////////////////////////
// INTERFACE
public:

	void Update();

	void Render();
	void RenderGame();

	void DoMode( InterafceModes_t Mode );
	void DoBuildMode( const DWORD& dwBuildWhat );
	void DoOrder( InterafceOrder_t Order, DWORD dwParam=0 );

	BOOL OnCreate();
	void OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );
	void OnMouseWheel( const std_Point_t& ptCursor, long lData, DWORD dwFlags );
	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility
protected:

	void CenterBuildRect( const std_Point& ptCursorOnGrid );

	void DoRectOrder();
	void DoPointOrder( const std_Point& ptCursor );

	void SingleSelection( class unit_Interface* pUnit );
	void MultiSelection();


/////////////////////////////////////////////////
// Data members
protected:

	std_Size					m_TerrainRes;
//	std_Size					m_NewTerrainRes;

	InterafceModes_t			m_InterfaceMode;

	gfx_Cursor*					m_CurrentCursor;

	std_RectF					m_BuildRect;
	BYTE						m_BuildHeight;
	const struct unit_Type*		m_BuildType;
//	DWORD						m_BuildType;
	BOOL						m_BuildInfoHasBeenSet;
	BOOL						m_CanBuild;
	const DWORD					m_BuildRect_CanBuildColor;
	const DWORD					m_BuildRect_CanNotBuildColor;

	BOOL						m_MButton1Down;
	std_Point					m_MButton1DownAt;
	std_Time_t					m_MButton1DownTime;
	const std_Time_t			m_MButton1HoldInterval;
	std_Rect					m_SelectionRect;
	BOOL						m_SelectionRectSet;
	const DWORD					m_SelectionRect_OuterColor;
	const DWORD					m_SelectionRect_InnerColor;

	class unit_Interface*		m_CurrentSelection;

	std_Time_t					m_LastSelectionTime;
	const std_Time_t			m_LastSelectionInterval;


/////////////////////////////////////////////////
// Type definitions
public:

	enum InterafceModes_t : short
	{
		MODE_Select,		// Nothing is selected and we are searching for something to select
		MODE_Default,		// Something is selected, but we haven't been told to do anything specifically
		MODE_Build,			// Search for somewhere to build
		MODE_Move,			// Search for somewhere to move
		MODE_Attack,		// Search for somewhere/something to attack
		MODE_Defend,		// Search for somewhere/something to defend
		MODE_Patrol,		// Search for somewhere to patrol
		MODE_Blast,
		MODE_Capture,
		MODE_Reclaim,
		MODE_Repair,
		MODE_Load,
		MODE_Unload,
	};

	enum InterafceOrder_t : short
	{
		ORDER_Stop,			//  Commands the selection to stop
		ORDER_FireOrd,
		ORDER_MoveOrd,
		ORDER_OnOff,
		ORDER_Cloak,
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_GameWindow();
	virtual ~igui_GameWindow();
/////////////////////////////////////////////////

}; // End class - igui_GameWindow
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_GAMEWINDOW_H_)
