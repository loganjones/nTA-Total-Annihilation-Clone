// ta_TDF.cpp //                                   \author Logan Jones
///////////////                                       \date 4/15/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "ta_TDF.h"
#include "util_TxtParse.h"


#define CASE_STRING_VALUE( VAR, DEST )		CASE( VAR ) strcpy( DEST, Info.Value );
#define CASE_LONG_VALUE( VAR, DEST )		CASE( VAR ) DEST = atol( Info.Value );
#define GET_SIDE_VALUE() \
	STRING_SWITCH( Info.Variable ) \
		CASE_STRING_VALUE( "name", SideData.name ) \
		CASE_STRING_VALUE( "nameprefix", SideData.nameprefix ) \
		CASE_STRING_VALUE( "commander", SideData.commander ) \
		CASE_STRING_VALUE( "intgaf", SideData.intgaf ) \
		CASE_STRING_VALUE( "font", SideData.font ) \
		CASE_STRING_VALUE( "fontgui", SideData.fontgui ) \
		CASE_LONG_VALUE( "energycolor", SideData.energycolor ) \
		CASE_LONG_VALUE( "metalcolor", SideData.metalcolor ) \
	END_STRING_SWITCH
#define CASE_DEST_RECT( NAME, VAR )		CASE( NAME ) pRect = &VAR;
#define GET_RECT_VALUE() {\
	std_Rect*	pRect=NULL; \
	STRING_SWITCH( Info.Value ) \
		CASE_DEST_RECT( "logo", SideData.Header.logo ) \
		CASE_DEST_RECT( "energybar", SideData.Header.energybar ) \
		CASE_DEST_RECT( "energynum", SideData.Header.energynum ) \
		CASE_DEST_RECT( "energymax", SideData.Header.energymax ) \
		CASE_DEST_RECT( "energy0", SideData.Header.energy0 ) \
		CASE_DEST_RECT( "metalbar", SideData.Header.metalbar ) \
		CASE_DEST_RECT( "metalnum", SideData.Header.metalnum ) \
		CASE_DEST_RECT( "metalmax", SideData.Header.metalmax ) \
		CASE_DEST_RECT( "metal0", SideData.Header.metal0 ) \
		CASE_DEST_RECT( "totalunits", SideData.Header.totalunits ) \
		CASE_DEST_RECT( "totaltime", SideData.Header.totaltime ) \
		CASE_DEST_RECT( "energyproduced", SideData.Header.energyproduced ) \
		CASE_DEST_RECT( "energyconsumed", SideData.Header.energyconsumed ) \
		CASE_DEST_RECT( "metalproduced", SideData.Header.metalproduced ) \
		CASE_DEST_RECT( "metalconsumed", SideData.Header.metalconsumed ) \
		CASE_DEST_RECT( "logo2", SideData.Footer.logo2 ) \
		CASE_DEST_RECT( "unitname", SideData.Footer.unitname ) \
		CASE_DEST_RECT( "damagebar", SideData.Footer.damagebar ) \
		CASE_DEST_RECT( "unitmetalmake", SideData.Footer.unitmetalmake ) \
		CASE_DEST_RECT( "unitmetaluse", SideData.Footer.unitmetaluse ) \
		CASE_DEST_RECT( "unitenergymake", SideData.Footer.unitenergymake ) \
		CASE_DEST_RECT( "unitenergyuse", SideData.Footer.unitenergyuse ) \
		CASE_DEST_RECT( "missiontext", SideData.Footer.missiontext ) \
		CASE_DEST_RECT( "unitname2", SideData.Footer.unitname2 ) \
		CASE_DEST_RECT( "damagebar2", SideData.Footer.damagebar2 ) \
		CASE_DEST_RECT( "name", SideData.Footer.name ) \
		CASE_DEST_RECT( "description", SideData.Footer.description ) \
		CASE_DEST_RECT( "reload1", SideData.Footer.reload1 ) \
		CASE_DEST_RECT( "reload2", SideData.Footer.reload2 ) \
		CASE_DEST_RECT( "reload3", SideData.Footer.reload3 ) \
	END_STRING_SWITCH \
	bValid = Parser.Continue( &Info ); \
	if( pRect && bValid && Info.Type==PARSE_LevelUp ) \
	{ \
		long loc = Parser.Locale(); \
		while( bValid && Parser.Locale()>=loc ) \
		{ \
			if( Info.Type==PARSE_VarVal && Parser.Locale()==loc ) \
				STRING_SWITCH( Info.Variable ) \
					CASE( "x1" ) pRect->left = atol(Info.Value); \
					CASE( "y1" ) pRect->top = atol(Info.Value); \
					CASE( "x2" ) pRect->right = atol(Info.Value); \
					CASE( "y2" ) pRect->bottom = atol(Info.Value); \
				END_STRING_SWITCH \
			bValid = Parser.Continue( &Info ); \
		} \
	} \
}



//////////////////////////////////////////////////////////////////////
// ta_tdf_GetSideData() //                         \author Logan Jones
/////////////////////////                              \date 4/15/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuf    - 
//  DWORD dwFileSize  - 
//  SideList_t& Sides - 
//
void ta_tdf_GetSideData( BYTE* pFileBuf, DWORD dwFileSize, SideList_t& Sides )
{
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bValid;
	SideData_t			SideData;

	// Begin the proccess
	bValid = Parser.Begin( pFileBuf, dwFileSize, &Info );
	while( bValid )
	{
		if( Info.Type==PARSE_Header && stricmp("SIDE",Info.Value)==0 && Parser.Locale()==0 )
		{
			// Continue to the var/val block
			bValid = Parser.Continue( &Info );
			if( !bValid || Info.Type!=PARSE_LevelUp ) break;

			// Fill out the side data
			while( bValid && Parser.Locale()>0 )
			{
				if( Parser.Locale()==1 )
					if( Info.Type==PARSE_VarVal ) GET_SIDE_VALUE()
					else if( Info.Type==PARSE_Header ) GET_RECT_VALUE()
				bValid = Parser.Continue( &Info );
			}

			// Add a side entry
			Sides.push_back( SideData );
		}
		else bValid = Parser.Continue(&Info);
	}
}
// End ta_tdf_GetSideData()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ta_TDF.cpp //
/////////////////////