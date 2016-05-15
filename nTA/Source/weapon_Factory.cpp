// weapon_Factory.cpp //                           \author Logan Jones
///////////////////////                               \date 5/16/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "weapon_Factory.h"
#include "util_TxtParse.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
weapon_Factory::weapon_Factory()
{}
weapon_Factory::~weapon_Factory()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// weapon_Factory::Create() //                     \author Logan Jones
/////////////////////////////                          \date 5/16/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL weapon_Factory::Create()
{
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;
	BOOL							bWorking;

	// Proccess each tdf in the weapons directory
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "tdf", "weapons", &FindInfo );
	bWorking = hFind!=NULL;
	for( ; bWorking; )
	{
		// What unit are we loading
		theApp.Console.Comment( CT_LOAD, "Processing weapons/%s", FindInfo.Name );

		// Only update the load screen every now and then. This keeps the load time
		// from being dominated by the rendering of the load screen
	//	if((std_Time()-time)>=200)
	//	{
	//		Load.Progress( count * 100.0f / FileCount );
	//		time=std_Time();
	//	}

		ProccessTdf( FindInfo.hFile );

		// Find next file
		bWorking = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( fbi files )
	theFileTree.CloseFind( hFind );

	return TRUE;
}
// End weapon_Factory::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// weapon_Factory::Destroy() //                    \author Logan Jones
//////////////////////////////                         \date 5/16/2002
//               
//====================================================================
//
void weapon_Factory::Destroy()
{
}
// End weapon_Factory::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// weapon_Factory::ProccessTdf() //                \author Logan Jones
//////////////////////////////////                     \date 5/16/2002
//               
//====================================================================
// Parameters:
//  void* hFile - 
//
void weapon_Factory::ProccessTdf( void* hFile )
{
	DWORD				FileSize;
	BYTE*				FileBuffer;
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bWorking;
	string				WeaponID;

	// Get the TDF file buffer
	FileSize = theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(hFile), &FileBuffer );

	// Start proccessing the TDF
	bWorking = Parser.Begin( FileBuffer, FileSize, &Info );
	while( bWorking )
	{
		// Check if this is a new weapon
		if( Info.Type==PARSE_Header && Parser.Locale()==0 )
		{
			WeaponID = Info.Value;

			// Move into the code block
			bWorking = Parser.Continue( &Info );
			if( !bWorking || Info.Type!=PARSE_LevelUp ) continue;

			// Proccess the weapon
			while( bWorking && Parser.Locale()>0 )
			{
				if( Info.Type==PARSE_VarVal && Parser.Locale()==1 )
                {
					//STRING_SWITCH( Info.Variable )
					//END_STRING_SWITCH
                }
				else if( Info.Type==PARSE_Header && Parser.Locale()==1 )
				{
				}
				bWorking = Parser.Continue( &Info );
			} // end while( proccessing weapon )

		} // end if( weapon )

		bWorking = Parser.Continue( &Info );

	} // end while( bWorking )

	SAFE_DELETE_ARRAY( FileBuffer );
}
// End weapon_Factory::ProccessTdf()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - weapon_Factory.cpp //
/////////////////////////////
