// script_Proccess_Handlers.cpp //                 \author Logan Jones
/////////////////////////////////                      \date 9/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "script.h"
#include "script_Proccess.h"
#include "game.h"
#include "ta_COB.h"
script_Proccess::OpcodeHandlerMap_t	script_Proccess::OpcodeHandlers;


//////////////////////////////////////////////////////////////////////
// script_Proccess::AssociateOpcodeHandlers() //   \author Logan Jones
///////////////////////////////////////////////         \date 9/1/2002
//               
//====================================================================
// Return: bool - 
//
bool script_Proccess::AssociateOpcodeHandlers()
{
	OpcodeHandlers = new LPOpcodeHandler_t[ Opcode_DropUnit - Opcode_MovePieceWithSpeed + 1 ];

	// Add an entry for each handler from the instructions defined in ta_COB_Instructions.h
	#define INSTRUCTION_MACRO		ASSOCCIATE_INSTRUCTION_TO_OPCODE_HANDLER
	#include "ta_COB_Instructions.h"
	#undef INSTRUCTION_MACRO

	return true;
}
// End script_Proccess::AssociateOpcodeHandlers()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::DestroyOpcodeHandlers() //     \author Logan Jones
/////////////////////////////////////////////           \date 9/1/2002
//               
//====================================================================
//
void script_Proccess::DestroyOpcodeHandlers()
{
//	OpcodeHandlers.clear();
	delete [] OpcodeHandlers;
}
// End script_Proccess::DestroyOpcodeHandlers()
//////////////////////////////////////////////////////////////////////


// Execution helper macros
#define SLEEP_TIME( TIME )		( (std_Time_t)((TIME) * (1/1.5f)) )
#define PopStack()				Thread.Stack.back(); Thread.Stack.pop_back()
#define IMPLEMENT_BINARY_OPPERATION( NAME, OPERATOR ) \
	IMPLEMENT_OPCODE_HANDLER( NAME ) \
	{ \
		long Right = Thread.Stack.back(); \
		Thread.Stack.pop_back(); \
		Thread.Stack.back() = Thread.Stack.back() OPERATOR Right; \
		++Thread.pCode; \
		return 0; \
	}
#define IMPLEMENT_UNARY_OPPERATION( NAME, OPERATOR ) \
	IMPLEMENT_OPCODE_HANDLER( NAME ) \
	{ \
		Thread.Stack.back() = OPERATOR Thread.Stack.back(); \
		++Thread.pCode; \
		return 0; \
	}


