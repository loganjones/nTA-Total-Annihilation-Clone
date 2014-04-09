// unit_Type.h //                                  \author Logan Jones
////////////////                                     \date 10/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_TYPE_H_
#define _UNIT_TYPE_H_
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Unit type: Most everything identifying a particular unit goes here
struct unit_Type
{
	// Unique ID for a unit type (case-insensitive hash of UnitName)
	UINT32							Key;
	char							UnitName[ 32 ];

	// Verbose name and desciption for the type
	char							Name[32];
	char							Description[64];

	class gfx_Model*				Model;
	class script_Code*				pScript;
	struct unit_SoundCategory*		pSounds;	

	std_Size						FootPrint;

	DWORD							MaxDamage;

	DWORD							BuildCostEnergy;
	DWORD							BuildCostMetal;
	float							BuildTime;
	float							BuildAngle;

	float							WorkerTime;
	DWORD							BuildDistance;

	DWORD							MetalStorage;
	DWORD							EnergyStorage;
	DWORD							MetalMake;
	DWORD							EnergyMake;
	long							MetalUse;
	long							EnergyUse;
	DWORD							MakesMetal;

	float							MetalExtractionRate;
	DWORD							WindGenerator;

	DWORD							Abilities;

	DWORD							BuildMenuCount;

	BYTE							InitialMoveOrder;
	BYTE							InitialFireOrder;
	BYTE							InitialActivationOrder;
	BYTE							InitialCloakOrder;

	float							Acceleration;
	float							BrakeRate;
	float							MaxSpeed;
	float							TurnRate;

	float							MaxSlope;
	DWORD							MaxWaterDepth;
	DWORD							MinWaterDepth;
	BOOL							Upright;

	float							CruiseAltitude;
	float							BankScale;
	float							PitchScale;

	enum unit_Behaviours_t {
		Structure,
		Groundcraft,
		Seacraft,
		Aircraft,
		Hovercraft,
	}								Behaviour;

	enum unit_Abilities_t {
		CanBuild		=	( 1<<0 ),
		CanMove			=	( 1<<1 ),
		CanPatrol		=	( 1<<2 ),
		CanStop			=	( 1<<3 ),
		CanDefend		=	( 1<<4 ),
		CanAttack		=	( 1<<5 ),
		CanReclaim		=	( 1<<6 ),
		CanCapture		=	( 1<<7 ),
		CanBlast		=	( 1<<8 ),
		CanLoad			=	( 1<<9 ),
		OnOffable		=	( 1<<10 ),
		CanCloak		=	( 1<<11 ),
		CanFly			=	( 1<<12 ),
		CanHover		=	( 1<<13 ),
		HasMoveOrders	=	( 1<<14 ),
		HasFireOrders	=	( 1<<15 ),
	};

}; // End struct - unit_Type
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_TYPE_H_)
