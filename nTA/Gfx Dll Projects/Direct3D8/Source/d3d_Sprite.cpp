// d3d_Sprite.cpp //                       Author: Logan "Burn" Jones
///////////////////                                   Date: 2/23/2002
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Sprite.h"
#include "../../../Source/util_RectFiller.h"

//#define USE_SPRITE_BUFFER
//#define USE_STRIPS


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateSpriteBuffer() //   Author: Logan "Burn" Jones
////////////////////////////////////////                Date: 7/7/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateSpriteBuffer()
{
	// Create the vertex buffer
	if( FAILED( m_pD3DDevice->CreateVertexBuffer(
		128*3 * sizeof(Surface_Vert),
		D3DUSAGE_WRITEONLY,
		D3DFVF_SURFACE_VERT,
		D3DPOOL_MANAGED,
		&m_SpriteBuffer ) ) )
    {
        return FALSE;
    }

	m_SpriteBufferOffset = 0;
	m_CurrentSpriteTexture = NULL;
	return TRUE;
}
// End gfx_Direct3D8::CreateSpriteBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DestroySpriteBuffer() //  Author: Logan "Burn" Jones
/////////////////////////////////////////               Date: 7/7/2002
//               
//====================================================================
//
void gfx_Direct3D8::DestroySpriteBuffer()
{
	SAFE_RELEASE( m_SpriteBuffer );
}
// End gfx_Direct3D8::DestroySpriteBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::FlushSpriteBuffer() //    Author: Logan "Burn" Jones
///////////////////////////////////////                 Date: 7/7/2002
//               
//====================================================================
//
void gfx_Direct3D8::FlushSpriteBuffer()
{
#ifdef USE_SPRITE_BUFFER
	m_pD3DDevice->SetTexture( 0, m_CurrentSpriteTexture );
	m_pD3DDevice->SetStreamSource( 0, m_SpriteBuffer, sizeof(Surface_Vert) );
	m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );

	dxSetWorldMatrix();
#ifdef USE_STRIPS
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_SpriteBufferOffset / 2 );
	m_TriangleCount += m_SpriteBufferOffset / 3;
#else
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_SpriteBufferOffset / 3 );
	m_TriangleCount += m_SpriteBufferOffset / 3;
#endif

	m_SpriteBufferOffset = 0;
	m_CurrentSpriteTexture = NULL;
