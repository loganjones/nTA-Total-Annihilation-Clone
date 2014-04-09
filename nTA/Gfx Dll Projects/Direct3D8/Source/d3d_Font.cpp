// d3d_Font.cpp //                         Author: Logan "Burn" Jones
/////////////////                                    Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Font.h"


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateFont() //           Author: Logan "Burn" Jones
////////////////////////////////                      Date: 10/10/2001
//               
//====================================================================
// Parameters:
//  FontEntry* pFontEntries      - 
//  std_Size_t szFontDist        - 
//  GFX_PIXEL_FORMAT pxSrcFormat - 
//  gfx_Image_t* pImage          - 
//  gfx_Font** ppFont            - 
//  BOOL bSetAsCurrent           - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateFont( FontEntry* pFontEntries, std_Size_t szFontDist, GFX_PIXEL_FORMAT pxSrcFormat, gfx_Image_t* pImage, gfx_Font** ppFont, BOOL bSetAsCurrent )
{
#define SET_VERTEX( X, Y, Z, U, V ) \
	pVerts[iIndex].position = D3DXVECTOR3( (X) - 0.5f, (Y) - 0.5f, Z ); \
	pVerts[iIndex].tu = U; \
	pVerts[iIndex].tv = V; \
	iIndex++
#define TEX_COORD_U( T )		( (float)(T) / pImage->Size.width )
#define TEX_COORD_V( T )		( (float)(T) / pImage->Size.height )

	LRESULT					lResult;
	gfx_SubImage_t			SubImage;
	d3d_Font				NewFont;
	FontEntry*				pEntry;
	Font_Vert_t*			pVerts;
	std_Size_t				MaxFontSize;
	std_Point_t				TexOffset;
	long					TotalHeight;
	std_Vector2				Origin;
	std_SizeF				Size;

	// Create the font texture
	SubImage.pImage = pImage;
	SubImage.pBytes = pImage->pBytes;
	SubImage.Offset = std_Point_t(0,0);
	SubImage.Size = pImage->Size;
	if( FAILED(lResult=CreateTexture(
		pxSrcFormat,
		&SubImage,
		pImage->Size,
		&NewFont.m_Texture )) )
	{
		ErrorComment( "ERROR: Failed to create font texture, %s %d.", GetErrorString(lResult), lResult );
		assertEx( 0, "Failed to create font texture." );
		return FALSE;
	}

	// Create the font vertex buffer
	if( FAILED(lResult=m_pD3DDevice->CreateVertexBuffer(
		*szFontDist * 4 * sizeof(Font_Vert_t),
        0,
		D3DFVF_FONT_VERT,
        D3DPOOL_MANAGED,
		&NewFont.m_VertexBuffer ) ) )
    {
		ErrorComment( "ERROR: Failed to create font vertex buffer, %s %d.", GetErrorString(lResult), lResult );
		assertEx( 0, "Failed to create font vertex buffer." );
        return FALSE;
    }

    if( FAILED(lResult=NewFont.m_VertexBuffer->Lock( 0, 0, (BYTE**)&pVerts, 0 )) )
	{
		ErrorComment( "ERROR: Failed to lock font vertex buffer, %s %d.", GetErrorString(lResult), lResult );
		assertEx( 0, "Failed to lock font vertex buffer." );
        return FALSE;
	}

	// Calculate the max font size
	MaxFontSize.width = pImage->Size.width / szFontDist.width;
	MaxFontSize.height= pImage->Size.height/ szFontDist.height;

	pEntry = pFontEntries;
	TexOffset = std_Point_t(0,0);
	NewFont.m_Height = TotalHeight = 0;
	for( long y=0,iIndex=0; y<szFontDist.height; ++y)
	{
		TexOffset.x = 0;
		for( long x=0; x<szFontDist.width; ++x,++pEntry)
		{
			Origin.Set( pEntry->Offset.x, pEntry->Size.height - pEntry->Offset.y );
			SET_VERTEX( Origin.x, Origin.y - pEntry->Size.height, 0, TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y) );
			SET_VERTEX( Origin.x, Origin.y, 0, TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );
			SET_VERTEX( Origin.x + pEntry->Size.width, Origin.y - pEntry->Size.height, 0, TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y) );
			SET_VERTEX( Origin.x + pEntry->Size.width, Origin.y, 0, TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );

			const long height = pEntry->Size.height/* - (pEntry->Size.height - pEntry->Offset.y)*/;
			if( height>NewFont.m_Height )
				NewFont.m_Height = height;

			TotalHeight += pEntry->Size.height;

			TexOffset.x += MaxFontSize.width;
		}

		TexOffset.y += MaxFontSize.height;
	}

	// Unlock the vertex buffer
	NewFont.m_VertexBuffer->Unlock();

	// Set the rest of the font's data and add it to the list
	memcpy( NewFont.m_Entries, pFontEntries, sizeof(NewFont.m_Entries) );
	NewFont.m_pHost = &m_Fonts;
	NewFont.m_Height = TotalHeight / *szFontDist;
	m_Fonts.push_front( NewFont );
	*ppFont = &m_Fonts.front();
	m_Fonts.front().m_HostIt = m_Fonts.begin();

	// Set the new font as the current one if desired
	if( bSetAsCurrent )
		SetCurrentFont( *ppFont );

	// Return success
	return TRUE;
}
// End gfx_Direct3D8::CreateFont()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_Font::d3d_Font():
	m_Texture( NULL ),
	m_VertexBuffer( NULL ),
	m_Height( 0 ),
	m_pHost( NULL )
{}
d3d_Font::~d3d_Font()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Font::Destroy() //                   Author: Logan "Burn" Jones
////////////////////////                              Date: 10/13/2001
//               
//====================================================================
//
void d3d_Font::Destroy()
{
	if( m_pHost )
	{
		SAFE_RELEASE( m_VertexBuffer );
		SAFE_RELEASE( m_Texture );
		list< d3d_Font >* pList = m_pHost;
		m_pHost = NULL;
		pList->erase( m_HostIt );
	}
}
// End d3d_Font::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Font::GetHeight() //                 Author: Logan "Burn" Jones
//////////////////////////                            Date: 10/24/2001
//               
//====================================================================
// Return: long - 
//
long d3d_Font::GetHeight() const
{
	return (m_Height);
}
// End d3d_Font::GetHeight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::SetCurrentFont() //       Author: Logan "Burn" Jones
////////////////////////////////////                  Date: 10/22/2001
//               
//====================================================================
// Parameters:
//  gfx_Font* pFont - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::SetCurrentFont( gfx_Font* pFont )
{
	return (m_ActiveFont = pFont)!=NULL;
}
// End gfx_Direct3D8::SetCurrentFont()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderString() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 4/19/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender      - 
//  const std_Vector3 vWhere - 
//  const DWORD dwColor      - 
//  gfx_Font* pFont          - 
//
void gfx_Direct3D8::RenderString( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
	const d3d_Font* Font = (pFont==NULL) ? static_cast<d3d_Font*>(m_ActiveFont) : static_cast<d3d_Font*>(pFont);

	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, Font->m_VertexBuffer, sizeof(Font_Vert_t) );

	m_pD3DDevice->SetTexture( 0, Font->m_Texture );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	dxPushMatrix();
	dxTranslatef( vWhere );
	long Len = strlen( strToRender );
	for( long n=0; n<Len; ++n)
	{
		dxSetWorldMatrix();
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, strToRender[n] * 4, 2 );
		dxTranslatef( Font->m_Entries[strToRender[n]].Size.width, 0, 0 );
	}
	dxPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_Direct3D8::RenderString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderString() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 5/10/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender      - 