//////////////////////////////////////////////////////////////////////
// MovePieceWithSpeed //                           \author Logan Jones
///////////////////////                                 \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( MovePieceWithSpeed )
{
	float			Dest = Thread.Stack.back() / LINEAR_CONSTANT; Thread.Stack.pop_back();
	float			Speed= Thread.Stack.back() / LINEAR_CONSTANT; Thread.Stack.pop_back();
	Animation_t&	Anim = Proccess.Animations[ MOVE_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]) ];

	Anim.Type = animation_Move;
	Anim.Piece= Thread.pCode[1];
	Anim.Axis = Thread.pCode[2];
	Anim.vCurrent.x = Proccess.pModel->PieceOffset( Anim.Piece, Anim.Axis );
	Anim.Velocity= ((Dest>Anim.vCurrent.x) ? Speed : -Speed) / 1000.0f;
	Anim.Desired = Dest;
	Anim.Last = std_Time();

	Thread.pCode += 3;
	Proccess.pUnit->StartAnimation();
	return 0;
}
// End MovePieceWithSpeed
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// TurnPieceWithSpeed //                           \author Logan Jones
///////////////////////                                 \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( TurnPieceWithSpeed )
{
	float			Dest = Thread.Stack.back() / ANGULAR_CONSTANT; Thread.Stack.pop_back();
	float			Speed= Thread.Stack.back() / ANGULAR_CONSTANT; Thread.Stack.pop_back();
	Animation_t&	Anim = Proccess.Animations[ TURN_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]) ];

	Anim.Type = animation_Turn;
	Anim.Piece= Thread.pCode[1];
	Anim.Axis = Thread.pCode[2];
	Anim.vCurrent.SetPolar( Proccess.pModel->PieceAngle(Anim.Piece,Anim.Axis) * (fPI/180.0f), 1 );
	Anim.vDesired.SetPolar( Dest * (fPI/180.0f), 1 );
	Anim.Velocity = Speed/*((math_Determinant(Anim.vCurrent,Anim.vDesired)>=0) ? Speed : -Speed)*/ * (fPI/180.0f) / 1000.0f;
	Anim.Desired = Dest;
	Anim.Last = std_Time();

	Thread.pCode += 3;
	Proccess.pUnit->StartAnimation();
	return 0;
}
// End TurnPieceWithSpeed
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// StartSpin //                                    \author Logan Jones
//////////////                                          \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( StartSpin )
{
	float			Speed= Thread.Stack.back() / ANGULAR_CONSTANT; Thread.Stack.pop_back();
	float			Accel= Thread.Stack.back() / ANGULAR_CONSTANT; Thread.Stack.pop_back();
	Animation_t&	Anim = Proccess.Animations[ SPIN_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]) ];

	Anim.Piece= Thread.pCode[1];
	Anim.Axis = Thread.pCode[2];
	Anim.vCurrent.SetPolar( Proccess.pModel->PieceAngle(Anim.Piece,Anim.Axis) * (fPI/180.0f), 1 );
	if( Accel )
		Anim.Type = animation_StartSpin,
		Anim.Velocity = 0,
		Anim.Desired = Speed * (fPI/180.0f) / 1000.0f,
		Anim.vDesired.x = Accel * (fPI/180.0f) / 1000.0f;
	else
		Anim.Type = animation_Spin,
		Anim.Velocity = Speed * (fPI/180.0f) / 1000.0f;
	Anim.Last = std_Time();

	Thread.pCode += 3;
	Proccess.pUnit->StartAnimation();
	return 0;
}
// End StartSpin
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// StopSpin //                                     \author Logan Jones
/////////////                                           \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( StopSpin )
{
	float				Deccel= Thread.Stack.back() / ANGULAR_CONSTANT; Thread.Stack.pop_back();
	AnimMap_t::iterator	it = Proccess.Animations.find( SPIN_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]) );

	if( it!=Proccess.Animations.end() )
    {
		if( Deccel==0 )
			Proccess.Animations.erase( it );
		else
			((*it).second).Type = animation_StopSpin,
			((*it).second).vDesired.x = Deccel * (fPI/180.0f) / 1000.0f,
			((*it).second).Last = std_Time();
    }

	Thread.pCode += 3;
	Proccess.pUnit->StartAnimation();
	return 0;
}
// End StopSpin
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ShowPiece //                                    \author Logan Jones
//////////////                                          \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( ShowPiece )
{
	Proccess.pModel->ClearPieceState( Thread.pCode[1], piece_Hide );
	Thread.pCode += 2;
	Proccess.StateChanged = true;
	return 0;
}
// End ShowPiece
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// HidePiece //                                    \author Logan Jones
//////////////                                          \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( HidePiece )
{
	Proccess.pModel->SetPieceState( Thread.pCode[1], piece_Hide );
	Thread.pCode += 2;
	Proccess.StateChanged = true;
	return 0;
}
// End HidePiece
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CachePiece //                                   \author Logan Jones
///////////////                                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( CachePiece )
{
	Proccess.pModel->SetPieceState( Thread.pCode[1], piece_Cache );
	Thread.pCode += 2;
	return 0;
}
// End CachePiece
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// DontCachePiece //                               \author Logan Jones
///////////////////                                     \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( DontCachePiece )
{
	Proccess.pModel->ClearPieceState( Thread.pCode[1], piece_Cache );
	Thread.pCode += 2;
	return 0;
}
// End DontCachePiece
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// DontShadow //                                   \author Logan Jones
///////////////                                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( DontShadow )
{
	Proccess.pModel->SetPieceState( Thread.pCode[1], piece_DontShadow );
	Thread.pCode += 2;
	return 0;
}
// End DontShadow
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// MovePieceNow //                                 \author Logan Jones
/////////////////                                       \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( MovePieceNow )
{
	long Position = PopStack();
	Proccess.pModel->SetPieceOffset( Thread.pCode[1], Thread.pCode[2], Position / LINEAR_CONSTANT );
	Thread.pCode += 3;
	Proccess.HasMoved = true;
	return 0;
}
// End MovePieceNow
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// TurnPieceNow //                                 \author Logan Jones
/////////////////                                       \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( TurnPieceNow )
{
	long Position = PopStack();
	Proccess.pModel->SetPieceAngle( Thread.pCode[1], Thread.pCode[2], Position / ANGULAR_CONSTANT );
	Thread.pCode += 3;
	Proccess.HasMoved = true;
	return 0;
}
// End TurnPieceNow
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// DontShade //                                    \author Logan Jones
//////////////                                          \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( DontShade )
{
	Proccess.pModel->SetPieceState( Thread.pCode[1], piece_DontShade );
	Thread.pCode += 2;
	return 0;
}
// End DontShade
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// EmitSfx //                                      \author Logan Jones
////////////                                            \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( EmitSfx )
{
	// TODO: Implement SFX system
	__unused long SFX = PopStack();
	Thread.pCode += 2;
	return 0;
}
// End EmitSfx
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// WaitForTurn //                                  \author Logan Jones
////////////////                                        \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( WaitForTurn )
{
	if( Proccess.Animations.find(TURN_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]))!=Proccess.Animations.end() )
		return 1;
	Thread.pCode += 3;
	return 0;
}
// End WaitForTurn
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// WaitForMove //                                  \author Logan Jones
////////////////                                        \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( WaitForMove )
{
	if( Proccess.Animations.find(MOVE_ANIM_INDEX(Thread.pCode[1],Thread.pCode[2]))!=Proccess.Animations.end() )
		return 1;
	Thread.pCode += 3;
	return 0;
}
// End WaitForMove
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Sleep //                                        \author Logan Jones
//////////                                              \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( Sleep )
{
	Thread.Sleep = std_Time() + SLEEP_TIME(Thread.Stack.back());
	Thread.Stack.pop_back();
	Thread.pCode += 1;
	Proccess.ReSync();
	return 1;
}
// End Sleep
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PushConstantOntoStack //                        \author Logan Jones
//////////////////////////                              \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( PushConstantOntoStack )
{
	Thread.Stack.push_back( Thread.pCode[1] );
	Thread.pCode += 2;
	return 0;
}
// End PushConstantOntoStack
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PushLocalVariableOntoStack //                   \author Logan Jones
///////////////////////////////                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( PushLocalVariableOntoStack )
{
	Thread.Stack.push_back( Thread.Locals[Thread.pCode[1]] );
	Thread.pCode += 2;
	return 0;
}
// End PushLocalVariableOntoStack
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PushStaticVariableOntoStack //                  \author Logan Jones
////////////////////////////////                        \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( PushStaticVariableOntoStack )
{
	Thread.Stack.push_back( Proccess.Statics[Thread.pCode[1]] );
	Thread.pCode += 2;
	return 0;
}
// End PushStaticVariableOntoStack
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// StackAllocate //                                \author Logan Jones
//////////////////                                      \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( StackAllocate )
{
	Thread.Locals.push_back( 0 );
	Thread.pCode += 1;
	return 0;
}
// End StackAllocate
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// SetLocalVariable //                             \author Logan Jones
/////////////////////                                   \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( SetLocalVariable )
{
	Thread.Locals[Thread.pCode[1]] = PopStack();
	Thread.pCode += 2;			
	return 0;
}
// End SetLocalVariable
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// SetStaticVariable //                            \author Logan Jones
//////////////////////                                  \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( SetStaticVariable )
{
	Proccess.Statics[Thread.pCode[1]] = PopStack();
	Thread.pCode += 2;
	return 0;
}
// End SetStaticVariable
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PopStack //                                     \author Logan Jones
/////////////                                           \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( PopStack )
{
	Thread.Stack.pop_back();
	Thread.pCode += 1;
	return 0;
}
// End PopStack
//////////////////////////////////////////////////////////////////////


