// script_Machine.h //                             \author Logan Jones
/////////////////////                                 \date 3/24/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SCRIPT_MACHINE_H_
#define _SCRIPT_MACHINE_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////// script_Machine ///////////////////////////
//
class script_Machine
{

/////////////////////////////////////////////////
// Type declarations
protected:

	typedef list< script_Code >						ScriptList_t;
	typedef map< unit_Object*, script_Proccess >	ProccessMap_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	// Loads a cob script file into the code bank
	BOOL LoadScript( LPTSTR strName, gfx_Model* pModel, script_Code** ppScript );

	// Creates a new code proccess for a new unit
	BOOL CreateProccess( script_Code* pScript, unit_Object* pUnit, gfx_ModelInstance* pModel, script_Proccess** ppProccess );

	// Destroys a proccess
	void DestroyProccess( unit_Object* pUnit );

	// Updates the script system
	void Update();

	// Destroys and deallocates all scripts and proccesses managed by the machine
	void Destroy();


/////////////////////////////////////////////////
// Utility
protected:

	static INLINE void RunProccess( ProccessMap_t::value_type& Proccess );


/////////////////////////////////////////////////
// Data members
protected:

	ScriptList_t			m_Scripts;
	ProccessMap_t			m_Proccesses;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	script_Machine();
	virtual ~script_Machine();
/////////////////////////////////////////////////

}; // End class - script_Machine
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "script_Machine.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SCRIPT_MACHINE_H_)
