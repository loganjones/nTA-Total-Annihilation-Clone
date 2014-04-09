// d3d_ModelInstance.cpp //                Author: Logan "Burn" Jones
//////////////////////////                           Date: 02-22-2003
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Model.h"
#include "d3d_ModelInstance.h"
#include "d3d_ModelPiece.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "d3d_ModelInstance.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
d3d_ModelInstance::d3d_ModelInstance():
	m_BaseModel(NULL),
	m_Pieces(NULL),
	m_VertexBuffer(NULL),
	m_VisibleIndices(NULL),
	m_RenderFlags( 0 ),
	m_ShadowOffset( 0 ),
	m_WaterLevel( 0 )
{}
d3d_ModelInstance::~d3d_ModelInstance()
{ if(m_BaseModel) Destroy();}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::Create() //          Author: Logan "Burn" Jones
////////////////////////////////                     Date: 02-22-2003
//
//===================================================================
// Parameters:
//  d3d_Model* pBase          - 
//  model_HInstance hInstance - 
//
void d3d_ModelInstance::Create( d3d_Model* pBase, model_HInstance hInstance )
{
	m_BaseModel = pBase;
	m_hInstance = hInstance;
	m_Pieces = new d3d_ModelPiece[ pBase->GetPieceCount() ];
	for( DWORD n=0; n<pBase->GetPieceCount(); ++n)
		m_Pieces[n].Model = this;

	// Create the vertex buffer
	g_pd3dDevice->CreateVertexBuffer(
		(pBase->VertexCount()/* + 5*/) * sizeof(model_Vertex),
		D3DUSAGE_WRITEONLY,
		D3DFVF_MODEL_VERTEX,
		D3DPOOL_MANAGED,
		&m_VertexBuffer );

	// Create the index buffer
	g_pd3dDevice->CreateIndexBuffer(
		((pBase->TriangleCount() * 3) + pBase->GoundPlateSize()) * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_VisibleIndices );
}
// End d3d_ModelInstance::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetRenderFlag() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const gfx_model_RenderFlag eFlag - 
//
void d3d_ModelInstance::SetRenderFlag( const gfx_model_RenderFlag eFlag )
{
	m_RenderFlags |= eFlag;
}
// End d3d_ModelInstance::SetRenderFlag()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::ClearRenderFlag() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const gfx_model_RenderFlag eFlag - 
//
void d3d_ModelInstance::ClearRenderFlag( const gfx_model_RenderFlag eFlag )
{
	m_RenderFlags &= ~eFlag;
}
// End d3d_ModelInstance::ClearRenderFlag()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetShadowOffset() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  float fShadowOffset - 
//
void d3d_ModelInstance::SetShadowOffset( float fShadowOffset )
{
	m_ShadowOffset = fShadowOffset;
}
// End d3d_ModelInstance::SetShadowOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetWaterLevel() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  float fWaterLevel - 
//
void d3d_ModelInstance::SetWaterLevel( float fWaterLevel )
{
	m_WaterLevel = fWaterLevel;
}
// End d3d_ModelInstance::SetWaterLevel()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetOrientation() //  Author: Logan "Burn" Jones
////////////////////////////////////////             Date: 02-26-2003
//
//===================================================================
// Parameters:
//  const std_Vector3& vOrientation - 
//
void d3d_ModelInstance::SetOrientation( const std_Vector3& vOrientation )
{
	// Setup the matrix that orients the model to the proper rotation
	D3DXMatrixRotationYawPitchRoll( &m_Orientation, vOrientation.y, vOrientation.x, -vOrientation.z );
}
// End d3d_ModelInstance::SetOrientation()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::Render() //          Author: Logan "Burn" Jones
////////////////////////////////                     Date: 02-22-2003
//
//===================================================================
// Parameters:
//  std_Vector3 vPosition    - 
//  std_Vector3 vOrientation - 
//
void d3d_ModelInstance::Render( std_Vector3 vPosition, std_Vector3 vOrientation ) const
{
	static D3DXMATRIX			PrePerspective;
	static const D3DXMATRIX		Perspective( -1,  0, 0, 0,
											  0,  1, 0, 0,
											  0,-.5, 1, 0,
											  0,  0, 0, 1 );
	static const D3DXMATRIX		Shadow( 1,0,0,0,
										0,1,0,0,
										0,0,0,0,
										5,0,0,1 );
	static D3DXPLANE			WaterPlane(0,0,1,0);
	static D3DXPLANE			TransformedWaterPlane;

	d3d->FlushSpriteBuffer();

	// Enable the depth buffer so the pieces will ne drawn properly
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	// Set the texture
	g_pd3dDevice->SetTexture( 0, m_BaseModel->Texture() );

	// Only the first color argument is neccessary
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

	// Set the stream
	g_pd3dDevice->SetStreamSource( 0, m_VertexBuffer, sizeof(model_Vertex) );
	g_pd3dDevice->SetVertexShader( D3DFVF_MODEL_VERTEX );
	g_pd3dDevice->SetIndices( m_VisibleIndices, 0 );

	// Move to the desired poition
	d3d->dxPushMatrix();
	d3d->dxTranslatef( vPosition );

	// Rotate to the unit's perspective and its orientation
	d3d->dxPushMatrix();
	PrePerspective = d3d->m_WorldMatrix[ d3d->m_WorldMatrixIndex ];
	d3d->dxMultMatrix( Perspective );
	d3d->dxMultMatrix( m_Orientation );
	d3d->dxSetWorldMatrix();

	// Do we have to worry about water coloring
	if( (m_RenderFlags & model_InWater) && (m_RenderFlags & model_VisibleInWater) )
	{
		// Setup the water level plane
		WaterPlane.d = -m_WaterLevel;
		D3DXPlaneTransform( &TransformedWaterPlane, &WaterPlane, &PrePerspective );
		g_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0 );
		g_pd3dDevice->SetClipPlane( 0, -TransformedWaterPlane );

		// Draw the submerged model
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_SPECULAR );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TEXTURE );
		g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_BaseModel->VertexCount(), m_BaseModel->GoundPlateSize(), m_VisiblePrimitives );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );

		// Update the tri count and inverse the clip plane (to draw stuff above water)
