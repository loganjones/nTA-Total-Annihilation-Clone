// d3d_ModelInstance.h //                  Author: Logan "Burn" Jones
////////////////////////                             Date: 02-22-2003
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_MODELINSTANCE_H_
#define _D3D_MODELINSTANCE_H_
/////////////////////////////////////////////////////////////////////

#include "../../../Source/gfx_ModelInstance.h"
///////////////////////// d3d_ModelInstance /////////////////////////
//
class d3d_ModelInstance : public gfx_ModelInstance
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// Create a valid, initialized model instance
	void Create( class d3d_Model* pBase, model_HInstance hInstance );

	// Set/Clear a flag that controls the Render() method
	void SetRenderFlag( const gfx_model_RenderFlag eFlag );
	void ClearRenderFlag( const gfx_model_RenderFlag eFlag );

	// Set the height offset from the model to its shadow
	void SetShadowOffset( float fShadowOffset );

	// Set the water level in model space (fWaterLevel - model base elevation)
	void SetWaterLevel( float fWaterLevel );

	// Set the models orientation to the viewer
	void SetOrientation( const std_Vector3& vOrientation );

	// Render the model using the data of this instance
	void Render( std_Vector3 vPosition, std_Vector3 vOrientation ) const;

	// Sync the instance with the piece move/turn/state data
	void SynchronizeVertices();
	void SynchronizeStates();

	// Retrieve a piece interface
	gfx_ModelPiece* Piece( const long& lPiece );

	// Set a piece's absolute offset/angle
	// NOTE: These values are not actually applied until
	//       the SynchronizeVertices() method has been called
	void SetPieceOffset( const UINT32 uiPiece, const UINT32 uiAxis, float fOffset );
	void SetPieceAngle( const UINT32 uiPiece, const UINT32 uiAxis, float fAngle );

	// Retrieve raw offset/angle data for a piece+axis
	// NOTE: For more substantial data use the gfx_ModelPiece interface
	float PieceOffset( const UINT32 uiPiece, const UINT32 uiAxis ) const;
	float PieceAngle( const UINT32 uiPiece, const UINT32 uiAxis ) const;

	// Set/Clear a particular piece state
	// NOTE: These states are not actually applied until
	//       the SynchronizeStates() method has been called
	void SetPieceState( const UINT32 uiPiece, const gfx_piece_State eState );
	void ClearPieceState(const UINT32 uiPiece, const gfx_piece_State eState);

	// Destroy this instance
	void Destroy();

	const D3DXMATRIX* Orientation() const {return &m_Orientation;}


/////////////////////////////////////////////////
// Data members
private:

	// Pointer to the base model interface
	d3d_Model*					m_BaseModel;
	model_HInstance				m_hInstance;

	// Various flags that are referenced during rendering
	DWORD						m_RenderFlags;

	// Model's orientation in the scene
	D3DXMATRIX					m_Orientation;

	// Pieces
	class d3d_ModelPiece*		m_Pieces;

	LPDIRECT3DVERTEXBUFFER8		m_VertexBuffer;
	LPDIRECT3DINDEXBUFFER8		m_VisibleIndices;
	DWORD						m_VisiblePrimitives;

	// TEMP - Handle these values more directly in the future (ie. direct to matrix)
	float						m_ShadowOffset;
	float						m_WaterLevel;


/////////////////////////////////////////////////
// Utility methods
private:

	// Recursive method to sync the VB with the dynamic piece data
	void SynchronizeVertices( d3d_Model::Piece_t* pBase, D3DXMATRIX* pParent, model_Vertex* pVerts );

	// Setup a transform matrix from the script piece data
	static INLINE D3DXMATRIX* SetupPieceTransform( const std_Vector3& Origin, const std_Vector3& Offset, const std_Vector3& sin, const std_Vector3& cos, D3DXMATRIX& M );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	d3d_ModelInstance();
	virtual ~d3d_ModelInstance();
/////////////////////////////////////////////////

}; // End class - d3d_ModelInstance
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "d3d_ModelInstance.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_MODELINSTANCE_H_)
