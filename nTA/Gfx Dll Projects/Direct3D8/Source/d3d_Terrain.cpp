// d3d_Terrain.cpp //                      Author: Logan "Burn" Jones
////////////////////                                   Date: 2/6/2002
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Terrain.h"
#include "../../../Source/ta_TNT.h"

// Implement inline methods here for a debug build
#ifdef _DEBUG
 #include "d3d_Terrain.inl"
#endif // defined( _DEBUG )


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::LoadTerrain() //          Author: Logan "Burn" Jones
/////////////////////////////////                       Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntBuffer        - 
//  gfx_Terrain** ppTerrain - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::LoadTerrain( BYTE* pTntBuffer, gfx_Terrain** ppTerrain )
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

	// Set the host interfce and the return pointer to the terrain member and
	// return the result of the terrain creation
	m_theTerrain.m_pHostInterface = this;
	(*ppTerrain) = &m_theTerrain;
	return m_theTerrain.Create( pTntBuffer );
}
// End gfx_Direct3D8::LoadTerrain()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_Terrain::d3d_Terrain():
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
d3d_Terrain::~d3d_Terrain()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::Create() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  BYTE* pTntBuffer - 
//
// Return: BOOL - 
//
BOOL d3d_Terrain::Create( BYTE* pTntBuffer )
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

	AllocateScreenTiles( m_ScreenTileColumns, m_ScreenTileRows );

	// Return success
	return TRUE;
}
// End d3d_Terrain::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::Destroy() //                Author: Logan "Burn" Jones
///////////////////////////                             Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::Destroy()
{
	if( m_ScreenTiles )
	{
	//	glDeleteLists( m_ScreenTileDisplayList, 1 );
	//	glDeleteTextures( m_NumberOfScreenTiles, m_ScreenTileTextures );
		SAFE_DELETE_ARRAY( m_ScreenTileTextures );
		SAFE_DELETE_ARRAY( m_ScreenTiles );
	}
}
// End d3d_Terrain::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::Render() const
{
	ScreenTile*		pTile,*pStartRow = m_TopLeftTile;
	long XOffset = m_ScreenTilePosition.x;
	long YOffset = m_ScreenTilePosition.y;

	m_pHostInterface->m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pHostInterface->m_pD3DDevice->SetStreamSource( 0, m_ScreenTileVertexBuffer, sizeof(ScreenTile_Vert_t) );
	//m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	//m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	m_pHostInterface->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );

	m_pHostInterface->dxPushMatrix();

	m_pHostInterface->dxTranslatef( (float)(m_ScreenTilePosition.x - m_MapView.left),
									(float)(m_ScreenTilePosition.y - m_MapView.top), 0 );

	for( ;pStartRow; pStartRow = pStartRow->Bottom)
	{
		m_pHostInterface->dxPushMatrix();
		XOffset = m_ScreenTilePosition.x;
		for( pTile=pStartRow; pTile; pTile = pTile->Right,XOffset+=m_ScreenTileSize)
		{
			if( !(((XOffset+m_ScreenTileSize)<m_MapView.left) ||
				  (XOffset>m_MapView.right) ||
				  ((YOffset+m_ScreenTileSize)<m_MapView.top)) )
			{
				m_pHostInterface->m_pD3DDevice->SetTexture( 0, pTile->pTexture );
				m_pHostInterface->dxSetWorldMatrix();
				m_pHostInterface->m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
				m_pHostInterface->m_TriangleCount += 2;
			}

			m_pHostInterface->dxTranslatef( (float)m_ScreenTileSize, 0, 0 );

		} // end for( pTile )
		m_pHostInterface->dxPopMatrix();
		m_pHostInterface->dxTranslatef( 0, (float)m_ScreenTileSize, 0 );
		YOffset += m_ScreenTileSize;

		if( YOffset>m_MapView.bottom )
			break;

	} // end for( pStartRow )
	m_pHostInterface->dxPopMatrix();

}
// End d3d_Terrain::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::AllocateScreenTiles() //    Author: Logan "Burn" Jones
///////////////////////////////////////                 Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long iColumns - 
//  long iRows    - 
//
void d3d_Terrain::AllocateScreenTiles( long iColumns, long iRows )
{
	// Sanity check
	assert( (iColumns>0)&&(iRows>0) );

	m_ScreenTiles = new ScreenTile[ m_NumberOfScreenTiles ];
	m_ScreenTileTextures = new LPDIRECT3DTEXTURE8[ m_NumberOfScreenTiles ];
	assert( m_ScreenTiles&&m_ScreenTileTextures );

	// Initialize the screen tile map
	long x,y,count=0;
	ScreenTile*			pTile = m_ScreenTiles;
	LPDIRECT3DTEXTURE8*	pTex = m_ScreenTileTextures;
	LRESULT				lResult;
	for( y=0; y<iRows; y++)
	{
		for( x=0; x<iColumns; x++, pTile++, pTex++)
		{
			// Check for first column
			if( x==0 ) pTile->Left = NULL;
			else pTile->Left = (pTile-1);

			// Check for last column
			if( x==(iColumns-1) ) pTile->Right = NULL;
			else pTile->Right = (pTile+1);

			// Check for first row
			if( y==0 ) pTile->Top = NULL;
			else pTile->Top = (pTile-iColumns);

			// Check for last row
			if( y==(iRows-1) ) pTile->Bottom = NULL;
			else pTile->Bottom = (pTile+iColumns);

			pTile->pTexture = *pTex;
			if( FAILED(lResult=m_pHostInterface->m_pD3DDevice->CreateTexture(
				m_ScreenTileSize,
				m_ScreenTileSize,
				1,
				0,
				D3DFMT_P8,
				D3DPOOL_MANAGED,
				&pTile->pTexture )) )
			{
				assert( !"Failed to create texture." );
			}

		} // end for( Columns )

	} // end for( Rows )

	// Create the screen tile vertex buffer
	if( FAILED(lResult=m_pHostInterface->m_pD3DDevice->CreateVertexBuffer(
		4 * sizeof(ScreenTile_Vert_t),
        0,
		D3DFVF_SCREENTILE_VERT,
        D3DPOOL_MANAGED,
		&m_ScreenTileVertexBuffer ) ) )
    {
		assertEx( 0, "Failed to create screen tile vertex buffer." );
    }

	ScreenTile_Vert_t* pVerts;
	m_ScreenTileVertexBuffer->Lock( 0, 0, (BYTE**)&pVerts, 0 );
	pVerts[0].tu = 0;
	pVerts[0].tv = 0;
	pVerts[0].position = D3DXVECTOR3( 0, 0, m_ScreenTileRenderDepth );
	pVerts[1].tu = 0;
	pVerts[1].tv = 1;
	pVerts[1].position = D3DXVECTOR3( 0, m_ScreenTileSize, m_ScreenTileRenderDepth );
	pVerts[2].tu = 1;
	pVerts[2].tv = 0;
	pVerts[2].position = D3DXVECTOR3( m_ScreenTileSize, 0, m_ScreenTileRenderDepth );
	pVerts[3].tu = 1;
	pVerts[3].tv = 1;
	pVerts[3].position = D3DXVECTOR3( m_ScreenTileSize, m_ScreenTileSize, m_ScreenTileRenderDepth );
	m_ScreenTileVertexBuffer->Unlock();

	// The top left tile should be the first one in the array initially
	m_TopLeftTile = m_ScreenTiles;
	m_TopRightTile = m_ScreenTiles + (m_ScreenTileColumns-1);
	m_BottomLeftTile = m_ScreenTiles + (m_NumberOfScreenTiles-m_ScreenTileColumns);
	m_BottomRightTile = m_ScreenTiles + (m_NumberOfScreenTiles-1);
}
// End d3d_Terrain::AllocateScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::InitializeViewport() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptCenterPosition - 
//  std_Size szViewport        - 
//
void d3d_Terrain::InitializeViewport( std_Point ptCenterPosition, std_Size szViewport )
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
	POINT NewScreenTilePoint;
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
// End d3d_Terrain::InitializeViewport()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::CenterViewAt() //           Author: Logan "Burn" Jones
////////////////////////////////                        Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptCenterPosition - 
//
void d3d_Terrain::CenterViewAt( std_Point ptCenterPosition )
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
// End d3d_Terrain::CenterViewAt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::Scroll() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Point ptScroll - 
//
void d3d_Terrain::Scroll( std_Point ptScroll )
{
	ScrollHorizontaly( ptScroll.x );
	ScrollVerticaly( ptScroll.y );
	return;

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
}
// End d3d_Terrain::Scroll()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ScrollHorizontaly() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long lX           - 
//  BOOL EarlyOutIsOK - 
//
void d3d_Terrain::ScrollHorizontaly( long lX, BOOL EarlyOutIsOK )
{
	// Make a copy the viewport and shift it with the proposed values
	RECT NewView = m_MapView;
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
// End d3d_Terrain::ScrollHorizontaly()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ScrollVerticaly() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long lY           - 
//  BOOL EarlyOutIsOK - 
//
void d3d_Terrain::ScrollVerticaly( long lY, BOOL EarlyOutIsOK )
{
	// Make a copy the viewport and shift it with the proposed values
	RECT NewView = m_MapView;
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
// End d3d_Terrain::ScrollVerticaly()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::SetViewportSize() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  std_Size szViewport - 
//
// Return: BOOL - 
//
BOOL d3d_Terrain::SetViewportSize( std_Size szViewport )
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
// End d3d_Terrain::SetViewportSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::GetViewportSize() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Return: std_Size - 
//
std_Size d3d_Terrain::GetViewportSize() const
{
	return m_MapView.Size();
}
// End d3d_Terrain::GetViewportSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::GetViewportRect() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect d3d_Terrain::GetViewportRect() const
{
	return( m_MapView );
}
// End d3d_Terrain::GetViewportRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::GetWorldRect() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 2/12/2002
//               
//====================================================================
// Return: std_Rect - 
//
std_Rect d3d_Terrain::GetWorldRect() const
{
	return ( std_Rect(0,0,m_MapDimensions) );
}
// End d3d_Terrain::GetWorldRect()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Terrain.cpp //
//////////////////////////
