// fe_GuiManager.h //                              \author Logan Jones
////////////////////                                  \date 10/2/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_GUIMANAGER_H_
#define _FE_GUIMANAGER_H_
/////////////////////////////////////////////////////////////////////


#define SWITCH_ROOTS( NEW_ROOT_CLASS ) \
	((NEW_ROOT_CLASS*)((fe_GuiManager*)m_pWndManager)->SwitchRoots( new NEW_ROOT_CLASS, this ))


//////////////////////////// fe_GuiManager ////////////////////////////
//
class fe_GuiManager : public wnd_Manager
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void OnFrame();

	BOOL Create( LPTSTR strStartMenu=NULL );
	void Destroy();

	void Update();
	void Render();

	//BOOL HandleInput( in_Devices inDevice, DWORD dwID, long lData );

	BOOL QuickStart( LPCTSTR strGameType );

	fe_Wnd* SwitchRoots( fe_Wnd* pNew, fe_Wnd* pOld );


/////////////////////////////////////////////////
// Menus
protected:

	fe_Wnd*				m_CurrentRoot;
	fe_Wnd*				m_OldRoot;


/////////////////////////////////////////////////
// Data members
protected:

	DWORD				m_UpdateTime;

	char				m_strFPS[32];
	DWORD				m_FrameCount;
	float				m_FrameTime;

	LPSOUND				m_AmbientSond;


/////////////////////////////////////////////////
// Internal methods
protected:


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_GuiManager();
	virtual ~fe_GuiManager();
/////////////////////////////////////////////////

}; // End class - fe_GuiManager
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_GUIMANAGER_H_)
