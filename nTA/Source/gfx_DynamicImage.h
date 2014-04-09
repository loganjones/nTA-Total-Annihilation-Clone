// gfx_DynamicImage.h //                           \author Logan Jones
///////////////////////                               \date 5/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_DYNAMICIMAGE_H_
#define _GFX_DYNAMICIMAGE_H_
/////////////////////////////////////////////////////////////////////


///////////////////////// gfx_DynamicImage //////////////////////////
//
class gfx_DynamicImage
{

/////////////////////////////////////////////////////////////////////
// Virtual Interface
public:

	virtual void Destroy() = 0;

	virtual const std_Size& GetSize() const = 0;

	virtual void Render( std_Vector3 vPosition ) = 0;
	virtual void Render( std_Vector3 vPosition, std_SizeF sz ) = 0;

	virtual void GetBits( BYTE** ppImage ) = 0;
	virtual void Update( BYTE** ppImage ) = 0;

}; // End class - gfx_DynamicImage
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_DYNAMICIMAGE_H_)
