// ogl_DynamicImage.cpp //                 Author: Logan "Burn" Jones
/////////////////////////                             Date: 5/14/2002
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_DynamicImage.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_DynamicImage::ogl_DynamicImage():m_LocalCopy( NULL ),m_LocalTex( NULL )
{}
ogl_DynamicImage::~ogl_DynamicImage()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateDynamicImage() //   Author: Logan "Burn" Jones
////////////////////////////////////////               Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Size szImage           - 
//  gfx_DynamicImage** ppImage - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateDynamicImage( std_Size szImage, gfx_DynamicImage** ppImage )
{
	ogl_DynamicImage*	pImage;
	BOOL				bResult = FALSE;
	const BOOL			NeedsPaletteConversion = (glColorTableEXT == NULL);
	const GLuint		GLTexInternalFormat = NeedsPaletteConversion ? GL_RGBA : GL_COLOR_INDEX8_EXT;
	const GLuint		GLTexFormat = NeedsPaletteConversion ? GL_RGBA : GL_COLOR_INDEX;

	m_DynamicImages.push_front(ogl_DynamicImage());
	pImage = &m_DynamicImages.front();

	BEGIN_CODE_BLOCK

		glGenTextures( 1, &pImage->m_TextureIndex );
		glBindTexture( GL_TEXTURE_2D, pImage->m_TextureIndex );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		pImage->m_TextureSize.Set( math_NextPowerOfTwo((UINT32)szImage.width), math_NextPowerOfTwo((UINT32)szImage.height) );
		pImage->m_LocalCopy = new BYTE[ *pImage->m_TextureSize ];
		pImage->m_LocalTex = NeedsPaletteConversion ? new BYTE[*pImage->m_TextureSize * 4] : pImage->m_LocalCopy;
		pImage->m_ImageSize = szImage;
		glTexImage2D(
            GL_TEXTURE_2D,
            0,
			GLTexInternalFormat,
            (GLsizei)pImage->m_TextureSize.width,
            (GLsizei)pImage->m_TextureSize.height,
            0,
			GLTexFormat,
            GL_UNSIGNED_BYTE,
            pImage->m_LocalTex );

	bResult = TRUE;
	END_CODE_BLOCK

	if( bResult )
	{
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
// End gfx_OpenGL::CreateDynamicImage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::Destroy() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
//
void ogl_DynamicImage::Destroy()
{
	if ( m_LocalTex != m_LocalCopy )
	{
		SAFE_DELETE_ARRAY( m_LocalTex );
	}
	SAFE_DELETE_ARRAY( m_LocalCopy );
	glDeleteTextures( 1, &m_TextureIndex );
	m_pInterface->m_DynamicImages.erase( m_Index );
}
// End ogl_DynamicImage::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::GetSize() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
// Return: const std_Size& - 
//
const std_Size& ogl_DynamicImage::GetSize() const
{
	return m_TextureSize;
}
// End ogl_DynamicImage::GetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::Render() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//
void ogl_DynamicImage::Render( std_Vector3 vPosition )
{
	glBindTexture( GL_TEXTURE_2D, m_TextureIndex );	
	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f(0,0);
		glVertex3f(vPosition.x,vPosition.y,vPosition.z);
		glTexCoord2f(0,m_SizeRatio.height);
		glVertex3f(vPosition.x,vPosition.y+m_ImageSize.height,vPosition.z);
		glTexCoord2f(m_SizeRatio.width,0);
		glVertex3f(vPosition.x+m_ImageSize.width,vPosition.y,vPosition.z);
		glTexCoord2f(m_SizeRatio.width,m_SizeRatio.height);
		glVertex3f(vPosition.x+m_ImageSize.width,vPosition.y+m_ImageSize.height,vPosition.z);
	glEnd();
	m_pInterface->m_TriangleCount += 2;
}
// End ogl_DynamicImage::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::Render() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//  std_SizeF sz          - 
//
void ogl_DynamicImage::Render( std_Vector3 vPosition, std_SizeF sz )
{
	glBindTexture( GL_TEXTURE_2D, m_TextureIndex );
	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f(0,0);
		glVertex3f(vPosition.x,vPosition.y,vPosition.z);
		glTexCoord2f(0,m_SizeRatio.height);
		glVertex3f(vPosition.x,vPosition.y+sz.height,vPosition.z);
		glTexCoord2f(m_SizeRatio.width,0);
		glVertex3f(vPosition.x+sz.width,vPosition.y,vPosition.z);
		glTexCoord2f(m_SizeRatio.width,m_SizeRatio.height);
		glVertex3f(vPosition.x+sz.width,vPosition.y+sz.height,vPosition.z);
	glEnd();
	m_pInterface->m_TriangleCount += 2;
}
// End ogl_DynamicImage::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::GetBits() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  BYTE** ppImage - 
//
void ogl_DynamicImage::GetBits( BYTE** ppImage )
{
	(*ppImage) = m_LocalCopy;
}
// End ogl_DynamicImage::GetBits()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_DynamicImage::gfx_Direct3D8::Update() // Author: Logan "Burn" Jones
//////////////////////////////////////////////         Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  BYTE** ppImage - 
//
void ogl_DynamicImage::Update( BYTE** ppImage )
{
	const BOOL			NeedsPaletteConversion = (m_LocalTex != m_LocalCopy);
	const GLuint		GLTexFormat = NeedsPaletteConversion ? GL_RGBA : GL_COLOR_INDEX;

	if( NeedsPaletteConversion )
	{
		gfx_Interface::LPPALETTE pPalette;
		pGfxSystem->GetCurrentPalette(&pPalette);

		gfx_Image_t		SourceImage;
		SourceImage.pBytes = m_LocalCopy;
		SourceImage.Size = m_TextureSize;
		SourceImage.Stride = 1;
		SourceImage.Pitch = m_TextureSize.width;
		gfx_Image_t		ConvertedImage;
		gfx_OpenGL::ResolvePalettedImage(&SourceImage, pPalette, PIXEL_RGBA, &ConvertedImage);

		SAFE_DELETE_ARRAY(m_LocalTex);
		m_LocalTex = ConvertedImage.pBytes;
	}

	glBindTexture( GL_TEXTURE_2D, m_TextureIndex );	
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		0, 0,
		(GLsizei)m_TextureSize.width,
		(GLsizei)m_TextureSize.height,
		GLTexFormat,
		GL_UNSIGNED_BYTE,
		m_LocalTex);
}
// End ogl_DynamicImage::gfx_Direct3D8::Update()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_DynamicImage.cpp //
///////////////////////////////
