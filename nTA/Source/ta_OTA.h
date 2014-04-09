// ta_OTA.h //                                     \author Logan Jones
/////////////                                          \date 4/8/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_OTA_H_
#define _TA_OTA_H_
/////////////////////////////////////////////////////////////////////


struct ta_ota_GlobalHeader_t
{
	char				MissionName[64];
	char				MissionDescription[128];
	char				Planet[32];
	long				TidalStrength;
	long				SolarStrength;
	long				MinWindSpeed;
	long				MaxWindSpeed;
	long				Gravity;
	long				SchemaCount;
};

struct ta_ota_Unit_t
{
	char				Unitname[32];
	char				Ident[32];
	long				XPos;
	long				YPos;
	long				ZPos;
	long				Player;
	long				HealthPercentage;
	long				Angle;
	long				Kills;
};
typedef list< ta_ota_Unit_t >		SchemaUnitList_t;

struct ta_ota_Schema_t
{
	long				SurfaceMetal;
	long				MohoMetal;
	char				AI[32];

	SchemaUnitList_t	Units;
	std_Point			StartPositions[10];
};

#include <string>
typedef vector<string>		SchemaList_t;


BOOL ta_ota_GetSchemaList( BYTE* pFileBuffer, DWORD dwFileSize, SchemaList_t& SchemaList, ta_ota_GlobalHeader_t* pOtaInfo=NULL );
BOOL ta_ota_GetSchema( BYTE* pFileBuffer, DWORD dwFileSize, LPCTSTR strSchema, ta_ota_Schema_t* pSchema, ta_ota_GlobalHeader_t* pOtaInfo=NULL );


/////////////////////////////////////////////////////////////////////
#endif // !defined(_TA_OTA_H_)
