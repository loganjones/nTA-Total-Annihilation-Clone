// gui_Resources.cpp //                            \author Logan Jones
//////////////////////                                \date 4/21/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#define INSTANTIATE_GUI_RESOURCES_INTERFACE
#include "gui.h"
#include "gui_Resources.h"
#include "util_TxtParse.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gui_Resources::gui_Resources():
	m_bCreated( FALSE ),
	CommonGuiBuffer( NULL )
{}
gui_Resources::~gui_Resources()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


#define CreateStandardButton( BUTTON, START ) \
{ \
	(BUTTON).StageCount = 1; \
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "Buttons0", (START), &(BUTTON).Stages[0] ); \
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "Buttons0", (START) + 1, &(BUTTON).Pressed ); \
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "Buttons0", (START) + 2, &(BUTTON).Disabled ); \
}
#define CreateStageButton( STAGES ) \
{ \
	StageButton_##STAGES.StageCount = ((STAGES)==1) ? (STAGES)+1 : (STAGES); \
	gfx->CreateSurfacesFromGAF(CommonGuiBuffer, "Stagebuttn" #STAGES, 1, StageButton_##STAGES.StageCount + 1, &StageButton_##STAGES.Stages[0] ); \
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "Stagebuttn" #STAGES, (StageButton_##STAGES.StageCount + 2), &StageButton_##STAGES.Pressed ); \
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "Stagebuttn" #STAGES, (StageButton_##STAGES.StageCount + 3), &StageButton_##STAGES.Disabled ); \
}


//////////////////////////////////////////////////////////////////////
// gui_Resources::Create() //                      \author Logan Jones
////////////////////////////                           \date 4/21/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gui_Resources::Create()
{
	DWORD Size;

	// Load the common gui buffer
	if( CommonGuiBuffer==NULL )
	{
		Size = theFileTree.DumpFileToBuffer( "anims/commongui.gaf", &CommonGuiBuffer );
		if( (CommonGuiBuffer==NULL) || (Size==0) )
		{
			return FALSE;
		}
	}

	if( m_bCreated ) return TRUE;

	CreateStandardButton( StandardButton_16x16,	1 );
	CreateStandardButton( StandardButton_321x20, 5 );
	CreateStandardButton( StandardButton_120x20, 9 );
	CreateStandardButton( StandardButton_96x20, 13 );
	CreateStandardButton( StandardButton_112x20, 17 );
	CreateStandardButton( StandardButton_80x20, 21 );
	CreateStandardButton( StandardButton_96x31, 25 );
	CreateStageButton( 1 );
	CreateStageButton( 2 );
	CreateStageButton( 3 );
	CreateStageButton( 4 );

	Checkbox.StageCount = 2;
	gfx->CreateSurfacesFromGAF(CommonGuiBuffer, "CHECKBOX", 1, 2, &Checkbox.Stages[0] );
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "CHECKBOX", 3, &Checkbox.Disabled );
	gfx->CreateSurfaceFromGAF( CommonGuiBuffer, "CHECKBOX", 4, &Checkbox.Pressed );

	gfx->CreateSurfacesFromGAF( CommonGuiBuffer, "Sliders", 7, 2, m_SliderButtonUp );
	gfx->CreateSurfacesFromGAF( CommonGuiBuffer, "Sliders", 9, 2, m_SliderButtonDown );
	gfx->CreateSurfacesFromGAF( CommonGuiBuffer, "Sliders", 17, 2, m_SliderButtonLeft );
	gfx->CreateSurfacesFromGAF( CommonGuiBuffer, "Sliders", 19, 2, m_SliderButtonRight );

	gfx_Interface::RetrieveImagesFromGAF( CommonGuiBuffer, "Sliders", 1, 3, m_VerticalStrip );
	gfx_Interface::RetrieveImagesFromGAF( CommonGuiBuffer, "Sliders", 11, 3, m_HorizontalStrip );

	gfx_Interface::RetrieveImageFromGAF( CommonGuiBuffer, "Sliders", 14, &m_StadardTicker );

	gfx_Image_t		Temp;
	gfx_Interface::RetrieveImagesFromGAF( CommonGuiBuffer, "Sliders", 4, 3, m_VerticalTicker );

	Temp = m_VerticalTicker[0];
	m_VerticalTicker[0].Size = std_Size_t( Temp.Size.width, Temp.Size.height + m_VerticalTicker[1].Size.height );
	m_VerticalTicker[0].Stride = Temp.Stride;
	m_VerticalTicker[0].Pitch = m_VerticalTicker[0].Size.width * m_VerticalTicker[0].Stride;
	m_VerticalTicker[0].pBytes = new BYTE[ *m_VerticalTicker[0].Size ];
	gfx_Interface::BitBlt( std_Point_t(0,0), &m_VerticalTicker[0], &Temp );
	gfx_Interface::BitBlt( std_Point_t(0,Temp.Size.height), &m_VerticalTicker[0], &m_VerticalTicker[1] );
	delete [] Temp.pBytes;
	Temp = m_VerticalTicker[2];
	m_VerticalTicker[2].Size = std_Size_t( Temp.Size.width, Temp.Size.height + m_VerticalTicker[1].Size.height );
	m_VerticalTicker[2].Stride = Temp.Stride;
	m_VerticalTicker[2].Pitch = m_VerticalTicker[2].Size.width * m_VerticalTicker[2].Stride;
	m_VerticalTicker[2].pBytes = new BYTE[ *m_VerticalTicker[2].Size ];
	gfx_Interface::BitBlt( std_Point_t(0,0), &m_VerticalTicker[2], &m_VerticalTicker[1] );
	gfx_Interface::BitBlt( std_Point_t(0,m_VerticalTicker[1].Size.height), &m_VerticalTicker[2], &Temp );
	delete [] Temp.pBytes;

	std_Rect_t		CopyRect;
	m_HorizontalTicker[0].Size.Set( (m_StadardTicker.Size.width / 3) + 1, m_StadardTicker.Size.height );
	m_HorizontalTicker[0].Stride = m_StadardTicker.Stride;
	m_HorizontalTicker[0].Pitch = m_HorizontalTicker[0].Size.width * m_HorizontalTicker[0].Stride;
	m_HorizontalTicker[0].pBytes = new BYTE[ *m_HorizontalTicker[0].Size ];
	CopyRect.Set( std_Point_t(0,0), m_HorizontalTicker[0].Size );
	gfx_Interface::BitBlt( std_Point_t(0,0), &m_HorizontalTicker[0], &m_StadardTicker, &CopyRect );
	m_HorizontalTicker[1].Size.Set( m_StadardTicker.Size.width / 3, m_StadardTicker.Size.height );
	m_HorizontalTicker[1].Stride = m_StadardTicker.Stride;
	m_HorizontalTicker[1].Pitch = m_HorizontalTicker[1].Size.width * m_HorizontalTicker[1].Stride;
	m_HorizontalTicker[1].pBytes = new BYTE[ *m_HorizontalTicker[1].Size ];
	CopyRect.Set( std_Point_t(m_HorizontalTicker[0].Size.width,0), m_HorizontalTicker[1].Size );
	gfx_Interface::BitBlt( std_Point_t(0,0), &m_HorizontalTicker[1], &m_StadardTicker, &CopyRect );
	m_HorizontalTicker[2] = m_HorizontalTicker[0];
	m_HorizontalTicker[2].pBytes = new BYTE[ *m_HorizontalTicker[2].Size ];
	CopyRect.Set( std_Point_t(m_StadardTicker.Size.width - m_HorizontalTicker[2].Size.width,0), m_HorizontalTicker[2].Size );
	gfx_Interface::BitBlt( std_Point_t(0,0), &m_HorizontalTicker[2], &m_StadardTicker, &CopyRect );

	LoadFonts();
	LoadCursors();

	ZeroMemory( &Sounds, sizeof(Sounds_t) );
	LoadSounds();

	m_bCreated = TRUE;
	return TRUE;
}
// End gui_Resources::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Resources::Destroy() //                     \author Logan Jones
/////////////////////////////                          \date 4/21/2002
//               
//====================================================================
//
void gui_Resources::Destroy()
{
	int  i;

	if( m_bCreated==FALSE ) return;

	SAFE_DELETE_ARRAY( CommonGuiBuffer );

	for( ButtonImages_t* pImage=&StandardButton_16x16; pImage<=&StageButton_4; ++pImage)
		pImage->Destroy();

	for( i=1; i>=0; --i) { SAFE_DESTROY( m_SliderButtonUp[i] ); }
	for( i=1; i>=0; --i) { SAFE_DESTROY( m_SliderButtonDown[i] ); }
	for( i=1; i>=0; --i) { SAFE_DESTROY( m_SliderButtonLeft[i] ); }
	for( i=1; i>=0; --i) { SAFE_DESTROY( m_SliderButtonRight[i] ); }

	for( i=2; i>=0; --i) { SAFE_DELETE_ARRAY(m_VerticalStrip[i].pBytes); }
	for( i=2; i>=0; --i) { SAFE_DELETE_ARRAY(m_HorizontalStrip[i].pBytes); }

	for( i=2; i>=0; --i) { SAFE_DELETE_ARRAY(m_VerticalTicker[i].pBytes); }
	for( i=2; i>=0; --i) { SAFE_DELETE_ARRAY(m_HorizontalTicker[i].pBytes); }
	SAFE_DELETE_ARRAY( m_StadardTicker.pBytes );

	for( gfx_Cursor** ppCursor=&Cursors.Move; ppCursor<=&Cursors.Path; ++ppCursor)
		SAFE_DESTROY( (*ppCursor) );

	for( gfx_Font** ppFont=&Fonts.Standard; ppFont<=&Fonts.StandardSmall; ++ppFont)
		SAFE_DESTROY( (*ppFont) );

	m_bCreated = FALSE;
}
// End gui_Resources::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Resources::GetClosestStandardButton() //    \author Logan Jones
//////////////////////////////////////////////         \date 4/22/2002
//               
//====================================================================
// Parameters:
//  std_Size sz - 
//
// Return: ButtonImages_t* - 
//
gui_Resources::ButtonImages_t* gui_Resources::GetClosestStandardButton( std_Size sz )
{
	ButtonImages_t*	pImage = &StandardButton_16x16;
	ButtonImages_t*	pMin = pImage;
	long			Min = 9999;
	long			Deviation;

	for( ; pImage<=&StandardButton_96x31; ++pImage)
	{
		if( sz==pImage->Stages[0]->GetSize() ) { pMin=pImage; break; }
		Deviation = abs( *(sz - pImage->Stages[0]->GetSize()) * abs( *sz - *(pImage->Stages[0]->GetSize()) ) );
		if( Deviation<Min ) { Min=Deviation; pMin=pImage; }
	}
	return pMin;
}
// End gui_Resources::GetClosestStandardButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ButtonImages_t::Destroy() //                    \author Logan Jones
//////////////////////////////                         \date 4/22/2002
//               
//====================================================================
//
void gui_Resources::ButtonImages_t::Destroy()
{
	for( int i=0; i<StageCount; ++i ) SAFE_DESTROY(Stages[i]);
	SAFE_DESTROY( Pressed );
	SAFE_DESTROY( Disabled );
}
// End gui_Resources::ButtonImages_t::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Resources::LoadFonts() //                   \author Logan Jones
///////////////////////////////                        \date 4/22/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gui_Resources::LoadFonts()
{
	BYTE* pBuf = NULL;
	DWORD Size;

	// Load the small standard font
	Size = theFileTree.DumpFileToBuffer( "anims/hattfont11.gaf", &pBuf );
	if( (Size==0) || (pBuf==NULL) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to load file \'%s\'.", "anims/hattfont11.gaf" );
		return FALSE;
	}
	if( bFAILED(gfx->CreateFontFromGAF( pBuf, NULL, &Fonts.StandardSmall )) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the standard small font." );
		delete [] pBuf;
		return FALSE;
	}
	delete [] pBuf;

	// Load the normal standard font
	Size = theFileTree.DumpFileToBuffer( "anims/hattfont12.gaf", &pBuf );
	if( (Size==0) || (pBuf==NULL) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to load file \'%s\'.", "anims/hattfont12.gaf" );
		return FALSE;
	}
	if( bFAILED(gfx->CreateFontFromGAF( pBuf, NULL, &Fonts.Standard )) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the standard font." );
		delete [] pBuf;
		return FALSE;
	}
	delete [] pBuf;

	return TRUE;
}
// End gui_Resources::LoadFonts()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Resources::LoadCursors() //                 \author Logan Jones
/////////////////////////////////                      \date 4/22/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gui_Resources::LoadCursors()
{
	BYTE* pBuf = NULL;
	DWORD Size;

	// Dump the cursor gaf file
	Size = theFileTree.DumpFileToBuffer( "anims/cursors.gaf", &pBuf );
	if( (Size==0) || (pBuf==NULL) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to load file \'%s\'.", "anims/cursors.gaf" );
		return FALSE;
	}

	// Load all the cursors
	gfx->CreateCursorFromGAF( pBuf, "cursormove", &Cursors.Move );
	gfx->CreateCursorFromGAF( pBuf, "cursorgrn", &Cursors.Green );
	gfx->CreateCursorFromGAF( pBuf, "cursorselect", &Cursors.Select );
	gfx->CreateCursorFromGAF( pBuf, "cursorred", &Cursors.Red );
	gfx->CreateCursorFromGAF( pBuf, "cursorload", &Cursors.Load );
	gfx->CreateCursorFromGAF( pBuf, "cursorrevive", &Cursors.Revive );
	gfx->CreateCursorFromGAF( pBuf, "cursordefend", &Cursors.Defend );
	gfx->CreateCursorFromGAF( pBuf, "cursorpatrol", &Cursors.Patrol );
	gfx->CreateCursorFromGAF( pBuf, "cursorprotect", &Cursors.Protect );
	gfx->CreateCursorFromGAF( pBuf, "cursorrepair", &Cursors.Repair );
	gfx->CreateCursorFromGAF( pBuf, "cursorattack", &Cursors.Attack );
	gfx->CreateCursorFromGAF( pBuf, "cursornormal", &Cursors.Normal );
	gfx->CreateCursorFromGAF( pBuf, "cursorpickup", &Cursors.PickUp );
	gfx->CreateCursorFromGAF( pBuf, "cursorairstrike", &Cursors.AirStrike );
	gfx->CreateCursorFromGAF( pBuf, "cursorTeleport", &Cursors.Teleport );
	gfx->CreateCursorFromGAF( pBuf, "cursorReclamate", &Cursors.Reclamate );
	gfx->CreateCursorFromGAF( pBuf, "cursorFindSite", &Cursors.FindSite );
	gfx->CreateCursorFromGAF( pBuf, "cursorCapture", &Cursors.Capture );
	gfx->CreateCursorFromGAF( pBuf, "cursorUnload", &Cursors.Unload );
	gfx->CreateCursorFromGAF( pBuf, "cursorHourGlass", &Cursors.HourGlass );
	gfx->CreateCursorFromGAF( pBuf, "cursorTooFar", &Cursors.TooFar );
	gfx->CreateCursorFromGAF( pBuf, "pathicon", &Cursors.Path );
	delete [] pBuf;

	return TRUE;
}
// End gui_Resources::LoadCursors()
//////////////////////////////////////////////////////////////////////


