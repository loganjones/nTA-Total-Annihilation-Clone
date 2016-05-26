// ogl_Terrain.cpp //                      Author: Logan "Burn" Jones
////////////////////                                   Date: 2/6/2002
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Terrain.h"
#include "../../../Source/ta_TNT.h"

// Implement inline methods here for a debug build
#ifdef _DEBUG
 #include "ogl_Terrain.inl"
#endif // defined( _DEBUG )


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::LoadTerrain() //             Author: Logan "Burn" Jones
//////////////////////////////                          Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntBuffer        - 
//  gfx_Terrain** ppTerrain - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::LoadTerrain( BYTE* pTntBuffer, gfx_Terrain** ppTerrain )
{
	LPTA_TNT_HEADER			pHeader;
	LPTA_TNT_EXT_HEADER		pMapHeader;

	// Sanity checks
	if( pTntBuffer==NULL || ppTerrain==NULL )
		return FALSE;

	// Get the header
	pHeader = (LPTA_TNT_HEADER)pTntBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(pTntBuffer + sizeof(TA_TNT_HEADER));

	// Make sure this is a TA tnt file (TAK not accepted yet)
	if( pHeader->Version!=TA_TNT_TOTAL_ANNIHILATION )
		return FALSE;

	// Set the return pointer to the terrain member and return the result of the terrain creation
	(*ppTerrain) = &m_theTerrain;
	return m_theTerrain.Create( pTntBuffer, glColorTableEXT == NULL );
}
// End gfx_OpenGL::LoadTerrain()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Terrain::ogl_Terrain():
	m_pTntBuffer( NULL ),
// Default map metrics
	m_MaxMapWindowWidth( 1600 ),
	m_MaxMapWindowHeight( 1200 ),
	m_ScreenTileSize( 256 ),
	m_ScreenTileColumns( (m_MaxMapWindowWidth / m_ScreenTileSize) + 2 ),
	m_ScreenTileRows( (m_MaxMapWindowHeight / m_ScreenTileSize) + 2 ),
	m_NumberOfScreenTiles( m_ScreenTileColumns * m_ScreenTileRows ),
	m_ScreenTileGridWidth( m_ScreenTileColumns * m_ScreenTileSize ),
	m_ScreenTileGridHeight( m_ScreenTileRows * m_ScreenTileSize ),
	m_GfxPerScreenTile( m_ScreenTileSize / 32 ),
	m_GfxPerGridWidth( m_GfxPerScreenTile * m_ScreenTileColumns ),
	m_GfxPerGridHeight( m_GfxPerScreenTile * m_ScreenTileRows ),
	m_ScreenTileRenderDepth( -1 ),
	m_ComfortZone( m_ScreenTileSize / 2 ),
// Screen tiles
	m_ScreenTiles(NULL)
{}
ogl_Terrain::~ogl_Terrain()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::Create() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntBuffer - 
//
// Return: BOOL - 
//
BOOL ogl_Terrain::Create( BYTE* pTntBuffer, BOOL NeedsPaletteConversion )
{
	LPTA_TNT_HEADER			pHeader;
	LPTA_TNT_EXT_HEADER		pMapHeader;

	// Set the buffer pointer
	m_pTntBuffer = pTntBuffer;

	// Get the header
	pHeader = (LPTA_TNT_HEADER)m_pTntBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(m_pTntBuffer + sizeof(TA_TNT_HEADER));

	m_MapDimensions.width = (pHeader->Width *16)-32;
	m_MapDimensions.height = (pHeader->Height*16)-128;

	if( (m_ScreenTileGridWidth>m_MapDimensions.width)||(m_ScreenTileGridHeight>m_MapDimensions.height) )
		RecalculateMetrics();

	m_ScreenTileGridExtent.width = (m_MapDimensions.width/m_ScreenTileSize)*m_ScreenTileSize;
	if( m_ScreenTileGridExtent.width<m_MapDimensions.width )
		m_ScreenTileGridExtent.width += m_ScreenTileSize;
	m_ScreenTileGridExtent.height = (m_MapDimensions.height/m_ScreenTileSize)*m_ScreenTileSize;
	if( m_ScreenTileGridExtent.height<m_MapDimensions.height )
		m_ScreenTileGridExtent.height += m_ScreenTileSize;

	m_TileIndeces = (WORD*)(pTntBuffer + pMapHeader->OffsetToTileIndexArray);
	m_NumTileIndeces.width = pHeader->Width / 2;
	m_NumTileIndeces.height = pHeader->Height/ 2;

	m_GfxTiles = pTntBuffer + pMapHeader->OffsetToTileArray;

	m_NeedsPaletteConversion = NeedsPaletteConversion;

	AllocateScreenTiles( m_ScreenTileColumns, m_ScreenTileRows );

	// Return success
	return TRUE;
}
// End ogl_Terrain::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::Destroy() //                Author: Logan "Burn" Jones
///////////////////////////                             Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::Destroy()
{
	if( m_ScreenTiles )
	{
		glDeleteLists( m_ScreenTileDisplayList, 1 );
		glDeleteTextures( m_NumberOfScreenTiles, m_ScreenTileTextures );
		SAFE_DELETE_ARRAY( m_ScreenTileTextures );
		SAFE_DELETE_ARRAY( m_ScreenTiles );
	}
}
// End ogl_Terrain::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::Render() const
{
	ScreenTile*		pTile,*pStartRow = m_TopLeftTile;
	long XOffset = m_ScreenTilePosition.x;
	long YOffset = m_ScreenTilePosition.y;

#ifdef VISUALIZE_MAP_ALGORITHM
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, m_ScreenTileGridWidth+256, m_ScreenTileGridHeight+256, 0, -256, 1024 );
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef( 128, 128, 0 );
#endif

	glPushMatrix();

