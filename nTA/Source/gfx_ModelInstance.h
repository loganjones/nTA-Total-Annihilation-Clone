// gfx_ModelInstance.h //                          \author Logan Jones
////////////////////////                             \date 02-22-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_MODELINSTANCE_H_
#define _GFX_MODELINSTANCE_H_
/////////////////////////////////////////////////////////////////////

enum gfx_piece_State {
	piece_Hide			= 1,
	piece_Cache			= 2,
	piece_DontShade		= 4,
	piece_DontShadow	= 8,
};

enum gfx_model_RenderFlag {
	model_Selected		= 1,
	model_InWater		= 2,
	model_VisibleInWater= 4,
	model_Cloaked		= 8,
};

///////////////////////// gfx_ModelInstance /////////////////////////
//
class gfx_ModelInstance { public:

	// Set/Clear a flag that controls the Render() method
	virtual void SetRenderFlag( const gfx_model_RenderFlag eFlag ) = 0;
	virtual void ClearRenderFlag( const gfx_model_RenderFlag eFlag ) = 0;

	// Set the height offset from the model to its shadow
	virtual void SetShadowOffset( float fShadowOffset ) = 0;

	// Set the water level in model space (fWaterLevel - model base elevation)
	virtual void SetWaterLevel( float fWaterLevel ) = 0;

	// Set the models orientation to the viewer
	virtual void SetOrientation( const std_Vector3& vOrientation ) = 0;

	// Render the model using the data of this instance
	virtual void Render( std_Vector3 vPosition, std_Vector3 vOrientation ) const = 0;

	// Sync the instance with the piece move/turn/state data
	virtual void SynchronizeVertices() = 0;
	virtual void SynchronizeStates() = 0;

	// Retrieve a piece interface
	virtual class gfx_ModelPiece* Piece( const long& lPiece ) = 0;

	// Set a piece's absolute offset/angle
	// NOTE: These values are not actually applied until
	//       the SynchronizeVertices() method has been called
	virtual void SetPieceOffset( const UINT32 uiPiece, const UINT32 uiAxis, float fOffset ) = 0;
	virtual void SetPieceAngle( const UINT32 uiPiece, const UINT32 uiAxis, float fAngle ) = 0;

	// Retrieve raw offset/angle data for a piece+axis
	// NOTE: For more substantial data use the gfx_ModelPiece interface
	virtual float PieceOffset( const UINT32 uiPiece, const UINT32 uiAxis ) const = 0;
	virtual float PieceAngle( const UINT32 uiPiece, const UINT32 uiAxis ) const = 0;

	// Set/Clear a particular piece state
	// NOTE: These states are not actually applied until
	//       the SynchronizeStates() method has been called
	virtual void SetPieceState( const UINT32 uiPiece, const gfx_piece_State eState ) = 0;
	virtual void ClearPieceState(const UINT32 uiPiece, const gfx_piece_State eState) = 0;

	// Destroy this instance
	virtual void Destroy() = 0;

}; // End class - gfx_ModelInstance
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_MODELINSTANCE_H_)
