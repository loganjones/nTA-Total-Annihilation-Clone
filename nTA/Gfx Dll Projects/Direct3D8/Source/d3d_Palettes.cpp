// d3d_Palettes.cpp //                     Author: Logan "Burn" Jones
/////////////////////                                 Date: 10/8/2001
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "gfx_Direct3D8.h"


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreatePalette() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  PALETTEENTRY* pPaletteEntries - 
//  LPPALETTE* ppPalette          - 
//  BOOL bSetAsCurrent            - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreatePalette( PALETTEENTRY* pPaletteEntries, LPPALETTE* ppPalette, BOOL bSetAsCurrent )
{
	Palette_t		newPal;

	// Copy the entries and the new index
	memcpy( newPal.Entries, pPaletteEntries, sizeof(PALETTEENTRY) * 256 );
	newPal.Index = m_NumPalettes;

	// Copy the entries to the D3D palette index
	if( FAILED(m_pD3DDevice->SetPaletteEntries(
		m_NumPalettes,
		newPal.Entries )) )
	{
		ErrorComment( "Failed to set palette entries." );
		return FALSE;
	}

	// Add the structure to the list
	m_Palettes.push_front( newPal );
	(*ppPalette) = &m_Palettes.front();
	m_NumPalettes++;

	// Set the new palette as the current one if desired
	if( bSetAsCurrent )
		return SetCurrentPalette( *ppPalette );

	return TRUE;
}
// End gfx_Direct3D8::CreatePalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::SetCurrentPalette() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE pPalette - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::SetCurrentPalette( LPPALETTE pPalette )
{
	// Set the desired palette
	if( FAILED(m_pD3DDevice->SetCurrentTexturePalette( static_cast<Palette_t*>(pPalette)->Index )) )
	{
		ErrorComment( "Failed to set the texture palette." );
		return FALSE;
	}

	// Record the new current palette
	m_pCurrentPalette = pPalette;

	return TRUE;
}
// End gfx_Direct3D8::SetCurrentPalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::GetCurrentPalette() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE* ppPalette - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::GetCurrentPalette( LPPALETTE* ppPalette )
{
	*ppPalette = m_pCurrentPalette;

	return TRUE;
}
// End gfx_Direct3D8::GetCurrentPalette()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DestroyPalette() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPPALETTE* pPalette - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::DestroyPalette( LPPALETTE* pPalette )
{
//	m_Palettes.remove( *static_cast<Palette_t*>(*pPalette) );
	*pPalette = NULL;

	return TRUE;
}
// End gfx_Direct3D8::DestroyPalette()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Palettes.cpp //
///////////////////////////