#ifdef VISUALIZE_MAP_ALGORITHM
	glDisable( GL_TEXTURE_2D );
	glPushMatrix();
	glTranslatef( m_MapView.left - m_ScreenTilePosition.x,
				  m_MapView.top - m_ScreenTilePosition.y, 0 );
	glColor3f(1,0,1);
	glBegin( GL_LINE_LOOP );
		glVertex2i( 0,0 );
		glVertex2i( 0,m_MapView.Height() );
		glVertex2i( m_MapView.Width(),m_MapView.Height() );
		glVertex2i( m_MapView.Width(),0 );
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslatef( (m_MapViewFootprint.left*32) - m_ScreenTilePosition.x,
				  (m_MapViewFootprint.top*32) - m_ScreenTilePosition.y, 0 );
	glColor3i(100,100,100);
	glBegin( GL_LINE_LOOP );
		glVertex2i( 0,0 );
		glVertex2i( 0,(m_MapViewFootprint.bottom-m_MapViewFootprint.top)*32 );
		glVertex2i( (m_MapViewFootprint.right-m_MapViewFootprint.left)*32,(m_MapViewFootprint.bottom-m_MapViewFootprint.top)*32 );
		glVertex2i( (m_MapViewFootprint.right-m_MapViewFootprint.left)*32,0 );
	glEnd();
	glPopMatrix();
	glColor3f(1,1,1);
	glEnable( GL_TEXTURE_2D );
#else
	glTranslatef( (float)(m_ScreenTilePosition.x - m_MapView.left),
				  (float)(m_ScreenTilePosition.y - m_MapView.top), 0 );
#endif

	for( ;pStartRow; pStartRow = pStartRow->Bottom)
	{
		glPushMatrix();
		XOffset = m_ScreenTilePosition.x;
		for( pTile=pStartRow; pTile; pTile = pTile->Right,XOffset+=m_ScreenTileSize)
		{
			if( !(((XOffset+m_ScreenTileSize)<m_MapView.left) ||
				  (XOffset>m_MapView.right) ||
				  ((YOffset+m_ScreenTileSize)<m_MapView.top)) )
			{
#ifdef VISUALIZE_MAP_ALGORITHM
				if( YOffset>m_MapView.bottom ) glColor3f( 0.5, 0.5, 0.5 );
#endif
				glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );
				glCallList( m_ScreenTileDisplayList );
			}
#ifdef VISUALIZE_MAP_ALGORITHM
			else
			{
				glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );
				glColor3f( 0.5, 0.5, 0.5 );
				glCallList( m_ScreenTileDisplayList );
				pGfxSystem->m_TriangleCount += 2;
			}
			glColor3f(1,1,1);
#endif
			glTranslatef( (float)m_ScreenTileSize, 0, 0 );

		} // end for( pTile )
		glPopMatrix();
		glTranslatef( 0, (float)m_ScreenTileSize, 0 );
		YOffset += m_ScreenTileSize;

#ifndef VISUALIZE_MAP_ALGORITHM
		if( YOffset>m_MapView.bottom )
			break;
#endif

	} // end for( pStartRow )
	glPopMatrix();

#ifdef VISUALIZE_MAP_ALGORITHM

	glDisable( GL_TEXTURE_2D );
	glPointSize( 3 );
	glBegin( GL_POINTS );
		glColor3f(1,0,0);
		glVertex2i( m_ComfortPoint.x, m_ComfortPoint.y );
	glEnd();
	glPointSize( 1 );
	glColor4f(1,1,0,0.33f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin( GL_QUADS );
		glVertex2i( (m_DirtyStrips.left*32)-m_ScreenTilePosition.x, 0 );
		glVertex2i( (m_DirtyStrips.left*32)-m_ScreenTilePosition.x, m_ScreenTileGridHeight );
		glVertex2i( (m_DirtyStrips.left*32)-m_ScreenTilePosition.x+32, m_ScreenTileGridHeight );
		glVertex2i( (m_DirtyStrips.left*32)-m_ScreenTilePosition.x+32, 0 );
		glVertex2i( 0, (m_DirtyStrips.top*32)-m_ScreenTilePosition.y );
		glVertex2i( m_ScreenTileGridWidth, (m_DirtyStrips.top*32)-m_ScreenTilePosition.y );
		glVertex2i( m_ScreenTileGridWidth, (m_DirtyStrips.top*32)-m_ScreenTilePosition.y+32 );
		glVertex2i( 0, (m_DirtyStrips.top*32)-m_ScreenTilePosition.y+32 );
		glVertex2i( (m_DirtyStrips.right*32)-m_ScreenTilePosition.x, 0 );
		glVertex2i( (m_DirtyStrips.right*32)-m_ScreenTilePosition.x, m_ScreenTileGridHeight );
		glVertex2i( (m_DirtyStrips.right*32)-m_ScreenTilePosition.x+32, m_ScreenTileGridHeight );
		glVertex2i( (m_DirtyStrips.right*32)-m_ScreenTilePosition.x+32, 0 );
		glVertex2i( 0, (m_DirtyStrips.bottom*32)-m_ScreenTilePosition.y );
		glVertex2i( m_ScreenTileGridWidth, (m_DirtyStrips.bottom*32)-m_ScreenTilePosition.y );
		glVertex2i( m_ScreenTileGridWidth, (m_DirtyStrips.bottom*32)-m_ScreenTilePosition.y+32 );
		glVertex2i( 0, (m_DirtyStrips.bottom*32)-m_ScreenTilePosition.y+32 );
	glEnd();
	glDisable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );


	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glColor3f(1,1,1);
