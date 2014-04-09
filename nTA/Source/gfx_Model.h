// gfx_Model.h //                                  \author Logan Jones
////////////////                                      \date 3/19/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_MODEL_H_
#define _GFX_MODEL_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// gfx_Model /////////////////////////////
//
class gfx_Model
{
/////////////////////////////////////////////////
// Types
public:

	//struct PieceMetric_t;
	//enum PieceMetricFlags_t;

	//struct Instruction_t;
	//typedef list< Instruction_t >	InstructionList_t;


/////////////////////////////////////////////////////////////////////
// Virtual Interface
public:

	// Returns the number of pieces in this model
	virtual const DWORD GetPieceCount() const = 0;

	// Returns the id from a given piece name
	virtual long GetPieceID( LPCTSTR strPiece ) const = 0;

	// Creates an instance of the model
//	virtual void CreateInstance( const LPVOID pCreator ) = 0;

	// Create a new instance of this model
	virtual void NewInstance( class gfx_ModelInstance** ppInstance ) = 0;

//	virtual PieceMetric_t* GetMetrics( const LPVOID pModelInstance ) = 0;

	// Destroys this model interface and all its instances
	virtual void Destroy() = 0;

	// Sync the model with the instance
//	virtual void SynchronizeVertices( const LPVOID pModelInstance ) = 0;
//	virtual void SynchronizeStates( const LPVOID pModelInstance ) = 0;

	// Renders an instance of the model
//	virtual void Render( const PieceMetric_t* pMetrics, std_Vector3 vPosition, std_Vector3 vOrientation, float fShadowOffset, float fWaterLevel, DWORD dwFlags, const LPVOID pCreator ) = 0;

	// Retrieves the transformed origin of the desired piece
//	virtual void GetPieceOrigin( const DWORD dwPiece, const PieceMetric_t* pMetrics, std_Vector3 vOrientation, std_Vector3& vOrigin ) const = 0;


/////////////////////////////////////////////////
// Internal structures
public:
/*
	struct PieceMetric_t
	{
		std_Vector3		Move;
		std_Vector3		Turn;
		std_Vector3		Turn_sin;
		std_Vector3		Turn_cos;
		DWORD			Flags;
	};

	enum PieceMetricFlags_t
	{
		PMF_HIDE			= 1,
		PMF_DONT_CACHE		= 2,
		PMF_DONT_SHADE		= 4,
		PMF_DONT_SHADOW		= 8,
	};

	enum RenderFlags_t
	{
		Selected			= 1,
		InWater				= 2,
		SubmergedVisible	= 4,
	};

*/
}; // End class - gfx_Model
/////////////////////////////////////////////////////////////////////

/*
enum gfx_Model_Render_Flags_t
{
	MODEL_Selected			= 1,
	MODEL_InWater			= 2,
	MODEL_SubmergedVisible	= 4,
};
*/

/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_MODEL_H_)
