// d3d_Terrain.inl //                      Author: Logan "Burn" Jones
////////////////////                                   Date: 2/6/2002
//
/////////////////////////////////////////////////////////////////////


#define GfxTileBits( int_TILE_INDEX )		( m_GfxTiles+((int_TILE_INDEX)*(32*32)) )


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::RecalculateMetrics() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::RecalculateMetrics()
{	
	if( m_ScreenTileGridWidth>m_MapDimensions.width )
	{
		m_MaxMapWindowWidth = (m_MapDimensions.width / 4) * 4;

		m_ScreenTileColumns = m_MaxMapWindowWidth / m_ScreenTileSize;
		if( (m_ScreenTileColumns*m_ScreenTileSize)<m_MaxMapWindowWidth )
			m_ScreenTileColumns++;
	}

	if( m_ScreenTileGridHeight>m_MapDimensions.height )
	{
		m_MaxMapWindowHeight = (m_MapDimensions.height / 3) * 3;

		m_ScreenTileRows = m_MaxMapWindowHeight / m_ScreenTileSize;
		if( (m_ScreenTileRows*m_ScreenTileSize)<m_MaxMapWindowHeight )
			m_ScreenTileRows++;
	}

	m_NumberOfScreenTiles = m_ScreenTileColumns * m_ScreenTileRows;
	m_ScreenTileGridWidth = m_ScreenTileColumns * m_ScreenTileSize;
	m_ScreenTileGridHeight = m_ScreenTileRows * m_ScreenTileSize;
	m_GfxPerGridWidth = m_GfxPerScreenTile * m_ScreenTileColumns;
	m_GfxPerGridHeight = m_GfxPerScreenTile * m_ScreenTileRows;
}
// End d3d_Terrain::RecalculateMetrics()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::FillScreenTiles() //        Author: Logan "Burn" Jones
///////////////////////////////////                     Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::FillScreenTiles()
{
	ScreenTile*		pStartRow = m_TopLeftTile;
	ScreenTile*		pTile;
	WORD*			pFirstIndexOnThisRow;
	WORD*			pFirstIndexForThisTile;
	WORD*			pTileIndex;
	D3DLOCKED_RECT	LockedRect;
	BYTE*			pTexStart;
	BYTE*			pTexTile;
	BYTE*			pTex;
	BYTE*			pTileImage;

	pFirstIndexOnThisRow = m_TileIndeces + ((m_ScreenTileStartingIndex.y*m_NumTileIndeces.width) + m_ScreenTileStartingIndex.x);
	for( ;pStartRow; pStartRow = pStartRow->Bottom)
	{
		pFirstIndexForThisTile = pFirstIndexOnThisRow;
		for( pTile=pStartRow; pTile; pTile = pTile->Right)
		{
	//		glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );
			pTile->pTexture->LockRect( 0, &LockedRect, NULL, 0 );

			pTexStart = (BYTE*)LockedRect.pBits;
			pTileIndex = pFirstIndexForThisTile;
			for( int y=0; y<m_GfxPerScreenTile; y++)
			{
				pTexTile = pTexStart;
				for( int x=0; x<m_GfxPerScreenTile; x++)
				{
					pTileImage = GfxTileBits( pTileIndex[x] );
					pTex = pTexTile;
					for( long rows=32; rows>0; --rows)
					{
						memcpy( pTex, pTileImage, 32 );
						pTex += LockedRect.Pitch;
						pTileImage += 32;
					}
	//				glTexSubImage2D( GL_TEXTURE_2D, 0,
	//						 x*32, y*32,
	//						 32, 32,
	//						 GL_COLOR_INDEX,
	//						 GL_UNSIGNED_BYTE,
	//						 GfxTileBits( pTileIndex[x] ) );
					pTexTile += 32;
				}
				pTexStart += LockedRect.Pitch * 32;
				pTileIndex += m_NumTileIndeces.width;
			}

			pTile->pTexture->UnlockRect( 0 );
			pFirstIndexForThisTile += m_GfxPerScreenTile;

		} // end for( pTile )

		pFirstIndexOnThisRow += m_NumTileIndeces.width * m_GfxPerScreenTile;
		//pFirstIndexOnThisRow = pTileIndex - (m_NumTileIndeces.width - m_GfxPerScreenTile);

	} // end for( pStartRow )
}
// End d3d_Terrain::FillScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::UploadGfxRectToScreenTiles() // Author: Logan "Burn" Jones
//////////////////////////////////////////////          Date: 2/6/2002
//               
//====================================================================
// Parameters:
//  long LeftIndex  - 
//  long TopIndex   - 
//  long FillWidth  - 
//  long FillHeight - 
//
void d3d_Terrain::UploadGfxRectToScreenTiles( long LeftIndex, long TopIndex, long FillWidth, long FillHeight )
{
	long XOffset = (LeftIndex*32) - m_ScreenTilePosition.x;
	long YOffset = (TopIndex*32) - m_ScreenTilePosition.y;
	long XSreenTile = XOffset / m_ScreenTileSize;
	long YSreenTile = YOffset / m_ScreenTileSize;
	D3DLOCKED_RECT	LockedRect;
	BYTE*			pTex;
	BYTE*			pTileImage;

	ScreenTile* pStartingTile = m_TopLeftTile;
	long n;
	for( n=0; n<XSreenTile; n++,pStartingTile=pStartingTile->Right);
	for( n=0; n<YSreenTile; n++,pStartingTile=pStartingTile->Bottom);
	
	long StartingXOffset = XOffset - (XSreenTile*m_ScreenTileSize);
	YOffset = YOffset - (YSreenTile*m_ScreenTileSize);

	WORD* pStartingIndex = m_TileIndeces + ((TopIndex*m_NumTileIndeces.width) + LeftIndex);
	for( long y=0; y<FillHeight; y++,YOffset+=32,pStartingIndex+=m_NumTileIndeces.width)
	{
		if( YOffset==m_ScreenTileSize )
		{
			YOffset = 0;
			pStartingTile = pStartingTile->Bottom;
			if( pStartingTile==NULL ) break;
		}

		XOffset = StartingXOffset;
		WORD* pIndex = pStartingIndex;
		ScreenTile* pTile = pStartingTile;
		pTile->pTexture->LockRect( 0, &LockedRect, NULL, 0 );
		for( long x=0; x<FillWidth; x++,XOffset+=32)
		{
			if( XOffset==m_ScreenTileSize )
			{
				XOffset = 0;
				pTile->pTexture->UnlockRect( 0 );
				pTile = pTile->Right;
				if( pTile==NULL ) break;
				pTile->pTexture->LockRect( 0, &LockedRect, NULL, 0 );
			}
			pTileImage = GfxTileBits( pIndex[x] );
			pTex = ((BYTE*)LockedRect.pBits) + ( XOffset + (YOffset * LockedRect.Pitch) );
			for( long rows=32; rows>0; --rows)
			{
				memcpy( pTex, pTileImage, 32 );
				pTex += LockedRect.Pitch;
				pTileImage += 32;
			}

		} // end for( FillWidth )
		pTile->pTexture->UnlockRect( 0 );

	} // end for( FillHeight )
}
// End d3d_Terrain::UploadGfxRectToScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ShiftColumnsLeft() //       Author: Logan "Burn" Jones
////////////////////////////////////                    Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::ShiftColumnsLeft()
{
	ScreenTile*		pShiftTile = m_TopLeftTile;
	ScreenTile*		pEndTile = m_TopRightTile;

	m_TopRightTile = m_TopLeftTile;
	m_TopLeftTile = m_TopLeftTile->Right;
	m_BottomRightTile = m_BottomLeftTile;
	m_BottomLeftTile = m_BottomLeftTile->Right;

	while( pShiftTile )
	{
		(pShiftTile->Right)->Left = NULL;
		pShiftTile->Left = pEndTile;
		pEndTile->Right = pShiftTile;
		pShiftTile->Right = NULL;

		pShiftTile = pShiftTile->Bottom;
		pEndTile = pEndTile->Bottom;
	}
}
// End d3d_Terrain::ShiftColumnsLeft()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ShiftColumnsRight() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::ShiftColumnsRight()
{
	ScreenTile*		pShiftTile = m_TopRightTile;
	ScreenTile*		pEndTile = m_TopLeftTile;

	m_TopLeftTile = m_TopRightTile;
	m_TopRightTile = m_TopRightTile->Left;
	m_BottomLeftTile = m_BottomRightTile;
	m_BottomRightTile = m_BottomRightTile->Left;

	while( pShiftTile )
	{
		(pShiftTile->Left)->Right = NULL;
		pShiftTile->Right = pEndTile;
		pEndTile->Left = pShiftTile;
		pShiftTile->Left = NULL;

		pShiftTile = pShiftTile->Bottom;
		pEndTile = pEndTile->Bottom;
	}
}
// End d3d_Terrain::ShiftColumnsRight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ShiftRowsUp() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::ShiftRowsUp()
{
	ScreenTile*		pShiftTile = m_TopLeftTile;
	ScreenTile*		pEndTile = m_BottomLeftTile;

	m_BottomLeftTile = m_TopLeftTile;
	m_TopLeftTile = m_TopLeftTile->Bottom;
	m_BottomRightTile = m_TopRightTile;
	m_TopRightTile = m_TopRightTile->Bottom;

	while( pShiftTile )
	{
		(pShiftTile->Bottom)->Top = NULL;
		pShiftTile->Top = pEndTile;
		pEndTile->Bottom = pShiftTile;
		pShiftTile->Bottom = NULL;

		pShiftTile = pShiftTile->Right;
		pEndTile = pEndTile->Right;
	}
}
// End d3d_Terrain::ShiftRowsUp()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Terrain::ShiftRowsDown() //          Author: Logan "Burn" Jones
/////////////////////////////////                       Date: 2/6/2002
//               
//====================================================================
//
void d3d_Terrain::ShiftRowsDown()
{
	ScreenTile*		pShiftTile = m_BottomLeftTile;
	ScreenTile*		pEndTile = m_TopLeftTile;

	m_TopLeftTile = m_BottomLeftTile;
	m_BottomLeftTile = m_BottomLeftTile->Top;
	m_TopRightTile = m_BottomRightTile;
	m_BottomRightTile = m_BottomRightTile->Top;

	while( pShiftTile )
	{
		(pShiftTile->Top)->Bottom = NULL;
		pShiftTile->Bottom = pEndTile;
		pEndTile->Top = pShiftTile;
		pShiftTile->Top = NULL;

		pShiftTile = pShiftTile->Right;
		pEndTile = pEndTile->Right;
	}
}
// End d3d_Terrain::ShiftRowsDown()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Terrain.inl //
//////////////////////////
