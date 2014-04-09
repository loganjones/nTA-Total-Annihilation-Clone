// ta_OTA.cpp //                                   \author Logan Jones
///////////////                                        \date 4/8/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "ta_OTA.h"
#include "util_TxtParse.h"

// Macros used in filling the strucure with info from the file
#define STRING_MEMBER( VARIABLE_NAME, DESTINATION ) \
	CASE( VARIABLE_NAME ) \
		strcpy( DESTINATION, Info.Value );
#define INT_MEMBER( VARIABLE_NAME, DESTINATION ) \
	CASE( VARIABLE_NAME ) \
		(DESTINATION) = atol( Info.Value );


#define GET_HEADER_ITEM( HEADER ) STRING_SWITCH( Info.Variable ) \
	STRING_MEMBER( "MissionName", (HEADER).MissionName ) \
	STRING_MEMBER( "MissionDescription", (HEADER).MissionDescription ) \
	STRING_MEMBER( "planet", (HEADER).Planet ) \
	INT_MEMBER( "TidalStrength", (HEADER).TidalStrength ) \
	INT_MEMBER( "SolarStrength", (HEADER).SolarStrength ) \
	INT_MEMBER( "MinWindSpeed", (HEADER).MinWindSpeed ) \
	INT_MEMBER( "MaxWindSpeed", (HEADER).MaxWindSpeed ) \
	INT_MEMBER( "Gravity", (HEADER).Gravity ) \
	INT_MEMBER( "SCHEMACOUNT", (HEADER).SchemaCount ) \
END_STRING_SWITCH


#define GET_SCHEMA_ITEM( pSCHEMA ) STRING_SWITCH( Info.Variable ) \
	INT_MEMBER( "SurfaceMetal", (pSCHEMA)->SurfaceMetal ) \
	INT_MEMBER( "MohoMetal", (pSCHEMA)->MohoMetal ) \
	STRING_MEMBER( "aiprofile", (pSCHEMA)->AI ) \
END_STRING_SWITCH


#define GET_UNIT_ITEM( UNIT ) STRING_SWITCH( Info.Variable ) \
	STRING_MEMBER( "Unitname", (UNIT).Unitname ) \
	STRING_MEMBER( "Ident", (UNIT).Ident ) \
	INT_MEMBER( "XPos", (UNIT).XPos ) \
	INT_MEMBER( "YPos", (UNIT).YPos ) \
	INT_MEMBER( "ZPos", (UNIT).ZPos ) \
	INT_MEMBER( "Player", (UNIT).Player ) \
	INT_MEMBER( "HealthPercentage", (UNIT).HealthPercentage ) \
	INT_MEMBER( "Angle", (UNIT).Angle ) \
	INT_MEMBER( "Kills", (UNIT).Kills ) \
END_STRING_SWITCH


