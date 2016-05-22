// ogl_Palettes.cpp //                     Author: Logan "Burn" Jones
/////////////////////                                 Date: 10/8/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "gfx_OpenGL.h"


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreatePalette() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  PALETTEENTRY* pPaletteEntries - 
//  LPPALETTE* ppPalette          - 
//  BOOL bSetAsCurrent            - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreatePalette( PALETTEENTRY* pPaletteEntries, LPPALETTE* ppPalette, BOOL bSetAsCurrent )
{
	Palette_t		newPal;

	// Copy the palette to the structure
	memcpy( newPal.Entries, pPaletteEntries, 256 * sizeof(PALETTEENTRY) );

	// Add the structure to the list
	m_Palettes.push_front( newPal );
	(*ppPalette) = &m_Palettes.front();

	// Set the new palette as the current one if desired
	if( bSetAsCurrent )
		return SetCurrentPalette( *ppPalette );

	return TRUE;
}
// End gfx_OpenGL::CreatePalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::SetCurrentPalette() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE pPalette - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::SetCurrentPalette( LPPALETTE pPalette )
{
	// Set it as the shared palette
	if (glColorTableEXT)
	{
		glColorTableEXT(
			GL_SHARED_TEXTURE_PALETTE_EXT,
			GL_RGBA8,
			256,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			static_cast<Palette_t*>(pPalette)->Entries );
		glEnable( GL_SHARED_TEXTURE_PALETTE_EXT );
	}

	// Record the new current palette
	m_pCurrentPalette = pPalette;

	return TRUE;
}
// End gfx_OpenGL::SetCurrentPalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::GetCurrentPalette() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE* ppPalette - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::GetCurrentPalette( LPPALETTE* ppPalette )
{
	*ppPalette = m_pCurrentPalette;

	return TRUE;
}
// End gfx_OpenGL::GetCurrentPalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DestroyPalette() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE* pPalette - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::DestroyPalette( LPPALETTE* pPalette )
{
//	m_Palettes.erase( *(static_cast<PaletteList_t::iterator*>(*pPalette)) );
	*pPalette = NULL;

	return TRUE;
}
// End gfx_OpenGL::DestroyPalette()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Palettes.cpp //
///////////////////////////
