// ta_COB.h //                                     \author Logan Jones
/////////////                                         \date 2/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_COB_H_
#define _TA_COB_H_
#pragma pack(1)
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Every COB begins with this header
typedef struct TA_COB_HEADER
{
    // Specifies the COB version
	UINT32		Version;

    // The number of script modules in this COB file
	UINT32		NumberOfModules;

    // The number of pieces declared in this COB file
	UINT32		NumberOfPieces;

    // The size in bytes of all the script modules
	UINT32		LengthOfAllModules;

    // The number of static variables declared in this COB
	UINT32		NumberOfStaticVars;

    // This is always 0
	INT32		Always_0;

    // Offset to an array of indecies. These indecies point to the script
    // modules of the COB. The array consists of UINT32[ NumberOfScripts ]
	UINT32		OffsetToModulePointerArray;

    // Offset to an array of indecies. These indecies point to the names of
    // the script modules in the name array. The array consists of UINT32[ NumberOfScripts ]
	UINT32		OffsetToModuleNameOffsetArray;

    // Offset to an array of indecies. These indecies point to the names of
    // the pieces in the name array. The array consists of UINT32[ NumberOfPieces ]
	UINT32		OffsetToPieceNameOffsetArray;

    // Offset to the first module in the COB
	UINT32		OffsetToFirstModule;

    // Offset to the name array
	UINT32		OffsetToNameArray;

    // Offset to an array of indecies. These indecies point to the names of
    // the sounds in the name array. The array consists of UINT32[ NumberOfSounds ]
	UINT32		OffsetToSoundNameArray;

    // The number of sounds used in the script
	UINT32		NumberOfSounds;

} *LPTA_COB_HEADER;

// Indicates that the COB is form Total Annihilation
#define TA_COB_TOTAL_ANNIHILATION			( 0x00000004 )

// Indicates that the COB is form Kingdoms
#define TA_COB_KINGDOMS						( 0x00000006 )


/////////////////////////////////////////////////////////////////////
// Known opcodes
enum enumOpcode
{
	// Create enumerations from the defined instructions in ta_COB_Instructions.h
	#define MAKE_OPCODE( NAME, CODE )	Opcode_##NAME = CODE,
	#define INSTRUCTION_MACRO			MAKE_OPCODE
	#include "ta_COB_Instructions.h"
	#undef INSTRUCTION_MACRO
	#undef MAKE_OPCODE

	// Cobbler signature - First 4 bytes are 'Buil', continues for  bytes
	Opcode_CobblerCrap					=	0x6c697542,

	// Fake opcodes used by Scriptor
	Opcode_ControlBlock					=	0x90000000,

};


/////////////////////////////////////////////////////////////////////
#pragma pack()
#endif // !defined(_TA_COB_H_)
