// gfx_ModelPiece.h //                             \author Logan Jones
/////////////////////                                \date 02-22-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_MODELPIECE_H_
#define _GFX_MODELPIECE_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// gfx_ModelPiece ///////////////////////////
//
class gfx_ModelPiece { public:

	// Retrieve the position of this piece (in model space)
	virtual std_Vector3 Position() const = 0;

	// Retrieve the position of this piece (in model space projected onto the screen)
	virtual std_Vector2 ScreenPosition() const = 0;

}; // End class - gfx_ModelPiece
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_MODELPIECE_H_)
