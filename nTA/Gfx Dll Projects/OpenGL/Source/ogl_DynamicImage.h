// ogl_DynamicImage.h //                   Author: Logan "Burn" Jones
///////////////////////                               Date: 5/14/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_DYNAMICIMAGE_H_
#define _OGL_DYNAMICIMAGE_H_
/////////////////////////////////////////////////////////////////////

class gfx_OpenGL;


////////////////////////// ogl_DynamicImage //////////////////////////
//
class ogl_DynamicImage : public gfx_DynamicImage
{

/////////////////////////////////////////////////
// INTERFACE
public:

	virtual void Destroy();

	virtual const std_Size_t& GetSize() const;

	virtual void Render( std_Vector3 vPosition );
	virtual void Render( std_Vector3 vPosition, std_SizeF sz );

	virtual void GetBits( BYTE** ppImage );
	virtual void Update( BYTE** ppImage );


/////////////////////////////////////////////////
// Data members
public:

	list<ogl_DynamicImage>::iterator	m_Index;
	gfx_OpenGL*							m_pInterface;
	std_Size							m_ImageSize;
	std_Size							m_TextureSize;
	std_SizeF							m_SizeRatio;
	GLuint								m_TextureIndex;
	BYTE*								m_LocalCopy;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	ogl_DynamicImage();
	virtual ~ogl_DynamicImage();
/////////////////////////////////////////////////

}; // End class - ogl_DynamicImage
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_DYNAMICIMAGE_H_)
