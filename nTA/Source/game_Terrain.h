// game_Terrain.h //                               \author Logan Jones
///////////////////                                   \date 5/23/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_TERRAIN_H_
#define _GAME_TERRAIN_H_
/////////////////////////////////////////////////////////////////////


#include "game_LoadFacilitator.h"
class gfx_Terrain;

//////////////////////////// game_Terrain ////////////////////////////
//
class game_Terrain
{

/////////////////////////////////////////////////
// Type declarations
public:

	struct MapNode;


/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( LPCTSTR strTntPath, game_LoadFacilitator Load );
	void Destroy();

	INLINE BYTE* GetTntBuffer();
//	INLINE gfx_Terrain* GetGfx();

	void InitTerrain( std_Point ptPos, std_Size szView );
	INLINE void Scroll( std_Point ptScroll );
	INLINE void SetCenter( std_Point ptPos );
	INLINE void Zoom( long lx, long ly );
	INLINE void SetZoom( std_Size szZoom );
	void ApplyCommands();

	INLINE std_Point GetPosition() const;
	INLINE std_Size GetViewSize() const;
	INLINE std_Rect GetRect() const;
	INLINE std_Point GetPoint( const std_Point& ptSrc, const std_Size& szSrcRes, const std_Size& szDestRes );

	INLINE MapNode* GetMapNode( const long lX, const long lY ) const;
	INLINE MapNode* GetMapNode( const std_Point pt ) const;
	INLINE const bool MapPointIsValid( std_Point pt ) const;
	INLINE const long GetMapPitch() const;

	INLINE const std_Size GetTerrainSize() const;
	INLINE const BYTE GetSeaLevel() const;
	INLINE const float GetElevation( const std_Vector2 vPosition ) const;
	INLINE const float GetElevationAboveSeaLevel( const std_Vector2 vPosition ) const;
	INLINE const BYTE GetHeight( const std_Point pt ) const;

	INLINE const std_Point GetMapPoint( const std_Point ptTerrain ) const;


/////////////////////////////////////////////////
// Utility Methods
protected:

	BOOL InitializeMap();

	static int CALLBACK TntFileLoadProc( int iTotal, int iProgress, LPVOID lpLoader );


/////////////////////////////////////////////////
// Data Members
protected:

	BYTE*			m_TntBuffer;

	gfx_Terrain*	m_TerrainGfx;

	bool			m_Centering;
	std_Point		m_Center;

	std_Point		m_ScrollAmount;

	std_Size*		m_GuiResolution;
	std_Size*		m_GuiViewport;
	std_Size		m_CurrentResolution;
	std_Size		m_DesiredResolution;
	std_Size		m_Viewport;

	std_Point		m_Position;
	std_Size		m_Resolution;

	std_Size		m_CurrentZoom;
	std_Size		m_DesiredZoom;

	MapNode*		m_Map;
	std_Size		m_MapSize;


/////////////////////////////////////////////////
// Type declarations
public:

	struct MapNode
	{
		float			BaseHeight;
		std_Vector3		Normal;
		long			OccupationCount_Dynamic;
		long			OccupationCount_Static;
		std_Rect		Screen;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	game_Terrain();
	virtual ~game_Terrain();
/////////////////////////////////////////////////

}; // End class - game_Terrain
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "game_Terrain.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_TERRAIN_H_)
