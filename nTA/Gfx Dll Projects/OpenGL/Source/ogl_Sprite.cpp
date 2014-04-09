// ogl_Sprite.cpp //                       Author: Logan "Burn" Jones
///////////////////                                   Date: 2/23/2002
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Sprite.h"


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateSprite() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 2/23/2002
//               
//====================================================================
// Parameters:
//  gfx_Image_t* pImage   - 
//  std_Vector2 vOffset   - 
//  gfx_Sprite** ppSprite - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateSprite( gfx_Image_t* pImage, std_Vector2 vOffset, gfx_Sprite** ppSprite )
{
	ogl_Sprite*			pNewSprite;

	// Sanity checks
	if( !pImage || !ppSprite )
		return FALSE;

	// Check to see if the surface needs to be chopped up
	if(true)//( !(TextureSize <= m_Caps.MaxTextureSize) )
	{
		gfx_Surface*		pSurface = NULL;

		// Create a surface from the sprite image
		if( bFAILED(CreateSurface( PIXEL_PALETTISED, pImage, &pSurface )) )
			return FALSE;

		pNewSprite = new ogl_Sprite;
		pNewSprite->m_pSurface = pSurface;
		pNewSprite->m_pHostInterface = this;
		pNewSprite->m_ImageOffset = vOffset;
	}
	else
	{
	}

	(*ppSprite) = pNewSprite;
	return TRUE;
}
// End gfx_OpenGL::CreateSprite()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Sprite::ogl_Sprite():
	m_pSurface( NULL )
{}
ogl_Sprite::~ogl_Sprite()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Sprite::Destroy() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 2/23/2002
//               
//====================================================================
//
void ogl_Sprite::Destroy()
{
	SAFE_DESTROY( m_pSurface );
}
// End ogl_Sprite::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Sprite::Render() //                  Author: Logan "Burn" Jones
/////////////////////////                              Date: 2/23/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPosition - 
//
void ogl_Sprite::Render( std_Vector3 vPosition ) const
{
	// Render the surface if its valid
	if( m_pSurface )
	{
		glPushMatrix();
		glTranslatef( vPosition.x + m_ImageOffset.x, vPosition.y + m_ImageOffset.y, vPosition.z );
		m_pHostInterface->RenderSurface( m_pSurface );
		glPopMatrix();
	}
	else
	{
	}
}
// End ogl_Sprite::Render()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Sprite.cpp //
/////////////////////////
