// gfx.h //                                        \author Logan Jones
//////////                                            \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_H_
#define _GFX_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "sys.h"

// Define some stuff
#include "gfx_Defs.h"

// Specialized interfaces
#include "gfx_Surface.h"
#include "gfx_Font.h"
#include "gfx_Cursor.h"
#include "gfx_Terrain.h"
#include "gfx_Sprite.h"
#include "gfx_Model.h"
#include "gfx_ModelInstance.h"
#include "gfx_ModelPiece.h"
#include "gfx_DynamicImage.h"

// Primary gfx interface
#include "gfx_Interface.h"

// Global gfx_Interface pointer
// NOTE: Define 'INSTANTIATE_GFX_INTERFACE_POINTER' in some source file
//       to instantiate the global gfx interface pointer
#ifdef INSTANTIATE_GFX_INTERFACE_POINTER
	gfx_Interface*			gfx = NULL;
#else
	extern gfx_Interface*	gfx;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_H_)
