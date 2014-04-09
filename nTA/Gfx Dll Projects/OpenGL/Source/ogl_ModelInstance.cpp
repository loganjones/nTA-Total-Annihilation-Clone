// ogl_ModelInstance.cpp //                Author: Logan "Burn" Jones
//////////////////////////                           Date: 03-07-2003
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Model.h"
#include "ogl_ModelInstance.h"
#include "ogl_ModelPiece.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "ogl_ModelInstance.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_ModelInstance::ogl_ModelInstance():
	m_BaseModel(NULL),
	m_Pieces(NULL),
	//m_VertexBuffer(NULL),
	//m_VisibleIndices(NULL),
	m_RenderFlags( 0 ),
	m_ShadowOffset( 0 ),
	m_WaterLevel( 0 )
{}
ogl_ModelInstance::~ogl_ModelInstance()
{ if(m_BaseModel) Destroy();}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::Create() //          Author: Logan "Burn" Jones
////////////////////////////////                     Date: 02-22-2003
//
//===================================================================
// Parameters:
//  ogl_Model* pBase          - 
//  model_HInstance hInstance - 
//
void ogl_ModelInstance::Create( ogl_Model* pBase, model_HInstance hInstance )
{
	m_BaseModel = pBase;
	m_hInstance = hInstance;
	m_Pieces = new ogl_ModelPiece[ pBase->GetPieceCount() ];
	for( DWORD n=0; n<pBase->GetPieceCount(); ++n)
		m_Pieces[n].Model = this;
}
// End ogl_ModelInstance::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetRenderFlag() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const gfx_model_RenderFlag eFlag - 
//
void ogl_ModelInstance::SetRenderFlag( const gfx_model_RenderFlag eFlag )
{
	m_RenderFlags |= eFlag;
}
// End ogl_ModelInstance::SetRenderFlag()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::ClearRenderFlag() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const gfx_model_RenderFlag eFlag - 
//
void ogl_ModelInstance::ClearRenderFlag( const gfx_model_RenderFlag eFlag )
{
	m_RenderFlags &= ~eFlag;
}
// End ogl_ModelInstance::ClearRenderFlag()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetShadowOffset() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  float fShadowOffset - 
//
void ogl_ModelInstance::SetShadowOffset( float fShadowOffset )
{
	m_ShadowOffset = fShadowOffset;
}
// End ogl_ModelInstance::SetShadowOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetWaterLevel() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  float fWaterLevel - 
//
void ogl_ModelInstance::SetWaterLevel( float fWaterLevel )
{
	m_WaterLevel = fWaterLevel;
}
// End ogl_ModelInstance::SetWaterLevel()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetOrientation() //  Author: Logan "Burn" Jones
////////////////////////////////////////             Date: 02-26-2003
//
//===================================================================
// Parameters:
//  const std_Vector3& vOrientation - 
//
void ogl_ModelInstance::SetOrientation( const std_Vector3& vOrientation )
{
	// Setup the matrix that orients the model to the proper rotation
	m_Orientation = vOrientation * (180.0f / fPI);
}
// End ogl_ModelInstance::SetOrientation()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::Render() //          Author: Logan "Burn" Jones
////////////////////////////////                     Date: 02-22-2003
//
//===================================================================
// Parameters:
//  std_Vector3 vPosition    - 
//  std_Vector3 vOrientation - 
//
void ogl_ModelInstance::Render( std_Vector3 vPosition, std_Vector3 vOrientation ) const
{
	static GLfloat	PerspectiveMatrix[16] = {	-1,  0,0,0,
												 0,  1,0,0,
												 0,-.5,1,0,
												 0,  0,0,1 };
	static GLfloat	ShadowMatrix[16] = {	1,0,0,0,
											0,1,0,0,
											0,0,0,0,
											4,0,0,1 };

	// We need the z buffer to render the model correctly
	glEnable( GL_DEPTH_TEST );

	// Set the texture
	glBindTexture( GL_TEXTURE_2D, m_BaseModel->Texture() );

	// Move to the desired poition
	glPushMatrix();
	glTranslatef( vPosition.x, vPosition.y, vPosition.z );
	{
		// Draw the model normally
		glPushMatrix();
		{
			// Rotate to the unit's perspective and its direction
			glMultMatrixf( PerspectiveMatrix );
			glRotatef( -m_Orientation.z, 0,0,1 );

			// Do we have to worry about water coloring
			if( m_RenderFlags & model_InWater )
			{
				double Plane[] = {0,0,1,-m_WaterLevel};
				glEnable( GL_CLIP_PLANE0 );

				// Draw the part of the model that is above water
				glClipPlane( GL_CLIP_PLANE0, Plane );
				glPushMatrix();
				glRotatef( m_Orientation.x, 1,0,0 );
				glRotatef( m_Orientation.y, 0,1,0 );
				RenderPiece( m_BaseModel->Pieces(), m_Pieces, piece_Hide );

				// Draw the part of the model that is below water and
				// modulate the texture color with a watery color
				if( m_RenderFlags & model_VisibleInWater )
				{
					glPopMatrix();
					glPushMatrix();
					Plane[2] = -Plane[2]; Plane[3] = -Plane[3];
					glClipPlane( GL_CLIP_PLANE0, Plane );
					glRotatef( m_Orientation.x, 1,0,0 );
					glRotatef( m_Orientation.y, 0,1,0 );
					glColor3ub(0,0,200);
					RenderPiece( m_BaseModel->Pieces(), m_Pieces, piece_Hide );
				}

				glDisable( GL_CLIP_PLANE0 );
			}
			else // Not in water, just render
			{
				glPushMatrix();
				glRotatef( m_Orientation.x, 1,0,0 );
				glRotatef( m_Orientation.y, 0,1,0 );

				// Render the pieces
				RenderPiece( m_BaseModel->Pieces(), m_Pieces, piece_Hide );
			}

			// Draw the ground plate
			glDisable( GL_TEXTURE_2D );
			glColor3f(1,1,0);
			if( m_RenderFlags & model_Selected ) glCallList( m_BaseModel->Pieces()->DisplayList + m_BaseModel->GetPieceCount() + 1 );
			glPopMatrix();
		}
		glPopMatrix();

		// Draw the model using the shadow matrix and without the texture
		glColor4ub( 0,0,0, 100 );
		{
			// Insert the shadow matrix
			glMultMatrixf( ShadowMatrix );

			// Rotate to the unit's perspective and its orientation (again)
			glMultMatrixf( PerspectiveMatrix );
			glRotatef( -m_Orientation.z, 0,0,1 );
			glRotatef( m_Orientation.x, 1,0,0 );
			glRotatef( m_Orientation.y, 0,1,0 );

			// Render the pieces (again)
			RenderPiece( m_BaseModel->Pieces(), m_Pieces, piece_Hide | piece_DontShade );
		}
		glColor4f(1,1,1,1);

	}
	glPopMatrix();

	// Reset some stuff we changed
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
}
// End ogl_ModelInstance::Render()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::RenderPiece() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 03-07-2003
//
//===================================================================
// Parameters:
//  ogl_Model::Piece_t* pPiece     - 
//  const ogl_ModelPiece* pMetrics - 
//  DWORD dwSkipFlags              - 
//
void ogl_ModelInstance::RenderPiece( ogl_Model::Piece_t* pPiece, const ogl_ModelPiece* pMetrics, DWORD dwSkipFlags ) const
{
	static GLfloat	M[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1};

	// Iterate through each piece
	while( pPiece!=NULL )
	{
		if( dwSkipFlags & pMetrics[pPiece->Index].Flags )
		{
			if( pPiece->pChild ) RenderPiece( pPiece->pChild, pMetrics, dwSkipFlags );
		}
		else
		{
			glPushMatrix();

			// Translate and rotate
			glMultMatrixf(SetupPieceTransform(pPiece->Origin,pMetrics[pPiece->Index].Move,pMetrics[pPiece->Index].Turn_sin,pMetrics[pPiece->Index].Turn_cos,M));

			if( pPiece->Primitives )
			{
				glCallList( pPiece->DisplayList );
				//((gfx_OpenGL*)pGfxSystem)->m_TriangleCount += pPiece->Primitives;
			}
			if( pPiece->pChild ) RenderPiece( pPiece->pChild, pMetrics, dwSkipFlags );
			glPopMatrix();
		}
		pPiece = pPiece->pSibling;
	} // end while( pPiece )
}
// End ogl_ModelInstance::RenderPiece()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SynchronizeVertices() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 02-22-2003
//
//===================================================================
//
void ogl_ModelInstance::SynchronizeVertices()
{
}
// End ogl_ModelInstance::SynchronizeVertices()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SynchronizeVertices() // Author: Logan "Burn" Jones
/////////////////////////////////////////////        Date: 02-23-2003
//
//===================================================================
// Parameters:
//  ogl_Model::Piece_t* pBase -
//  D3DXMATRIX* pParent       - 
//  model_Vertex* pVerts      - 
//
//void ogl_ModelInstance::SynchronizeVertices( ogl_Model::Piece_t* pBase, D3DXMATRIX* pParent, model_Vertex* pVerts )
//{
//}
// End ogl_ModelInstance::SynchronizeVertices()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SynchronizeStates() // Author: Logan "Burn" Jones
///////////////////////////////////////////          Date: 02-22-2003
//
//===================================================================
//
void ogl_ModelInstance::SynchronizeStates()
{
}
// End ogl_ModelInstance::SynchronizeStates()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::Piece() //           Author: Logan "Burn" Jones
///////////////////////////////                      Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const long& lPiece - 
//
// Return: gfx_ModelPiece* - 
//
gfx_ModelPiece* ogl_ModelInstance::Piece( const long& lPiece )
{
	return (m_Pieces + lPiece);
}
// End ogl_ModelInstance::Piece()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetPieceOffset() //  Author: Logan "Burn" Jones
////////////////////////////////////////             Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//  float fOffset        - 
//
void ogl_ModelInstance::SetPieceOffset( const UINT32 uiPiece, const UINT32 uiAxis, float fOffset )
{
	m_Pieces[uiPiece].Move[uiAxis] = fOffset;
}
// End ogl_ModelInstance::SetPieceOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetPieceAngle() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//  float fAngle         - 
//
void ogl_ModelInstance::SetPieceAngle( const UINT32 uiPiece, const UINT32 uiAxis, float fAngle )
{
	m_Pieces[uiPiece].Turn[uiAxis] = fAngle;
	m_Pieces[uiPiece].Turn_sin[uiAxis] = sinf( fAngle * (fPI/180.0f) );
	m_Pieces[uiPiece].Turn_cos[uiAxis] = cosf( fAngle * (fPI/180.0f) );
}
// End ogl_ModelInstance::SetPieceAngle()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::PieceOffset() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 02-23-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//
// Return: float - 
//
float ogl_ModelInstance::PieceOffset( const UINT32 uiPiece, const UINT32 uiAxis ) const
{
	return m_Pieces[uiPiece].Move[uiAxis];
}
// End ogl_ModelInstance::PieceOffset()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::PieceAngle() //      Author: Logan "Burn" Jones
////////////////////////////////////                 Date: 02-23-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece - 
//  const UINT32 uiAxis  - 
//
// Return: float - 
//
float ogl_ModelInstance::PieceAngle( const UINT32 uiPiece, const UINT32 uiAxis ) const
{
	return m_Pieces[uiPiece].Turn[uiAxis];
}
// End ogl_ModelInstance::PieceAngle()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::SetPieceState() //   Author: Logan "Burn" Jones
///////////////////////////////////////              Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece         - 
//  const gfx_piece_State eState - 
//
void ogl_ModelInstance::SetPieceState( const UINT32 uiPiece, const gfx_piece_State eState )
{
	m_Pieces[uiPiece].Flags |= eState;
}
// End ogl_ModelInstance::SetPieceState()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::ClearPieceState() // Author: Logan "Burn" Jones
/////////////////////////////////////////            Date: 02-22-2003
//
//===================================================================
// Parameters:
//  const UINT32 uiPiece         - 
//  const gfx_piece_State eState - 
//
void ogl_ModelInstance::ClearPieceState(const UINT32 uiPiece, const gfx_piece_State eState)
{
	m_Pieces[uiPiece].Flags &= ~eState;
}
// End ogl_ModelInstance::ClearPieceState()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_ModelInstance::Destroy() //         Author: Logan "Burn" Jones
/////////////////////////////////                    Date: 02-22-2003
//
//===================================================================
//
void ogl_ModelInstance::Destroy()
{
	ogl_Model* pBase = m_BaseModel;

	m_BaseModel = NULL;
	SAFE_DELETE_ARRAY( m_Pieces );
//	SAFE_RELEASE( m_VertexBuffer );
//	SAFE_RELEASE( m_VisibleIndices );
	if( pBase ) pBase->ReleaseInstance( this, m_hInstance );
}
// End ogl_ModelInstance::Destroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_ModelInstance.cpp //
////////////////////////////////