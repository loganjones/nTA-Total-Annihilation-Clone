// ogl_Model.h //                          Author: Logan "Burn" Jones
////////////////                                      Date: 3/22/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _OGL_MODEL_H_
#define _OGL_MODEL_H_
/////////////////////////////////////////////////////////////////////

class ogl_ModelInstance;
typedef list<ogl_ModelInstance*>		model_InstanceList;
typedef model_InstanceList::iterator	model_HInstance;

#include <string>
////////////////////////////// ogl_Model //////////////////////////////
//
class ogl_Model : public gfx_Model
{
	friend class gfx_OpenGL;

/////////////////////////////////////////////////
// Types
public:


	struct Part_t;
	struct Piece_t;
	struct ModelVertex_t;
	struct UsedTexture_t;
	typedef list< UsedTexture_t >	UsedTextureList_t;
	typedef unordered_map< string, long >	PieceNameMap_t;

	struct PieceInstance_t;
	struct lpvoid_hash{ size_t operator()(const LPVOID pA) const { return (size_t)pA; } };
	typedef unordered_map< const LPVOID, PieceInstance_t*, lpvoid_hash >	PieceInstanceMap_t;

	struct Instance_t
	{
//		PieceMetric_t*				Metrics;
//		LPDIRECT3DVERTEXBUFFER8		VertexBuffer;
//		LPDIRECT3DINDEXBUFFER8		VisibleIndices;
//		DWORD						VisiblePrimitives;
	};
	typedef unordered_map< LPVOID, Instance_t >	InstanceMap_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	// Returns the number of pieces in this model
	const DWORD GetPieceCount() const;

	// Returns the id from a given piece name
	long GetPieceID( LPCTSTR strPiece ) const;

	// Creates an instance of the model
	void CreateInstance( const LPVOID pCreator );

	// Create a new instance of this model
	void NewInstance( class gfx_ModelInstance** ppInstance );

//	PieceMetric_t* GetMetrics( const LPVOID pModelInstance );

	// Destroys this model interface and all its instances
	void Destroy();

	// Passes an instruction to the specified instance to be executed
//	DWORD ExecuteInstruction( const LPVOID pOwner, Instruction_t Instruction );

	void SynchronizeVertices( const LPVOID pModelInstance );
	void SynchronizeStates( const LPVOID pModelInstance );

	// Renders an instance of the model
//	void Render( const PieceMetric_t* pMetrics, std_Vector3 vPosition, std_Vector3 vOrientation, float fShadowOffset, float fWaterLevel, DWORD dwFlags, const LPVOID pCreator );

	// Retrieves the transformed origin of the desired piece
//	void GetPieceOrigin( const DWORD dwPiece, const PieceMetric_t* pMetrics, std_Vector3 vOrientation, std_Vector3& vOrigin ) const;

	// Release a valid model instance
	void ReleaseInstance( ogl_ModelInstance* pInstance, model_HInstance hInstance );

	Piece_t* Pieces() const {return m_Pieces;}
	GLuint Texture() const {return m_Texture;}


/////////////////////////////////////////////////
// Utility methods
protected:

	BOOL Create( BYTE* pFileBuffer, BOOL NeedsPaletteConversion );

	void ScanModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwNameBufferSize, std_Vertex3* pGoundPlate, UsedTextureList_t& Textures );

	void MarkTexture( LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures, LPTSTR strPiece=NULL );

	BOOL AssembleTexture( UsedTextureList_t& Textures );

	void ReduceImage( gfx_Image_t& Image );

	void LoadModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwNameOffset, DWORD& dwPieceOffset, GLuint uiListBase, UsedTextureList_t& Textures );

	DWORD GetTextureCoordinates( float* pTexCoords, LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures );

//	void RenderPiece( Piece_t* pPiece, const PieceMetric_t* pMetrics, DWORD dwSkipFlags ) const;


/////////////////////////////////////////////////
// Data members
protected:
								
	DWORD						m_PieceCount;
	char*						m_PieceNameBuffer;
	Piece_t*					m_Pieces;
	Part_t*						m_PartBank;
	GLuint						m_Texture;
	PieceNameMap_t				m_PieceNameMap;

	InstanceMap_t				m_Instances;

	model_InstanceList			m_ModelInstances;

	BOOL						m_NeedsPaletteConversion;


/////////////////////////////////////////////////
// Internal structures
public:

	struct Part_t
	{
		float			tu,tv;
		GLuint			DisplayList;
		Part_t*			Next;
	};

	struct Piece_t
	{
		LPTSTR			Name;
		DWORD			Index;
		int				Parent;
		std_Vector3		Origin;

		GLuint			DisplayList;
		DWORD			Primitives;
		Part_t*			Parts;

		Piece_t*		pChild;
		Piece_t*		pSibling;
	};

	struct UsedTexture_t
	{
		LPTSTR			Texture;
		DWORD			Color;

		gfx_Image_t		Image;
		float			TexCoords[8];
	};

	struct PieceInstance_t
	{
		std_Vector3		Move;
		std_Vector3		Turn;
		DWORD			Flags;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	ogl_Model();
	virtual ~ogl_Model();
/////////////////////////////////////////////////

}; // End class - ogl_Model
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_OGL_MODEL_H_)
