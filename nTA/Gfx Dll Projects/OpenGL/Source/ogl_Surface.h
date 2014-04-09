// ogl_Surface.h //                        Author: Logan "Burn" Jones
//////////////////                                   Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_SURFACE_H_
#define _OGL_SURFACE_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// ogl_Surface ////////////////////////////
//
class ogl_Surface : public gfx_Surface
{
	friend class gfx_OpenGL;

/////////////////////////////////////////////////
// INTERFACE
public:

	void Destroy();

	const std_Size_t& GetSize() const;

	void Render();
	void Render( std_Vector3 vPos );
	void Render( std_Vector3 vPos, std_SizeF sz );
	void Render( std_RectF rctPos );


/////////////////////////////////////////////////
// Data members
protected:

	std_Size_t						m_ImageSize;
	std_Size_t						m_Tiles;
	GLuint							m_Textures[ sqr(4) ];
	GLuint							m_DisplayList;
	std_SizeF						m_Ratio;
	list< ogl_Surface >*			m_pHost;
	list< ogl_Surface >::iterator	m_HostIt;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	ogl_Surface();
	virtual ~ogl_Surface();
/////////////////////////////////////////////////

}; // End class - ogl_Surface
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_SURFACE_H_)
