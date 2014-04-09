// script_Functions.cpp //                         \author Logan Jones
/////////////////////////                            \date 02-25-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "script.h"
#include "gfx.h"
#include "ta_COB.h"
#include "game_Constants.h"


//////////////////////////////////////////////////////////////////////
// script_Proccess::Create() //                    \author Logan Jones
//////////////////////////////                          \date 5/5/2002
//               
//====================================================================
//
void script_Proccess::Create()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "Create" );
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
}
// End script_Proccess::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::SweetSpot() //                 \author Logan Jones
/////////////////////////////////                       \date 5/5/2002
//               
//====================================================================
// Return: gfx_ModelPiece* - 
//
gfx_ModelPiece* script_Proccess::SweetSpot()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "SweetSpot" );
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( 0 );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it
	if( NewThread.Start!=0xFFFFFFFF )
		ExecuteThread( NewThread );

	// Return the piece indexed by local0
	long Piece = NewThread.Locals[0];
	NewThread.Locals.clear();
	return pModel->Piece( pScript->ResolvePiece(Piece) );
}
// End script_Proccess::SweetSpot()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::AimFrom() //                   \author Logan Jones
//////////////////////////////                          \date 5/4/2002
//               
//====================================================================
// Parameters:
//  const int iWeapon    - 
//  long& lPiece         - 
//
// Return: long - 
//
long script_Proccess::AimFrom( const int iWeapon, long& lPiece )
{
	Thread_t	NewThread;
	long		lResult = 0;

	// Get the module
	switch( iWeapon )
	{
		case 0: NewThread.Start = pScript->GetModule( "AimFromPrimary" ); break;
		case 1: NewThread.Start = pScript->GetModule( "AimFromSecondary" ); break;
		case 2: NewThread.Start = pScript->GetModule( "AimFromTertiary" ); break;
	}

	// Initialize the new thread
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( lPiece );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it and return the result as well as the piece index
	if( NewThread.Start!=0xFFFFFFFF )
	{
		//lResult = QuickExecute( NewThread );
		lPiece = pScript->ResolvePiece( NewThread.Locals[0] );
	}
	return lResult;
}
// End script_Proccess::AimFrom()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::AimWeapon() //                 \author Logan Jones
/////////////////////////////////                       \date 5/5/2002
//               
//====================================================================
// Parameters:
//  const int iWeapon - 
//  float fHeading    - 
//  float fPitch      - 
//  BOOL* pDoneAiming - 
//
// Return: long - 
//
long script_Proccess::AimWeapon( const int iWeapon, long lHeading, long lPitch, BOOL* pDoneAiming )
{
	Thread_t	NewThread;
	long		lResult = 0;

	// Get the module
	switch( iWeapon )
	{
		case 0: NewThread.Start = pScript->GetModule( "AimPrimary" ); break;
		case 1: NewThread.Start = pScript->GetModule( "AimSecondary" ); break;
		case 2: NewThread.Start = pScript->GetModule( "AimTertiary" ); break;
	}

	// Initialize the new thread
	NewThread.Offset = NewThread.Start + 2;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
//	NewThread.Locals.push_back( long((fHeading * (180.0f / fPI)) * ANGULAR_CONSTANT) );
//	NewThread.Locals.push_back( long((fPitch * (180.0f / fPI)) * ANGULAR_CONSTANT)  );
	NewThread.Locals.push_back( lHeading );
	NewThread.Locals.push_back( lPitch  );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = pDoneAiming;

	// If the module is valid, execute it and return the result as well as the piece index
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
	return lResult;
}
// End script_Proccess::AimWeapon()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::QueryWeapon() //               \author Logan Jones
///////////////////////////////////                     \date 5/5/2002
//               
//====================================================================
// Parameters:
//  const int iWeapon - 
//  long& lPiece      - 
//
// Return: long - 
//
long script_Proccess::QueryWeapon( const int iWeapon, long& lPiece )
{
	Thread_t	NewThread;
	long		lResult = 0;

	// Get the module
	switch( iWeapon )
	{
		case 0: NewThread.Start = pScript->GetModule( "QueryPrimary" ); break;
		case 1: NewThread.Start = pScript->GetModule( "QuerySecondary" ); break;
		case 2: NewThread.Start = pScript->GetModule( "QueryTertiary" ); break;
	}

	// Initialize the new thread
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( lPiece );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it and return the result as well as the piece index
	if( NewThread.Start!=0xFFFFFFFF )
	{
		//lResult = QuickExecute( NewThread );
		lPiece = pScript->ResolvePiece( NewThread.Locals[0] );
	}
	return lResult;
}
// End script_Proccess::QueryWeapon()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::FireWeapon() //                \author Logan Jones
//////////////////////////////////                      \date 5/5/2002
//               
//====================================================================
// Parameters:
//  const int iWeapon - 
//
// Return: long - 
//
long script_Proccess::FireWeapon( const int iWeapon )
{
	Thread_t	NewThread;
	long		lResult = 0;

	// Get the module
	switch( iWeapon )
	{
		case 0: NewThread.Start = pScript->GetModule( "FirePrimary" ); break;
		case 1: NewThread.Start = pScript->GetModule( "FireSecondary" ); break;
		case 2: NewThread.Start = pScript->GetModule( "FireTertiary" ); break;
	}

	// Initialize the new thread
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
	return lResult;
}
// End script_Proccess::FireWeapon()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::QueryNanoPiece() //            \author Logan Jones
//////////////////////////////////////                  \date 5/5/2002
//               
//====================================================================
// Return: gfx_ModelPiece* - 
//
gfx_ModelPiece* script_Proccess::QueryNanoPiece()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "QueryNanoPiece" );
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( 0 );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it
	if( NewThread.Start!=0xFFFFFFFF )
		ExecuteThread( NewThread );

	// Return the piece indexed by local0
	long Piece = NewThread.Locals[0];
	NewThread.Locals.clear();
	return pModel->Piece( pScript->ResolvePiece(Piece) );
}
// End script_Proccess::QueryNanoPiece()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::QueryBuildInfo() //            \author Logan Jones
//////////////////////////////////////                  \date 5/5/2002
//               
//====================================================================
// Return: gfx_ModelPiece* - 
//
gfx_ModelPiece* script_Proccess::QueryBuildInfo()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "QueryBuildInfo" );
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( 0 );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it
	if( NewThread.Start!=0xFFFFFFFF )
		ExecuteThread( NewThread );

	// Return the piece indexed by local0
	long Piece = NewThread.Locals[0];
	NewThread.Locals.clear();
	return pModel->Piece( pScript->ResolvePiece(Piece) );
}
// End script_Proccess::QueryBuildInfo()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::StartBuilding() //             \author Logan Jones
/////////////////////////////////////                   \date 5/5/2002
//               
//====================================================================
// Parameters:
//  float fHeading - 
//  float fPitch   - 
//
void script_Proccess::StartBuilding( float fHeading, float fPitch )
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "StartBuilding" );
	NewThread.Offset = NewThread.Start;
	if( pScript->pStart[NewThread.Offset]==Opcode_StackAllocate ) {
		NewThread.Locals.push_back( long(fHeading * ANGULAR_CONSTANT) );
		++NewThread.Offset;
		if( pScript->pStart[NewThread.Offset]==Opcode_StackAllocate )
			NewThread.Locals.push_back( long(fPitch * ANGULAR_CONSTANT) ),
			++NewThread.Offset;
	}
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
	NewThread.Locals.clear();
}
// End script_Proccess::StartBuilding()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::StopBuilding() //              \author Logan Jones
////////////////////////////////////                    \date 5/5/2002
//               
//====================================================================
//
void script_Proccess::StopBuilding()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "StopBuilding" );
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
}
// End script_Proccess::StopBuilding()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::Activate() //                  \author Logan Jones
////////////////////////////////                        \date 5/5/2002
//               
//====================================================================
//
void script_Proccess::Activate()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "Activate" );
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
}
// End script_Proccess::Activate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::Deactivate() //                \author Logan Jones
//////////////////////////////////                      \date 5/5/2002
//               
//====================================================================
//
void script_Proccess::Deactivate()
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "Deactivate" );
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, add the new thread to the list
	if( NewThread.Start!=0xFFFFFFFF )
		Threads.push_back( NewThread );
}
// End script_Proccess::Deactivate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::Killed() //                    \author Logan Jones
//////////////////////////////                          \date 5/5/2002
//               
//====================================================================
// Parameters:
//  long lSeverity    - 
//  long& lCorpseType - 
//
// Return: long - 
//
void script_Proccess::Killed( long lSeverity, long& lCorpseType )
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "Killed" );
	NewThread.Offset = NewThread.Start + 2;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( lSeverity );
	NewThread.Locals.push_back( lCorpseType );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it and return the result as well as the corpse type
	if( NewThread.Start!=0xFFFFFFFF )
	{
		//lResult = QuickExecute( NewThread );
		lCorpseType = NewThread.Locals[1];
	}
}
// End script_Proccess::Killed()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// script_Proccess::SetSpeed() //                  \author Logan Jones
////////////////////////////////                     \date 02-28-2003
//
//===================================================================
// Parameters:
//  float fSpeed - 
//
void script_Proccess::SetSpeed( float fSpeed )
{
	// Initialize the new thread
	NewThread.Start = pScript->GetModule( "SetSpeed" );
	NewThread.Offset = NewThread.Start + 1;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.push_back( long(fSpeed * ANGULAR_CONSTANT) );
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// If the module is valid, execute it
	if( NewThread.Start!=0xFFFFFFFF )
		ExecuteThread( NewThread );
	NewThread.Locals.clear();
}
// End script_Proccess::SetSpeed()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Functions.cpp //
///////////////////////////////