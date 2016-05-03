// igui_MiniMap.cpp //                             \author Logan Jones
/////////////////////                                 \date 5/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_MiniMap.h"
#include "game.h"
#include "ta_TNT.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_MiniMap::igui_MiniMap():wnd_Window(),
	m_bScrolling( FALSE )
{}
igui_MiniMap::~igui_MiniMap()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::OnCreate() //                     \author Logan Jones
/////////////////////////////                          \date 5/14/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL igui_MiniMap::OnCreate()
{
	BYTE*					pTntBuffer = theGame.Terrain.GetTntBuffer();
	LPTA_TNT_HEADER			pHeader;
	LPTA_TNT_EXT_HEADER		pMapHeader;
	std_Size				RawSize;
	BYTE*					pRawMiniMap;
	long					x,y;
	std_Size				MiniMapSize;
	BYTE*					pMiniMap;

	pHeader = (LPTA_TNT_HEADER)pTntBuffer;
	pMapHeader = (LPTA_TNT_EXT_HEADER)(pTntBuffer + sizeof(TA_TNT_HEADER));
	m_WorldSize.Set( (pHeader->Width * 16) - 32, (pHeader->Height * 16) - 128 );

	// Get the width, height, and data of the minimap
	RawSize.width = ((DWORD*)(pTntBuffer + pMapHeader->OffsetToMiniMap))[0];
	RawSize.height= ((DWORD*)(pTntBuffer + pMapHeader->OffsetToMiniMap))[1];
	pRawMiniMap = pTntBuffer + (pMapHeader->OffsetToMiniMap + 8);

	// Find the actual size of the minimap
	for( m_MiniMapBufferSize.width=RawSize.width,x=0; x<RawSize.width; ++x)
		if( pRawMiniMap[x]==PALETTE_CYAN && pRawMiniMap[x+RawSize.width]==PALETTE_CYAN && pRawMiniMap[x+RawSize.width*2]==PALETTE_CYAN )
		{ m_MiniMapBufferSize.width = x; break; }
	for( m_MiniMapBufferSize.height=RawSize.height,x=y=0; y<RawSize.height; x+=RawSize.width,++y)
		if( pRawMiniMap[x]==PALETTE_CYAN && pRawMiniMap[x+1]==PALETTE_CYAN && pRawMiniMap[x+2]==PALETTE_CYAN )
		{ m_MiniMapBufferSize.height = y; break; }

	// Allocate the buffer for the minimap and fill it
	m_MiniMapBuffer = new BYTE[ *m_MiniMapBufferSize ];
	for( y=0; y<m_MiniMapBufferSize.height; ++y )
		memcpy( m_MiniMapBuffer + (y * m_MiniMapBufferSize.width), pRawMiniMap + (y * RawSize.width), m_MiniMapBufferSize.width );

	gfx->CreateDynamicImage( m_MiniMapBufferSize, &m_MiniMapImage );
	MiniMapSize = m_MiniMapImage->GetSize();
	m_MiniMapImage->GetBits( &pMiniMap );
	for( y=0; y<m_MiniMapBufferSize.height; ++y )
	{
		memcpy( pMiniMap, m_MiniMapBuffer + (y * m_MiniMapBufferSize.width), m_MiniMapBufferSize.width );
		pMiniMap += MiniMapSize.width;
	}
	m_MiniMapImage->Update( NULL );

	m_MiniMapScreenRect.Set(
		(RawSize.width - m_MiniMapBufferSize.width) / 4,
		(RawSize.height - m_MiniMapBufferSize.height) / 4,
		m_MiniMapBufferSize / 2 );

	return TRUE;
}
// End igui_MiniMap::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::OnDestroy() //                    \author Logan Jones
//////////////////////////////                         \date 5/14/2002
//               
//====================================================================
//
void igui_MiniMap::OnDestroy()
{
	SAFE_DESTROY( m_MiniMapImage );
	SAFE_DELETE_ARRAY( m_MiniMapBuffer );
}
// End igui_MiniMap::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::Render() //                       \author Logan Jones
///////////////////////////                            \date 5/14/2002
//               
//====================================================================
//
void igui_MiniMap::Render()
{
	gfx->PushMatrix();
	gfx->Translate( m_ScreenPosition );

	m_MiniMapImage->Render( m_MiniMapScreenRect.Position(), m_MiniMapScreenRect.Size() );
	gfx->DrawWireRect( m_ViewRect, (DWORD)0xFFFF00FF );

	gfx->PopMatrix();
}
// End igui_MiniMap::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::Update() //                       \author Logan Jones
///////////////////////////                            \date 5/14/2002
//               
//====================================================================
//
void igui_MiniMap::Update()
{
	std_Size				MiniMapSize;
	BYTE*					pMiniMap;

	m_ViewRect = theGame.Terrain.GetRect();
	m_ViewRect.left = ((m_ViewRect.left * m_MiniMapScreenRect.Width()) / m_WorldSize.width) + m_MiniMapScreenRect.left;
	m_ViewRect.right = ((m_ViewRect.right * m_MiniMapScreenRect.Width()) / m_WorldSize.width) + m_MiniMapScreenRect.left;
	m_ViewRect.top = ((m_ViewRect.top * m_MiniMapScreenRect.Height()) / m_WorldSize.height) + m_MiniMapScreenRect.top;
	m_ViewRect.bottom = ((m_ViewRect.bottom * m_MiniMapScreenRect.Height()) / m_WorldSize.height) + m_MiniMapScreenRect.top;

	MiniMapSize = m_MiniMapImage->GetSize();
	m_MiniMapImage->GetBits( &pMiniMap );
	for( long y=0; y<m_MiniMapBufferSize.height; ++y )
	{
		memcpy( pMiniMap, m_MiniMapBuffer + (y * m_MiniMapBufferSize.width), m_MiniMapBufferSize.width );
		pMiniMap += MiniMapSize.width;
	}
	m_MiniMapImage->Update( NULL );
}
// End igui_MiniMap::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::OnMouseButton() //                \author Logan Jones
//////////////////////////////////                     \date 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void igui_MiniMap::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	switch( dwButton )
	{
		case 2: if( bDown )
			{
				if( m_MiniMapScreenRect.PointInRect(ptCursor) )
					theGame.Terrain.SetCenter( std_Point(
						((ptCursor.x - m_MiniMapScreenRect.left) * m_WorldSize.width) / m_MiniMapScreenRect.Width(),
						((ptCursor.y - m_MiniMapScreenRect.top) * m_WorldSize.height) / m_MiniMapScreenRect.Height()) );
				SetCapture();
				m_bScrolling = TRUE;
			}
			else // Button 2 was released
			{
				ReleaseCapture();
				m_bScrolling = FALSE;
			}
			break;
	} // end switch( dwButton )
}
// End igui_MiniMap::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::OnCursorMove() //                 \author Logan Jones
/////////////////////////////////                      \date 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwFlags         - 
//
void igui_MiniMap::OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags )
{
	if( m_bScrolling && m_MiniMapScreenRect.PointInRect(ptCursor) )
		theGame.Terrain.SetCenter( std_Point(
			((ptCursor.x - m_MiniMapScreenRect.left) * m_WorldSize.width) / m_MiniMapScreenRect.Width(),
			((ptCursor.y - m_MiniMapScreenRect.top) * m_WorldSize.height) / m_MiniMapScreenRect.Height()) );
}
// End igui_MiniMap::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_MiniMap::OnKeyboardButton() //             \author Logan Jones
/////////////////////////////////////                  \date 7/30/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void igui_MiniMap::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	theInterface->ControlBar().OnKeyboardButton( dwButton, bDown, dwFlags );
}
// End igui_MiniMap::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_MiniMap.cpp //
///////////////////////////
