// script_Code.h //                                \author Logan Jones
//////////////////                                     \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SCRIPT_CODE_H_
#define _SCRIPT_CODE_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// script_Code ////////////////////////////
//
class script_Code
{
	friend class script_Machine;
	friend class script_Proccess;

/////////////////////////////////////////////////
// Type declarations
protected:

	typedef vector< UINT32 >	ModuleArray_t;
	typedef map< LPCTSTR, UINT32, pchar_less_nocase_t >	ModuleMap_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	INLINE const UINT32 GetModule( LPCTSTR strModuleName ) const;


/////////////////////////////////////////////////
// Utility Methods
protected:

	BOOL Ininialize( BYTE* pScript, gfx_Model* pModel );

	// Changes the piece indecies in the script to the actual piece indecies in the model
	void RealizePieces( BYTE* pScript, gfx_Model* pModel );

	INLINE long ResolvePiece( long lScriptPiece ) const;


/////////////////////////////////////////////////
// Data members
protected:

	BYTE*						pFileBuffer;
	UINT32*						pStart;
	DWORD						PieceCount;
	ModuleArray_t				Modules;
	ModuleMap_t					ModuleMap;
	UINT32*						ModuleNamePointers;
	gfx_Model*					Model;
	vector<UINT32>				PieceIndices;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	script_Code();
	virtual ~script_Code();
/////////////////////////////////////////////////

}; // End class - script_Code
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "script_Code.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SCRIPT_CODE_H_)
