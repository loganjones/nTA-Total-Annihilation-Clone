// fe_MultiplayerSetup.h //                        \author Logan Jones
//////////////////////////                            \date 7/31/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_MULTIPLAYERSETUP_H_
#define _FE_MULTIPLAYERSETUP_H_
/////////////////////////////////////////////////////////////////////


//////////////////////// fe_MultiplayerSetup ////////////////////////
//
class fe_MultiplayerSetup : public fe_Wnd
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Init( const char* strGameName, const char* strPlayerName, const char* strPassword );

	gfx_Surface** Logos() {return m_Logos;}

	UINT8 GetColor( UINT8 uiStart=0, bool bReplaceStart=false, bool bFoward=true );

	std_Point OpenSlot();

	void Replace( UINT8 uiColor, std_Point ptSlot );


/////////////////////////////////////////////////
// Event Handlers

	virtual BOOL OnCreate();
	virtual void OnDestroy();

	virtual void OnUpdate();

	virtual DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB );


/////////////////////////////////////////////////
// data members
private:

	char				m_GameName[64];
	char				m_Password[32];

	gfx_Surface*		m_Logos[10];

	gfx_Surface*		m_BattleStartAnim[9];

	WORD				m_ColorsAvailable;

	list<std_Point>		m_SlotsOpen;
	std_Point			m_FirstPos;


/////////////////////////////////////////////////
// utility methods

	void LoadSettings();

	void ResetPlayers();

	void DestroyPlayers();

	void DestroyPlayer( UINT32 uiWho );

	void RemovePlayers( UINT32 uiNetID );

	void OnSystemMessage();

	void ResyncClient( UINT32 uiWho );

	void OnClientLogin();

	void OnResync();

	void OnNewPlayer();

	void SaveSettings();


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_MultiplayerSetup();
	virtual ~fe_MultiplayerSetup();
/////////////////////////////////////////////////

}; // End class - fe_MultiplayerSetup
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_MULTIPLAYERSETUP_H_)