IMPLEMENT_BINARY_OPPERATION( Add, + )
IMPLEMENT_BINARY_OPPERATION( Subtract, - )
IMPLEMENT_BINARY_OPPERATION( Multiply, * )
IMPLEMENT_BINARY_OPPERATION( Divide, / )
IMPLEMENT_BINARY_OPPERATION( BitwiseAnd, & )
IMPLEMENT_BINARY_OPPERATION( BitwiseOr, | )
IMPLEMENT_BINARY_OPPERATION( Unknown1, + )
IMPLEMENT_BINARY_OPPERATION( Unknown2, + )
IMPLEMENT_BINARY_OPPERATION( Unknown3, + )
IMPLEMENT_BINARY_OPPERATION( LessThan, < )
IMPLEMENT_BINARY_OPPERATION( LessThanOrEqual, <= )
IMPLEMENT_BINARY_OPPERATION( GreaterThan, > )
IMPLEMENT_BINARY_OPPERATION( GreaterThanOrEqual, >= )
IMPLEMENT_BINARY_OPPERATION( Equal, == )
IMPLEMENT_BINARY_OPPERATION( NotEqual, != )
IMPLEMENT_BINARY_OPPERATION( And, && )
IMPLEMENT_BINARY_OPPERATION( Or, || )
IMPLEMENT_UNARY_OPPERATION( Not, ! )


