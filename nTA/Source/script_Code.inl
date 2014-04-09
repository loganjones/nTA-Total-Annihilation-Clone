// script_Code.inl //                              \author Logan Jones
////////////////////                                   \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Code::GetModule() //                     \author Logan Jones
/////////////////////////////                           \date 5/4/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strModuleName - 
//
// Return: const UINT32 - 
//
const UINT32 script_Code::GetModule( LPCTSTR strModuleName ) const
{	
	ModuleMap_t::const_iterator	it = ModuleMap.find( strModuleName );
	return it==ModuleMap.end() ? 0xFFFFFFFF : (*it).second;
}
// End script_Code::GetModule()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Code::ResolvePiece() //                  \author Logan Jones
////////////////////////////////                        \date 5/5/2002
//               
//====================================================================
// Parameters:
//  long lScriptPiece - 
//
// Return: long - 
//
long script_Code::ResolvePiece( long lScriptPiece ) const
{
	//return Model->GetPieceID( LPTSTR(pFileBuffer + ModuleNamePointers[lScriptPiece]) );
	return PieceIndices[lScriptPiece];
}
// End script_Code::ResolvePiece()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Code.inl //
//////////////////////////