#endif
}
// End ogl_Terrain::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::InitializeViewport() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptCenterPosition - 
//  std_Size szViewport        - 
//
void ogl_Terrain::InitializeViewport( std_Point ptCenterPosition, std_Size szViewport )
{
	// Sanity checks
	assert( (szViewport.width<=m_MaxMapWindowWidth)&&(szViewport.height<=m_MaxMapWindowHeight) );

	// Calculate the proposed left corner of the viewport
	m_MapView.left = ptCenterPosition.x - (szViewport.width / 2);
	m_MapView.top = ptCenterPosition.y - (szViewport.height / 2);

	// Check if the desired coordinates are out of bounds
	if( m_MapView.left<0 ) m_MapView.left = 0;
	else if( m_MapView.left>(m_MapDimensions.width-szViewport.width) )
		m_MapView.left = (m_MapDimensions.width-szViewport.width);
	if( m_MapView.top<0 ) m_MapView.top = 0;
	else if( m_MapView.top>(m_MapDimensions.height-szViewport.height) )
		m_MapView.top = (m_MapDimensions.height-szViewport.height);

	// Add the right and bottom
	m_MapView.right = m_MapView.left + szViewport.width;
	m_MapView.bottom = m_MapView.top + szViewport.height;

	m_MapViewFootprint.left = m_MapView.left / 32;
	m_MapViewFootprint.top = m_MapView.top / 32;
	m_MapViewFootprint.right = m_MapView.right / 32;
	if( (m_MapViewFootprint.right*32)<m_MapView.right ) m_MapViewFootprint.right++;
	m_MapViewFootprint.bottom = m_MapView.bottom / 32;
	if( (m_MapViewFootprint.bottom*32)<m_MapView.bottom ) m_MapViewFootprint.bottom++;

	// Calculate the scrolling comfort point for this zoom level
	m_ComfortPoint.x = ((m_ScreenTileGridWidth)/2) - (szViewport.width/2);
	m_ComfortPoint.y = ((m_ScreenTileGridHeight)/2) - (szViewport.height/2);

	// Calculate the stating position for the grid. MUST be alligned on the grid
	// (ie every m_ScreenTileSize pixels)
	// Check for out of bounds here as well
	long Closest,Possible1,Possible2;
	std_Point NewScreenTilePoint;
	Closest = ptCenterPosition.x - (m_ScreenTileGridWidth / 2);
	if( Closest<m_ScreenTileSize )
		NewScreenTilePoint.x = 0;
	else if( (Closest+m_ScreenTileGridWidth)>(m_ScreenTileGridExtent.width) )
		NewScreenTilePoint.x = m_ScreenTileGridExtent.width - m_ScreenTileGridWidth;
	else
	{
		Possible1 = ((Closest/m_ScreenTileSize)*m_ScreenTileSize);
		Possible2 = Possible1 + m_ScreenTileSize;
		NewScreenTilePoint.x = ( (Closest-Possible1)<(Possible2-Closest) )? Possible1:Possible2;
	}
	Closest = ptCenterPosition.y - (m_ScreenTileGridHeight / 2);
	if( Closest<m_ScreenTileSize )
		NewScreenTilePoint.y = 0;
	else if( (Closest+m_ScreenTileGridHeight)>(m_ScreenTileGridExtent.height) )
		NewScreenTilePoint.y = m_ScreenTileGridExtent.height - m_ScreenTileGridHeight;
	else
	{
		Possible1 = ((Closest/m_ScreenTileSize)*m_ScreenTileSize);
		Possible2 = Possible1 + m_ScreenTileSize;
		NewScreenTilePoint.y = ( (Closest-Possible1)<(Possible2-Closest) )? Possible1:Possible2;
	}

	// Get both the starting indeces and top left position from the new point
	m_ScreenTileStartingIndex.x = NewScreenTilePoint.x / 32;
	m_ScreenTileStartingIndex.y = NewScreenTilePoint.y / 32;
	m_ScreenTilePosition.x = m_ScreenTileStartingIndex.x * 32;
	m_ScreenTilePosition.y = m_ScreenTileStartingIndex.y * 32;

	m_DirtyStrips.left = m_ScreenTileStartingIndex.x - 1;
	m_DirtyStrips.top = m_ScreenTileStartingIndex.y - 1;
	m_DirtyStrips.right = m_ScreenTileStartingIndex.x + (m_GfxPerScreenTile*m_ScreenTileColumns);
	m_DirtyStrips.bottom = m_ScreenTileStartingIndex.y + (m_GfxPerScreenTile*m_ScreenTileRows);

	// Fill the screen tile grid
	FillScreenTiles();
}
// End ogl_Terrain::InitializeViewport()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::CenterViewAt() //           Author: Logan "Burn" Jones
////////////////////////////////                        Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptCenterPosition - 
//
void ogl_Terrain::CenterViewAt( std_Point ptCenterPosition )
{
	// Calculate the scroll distance to the new point
	std_Point ScrollDelta(	ptCenterPosition.x - (m_MapView.left + (m_MapView.Width() / 2)),
							ptCenterPosition.y - (m_MapView.top + (m_MapView.Height() / 2)) );

	// Check how far we are supposed to move
	if( (ScrollDelta.x>m_MaxMapWindowWidth)||(ScrollDelta.x<(0-m_MaxMapWindowWidth))||
		(ScrollDelta.y>m_MaxMapWindowHeight)||(ScrollDelta.y<(0-m_MaxMapWindowHeight)) )
	{
		// That's a long way, re-initialize the view
		InitializeViewport( ptCenterPosition, m_MapView.Size() );
	}
	else
	{
		// It's not that far, just scroll there
		Scroll( ScrollDelta );
	}
}
// End ogl_Terrain::CenterViewAt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::Scroll() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptScroll - 
//
void ogl_Terrain::Scroll( std_Point ptScroll )
{
	ScrollHorizontaly( ptScroll.x );
	ScrollVerticaly( ptScroll.y );
	return;
#if 0 // Unreachable Code
    
	// Make a copy the viewport and shift it with the proposed values
	std_Rect NewView( m_MapView + ptScroll );

	// Check if the new coordinate is out of bounds
	if( NewView.left<0 ) ptScroll.x = 0 - m_MapView.left;
	else if( NewView.right>(m_MapDimensions.width) )
		ptScroll.x = (m_MapDimensions.width - m_MapView.right);
	if( NewView.top<0 ) ptScroll.y = 0 - m_MapView.top;
	else if( NewView.bottom>(m_MapDimensions.height) )
		ptScroll.y = (m_MapDimensions.height - m_MapView.bottom);

	// Shift the viewport with the adjusted values
	m_MapView += ptScroll;

	m_MapViewFootprint.left = m_MapView.left / 32;
	m_MapViewFootprint.top = m_MapView.top / 32;
	m_MapViewFootprint.right = m_MapView.right / 32;
	if( (m_MapViewFootprint.right*32)<m_MapView.right ) m_MapViewFootprint.right++;
	m_MapViewFootprint.bottom = m_MapView.bottom / 32;
	if( (m_MapViewFootprint.bottom*32)<m_MapView.bottom ) m_MapViewFootprint.bottom++;

//	InitViewAt( NewView.left+((NewView.right-NewView.left)/2),
//				NewView.top+((NewView.bottom-NewView.top)/2),
//				NewView.right-NewView.left,
//				NewView.bottom-NewView.top );

	// Check for a horizontal grid shift
	if( (m_ComfortPoint.x-(m_MapView.left-m_ScreenTilePosition.x))>m_ComfortZone )
	{
		if( m_ScreenTileStartingIndex.x>=m_GfxPerScreenTile )
		{
			m_ScreenTilePosition.x -= m_ScreenTileSize;
			m_ScreenTileStartingIndex.x -= m_GfxPerScreenTile;
			ShiftColumnsRight();
		}
	}
	else if( ((m_MapView.left-m_ScreenTilePosition.x)-m_ComfortPoint.x)>m_ComfortZone )
	{
		if( m_ScreenTileStartingIndex.x<=(m_NumTileIndeces.width-(m_ScreenTileColumns*m_GfxPerScreenTile)-1) )
		{
			m_ScreenTilePosition.x += m_ScreenTileSize;
			m_ScreenTileStartingIndex.x += m_GfxPerScreenTile;
			ShiftColumnsLeft();
		}
	}
	
	// Check for a vertical grid shift
	if( (m_ComfortPoint.y-(m_MapView.top-m_ScreenTilePosition.y))>m_ComfortZone )
	{
		if( m_ScreenTileStartingIndex.y>=m_GfxPerScreenTile )
		{
			m_ScreenTilePosition.y -= m_ScreenTileSize;
			m_ScreenTileStartingIndex.y -= m_GfxPerScreenTile;
			ShiftRowsDown();
		}
	}
	else if( ((m_MapView.top-m_ScreenTilePosition.y)-m_ComfortPoint.y)>m_ComfortZone )
	{
		if( m_ScreenTileStartingIndex.y<=(m_NumTileIndeces.height-(m_ScreenTileRows*m_GfxPerScreenTile)-4) )
		{
			m_ScreenTilePosition.y += m_ScreenTileSize;
			m_ScreenTileStartingIndex.y += m_GfxPerScreenTile;
			ShiftRowsUp();
		}
	}

	if( m_MapViewFootprint.left<=m_DirtyStrips.left )
	{
		long ShiftCount = (m_DirtyStrips.left - m_MapViewFootprint.left) + 1;
		//UploadToLeftDirtyTiles( ShiftCount );
		UploadGfxRectToScreenTiles( m_DirtyStrips.left - (ShiftCount-1),
									m_MapViewFootprint.top,
									ShiftCount,
									m_MapViewFootprint.bottom - m_MapViewFootprint.top + 1 );
		m_DirtyStrips.left -= ShiftCount;
		m_DirtyStrips.right -= ShiftCount;
		m_DirtyStrips.top = m_MapViewFootprint.top - 1;
		m_DirtyStrips.bottom = m_MapViewFootprint.bottom;
	}
	else if( m_MapViewFootprint.right>m_DirtyStrips.right )
	{
		long ShiftCount = (m_MapViewFootprint.right - m_DirtyStrips.right);
		UploadGfxRectToScreenTiles( m_DirtyStrips.right,
									m_MapViewFootprint.top,
									ShiftCount,
									m_MapViewFootprint.bottom - m_MapViewFootprint.top + 1 );
		m_DirtyStrips.left += ShiftCount;
		m_DirtyStrips.right += ShiftCount;
		m_DirtyStrips.top = m_MapViewFootprint.top - 1;
		m_DirtyStrips.bottom = m_MapViewFootprint.bottom;
	}
	if( m_MapViewFootprint.top<=m_DirtyStrips.top )
	{
		long ShiftCount = (m_DirtyStrips.top - m_MapViewFootprint.top) + 1;
		UploadGfxRectToScreenTiles( m_MapViewFootprint.left,
									m_DirtyStrips.top - (ShiftCount-1),
									m_MapViewFootprint.right - m_MapViewFootprint.left + 1,
									ShiftCount );
		m_DirtyStrips.top -= ShiftCount;
		m_DirtyStrips.bottom -= ShiftCount;
		m_DirtyStrips.left = m_MapViewFootprint.left - 1;
		m_DirtyStrips.right = m_MapViewFootprint.right;
	}
	else if( m_MapViewFootprint.bottom>m_DirtyStrips.bottom )
	{
		long ShiftCount = (m_MapViewFootprint.bottom - m_DirtyStrips.bottom);
		UploadGfxRectToScreenTiles( m_MapViewFootprint.left,
									m_DirtyStrips.bottom,
									m_MapViewFootprint.right - m_MapViewFootprint.left + 1,
									ShiftCount);
		m_DirtyStrips.top += ShiftCount;
		m_DirtyStrips.bottom += ShiftCount;
		m_DirtyStrips.left = m_MapViewFootprint.left - 1;
		m_DirtyStrips.right = m_MapViewFootprint.right;
	}
#endif
}
// End ogl_Terrain::Scroll()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ScrollHorizontaly() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long lX           - 
//  BOOL EarlyOutIsOK - 
//
void ogl_Terrain::ScrollHorizontaly( long lX, BOOL EarlyOutIsOK )
{
	// Make a copy the viewport and shift it with the proposed values
	std_Rect NewView = m_MapView;
	NewView.left += lX;
	NewView.right += lX;

	// Check if the new coordinate is out of bounds
	if( NewView.left<0 ) lX = 0 - m_MapView.left;
	else if( NewView.right>(m_MapDimensions.width) )
		lX = (m_MapDimensions.width - m_MapView.right);

	if( (lX==0)&&EarlyOutIsOK ) return;

	// Shift the viewport with the adjusted values
	m_MapView.left += lX;
	m_MapView.right += lX;

	m_MapViewFootprint.left = m_MapView.left / 32;
	m_MapViewFootprint.right = m_MapView.right / 32;
	if( (m_MapViewFootprint.right*32)<m_MapView.right ) m_MapViewFootprint.right++;

	// Check for a horizontal grid shift
	if( (m_ComfortPoint.x-(m_MapView.left-m_ScreenTilePosition.x))>m_ComfortZone )
	{
		do {
			if( m_ScreenTileStartingIndex.x>=m_GfxPerScreenTile )
			{
				m_ScreenTilePosition.x -= m_ScreenTileSize;
				m_ScreenTileStartingIndex.x -= m_GfxPerScreenTile;
				ShiftColumnsRight();
			}
			else break;
		} while( (m_ComfortPoint.x-(m_MapView.left-m_ScreenTilePosition.x))>m_ComfortZone );
	}
	else if( ((m_MapView.left-m_ScreenTilePosition.x)-m_ComfortPoint.x)>m_ComfortZone )
	{
		do {
			if( m_ScreenTileStartingIndex.x<=(m_NumTileIndeces.width-(m_ScreenTileColumns*m_GfxPerScreenTile)-1) )
			{
				m_ScreenTilePosition.x += m_ScreenTileSize;
				m_ScreenTileStartingIndex.x += m_GfxPerScreenTile;
				ShiftColumnsLeft();
			}
			else break;
		} while( ((m_MapView.left-m_ScreenTilePosition.x)-m_ComfortPoint.x)>m_ComfortZone );
	}

	if( m_MapViewFootprint.left<=m_DirtyStrips.left )
	{
		long ShiftCount = (m_DirtyStrips.left - m_MapViewFootprint.left) + 1;
		//UploadToLeftDirtyTiles( ShiftCount );
		UploadGfxRectToScreenTiles( m_DirtyStrips.left - (ShiftCount-1),
									m_MapViewFootprint.top,
									ShiftCount,
									m_MapViewFootprint.bottom - m_MapViewFootprint.top + 1 );
		m_DirtyStrips.left -= ShiftCount;
		if( (m_DirtyStrips.right-m_DirtyStrips.left)>m_GfxPerGridWidth )
			m_DirtyStrips.right -= ShiftCount;
		m_DirtyStrips.top = m_MapViewFootprint.top - 1;
		m_DirtyStrips.bottom = m_MapViewFootprint.bottom;
	}
	if( m_MapViewFootprint.right>m_DirtyStrips.right )
	{
		long ShiftCount = (m_MapViewFootprint.right - m_DirtyStrips.right);
		UploadGfxRectToScreenTiles( m_DirtyStrips.right,
									m_MapViewFootprint.top,
									ShiftCount,
									m_MapViewFootprint.bottom - m_MapViewFootprint.top + 1 );
		m_DirtyStrips.right += ShiftCount;
		if( (m_DirtyStrips.right-m_DirtyStrips.left)>m_GfxPerGridWidth )
			m_DirtyStrips.left += ShiftCount;
		m_DirtyStrips.top = m_MapViewFootprint.top - 1;
		m_DirtyStrips.bottom = m_MapViewFootprint.bottom;
	}
}
// End ogl_Terrain::ScrollHorizontaly()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ScrollVerticaly() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long lY           - 
//  BOOL EarlyOutIsOK - 
//
void ogl_Terrain::ScrollVerticaly( long lY, BOOL EarlyOutIsOK )
{
	// Make a copy the viewport and shift it with the proposed values
	std_Rect NewView = m_MapView;
	NewView.top += lY;
	NewView.bottom += lY;

	// Check if the new coordinate is out of bounds
	if( NewView.top<0 ) lY = 0 - m_MapView.top;
	else if( NewView.bottom>(m_MapDimensions.height) )
		lY = (m_MapDimensions.height - m_MapView.bottom);

	if( (lY==0)&&EarlyOutIsOK ) return;

	// Shift the viewport with the adjusted values
	m_MapView.top += lY;
	m_MapView.bottom += lY;

	m_MapViewFootprint.top = m_MapView.top / 32;
	m_MapViewFootprint.bottom = m_MapView.bottom / 32;
	if( (m_MapViewFootprint.bottom*32)<m_MapView.bottom ) m_MapViewFootprint.bottom++;

	// Check for a vertical grid shift
	if( (m_ComfortPoint.y-(m_MapView.top-m_ScreenTilePosition.y))>m_ComfortZone )
	{
		do {
			if( m_ScreenTileStartingIndex.y>=m_GfxPerScreenTile )
			{
				m_ScreenTilePosition.y -= m_ScreenTileSize;
				m_ScreenTileStartingIndex.y -= m_GfxPerScreenTile;
				ShiftRowsDown();
			}
			else break;
		} while( (m_ComfortPoint.y-(m_MapView.top-m_ScreenTilePosition.y))>m_ComfortZone );
	}
	else if( ((m_MapView.top-m_ScreenTilePosition.y)-m_ComfortPoint.y)>m_ComfortZone )
	{
		do {
			if( m_ScreenTileStartingIndex.y<=(m_NumTileIndeces.height-(m_ScreenTileRows*m_GfxPerScreenTile)-4) )
			{
				m_ScreenTilePosition.y += m_ScreenTileSize;
				m_ScreenTileStartingIndex.y += m_GfxPerScreenTile;
				ShiftRowsUp();
			}
			else break;
		} while( ((m_MapView.top-m_ScreenTilePosition.y)-m_ComfortPoint.y)>m_ComfortZone );
	}

	if( m_MapViewFootprint.top<=m_DirtyStrips.top )
	{
		long ShiftCount = (m_DirtyStrips.top - m_MapViewFootprint.top) + 1;
		UploadGfxRectToScreenTiles( m_MapViewFootprint.left,
									m_DirtyStrips.top - (ShiftCount-1),
									m_MapViewFootprint.right - m_MapViewFootprint.left + 1,
									ShiftCount );
		m_DirtyStrips.top -= ShiftCount;
		if( (m_DirtyStrips.bottom-m_DirtyStrips.top)>m_GfxPerGridHeight )
			m_DirtyStrips.bottom -= ShiftCount;
		m_DirtyStrips.left = m_MapViewFootprint.left - 1;
		m_DirtyStrips.right = m_MapViewFootprint.right;
	}
	if( m_MapViewFootprint.bottom>m_DirtyStrips.bottom )
	{
		long ShiftCount = (m_MapViewFootprint.bottom - m_DirtyStrips.bottom);
		UploadGfxRectToScreenTiles( m_MapViewFootprint.left,
									m_DirtyStrips.bottom,
									m_MapViewFootprint.right - m_MapViewFootprint.left + 1,
									ShiftCount);
		m_DirtyStrips.bottom += ShiftCount;
		if( (m_DirtyStrips.bottom-m_DirtyStrips.top)>m_GfxPerGridHeight )
			m_DirtyStrips.top += ShiftCount;
		m_DirtyStrips.left = m_MapViewFootprint.left - 1;
		m_DirtyStrips.right = m_MapViewFootprint.right;
	}
}
// End ogl_Terrain::ScrollVerticaly()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::SetViewportSize() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Size szViewport - 
//
// Return: BOOL - 
//
BOOL ogl_Terrain::SetViewportSize( std_Size szViewport )
{
	// Sanity check
	if( (szViewport.width<=0)||(szViewport.height<=0) )
		return FALSE;

	// No larger than this please
	if( (szViewport.width>m_MaxMapWindowWidth)||(szViewport.height>m_MaxMapWindowHeight) )
		return FALSE;

	// Check if the desired viewport is larger then the actual map
	if( !(szViewport < m_MapDimensions) )
		return FALSE;

	// Check for redundancy
	if( (m_MapView.Width()==szViewport.width)&&(m_MapView.Height()==szViewport.height) )
		return FALSE;

	//InitViewAt( m_MapView.left + ((m_MapView.right-m_MapView.left)/2), m_MapView.top + ((m_MapView.bottom-m_MapView.top)/2), szViewport.width, szViewport.height );

	// Calculate the dimensions difference
	long WidthDifference = m_MapView.Width() - szViewport.width;
	long HeightDifference = m_MapView.Height() - szViewport.height;

	// Change the view
	m_MapView.right = m_MapView.left + szViewport.width;
	m_MapView.bottom = m_MapView.top + szViewport.height;

	// Calculate the scrolling comfort point for this zoom level
	m_ComfortPoint.x = ((m_ScreenTileGridWidth)/2) - (szViewport.width/2);
	m_ComfortPoint.y = ((m_ScreenTileGridHeight)/2) - (szViewport.height/2);

	// Scroll the new view to the same center as before
	ScrollHorizontaly( WidthDifference/2, FALSE );
	ScrollVerticaly( HeightDifference/2, FALSE );

	return TRUE;
}
// End ogl_Terrain::SetViewportSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::GetViewportSize() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Return: std_Size - 
//
std_Size ogl_Terrain::GetViewportSize() const
{
	return m_MapView.Size();
}
// End ogl_Terrain::GetViewportSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::GetViewportRect() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect ogl_Terrain::GetViewportRect() const
{
	return( m_MapView );
}
// End ogl_Terrain::GetViewportRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::GetWorldRect() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 2/12/2002
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect ogl_Terrain::GetWorldRect() const
{
	return ( std_Rect(0,0,m_MapDimensions) );
}
// End ogl_Terrain::GetWorldRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::AllocateScreenTiles() //    Author: Logan "Burn" Jones
///////////////////////////////////////                 Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long iColumns - 
//  long iRows    - 
//
void ogl_Terrain::AllocateScreenTiles(long iColumns, long iRows)
{
	// Sanity check
	assert((iColumns>0) && (iRows>0));

	//m_NumberOfScreenTiles.width = iColumns;
	//m_NumberOfScreenTiles.height = iRows;

	m_ScreenTiles = new ScreenTile[m_NumberOfScreenTiles];
	m_ScreenTileTextures = new GLuint[m_NumberOfScreenTiles];
	assert(m_ScreenTiles&&m_ScreenTileTextures);
	glGenTextures(m_NumberOfScreenTiles, m_ScreenTileTextures);

	// Initialize the screen tile map
	long x, y;//,count=0;
	ScreenTile*		pTile = m_ScreenTiles;
	GLuint*			pTex = m_ScreenTileTextures;
	//BYTE			DefaultTexture[m_ScreenTileSize*m_ScreenTileSize];
	BYTE*			DefaultTexture = m_NeedsPaletteConversion ? new BYTE[256 * 256 * 3] : new BYTE[256 * 256];
	const GLuint    GLTexInternalFormat = m_NeedsPaletteConversion ? GL_RGB : GL_COLOR_INDEX8_EXT;
	const GLuint    GLTexFormat = m_NeedsPaletteConversion ? GL_RGB : GL_COLOR_INDEX;
	for (y = 0; y<iRows; y++)
	{
		for (x = 0; x<iColumns; x++, pTile++, pTex++)
		{
			// Check for first column
			if (x == 0) pTile->Left = NULL;
			else pTile->Left = (pTile - 1);

			// Check for last column
			if (x == (iColumns - 1)) pTile->Right = NULL;
			else pTile->Right = (pTile + 1);

			// Check for first row
			if (y == 0) pTile->Top = NULL;
			else pTile->Top = (pTile - iColumns);

			// Check for last row
			if (y == (iRows - 1)) pTile->Bottom = NULL;
			else pTile->Bottom = (pTile + iColumns);

			pTile->pTexture = pTex;
			glBindTexture(GL_TEXTURE_2D, *(pTile->pTexture));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GLTexInternalFormat,
				m_ScreenTileSize,
				m_ScreenTileSize,
				0,
				GLTexFormat,
				GL_UNSIGNED_BYTE,
				DefaultTexture);

		} // end for( Columns )

	} // end for( Rows )

	delete[] DefaultTexture;

	m_ScreenTileDisplayList = glGenLists(1);
	glNewList(m_ScreenTileDisplayList, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex3i(0, 0, m_ScreenTileRenderDepth);
	glTexCoord2f(0, 1);
	glVertex3i(0, m_ScreenTileSize, m_ScreenTileRenderDepth);
	glTexCoord2f(1, 0);
	glVertex3i(m_ScreenTileSize, 0, m_ScreenTileRenderDepth);
	glTexCoord2f(1, 1);
	glVertex3i(m_ScreenTileSize, m_ScreenTileSize, m_ScreenTileRenderDepth);
	glEnd();
	glEndList();

	// The top left tile should be the first one in the array initially
	m_TopLeftTile = m_ScreenTiles;
	m_TopRightTile = m_ScreenTiles + (m_ScreenTileColumns - 1);
	m_BottomLeftTile = m_ScreenTiles + (m_NumberOfScreenTiles - m_ScreenTileColumns);
	m_BottomRightTile = m_ScreenTiles + (m_NumberOfScreenTiles - 1);
}
// End ogl_Terrain::AllocateScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::FillScreenTiles() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::FillScreenTiles()
{
	ScreenTile*		pStartRow = m_TopLeftTile;
	ScreenTile*		pTile;
	WORD*			pFirstIndexOnThisRow;
	WORD*			pFirstIndexForThisTile;
	WORD*			pTileIndex;
	const BOOL		NeedsPaletteConversion = m_NeedsPaletteConversion;
	const GLuint    GLTexFormat = NeedsPaletteConversion ? GL_RGB : GL_COLOR_INDEX;
	BYTE*           ConvertedTileBits = NeedsPaletteConversion ? new BYTE[32 * 32 * 3] : NULL;
	gfx_OpenGL::LPPALETTE pPalette = NULL;

	if (NeedsPaletteConversion)
	{
		pGfxSystem->GetCurrentPalette(&pPalette);
	}

	pFirstIndexOnThisRow = m_TileIndeces + ((m_ScreenTileStartingIndex.y*m_NumTileIndeces.width) + m_ScreenTileStartingIndex.x);
	for (; pStartRow; pStartRow = pStartRow->Bottom)
	{
		pFirstIndexForThisTile = pFirstIndexOnThisRow;
		for (pTile = pStartRow; pTile; pTile = pTile->Right)
		{
			glBindTexture(GL_TEXTURE_2D, *(pTile->pTexture));

			pTileIndex = pFirstIndexForThisTile;
			for (int y = 0; y<m_GfxPerScreenTile; y++)
			{
				for (int x = 0; x<m_GfxPerScreenTile; x++)
				{
					const BYTE *tilePixels = GfxTileBits(pTileIndex[x]);
					if (NeedsPaletteConversion)
					{
						const PALETTEENTRY *palette = static_cast<PALETTEENTRY *>(pPalette);
						for (int a = 0, b = 0, n = 32 * 32; a < n; ++a, b += 3)
						{
							const BYTE index = tilePixels[a];
							const PALETTEENTRY *entry = palette + index;
							ConvertedTileBits[b + 0] = entry->peRed;
							ConvertedTileBits[b + 1] = entry->peGreen;
							ConvertedTileBits[b + 2] = entry->peBlue;
						}
						tilePixels = ConvertedTileBits;
					}

					glTexSubImage2D(
						GL_TEXTURE_2D, 0,
						x * 32, y * 32,
						32, 32,
						GLTexFormat,
						GL_UNSIGNED_BYTE,
						tilePixels);
				}
				pTileIndex += m_NumTileIndeces.width;
			}

			pFirstIndexForThisTile += m_GfxPerScreenTile;

		} // end for( pTile )

		pFirstIndexOnThisRow += m_NumTileIndeces.width * m_GfxPerScreenTile;
		//pFirstIndexOnThisRow = pTileIndex - (m_NumTileIndeces.width - m_GfxPerScreenTile);

	} // end for( pStartRow )

	SAFE_DELETE_ARRAY(ConvertedTileBits);
}
// End ogl_Terrain::FillScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::UploadGfxRectToScreenTiles() // Author: Logan "Burn" Jones
//////////////////////////////////////////////          Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long LeftIndex  - 
//  long TopIndex   - 
//  long FillWidth  - 
//  long FillHeight - 
//
void ogl_Terrain::UploadGfxRectToScreenTiles(long LeftIndex, long TopIndex, long FillWidth, long FillHeight)
{
	const BOOL		NeedsPaletteConversion = m_NeedsPaletteConversion;
	const GLuint    GLTexFormat = NeedsPaletteConversion ? GL_RGB : GL_COLOR_INDEX;
	BYTE*           ConvertedTileBits = NeedsPaletteConversion ? new BYTE[32 * 32 * 3] : NULL;
	gfx_OpenGL::LPPALETTE pPalette = NULL;

	if (NeedsPaletteConversion)
	{
		pGfxSystem->GetCurrentPalette(&pPalette);
	}

	long XOffset = (LeftIndex * 32) - m_ScreenTilePosition.x;
	long YOffset = (TopIndex * 32) - m_ScreenTilePosition.y;
	long XSreenTile = XOffset / m_ScreenTileSize;
	long YSreenTile = YOffset / m_ScreenTileSize;

	ScreenTile* pStartingTile = m_TopLeftTile;
	long n;
	for (n = 0; n<XSreenTile; n++, pStartingTile = pStartingTile->Right);
	for (n = 0; n<YSreenTile; n++, pStartingTile = pStartingTile->Bottom);

	long StartingXOffset = XOffset - (XSreenTile*m_ScreenTileSize);
	YOffset = YOffset - (YSreenTile*m_ScreenTileSize);

	WORD* pStartingIndex = m_TileIndeces + ((TopIndex*m_NumTileIndeces.width) + LeftIndex);
	for (long y = 0; y<FillHeight; y++, YOffset += 32, pStartingIndex += m_NumTileIndeces.width)
	{
		if (YOffset == m_ScreenTileSize)
		{
			YOffset = 0;
			pStartingTile = pStartingTile->Bottom;
			if (pStartingTile == NULL) break;
		}
		glBindTexture(GL_TEXTURE_2D, *(pStartingTile->pTexture));

		XOffset = StartingXOffset;
		WORD* pIndex = pStartingIndex;
		ScreenTile* pTile = pStartingTile;
		for (long x = 0; x<FillWidth; x++, XOffset += 32)
		{
			if (XOffset == m_ScreenTileSize)
			{
				XOffset = 0;
				pTile = pTile->Right;
				if (pTile == NULL) break;
				glBindTexture(GL_TEXTURE_2D, *(pTile->pTexture));
			}

			const BYTE *tilePixels = GfxTileBits(pIndex[x]);
			if (NeedsPaletteConversion)
			{
				const PALETTEENTRY *palette = static_cast<PALETTEENTRY *>(pPalette);
				for (int a = 0, b = 0, n = 32 * 32; a < n; ++a, b += 3)
				{
					const BYTE index = tilePixels[a];
					const PALETTEENTRY *entry = palette + index;
					ConvertedTileBits[b + 0] = entry->peRed;
					ConvertedTileBits[b + 1] = entry->peGreen;
					ConvertedTileBits[b + 2] = entry->peBlue;
				}
				tilePixels = ConvertedTileBits;
			}

			glTexSubImage2D(
				GL_TEXTURE_2D, 0,
				(GLint)XOffset, (GLint)YOffset,
				32, 32,
				GLTexFormat,
				GL_UNSIGNED_BYTE,
				tilePixels);

		} // end for( FillWidth )

	} // end for( FillHeight )

	SAFE_DELETE_ARRAY(ConvertedTileBits);
}
// End ogl_Terrain::UploadGfxRectToScreenTiles()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Terrain.cpp //
//////////////////////////
