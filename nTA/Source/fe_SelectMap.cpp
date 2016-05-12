// fe_SelectMap.cpp //                             \author Logan Jones
/////////////////////                                 \date 7/30/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_SelectMap.h"
#include "gadget_ListBox.h"
#include "ta_OTA.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_SelectMap::fe_SelectMap():fe_Wnd()
{}
fe_SelectMap::~fe_SelectMap()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectMap::OnCreate() //                     \author Logan Jones
/////////////////////////////                          \date 7/30/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_SelectMap::OnCreate()
{
//	gadget_ListBox*					pBox;
//	app_FileTree::HFIND				hFind;
//	app_FileTree::FindData_t		FindInfo;	
//	BOOL							bWorkingMapFiles;

	// Load the gui file
	if( bFAILED(LoadGui("SelMap")) )
	{
		return FALSE;
	}

	// Load the background
	if( bFAILED(LoadPanel("DSelectMap2")) )
	{
		return FALSE;
	}

	m_MapBox = (gadget_ListBox*)GetGadget( "MapNames" );
	m_hFindMaps = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "ota", "maps", &m_MapFindInfo );
	if( m_hFindMaps ) CheckMap( m_MapFindInfo.Name, m_MapFindInfo.hFile, m_MapBox );
/*
	bWorkingMapFiles = hFind!=NULL;
	while( bWorkingMapFiles )
	{
		CheckMap( FindInfo.Name, FindInfo.hFile, pBox );

		// Find next file
		bWorkingMapFiles = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( ota files )
	theFileTree.CloseFind( hFind );
*/
	m_MapBox->AttachSlider( (gadget_Slider*)GetGadget("Slider") );
	m_MapBox->MoveToTop();

	// Deactivate the parent and return the result of the default
//	m_pParent->SetActive( FALSE );
	return fe_Wnd::OnCreate();
}
// End fe_SelectMap::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectMap::OnDestroy() //                    \author Logan Jones
//////////////////////////////                         \date 7/30/2002
//               
//====================================================================
//
void fe_SelectMap::OnDestroy()
{
	// Reactivate the parent and call the default
//	m_pParent->SetActive( TRUE );
	fe_Wnd::OnDestroy();
}
// End fe_SelectMap::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectMap::OnUpdate() //                     \author Logan Jones
/////////////////////////////                          \date 8/16/2002
//               
//====================================================================
//
void fe_SelectMap::OnUpdate()
{
	std_Time_t		time = std_Time();

	while( m_hFindMaps && (std_Time() - time)<50 )
	{
		if( theFileTree.ContinueFind(m_hFindMaps,&m_MapFindInfo)==FALSE )
			m_hFindMaps = NULL;
		else CheckMap( m_MapFindInfo.Name, m_MapFindInfo.hFile, m_MapBox );
	}
}
// End fe_SelectMap::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectMap::OnWndMessage() //                 \author Logan Jones
/////////////////////////////////                      \date 7/30/2002
//               
//====================================================================
// Parameters:
//  wnd_Window* pSender - 
//  DWORD dwMessage     - 
//  DWORD dwParamA      - 
//  DWORD dwParamB      - 
//
// Return: DWORD - 
//
DWORD fe_SelectMap::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;

	if( dwMessage==gui_msg_ButtonPressed )
		STRING_SWITCH( strSender )

			CASE( "Load" )
				m_pParent->OnWndMessage( this, gui_msg_ButtonPressed, (DWORD)(m_MapBox->GetCurrentSelection()), 0 );
				sound.PlaySound( "Button1" );
				Destroy();
				return 1;

			CASE( "PrevMenu" )
				sound.PlaySound( guiResources.Sounds.Previous );
				Destroy();
				return 1;

		END_STRING_SWITCH
	// end if( dwMessage==msg_ButtonPressed )

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_SelectMap::OnWndMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_SelectMap::CheckMap() //                     \author Logan Jones
/////////////////////////////                          \date 7/30/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strName       - 
//  LPVOID lpOtaFile     - 
//  gadget_ListBox* pBox - 
//
void fe_SelectMap::CheckMap( LPTSTR strName, LPVOID lpOtaFile, gadget_ListBox* pBox )
{
	BYTE*					pOta = NULL;
	DWORD					FileSize;
	ta_ota_Schema_t			Schema;
	ta_ota_GlobalHeader_t	Header;
	char					Path[32];
	//BYTE*					pTnt = NULL;

	FileSize = theFileTree.DumpFileToBuffer( (app_FileTree::HFILEENTRY)lpOtaFile, &pOta );
	if( ta_ota_GetSchema( pOta, FileSize, "Network 1", &Schema, &Header ) )
	{
		sprintf( Path, "maps/%s.tnt", Header.MissionName );
		if( theFileTree.OpenFile(Path) )
			pBox->AddListBoxItem( Header.MissionName );
	//	FileSize = theFileTree.DumpFileToBuffer( Path, &pTnt );
	//	if(FileSize) pBox->AddListBoxItem( Header.MissionName );
	//	SAFE_DELETE_ARRAY( pTnt );
	}
//	if(FileSize) pBox->AddListBoxItem( strName );

	SAFE_DELETE_ARRAY( pOta );
}
// End fe_SelectMap::CheckMap()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_SelectMap.cpp //
///////////////////////////