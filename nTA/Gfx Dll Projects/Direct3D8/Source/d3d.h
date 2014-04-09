// d3d.h //                                Author: Logan "Burn" Jones
//////////                                           Date: 02-23-2003
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_H_
#define _D3D_H_
/////////////////////////////////////////////////////////////////////


// Base stuff
#include "../../../Source/std.h"
#include "../../../Source/gfx.h"

// Derived gfx_Interface
#include "gfx_Direct3D8.h"

// Global pointers
// NOTE: Define 'INSTANTIATE_GLOBAL_POINTERS' in some source file
//       to instantiate the global pointers
#ifdef INSTANTIATE_GLOBAL_POINTERS
	IDirect3DDevice8*			g_pd3dDevice = NULL;
	gfx_Direct3D8*				d3d = NULL;
#else
	extern IDirect3DDevice8*	g_pd3dDevice;
	extern gfx_Direct3D8*		d3d;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_H_)
