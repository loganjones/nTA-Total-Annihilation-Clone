// d3d_DynamicImage.cpp //                 Author: Logan "Burn" Jones
/////////////////////////                             Date: 5/14/2002
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_DynamicImage.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_DynamicImage::d3d_DynamicImage()
{}
d3d_DynamicImage::~d3d_DynamicImage()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateDynamicImage() //   Author: Logan "Burn" Jones
////////////////////////////////////////               Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Size szImage           - 
//  gfx_DynamicImage** ppImage - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateDynamicImage( std_Size szImage, gfx_DynamicImage** ppImage )
{
	d3d_DynamicImage*	pImage;
	BOOL				bResult = FALSE;
	LRESULT				lResult;

	m_DynamicImages.push_front();
	pImage = &m_DynamicImages.front();

	BEGIN_CODE_BLOCK

		pImage->m_TextureSize.Set( math_NextPowerOfTwo(szImage.width), math_NextPowerOfTwo(szImage.height) );

		// Create the static texture
		if( FAILED(lResult=m_pD3DDevice->CreateTexture(
			pImage->m_TextureSize.width,
			pImage->m_TextureSize.height,
			1,0,
			D3DFMT_P8, D3DPOOL_DEFAULT,
			&pImage->m_VideoMemTexture )) )
			EXIT_CODE_BLOCK;

		// Create the system texture
		if( FAILED(lResult=m_pD3DDevice->CreateTexture(
			pImage->m_TextureSize.width,
			pImage->m_TextureSize.height,
			1,0,
			D3DFMT_P8, D3DPOOL_SYSTEMMEM,
			&pImage->m_SystemMemTexture )) )
			EXIT_CODE_BLOCK;

	bResult = TRUE;
	END_CODE_BLOCK

	if( bResult )
	{
		pImage->m_ImageSize = szImage;
		pImage->m_SizeRatio.Set( float(szImage.width) / pImage->m_TextureSize.width, float(szImage.height) / pImage->m_TextureSize.height );
		pImage->m_pInterface = this;
		pImage->m_Index = m_DynamicImages.begin();
		*ppImage = pImage;
		return TRUE;
	}
	else
	{
		m_DynamicImages.pop_front();
		return FALSE;
	}
}
// End gfx_Direct3D8::CreateDynamicImage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::Destroy() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
//
void d3d_DynamicImage::Destroy()
{
	SAFE_RELEASE( m_VideoMemTexture );
	SAFE_RELEASE( m_SystemMemTexture );
	m_pInterface->m_DynamicImages.erase( m_Index );
}
// End d3d_DynamicImage::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::GetSize() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
// Return: const std_Size& - 
//
const std_Size& d3d_DynamicImage::GetSize() const
{
	return m_TextureSize;
}
// End d3d_DynamicImage::GetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::Render() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//
void d3d_DynamicImage::Render( std_Vector3 vPosition )
{
	Surface_Vert		Verts[4];
	Verts[0].position = D3DXVECTOR3(vPosition.x,vPosition.y,vPosition.z); Verts[0].tu = 0; Verts[0].tv = 0;
	Verts[1].position = D3DXVECTOR3(vPosition.x,vPosition.y+m_ImageSize.height,vPosition.z); Verts[1].tu = 0; Verts[1].tv = m_SizeRatio.height;
	Verts[2].position = D3DXVECTOR3(vPosition.x+m_ImageSize.width,vPosition.y,vPosition.z); Verts[2].tu = m_SizeRatio.width; Verts[2].tv = 0;
	Verts[3].position = D3DXVECTOR3(vPosition.x+m_ImageSize.width,vPosition.y+m_ImageSize.height,vPosition.z); Verts[3].tu = m_SizeRatio.width; Verts[3].tv = m_SizeRatio.height;

	m_pInterface->dxSetWorldMatrix();
	m_pInterface->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	m_pInterface->m_pD3DDevice->SetTexture( 0, m_VideoMemTexture );
	m_pInterface->m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Surface_Vert) );
	m_pInterface->m_TriangleCount += 2;
}
// End d3d_DynamicImage::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::Render() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//  std_SizeF sz          - 
//
void d3d_DynamicImage::Render( std_Vector3 vPosition, std_SizeF sz )
{
	Surface_Vert		Verts[4];
	Verts[0].position = D3DXVECTOR3(vPosition.x,vPosition.y,vPosition.z); Verts[0].tu = 0; Verts[0].tv = 0;
	Verts[1].position = D3DXVECTOR3(vPosition.x,vPosition.y+sz.height,vPosition.z); Verts[1].tu = 0; Verts[1].tv = m_SizeRatio.height;
	Verts[2].position = D3DXVECTOR3(vPosition.x+sz.width,vPosition.y,vPosition.z); Verts[2].tu = m_SizeRatio.width; Verts[2].tv = 0;
	Verts[3].position = D3DXVECTOR3(vPosition.x+sz.width,vPosition.y+sz.height,vPosition.z); Verts[3].tu = m_SizeRatio.width; Verts[3].tv = m_SizeRatio.height;

	m_pInterface->dxSetWorldMatrix();
	m_pInterface->m_pD3DDevice->SetVertexShader( D3DFVF_SURFACE_VERT );
	m_pInterface->m_pD3DDevice->SetTexture( 0, m_VideoMemTexture );
	m_pInterface->m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Surface_Vert) );
	m_pInterface->m_TriangleCount += 2;
}
// End d3d_DynamicImage::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::GetBits() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  BYTE** ppImage - 
//
void d3d_DynamicImage::GetBits( BYTE** ppImage )
{
	m_SystemMemTexture->LockRect( 0, &m_LockedRect, NULL, 0 );
	(*ppImage) = (BYTE*)m_LockedRect.pBits;
}
// End d3d_DynamicImage::GetBits()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// d3d_DynamicImage::gfx_Direct3D8::Update() // Author: Logan "Burn" Jones
//////////////////////////////////////////////         Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  BYTE** ppImage - 
//
void d3d_DynamicImage::Update( BYTE** ppImage )
{
	m_SystemMemTexture->UnlockRect( 0 );
	m_pInterface->m_pD3DDevice->UpdateTexture( m_SystemMemTexture, m_VideoMemTexture );
}
// End d3d_DynamicImage::gfx_Direct3D8::Update()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_DynamicImage.cpp //
///////////////////////////////