#endif
}
// End gfx_Direct3D8::FlushSpriteBuffer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateSprite() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 2/23/2002
//               
//====================================================================
// Parameters:
//  gfx_Image_t* pImage   - 
//  std_Vector2 vOffset   - 
//  gfx_Sprite** ppSprite - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateSprite( gfx_Image_t* pImage, std_Vector2 vOffset, gfx_Sprite** ppSprite )
{
	d3d_Sprite*			pNewSprite;
	SpriteTexItem_t		NewSpriteTex;
	SpriteTexItem_t*	pSpriteTex;
	std_Rect			TexRect;
	std_Size			TextureSize;
	LPDIRECT3DTEXTURE8	pTexture;
	D3DLOCKED_RECT		LockedRect;

	// Sanity checks
	if( !pImage || !ppSprite )
		return FALSE;

	if( m_SpriteBuffer==NULL )
		CreateSpriteBuffer();

	// Check to see if the surface needs to be chopped up
	if( !(TextureSize <= std_Size(256,256)) )
	{
		gfx_Surface*		pSurface = NULL;

		// Create a surface from the sprite image
		if( bFAILED(CreateSurface( PIXEL_PALETTISED, pImage, &pSurface )) )
			return FALSE;

		pNewSprite = new d3d_Sprite;
		pNewSprite->m_pSurface = pSurface;
	}
	else
	{
		BOOL NewTexNeeded = FALSE;

		// Try to fit the sprite into a texture
		if( m_SpriteTextures.empty() ) NewTexNeeded = TRUE;
		else
		{
			pSpriteTex = &m_SpriteTextures.back();
			pTexture = pSpriteTex->pTexture;
			NewTexNeeded = !pSpriteTex->pSpace->FindSuitableRect( pImage->Size.width, pImage->Size.height, &TexRect );
		}

		// If a new texture needs to be created, do so
		if( NewTexNeeded )
		{
			LoadComment( "New sprite tex created." );
			ZeroMemory( &NewSpriteTex, sizeof(NewSpriteTex) );
			m_SpriteTextures.push_back( NewSpriteTex );
			pSpriteTex = &m_SpriteTextures.back();

			// Create the texture
			if( FAILED(m_pD3DDevice->CreateTexture( m_SpriteTextureSize.width, m_SpriteTextureSize.height, 1, 0, D3DFMT_P8, D3DPOOL_MANAGED, &pSpriteTex->pTexture )) )
			{
				m_SpriteTextures.pop_back();
				return FALSE;
			}

			pSpriteTex->pSpace = new util_RectFiller;
			pSpriteTex->pSpace->Initialize( m_SpriteTextureSize.width, m_SpriteTextureSize.height );
			pSpriteTex->pSpace->FindSuitableRect( pImage->Size.width, pImage->Size.height, &TexRect );
			pTexture = pSpriteTex->pTexture;
		}

		pTexture->LockRect( 0, &LockedRect, NULL, 0 );
		BYTE* pTex = ((BYTE*)LockedRect.pBits) + (TexRect.left + TexRect.top * LockedRect.Pitch);
		BYTE* pImg = pImage->pBytes;
		for( long y=pImage->Size.height; y>0; --y)
		{
			memcpy( pTex, pImg, pImage->Size.width );
			pTex += LockedRect.Pitch;
			pImg += pImage->Pitch;
		}
		pTexture->UnlockRect( 0 );

		pNewSprite = new d3d_Sprite;
		pNewSprite->m_pSurface = NULL;
		pNewSprite->m_pTexture = pTexture;
		pNewSprite->m_SpriteRect = TexRect;
	}

	pNewSprite->m_pHostInterface = this;
	pNewSprite->m_ImageOffset = vOffset;
	(*ppSprite) = pNewSprite;
	return TRUE;
}
// End gfx_Direct3D8::CreateSprite()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_Sprite::d3d_Sprite():
	m_pSurface( NULL ),
	m_pTexture( NULL )
{}
d3d_Sprite::~d3d_Sprite()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Sprite::Destroy() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 2/23/2002
//               
//====================================================================
//
void d3d_Sprite::Destroy()
{
	SAFE_RELEASE( m_pTexture );
	SAFE_DESTROY( m_pSurface );
}
// End d3d_Sprite::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Sprite::Render() //                  Author: Logan "Burn" Jones
/////////////////////////                              Date: 2/23/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//
void d3d_Sprite::Render( std_Vector3 vPosition ) const
{
	// Render the surface if its valid
	if( m_pSurface )
	{
		m_pHostInterface->FlushSpriteBuffer();

		m_pHostInterface->dxPushMatrix();
		m_pHostInterface->dxTranslatef( vPosition + m_ImageOffset );
		m_pHostInterface->RenderSurface( m_pSurface );
		m_pHostInterface->dxPopMatrix();
	}
	else
	{
		const float			x = vPosition.x + m_ImageOffset.x;
		const float			y = vPosition.y + m_ImageOffset.y;
		const float			z = vPosition.z;
		const float			width = m_SpriteRect.Width();
		const float			height= m_SpriteRect.Height();
		const float			leftX = (float)m_SpriteRect.left * (1.0f / m_pHostInterface->m_SpriteTextureSize.width);
		const float			rightX= (float)m_SpriteRect.right* (1.0f / m_pHostInterface->m_SpriteTextureSize.width);
		const float			topY   = (float)m_SpriteRect.top * (1.0f / m_pHostInterface->m_SpriteTextureSize.height);
		const float			bottomY= (float)m_SpriteRect.bottom* (1.0f / m_pHostInterface->m_SpriteTextureSize.height);
#ifndef USE_SPRITE_BUFFER
		Surface_Vert		Verts[4];
		Verts[0].tu = leftX; Verts[0].tv = topY;
		Verts[0].position = D3DXVECTOR3( x, y, z );
		Verts[1].tu = leftX; Verts[1].tv = bottomY;
		Verts[1].position = D3DXVECTOR3( x, y + height, z );
		Verts[2].tu = rightX; Verts[2].tv = topY;
		Verts[2].position = D3DXVECTOR3( x + width, y, z );
		Verts[3].tu = rightX; Verts[3].tv = bottomY;
		Verts[3].position = D3DXVECTOR3( x + width, y + height, z );

		m_pHostInterface->m_pD3DDevice->SetTexture( 0, m_pTexture );
		m_pHostInterface->dxSetWorldMatrix();
		m_pHostInterface->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
		m_pHostInterface->m_pD3DDevice->DrawPrimitiveUP(
				D3DPT_TRIANGLESTRIP,
				2,
				Verts,
				sizeof(Surface_Vert) );
		m_pHostInterface->m_TriangleCount += 2;
#else
		if( m_pHostInterface->m_CurrentSpriteTexture!=m_pTexture || m_pHostInterface->m_SpriteBufferOffset+4>=128*3 )
			m_pHostInterface->FlushSpriteBuffer();

		Surface_Vert* pVerts;
		m_pHostInterface->m_CurrentSpriteTexture = m_pTexture;
#ifdef USE_STRIPS
		m_pHostInterface->m_SpriteBuffer->Lock(
			m_pHostInterface->m_SpriteBufferOffset * sizeof(Surface_Vert),
			4 * sizeof(Surface_Vert),
			(BYTE**)&pVerts,
			0 );
		m_pHostInterface->m_SpriteBufferOffset += 4;
#else
		m_pHostInterface->m_SpriteBuffer->Lock(
			m_pHostInterface->m_SpriteBufferOffset * sizeof(Surface_Vert),
			6 * sizeof(Surface_Vert),
			(BYTE**)&pVerts,
			0 );
		m_pHostInterface->m_SpriteBufferOffset += 6;
#endif

		pVerts[0].tu = leftX; pVerts[0].tv = topY;
		pVerts[0].position = D3DXVECTOR3( x, y, z );
		pVerts[1].tu = leftX; pVerts[1].tv = bottomY;
		pVerts[1].position = D3DXVECTOR3( x, y + height, z );
		pVerts[2].tu = rightX; pVerts[2].tv = topY;
		pVerts[2].position = D3DXVECTOR3( x + width, y, z );
#ifdef USE_STRIPS
		pVerts[3].tu = rightX; pVerts[3].tv = bottomY;
		pVerts[3].position = D3DXVECTOR3( x + width, y + height, z );
#else
		pVerts[3].tu = rightX; pVerts[3].tv = topY;
		pVerts[3].position = D3DXVECTOR3( x + width, y, z );
		pVerts[4].tu = leftX; pVerts[4].tv = bottomY;
		pVerts[4].position = D3DXVECTOR3( x, y + height, z );
		pVerts[5].tu = rightX; pVerts[5].tv = bottomY;
		pVerts[5].position = D3DXVECTOR3( x + width, y + height, z );
#endif
		m_pHostInterface->m_SpriteBuffer->Unlock();
#endif
	}
}
// End d3d_Sprite::Render()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Sprite.cpp //
/////////////////////////
