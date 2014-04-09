// unit_Factory_Load.cpp //                        \author Logan Jones
//////////////////////////                            \date 4/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Factory.h"
#include "game.h"
#include "gfx.h"
#include "util_TxtParse.h"
#include "ta_OTA.h"


//////////////////////////////////////////////////////////////////////
// unit_Factory::Create() //                       \author Logan Jones
///////////////////////////                            \date 2/19/2002
//               
//====================================================================
// Parameters:
//  game_LoadFacilitator Load - 
//
// Return: BOOL - 
//
BOOL unit_Factory::Create( game_LoadFacilitator Load )
{
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;
	BOOL							bWorking;
	DWORD							FileCount;
	DWORD							count;
	std_Time_t						time = 0;

	// Proccess the downloads and enlist the textures
	theApp.Console.Comment( CT_LOAD, "Processing downloads..." );
	Load.Refresh();
	ProccessDownloads();
	gfx->ListModelTextures();

	// Get the sound categories
	theApp.Console.Comment( CT_LOAD, "Categorizing unit sounds..." );
	Load.Refresh();
	if( bFAILED(GetSoundCategories()) )
		return FALSE;

	FileCount = theFileTree.GetFileCountInDirectory("units");

	// Proccess each ufo
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "fbi", "units", &FindInfo );
	bWorking = hFind!=NULL;
	for( count=0; bWorking; ++count )
	{
		// What unit are we loading
		theApp.Console.Comment( CT_LOAD, "Processing units/%s", FindInfo.Name );

		// Only update the load screen every now and then. This keeps the load time
		// from being dominated by the rendering of the load screen
		if((std_Time()-time)>=200)
		{
			Load.Progress( count * 100.0f / FileCount );
			time=std_Time();
		}

		// Attempt to load the unit type
		if( !LoadUnitType(FindInfo.hFile) ) {
			// We failed to load this unit, let the user see the error message
			Load.Progress( count * 100.0f / FileCount );
			time=std_Time();
		}

		// Find next file
		bWorking = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( fbi files )
	theFileTree.CloseFind( hFind );

	// Clean up
	theGame.Interface.UnitLoadComplete();
	m_SoundCategoryMap.clear();
	gfx->DestroyModelTextureList();

	return TRUE;
}
// End unit_Factory::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::Destroy() //                      \author Logan Jones
////////////////////////////                           \date 2/19/2002
//               
//====================================================================
//
void unit_Factory::Destroy()
{
	m_Units.clear();
	m_UnitTypes.clear();
}
// End unit_Factory::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::ProccessDownloads() //            \author Logan Jones
//////////////////////////////////////                 \date 8/19/2002
//               
//====================================================================
//
void unit_Factory::ProccessDownloads()
{
	app_FileTree::HFIND			hFind;
	app_FileTree::FindData_t		FindInfo;
	BOOL							bSearching;
	DWORD							FileSize;
	BYTE*							FileBuffer;
	util_TxtParse					Parser;
	Parse_Info						Info;
	BOOL							bWorking;
	DWORD							Builder;
	DWORD							Page;
	DWORD							Button;
	char							Buildee[32];

	// Proccess downloads
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "tdf", "download", &FindInfo );
	bSearching = hFind!=NULL;
	while( bSearching )
	{
		// Get the TDF file buffer and start parsing
		FileSize = theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(FindInfo.hFile), &FileBuffer );
		bWorking = Parser.Begin( FileBuffer, FileSize, &Info );
		while( bWorking )
			if( Info.Type==PARSE_Header && strnicmp("MenuEntry",Info.Value,9)==0 && Parser.Locale()==0 )
			{
				// Move into the block
				bWorking = Parser.Continue( &Info );
				if( !bWorking || Info.Type!=PARSE_LevelUp ) continue;

				// Init the info
				Builder = Page = Button = 0;
				Buildee[0] = '\0';

				// Get the blocks info
				while( bWorking && Parser.Locale()>=1 )
				{
					if( Info.Type==PARSE_VarVal && Parser.Locale()==1 ) STRING_SWITCH( Info.Variable )
						CASE( "UnitMenu" )	Builder = std_NameHash(Info.Value);
						CASE( "Menu" )		Page = atol(Info.Value);
						CASE( "Button" )	Button = atol(Info.Value);
						CASE( "UnitName" )	strcpy( Buildee, Info.Value );
					END_STRING_SWITCH
					bWorking = Parser.Continue( &Info );
				}

				// Apply the data
				theGame.Interface.AddDownload( Builder, Page, Button, Buildee );
			}
			else bWorking = Parser.Continue( &Info );

		// Delete the file buffer and find next file
		SAFE_DELETE_ARRAY( FileBuffer );
		bSearching = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( tdf files )
	theFileTree.CloseFind( hFind );
}
// End unit_Factory::ProccessDownloads()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::LoadUnitType() //                 \author Logan Jones
/////////////////////////////////                      \date 3/24/2002
//               
//====================================================================
// Parameters:
//  void* hFile - 
//
// Return: bool - 
//
bool unit_Factory::LoadUnitType( void* hFile )
{
	unit_Type			UnitTypeInfo;
	static char			ObjectName[ 64 ] = "Objects3D/";
	BYTE*				pFileBuffer = NULL;
	BOOL				bResult;

	// Sanity check
	if( hFile==NULL ) return false;

	// Proccess the FBI info file
	ProccessFBI( hFile, UnitTypeInfo, ObjectName + 10 );

	// Get the unit's unique key
	UnitTypeInfo.Key = std_NameHash( UnitTypeInfo.UnitName );

	// Check if a unit by this name already exists
	UnitTypeMap_t::iterator it = m_UnitTypes.find( UnitTypeInfo.Key );
	if( it!=m_UnitTypes.end() ) {
		theApp.Console.Comment( CT_LOAD, "\'%s\'(%d) already exists. [%s, %d]",
			UnitTypeInfo.UnitName, UnitTypeInfo.Key,
			((*it).second).UnitName, ((*it).second).Key );
		return false;
	}

	// Load the model
	strcat( ObjectName, ".3do" );
	theFileTree.DumpFileToBuffer( ObjectName, &pFileBuffer );
	bResult = gfx->CreateModel( pFileBuffer, &UnitTypeInfo.Model );
	delete [] pFileBuffer;
	if(	!bResult ) {
		theApp.Console.Comment( CT_LOAD, "Failed to load \'%s\'", ObjectName );
		return false;
	}

	// Load the script
	if( bFAILED(m_ScriptMachine.LoadScript(
				UnitTypeInfo.UnitName,
				UnitTypeInfo.Model,
				&UnitTypeInfo.pScript )) ) {
		UnitTypeInfo.Model->Destroy();
		return false;
	}

	// Load the build menu
	if( UnitTypeInfo.Abilities & unit_Type::CanBuild )
		theGame.Interface.CreateBuildMenu( UnitTypeInfo );
	else UnitTypeInfo.BuildMenuCount = 0;

	// Assocciate the type with its key and return
	m_UnitTypes[ UnitTypeInfo.Key ] = UnitTypeInfo;
	return true;
}
// End unit_Factory::LoadUnitType()
//////////////////////////////////////////////////////////////////////


