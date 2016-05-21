// script_Code.cpp //                              \author Logan Jones
////////////////////                                   \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "script.h"
#include "script_Code.h"
#include "ta_COB.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "script_Code.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
script_Code::script_Code()
{}
script_Code::~script_Code()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Code::Ininialize() //                    \author Logan Jones
//////////////////////////////                          \date 5/4/2002
//               
//====================================================================
// Parameters:
//  BYTE* pScript     - 
//  gfx_Model* pModel - 
//
// Return: BOOL - 
//
BOOL script_Code::Ininialize( BYTE* pScript, gfx_Model* pModel )
{
	LPTA_COB_HEADER		pHeader = (LPTA_COB_HEADER)pScript;
	UINT32*				pModulePointers;

	// Convert the piece indecies
	RealizePieces( pScript, pModel );
	Model = pModel;

	// Set the start of the code and the piece count
	pFileBuffer = pScript;
	pStart = (UINT32*)(pFileBuffer + pHeader->OffsetToFirstModule);
	PieceCount = pModel->GetPieceCount();

	// Get the module pointers, name pointers
	pModulePointers = (UINT32*)(pFileBuffer + pHeader->OffsetToModulePointerArray);
	ModuleNamePointers = (UINT32*)(pFileBuffer + pHeader->OffsetToModuleNameOffsetArray);

	// Add each module
	for( DWORD i=0; i<pHeader->NumberOfModules; ++i)
	{
		Modules.push_back( pModulePointers[i] );
		ModuleMap[ (LPCTSTR)(pFileBuffer + ModuleNamePointers[i]) ] = pModulePointers[i];
	}
	return TRUE;
}
// End script_Code::Ininialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Code::RealizePieces() //                 \author Logan Jones
////////////////////////////////////                   \date 3/25/2002
//               
//====================================================================
// Parameters:
//  BYTE* pScript     - 
//  gfx_Model* pModel - 
//
void script_Code::RealizePieces( BYTE* pScript, gfx_Model* pModel )
{
	LPTA_COB_HEADER		pHeader = (LPTA_COB_HEADER)pScript;
	DWORD*				PieceNamePointers = (DWORD*)(pScript + pHeader->OffsetToPieceNameOffsetArray);
//	vector< UINT32>		Index( pHeader->NumberOfPieces );
	BOOL				RealizationNeeded = FALSE;
	UINT32*				pCode = (UINT32*)(pScript + pHeader->OffsetToFirstModule);
	LPTSTR				PieceName;
    UINT32				n;

	// Iterate through each piece
	PieceIndices.resize( pHeader->NumberOfPieces );
	for( n=0; n<pHeader->NumberOfPieces; ++n)
	{
		// Get the pointer to the name
		PieceName = (LPTSTR)(pScript + PieceNamePointers[n]);

		// Convert the name to uppercase
		for( int i=0; PieceName[i]!='\0'; ++i)
			PieceName[i] = toupper(PieceName[i]);

		// Get the actual index of the piece
		PieceIndices[ n ] = (UINT32)pModel->GetPieceID( PieceName );
		if( n != PieceIndices[ n ] )
			RealizationNeeded = TRUE;
	}

	// Iterate through the code and translate
	if( RealizationNeeded ) for( n=0; n<pHeader->LengthOfAllModules; ) switch( pCode[n] )
	{
		case Opcode_MovePieceWithSpeed:
		case Opcode_TurnPieceWithSpeed:
		case Opcode_StartSpin:
		case Opcode_StopSpin:
		case Opcode_MovePieceNow:
		case Opcode_TurnPieceNow:
		case Opcode_WaitForTurn:
		case Opcode_WaitForMove:
			pCode[n+1] = PieceIndices[ pCode[n+1] ];
			n += 3;
			break;

		case Opcode_ShowPiece:
		case Opcode_HidePiece:
		case Opcode_CachePiece:
		case Opcode_DontCachePiece:
		case Opcode_DontShadow:
		case Opcode_DontShade:
		case Opcode_EmitSfx:
		case Opcode_Explode:
			pCode[n+1] = PieceIndices[ pCode[n+1] ];
			n += 2;
			break;

		default:
			++n;
	}
}
// End script_Code::RealizePieces()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Code.cpp //
//////////////////////////
