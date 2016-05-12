// feature_Factory.cpp //                          \author Logan Jones
////////////////////////                              \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "feature.h"
#include "feature_Factory.h"
#include "game.h"
#include "ta_TNT.h"
#include "ta_GAF.h"
#include "util_TxtParse.h"
#include "gfx.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "feature_Factory.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
feature_Factory::feature_Factory():
	m_ToLoadCount(0)
{}
feature_Factory::~feature_Factory()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::Create() //                    \author Logan Jones
//////////////////////////////                         \date 2/12/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntFileBuffer           - 
//  LPTSTR strWorld                - 
//  game_Main::LoadGameStruct Load - 
//
// Return: BOOL - 
//
BOOL feature_Factory::Create( BYTE* pTntFileBuffer, LPCTSTR strWorld, game_LoadFacilitator Load )
{
	app_FileTree::HFIND				hList;
	app_FileTree::FindData_t		ListInfo;
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;
	BOOL							bWorkingFeatureFolders;
	BOOL							bWorkingFeatureFiles;
	char							SubDirectoryToSearch[128];
	LPTA_TNT_HEADER					pHeader;
	LPTA_TNT_EXT_HEADER				pMapHeader;
	DWORD							count;
	TA_TNT_FEATURE_ENTRY*			pFeatureToLoad;
	int								CurrentProgress;

	if( bFAILED( AddHardcodedFeatureTypes() ) )
	{
		return FALSE;
	}

	// Initialize some loading members
	m_LoadPass = -1;
	m_CurrentAnimFile = NULL;
	m_CurrentAnimFileName[0] = '\0';

	// TODO: Add unit feature stuff (corpses, etc...) to the list

	pHeader = (LPTA_TNT_HEADER)pTntFileBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(pTntFileBuffer + sizeof(TA_TNT_HEADER));
	
	// Add the features from the map to the list
	pFeatureToLoad = (TA_TNT_FEATURE_ENTRY*)(pTntFileBuffer + pMapHeader->OffsetToFeatureEntryArray);
	for( count=pMapHeader->NumberOfFeatures; count>0; --count, ++pFeatureToLoad)
		AddToLoadList( pFeatureToLoad->Name, 0 );


	// Precheck features in the all worlds folder to speed loading
	theApp.Console.Comment( CT_LOAD, "Processing features/all worlds" );
	for( m_NewFeatureAdded=TRUE; m_NewFeatureAdded; )
	{
		m_NewFeatureAdded = FALSE;
		ProcessDirectory( "all worlds", Load );
	}
	Load.Progress( 10 );


	// Precheck features in the specific world folder to speed loading
	if( strWorld[0]!='\0' )
	{
		theApp.Console.Comment( CT_LOAD, "Processing features/%s", strWorld );
		for( m_NewFeatureAdded=TRUE; m_NewFeatureAdded; )
		{
			m_NewFeatureAdded = FALSE;
			ProcessDirectory( strWorld, Load );
		}
	}
	Load.Progress( 20 );


	// If there are any features left to load, look through all the other subdirectories
	for( m_LoadPass=0,CurrentProgress=20; !m_FeaturesToLoad.empty(); ++m_LoadPass )
	{
		// Check if we need to check for unfound features
		if( m_LoadPass>0 )
		{
			FeatureToLoadList_t::iterator		e,it = m_FeaturesToLoad.begin();
			FeatureToLoadList_t::const_iterator	end= m_FeaturesToLoad.end();
			for( ; it!=end; )
			{
				if( (*it).second.SearchAdded<m_LoadPass )
				{
					theApp.Console.Comment( CT_LOAD, "Could not find feature \"%s\"", (*it).second.Name );
					m_FeaturesToLoad.erase( e=it++ );
				}
				else ++it;
			}

			if( m_FeaturesToLoad.empty() )
				break;

		} // end if( m_LoadPass > 0 )

		// Search all subdirectories ind 'features'
		hList = theFileTree.Find( app_FileTree::DIRECTORIES, NULL, "features", &ListInfo );
		bWorkingFeatureFolders = hList!=NULL;
		while( bWorkingFeatureFolders )
		{
			// Make sure this is a directory
			if( ListInfo.bIsFile==FALSE )
			{
				theApp.Console.Comment( CT_LOAD, "Processing features/%s", ListInfo.Name );

				// Search all tdf files in this subdirectory
				sprintf( SubDirectoryToSearch, "features/%s", ListInfo.Name );
				hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "tdf", SubDirectoryToSearch, &FindInfo );
				bWorkingFeatureFiles = hFind!=NULL;
				while( bWorkingFeatureFiles )
				{
					//theApp.Console.Comment( CT_LOAD, "Processing features/%s/%s", ListInfo.ItemName, FindInfo.Name );
					ProcessTDF( FindInfo.hFile, ListInfo.Name, FindInfo.Name, Load );

					// Check if there is anything else to load
					if( m_FeaturesToLoad.empty() )
						break;

					// Find next file
					bWorkingFeatureFiles = theFileTree.ContinueFind( hFind, &FindInfo );

				} // end while( tdf files )

				theFileTree.CloseFind( hFind );

			} // end if( IsDirectory )

			// Check if there is anything else to load
			if( m_FeaturesToLoad.empty() )
				break;

			// Find next subdirectory
			bWorkingFeatureFolders = theFileTree.ContinueFind( hList, &ListInfo );

		} // end while( subdirectories )

		theFileTree.CloseFind( hList );
	} // end while( there are still features to load )

	// Clean up some loading members
	SAFE_DELETE_ARRAY( m_CurrentAnimFile );

	theApp.Console.Comment( CT_LOAD, "Placing features on the terrain..." );
	Load.Progress( 90 );

	// Spawn all the features on the map
	pFeatureToLoad = (TA_TNT_FEATURE_ENTRY*)(pTntFileBuffer + pMapHeader->OffsetToFeatureEntryArray);
	LPTA_TNT_MAP_ENTRY pMapStuff = (LPTA_TNT_MAP_ENTRY)(pTntFileBuffer + pMapHeader->OffsetToMapInfoArray);
	for( DWORD y=0; y<pHeader->Height-1; ++y,pMapStuff+=pHeader->Width)
		for( DWORD x=0; x<pHeader->Width-1; ++x)
			if( pMapStuff[x].Special>=0 )
				SpawnFeature( pFeatureToLoad[pMapStuff[x].Special].Name, x * 16, y * 16, pMapStuff[x].Elevation );

	Load.Progress( 100 );
	return TRUE;
}
// End feature_Factory::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::AddHardcodedFeatureTypes() //  \author Logan Jones
////////////////////////////////////////////////       \date 2/13/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL feature_Factory::AddHardcodedFeatureTypes()
{
	FeatureFileInfo_t		FeatureFileInfo;

	ZeroMemory( &FeatureFileInfo, sizeof(FeatureFileInfo_t) );
	strcpy( FeatureFileInfo.World, "All Worlds" );
	strcpy( FeatureFileInfo.Description, "Damaged Terrain" );
	strcpy( FeatureFileInfo.Category, "Scars" );
	FeatureFileInfo.FootPrintX = 1;
	FeatureFileInfo.FootPrintZ = 1;
	strcpy( FeatureFileInfo.FileName, "trees" );
	FeatureFileInfo.Indestructible = 1;

	theApp.Console.Comment( CT_LOAD, "Loading feature \"Smudge01\"" );
	strcpy( FeatureFileInfo.SeqName, "smudge01" );
	AddFeatureType( "Smudge01", &FeatureFileInfo );

	theApp.Console.Comment( CT_LOAD, "Loading feature \"Smudge02\"" );
	strcpy( FeatureFileInfo.SeqName, "smudge02" );
	AddFeatureType( "Smudge02", &FeatureFileInfo );

	theApp.Console.Comment( CT_LOAD, "Loading feature \"Smudge03\"" );
	strcpy( FeatureFileInfo.SeqName, "smudge03" );
	AddFeatureType( "Smudge03", &FeatureFileInfo );

	theApp.Console.Comment( CT_LOAD, "Loading feature \"Smudge04\"" );
	strcpy( FeatureFileInfo.SeqName, "smudge04" );
	AddFeatureType( "Smudge04", &FeatureFileInfo );

	return TRUE;
}
// End feature_Factory::AddHardcodedFeatureTypes()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::ProcessDirectory() //          \author Logan Jones
////////////////////////////////////////               \date 2/13/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strDirectory       - 
//  game_LoadFacilitator Load - 
//
// Return: BOOL - 
//
BOOL feature_Factory::ProcessDirectory( LPCTSTR strDirectory, game_LoadFacilitator Load )
{
	char							SubDirectoryToSearch[128];
	BOOL							bWorkingFeatureFiles;
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;

	sprintf( SubDirectoryToSearch, "features/%s", strDirectory );
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "tdf", SubDirectoryToSearch, &FindInfo );
	bWorkingFeatureFiles = hFind!=NULL;
	while( bWorkingFeatureFiles )
	{
		ProcessTDF( FindInfo.hFile, strDirectory, FindInfo.Name, Load );

		// Find next file
		bWorkingFeatureFiles = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( tdf files )

	theFileTree.CloseFind( hFind );
	return TRUE;
}
// End feature_Factory::ProcessDirectory()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::ProcessTDF() //                \author Logan Jones
//////////////////////////////////                     \date 2/13/2002
//               
//====================================================================
// Parameters:
//  void* hFile               - 
//  LPCTSTR strHostDirectory  - 
//  LPCTSTR strFileName       - 
//  game_LoadFacilitator Load - 
//
// Return: BOOL - 
//
BOOL feature_Factory::ProcessTDF( void* hFile, LPCTSTR strHostDirectory, LPCTSTR strFileName, game_LoadFacilitator Load )
{
	DWORD				FileSize;
	BYTE*				FileBuffer;
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bWorking;
	char				FeatureName[32];
	FeatureFileInfo_t	FeatureInfo;

// Macros used in filling the strucure with info from the file
#define STRING_MEMBER( NAME ) CASE( #NAME ) \
	strcpy( FeatureInfo.NAME, Info.Value );
#define INT_MEMBER( NAME ) CASE( #NAME ) \
		FeatureInfo.NAME = atoi( Info.Value ); \

	// Sanity check
	if( hFile==NULL ) return FALSE;

	// Get the file
	FileSize = theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(hFile), &FileBuffer );

	// Begin working on the TDF
	bWorking = Parser.Begin( FileBuffer, FileSize, &Info );
	while( bWorking )
	{
		// Find the next feature
		while( bWorking )
		{
			if( (Info.Type==PARSE_Header)&&(Parser.Locale()==0) ) break;
			bWorking = Parser.Continue( &Info );
		}

		// Check if this feature is in the list
		if( IsInToLoadList(Info.Value) )
		{
			strcpy( FeatureName, Info.Value );
			theApp.Console.Comment( CT_LOAD, "Loading feature \"%s\"", FeatureName );
			Load.Progress( 90.0f * m_FeatureTypes.size() / m_ToLoadCount );

			// Initialize and fill the feature info structure
			ZeroMemory( &FeatureInfo, sizeof(FeatureFileInfo_t) );
			bWorking = Parser.Continue( &Info );
			if( bWorking && Info.Type==PARSE_LevelUp )
				for( ;bWorking && Parser.Locale()>0; bWorking=Parser.Continue( &Info ) )
					if( (Info.Type==PARSE_VarVal)&&(Parser.Locale()==1) )
						STRING_SWITCH( Info.Variable )
							STRING_MEMBER( World ) 
							STRING_MEMBER( Description ) 
							STRING_MEMBER( Category ) 
							INT_MEMBER( FootPrintX ) 
							INT_MEMBER(	FootPrintZ ) 
							INT_MEMBER(	Height ) 
							STRING_MEMBER( Object ) 
							STRING_MEMBER( FileName )
							STRING_MEMBER( SeqName ) 
							STRING_MEMBER( SeqNameShad ) 
							STRING_MEMBER( SeqNameReclamate ) 
							STRING_MEMBER( FeatureReclamate ) 
							STRING_MEMBER( SeqNameBurn ) 
							STRING_MEMBER( FeatureBurnt ) 
							STRING_MEMBER( SeqNameDie ) 
							STRING_MEMBER( FeatureDead ) 
							INT_MEMBER(	Energy ) 
							INT_MEMBER(	Metal ) 
							INT_MEMBER(	BurnMin ) 
							INT_MEMBER(	BurnMax ) 
							INT_MEMBER(	SparkTime ) 
							INT_MEMBER(	SpreadChance ) 
							STRING_MEMBER( BurnWeapon ) 
							INT_MEMBER(	AnimTrans ) 
							INT_MEMBER(	ShadTrans ) 
							INT_MEMBER(	Animating ) 
							INT_MEMBER(	Flamable ) 
							INT_MEMBER(	Reclaimable ) 
							INT_MEMBER(	Reproduce ) 
							INT_MEMBER(	ReproduceArea ) 
							INT_MEMBER(	Blocking ) 
							INT_MEMBER(	HitDensity ) 
							INT_MEMBER(	Damage ) 
							INT_MEMBER(	Geothermal ) 
							INT_MEMBER(	Indestructible ) 
							INT_MEMBER(	NoDisplayInfo ) 
							INT_MEMBER(	Permanent ) 
						END_STRING_SWITCH

			// Add the feature
			AddFeatureType( FeatureName, &FeatureInfo );

		} // end if( IsInToLoadList )
		else bWorking = Parser.Continue( &Info );

	} // end while( bWorking )

	return TRUE;
}
// End feature_Factory::ProcessTDF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::AddFeatureType() //            \author Logan Jones
//////////////////////////////////////                 \date 2/13/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strName                     - 
//  FeatureFileInfo_t* pFeatureFileInfo - 
//
// Return: BOOL - 
//
BOOL feature_Factory::AddFeatureType( LPCTSTR strName, FeatureFileInfo_t* pFeatureFileInfo )
{
#define StringIsValid( STRING_TO_CHECK )  ( (STRING_TO_CHECK)[0]!=0 && stricmp((STRING_TO_CHECK),"empty")!=0 )
	DWORD				NameKey = std_NameHash( strName );
	FeatureType_t		Feature;
	char				FilePath[ MAX_PATH ];

	// Make sure that there isn't already an entry for this key
	FeatureTypeMap_t::iterator it = m_FeatureTypes.find( NameKey );
	if( it!=m_FeatureTypes.end() )
	{
		theApp.Console.Comment( CT_LOAD,
		"ERROR: Key for \'%s\'(%d) already used by \'%s\'(%d).",
		strName,
		std_NameHash(strName),
		(*it).second.Name.c_str(),
		std_NameHash((*it).second.Name.c_str()) );
		return FALSE;
	}

	// Copy some stuff from the info
	Feature.Name = strName;
	Feature.Description = pFeatureFileInfo->Description;
	Feature.Footprint.Set( pFeatureFileInfo->FootPrintX, pFeatureFileInfo->FootPrintZ );
	Feature.Height = pFeatureFileInfo->Height;
	Feature.Flags =
		(pFeatureFileInfo->Animating ? FTF_Animating : 0) |
		(pFeatureFileInfo->Flamable ? FTF_Flamable : 0) |
		(pFeatureFileInfo->Reclaimable ? FTF_Reclaimable : 0) |
		(pFeatureFileInfo->Blocking ? FTF_Blocking : 0) |
		(pFeatureFileInfo->Geothermal ? FTF_Geothermal : 0) |
		(pFeatureFileInfo->Indestructible ? FTF_Indestructible : 0) |
		(pFeatureFileInfo->Permanent ? FTF_Permanent : 0) |
		(pFeatureFileInfo->NoDisplayInfo ? FTF_NoDisplayInfo : 0);

	// Check if the feature is to use sprites
	if( StringIsValid(pFeatureFileInfo->FileName) )
	{
		// Check if this anim file is not already loaded
		if( stricmp(pFeatureFileInfo->FileName,m_CurrentAnimFileName)!=0 )
		{
			// Not loaded, do so
			sprintf( FilePath, "anims/%s.gaf", pFeatureFileInfo->FileName );
			if( !theFileTree.DumpFileToBuffer(FilePath,&m_CurrentAnimFile) ) {
				theApp.Console.Comment( CT_LOAD, "ERROR: Unable to retrieve \'%s\'", FilePath );
				return FALSE; }
			strcpy( m_CurrentAnimFileName, pFeatureFileInfo->FileName );
		}

		// Create the sprites used to render the feature
		if( bFAILED(CreateFeatureImages(pFeatureFileInfo,&Feature)) )
			return FALSE;
	}
	// No sprites, but maybe a model
	else if( StringIsValid(pFeatureFileInfo->Object) )
	{
		assertEx( 0, "Feature models not yet implemented." );
		return FALSE;
	}
	else // Error
	{
		theApp.Console.Comment( CT_LOAD, "ERROR: Unable to load images or a model for %s.", strName );
		return FALSE;
	}

	// Add this feature type to the list
	m_FeatureTypes[ NameKey ] = Feature;

	// Add some more features to the to-load list
	if( StringIsValid( pFeatureFileInfo->FeatureDead ) ) {
		AddToLoadList( pFeatureFileInfo->FeatureDead, m_LoadPass + 1 );
		m_FeatureTypes[ NameKey ].FeatureDead = std_NameHash( pFeatureFileInfo->FeatureDead ); }
	if( StringIsValid( pFeatureFileInfo->FeatureBurnt ) ) {
		AddToLoadList( pFeatureFileInfo->FeatureBurnt, m_LoadPass + 1 );
		m_FeatureTypes[ NameKey ].FeatureBurnt = std_NameHash( pFeatureFileInfo->FeatureBurnt ); }
	if( StringIsValid( pFeatureFileInfo->FeatureReclamate ) ) {
		AddToLoadList( pFeatureFileInfo->FeatureReclamate, m_LoadPass + 1 );
		m_FeatureTypes[ NameKey ].FeatureReclaimed = std_NameHash( pFeatureFileInfo->FeatureReclamate ); }

	return TRUE;
}
// End feature_Factory::AddFeatureType()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::CreateFeatureImages() //       \author Logan Jones
///////////////////////////////////////////            \date 2/15/2002
//               
//====================================================================
// Parameters:
//  FeatureFileInfo_t* pFeatureFileInfo - 
//  FeatureType_t* pNewFeatureType      - 
//
// Return: BOOL - 
//
BOOL feature_Factory::CreateFeatureImages( FeatureFileInfo_t* pFeatureFileInfo, FeatureType_t* pNewFeatureType )
{
	BOOL					bResult = FALSE;
	LPTA_GAF_FRAME_DATA		pFrameData;
	gfx_Image_t				Image;
	LPTA_GAF_FRAME_DATA		pShadowFrameData;
	gfx_Image_t				ShadowImage;
	gfx_Image_t				FinalImage;

	assert( m_CurrentAnimFile );

	// Start loading the images
	BEGIN_CODE_BLOCK
	{

		// Its not really a feature if there is no image now is there
		if( !StringIsValid(pFeatureFileInfo->SeqName) )
		{
			theApp.Console.Comment( CT_LOAD, "ERROR: No sequence name given" );
			EXIT_CODE_BLOCK;
		}

		// Extract the image from the gaf
		if( bFAILED(ExtractImage(pFeatureFileInfo->SeqName,m_CurrentAnimFile,&Image,(BYTE**)&pFrameData)) )
		{
			EXIT_CODE_BLOCK;
		}		

		// Record the image offset and size
		pNewFeatureType->ViewOffset.Set( pFrameData->XOffset, pFrameData->YOffset );
		pNewFeatureType->ViewSize.Set( pFrameData->Width, pFrameData->Height );
		pNewFeatureType->ImageRect.Set( std_Point(pFrameData->XOffset,pFrameData->YOffset), std_Size(pFrameData->Width,pFrameData->Height) );

		// If there is no shadow, there is no need to go on
		if( !StringIsValid(pFeatureFileInfo->SeqNameShad) )
		{
			if( bFAILED(gfx->CreateSprite( &Image, -std_Vector2(pFrameData->XOffset,pFrameData->YOffset), &pNewFeatureType->Image )) )
			{
				theApp.Console.Comment( CT_LOAD, "ERROR: Failed to create a sprite for %s in \'anims/%s.gaf\'", pFeatureFileInfo->SeqName, m_CurrentAnimFileName );
				bResult = FALSE;
			}
			else bResult = TRUE;
			delete [] Image.pBytes;
			pNewFeatureType->Shadow = NULL;
			EXIT_CODE_BLOCK;
		}

		// Extract the image from the gaf
		if( bFAILED(ExtractImage(pFeatureFileInfo->SeqNameShad,m_CurrentAnimFile,&ShadowImage,(BYTE**)&pShadowFrameData)) )
		{
			EXIT_CODE_BLOCK;
		}

		// Calculate the total area needed for the shadow image
		int RightOfShadow = pShadowFrameData->Width - pShadowFrameData->XOffset;
		int LeftOfShadow = pShadowFrameData->Width - RightOfShadow;
		int BelowShadow = pShadowFrameData->Height - pShadowFrameData->YOffset;
		int AboveShadow = pShadowFrameData->Height - BelowShadow;

		// Calculate the total area needed for the feature image
		int RightOfImage = pFrameData->Width - pFrameData->XOffset;
		int LeftOfImage = pFrameData->Width - RightOfImage;
		int BelowImage = pFrameData->Height - pFrameData->YOffset;
		int AboveImage = pFrameData->Height - BelowImage;

		// Determine the need size of the resulting melded texture
		int RightOfTexture = (RightOfShadow > RightOfImage) ? RightOfShadow : RightOfImage;
		int LeftOfTexture = (LeftOfShadow > LeftOfImage) ? LeftOfShadow : LeftOfImage;
		int BelowTexture = (BelowShadow > BelowImage) ? BelowShadow : BelowImage;
		int AboveTexture = (AboveShadow > AboveImage) ? AboveShadow : AboveImage;

		// Set up the final image structure
		FinalImage.Size.Set( RightOfTexture + LeftOfTexture, BelowTexture + AboveTexture );
		FinalImage.Pitch = FinalImage.Size.width;
		FinalImage.Stride = 1;
		FinalImage.pBytes = new BYTE[ *FinalImage.Size ];
		memset( FinalImage.pBytes, PALETTE_CLEAR, *FinalImage.Size );

		// Place the shadow in the final image at its offset
		BYTE* pFinal = FinalImage.pBytes + ( ((AboveTexture-AboveShadow)*FinalImage.Pitch) + (LeftOfTexture-LeftOfShadow) );
		BYTE* pShadow= ShadowImage.pBytes;
        DWORD y;
		for( y=0; y<pShadowFrameData->Height; y++)
		{
			for( DWORD x=0; x<pShadowFrameData->Width; x++)
				if( pShadow[x]!=PALETTE_CLEAR && pShadow[x]!=PALETTE_CLEAR2 )
					pFinal[x] = PALETTE_SHADOW;
			pFinal += FinalImage.Pitch;
			pShadow += ShadowImage.Pitch;
		}
		delete [] ShadowImage.pBytes;

		// Place the feature in the final image at its offset
		pFinal = FinalImage.pBytes + ( ((AboveTexture-AboveImage)*FinalImage.Pitch) + (LeftOfTexture-LeftOfImage) );
		BYTE* pFeature = Image.pBytes;
		for( y=0; y<pFrameData->Height; y++)
		{
			for( DWORD x=0; x<pFrameData->Width; x++)
				if( pFeature[x]!=PALETTE_CLEAR && pFeature[x]!=PALETTE_CLEAR2 )
					pFinal[x] = pFeature[x];
			pFinal += FinalImage.Pitch;
			pFeature += Image.Pitch;
		}
		delete [] Image.pBytes;

		// Record the image offset and size
		pNewFeatureType->ViewOffset.Set( LeftOfTexture, AboveTexture );
		pNewFeatureType->ViewSize = FinalImage.Size;
		pNewFeatureType->ImageRect.Set( pNewFeatureType->ViewOffset, FinalImage.Size );

		// Create the surface
		if( bFAILED(gfx->CreateSprite( &FinalImage, -std_Vector2(LeftOfTexture,AboveTexture), &pNewFeatureType->Image )) )
		{
			theApp.Console.Comment( CT_LOAD, "ERROR: Failed to create a combined sprite for %s in \'anims/%s.gaf\'", pFeatureFileInfo->SeqName, m_CurrentAnimFileName );
			delete [] FinalImage.pBytes;
			EXIT_CODE_BLOCK;
		}
		delete [] FinalImage.pBytes;

		bResult = TRUE;
	}
	END_CODE_BLOCK
	// End loading the images

	return bResult;
}
// End feature_Factory::CreateFeatureImages()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::ExtractImage() //              \author Logan Jones
////////////////////////////////////                   \date 2/23/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strName      - 
//  BYTE* pGafBuffer    - 
//  gfx_Image_t* pImage - 
//  BYTE** ppFrameData  - 
//
// Return: BOOL - 
//
BOOL feature_Factory::ExtractImage( LPTSTR strName, BYTE* pGafBuffer, gfx_Image_t* pImage, BYTE** ppFrameData )
{
	DWORD					FeatureEntryOffset;
	LPTA_GAF_ENTRY			pEntry;
	LPTA_GAF_FRAME_ENTRY	pFrameEntry;
	LPTA_GAF_FRAME_DATA		pFrameData;

	// Get the entry offset
	FeatureEntryOffset = ta_GAF_GetEntryPointer( pGafBuffer, strName );
	if( FeatureEntryOffset==0 )
	{
		theApp.Console.Comment( CT_LOAD, "ERROR: Could not find %s in \'anims/%s.gaf\'", strName, m_CurrentAnimFileName );
		return FALSE;
	}

	// Get the entry
	pEntry = (LPTA_GAF_ENTRY)(pGafBuffer + FeatureEntryOffset);
	if( pEntry->NumberOfFrames==0 )
	{
		theApp.Console.Comment( CT_LOAD, "ERROR: %s contains no frames in \'anims/%s.gaf\'", strName, m_CurrentAnimFileName );
		return FALSE;
	}

	// The feature is animating
	if( pEntry->NumberOfFrames>1 )
	{
		//theApp.Console.Comment( CT_LOAD, "WARNING: Animating features not implemented yet" );
		// TODO: Handle animating features
	}

	// Get the frame data structure
	pFrameEntry = (LPTA_GAF_FRAME_ENTRY)(pGafBuffer + ( FeatureEntryOffset + sizeof(TA_GAF_ENTRY) ));
	(*ppFrameData) = pGafBuffer + pFrameEntry->OffsetToFrameData;
	pFrameData = (LPTA_GAF_FRAME_DATA)(*ppFrameData);

	// Get the dimensions and allocate the image buffer
	pImage->pBytes = new BYTE[ pFrameData->Width*pFrameData->Height ];
	memset( pImage->pBytes, PALETTE_CLEAR, pFrameData->Width * pFrameData->Height );
	pImage->Size.Set( pFrameData->Width, pFrameData->Height );
	pImage->Pitch = pImage->Size.width;
	pImage->Stride = 1;

	// Get the frame bits
	ta_GAF_GetPalettedFrameBits( 
		pGafBuffer,
		pFrameData,
		pImage->pBytes,
		PALETTE_CLEAR );

	return TRUE;
}
// End feature_Factory::ExtractImage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// feature_Factory::SpawnFeature() //              \author Logan Jones
////////////////////////////////////                   \date 2/13/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strType - 
//  long x          - 
//  long y          - 
//  BYTE z          - 
//
// Return: BOOL - 
//
BOOL feature_Factory::SpawnFeature( LPTSTR strType, long x, long y, BYTE z )
{
	feature_Object* pObject = new feature_Object;
	std_SizeF		Size;

	FeatureType_t* pFeatureType = GetFeatureType( strType );
	if( stricmp(pFeatureType->Name.c_str(),strType)!=0 )
	{
		__unused DWORD A = std_NameHash( strType );
		__unused DWORD B = std_NameHash( pFeatureType->Name.c_str() );
		assert( 0 );
	}
	assert( pFeatureType );
	pObject->Create( std_Point(x,y), pFeatureType );
//	objectManager.InsertObject( pObject );

	return TRUE;
}
// End feature_Factory::SpawnFeature()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - feature_Factory.cpp //
//////////////////////////////
