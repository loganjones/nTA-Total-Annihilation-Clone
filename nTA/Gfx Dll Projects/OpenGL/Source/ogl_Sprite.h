// ogl_Sprite.h //                         Author: Logan "Burn" Jones
/////////////////                                     Date: 2/23/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_SPRITE_H_
#define _OGL_SPRITE_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// ogl_Sprite /////////////////////////////
//
class ogl_Sprite : public gfx_Sprite
{
	friend class gfx_OpenGL;

/////////////////////////////////////////////////
// INTERFACE
public:

	void Destroy();

	void Render( std_Vector3 vPosition ) const;


/////////////////////////////////////////////////
// Data members
protected:

	// Pointer to the interface that created this terrain object
	gfx_OpenGL*					m_pHostInterface;

	// If more than one texture is needed for the sprite
	// this will point to a surface for the sprite, otherwise
	// it will be NULL
	class gfx_Surface*			m_pSurface;

	// Translation offset for the image
	std_Vector2					m_ImageOffset;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	ogl_Sprite();
	virtual ~ogl_Sprite();
/////////////////////////////////////////////////

}; // End class - ogl_Sprite
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_SPRITE_H_)
