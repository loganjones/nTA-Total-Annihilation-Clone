// fe_SelectMap.h //                               \author Logan Jones
///////////////////                                   \date 7/30/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_SELECTMAP_H_
#define _FE_SELECTMAP_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// fe_SelectMap ////////////////////////////
//
class fe_SelectMap : public fe_Wnd
{

/////////////////////////////////////////////////
// INTERFACE
public:


/////////////////////////////////////////////////
// Data members
protected:

//	gfx_Surface*	m_Surfaces[ 5 ];
	class gadget_ListBox*			m_MapBox;
	app_FileTree::HFIND				m_hFindMaps;
	app_FileTree::FindData_t		m_MapFindInfo;


/////////////////////////////////////////////////
// Event Handlers

	virtual BOOL OnCreate();
	virtual void OnDestroy();

	virtual void OnUpdate();

	virtual DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB );


/////////////////////////////////////////////////
// Utility

	void CheckMap( LPTSTR strName, LPVOID lpOtaFile, class gadget_ListBox* pBox );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_SelectMap();
	virtual ~fe_SelectMap();
/////////////////////////////////////////////////

}; // End class - fe_SelectMap
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_SELECTMAP_H_)
