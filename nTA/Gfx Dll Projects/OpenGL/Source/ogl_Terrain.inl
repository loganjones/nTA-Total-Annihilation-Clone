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
		m_MaxMapWindowWidth = (int)(m_MapDimensions.width / 4) * 4;

		m_ScreenTileColumns = m_MaxMapWindowWidth / m_ScreenTileSize;
		if( (m_ScreenTileColumns*m_ScreenTileSize)<m_MaxMapWindowWidth )
			m_ScreenTileColumns++;
	}

	if( m_ScreenTileGridHeight>m_MapDimensions.height )
	{
		m_MaxMapWindowHeight = (int)(m_MapDimensions.height / 3) * 3;

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
