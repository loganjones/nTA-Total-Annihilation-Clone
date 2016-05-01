// d3d_Model.h //                          Author: Logan "Burn" Jones
////////////////                                      Date: 3/22/2002
//
/////////////////////////////////////////////////////////////////////
#ifndef _D3D_MODEL_H_
#define _D3D_MODEL_H_
/////////////////////////////////////////////////////////////////////


struct model_Vertex {		
	D3DXVECTOR3	position;	// Vertex position
	D3DCOLOR    diffuse;	// RGB values used for shadow color
							// Alpha value used for shade state
	D3DCOLOR    specular;	// RGB values used for water color
							// Alpha value used for show state
	FLOAT       tu_0, tv_0;	// Primary texture (shared per model)
//	FLOAT       tu_1, tv_1;	// Logo/animation texture (unique per model instance)
};
#define D3DFVF_MODEL_VERTEX		( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )

class d3d_ModelInstance;
typedef list<d3d_ModelInstance*>		model_InstanceList;
typedef model_InstanceList::iterator	model_HInstance;


///////////////////////////// d3d_Model /////////////////////////////
//
class d3d_Model : public gfx_Model
{
	friend class gfx_Direct3D8;

/////////////////////////////////////////////////
// Type delarations
public:

	typedef unordered_map< DWORD, long >	PieceNameHash_t;

	struct UsedTexture_t;
	typedef list< UsedTexture_t >	UsedTextureList_t;

	struct Piece_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	// Returns the number of pieces in this model
	const DWORD GetPieceCount() const;

	// Returns the id from a given piece name
	long GetPieceID( LPCTSTR strPiece ) const;

	// Create a new instance of this model
	void NewInstance( class gfx_ModelInstance** ppInstance );

	// Destroys the model interface
	void Destroy();

// Accessors
	LPDIRECT3DTEXTURE8 Texture() const {return m_PrimaryTexture;}
	DWORD VertexCount() const {return m_VertexCount;}
	DWORD TriangleCount() const {return m_TriangleCount;}
	DWORD GoundPlateSize() const {return m_GoundPlateSize;}
	DWORD GoundPlateIndex() const {return m_GoundPlateIndex;}
	DWORD GoundPlateVertex() const {return m_GoundPlateVertex;}
	Piece_t* Pieces() const {return m_Pieces;}
	model_Vertex* Vertices() const {return m_RawVertexArray;}
	WORD* Indices() const {return m_RawIndexArray;}

	void ReleaseInstance( gfx_ModelInstance* pInstance, model_HInstance hInstance );


/////////////////////////////////////////////////
// Utility methods
protected:

	BOOL Create( BYTE* pFileBuffer );

	void ScanModel( BYTE* pFileBuffer, DWORD dwOffset, std_Vertex3* pGoundPlate, UsedTextureList_t& Textures );

	void MarkTexture( LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures );

	BOOL AssembleTexture( UsedTextureList_t& Textures );

	void ReduceImage( gfx_Image_t& Image );

	void LoadModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwPieceIndex, model_Vertex* pVertexBuffer, DWORD& dwVertexIndex, WORD* pIndexBuffer, DWORD& dwIndexIndex, UsedTextureList_t& Textures );

	void GetTextureCoordinates( float* pTexCoords, LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures );

	void SetGroundPlate( DWORD dwStartIndex, DWORD dwNumVerts, model_Vertex* pVertexBuffer, WORD* pIndexBuffer );


/////////////////////////////////////////////////
// Data members
protected:

	// Pointer to the interface that created this terrain object
	gfx_Direct3D8*				m_pInterface;

	DWORD						m_PieceCount;
	PieceNameHash_t				m_PieceNames;

	DWORD						m_VertexCount;
	DWORD						m_TriangleCount;

	DWORD						m_GoundPlateSize;
	DWORD						m_GoundPlateIndex;
	DWORD						m_GoundPlateVertex;

	Piece_t*					m_Pieces;
	model_Vertex*				m_RawVertexArray;
	WORD*						m_RawIndexArray;

	LPDIRECT3DTEXTURE8			m_PrimaryTexture;

	model_InstanceList			m_ModelInstances;


/////////////////////////////////////////////////
// Type definitions
public:

	struct UsedTexture_t
	{
		LPTSTR				Texture;
		DWORD				Color;

		gfx_Image_t			Image;
		float				TexCoords[8];
	};

	struct Piece_t
	{
		DWORD			Index;
		std_Vector3		Origin;

		DWORD			VertexOffset;
		DWORD			VertexCount;
		DWORD			IndexOffset;
		DWORD			IndexCount;

		Piece_t*		pChild;
		Piece_t*		pSibling;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	d3d_Model( gfx_Direct3D8* pD3DInterface=NULL ):m_pInterface( pD3DInterface ),m_Pieces(NULL),m_RawVertexArray(NULL),m_RawIndexArray(NULL){}
	virtual ~d3d_Model(){Destroy();}
/////////////////////////////////////////////////

}; // End class - d3d_Model
/////////////////////////////////////////////////////////////////////

// Implement inline methods here for a NON debug build
#ifndef _DEBUG
 #include "d3d_Model.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_D3D_MODEL_H_)