//////////////////////////////////////////////////////////////////////
// ta_ota_GetSchemaList() //                       \author Logan Jones
///////////////////////////                             \date 4/9/2002
// Description:  Returns a list of strings that indcate the schemas
//               avialable in the ota. IfpOtaInfo is not NULL, it is
//               filled with the ota header data.
//====================================================================
// Parameters:
//  BYTE* pFileBuffer               - 
//  DWORD dwFileSize                - 
//  SchemaList_t& SchemaList        - 
//  ta_ota_GlobalHeader_t* pOtaInfo - 
//
// Return: BOOL - 
//
BOOL ta_ota_GetSchemaList( BYTE* pFileBuffer, DWORD dwFileSize, SchemaList_t& SchemaList, ta_ota_GlobalHeader_t* pOtaInfo )
{
	util_TxtParse		TxtParse;
	Parse_Info			Info;
	BOOL				bValid;
	DWORD				Index;
	string				Schema;

	// Begin the proccess
	bValid = TxtParse.Begin( pFileBuffer, dwFileSize, &Info );

	// Make sure this is the header
	if( !bValid || Info.Type!=PARSE_Header || (stricmp("GlobalHeader",Info.Value)!=0) )
		return FALSE;
	
	// Continue into the header
	if( !TxtParse.Continue(&Info) || Info.Type!=PARSE_LevelUp ) return FALSE;
	bValid = TxtParse.Continue(&Info);
	while( bValid && TxtParse.Locale()>0 )
	{
		switch( Info.Type )
		{
			case PARSE_VarVal: if( pOtaInfo )
			{
				GET_HEADER_ITEM( *pOtaInfo );
				if( stricmp(Info.Variable,"SchemaCount")==0 )
					SchemaList.resize( atol(Info.Value) );
			}
			break; // End case PARSE_VarVal

			case PARSE_Header: if( strnicmp(Info.Value,"Schema ",7)==0 )
			{
				// Get the index # from the header title; ie [Schema #]
				Index = atol( Info.Value + 7 );

				// Continue to the type var/val
				bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
				bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type==PARSE_LevelDown ) continue;
				while( bValid && TxtParse.Locale()>=2 )
				{
					if( TxtParse.Locale()==2 && Info.Type==PARSE_VarVal && stricmp(Info.Variable,"Type")==0 )
					{
						SchemaList[ Index ] = Info.Value;
						break;
					}
					bValid = TxtParse.Continue(&Info);
				}

				// Continue until we are out of this header
				while( bValid && TxtParse.Locale()>=2 ) bValid = TxtParse.Continue(&Info);
			}
			break; // End case PARSE_Header

		} // end switch( Info.Type )

		// Next
		bValid = TxtParse.Continue(&Info);
	} // end while( in GlobalHeader )

	return !SchemaList.empty();
}
// End ta_ota_GetSchemaList()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ta_ota_GetSchema() //                           \author Logan Jones
///////////////////////                                 \date 4/9/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer               - 
//  DWORD dwFileSize                - 
//  LPCTSTR strSchema               - 
//  ta_ota_Schema_t* pSchema        - 
//  ta_ota_GlobalHeader_t* pOtaInfo - 
//
// Return: BOOL - 
//
BOOL ta_ota_GetSchema( BYTE* pFileBuffer, DWORD dwFileSize, LPCTSTR strSchema, ta_ota_Schema_t* pSchema, ta_ota_GlobalHeader_t* pOtaInfo )
{
	util_TxtParse		TxtParse;
	Parse_Info			Info;
	BOOL				bValid;
	BOOL				bFound = FALSE;

	// Begin the proccess
	bValid = TxtParse.Begin( pFileBuffer, dwFileSize, &Info );

	// Make sure this is the header
	if( !bValid || Info.Type!=PARSE_Header || (stricmp("GlobalHeader",Info.Value)!=0) )
		return FALSE;
	
	// Continue into the header
	if( !TxtParse.Continue(&Info) || Info.Type!=PARSE_LevelUp ) return FALSE;
	bValid = TxtParse.Continue(&Info);
	while( bValid && TxtParse.Locale()>0 && !bFound )
	{
		switch( Info.Type )
		{
			case PARSE_VarVal: if( pOtaInfo )
			{
				GET_HEADER_ITEM( *pOtaInfo );
			}
			break; // End case PARSE_VarVal

			case PARSE_Header: if( strnicmp(Info.Value,"Schema ",7)==0 )
			{
				// Continue to the type var/val
				bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
				bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type==PARSE_LevelDown ) continue;
				while( bValid && TxtParse.Locale()>=2 )
				{
					if( TxtParse.Locale()>2 ) ; // Do nothing
					else if( Info.Type==PARSE_VarVal )
					{
						// Check if this is not the desired schema
						if( stricmp(Info.Variable,"Type")==0 )
							if( stricmp(Info.Value,strSchema)==0 ) bFound = TRUE;
							else {
							//	while( bValid && TxtParse.Locale()>=2 )
							//		bValid = TxtParse.Continue(&Info);
								bValid = TxtParse.SkipUntil( 1 );
								break;
							}

						GET_SCHEMA_ITEM( pSchema );
					}
					else if( Info.Type==PARSE_Header && stricmp(Info.Value,"specials")==0 )
					{
						bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
						bValid = TxtParse.Continue(&Info);
						while( bValid && TxtParse.Locale()>2 )
						{
							if( Info.Type==PARSE_LevelUp && TxtParse.Locale()==4 )
							{
								bValid = TxtParse.Continue(&Info);
								if( bValid && Info.Type==PARSE_VarVal && stricmp(Info.Variable,"specialwhat")==0 )
								{
									if( strnicmp(Info.Value,"StartPos",8)==0 )
									{
										long index = atol( Info.Value + 8 ) - 1;
										bValid = TxtParse.Continue(&Info);
										if( !bValid || Info.Type!=PARSE_VarVal || stricmp(Info.Variable,"XPos")!=0 ) continue;
										pSchema->StartPositions[index].x = atol(Info.Value);
										bValid = TxtParse.Continue(&Info);
										if( !bValid || Info.Type!=PARSE_VarVal || stricmp(Info.Variable,"ZPos")!=0 ) continue;
										pSchema->StartPositions[index].y = atol(Info.Value);
									}
								}
								else continue;
							}

							bValid = TxtParse.Continue(&Info);
						}
						continue;
					}
					else if( Info.Type==PARSE_Header && stricmp(Info.Value,"units")==0 )
					{
						ta_ota_Unit_t		NewUnit;

						bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
						bValid = TxtParse.Continue(&Info);
						while( bValid && TxtParse.Locale()>2 )
						{
							if( Info.Type==PARSE_Header && strnicmp(Info.Value,"unit",4)==0 )
							{
								bValid = TxtParse.Continue(&Info); if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
								bValid = TxtParse.Continue(&Info);
								while( bValid && TxtParse.Locale()>3 )
								{
									if( TxtParse.Locale()==4 && Info.Type==PARSE_VarVal )
									{
										GET_UNIT_ITEM( NewUnit );
									}

									bValid = TxtParse.Continue(&Info);
								}
								pSchema->Units.push_back( NewUnit );
								continue;
							}

							bValid = TxtParse.Continue(&Info);
						}
						continue;
					}

					bValid = TxtParse.Continue(&Info);
				} // end while( in schema )
				
			} // end if( header is schema )
			break; // End case PARSE_Header

		} // end switch( Info.Type )

		// Next
		bValid = TxtParse.Continue(&Info);
	} // end while( in GlobalHeader )

	return bFound;
}
// End ta_ota_GetSchema()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ta_OTA.cpp //
/////////////////////