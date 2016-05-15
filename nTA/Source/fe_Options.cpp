// fe_Options.cpp //                               \author Logan Jones
///////////////////                                  \date 10/17/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_Options.h"
#include "gadget_Button.h"
#include "gadget_ListBox.h"
#include "fe_Single.h"

#define ActivateMenu( MENU, PANEL ) \
	if(m_pActiveWnd != &MENU) \
	{ \
		if(m_pActiveWnd) \
			sound.PlaySound( guiResources.Sounds.Options ), \
			m_pActiveWnd->Detach(); \
		m_pActiveWnd = &MENU; \
		m_pActiveWnd->Attach( this ); \
		m_Panel = PANEL; \
	}

#define VISUALS_API			1
#define VISUALS_RESOLUTION	2
#define VISUALS_COLORDEPTH	4

#define HAVE_VISUALS_CHANGED( CONDITION, WHAT ) \
	if( CONDITION ) m_RendererSettingsChanged |= WHAT; \
	else m_RendererSettingsChanged &= ~(WHAT)

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
fe_Options::fe_Options()
{}
fe_Options::~fe_Options()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::OnCreate() //                       \author Logan Jones
///////////////////////////                           \date 10/17/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_Options::OnCreate()
{
	BYTE*				pBuf = NULL;
	DWORD				Size;

	// Load up the custom image gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/Music.gaf", &pBuf );
	if( (pBuf!=NULL) && (Size!=0) )
	{
		gfx->CreateSurfacesFromGAF( pBuf, "CDPrev", 1, 3, m_CDPrevImages );
		gfx->CreateSurfacesFromGAF( pBuf, "CDStop", 1, 3, m_CDStopImages );
		gfx->CreateSurfacesFromGAF( pBuf, "CDPlay", 1, 3, m_CDPlayImages );
		gfx->CreateSurfacesFromGAF( pBuf, "CDNext", 1, 3, m_CDNextImages );

		// Delete the gaf buffer
		SAFE_DELETE_ARRAY( pBuf );
	}

	m_Sounds.Create( std_Point_t(), std_Size_t(), this );
	m_Sounds.LoadGui( "StartOpt" );
	m_Sounds.LoadGui( "Sounds" );
	m_Sounds.Detach();
	LoadSounds();


	m_Interface.Create( std_Point_t(), std_Size_t(), this );
	m_Interface.LoadGui( "StartOpt" );
	m_Interface.LoadGui( "Speeds" );
	m_Interface.Detach();
	LoadInterface();

	m_Music.Create( std_Point_t(), std_Size_t(), this );
	m_Music.LoadGui( "StartOpt" );
	m_Music.LoadGui( "Music" );
	m_Music.Detach();
	LoadMusic();

	m_Visuals.Create( std_Point_t(), std_Size_t(), this );
	m_Visuals.LoadGui( "StartOpt" );
	m_Visuals.LoadGui( "Visuals" );
	m_Visuals.Detach();
	LoadVisuals();

	LoadPanel("OptSound4x"); m_pSoundPanel = m_Panel;
	LoadPanel("OptInterface4x"); m_pInterfacePanel = m_Panel;
	LoadPanel("OptVisual4x"); m_pVisualPanel = m_Panel;
	LoadPanel("OptMusic4x"); m_pMusicPanel = m_Panel;

	m_pActiveWnd = NULL;
	ActivateMenu( m_Sounds, m_pSoundPanel );

	m_RendererSettingsChanged = 0;

	return fe_Wnd::OnCreate();
}
// End fe_Options::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::OnDestroy() //                      \author Logan Jones
////////////////////////////                          \date 10/17/2001
//               
//====================================================================
//
void fe_Options::OnDestroy()
{
	int		n;

	// Destroy the sub windows
	m_Sounds.Destroy();
	m_Interface.Destroy();
	m_Visuals.Destroy();
	m_Music.Destroy();
	m_pActiveWnd = NULL;

	// Destroy the panels
	SAFE_DESTROY( m_pSoundPanel );
	SAFE_DESTROY( m_pInterfacePanel );
	SAFE_DESTROY( m_pVisualPanel );
	SAFE_DESTROY( m_pMusicPanel );
	m_Panel = NULL;

	// Destroy the custom images
	for( n=0; n<3; ++n) SAFE_DESTROY( m_CDPrevImages[n] );
	for( n=0; n<3; ++n) SAFE_DESTROY( m_CDStopImages[n] );
	for( n=0; n<3; ++n) SAFE_DESTROY( m_CDPlayImages[n] );
	for( n=0; n<3; ++n) SAFE_DESTROY( m_CDNextImages[n] );

	fe_Wnd::OnDestroy();
}
// End fe_Options::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::OnUpdate() //                       \author Logan Jones
///////////////////////////                            \date 8/11/2002
//               
//====================================================================
//
void fe_Options::OnUpdate()
{
	((gadget_Button*)m_pActiveWnd->GetGadget("Prev"))->SetActive( m_RendererSettingsChanged );
}
// End fe_Options::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::OnWndMessage() //                   \author Logan Jones
///////////////////////////////                       \date 10/17/2001
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
DWORD fe_Options::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR		strSender = (LPTSTR)dwParamA;
	std_Size*	pRes = NULL;

	if( m_pActiveWnd==&m_Visuals )
	{
		if( dwMessage==gui_msg_ListBoxSelectionChanged ) STRING_SWITCH( strSender )
			CASE( "ApiList" )
				m_ResolutionBox->GetCurrentSelection( (void**)&pRes );
				SetModes( m_RendererBox->GetCurrentSelection(), 1 << (m_ColorDepthButton->GetStage() + 4), (pRes) ? *pRes : theApp.GetResolution() );
				HAVE_VISUALS_CHANGED( stricmp(m_RendererBox->GetCurrentSelection(),theApp.GetRenderer())!=0, VISUALS_API );
				return 1;
			CASE( "ModeList" )
				m_ResolutionBox->GetCurrentSelection( (void**)&pRes );
				HAVE_VISUALS_CHANGED( *pRes!=theApp.GetResolution(), VISUALS_RESOLUTION );
		END_STRING_SWITCH
		else if( dwMessage==gui_msg_ButtonPressed ) STRING_SWITCH( strSender )
			CASE( "COLORDEPTH" )
				sound.PlaySound( guiResources.Sounds.Options );
				m_ResolutionBox->GetCurrentSelection( (void**)&pRes );
				SetModes( m_RendererBox->GetCurrentSelection(), 1 << (m_ColorDepthButton->GetStage() + 4), (pRes) ? *pRes : theApp.GetResolution() );
				HAVE_VISUALS_CHANGED( (1 << (m_ColorDepthButton->GetStage() + 4))!=theApp.GetColorDepth(), VISUALS_COLORDEPTH );
				return 1;
		END_STRING_SWITCH
	}

	if( dwMessage==gui_msg_ButtonPressed )
	{
		// Check for a mode specfic button press
		/*switch( m_Mode )
		{
			case Mode_Visuals:
				STRING_SWITCH( strSender )
					CASE( "Prev" )
						return 1;
				END_STRING_SWITCH
				break;
		} // end switch( m_Mode )*/

		STRING_SWITCH( strSender )
				
			CASE( "Prev" )
				sound.PlaySound( guiResources.Sounds.Options );
				if( m_RendererSettingsChanged )
				{
					theApp.SetRenderer( m_RendererBox->GetCurrentSelection() );
					m_ResolutionBox->GetCurrentSelection( (void**)&pRes );
					theApp.SetResolution( *pRes );
					theApp.SetColorDepth( 1 << (m_ColorDepthButton->GetStage() + 4) );
					theApp.SaveRendererSettings();
					theApp.SetStartMenu( "Main" );
					theApp.Restart( app_nTA::RESTART_Subsystems );
				}
				else SWITCH_ROOTS( fe_Single );
				return 1;
				
			CASE( "Cancel" )
				sound.PlaySound( guiResources.Sounds.Previous );
				SWITCH_ROOTS( fe_Single );
				return 1;
				
			CASE( "Sound" )
				ActivateMenu( m_Sounds, m_pSoundPanel );
				return 1;
				
			CASE( "Speeds" )
				ActivateMenu( m_Interface, m_pInterfacePanel );
				return 1;
				
			CASE( "Visuals" )
				ActivateMenu( m_Visuals, m_pVisualPanel );
				return 1;
				
			CASE( "Music" )
				ActivateMenu( m_Music, m_pMusicPanel );
				return 1;
				
			CASE( "Restore" )
				sound.PlaySound( guiResources.Sounds.Options );
				return 1;
				
			CASE( "Undo" )
				sound.PlaySound( guiResources.Sounds.Options );
				return 1;

			CASE( "KillnTA" )
				sound.PlaySound( "MetlOff1" );
				theApp.DestroyNTASettings();
				SWITCH_ROOTS( fe_Single );
				return 1;


		END_STRING_SWITCH

	} // end if( dwMessage==msg_ButtonPressed )

	return fe_Wnd::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End fe_Options::OnWndMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::LoadSounds() //                     \author Logan Jones
