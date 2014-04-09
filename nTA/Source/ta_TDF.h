// ta_TDF.h //                                     \author Logan Jones
/////////////                                         \date 4/15/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_TDF_H_
#define _TA_TDF_H_
/////////////////////////////////////////////////////////////////////


struct SideData_t;
typedef vector< SideData_t >		SideList_t;


// Parse a TDF for side data
void ta_tdf_GetSideData( BYTE* pFileBuf, DWORD dwFileSize, SideList_t& Sides );


struct SideData_t
{
	char		name[ 16 ];
	char		nameprefix[ 8 ];
	char		commander[ 32 ];
	char		intgaf[ 32 ];
	char		set[16];

	char		font[ 16 ];
	char		fontgui[ 16 ];
	long		energycolor;
	long		metalcolor;

	struct Header_t{
		std_Rect	logo;
		std_Rect	energybar;
		std_Rect	energynum;
		std_Rect	energymax;
		std_Rect	energy0;
		std_Rect	metalbar;
		std_Rect	metalnum;
		std_Rect	metalmax;
		std_Rect	metal0;
		std_Rect	totalunits;
		std_Rect	totaltime;
		std_Rect	energyproduced;
		std_Rect	energyconsumed;
		std_Rect	metalproduced;
		std_Rect	metalconsumed;
	}			Header;

	struct Footer_t{
		std_Rect	logo2;
		std_Rect	unitname;
		std_Rect	damagebar;
		std_Rect	unitmetalmake;
		std_Rect	unitmetaluse;
		std_Rect	unitenergymake;
		std_Rect	unitenergyuse;
		std_Rect	missiontext;
		std_Rect	unitname2;
		std_Rect	damagebar2;
		std_Rect	name;
		std_Rect	description;
		std_Rect	reload1;
		std_Rect	reload2;
		std_Rect	reload3;
	}			Footer;
};


/////////////////////////////////////////////////////////////////////
#endif // !defined(_TA_TDF_H_)
