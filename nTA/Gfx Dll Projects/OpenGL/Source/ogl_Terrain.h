// ogl_Terrain.h //                        Author: Logan "Burn" Jones
//////////////////                                     Date: 2/6/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_TERRAIN_H_
#define _OGL_TERRAIN_H_
/////////////////////////////////////////////////////////////////////


#define GfxTileBits( int_TILE_INDEX )		( m_GfxTiles+((int_TILE_INDEX)*(32*32)) )


//////////////////////////// ogl_Terrain ////////////////////////////
//
class ogl_Terrain : public gfx_Terrain
{
	friend class gfx_OpenGL;

/////////////////////////////////////////////////
// Interface
public:

	// Destroys the terrain and this object
	void Destroy();

	// Renders the terrain image
	void Render() const;

	// Initializes the viewport with image data from the given center point and size
	void InitializeViewport( std_Point ptCenterPosition, std_Size szViewport );

	// Scrolls the viewport to a position centered at the given point
	void CenterViewAt( std_Point ptCenterPosition );

	// Scrolls the view
	void Scroll( std_Point ptScroll );
	void ScrollHorizontaly( long lX, BOOL EarlyOutIsOK=TRUE );
	void ScrollVerticaly( long lY, BOOL EarlyOutIsOK=TRUE );

	// Sets and returns the viewport size respectively
	BOOL SetViewportSize( std_Size szViewport );
	std_Size GetViewportSize() const;

	// Returns a rect with the current terrain viewport
	std_Rect GetViewportRect() const;

	// Returns a rect with the dimensions of the entire map
	std_Rect GetWorldRect() const;


/////////////////////////////////////////////////
// Utility methods
protected:

	// Called by the gfx system to create and initialize the terrain
	BOOL Create( BYTE* pTntBuffer, BOOL NeedsPaletteConversion );

	// Recalculates some of the necessary metrics
	INLINE void RecalculateMetrics();

	// Allocate and initialize the ScreenTile array
	INLINE void AllocateScreenTiles( long iColumns, long iRows );

	// Fills the current screen tile setup with the appropriate map image data
	// SLOW! Uses up a lot of mem bandwidth. Use only on initialization, and worst case
	INLINE void FillScreenTiles();

	// Fills a subset of the screen tile grid with image data
	INLINE void UploadGfxRectToScreenTiles( long LeftIndex, long TopIndex, long FillWidth, long FillHeight );

	// These shift the screen tiles around when neccessary
	INLINE void ShiftColumnsLeft();
	INLINE void ShiftColumnsRight();
	INLINE void ShiftRowsUp();
	INLINE void ShiftRowsDown();


/////////////////////////////////////////////////////////////////////
// Map metrics
protected:

	// These define the largest possible size for the map window
	int				m_MaxMapWindowWidth;
	int				m_MaxMapWindowHeight;

	// This is the size in pixels of a single screen tile
	// ie, a screen tile would contain m_ScreenTileSize x m_ScreenTileSize pixels
	int				m_ScreenTileSize;

	// These define the amount of screen tiles in the screen tile grid
	// ( m_ScreenTileColumns x m_ScreenTileRows )
	int				m_ScreenTileColumns;
	int				m_ScreenTileRows;
	int				m_NumberOfScreenTiles;

	// These indicate the size (in pixels) of the grid
	int				m_ScreenTileGridWidth;
	int				m_ScreenTileGridHeight;

	// These indicate the amount of graphical tile in side a screen tile or grid
	int				m_GfxPerScreenTile;
	int				m_GfxPerGridWidth;
	int				m_GfxPerGridHeight;

	// This is the Z depth at which the visible grid will be rendered
	const int		m_ScreenTileRenderDepth;

	// This is the scrolling metric. When the view offset moves out of this
	// range, the grid is shifted, thus scolled.
	int				m_ComfortZone;


/////////////////////////////////////////////////
// Data members
protected:

	// Pointer to the valid tnt file buffer
	BYTE*			m_pTntBuffer;

	// Holds the current position and dimension of the map viewport
	std_Rect		m_MapView;
	std_Rect		m_MapViewFootprint;

	// The viewable map size measured in pixels
	std_Size		m_MapDimensions;

	// Pointer into the map file at which the gfx tile indeces reside
	WORD*			m_TileIndeces;
	std_Size		m_NumTileIndeces;

	// Pointer into the map file at which the gfx tiles reside
	BYTE*			m_GfxTiles;

	// A screen tile acts as a portion of visible map.
	// As the viewpoint is scrolled, the furthest column or row is
	// moved to its opposite side and its texture is updated.
	struct ScreenTile
	{
		UINT*			pTexture;

		// Neighbors
		ScreenTile*		Left;
		ScreenTile*		Right;
		ScreenTile*		Top;
		ScreenTile*		Bottom;
	};
	ScreenTile*		m_ScreenTiles;
	UINT*			m_ScreenTileTextures;
	UINT			m_ScreenTileDisplayList;
	//std_Size		m_NumberOfScreenTiles;
	std_Point		m_ScreenTilePosition;
	std_Point		m_ScreenTileStartingIndex;
	std_Point		m_ComfortPoint;
	std_Size		m_ScreenTileGridExtent;

	// Pointers to specific tiles in the array
	ScreenTile*		m_TopLeftTile;
	ScreenTile*		m_TopRightTile;
	ScreenTile*		m_BottomLeftTile;
	ScreenTile*		m_BottomRightTile;

	// The boundries of the "clean" rect for the screen tile grid
	// Anything outside needs to be "refreshed" before being displayed
	std_Rect		m_DirtyStrips;

	// TRUE if the terrain should be drawn directly as RGB pixels
	// instead of palette indexed pixels.
	BOOL			m_NeedsPaletteConversion;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	ogl_Terrain();
	virtual ~ogl_Terrain();
/////////////////////////////////////////////////

}; // End class - ogl_Terrain
/////////////////////////////////////////////////////////////////////

// Implement inline methods here for a NON debug build
#ifndef _DEBUG
 #include "ogl_Terrain.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_TERRAIN_H_)