//		d3d->m_TriangleCount += m_BaseModel->TriangleCount();
		g_pd3dDevice->SetClipPlane( 0, TransformedWaterPlane );
	} // end if( render under-water portion )

	// Draw the model normally
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_BaseModel->VertexCount(), m_BaseModel->GoundPlateSize(), m_VisiblePrimitives );
	g_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, 0 );

	// Draw the ground plate if we have to
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );
	g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );
	if( m_RenderFlags & model_Selected )
		g_pd3dDevice->DrawIndexedPrimitive( D3DPT_LINESTRIP, m_BaseModel->GoundPlateVertex(), m_BaseModel->GoundPlateSize(), 0, m_BaseModel->GoundPlateSize()-1 );

	// Insert the shadow matrix and rotate to the unit's perspective and its orientation (again)
	d3d->dxPopMatrix();
	d3d->dxTranslatef( 0, m_ShadowOffset, 0 );
	d3d->dxMultMatrix( Shadow );
	d3d->dxMultMatrix( Perspective );
	d3d->dxMultMatrix( m_Orientation );
	d3d->dxSetWorldMatrix();

	// Draw the shadow model
	g_pd3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, m_BaseModel->VertexCount(), m_BaseModel->GoundPlateSize(), m_VisiblePrimitives );

	// Update the tri count
