// game_UserInterface.h //                         \author Logan Jones
/////////////////////////                              \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_USERINTERFACE_H_
#define _GAME_USERINTERFACE_H_
/////////////////////////////////////////////////////////////////////


// Gfx Surfaces available to the interface
// These surfaces are retrivable using the Surface() method
// of game_UserInterface
enum InterfaceSurfaces_t {
	TopPanel,
	BottomPanel,
	SidePanel,
	SidePanelLower,
	INTERFACE_SURFACE_COUNT,
};


// Misc types used in game_UserInterface definition
struct SideData_t;
class player_Local;
class gfx_Font;
class gfx_Surface;


/////////////////////////// game_UserInterface ///////////////////////////
//
class game_UserInterface : public wnd_Manager
{
/////////////////////////////////////////////////
// Types
public:

	typedef map< long, string >						DownloadMenuPage_t;
	typedef vector< DownloadMenuPage_t >			DownloadMenuPages_t;
	typedef hash_map< DWORD, DownloadMenuPages_t >	DownloadMenus_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	// Creation and destruction of the interface
	BOOL Create();
	BOOL Initialize( player_Local* pPlayer, std_Point StartPosition, SideData_t* pSideData );
	void Destroy();

	INLINE void AddDownload( DWORD dwBuilder, long lPage, long lButton, LPCTSTR strBuildee );
	INLINE void UnitLoadComplete();

	// Creates the build menus to be associated with the given unit type
	BOOL CreateBuildMenu( unit_Type& UnitType );

	// Handle a build request
	void BuildButtonPressed( LPCTSTR strTypeName, bool bWeapon, bool bAdd );

	// Deastroy the given build oreders
	void KillBuildOrders( igui_BuildOrders* pOrders );

	// Updates the inerface and its associations
	void Update();

	// Renders everything to the screen
	void Render();

	// Inherited input handler to proccess input or pass it to the base class
	BOOL HandleInput( in_Devices inDevice, DWORD dwID, long lData );

	player_Local* User() {return m_Player;}
	const std_Size& GuiResolution() const {return m_GuiResolution;}

	igui_GameWindow& GameWindow() {return m_GameWindow;}
	igui_ControlBar& ControlBar() {return m_ControlBar;}
	igui_ResourceBar& ResourceBar() {return m_ResourceBar;}
	igui_InfoBar& InfoBar() {return m_InfoBar;}

	gfx_Font* Font() const {return m_Font;}
	gfx_Font* GuiFont() const {return m_GuiFont;}
	gfx_Font* ConsoleFont() const {return m_ConsoleFont;}
	gfx_Surface* Surface( InterfaceSurfaces_t iWhat ) const {return m_Surfaces[iWhat];}

	bool ShiftKeyDown() const {return m_ShiftKeyDown>0;}
	bool CtrlKeyDown() const {return m_CtrlKeyDown>0;}


/////////////////////////////////////////////////
// Data members
protected:

	player_Local*			m_Player;

	// The resoltuion to render the gui stuff at
	std_Size				m_GuiResolution;

	// Sub-windows
	igui_GameWindow			m_GameWindow;
	igui_ControlBar			m_ControlBar;
	igui_ResourceBar		m_ResourceBar;
	igui_InfoBar			m_InfoBar;
	igui_MiniMap			m_MiniMap;
	igui_Console			m_Console;

	gfx_Font*				m_Font;
	gfx_Font*				m_GuiFont;
	gfx_Font*				m_ConsoleFont;

	// Surfaces used for the gui
	gfx_Surface*			m_Surfaces[ INTERFACE_SURFACE_COUNT ];

	DownloadMenus_t			m_Downloads;

	int						m_ShiftKeyDown;
	int						m_CtrlKeyDown;

	list<igui_BuildOrders>	m_BuildOrders;


/////////////////////////////////////////////////
// Utility methods
protected:


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	game_UserInterface();
	virtual ~game_UserInterface();
/////////////////////////////////////////////////

}; // End class - game_UserInterface
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "game_UserInterface.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_USERINTERFACE_H_)
