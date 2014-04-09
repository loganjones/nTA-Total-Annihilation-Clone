// d3d_Surface.h //                        Author: Logan "Burn" Jones
//////////////////                                   Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_SURFACE_H_
#define _D3D_SURFACE_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// d3d_Surface ////////////////////////////
//
class d3d_Surface : public gfx_Surface
{
	friend class gfx_Direct3D8;

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
	LPDIRECT3DTEXTURE8				m_Textures[ sqr(4) ];
	Surface_Vert*					m_Verts;
	std_SizeF						m_Ratio;
	list< d3d_Surface >*			m_pHost;
	list< d3d_Surface >::iterator	m_HostIt;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	d3d_Surface();
	virtual ~d3d_Surface();
/////////////////////////////////////////////////

}; // End class - d3d_Surface
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_SURFACE_H_)
