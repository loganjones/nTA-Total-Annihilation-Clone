// d3d_VertexArray.cpp //                  Author: Logan "Burn" Jones
////////////////////////                              Date: 10/7/2001
//
/////////////////////////////////////////////////////////////////////
/*
#include "d3d.h"
#include "gfx_Direct3D8.h"


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::VA_Create() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 10/7/2001
//               
//====================================================================
// Parameters:
//  DWORD dwNumVerts             - 
//  VA_VERTEX_TYPES enumType     - 
//  DWORD dwUsage                - 
//  LPVERTEXARRAY* ppVertexArray - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::VA_Create( DWORD dwNumVerts, VA_VERTEX_TYPES enumType, DWORD dwUsage, LPVERTEXARRAY* ppVertexArray )
{
	LRESULT						lResult;
	DWORD						VertexSize;
	DWORD						FVF;
	LPDIRECT3DVERTEXBUFFER8		lpVB;

	// What type is desired
	switch( enumType )
	{
		case VA_GUI_VERTEX:
			VertexSize = (sizeof(float) * 3) + sizeof(UINT32) + sizeof(float) + sizeof(float);
			FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
			break;

		default: // Unknown type
			ErrorComment( "ERROR: Unknown vertex type \'%d\'", enumType );
			return FALSE;

	} // end switch( enumType )

	if( FAILED(lResult=m_pD3DDevice->CreateVertexBuffer(
		dwNumVerts * VertexSize,
		0,
		FVF,
		D3DPOOL_MANAGED,
		&lpVB )) )
	{
		ErrorComment( "ERROR: Failed to create vertex buffer, %s %d.", GetErrorString(lResult), lResult );
		return FALSE;
	}

	return TRUE;
}
// End gfx_Direct3D8::VA_Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::VA_Lock() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 10/7/2001
//               
//====================================================================
// Parameters:
//  LPVERTEXARRAY lpVertexArray - 
//  DWORD dwOffset              - 
//  DWORD dwSize                - 
//  LPVOID* ppVertData          - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::VA_Lock( LPVERTEXARRAY lpVertexArray, DWORD dwOffset, DWORD dwSize, LPVOID* ppVertData )
{
	return FALSE;
}
// End gfx_Direct3D8::VA_Lock()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::VA_Unlock() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 10/7/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_Direct3D8::VA_Unlock()
{
	return FALSE;
}
// End gfx_Direct3D8::VA_Unlock()
//////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// End - d3d_VertexArray.cpp //
//////////////////////////////
