// d3d_ModelPiece.h //                     Author: Logan "Burn" Jones
/////////////////////                                Date: 02-22-2003
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_MODELPIECE_H_
#define _D3D_MODELPIECE_H_
/////////////////////////////////////////////////////////////////////

class d3d_ModelInstance;
////////////////////////// d3d_ModelPiece ///////////////////////////
//
class d3d_ModelPiece : public gfx_ModelPiece
{

/////////////////////////////////////////////////
// INTERFACE
public:

	std_Vector3 Position() const;

	// Retrieve the position of this piece (in model space projected onto the screen)
	std_Vector2 ScreenPosition() const;


/////////////////////////////////////////////////
// Data members
public:
	d3d_ModelInstance*	Model;

	std_Vector3			Move;
	std_Vector3			Turn;
	std_Vector3			Turn_sin;
	std_Vector3			Turn_cos;
	DWORD				Flags;

	D3DXVECTOR4			Origin;


/////////////////////////////////////////////////
// Default Constructor
public:
	d3d_ModelPiece():Move(0,0,0),Turn(0,0,0),Turn_sin(0,0,0),Turn_cos(1,1,1),Flags(0){}
/////////////////////////////////////////////////

}; // End class - d3d_ModelPiece
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_MODELPIECE_H_)
