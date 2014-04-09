// game_Terrain.inl //                             \author Logan Jones
/////////////////////                                 \date 5/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "ta_TNT.h"


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetTntBuffer() //                 \author Logan Jones
/////////////////////////////////                      \date 5/23/2002
//               
//====================================================================
// Return: BYTE* - 
//
BYTE* game_Terrain::GetTntBuffer()
{
	return m_TntBuffer;
}
// End game_Terrain::GetTntBuffer()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// game_Terrain::GetGfx() //                       \author Logan Jones
///////////////////////////                            \date 5/23/2002
//               
//====================================================================
// Return: gfx_Terrain* - 
//
gfx_Terrain* game_Terrain::GetGfx()
{
	return m_TerrainGfx;
}
// End game_Terrain::GetGfx()
//////////////////////////////////////////////////////////////////////

*/
//////////////////////////////////////////////////////////////////////
// game_Terrain::Scroll() //                       \author Logan Jones
///////////////////////////                            \date 6/13/2002
//               
//====================================================================
// Parameters:
//  std_Point ptScroll - 
//
void game_Terrain::Scroll( std_Point ptScroll )
{
	m_ScrollAmount += ptScroll;
}
// End game_Terrain::Scroll()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::SetCenter() //                    \author Logan Jones
//////////////////////////////                         \date 6/13/2002
//               
//====================================================================
// Parameters:
//  std_Point ptPos - 
//
void game_Terrain::SetCenter( std_Point ptPos )
{
	m_Centering = true;
	m_Center = ptPos;
}
// End game_Terrain::SetCenter()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::Zoom() //                         \author Logan Jones
/////////////////////////                              \date 6/13/2002
//               
//====================================================================
// Parameters:
//  long lx - 
//  long ly - 
//
void game_Terrain::Zoom( long lx, long ly )
{
	m_DesiredZoom.Shift( lx, ly );
}
// End game_Terrain::Zoom()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::SetZoom() //                      \author Logan Jones
////////////////////////////                           \date 6/13/2002
//               
//====================================================================
// Parameters:
//  std_Size szZoom - 
//
void game_Terrain::SetZoom( std_Size szZoom )
{
	m_DesiredZoom = szZoom;
}
// End game_Terrain::SetZoom()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetPosition() //                  \author Logan Jones
////////////////////////////////                       \date 6/13/2002
//               
//====================================================================
// Return: std_Point - 
//
std_Point game_Terrain::GetPosition() const
{
	return m_Position;
}
// End game_Terrain::GetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetViewSize() //                  \author Logan Jones
////////////////////////////////                       \date 6/13/2002
//               
//====================================================================
// Return: std_Size - 
//
std_Size game_Terrain::GetViewSize() const
{
	return m_CurrentZoom;
}
// End game_Terrain::GetViewSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetRect() //                      \author Logan Jones
////////////////////////////                           \date 6/15/2002
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect game_Terrain::GetRect() const
{
	return std_Rect( m_Position, m_CurrentZoom );
}
// End game_Terrain::GetRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetPoint() //                     \author Logan Jones
/////////////////////////////                          \date 6/13/2002
//               
//====================================================================
// Parameters:
//  const std_Point& ptSrc    - 
//  const std_Size& szSrcRes  - 
//  const std_Size& szDestRes - 
//
// Return: std_Point - 
//
std_Point game_Terrain::GetPoint( const std_Point& ptSrc, const std_Size& szSrcRes, const std_Size& szDestRes )
{
	std_Point TerrainPoint = ptSrc;
	TerrainPoint.ChangeSpace( szSrcRes, szDestRes );
	return TerrainPoint + m_Position;
}
// End game_Terrain::GetPoint()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetMapNode() //                   \author Logan Jones
///////////////////////////////                        \date 5/23/2002
//               
//====================================================================
// Parameters:
//  const long lX - 
//  const long lY - 
//
// Return: MapNode* - 
//
game_Terrain::MapNode* game_Terrain::GetMapNode( const long lX, const long lY ) const
{
	return m_Map + ((lX+1) + ((lY+1) * m_MapSize.width));
}
// End game_Terrain::GetMapNode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetMapNode() //                   \author Logan Jones
///////////////////////////////                        \date 5/23/2002
//               
//====================================================================
// Parameters:
//  const std_Point pt - 
//
// Return: MapNode* - 
//
game_Terrain::MapNode* game_Terrain::GetMapNode( const std_Point pt ) const
{
	return m_Map + ((pt.x+1) + ((pt.y+1) * m_MapSize.width));
}
// End game_Terrain::GetMapNode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::MapPointIsValid() //              \author Logan Jones
////////////////////////////////////                   \date 5/23/2002
//               
//====================================================================
// Parameters:
//  std_Point pt - 
//
// Return: const bool - 
//
const bool game_Terrain::MapPointIsValid( std_Point pt ) const
{
	pt.Shift(1,1);
	if( pt.x<=0 || pt.y<=0 || pt.x>=m_MapSize.width-1 || pt.y>=m_MapSize.height-1 )
		return false;
	else return true;
}
// End game_Terrain::MapPointIsValid()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetMapPitch() //                  \author Logan Jones
////////////////////////////////                       \date 5/23/2002
//               
//====================================================================
// Return: const long - 
//
const long game_Terrain::GetMapPitch() const
{
	return m_MapSize.width;
}
// End game_Terrain::GetMapPitch()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetTerrainSize() //               \author Logan Jones
///////////////////////////////////                    \date 5/29/2002
//               
//====================================================================
// Return: const std_Size - 
//
const std_Size game_Terrain::GetTerrainSize() const
{
	return std_Size( ((LPTA_TNT_HEADER)m_TntBuffer)->Width, ((LPTA_TNT_HEADER)m_TntBuffer)->Height );
}
// End game_Terrain::GetTerrainSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetSeaLevel() //                  \author Logan Jones
////////////////////////////////                       \date 5/23/2002
//               
//====================================================================
// Return: const BYTE - 
//
const BYTE game_Terrain::GetSeaLevel() const
{
	return ((LPTA_TNT_EXT_HEADER)(m_TntBuffer + sizeof(TA_TNT_HEADER)))->SeaLevel;
}
// End game_Terrain::GetSeaLevel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetElevation() //                 \author Logan Jones
/////////////////////////////////                      \date 5/23/2002
//               
//====================================================================
// Parameters:
//  const std_Vector2 vPosition - 
//
// Return: const float - 
//
const float game_Terrain::GetElevation( const std_Vector2 vPosition ) const
{
	// Get the map node
	const std_Point		NodePos = vPosition / 16;
	const MapNode*		pNode = GetMapNode( NodePos );

	// Claculate the relative position in this node
	const std_Vector2	V = vPosition - (NodePos * 16);

	// Get the height by solving the plane equation, (Ax + By + Cz = D), for z and
	// using V.x and V.y as x and y. (The A,B,C, and D were calculated for the quad at load time)
	return (pNode->BaseHeight - ((pNode->Normal.x*V.x + pNode->Normal.y*V.y) / pNode->Normal.z));
}
// End game_Terrain::GetElevation()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Terrain::GetElevationAboveSeaLevel() //    \author Logan Jones
//////////////////////////////////////////////        \date 1/18/2003
//
//===================================================================
// Parameters:
//  const std_Vector2 vPosition - 
//
// Return: const float - 
//
const float game_Terrain::GetElevationAboveSeaLevel( const std_Vector2 vPosition ) const
{
	// Get the map node
	const std_Point		NodePos = vPosition / 16;
	const MapNode*		pNode = GetMapNode( NodePos );

	// Claculate the relative position in this node
	const std_Vector2	V = vPosition - (NodePos * 16);

	// Get the height by solving the plane equation, (Ax + By + Cz = D), for z and
	// using V.x and V.y as x and y. (The A,B,C, and D were calculated for the quad at load time)
	const float Elevetion = pNode->BaseHeight - ((pNode->Normal.x*V.x + pNode->Normal.y*V.y) / pNode->Normal.z);

	// Return an elevation truncated if below sea level
	return (Elevetion<GetSeaLevel()) ? GetSeaLevel() : Elevetion;
}
// End game_Terrain::GetElevationAboveSeaLevel()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetHeight() //                    \author Logan Jones
//////////////////////////////                         \date 5/23/2002
//               
//====================================================================
// Parameters:
//  const std_Point pt - 
//
// Return: const BYTE - 
//
const BYTE game_Terrain::GetHeight( const std_Point pt ) const
{
	return GetMapNode(pt)->BaseHeight;
}
// End game_Terrain::GetHeight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Terrain::GetMapPoint() //                  \author Logan Jones
////////////////////////////////                       \date 5/23/2002
//               
//====================================================================
// Parameters:
//  const std_Point ptTerrain - 
//
// Return: const std_Point - 
//
const std_Point game_Terrain::GetMapPoint( const std_Point ptTerrain ) const
{
	std_Point		GridPos = ptTerrain / 16;
	MapNode*		pNode = GetMapNode( GridPos );
	while( !pNode->Screen.PointInRect(ptTerrain) )
	{
		++GridPos.y;
		pNode += m_MapSize.width;
	}

	return GridPos * 16;
}
// End game_Terrain::GetMapPoint()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Terrain.inl //
///////////////////////////
