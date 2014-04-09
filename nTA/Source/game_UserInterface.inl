// game_UserInterface.inl //                       \author Logan Jones
///////////////////////////                            \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::AddDownload() //            \author Logan Jones
//////////////////////////////////////                 \date 8/19/2002
//               
//====================================================================
// Parameters:
//  DWORD dwBuilder    - 
//  long lPage         - 
//  long lButton       - 
//  LPCTSTR strBuildee - 
//
void game_UserInterface::AddDownload( DWORD dwBuilder, long lPage, long lButton, LPCTSTR strBuildee )
{	
	game_UserInterface::DownloadMenuPages_t*	pPages;
	game_UserInterface::DownloadMenuPage_t*		pPage;

	// Make sure this entry is valid
	if( dwBuilder==0 || lPage==0 || strBuildee[0]=='\0' ) return;

	// Add the entry
	--lPage;
	pPages = &m_Downloads[ dwBuilder ];
	if( pPages->size()<lPage ) pPages->resize( lPage );
	pPage = &((*pPages)[lPage-1]);
	(*pPage)[lButton] = strBuildee;
}
// End game_UserInterface::AddDownload()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_UserInterface::UnitLoadComplete() //Author: Logan "Burn" Jones
///////////////////////////////////////////            \date 8/21/2002
//               
//====================================================================
//
void game_UserInterface::UnitLoadComplete()
{
	// Clear out the downloads
	m_Downloads.clear();
}
// End game_UserInterface::UnitLoadComplete()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_UserInterface.inl //
/////////////////////////////////