//	d3d->m_TriangleCount += m_BaseModel->TriangleCount() * 2;

	// Reset some stuff we changed
	d3d->dxPopMatrix();
	g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
}
// End d3d_ModelInstance::Render()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SynchronizeVertices() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 02-22-2003
//
//===================================================================
//
void d3d_ModelInstance::SynchronizeVertices()
{
	D3DXMATRIX				Start;
	model_Vertex*			pVerts;

	m_VertexBuffer->Lock( 0, m_BaseModel->VertexCount() * sizeof(model_Vertex), (BYTE**)&pVerts, 0 );
	SynchronizeVertices( m_BaseModel->Pieces(), D3DXMatrixIdentity(&Start), pVerts );
	m_VertexBuffer->Unlock();
}
// End d3d_ModelInstance::SynchronizeVertices()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SynchronizeVertices() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 02-23-2003
//
//===================================================================
// Parameters:
//  d3d_Model::Piece_t* pBase -
//  D3DXMATRIX* pParent       - 
//  model_Vertex* pVerts      - 
//
void d3d_ModelInstance::SynchronizeVertices( d3d_Model::Piece_t* pBase, D3DXMATRIX* pParent, model_Vertex* pVerts )
{
	D3DXMATRIX			M, Transform;
	static D3DXVECTOR4	TmpVert;
	d3d_ModelPiece*		pPiece;

	for( ; pBase; pBase=pBase->pSibling )
	{
		pPiece = m_Pieces + pBase->Index;
		SetupPieceTransform( pBase->Origin, pPiece->Move, pPiece->Turn_sin, pPiece->Turn_cos, M );
		D3DXMatrixMultiply( &Transform, &M, pParent );

		//pPiece->Origin.Set( M._41, M._42, M._43 );
		//pPiece->Origin.x=M._41; pPiece->Origin.y=M._42; pPiece->Origin.z=M._43;
		TmpVert.x=TmpVert.y=TmpVert.z=0;TmpVert.w=1;
		D3DXVec4Transform( &pPiece->Origin, &TmpVert, &Transform );

		for( DWORD n=pBase->VertexOffset; n<pBase->VertexOffset+pBase->VertexCount; ++n)
		{
			model_Vertex& Raw = *(m_BaseModel->Vertices() + n);
			D3DXVec3Transform( &TmpVert, &Raw.position, &Transform );
			pVerts[n].position.x = TmpVert.x;
			pVerts[n].position.y = TmpVert.y;
			pVerts[n].position.z = TmpVert.z;
			pVerts[n].diffuse = Raw.diffuse;
			pVerts[n].specular= Raw.specular;
			pVerts[n].tu_0 = Raw.tu_0;
			pVerts[n].tv_0 = Raw.tv_0;
		}

		SynchronizeVertices( pBase->pChild, &Transform, pVerts );
	}
}
// End d3d_ModelInstance::SynchronizeVertices()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SynchronizeStates() // Author: Logan "Burn" Jones
///////////////////////////////////////////          Date: 02-22-2003
//
//===================================================================
//
void d3d_ModelInstance::SynchronizeStates()
{
	WORD*				pIndices;
	d3d_Model::Piece_t*	pPiece = m_BaseModel->Pieces();
	d3d_ModelPiece*		pMetric = m_Pieces;
	const DWORD			count = m_BaseModel->GetPieceCount();
	DWORD				n, Offset;

	m_VisibleIndices->Lock(0,0,(BYTE**)&pIndices,0);
	m_VisiblePrimitives = 0;

	memcpy( pIndices, m_BaseModel->Indices() + m_BaseModel->GoundPlateIndex(), m_BaseModel->GoundPlateSize() * sizeof(WORD) );
	Offset = m_BaseModel->GoundPlateSize();

	for( n=0; n<count; ++n,++pPiece,++pMetric)
	{
		if( pMetric->Flags & piece_Hide ) continue;
		else
		{
			memcpy( pIndices + Offset, m_BaseModel->Indices() + pPiece->IndexOffset, pPiece->IndexCount * sizeof(WORD) );
			Offset += pPiece->IndexCount;
			m_VisiblePrimitives += pPiece->IndexCount;
		}
	}
	m_VisiblePrimitives /= 3;
	m_VisibleIndices->Unlock();
}
// End d3d_ModelInstance::SynchronizeStates()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::Piece() //           Author: Logan "Burn" Jones
///////////////////////////////                      Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const long& lPiece - 
//
// Return: gfx_ModelPiece* - 
//
gfx_ModelPiece* d3d_ModelInstance::Piece( const long& lPiece )
{
	return (m_Pieces + lPiece);
}
// End d3d_ModelInstance::Piece()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetPieceOffset() //  Author: Logan "Burn" Jones
////////////////////////////////////////             Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//  float fOffset        - 
//
void d3d_ModelInstance::SetPieceOffset( const UINT32 uiPiece, const UINT32 uiAxis, float fOffset )
{
	m_Pieces[uiPiece].Move[uiAxis] = fOffset;
}
// End d3d_ModelInstance::SetPieceOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetPieceAngle() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//  float fAngle         - 
//
void d3d_ModelInstance::SetPieceAngle( const UINT32 uiPiece, const UINT32 uiAxis, float fAngle )
{
	m_Pieces[uiPiece].Turn[uiAxis] = fAngle;
	m_Pieces[uiPiece].Turn_sin[uiAxis] = sinf( fAngle * (fPI/180.0f) );
	m_Pieces[uiPiece].Turn_cos[uiAxis] = cosf( fAngle * (fPI/180.0f) );
}
// End d3d_ModelInstance::SetPieceAngle()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::PieceOffset() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 02-23-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//
// Return: float - 
//
float d3d_ModelInstance::PieceOffset( const UINT32 uiPiece, const UINT32 uiAxis ) const
{
	return m_Pieces[uiPiece].Move[uiAxis];
}
// End d3d_ModelInstance::PieceOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::PieceAngle() //      Author: Logan "Burn" Jones
////////////////////////////////////                 Date: 02-23-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//
// Return: float - 
//
float d3d_ModelInstance::PieceAngle( const UINT32 uiPiece, const UINT32 uiAxis ) const
{
	return m_Pieces[uiPiece].Turn[uiAxis];
}
// End d3d_ModelInstance::PieceAngle()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::SetPieceState() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece         - 
//  const gfx_piece_State eState - 
//
void d3d_ModelInstance::SetPieceState( const UINT32 uiPiece, const gfx_piece_State eState )
{
	m_Pieces[uiPiece].Flags |= eState;
}
// End d3d_ModelInstance::SetPieceState()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::ClearPieceState() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece         - 
//  const gfx_piece_State eState - 
//
void d3d_ModelInstance::ClearPieceState(const UINT32 uiPiece, const gfx_piece_State eState)
{
	m_Pieces[uiPiece].Flags &= ~eState;
}
// End d3d_ModelInstance::ClearPieceState()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_ModelInstance::Destroy() //         Author: Logan "Burn" Jones
/////////////////////////////////                    Date: 02-22-2003
//
//===================================================================
//
void d3d_ModelInstance::Destroy()
{
	d3d_Model* pBase = m_BaseModel;

	m_BaseModel = NULL;
	SAFE_DELETE_ARRAY( m_Pieces );
	SAFE_RELEASE( m_VertexBuffer );
	SAFE_RELEASE( m_VisibleIndices );
	if( pBase ) pBase->ReleaseInstance( this, m_hInstance );
}
// End d3d_ModelInstance::Destroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_ModelInstance.cpp //
////////////////////////////////