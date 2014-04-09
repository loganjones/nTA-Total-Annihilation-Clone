// d3d_Utility.h //                        Author: Logan "Burn" Jones
//////////////////                                   Date: 10/10/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_UTILITY_H_
#define _D3D_UTILITY_H_
/////////////////////////////////////////////////////////////////////


#define WORLD_MATRIX       ( m_WorldMatrix[ m_WorldMatrixIndex ] )
#define MULT_WORLD_MATRIX( IN_MATRIX ) \
    D3DXMatrixMultiply( &WORLD_MATRIX, &IN_MATRIX, &WORLD_MATRIX );

#define dxSetWorldMatrix() \
{ \
    m_pD3DDevice->SetTransform( D3DTS_WORLD, &WORLD_MATRIX ); \
}

#define dxLoadIdentity() \
{ \
    D3DXMatrixIdentity( &WORLD_MATRIX ); \
}

#define dxPushMatrix() \
{ \
    assert( (m_WorldMatrixIndex<32)&&("World Matrix Overflow") ); \
    m_WorldMatrix[ m_WorldMatrixIndex+1 ] = WORLD_MATRIX; \
	m_WorldMatrixIndex++; \
}

#define dxPopMatrix() \
{ \
    assert( (m_WorldMatrixIndex>=0)&&("World Matrix Underflow") ); \
    m_WorldMatrixIndex--; \
}

#define dxTranslatef( X_VALUE, Y_VALUE, Z_VALUE ) \
{ \
    D3DXMATRIX matOp; \
    D3DXMatrixTranslation( &matOp, X_VALUE, Y_VALUE, Z_VALUE ); \
    MULT_WORLD_MATRIX( matOp ); \
}

#define dxRotateX( ROTATION_VALUE ) \
{ \
    D3DXMATRIX matOp; \
    D3DXMatrixRotationX( &matOp, D3DXToRadian(ROTATION_VALUE) ); \
    MULT_WORLD_MATRIX( matOp ); \
}

#define dxRotateY( ROTATION_VALUE ) \
{ \
    D3DXMATRIX matOp; \
    D3DXMatrixRotationY( &matOp, D3DXToRadian(ROTATION_VALUE) ); \
    MULT_WORLD_MATRIX( matOp ); \
}

#define dxRotateZ( ROTATION_VALUE ) \
{ \
    D3DXMATRIX matOp; \
    D3DXMatrixRotationZ( &matOp, D3DXToRadian(ROTATION_VALUE) ); \
    MULT_WORLD_MATRIX( matOp ); \
}


/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_UTILITY_H_)