#define SET_SOUND( theSOUND ) \
	CASE( #theSOUND ) \
		bValid = Parser.Continue( &Info ); \
		if( bValid && Info.Type==PARSE_LevelUp ) { \
			while( bValid && Parser.Locale()>0 ) { \
				if( Info.Type==PARSE_VarVal && Parser.Locale()==1 && stricmp("Sound",Info.Variable)==0 ) \
				{ sprintf(Path,"sounds/%s.wav",Info.Value); Sounds.theSOUND = theFileTree.OpenFile( Path ); } \
				bValid = Parser.Continue( &Info ); } }


//////////////////////////////////////////////////////////////////////
// gui_Resources::LoadSounds() //                  \author Logan Jones
////////////////////////////////                       \date 4/22/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gui_Resources::LoadSounds()
{
	BYTE*				pBuf = NULL;
	DWORD				Size;
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bValid;
	char				Path[ MAX_PATH ];

	// Load the file buffer
	Size = theFileTree.DumpFileToBuffer( "gamedata/AllSound.tdf", &pBuf );
	if( (pBuf==NULL) || (Size==0) ) return FALSE;

	// Start a parsin'
	bValid = Parser.Begin( pBuf, Size, &Info );
	while( bValid )
	{
		if( Info.Type==PARSE_Header && Parser.Locale()==0 )
		{
			STRING_SWITCH( Info.Value )
				SET_SOUND( ActivateAllStatBars )
				SET_SOUND( EndGameStatBar )
				SET_SOUND( EndGameScore )
				SET_SOUND( BGM )
				SET_SOUND( BigButton )
				SET_SOUND( Skirmish )
				SET_SOUND( Multi )
				SET_SOUND( SkirmishCheat )
				SET_SOUND( Options )
				SET_SOUND( Exit )
				SET_SOUND( SmlButton )
				SET_SOUND( Slider )
				SET_SOUND( SmallButton )
				SET_SOUND( SideSelect )
				SET_SOUND( SideSelect2 )
				SET_SOUND( More )
				SET_SOUND( JoinGame )
				SET_SOUND( MainMenu )
				SET_SOUND( TreeBurn )
				SET_SOUND( SelectMultipleUnits )
				SET_SOUND( MessageArrived )
				SET_SOUND( CreateSquad )
				SET_SOUND( SelectSquad )
				SET_SOUND( OutOfRange )
				SET_SOUND( NoMetal )
				SET_SOUND( NoEnergy )
				SET_SOUND( AddBuild )
				SET_SOUND( NotOkToBuild )
				SET_SOUND( OkToBuild )
				SET_SOUND( OrdersButton )
				SET_SOUND( BuildButton )
				SET_SOUND( SubBuild )
				SET_SOUND( NextBuildMenu )
				SET_SOUND( SetFireOrders )
				SET_SOUND( SetMoveOrders )
				SET_SOUND( SpecialOrders )
				SET_SOUND( ImmediateOrders )
				SET_SOUND( Ally )
				SET_SOUND( Previous )
				SET_SOUND( Panel )
				SET_SOUND( VictoryCondition )
			END_STRING_SWITCH
		} // end if( PARSE_Header )

		// Continue the parse
		bValid = Parser.Continue( &Info );

	} // end while( bValid )

	// Hardcoded sounds
	Sounds.NotYetImplemented = theFileTree.OpenFile( "sounds/CantDo.wav" );

	delete [] pBuf;
	return TRUE;
}
// End gui_Resources::LoadSounds()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gui_Resources.cpp //
////////////////////////////