//  DWORD dwLength           - 
//  const std_Vector3 vWhere - 
//  const DWORD dwColor      - 
//  gfx_Font* pFont          - 
//
// Return: DWORD - 
//
DWORD gfx_Direct3D8::RenderString( LPCTSTR strToRender, DWORD dwLength, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
	const d3d_Font* Font = (pFont==NULL) ? static_cast<d3d_Font*>(m_ActiveFont) : static_cast<d3d_Font*>(pFont);
	DWORD Width = 0;

	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, Font->m_VertexBuffer, sizeof(Font_Vert_t) );

	m_pD3DDevice->SetTexture( 0, Font->m_Texture );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	dxPushMatrix();
	dxTranslatef( vWhere );
	for( long n=0; n<dwLength; ++n)
	{
		dxSetWorldMatrix();
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, strToRender[n] * 4, 2 );
		dxTranslatef( Font->m_Entries[strToRender[n]].Size.width, 0, 0 );
		Width += Font->m_Entries[strToRender[n]].Size.width;
	}
	dxPopMatrix();
	m_TriangleCount += dwLength * 2;
	return Width;
}
// End gfx_Direct3D8::RenderString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderStringRight() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender      - 
//  const std_Vector3 vWhere - 
//  const DWORD dwColor      - 
//  gfx_Font* pFont          - 
//
void gfx_Direct3D8::RenderStringRight( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
	const d3d_Font* Font = (pFont==NULL) ? static_cast<d3d_Font*>(m_ActiveFont) : static_cast<d3d_Font*>(pFont);

	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, Font->m_VertexBuffer, sizeof(Font_Vert_t) );

	m_pD3DDevice->SetTexture( 0, Font->m_Texture );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );

	dxPushMatrix();
	dxTranslatef( vWhere );
	long Len = strlen( strToRender );
	for( long n=Len-1; n>=0; --n)
	{
		dxSetWorldMatrix();
		dxTranslatef( -Font->m_Entries[strToRender[n]].Size.width, 0, 0 );
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, strToRender[n] * 4, 2 );
	}
	dxPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_Direct3D8::RenderStringRight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderStringAt() //       Author: Logan "Burn" Jones
