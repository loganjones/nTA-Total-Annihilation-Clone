// ogl_ModelPiece.h //                     Author: Logan "Burn" Jones
/////////////////////                                Date: 03-07-2003
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_MODELPIECE_H_
#define _OGL_MODELPIECE_H_
/////////////////////////////////////////////////////////////////////

class ogl_ModelInstance;
////////////////////////// ogl_ModelPiece ///////////////////////////
//
class ogl_ModelPiece : public gfx_ModelPiece
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// Retrieve the position of this piece (in model space)
	std_Vector3 Position() const;

	// Retrieve the position of this piece (in model space projected onto the screen)
	std_Vector2 ScreenPosition() const;


/////////////////////////////////////////////////
// Data members
public:
	ogl_ModelInstance*	Model;

	std_Vector3			Move;
	std_Vector3			Turn;
	std_Vector3			Turn_sin;
	std_Vector3			Turn_cos;
	DWORD				Flags;

	//D3DXVECTOR4			Origin;


/////////////////////////////////////////////////
// Default Constructor
public:
	ogl_ModelPiece():Move(0,0,0),Turn(0,0,0),Turn_sin(0,0,0),Turn_cos(1,1,1),Flags(0){}
/////////////////////////////////////////////////

}; // End class - ogl_ModelPiece
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_MODELPIECE_H_)