// Macros used in filling the type strucure with info from the file
#define CASE_STRING( STRING )			CASE( #STRING )		strcpy( Type.##STRING, Info.Value );
#define CASE_STRING2( STRING, STR2 )	CASE( #STRING )		strcpy( Type.##STR2, Info.Value );

#define CASE_ABILITY( ABILITY ) \
	CASE( #ABILITY )	if(atoi(Info.Value)) Type.Abilities |= unit_Type::##ABILITY##;
#define CASE_ABILITY2( ABILITY, ACTUAL ) \
	CASE( #ABILITY )	if(atoi(Info.Value)) Type.Abilities |= unit_Type::##ACTUAL##;

#define CASE_LONG_VALUE( VAR )			CASE( #VAR )		Type.##VAR = atol( Info.Value );
#define CASE_LONG_VALUE2( VAR, VAR2 )	CASE( #VAR )		Type.##VAR2 = atol( Info.Value );
#define CASE_FLOAT_VALUE( VAR )			CASE( #VAR )		Type.##VAR = atof( Info.Value );
#define CASE_FLOAT_VALUE2( VAR, VAR2 )	CASE( #VAR )		Type.##VAR2 = atof( Info.Value );

#define SET_TYPE_MEMBER()	STRING_SWITCH( Info.Variable ) \
		CASE_STRING( Name ) \
		CASE_ABILITY( CanCapture ) \
		CASE( "CanDGun" )	if(atoi(Info.Value)) Type.Abilities |= unit_Type::CanBlast; \
	END_STRING_SWITCH


//////////////////////////////////////////////////////////////////////
// unit_Factory::ProccessFBI() //                  \author Logan Jones
////////////////////////////////                       \date 3/24/2002
//               
//====================================================================
// Parameters:
//  void* hFile              - 
//  UnitFileInfo_t& UnitInfo - 
//
void unit_Factory::ProccessFBI( void* hFile, unit_Type& Type, LPTSTR strObjectName )
{
	DWORD				FileSize;
	BYTE*				FileBuffer;
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bWorking;
	long				lValue;
	std_Vector2			vect2;

	// Get the FBI file buffer
	FileSize = theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(hFile), &FileBuffer );

	// Proccess the FBI
	bWorking = Parser.Begin( FileBuffer, FileSize, &Info );

	// Move past the first header and its '{'
	while( bWorking && !(Info.Type==PARSE_Header && Parser.Locale()==0) ) bWorking = Parser.Continue( &Info );
	while( bWorking && Info.Type!=PARSE_LevelUp ) bWorking = Parser.Continue( &Info );

	// Initialize the unit info
	ZeroMemory( &Type, sizeof(Type) );

	// Load the fbi structure
	while( bWorking && Parser.Locale()>0 )
	{
		if( Info.Type==PARSE_VarVal && Parser.Locale()==1 )
			STRING_SWITCH( Info.Variable )
				CASE_STRING( Name )
				CASE_STRING( Description )
				CASE( "FootprintX" )	Type.FootPrint.width = atol(Info.Value);
				CASE( "FootprintZ" )	Type.FootPrint.height =atol(Info.Value);
				
				CASE_LONG_VALUE( BuildCostEnergy )
				CASE_LONG_VALUE( BuildCostMetal )
				CASE( "BuildTime" )		Type.BuildTime = atol(Info.Value) * WORKER_TIME_MODIFIER;
				CASE( "BuildAngle" ) 	Type.BuildAngle = (atol(Info.Value) / ANGULAR_CONSTANT);
				
				CASE_LONG_VALUE( MaxDamage )
			//	float	DamageModifier;
				CASE( "MaxSlope" )
					// Set the max slope
					lValue = atol(Info.Value);
					if( lValue==0 ) lValue = 256;
					vect2.SetPolar( atanf( lValue / 16.0f ), 1.0f );
					Type.MaxSlope = vect2.y;
				CASE_LONG_VALUE( MaxWaterDepth )
				CASE_LONG_VALUE( MinWaterDepth )
			//	long	WaterLine;
				
			//	long	EnergyUse;
				CASE( "WorkerTime" )	Type.WorkerTime = atol(Info.Value) * WORKER_TIME_MODIFIER;
				CASE_LONG_VALUE( BuildDistance )
				
				CASE_ABILITY2( Builder, CanBuild )

				CASE( "BMcode" )
					if( atol(Info.Value)==0 )
						Type.Behaviour = unit_Type::Structure;
					else // Assume ground for now
						Type.Behaviour = unit_Type::Groundcraft;
				
			/*	long	NoAutoFire;
				
				long	SightDistance;
				long	RadarDistance;
				long	SonarDistance;
			*/	
				CASE( "SoundCategory" )
					// Find the desired sound category and set it
					SoundCategoryMap_t::const_iterator SndCat = m_SoundCategoryMap.find( Info.Value );
					if( SndCat!=m_SoundCategoryMap.end() )
						Type.pSounds = (*SndCat).second;
					else Type.pSounds = &m_SoundCategories.front();
				
				CASE_LONG_VALUE( EnergyStorage )
				CASE_LONG_VALUE( MetalStorage )
				CASE_LONG_VALUE( EnergyMake )
				CASE_LONG_VALUE( MetalMake )
				CASE_LONG_VALUE( EnergyUse )
				CASE_LONG_VALUE( MetalUse )
				CASE_LONG_VALUE( MakesMetal )
				CASE_FLOAT_VALUE2( ExtractsMetal, MetalExtractionRate )
				CASE_LONG_VALUE( WindGenerator )
			/*	
				char	ExplodeAs[32];
				char	SelfDestructAs[32];
			*/	
				CASE_STRING( UnitName )
				CASE( "ObjectName" )	strcpy( strObjectName, Info.Value );
			//	char	Corpse[32];
			//	char	YardMap[256];
				
				CASE_ABILITY2( FireStandOrders, HasFireOrders )
				CASE_LONG_VALUE2( StandingFireOrder, InitialFireOrder )
				CASE_ABILITY2( MobileStandOrders, HasMoveOrders )
				CASE_LONG_VALUE2( StandingMoveOrder, InitialMoveOrder )
				
			//	long	ManeuverLeashLength;
				
				CASE_ABILITY( CanMove )
				CASE_ABILITY( CanPatrol )
				CASE_ABILITY( CanStop )
				CASE_ABILITY2(CanGuard, CanDefend )
				CASE_ABILITY( CanAttack )
				CASE_ABILITY2(CanReclamate, CanReclaim )
				CASE_ABILITY( CanCapture )
				CASE_ABILITY2(CanDGun, CanBlast )
				CASE_ABILITY( CanLoad )
				CASE_ABILITY( OnOffable )
				CASE_LONG_VALUE2( ActivateWhenBuilt, InitialActivationOrder )
				
				CASE( "CanHover" ) if( atol(Info.Value)!=0 )
					if( Type.Behaviour!=unit_Type::Structure )
						Type.Behaviour = unit_Type::Hovercraft,
						Type.Abilities |= unit_Type::CanHover;
					else theApp.Console.Comment( CT_ERROR, "WARNING: Structure was given hovercraft behaviour. Ignored." );

				CASE( "CanFly" ) if( atol(Info.Value)!=0 )
					if( Type.Behaviour!=unit_Type::Structure )
						Type.Behaviour = unit_Type::Aircraft,
						Type.Abilities |= unit_Type::CanFly;
					else theApp.Console.Comment( CT_ERROR, "WARNING: Structure was given aircraft behaviour. Ignored." );

				CASE( "Floater" ) if( atol(Info.Value)!=0 )
					if( Type.Behaviour!=unit_Type::Structure )
						Type.Behaviour = unit_Type::Seacraft/*,
						Type.Abilities |= unit_Type::CanFly*/;
					else theApp.Console.Comment( CT_ERROR, "WARNING: Structure was given seacraft behaviour. Ignored." );

				CASE_FLOAT_VALUE2( CruiseAlt, CruiseAltitude )
				CASE_FLOAT_VALUE( BankScale )
				CASE_FLOAT_VALUE( PitchScale )
				
				CASE_ABILITY2( CloakCost, CanCloak )
			//	long	CloakCost;
			//	long	CloakCostMoving;			
				CASE_LONG_VALUE2( init_cloaked, InitialCloakOrder )
			//	long	MinCloakDistance;
				
			//	long	HealTime;
				
				CASE_FLOAT_VALUE2(MaxVelocity, MaxSpeed )
				CASE_FLOAT_VALUE( BrakeRate )
				CASE_FLOAT_VALUE( Acceleration )
				CASE( "TurnRate" )	Type.TurnRate = (atol(Info.Value) / ANGULAR_CONSTANT) * (fPI/180.0f);
			//	long	SteeringMode;
				CASE_LONG_VALUE( Upright )
			//	long	ShootMe;
			//	long	Floater;
			//	float	Scale;
			/*	
				char	Weapon1[ 32 ];
				char	WPri_BadTargetCategory[ 32 ];
				char	Weapon2[ 32 ];
				char	WSec_BadTargetCategory[ 32 ];
				char	Weapon3[ 32 ];
				char	WTer_BadTargetCategory[ 32 ];
				char	BadTargetCategory[ 32 ];
				char	NoChaseCategory[ 32 ];
				
				long	Commander;
				long	HideDamage;
				long	ShowPlayerName;
				long	ImmuneToParalyzer;
				
				long	Downloadable;
				long	OvrAdjust;
			*/
			END_STRING_SWITCH
		bWorking = Parser.Continue( &Info );
	} // end while( bWorking )
}
// End unit_Factory::ProccessFBI()
//////////////////////////////////////////////////////////////////////


#define SOUND_CATEGORY_CASE( A_SOUND ) \
	CASE( #A_SOUND ) \
		sprintf( SoundPath, "sounds/%s.wav", Info.Value ); \
		pCategory->A_SOUND = theFileTree.OpenFile( SoundPath );


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetSoundCategories() //           \author Logan Jones
///////////////////////////////////////                \date 4/15/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL unit_Factory::GetSoundCategories()
{
	typedef hash_map< string, LPVOID >	SoundMap_t;

	BYTE*							pFileBuf;
	DWORD							Size;
	SoundMap_t						Sounds;
	util_TxtParse					Parser;
	Parse_Info						Info;
	BOOL							bWorking;
	string							CategoryName;
	char							SoundPath[128];
	unit_SoundCategory*				pCategory;
	SoundCategoryMap_t::iterator	cat;

	// Retrieve the file
	Size = theFileTree.DumpFileToBuffer( "gamedata/sound.tdf", &pFileBuf );
	if( Size==0 )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to retrieve \'gamedata/sound.tdf\'" );
		return FALSE;
	}

	// Proccess the file
	bWorking = Parser.Begin( pFileBuf, Size, &Info );
	while( bWorking )
		if( Info.Type==PARSE_Header && Parser.Locale()==0 )
		{
			// Record the category name
			CategoryName = Info.Value;

			// Continue to the var/val block
			bWorking = Parser.Continue( &Info );
			if( !bWorking || Info.Type!=PARSE_LevelUp ) break;

			// Check if this category already exists
			if( (cat=m_SoundCategoryMap.find(CategoryName))!=m_SoundCategoryMap.end() )
			{
				theApp.Console.Comment( CT_EXTRA_LOAD, "Sound category \'%s\' already defined: \'%s\'; Redefining.", CategoryName.c_str(), ((*cat).first).c_str() );
				pCategory = (*cat).second;
			}
			else // Add a new category
			{
				m_SoundCategories.push_back( unit_SoundCategory() );
				m_SoundCategoryMap[ CategoryName ] = &m_SoundCategories.back();
				pCategory = &m_SoundCategories.back();
				ZeroMemory( pCategory, sizeof(unit_SoundCategory) );
			}

			// Fill out the category
			while( bWorking && Parser.Locale()>0 )
			{
				if( Info.Type==PARSE_VarVal && Parser.Locale()==1 )
					STRING_SWITCH( Info.Variable )
						SOUND_CATEGORY_CASE( Select1 )
						SOUND_CATEGORY_CASE( Ok1 )
						SOUND_CATEGORY_CASE( Arrived1 )
						SOUND_CATEGORY_CASE( Cant1 )
						SOUND_CATEGORY_CASE( Build )
						SOUND_CATEGORY_CASE( Repair )
						SOUND_CATEGORY_CASE( Working )
						SOUND_CATEGORY_CASE( Activate )
						SOUND_CATEGORY_CASE( Deactivate )
						SOUND_CATEGORY_CASE( UnderAttack )
						SOUND_CATEGORY_CASE( Count5 )
						SOUND_CATEGORY_CASE( Count4 )
						SOUND_CATEGORY_CASE( Count3 )
						SOUND_CATEGORY_CASE( Count2 )
						SOUND_CATEGORY_CASE( Count1 )
						SOUND_CATEGORY_CASE( Count0 )
						SOUND_CATEGORY_CASE( CancelDestruct )
					END_STRING_SWITCH
				bWorking = Parser.Continue( &Info );
			}
		}
		else bWorking = Parser.Continue( &Info );

	Sounds.clear();
	delete [] pFileBuf;
	return TRUE;
}
// End unit_Factory::GetSoundCategories()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Factory::ProccessSchema() //               \author Logan Jones
///////////////////////////////////                  \date 05-07-2003
//
//===================================================================
// Parameters:
//  ta_ota_Schema_t* pSchema - 
//
void unit_Factory::ProccessSchema( ta_ota_Schema_t* pSchema )
{
	const unit_Type*	pType;

	for( SchemaUnitList_t::iterator it=pSchema->Units.begin(),end=pSchema->Units.end(); it!=end; ++it)
		if(pType = GetUnitType( (*it).Unitname ))
			theGame.NotifySpawnCompleteUnit( pType, std_Point((*it).XPos,(*it).ZPos), (*it).Player - 1 );
}
// End unit_Factory::ProccessSchema()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Factory_Load.cpp //
////////////////////////////////
