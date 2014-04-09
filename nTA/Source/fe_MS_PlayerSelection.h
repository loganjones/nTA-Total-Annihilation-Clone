// fe_MS_PlayerSelection.h //                      \author Logan Jones
////////////////////////////                         \date 05-06-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_MS_PLAYERSELECTION_H_
#define _FE_MS_PLAYERSELECTION_H_
/////////////////////////////////////////////////////////////////////


/////////////////////// fe_MS_PlayerSelection ///////////////////////
//
class fe_MS_PlayerSelection
{
	friend class fe_MultiplayerSetup;

/////////////////////////////////////////////////
// INTERFACE
public:

	bool Create( const char* strName, UINT32 uiIndex, UINT32 uiNetID, const char* strSide, UINT8 uiColor, fe_MultiplayerSetup* pParent );

	void Destroy( fe_MultiplayerSetup* pParent );

	void Send( UINT32 uiTo, UINT32 uiIndex, UINT32 uiPrefix=0 );


/////////////////////////////////////////////////
// data
private:

	char						Name[32];
	UINT32						NetID;
	SideList_t::const_iterator	Side;
	UINT8						Color;

	gadget_String*				NameDisp;
	gadget_Button*				SideSel;
	gadget_Pic*					ColorSel;
	gadget_Button*				Ready;

	DWORD						Valid;

/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_MS_PlayerSelection(){}
	virtual ~fe_MS_PlayerSelection(){}
/////////////////////////////////////////////////

}; // End class - fe_MS_PlayerSelection
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_MS_PLAYERSELECTION_H_)
