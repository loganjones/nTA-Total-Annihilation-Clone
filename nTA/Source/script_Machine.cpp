// script_Machine.cpp //                           \author Logan Jones
///////////////////////                               \date 3/24/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "script_Machine.h"
#include "ta_COB.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "script_Machine.inl"
#endif // defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
script_Machine::script_Machine()
{
	script_Proccess::AssociateOpcodeHandlers();
}
script_Machine::~script_Machine()
{
	Destroy();
	script_Proccess::DestroyOpcodeHandlers();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Machine::LoadScript() //                 \author Logan Jones
/////////////////////////////////                      \date 3/24/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer - 
//  gfx_Model* pModel - 
//  script_Code& Script  - 
//
// Return: BOOL - 
//
BOOL script_Machine::LoadScript( LPTSTR strName, gfx_Model* pModel, script_Code** ppScript )
{
	char				FilePath[ 64 ];
	BYTE*				pFile;

	// Get the script file
	sprintf( FilePath, "Scripts/%s.cob", strName );
	theFileTree.DumpFileToBuffer( FilePath, &pFile );

	// Add a new lisat entry and initialize it
	m_Scripts.push_back( script_Code() );
	*ppScript = &m_Scripts.back();
	if( bFAILED((*ppScript)->Ininialize(pFile,pModel)) )
	{
		m_Scripts.pop_back();
		return FALSE;
	}
	else return TRUE;
}
// End script_Machine::LoadScript()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Machine::CreateProccess() //             \author Logan Jones
/////////////////////////////////////                  \date 3/24/2002
//               
//====================================================================
// Parameters:
//  script_Code& Script          - 
//  unit_Object* pUnit           - 
//  gfx_ModelInstance* pModel    - 
//  script_Proccess** ppProccess - 
//
// Return: BOOL - 
//
BOOL script_Machine::CreateProccess( script_Code* pScript, unit_Object* pUnit, gfx_ModelInstance* pModel, script_Proccess** ppProccess )
{
	script_Proccess*	pProccess;
	LPTA_COB_HEADER		pHeader;

	// Add a new proccess indexed by the unit pointer
	pProccess = &m_Proccesses[ pUnit ];

	// Set the script and unit pointers
	pProccess->pScript = pScript;
	pProccess->pUnit = pUnit;

	// Alocate and initialize the piece metrics
//	pProccess->pPieces = pUnit->m_pUnitType->Model->GetMetrics( pUnit );
	pProccess->pModel = pModel;
//	pProccess->pPieces = new gfx_Model::PieceMetric_t[ pProccess->pScript->PieceCount ];
//	ZeroMemory( pProccess->pPieces, pProccess->pScript->PieceCount * sizeof(gfx_Model::PieceMetric_t) );
//	for( long i=0; i<pProccess->pScript->PieceCount; ++i) pProccess->pPieces[i].Turn_cos=std_Vector3(1,1,1);

	// Get the header
	pHeader = (LPTA_COB_HEADER)pProccess->pScript->pFileBuffer;

	// Allocate the static variables and initialize them to be nice
	pProccess->Statics = new long[ pHeader->NumberOfStaticVars ];
	ZeroMemory( pProccess->Statics, pHeader->NumberOfStaticVars * sizeof(long) );

	// Set the proccess pointer and return
	*ppProccess = pProccess;
	return TRUE;
}
// End script_Machine::CreateProccess()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Machine::DestroyProccess() //            \author Logan Jones
//////////////////////////////////////                  \date 5/5/2002
//               
//====================================================================
// Parameters:
//  unit_Object* pUnit - 
//
void script_Machine::DestroyProccess( unit_Object* pUnit )
{
	m_Proccesses.erase( pUnit );
}
// End script_Machine::DestroyProccess()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Machine::Update() //                     \author Logan Jones
/////////////////////////////                          \date 3/24/2002
//               
//====================================================================
//
void script_Machine::Update()
{
	// Run each proccess
	for_each( m_Proccesses.begin(), m_Proccesses.end(), RunProccess );
}
// End script_Machine::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Machine::Destroy() //                    \author Logan Jones
//////////////////////////////                         \date 3/24/2002
//               
//====================================================================
//
void script_Machine::Destroy()
{
	// Destroy the scripts
	{
		ScriptList_t::iterator			it = m_Scripts.begin();
		ScriptList_t::const_iterator	end= m_Scripts.end();
		for(; it!=end; ++it)
		{
			(*it).Modules.clear();
			SAFE_DELETE( (*it).pFileBuffer );
		}
		m_Scripts.clear();
	}

	// Destroy any hanging proccesses
	m_Proccesses.clear();
}
// End script_Machine::Destroy()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Machine.cpp //
/////////////////////////////
