// game_LoadFacilitator.h //                       \author Logan Jones
///////////////////////////                           \date 3/28/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_LOADFACILITATOR_H_
#define _GAME_LOADFACILITATOR_H_
/////////////////////////////////////////////////////////////////////


// This structure (implemented in game_Load) provides access to methods that
// udate the user to the loading proccess. Use in Create() methos where neccessary
struct game_LoadFacilitator
{	
	int				Category;
	game_LoadFacilitator( int iCategory );
	void Progress( int iProgress );
	void Refresh();
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_LOADFACILITATOR_H_)
