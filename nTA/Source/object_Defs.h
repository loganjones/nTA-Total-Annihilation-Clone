// object_Defs.h //                                \author Logan Jones
//////////////////                                   \date 12/29/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _OBJECT_DEFS_H_
#define _OBJECT_DEFS_H_
/////////////////////////////////////////////////////////////////////


#define SQRT_OF_2			( 1.4142135623730950488016887242097f )
#define GRID_PRINT(X,Z)		((long)( SQRT_OF_2 * (((X)>(Z)) ? X:Z) + 0.5f ))

enum object_Type_t {
	OBJ_Unit,
	OBJ_Feature,
	OBJ_Weapon,
	OBJ_SFX,
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OBJECT_DEFS_H_)
