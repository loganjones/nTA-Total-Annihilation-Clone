// fe_SelectGame.h //                              \author Logan Jones
////////////////////                                  \date 5/13/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_SELECTGAME_H_
#define _FE_SELECTGAME_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// fe_SelectGame ////////////////////////////
//
class fe_SelectGame : public fe_Wnd
{

/////////////////////////////////////////////////
// INTERFACE
public:


/////////////////////////////////////////////////
// Data members
protected:

	class gadget_ListBox*	m_GameList;
	class gadget_Button*	m_UpdateButton;
	class gadget_Button*	m_JoinButton;
	class gadget_Button*	m_WatchButton;

	bool					m_UpdatingServerList;
	std_Time_t				m_UpdateStartTime;


/////////////////////////////////////////////////
// Event Handlers

	virtual BOOL OnCreate();
	virtual void OnDestroy();

	virtual void OnUpdate();

	virtual DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_SelectGame();
	virtual ~fe_SelectGame();
/////////////////////////////////////////////////

}; // End class - fe_SelectGame
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_SELECTGAME_H_)
