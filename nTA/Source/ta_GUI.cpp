// ta_GUI.cpp //                                   \author Logan Jones
///////////////                                      \date 10/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "ta_GUI.h"
#include "util_TxtParse.h"

//////////////////////////////////////////////////////////////////////
// ta_gui_LoadCommon() //                          \author Logan Jones
////////////////////////                               \date 4/23/2002
//               
//====================================================================
// Parameters:
//  util_TxtParse& Parser   - 
//  ta_gui_Common_t& Common - 
//
// Return: BOOL - 
//
BOOL ta_gui_LoadCommon( util_TxtParse& Parser, ta_gui_Common_t& Common )
{
	long				Entry = Parser.Locale();
	Parse_Info			Info;
	BOOL				bValid;

	// The parser should be sitting at [COMMON] so move ahead to the block
	bValid = Parser.Continue( &Info );
	if( !bValid || Info.Type!=PARSE_LevelUp ) return FALSE;

	while( bValid && Parser.Locale()>Entry )
	{
		if( Info.Type==PARSE_VarVal && Parser.Locale()==(Entry+1) )
			STRING_SWITCH( Info.Variable )
				CASE( "ID" )			Common.ID = (ta_gui_GadgetTypes_t)atol( Info.Value );
				CASE( "Assoc" )			Common.Assoc = atol( Info.Value );
				CASE( "Name" )			strcpy( Common.Name, Info.Value );
				CASE( "XPos" )			Common.XPos = atol( Info.Value );
				CASE( "YPos" )			Common.YPos = atol( Info.Value );
				CASE( "Width" )			Common.Width = atol( Info.Value );
				CASE( "Height" )		Common.Height = atol( Info.Value );
				CASE( "Attribs" )		Common.Attribs = atol( Info.Value );
				CASE( "Active" )		Common.Active = atol( Info.Value );
				CASE( "CommonAttribs" )	Common.CommonAttribs = atol( Info.Value );
			END_STRING_SWITCH
		bValid = Parser.Continue( &Info );
	}

	return bValid;
}
// End ta_gui_LoadCommon()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ta_GUI.cpp //
/////////////////////