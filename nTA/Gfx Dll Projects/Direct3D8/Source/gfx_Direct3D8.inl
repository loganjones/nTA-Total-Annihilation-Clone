// gfx_Direct3D8.inl //                    Author: Logan "Burn" Jones
//////////////////////                                 Date: 2/8/2002
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::EnumDisplayMode() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 8/8/2002
//               
//====================================================================
// Parameters:
//  LPDIRECT3D8 pD3D      - 
//  DWORD dwIndex         - 
//  std_Size* pResolution - 
//  long* pDepth          - 
//
// Return: DWORD - 
//
DWORD gfx_Direct3D8::EnumDisplayMode( LPDIRECT3D8 pD3D, DWORD dwIndex, std_Size* pResolution, long* pDepth )
{
	D3DDISPLAYMODE		Mode;
	std_Size			LastRes = *pResolution;
	long				LastDepth = *pDepth;
	long				Depth;

	// Sanity check
	if( pD3D==NULL ) return 0;

	// Start enumerating
	do {
		// Get this enumeration and return 0 if it fails
		if( pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,dwIndex++,&Mode)==D3DERR_INVALIDCALL )
			return 0;

		//  Get the depth
		switch( Mode.Format )
		{
			case D3DFMT_X8R8G8B8: Depth = 32; break;
			case D3DFMT_R5G6B5:
			case D3DFMT_X1R5G5B5: Depth = 16; break;
			default: continue;
		}
	} while( (LastRes.width==Mode.Width && LastRes.height==Mode.Height && LastDepth==Depth) );

	// Set the return data and return
	pResolution->Set( Mode.Width, Mode.Height );
	*pDepth = Depth;
	return dwIndex;
}
// End gfx_Direct3D8::EnumDisplayMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxSetWorldMatrix() //     Author: Logan "Burn" Jones
//////////////////////////////////////                  Date: 2/8/2002
//               
//====================================================================
//
void gfx_Direct3D8::dxSetWorldMatrix()
{
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxSetWorldMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxLoadIdentity() //       Author: Logan "Burn" Jones
////////////////////////////////////                    Date: 2/8/2002
//               
//====================================================================
//
void gfx_Direct3D8::dxLoadIdentity()
{
	D3DXMatrixIdentity( &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxLoadIdentity()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxPushMatrix() //         Author: Logan "Burn" Jones
//////////////////////////////////                      Date: 2/8/2002
//               
//====================================================================
//
void gfx_Direct3D8::dxPushMatrix()
{
	assert( (m_WorldMatrixIndex<WORLD_MATRIX_STACK_DEPTH)&&("World Matrix Overflow") );
    m_WorldMatrix[ m_WorldMatrixIndex+1 ] = m_WorldMatrix[ m_WorldMatrixIndex ];
	m_WorldMatrixIndex++;
}
// End gfx_Direct3D8::dxPushMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxPopMatrix() //          Author: Logan "Burn" Jones
/////////////////////////////////                       Date: 2/8/2002
//               
//====================================================================
//
void gfx_Direct3D8::dxPopMatrix()
{
	assert( (m_WorldMatrixIndex>=0)&&("World Matrix Underflow") );
    m_WorldMatrixIndex--;
}
// End gfx_Direct3D8::dxPopMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxMultMatrix() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 3/24/2002
//               
//====================================================================
// Parameters:
//  const D3DXMATRIX& matrix - 
//
void gfx_Direct3D8::dxMultMatrix( const D3DXMATRIX& matrix )
{
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matrix, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxMultMatrix()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxTranslatef() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 3/24/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vTranslation - 
//
void gfx_Direct3D8::dxTranslatef( std_Vector3 vTranslation )
{
	D3DXMATRIX	matOp;
    D3DXMatrixTranslation( &matOp, vTranslation.x, vTranslation.y, vTranslation.z );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxTranslatef()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxTranslatef() //         Author: Logan "Burn" Jones
//////////////////////////////////                      Date: 2/8/2002
//               
//====================================================================
// Parameters:
//  float fX - 
//  float fY - 
//  float fZ - 
//
void gfx_Direct3D8::dxTranslatef( float fX, float fY, float fZ )
{
	D3DXMATRIX	matOp;
    D3DXMatrixTranslation( &matOp, fX, fY, fZ );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxTranslatef()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxRotatef() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/8/2002
//               
//====================================================================
// Parameters:
//  float fAngle - 
//  float fX     - 
//  float fY     - 
//  float fZ     - 
//
void gfx_Direct3D8::dxRotatef( float fAngle, float fX, float fY, float fZ )
{
	D3DXVECTOR3	vector( fX, fY, fZ );
	D3DXMATRIX	matOp;
	D3DXMatrixRotationAxis( &matOp, &vector, D3DXToRadian(fX) );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxRotatef()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxRotateX() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/8/2002
//               
//====================================================================
// Parameters:
//  float fX - 
//
void gfx_Direct3D8::dxRotateX( float fX )
{
	D3DXMATRIX	matOp;
    D3DXMatrixRotationX( &matOp, D3DXToRadian(fX) );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxRotateX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxRotateY() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/8/2002
//               
//====================================================================
// Parameters:
//  float fY - 
//
void gfx_Direct3D8::dxRotateY( float fY )
{
	D3DXMATRIX	matOp;
    D3DXMatrixRotationY( &matOp, D3DXToRadian(fY) );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxRotateY()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxRotateZ() //            Author: Logan "Burn" Jones
///////////////////////////////                         Date: 2/8/2002
//               
//====================================================================
// Parameters:
//  float fZ - 
//
void gfx_Direct3D8::dxRotateZ( float fZ )
{
	D3DXMATRIX	matOp;
    D3DXMatrixRotationZ( &matOp, D3DXToRadian(fZ) );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxRotateZ()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::dxRotate() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 3/25/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vRotate - 
//
void gfx_Direct3D8::dxRotate( std_Vector3 vRotate )
{
	D3DXMATRIX	matOp;
    D3DXMatrixRotationYawPitchRoll( &matOp, D3DXToRadian(vRotate.z), D3DXToRadian(vRotate.x), D3DXToRadian(vRotate.y) );
	D3DXMatrixMultiply( &m_WorldMatrix[ m_WorldMatrixIndex ], &matOp, &m_WorldMatrix[ m_WorldMatrixIndex ] );
}
// End gfx_Direct3D8::dxRotate()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_Direct3D8.inl //
////////////////////////////
