// d3d_Cursor.cpp //                       Author: Logan "Burn" Jones
///////////////////                                  Date: 10/27/2001
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Cursor.h"

//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateCursor() //         Author: Logan "Burn" Jones
//////////////////////////////////                    Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName       - 
//  DWORD dwNumFrames     - 
//  CursorFrame* pFrames  - 
//  gfx_Cursor** ppCursor - 
//  BOOL bSetAsCurrent    - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateCursor( LPCTSTR strName, DWORD dwNumFrames, CursorFrame* pFrames, gfx_Cursor** ppCursor, BOOL bSetAsCurrent )
{
#define SET_VERTEX( X, Y, U, V ) \
	pVerts[iIndex].position = D3DXVECTOR3( (X) - 0.5f, (Y) - 0.5f, 0.0f ); \
	pVerts[iIndex].tu = U; \
	pVerts[iIndex].tv = V; \
	++iIndex

	d3d_Cursor		NewCursor;
	Font_Vert_t*	pVerts;
	long			iIndex = 0;
	std_Size_t		TexSize;
	D3DLOCKED_RECT	LockedRect;
	BYTE*			pImg;
	BYTE*			pTex;
	LRESULT			lResult;

	strcpy( NewCursor.m_Name, strName );
	NewCursor.m_pTextures = new LPDIRECT3DTEXTURE8[ dwNumFrames ];
	
	// Create the font vertex buffer
	if( FAILED(lResult=m_pD3DDevice->CreateVertexBuffer(
		dwNumFrames * 4 * sizeof(Font_Vert_t),
        0,
		D3DFVF_FONT_VERT,
        D3DPOOL_MANAGED,
		&NewCursor.m_VertexBuffer ) ) )
    {
		ErrorComment( "ERROR: Failed to create font vertex buffer, %s %d.", GetErrorString(lResult), lResult );
		assertEx( 0, "Failed to create font vertex buffer." );
        return FALSE;
    }

    if( FAILED(lResult=NewCursor.m_VertexBuffer->Lock( 0, 0, (BYTE**)&pVerts, 0 )) )
	{
		ErrorComment( "ERROR: Failed to lock font vertex buffer, %s %d.", GetErrorString(lResult), lResult );
		assertEx( 0, "Failed to lock font vertex buffer." );
        return FALSE;
	}

// TODO: Combine all the cursor frames into one texture

	for( DWORD num=0; num<dwNumFrames; ++num)
	{
		TexSize.width = math_NextPowerOfTwo( pFrames[num].Image.Size.width );
		TexSize.height= math_NextPowerOfTwo( pFrames[num].Image.Size.height);

		// Create the texture
		if( FAILED(lResult=m_pD3DDevice->CreateTexture(
			TexSize.width,
			TexSize.height,
			1,
			0,
			D3DFMT_P8,
			D3DPOOL_MANAGED,
			&NewCursor.m_pTextures[num] )) )
		{
			continue;
		}

		if( FAILED(lResult=NewCursor.m_pTextures[num]->LockRect( 0, &LockedRect, NULL, 0 )) )
			continue;
		pTex = (BYTE*)LockedRect.pBits;
		pImg = pFrames[num].Image.pBytes;
		for( long y=pFrames[num].Image.Size.height; y>0; --y)
		{
			memcpy( pTex, pImg, pFrames[num].Image.Size.width );
			pTex += LockedRect.Pitch;
			pImg += pFrames[num].Image.Pitch;
		}
		NewCursor.m_pTextures[num]->UnlockRect( 0 );

		SET_VERTEX( 0 - pFrames[num].HotSpot.x, 0 - pFrames[num].HotSpot.y, 0, 0 );
		SET_VERTEX( 0 - pFrames[num].HotSpot.x, pFrames[num].Image.Size.height - pFrames[num].HotSpot.y, 0 , float(pFrames[num].Image.Size.height) / TexSize.height );
		SET_VERTEX( pFrames[num].Image.Size.width - pFrames[num].HotSpot.x, 0 - pFrames[num].HotSpot.y, float(pFrames[num].Image.Size.width) / TexSize.width, 0 );
		SET_VERTEX( pFrames[num].Image.Size.width - pFrames[num].HotSpot.x, pFrames[num].Image.Size.height - pFrames[num].HotSpot.y, float(pFrames[num].Image.Size.width) / TexSize.width , float(pFrames[num].Image.Size.height) / TexSize.height );

	} // end for( dwNumFrames )

	// Unlock the vertex buffer
	NewCursor.m_VertexBuffer->Unlock();


	// Set the rest of the cursor's data and add it to the list
	NewCursor.m_NumFrames = dwNumFrames;
	NewCursor.m_CurrentFrame = 0;
	NewCursor.m_pHost = &m_Cursors;
	m_Cursors.push_front( NewCursor );
	*ppCursor = &m_Cursors.front();
	m_Cursors.front().m_HostIt = m_Cursors.begin();

	// Set the new font as the current one if desired
	if( bSetAsCurrent )
		SetCurrentCursor( *ppCursor );

	// Return success
	return TRUE;
}
// End gfx_Direct3D8::CreateCursor()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_Cursor::d3d_Cursor()
{}
d3d_Cursor::~d3d_Cursor()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_Cursor::Destroy() //                 Author: Logan "Burn" Jones
//////////////////////////                            Date: 10/27/2001
//               
//====================================================================
//
void d3d_Cursor::Destroy()
{
	if( m_pHost )
	{
		SAFE_RELEASE( m_VertexBuffer );
		for( int n=0; n<m_NumFrames; ++n)
			SAFE_RELEASE( m_pTextures[n] );
		SAFE_DELETE_ARRAY( m_pTextures );
		m_pHost->erase( m_HostIt );
		m_pHost = NULL;
	}
}
// End d3d_Cursor::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::SetCurrentCursor() //     Author: Logan "Burn" Jones
//////////////////////////////////////                Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  gfx_Cursor* pCursor - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::SetCurrentCursor( gfx_Cursor* pCursor )
{
	return (m_ActiveCursor = pCursor)!=NULL;
}
// End gfx_Direct3D8::SetCurrentCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::SetCurrentCursor() //     Author: Logan "Burn" Jones
//////////////////////////////////////                Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::SetCurrentCursor( LPCTSTR strName )
{
	CursorList_t::iterator		it = m_Cursors.begin();
	CursorList_t::iterator		end = m_Cursors.end();

	for( ; it!=end; ++it )
	{
		if( stricmp((*it).m_Name,strName)==0 )
		{
			m_ActiveCursor = &(*it);
			return TRUE;
		}
	}

	// If we get here, we have failed
	return FALSE;
}
// End gfx_Direct3D8::SetCurrentCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::UpdateCursor() //         Author: Logan "Burn" Jones
//////////////////////////////////                    Date: 10/27/2001
//               
//====================================================================
//
void gfx_Direct3D8::UpdateCursor()
{
	if( m_ActiveCursor )
	{
		d3d_Cursor* pCur = static_cast<d3d_Cursor*>(m_ActiveCursor);
		++pCur->m_CurrentFrame;
		if( pCur->m_CurrentFrame>=pCur->m_NumFrames )
			pCur->m_CurrentFrame = 0;
	}
}
// End gfx_Direct3D8::UpdateCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::RenderCursor() //         Author: Logan "Burn" Jones
//////////////////////////////////                    Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  std_Vector2 vWhere - 
//
void gfx_Direct3D8::RenderCursor( std_Vector2 vWhere )
{
	d3d_Cursor* pCur = static_cast<d3d_Cursor*>(m_ActiveCursor);

	dxPushMatrix();
	dxTranslatef( vWhere.x, vWhere.y, 0 );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_FONT_VERT );
	m_pD3DDevice->SetStreamSource( 0, pCur->m_VertexBuffer, sizeof(Font_Vert_t) );
	m_pD3DDevice->SetTexture( 0, pCur->m_pTextures[pCur->m_CurrentFrame] );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
	//m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_POINT );
	m_pD3DDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_POINT );
	m_pD3DDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, pCur->m_CurrentFrame * 4, 2 );
	dxPopMatrix();
	m_TriangleCount += 2;
}
// End gfx_Direct3D8::RenderCursor()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Cursor.cpp //
/////////////////////////