////////////////////////////////////                  Date: 10/14/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere - 
//  LPCTSTR strToRender  - 
//
void gfx_Direct3D8::RenderStringAt( std_Point_t& ptWhere, LPCTSTR strToRender )
{
	const d3d_Font* pFont = static_cast<d3d_Font*>(m_ActiveFont);

	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, pFont->m_VertexBuffer, sizeof(Font_Vert_t) );

	m_pD3DDevice->SetTexture( 0, pFont->m_Texture );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	dxPushMatrix();
	dxTranslatef( ptWhere.x, ptWhere.y/* + pFont->m_MaxHeight*/, 0 );
	long Len = strlen( strToRender );
	for( long n=0; n<Len; ++n)
	{
		dxSetWorldMatrix();
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, strToRender[n] * 4, 2 );
		dxTranslatef( pFont->m_Entries[strToRender[n]].Size.width, 0, 0 );
	}
	dxPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_Direct3D8::RenderStringAt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderStringCenteredAt() // Author: Logan "Burn" Jones
////////////////////////////////////////////          Date: 10/14/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere   - 
//  LPCTSTR strToRender    - 
//  BOOL bCenterHorizontal - 
//  BOOL bCenterVertical   - 
//
void gfx_Direct3D8::RenderStringCenteredAt( std_Point_t& ptWhere, LPCTSTR strToRender, BOOL bCenterHorizontal, BOOL bCenterVertical )
{
	long			n, Len, TotalWidth, MaxHeight;
	const d3d_Font* pFont = static_cast<d3d_Font*>(m_ActiveFont);

	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, pFont->m_VertexBuffer, sizeof(Font_Vert_t) );

	m_pD3DDevice->SetTexture( 0, pFont->m_Texture );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
//	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
//	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	Len = strlen( strToRender );
	for( n=0,TotalWidth=0,MaxHeight=0; n<Len; ++n)
	{
		const FontEntry* pEntry = pFont->m_Entries + strToRender[n];

		TotalWidth += pEntry->Size.width;

		const long height = pEntry->Size.height - (pEntry->Size.height - pEntry->Offset.y);
		if( height>MaxHeight )
			MaxHeight = height;
	}

	if( bCenterHorizontal )
		ptWhere.x -= TotalWidth / 2;

	if( bCenterVertical )
		ptWhere.y += pFont->m_Height / 2;

	dxPushMatrix();
	dxTranslatef( ptWhere.x, ptWhere.y, 0 );
	for( n=0; n<Len; ++n)
	{
		dxSetWorldMatrix();
		m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, strToRender[n] * 4, 2 );
		dxTranslatef( pFont->m_Entries[strToRender[n]].Size.width, 0, 0 );
	}
	dxPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_Direct3D8::RenderStringCenteredAt()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Font.cpp //
///////////////////////
