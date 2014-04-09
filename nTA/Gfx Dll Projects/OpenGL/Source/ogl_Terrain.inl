// ogl_Terrain.inl //                      Author: Logan "Burn" Jones
////////////////////                                   Date: 2/6/2002
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::RecalculateMetrics() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::RecalculateMetrics()
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
// End ogl_Terrain::RecalculateMetrics()
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
void ogl_Terrain::AllocateScreenTiles( long iColumns, long iRows )
{
	// Sanity check
	assert( (iColumns>0)&&(iRows>0) );

	//m_NumberOfScreenTiles.width = iColumns;
	//m_NumberOfScreenTiles.height = iRows;

	m_ScreenTiles = new ScreenTile[ m_NumberOfScreenTiles ];
	m_ScreenTileTextures = new GLuint[ m_NumberOfScreenTiles ];
	assert( m_ScreenTiles&&m_ScreenTileTextures );
	glGenTextures( m_NumberOfScreenTiles, m_ScreenTileTextures );

	// Initialize the screen tile map
	long x,y,count=0;
	ScreenTile*		pTile = m_ScreenTiles;
	GLuint*			pTex = m_ScreenTileTextures;
	//BYTE			DefaultTexture[m_ScreenTileSize*m_ScreenTileSize];
	BYTE			DefaultTexture[256*256];
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

			pTile->pTexture = pTex;
			glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
			glTexImage2D( GL_TEXTURE_2D,
						  0,
						  GL_COLOR_INDEX8_EXT,
						  m_ScreenTileSize,
						  m_ScreenTileSize,
						  0,
						  GL_COLOR_INDEX,
						  GL_UNSIGNED_BYTE,
						  DefaultTexture );

		} // end for( Columns )

	} // end for( Rows )

	m_ScreenTileDisplayList = glGenLists( 1 );
	glNewList( m_ScreenTileDisplayList, GL_COMPILE );
		glBegin( GL_TRIANGLE_STRIP );
			glTexCoord2f(0,0);
			glVertex3i( 0, 0, m_ScreenTileRenderDepth );
			glTexCoord2f(0,1);
			glVertex3i( 0, m_ScreenTileSize, m_ScreenTileRenderDepth );
			glTexCoord2f(1,0);
			glVertex3i( m_ScreenTileSize, 0, m_ScreenTileRenderDepth );
			glTexCoord2f(1,1);
			glVertex3i( m_ScreenTileSize, m_ScreenTileSize, m_ScreenTileRenderDepth );
		glEnd();
	glEndList();

	// The top left tile should be the first one in the array initially
	m_TopLeftTile = m_ScreenTiles;
	m_TopRightTile = m_ScreenTiles + (m_ScreenTileColumns-1);
	m_BottomLeftTile = m_ScreenTiles + (m_NumberOfScreenTiles-m_ScreenTileColumns);
	m_BottomRightTile = m_ScreenTiles + (m_NumberOfScreenTiles-1);
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

	pFirstIndexOnThisRow = m_TileIndeces + ((m_ScreenTileStartingIndex.y*m_NumTileIndeces.width) + m_ScreenTileStartingIndex.x);
	for( ;pStartRow; pStartRow = pStartRow->Bottom)
	{
		pFirstIndexForThisTile = pFirstIndexOnThisRow;
		for( pTile=pStartRow; pTile; pTile = pTile->Right)
		{
			glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );

			pTileIndex = pFirstIndexForThisTile;
			for( int y=0; y<m_GfxPerScreenTile; y++)
			{
				for( int x=0; x<m_GfxPerScreenTile; x++)
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0,
							 x*32, y*32,
							 32, 32,
							 GL_COLOR_INDEX,
							 GL_UNSIGNED_BYTE,
							 GfxTileBits( pTileIndex[x] ) );
				}
				pTileIndex += m_NumTileIndeces.width;
			}

			pFirstIndexForThisTile += m_GfxPerScreenTile;

		} // end for( pTile )

		pFirstIndexOnThisRow += m_NumTileIndeces.width * m_GfxPerScreenTile;
		//pFirstIndexOnThisRow = pTileIndex - (m_NumTileIndeces.width - m_GfxPerScreenTile);

	} // end for( pStartRow )
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
void ogl_Terrain::UploadGfxRectToScreenTiles( long LeftIndex, long TopIndex, long FillWidth, long FillHeight )
{
	long XOffset = (LeftIndex*32) - m_ScreenTilePosition.x;
	long YOffset = (TopIndex*32) - m_ScreenTilePosition.y;
	long XSreenTile = XOffset / m_ScreenTileSize;
	long YSreenTile = YOffset / m_ScreenTileSize;

	ScreenTile* pStartingTile = m_TopLeftTile;
	for( long n=0; n<XSreenTile; n++,pStartingTile=pStartingTile->Right);
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
		glBindTexture( GL_TEXTURE_2D, *(pStartingTile->pTexture) );

		XOffset = StartingXOffset;
		WORD* pIndex = pStartingIndex;
		ScreenTile* pTile = pStartingTile;
		for( long x=0; x<FillWidth; x++,XOffset+=32)
		{
			if( XOffset==m_ScreenTileSize )
			{
				XOffset = 0;
				pTile = pTile->Right;
				if( pTile==NULL ) break;
				glBindTexture( GL_TEXTURE_2D, *(pTile->pTexture) );
			}
			glTexSubImage2D( GL_TEXTURE_2D, 0,
							 XOffset, YOffset,
							 32, 32,
							 GL_COLOR_INDEX,
							 GL_UNSIGNED_BYTE,
							 GfxTileBits( pIndex[x] ) );

		} // end for( FillWidth )

	} // end for( FillHeight )
}
// End ogl_Terrain::UploadGfxRectToScreenTiles()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ShiftColumnsLeft() //       Author: Logan "Burn" Jones
////////////////////////////////////                    Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::ShiftColumnsLeft()
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
// End ogl_Terrain::ShiftColumnsLeft()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ShiftColumnsRight() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::ShiftColumnsRight()
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
// End ogl_Terrain::ShiftColumnsRight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ShiftRowsUp() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::ShiftRowsUp()
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
// End ogl_Terrain::ShiftRowsUp()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Terrain::ShiftRowsDown() //          Author: Logan "Burn" Jones
/////////////////////////////////                       Date: 2/6/2002
//               
//====================================================================
//
void ogl_Terrain::ShiftRowsDown()
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
// End ogl_Terrain::ShiftRowsDown()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Terrain.inl //
//////////////////////////
