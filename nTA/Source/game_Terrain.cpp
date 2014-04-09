// game_Terrain.cpp //                             \author Logan Jones
/////////////////////                                 \date 5/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "game_Terrain.h"
#include "ta_TNT.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "game_Terrain.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
game_Terrain::game_Terrain():
	m_TntBuffer( NULL ),
	m_TerrainGfx( NULL ),
	m_Map( NULL ),
	m_Centering( false )
{}
game_Terrain::~game_Terrain()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::Create() //                       \author Logan Jones
///////////////////////////                            \date 5/23/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strTntPath        - 
//  game_LoadFacilitator Load - 
//
// Return: BOOL - 
//
BOOL game_Terrain::Create( LPCTSTR strTntPath, game_LoadFacilitator Load )
{
	// Retrieve the tnt file buffer
	if( 0>=theFileTree.DumpFileToBuffer(strTntPath,&m_TntBuffer,TntFileLoadProc,&Load) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to dump file, \'%s\', to a buffer.", strTntPath );
		return FALSE;
	}
	Load.Progress( 90 );

	// Create the gfx
	if( bFAILED(gfx->LoadTerrain( m_TntBuffer, &m_TerrainGfx )) )
		return FALSE;

//	objectManager.Initialize( m_TntBuffer );
	InitializeMap();

	m_ScrollAmount.Nullify();
	m_CurrentZoom.Set( 0, 0 );
	m_DesiredZoom.Set( 0, 0 );

	// Creation complete
	Load.Progress( 100 );
	return TRUE;
}
// End game_Terrain::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::Destroy() //                      \author Logan Jones
////////////////////////////                           \date 5/23/2002
//               
//====================================================================
//
void game_Terrain::Destroy()
{
	SAFE_DELETE_ARRAY( m_TntBuffer );
	SAFE_DESTROY( m_TerrainGfx );
	SAFE_DELETE_ARRAY( m_Map );
}
// End game_Terrain::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::InitializeMap() //                \author Logan Jones
//////////////////////////////////                     \date 5/23/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL game_Terrain::InitializeMap()
{
	LPTA_TNT_HEADER			pHeader;
	LPTA_TNT_EXT_HEADER		pMapHeader;
	LPTA_TNT_MAP_ENTRY		pTntMap;
	long					x, y;
	MapNode*				pNode;
	BYTE					h1,h2,h3,h4;
	std_Vector3				V1,V2,V3,V4,N1,N2;
	std_Rect				VisibilityRect;

	// Get the header
	pHeader = (LPTA_TNT_HEADER)m_TntBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(m_TntBuffer + sizeof(TA_TNT_HEADER));

	VisibilityRect.Set( 0, 0, (pHeader->Width - 2) * 16, (pHeader->Height - 8) * 16 );

	// Set the sea level
//	m_WaterLevel = pMapHeader->SeaLevel;

	m_MapSize.Set( pHeader->Width + 1, pHeader->Height + 1 );
	m_Map = new MapNode[ *m_MapSize ];
	ZeroMemory( m_Map, *m_MapSize * sizeof(MapNode) );

	// Fill the borders
	for( x=0; x<m_MapSize.width; ++x)
		m_Map[x].OccupationCount_Static = 9999,
		m_Map[*m_MapSize - m_MapSize.width + x].OccupationCount_Static = 9999;
	for( y=x=0; y<m_MapSize.height; ++y,x+=m_MapSize.width)
		m_Map[x].OccupationCount_Static = 9999,
		m_Map[x + m_MapSize.width - 1].OccupationCount_Static = 9999;

	pTntMap = (LPTA_TNT_MAP_ENTRY)(m_TntBuffer + pMapHeader->OffsetToMapInfoArray);
	pNode = m_Map + (m_MapSize.width);
	for( y=1; y<(m_MapSize.height-1); ++y,pNode+=m_MapSize.width)
	{
		for( x=1; x<(m_MapSize.width-1); ++x,++pTntMap)
		{
			pNode[x].BaseHeight = pTntMap->Elevation;

			h1 = pTntMap->Elevation;
			h2 = (pTntMap + 1)->Elevation;
			h3 = (pTntMap + pHeader->Width)->Elevation;
			h4 = (pTntMap + (pHeader->Width+1))->Elevation;

			V1.Set(   0, -16, h1-h3 );
			V2.Set(  16,   0, h2-h1 );
			V3.Set(   0,  16, h4-h2 );
			V4.Set( -16,   0, h3-h4 );

			N1 = V1 % V2; N1.Normalize();
			N2 = V3 % V4; N2.Normalize();

			pNode[x].Normal = N1 + N2;
			pNode[x].Normal.Normalize();

			pNode[x].Screen.Set( (x-1)*16, ((y-1)*16) - (__max(h1,h2)/2), (x-1)*16 + 16, ((y-1)*16 + 16) - (__min(h3,h4)/2) );

			if( pNode[x].Screen.left > VisibilityRect.right ||
				pNode[x].Screen.right > VisibilityRect.right ||
				pNode[x].Screen.top < VisibilityRect.top ||
				pNode[x].Screen.bottom < VisibilityRect.top ||
				pNode[x].Screen.top > VisibilityRect.bottom ||
				pNode[x].Screen.bottom > VisibilityRect.bottom )
				pNode[x].OccupationCount_Static = 9119;
		} // end for( x: 1 to m_MapSize.width-1 )
		++pTntMap;
	} // end for( y: 1 to m_MapSize.height-1 )

	return TRUE;
}
// End game_Terrain::InitializeMap()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::InitTerrain() //                  \author Logan Jones
////////////////////////////////                       \date 6/13/2002
//               
//====================================================================
// Parameters:
//  std_Point ptPos - 
//  std_Size szView - 
//
void game_Terrain::InitTerrain( std_Point ptPos, std_Size szView )
{
	m_TerrainGfx->InitializeViewport( ptPos, szView );
	std_Rect rect = m_TerrainGfx->GetViewportRect();
	m_CurrentZoom = m_DesiredZoom = rect.Size();
	m_Position = rect.Position();
}
// End game_Terrain::InitTerrain()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::ApplyCommands() //                \author Logan Jones
//////////////////////////////////                     \date 6/13/2002
//               
//====================================================================
//
void game_Terrain::ApplyCommands()
{
	if( m_Centering )
	{
		m_TerrainGfx->CenterViewAt( m_Center );
		m_Centering = false;
		m_ScrollAmount.Nullify();
	}
	else if( !m_ScrollAmount.IsNull() )
	{
		m_TerrainGfx->Scroll( m_ScrollAmount );
		m_ScrollAmount.Nullify();
	}

	if( m_CurrentZoom!=m_DesiredZoom )
	{
		if( m_TerrainGfx->SetViewportSize(m_DesiredZoom) )
			m_CurrentZoom = m_DesiredZoom;
		else m_DesiredZoom = m_CurrentZoom;
	}

	m_Position = m_TerrainGfx->GetViewportRect().Position();
}
// End game_Terrain::ApplyCommands()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::TntFileLoadProc() //              \author Logan Jones
////////////////////////////////////                   \date 5/23/2002
//               
//====================================================================
// Parameters:
//  int iTotal      - 
//  int iProgress   - 
//  LPVOID lpLoader - 
//
// Return: int CALLBACK - 
//
int CALLBACK game_Terrain::TntFileLoadProc( int iTotal, int iProgress, LPVOID lpLoader )
{
	game_LoadFacilitator* pLoader = (game_LoadFacilitator*)lpLoader;
	pLoader->Progress( iProgress * 90.0f / iTotal );
	return 1;
}
// End game_Terrain::TntFileLoadProc()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Terrain.cpp //
///////////////////////////
