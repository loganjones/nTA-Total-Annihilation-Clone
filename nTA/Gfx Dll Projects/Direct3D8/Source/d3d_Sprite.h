// d3d_Sprite.h //                         Author: Logan "Burn" Jones
/////////////////                                     Date: 2/23/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_SPRITE_H_
#define _D3D_SPRITE_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// d3d_Sprite /////////////////////////////
//
class d3d_Sprite : public gfx_Sprite
{
	friend class gfx_Direct3D8;

/////////////////////////////////////////////////
// INTERFACE
public:

	void Destroy();

	void Render( std_Vector3 vPosition ) const;


/////////////////////////////////////////////////
// Data members
protected:

	// Pointer to the interface that created this terrain object
	gfx_Direct3D8*				m_pHostInterface;

	// If more than one texture is needed for the sprite
	// this will point to a surface for the sprite, otherwise
	// it will be NULL
	class gfx_Surface*			m_pSurface;

	// Texture used for the sprite image
	LPDIRECT3DTEXTURE8			m_pTexture;

	// Rect specifying the position of the sprite in the texture
	std_Rect					m_SpriteRect;

	// Translation offset for the image
	std_Vector2					m_ImageOffset;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	d3d_Sprite();
	virtual ~d3d_Sprite();
/////////////////////////////////////////////////

}; // End class - d3d_Sprite
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_SPRITE_H_)