//////////////////////////////////////////////////////////////////////
// Random //                                       \author Logan Jones
///////////                                             \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( Random )
{
	// TODO: Implement better random function
	long Max = PopStack();
	long Min = Thread.Stack.back();
	Thread.Stack.back() = Min + (rand() % ((Max+1) - Min));
	Thread.pCode += 1;
	return 0;
}
// End Random
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// GetUnitValue //                                 \author Logan Jones
/////////////////                                       \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( GetUnitValue )
{
	Thread.Stack.back() = Proccess.pUnit->GetUnitValue( Thread.Stack.back() );
	Thread.pCode += 1;
	return 0;
}
// End GetUnitValue
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// GetFunctionResult //                            \author Logan Jones
//////////////////////                                  \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( GetFunctionResult )
{
	// TODO: Implement GetFunctionResult opcode handler
	Thread.pCode += 1;
	return 0;
}
// End GetFunctionResult
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// StartScript //                                  \author Logan Jones
////////////////                                        \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( StartScript )
{
	Thread.Offset = (UINT32)(Thread.pCode - Proccess.pScript->pStart);
	Proccess.StartScript( Thread.pCode[1], Thread );
	Thread.pCode += 3;
	return 0;
}
// End StartScript
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CallScript //                                   \author Logan Jones
///////////////                                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( CallScript )
{
	Thread.Offset = (UINT32)(Thread.pCode - Proccess.pScript->pStart);
	Proccess.CallScript( Thread.pCode[1], Thread );
	Thread.pCode += 3;
	return 1;
}
// End CallScript
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// JumpToOffset //                                 \author Logan Jones
/////////////////                                       \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( JumpToOffset )
{
	Thread.pCode = Proccess.pScript->pStart + Thread.pCode[1];
	return 0;
}
// End JumpToOffset
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Return //                                       \author Logan Jones
///////////                                             \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( Return )
{
	if( Thread.pParent ) Thread.pParent->Sleep = 0;
	return -1;
}
// End Return
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// JumpToOffsetIfFalse //                          \author Logan Jones
////////////////////////                                \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( JumpToOffsetIfFalse )
{
	if( Thread.Stack.back() ) Thread.pCode += 2;
	else Thread.pCode = Proccess.pScript->pStart + Thread.pCode[1];
	Thread.Stack.pop_back();
	return 0;
}
// End JumpToOffsetIfFalse
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Signal //                                       \author Logan Jones
///////////                                             \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( Signal )
{
	long KillSignal = PopStack();
	ThreadList_t::iterator			it = Proccess.Threads.begin();
	ThreadList_t::const_iterator	end= Proccess.Threads.end();
	for(; it!=end; )
	{
		if( ((*it).Start!=Thread.Start) && ((*it).Mask & KillSignal) )
			it = Proccess.Threads.erase( it );
		else ++it;
	}
	if( Thread.Mask & KillSignal )
		return -1;
	Thread.pCode += 1;
	return 0;
}
// End Signal
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// SetSignalMask //                                \author Logan Jones
//////////////////                                      \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( SetSignalMask )
{
	Thread.Mask = (DWORD)PopStack();
	Thread.pCode += 1;
	return 0;
}
// End SetSignalMask
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Explode //                                      \author Logan Jones
////////////                                            \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( Explode )
{
	// TODO: Implement explode opcode handler
	Thread.Stack.pop_back();
	Thread.pCode += 2;
	return 0;
}
// End Explode
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// PlaySound //                                    \author Logan Jones
//////////////                                          \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( PlaySound )
{
	// TODO: Implement PlaySound opcode handler
	Thread.pCode += 2;
	return 0;
}
// End PlaySound
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// MapCommand //                                   \author Logan Jones
///////////////                                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( MapCommand )
{
	// TODO: Implement MapCommand opcode handler
	Thread.pCode += 3;
	return 0;
}
// End MapCommand
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// SetUnitValue //                                 \author Logan Jones
/////////////////                                       \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( SetUnitValue )
{
	long	Value = PopStack();
	Proccess.pUnit->SetUnitValue( Thread.Stack.back(), Value );
	Thread.Stack.pop_back();
	Thread.pCode += 1;
	return 0;
}
// End SetUnitValue
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// AttachUnit //                                   \author Logan Jones
///////////////                                         \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( AttachUnit )
{
	// TODO: Implement AttachUnit opcode handler
	Thread.pCode += 1;
	return 0;
}
// End AttachUnit
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// DropUnit //                                     \author Logan Jones
/////////////                                           \date 9/1/2002
//               
//====================================================================
// Parameters:
//  script_Proccess& Proccess - 
//  Thread_t& Thread
//
// Return: long - 
//
IMPLEMENT_OPCODE_HANDLER( DropUnit )
{
	// TODO: Implement DropUnit opcode handler
	Thread.pCode += 1;
	return 0;
}
// End DropUnit
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Proccess_Handlers.cpp //
///////////////////////////////////////