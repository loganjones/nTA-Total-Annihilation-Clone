// d3d_Surface.cpp //                      Author: Logan "Burn" Jones
////////////////////                                 Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Surface.h"


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateSurface() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 10/9/2001
//               
//====================================================================
// Parameters:
//  GFX_PIXEL_FORMAT pxFormat - 
//  gfx_Image_t* pImage       - 
//  gfx_Surface** ppSurface   - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateSurface( GFX_PIXEL_FORMAT pxFormat, gfx_Image_t* pImage, gfx_Surface** ppSurface )
{
#define SET_VERTEX( X, Y, Z, U, V ) \
	cpVerts[iIndex].position = D3DXVECTOR3( (X)/* - 0.5f*/, (Y)/* - 0.5f*/, Z ); \
	cpVerts[iIndex].tu = U; \
	cpVerts[iIndex].tv = V; \
	iIndex++

	LRESULT				lResult;
	DWORD				n;
	std_Size_t			TextureSize;
	gfx_SubImage_t		SubImage;
	d3d_Surface			NewSurface;

    // Sanity checks
    assert( pImage && (pImage->Size.width>0) && (pImage->Size.height>0 ) && ppSurface );

	// Check the pixel format
	switch( pxFormat )
	{
		case PIXEL_PALETTISED:
			break;
		// end case PIXEL_PALETTISED

		case PIXEL_RGB:
		{
			BYTE	Temp;
			BYTE*	rgb;

			// Direct3D8's D3DXLoadSurfaceFromMemory() seems to want BGR for an RGB image; convert it.
			rgb = pImage->pBytes;
			for( n=(*pImage->Size); n>0; --n,rgb+=3)
			{
				Temp = rgb[0];
				rgb[0] = rgb[2];
				rgb[2] = Temp;
			}
			break;
		}
		// end case PIXEL_RGB

		case PIXEL_BGR:
			pxFormat = PIXEL_RGB;
			break;
		// end case PIXEL_BGR

		case PIXEL_RGBA:
		{
			PALETTEENTRY	Old;
			PALETTEENTRY*	New;

			// Direct3D8 only has ARGB types; move the alpha up front.
			New = (PALETTEENTRY*)pImage->pBytes;
			for( n=(*pImage->Size); n>0; --n,++New)
			{
				Old = *New;
				New->peRed = Old.peFlags;
				New->peGreen = Old.peRed;
				New->peBlue = Old.peGreen;
				New->peFlags = Old.peBlue;
			}

			// Change the format and go to the next case
			pxFormat = PIXEL_ARGB;
		}
		// end case PIXEL_RGBA

		case PIXEL_ARGB:
			break;
		// end case PIXEL_ARGB

		default: // This is our error condition; not supposed to happen
			ErrorComment( "ERROR: Invalid surface creation type, %d.", pxFormat );
			assertEx( 0, "Invalid surface creation type." );
			return FALSE;

	} // end switch( pxFormat )

	// Calculate the required texture size
	TextureSize.Set( math_NextPowerOfTwo(pImage->Size.width), math_NextPowerOfTwo(pImage->Size.height) );
	NewSurface.m_ImageSize = pImage->Size;

	// Check to see if the surface needs to be chopped up
    if( TextureSize <= m_Caps.MaxTextureSize )
	{
		SubImage.pImage = pImage;
		SubImage.Size = pImage->Size;
		SubImage.Offset = std_Point_t(0,0);
		SubImage.pBytes = pImage->pBytes;

		// Create the texture
		if( FAILED(lResult=CreateTexture(
			pxFormat,
			&SubImage,
			TextureSize,
			&NewSurface.m_Textures[0] )) )
		{
			ErrorComment( "ERROR: Failed to create texture, %s %d.", GetErrorString(lResult), lResult );
			assertEx( 0, "Failed to create texture." );
			return FALSE;
		}

		NewSurface.m_Tiles.Set(1,1);
		NewSurface.m_Verts = new Surface_Vert[ *NewSurface.m_Tiles * 4 ];

		int iIndex = 0;
		Surface_Vert* cpVerts = NewSurface.m_Verts;
		float w_ratio = (float)pImage->Size.width / TextureSize.width;
		float h_ratio = (float)pImage->Size.height/ TextureSize.height;
		SET_VERTEX( 0, 0, 0, 0, 0 );
		SET_VERTEX( 0, pImage->Size.height, 0, 0, h_ratio );
		SET_VERTEX( pImage->Size.width, 0, 0, w_ratio, 0 );
		SET_VERTEX( pImage->Size.width, pImage->Size.height, 0, w_ratio, h_ratio );
		NewSurface.m_Ratio.Set( w_ratio, h_ratio );

		NewSurface.m_pHost = &m_Surfaces;
		m_Surfaces.push_front( NewSurface );
		*ppSurface = &m_Surfaces.front();
		m_Surfaces.front().m_HostIt = m_Surfaces.begin();

		return TRUE;
	}
	else // Split the surface up
	{
		std_Size_t		TileSize;
		std_Size_t		SizeOnEdge;
		std_Size_t		TexSizeOnEdge;
		std_Point_t		Offset = std_Point_t(0,0);
		BYTE*			TopLeft;
		
		SubImage.pImage = pImage;
		TopLeft = pImage->pBytes;

		// Calculate the number of texture tiles needed
		NewSurface.m_Tiles.width = (pImage->Size.width / m_Caps.MaxTextureSize.width) + ( ((pImage->Size.width % m_Caps.MaxTextureSize.width)==0) ? 0:1 );
		NewSurface.m_Tiles.height = (pImage->Size.height / m_Caps.MaxTextureSize.height) + ( ((pImage->Size.height % m_Caps.MaxTextureSize.height)==0) ? 0:1 );

		// Edge Sizes
		SizeOnEdge.width = pImage->Size.width - ( (NewSurface.m_Tiles.width - 1) * m_Caps.MaxTextureSize.width );
		SizeOnEdge.height= pImage->Size.height- ( (NewSurface.m_Tiles.height- 1) * m_Caps.MaxTextureSize.height);
		TexSizeOnEdge.width = math_NextPowerOfTwo( SizeOnEdge.width );
		TexSizeOnEdge.height= math_NextPowerOfTwo( SizeOnEdge.height);

		NewSurface.m_Verts = new Surface_Vert[ *NewSurface.m_Tiles * 4 ];

		// Iterate through each column
		for( long y=0,z=0; y<NewSurface.m_Tiles.height; ++y,Offset.y+=m_Caps.MaxTextureSize.height)
		{
			// Is this tile on the bottom row
			if( (y+1)==NewSurface.m_Tiles.height )
			{
				// Yes it is, use the edge height values
				TileSize.height = SizeOnEdge.height;
				TextureSize.height = TexSizeOnEdge.height;
			}
			else
			{
				// No it's not, use the max height values
				TileSize.height = m_Caps.MaxTextureSize.height;
				TextureSize.height = m_Caps.MaxTextureSize.height;
			}

			BYTE* pTopLeft = TopLeft;

			// Iterate through the tiles on this row
			for( long x=0; x<NewSurface.m_Tiles.width; ++x,++z,Offset.x+=m_Caps.MaxTextureSize.width)
			{
				// Is this tile on the right edge
				if( (x+1)==NewSurface.m_Tiles.width )
				{
					// Yes it is, use the edge width values
					TileSize.width = SizeOnEdge.width;
					TextureSize.width = TexSizeOnEdge.width;
				}
				else
				{
					// No it's not, use the max width values
					TileSize.width = m_Caps.MaxTextureSize.width;
					TextureSize.width = m_Caps.MaxTextureSize.width;
				}

				SubImage.Size = TileSize;
				SubImage.Offset = Offset;
				SubImage.pBytes = pTopLeft;
				CreateTexture(
					pxFormat,
					&SubImage,
					TextureSize,
					&NewSurface.m_Textures[ z ] );

				int iIndex = z * 4;
				Surface_Vert* cpVerts = NewSurface.m_Verts;
				float w_ratio = (float)TileSize.width / TextureSize.width;
				float h_ratio = (float)TileSize.height/ TextureSize.height;
				SET_VERTEX( (x*m_Caps.MaxTextureSize.width), (y*m_Caps.MaxTextureSize.height), 0, 0, 0 );
				SET_VERTEX( (x*m_Caps.MaxTextureSize.width), (y*m_Caps.MaxTextureSize.height) + TileSize.height, 0, 0, h_ratio );
				SET_VERTEX( (x*m_Caps.MaxTextureSize.width) + TileSize.width, (y*m_Caps.MaxTextureSize.height), 0, w_ratio, 0 );
				SET_VERTEX( (x*m_Caps.MaxTextureSize.width) + TileSize.width, (y*m_Caps.MaxTextureSize.height) + TileSize.height, 0, w_ratio, h_ratio );

				pTopLeft += (m_Caps.MaxTextureSize.width * pImage->Stride);

			} // end for( Number of tiles in this row )

			TopLeft += (pImage->Pitch * m_Caps.MaxTextureSize.height);

		} // end for( Number of tile rows )

		NewSurface.m_Ratio.Set( 1, 1 );
		NewSurface.m_pHost = &m_Surfaces;
		m_Surfaces.push_front( NewSurface );
		*ppSurface = &m_Surfaces.front();
		m_Surfaces.front().m_HostIt = m_Surfaces.begin();

		return TRUE;

	} // end else // Split the surface up

	return FALSE;
}
// End gfx_Direct3D8::CreateSurface()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_Surface::d3d_Surface():
	m_Verts( NULL ),
	m_pHost( NULL )
{
	ZeroMemory( m_Textures, sizeof(m_Textures) );
}
d3d_Surface::~d3d_Surface()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::Destroy() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/13/2001
//               
//====================================================================
//
void d3d_Surface::Destroy()
{
}
// End d3d_Surface::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::GetSize() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/13/2001
//               
//====================================================================
// Return: const std_Size_t& - 
//
const std_Size_t& d3d_Surface::GetSize() const
{
	return m_ImageSize;
}
// End d3d_Surface::GetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderSurface() //        Author: Logan "Burn" Jones
///////////////////////////////////                   Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  gfx_Surface* pSurface - 
//
void gfx_Direct3D8::RenderSurface( gfx_Surface* pSurface )
{
	const d3d_Surface* Surface = static_cast<d3d_Surface*>(pSurface);

	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	long x,y,z;
	for( y=0,z=0; y<Surface->m_Tiles.height; y++)
	{
		for( x=0; x<Surface->m_Tiles.width; x++,z++)
		{
			m_pD3DDevice->SetTexture( 0, Surface->m_Textures[z] );
			m_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				Surface->m_Verts + (z*4),
				sizeof(Surface_Vert) );
		}
	}
	m_TriangleCount += z * 2;
}
// End gfx_Direct3D8::RenderSurface()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderSurfaceEx() //      Author: Logan "Burn" Jones
/////////////////////////////////////                 Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere  - 
//  gfx_Surface* pSurface - 
//
void gfx_Direct3D8::RenderSurfaceEx( std_Point_t& ptWhere, gfx_Surface* pSurface )
{
	const d3d_Surface* Surface = static_cast<d3d_Surface*>(pSurface);

	dxPushMatrix();
	dxTranslatef( ptWhere.x, ptWhere.y, 0 );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU, D3DTADDRESS_CLAMP );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV, D3DTADDRESS_CLAMP );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	long x, y, z;
	for( y=0,z=0; y<Surface->m_Tiles.height; y++)
	{
		for( x=0; x<Surface->m_Tiles.width; x++,z++)
		{
			m_pD3DDevice->SetTexture( 0, Surface->m_Textures[z] );
			m_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				Surface->m_Verts + (z*4),
				sizeof(Surface_Vert) );
		}
	}
	dxPopMatrix();
	m_TriangleCount += z * 2;
}
// End gfx_Direct3D8::RenderSurfaceEx()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateTexture() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 10/9/2001
//               
//====================================================================
// Parameters:
//  GFX_PIXEL_FORMAT pxFormat     - 
//  gfx_SubImage_t* pImage        - 
//  std_Size_t& szTex             - 
//  LPDIRECT3DTEXTURE8* ppTexture - 
//
// Return: LRESULT - 
//
LRESULT gfx_Direct3D8::CreateTexture( GFX_PIXEL_FORMAT pxFormat, gfx_SubImage_t* pImage, std_Size_t& szTex, LPDIRECT3DTEXTURE8* ppTexture )
{
	LRESULT				lResult;
	LPDIRECT3DSURFACE8	pTexSurface;
	LPDIRECT3DSURFACE8	pImageSurface;
	D3DFORMAT			TextureFormat;
	D3DFORMAT			ImageFormat;
	int					Components;
	RECT				DestRect;
	RECT				SrcRect;

	// Check the pixel format
	switch( pxFormat )
	{
		case PIXEL_PALETTISED:
			TextureFormat = ImageFormat = D3DFMT_P8;
			Components = 1;
			break;
		// end case PIXEL_PALETTISED

		case PIXEL_RGB:
			TextureFormat = D3DFMT_R5G6B5;
			ImageFormat = D3DFMT_R8G8B8;
			Components = 3;
			break;
		// end case PIXEL_RGB

		case PIXEL_ARGB:
			TextureFormat = D3DFMT_A1R5G5B5;
			ImageFormat = D3DFMT_A8R8G8B8;
			Components = 4;
			break;
		// end case PIXEL_ARGB

	} // end switch( pxFormat )

	// Create the texture
	if( FAILED(lResult=m_pD3DDevice->CreateTexture(
		szTex.width,
		szTex.height,
		1,
		0,
		TextureFormat,
		D3DPOOL_MANAGED,
		ppTexture )) )
	{
		return lResult;
	}

	// Check if a straight lock and copy will be easier
	if( pxFormat==PIXEL_PALETTISED )
	{
		D3DLOCKED_RECT	LockedRect;

		if( FAILED(lResult=(*ppTexture)->LockRect( 0, &LockedRect, NULL, 0 )) )
			return lResult;

		BYTE* pTex = (BYTE*)LockedRect.pBits;
		BYTE* pImg = pImage->pBytes;
		for( long y=pImage->Size.height; y>0; --y)
		{
			memcpy( pTex, pImg, pImage->Size.width );
			pTex += LockedRect.Pitch;
			pImg += pImage->pImage->Pitch;
		}

		(*ppTexture)->UnlockRect( 0 );
		return D3D_OK;

	} // end if( PIXEL_PALETTISED )

	// Retrieve a D3D surface representing the texture memory
	if( FAILED(lResult=(*ppTexture)->GetSurfaceLevel( 0, &pTexSurface )) )
	{
		return lResult;
	}

	// Create a d3d surface for the image
	if( FAILED(lResult=m_pD3DDevice->CreateImageSurface(
		pImage->Size.width,
		pImage->Size.height,
		TextureFormat,
		&pImageSurface )) )
	{
		return lResult;
	}

	// Load the image into the image surface
    SetRect( &DestRect, 0, 0, pImage->Size.width, pImage->Size.height );
    SetRect( &SrcRect, pImage->Offset.x, pImage->Offset.y, pImage->Size.width, pImage->Size.height );
    if( FAILED(lResult=D3DXLoadSurfaceFromMemory(
        pImageSurface,
        NULL,
        &DestRect,
        pImage->pBytes,
        ImageFormat,
        pImage->pImage->Pitch,
        NULL,
        &SrcRect,
        D3DX_FILTER_NONE,
        0xFF000000 )) )
    {
		return lResult;
    }

	// Copy the image surface to the texture surface
    if( FAILED(lResult=m_pD3DDevice->CopyRects(
        pImageSurface,
        &DestRect,
        1,
        pTexSurface,
        NULL )) )
    {
        return lResult;
    }

	// Release the surfaces
    pImageSurface->Release();
    pTexSurface->Release();

	return D3D_OK;
}
// End gfx_Direct3D8::CreateTexture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
//
void d3d_Surface::Render()
{
	gfx_Direct3D8* pD3D = static_cast<gfx_Direct3D8*>(pGfxSystem);

	pD3D->dxSetWorldMatrix();
	pD3D->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	long x, y, z;
	for( y=0,z=0; y<m_Tiles.height; y++)
	{
		for( x=0; x<m_Tiles.width; x++,z++)
		{
			pD3D->m_pD3DDevice->SetTexture( 0, m_Textures[z] );
			pD3D->m_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				m_Verts + (z*4),
				sizeof(Surface_Vert) );
		}
	}
	pD3D->m_TriangleCount += z * 2;
}
// End d3d_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPos - 
//
void d3d_Surface::Render( std_Vector3 vPos )
{
	gfx_Direct3D8* pD3D = static_cast<gfx_Direct3D8*>(pGfxSystem);

	pD3D->dxPushMatrix();
	pD3D->dxTranslatef( vPos.x, vPos.y, vPos.z );
	pD3D->dxSetWorldMatrix();
	pD3D->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	long x, y, z;
	for( y=0,z=0; y<m_Tiles.height; y++)
	{
		for( x=0; x<m_Tiles.width; x++,z++)
		{
			pD3D->m_pD3DDevice->SetTexture( 0, m_Textures[z] );
			pD3D->m_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				m_Verts + (z*4),
				sizeof(Surface_Vert) );
		}
	}
	pD3D->dxPopMatrix();
	pD3D->m_TriangleCount += z * 2;
}
// End d3d_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPos - 
//  std_SizeF sz     - 
//
void d3d_Surface::Render( std_Vector3 vPos, std_SizeF sz )
{
	gfx_Direct3D8*	pD3D = static_cast<gfx_Direct3D8*>(pGfxSystem);
	Surface_Vert	Verts[4];

	pD3D->dxSetWorldMatrix();
	pD3D->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTexture( 0, m_Textures[0] );
	Verts[0].tu = 0; Verts[0].tv = 0;
	Verts[0].position = D3DXVECTOR3( vPos.x, vPos.y, vPos.z );
	Verts[1].tu = 0; Verts[1].tv = m_Ratio.height;
	Verts[1].position = D3DXVECTOR3( vPos.x, vPos.y + sz.height, vPos.z );
	Verts[2].tu = m_Ratio.width; Verts[2].tv = 0;
	Verts[2].position = D3DXVECTOR3( vPos.x + sz.width, vPos.y, vPos.z );
	Verts[3].tu = m_Ratio.width; Verts[3].tv = m_Ratio.height;
	Verts[3].position = D3DXVECTOR3( vPos.x + sz.width, vPos.y + sz.height, vPos.z );
	pD3D->m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Surface_Vert) );
	pD3D->m_TriangleCount += 2;
}
// End d3d_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctPos - 
//
void d3d_Surface::Render( std_RectF rctPos )
{
	gfx_Direct3D8*	pD3D = static_cast<gfx_Direct3D8*>(pGfxSystem);
	Surface_Vert	Verts[4];

	pD3D->dxSetWorldMatrix();
	pD3D->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	pD3D->m_pD3DDevice->SetTexture( 0, m_Textures[0] );
	Verts[0].tu = 0; Verts[0].tv = 0;
	Verts[0].position = D3DXVECTOR3( rctPos.left, rctPos.top, 0 );
	Verts[1].tu = 0; Verts[1].tv = m_Ratio.height;
	Verts[1].position = D3DXVECTOR3( rctPos.left, rctPos.bottom, 0 );
	Verts[2].tu = m_Ratio.width; Verts[2].tv = 0;
	Verts[2].position = D3DXVECTOR3( rctPos.right, rctPos.top, 0 );
	Verts[3].tu = m_Ratio.width; Verts[3].tv = m_Ratio.height;
	Verts[3].position = D3DXVECTOR3( rctPos.right, rctPos.bottom, 0 );
	pD3D->m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Surface_Vert) );
	pD3D->m_TriangleCount += 2;
}
// End d3d_Surface::Render()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Surface.cpp //
//////////////////////////