/////////////////////////////                          \date 5/18/2002
//               
//====================================================================
//
void fe_Options::LoadSounds()
{
}
// End fe_Options::LoadSounds()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::LoadInterface() //                  \author Logan Jones
////////////////////////////////                       \date 5/18/2002
//               
//====================================================================
//
void fe_Options::LoadInterface()
{
}
// End fe_Options::LoadInterface()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::LoadVisuals() //                    \author Logan Jones
//////////////////////////////                         \date 5/18/2002
//               
//====================================================================
//
void fe_Options::LoadVisuals()
{
	fio_FindFileData	Info;
	LPVOID				hFind = fio_FindFirstFile( GFX_DIRECTORY "/*.dll", &Info );
	BOOL				bWorking = hFind!=NULL;
	LPTSTR				NameEnd;
	char				Path[32];
	HDLL				hDll;
	LPEnumDisplayMode	gfx_EnumDisplayMode;
	DWORD				Enum;
	ModeList_t*			Modes;
	RenderMode_t		Mode;

	// Save a pointer to some controls
	m_RendererBox = (gadget_ListBox*)m_Visuals.GetGadget("ApiList");
	m_ResolutionBox = (gadget_ListBox*)m_Visuals.GetGadget("ModeList");
	m_ColorDepthButton = (gadget_Button*)m_Visuals.GetGadget("ColorDepth");

	// Check if the API list box exists
	if( m_RendererBox )
	{
		// Turn off sorting
		m_RendererBox->SetSort( FALSE );

		// Iterate through each dll found
		while( bWorking )
		{
			// Look for the file extension deliminator and null it
			if( (NameEnd=strrchr(Info.Name,'.'))==NULL ) continue;
			NameEnd[0] = '\0';

			// Is this the active renderer
			if( stricmp(Info.Name,theApp.GetRenderer())==0 )
			{
				Modes = &m_Renderers[ Info.Name ];
				Modes->clear();
				for( Enum=0; (Enum=gfx->EnumDisplayMode(Enum,&Mode.Resolution,&Mode.Depth));)
					Modes->push_back( Mode );
			}
			else // Load the dll and call the exported enum function
			{
				sprintf( Path, GFX_DIRECTORY "/%s.dll", Info.Name );
				if( (hDll = sys_LoadDll( Path ))==NULL ||
					(gfx_EnumDisplayMode = (LPEnumDisplayMode)sys_RetrieveProc( hDll, "gfx_EnumDisplayMode" ))==NULL )
					continue;
				Modes = &m_Renderers[ Info.Name ];
				Modes->clear();
				for( Enum=0; (Enum=gfx_EnumDisplayMode(Enum,&Mode.Resolution,&Mode.Depth));)
					Modes->push_back( Mode );
				sys_UnloadDll( hDll );
			}

			// Add the interface name to the box and look for another
			m_RendererBox->AddListBoxItem( Info.Name );
			bWorking = fio_FindNextFile( hFind, &Info );
		}

		// Set the current color depth
		if( m_ColorDepthButton )
			m_ColorDepthButton->SetStage( (DWORD)theApp.GetColorDepth() >> 5 );

		// Set the selection and attach the slider
		m_RendererBox->SetSelection( theApp.GetRenderer() );
		SetModes( theApp.GetRenderer(), theApp.GetColorDepth(), theApp.GetResolution() );
		m_RendererBox->AttachSlider( (gadget_Slider*)m_Visuals.GetGadget("APISLDR") );
	} // end if( API box exists )
}
// End fe_Options::LoadVisuals()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::LoadMusic() //                      \author Logan Jones
////////////////////////////                           \date 5/18/2002
//               
//====================================================================
//
void fe_Options::LoadMusic()
{
	gadget_Button*	pButton;

	if( (pButton = (gadget_Button*)m_Music.GetGadget( "CDPrev" )) )
		pButton->SetButtonImages( m_CDPrevImages, m_CDPrevImages[1], m_CDPrevImages[2] );
	if( (pButton = (gadget_Button*)m_Music.GetGadget( "CDStop" )) )
		pButton->SetButtonImages( m_CDStopImages, m_CDStopImages[1], m_CDStopImages[2] );
	if( (pButton = (gadget_Button*)m_Music.GetGadget( "CDPlay" )) )
		pButton->SetButtonImages( m_CDPlayImages, m_CDPlayImages[1], m_CDPlayImages[2] );
	if( (pButton = (gadget_Button*)m_Music.GetGadget( "CDNext" )) )
		pButton->SetButtonImages( m_CDNextImages, m_CDNextImages[1], m_CDNextImages[2] );
}
// End fe_Options::LoadMusic()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Options::SetModes() //                       \author Logan Jones
///////////////////////////                             \date 8/8/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strRenderer         - 
//  long Depth                  - 
//  std_Size szDesiredSelection - 
//
void fe_Options::SetModes( LPCTSTR strRenderer, long Depth, std_Size szDesiredSelection )
{
	RenderList_t::iterator	rend;
	ModeList_t*				Modes;
	ModeList_t::iterator	it, end;
	char					ResStr[32];

	// Don't do any thing if something doesn't exist
	if( m_ResolutionBox==NULL || strRenderer==NULL ) return;

	// Detach the slider
	m_ResolutionBox->AttachSlider( NULL );
	m_ResolutionBox->ClearList();
	m_ResolutionBox->SetSort( FALSE );

	// Find the specified renderer
	rend = m_Renderers.find( strRenderer );
	if( rend!=m_Renderers.end() )
		for( Modes=&(*rend).second,it=Modes->begin(),end=Modes->end(); it!=end; ++it)
			if( (*it).Depth==Depth ) {
				sprintf( ResStr, "%ld x %ld", (*it).Resolution.width, (*it).Resolution.height );
				m_ResolutionBox->AddListBoxItem( ResStr, &(*it) );
	}

	// Set the current selection
	sprintf( ResStr, "%ld x %ld", szDesiredSelection.width, szDesiredSelection.height );
	if( m_ResolutionBox->SetSelection( ResStr )==(-1) )
		// Failed to find a suitable selection, look for a close match		
		if( rend!=m_Renderers.end() )
			for( Modes=&(*rend).second,it=Modes->begin(),end=Modes->end(); it!=end; ++it)
				if( (*it).Depth==Depth && (*it).Resolution.width>=szDesiredSelection.width && (*it).Resolution.height>=szDesiredSelection.height ) {
					sprintf( ResStr, "%ld x %ld", (*it).Resolution.width, (*it).Resolution.height );
					m_ResolutionBox->SetSelection( ResStr );
					break;
		}

	// Re-attach the slider
	m_ResolutionBox->AttachSlider( (gadget_Slider*)m_Visuals.GetGadget("MODESLDR") );
}
// End fe_Options::SetModes()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_Options.cpp //
/////////////////////////
