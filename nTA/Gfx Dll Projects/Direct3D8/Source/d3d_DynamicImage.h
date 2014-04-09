// d3d_DynamicImage.h //                   Author: Logan "Burn" Jones
///////////////////////                               Date: 5/14/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_DYNAMICIMAGE_H_
#define _D3D_DYNAMICIMAGE_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// d3d_DynamicImage //////////////////////////
//
class d3d_DynamicImage : public gfx_DynamicImage
{
	friend gfx_Direct3D8;

/////////////////////////////////////////////////
// INTERFACE
public:

	void Destroy();

	const std_Size& GetSize() const;

	void Render( std_Vector3 vPosition );
	void Render( std_Vector3 vPosition, std_SizeF sz );

	void GetBits( BYTE** ppImage );
	void Update( BYTE** ppImage );


/////////////////////////////////////////////////
// Data members
public:

	list<d3d_DynamicImage>::iterator	m_Index;
	gfx_Direct3D8*						m_pInterface;
	std_Size							m_ImageSize;
	std_Size							m_TextureSize;
	std_SizeF							m_SizeRatio;
	LPDIRECT3DTEXTURE8					m_SystemMemTexture;
	LPDIRECT3DTEXTURE8					m_VideoMemTexture;
	D3DLOCKED_RECT						m_LockedRect;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	d3d_DynamicImage();
	virtual ~d3d_DynamicImage();
/////////////////////////////////////////////////

}; // End class - d3d_DynamicImage
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_DYNAMICIMAGE_H_)
