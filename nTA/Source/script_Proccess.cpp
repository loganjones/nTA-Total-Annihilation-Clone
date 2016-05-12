// script_Proccess.cpp //                          \author Logan Jones
////////////////////////                               \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "script.h"
#include "script_Proccess.h"
#include "game.h"
#include "ta_COB.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "script_Proccess.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
script_Proccess::script_Proccess():
	HasMoved( false ),
	StateChanged( false )
{}
script_Proccess::~script_Proccess()
{}
//
/////////////////////////////////////////////////////////////////////

script_Proccess::Thread_t		script_Proccess::NewThread;


// Execution helper macros
#define SLEEP_TIME( TIME )		( (std_Time_t)((TIME) * (1/1.5f)) )
#define PopStack( DEST )		(DEST) = Thread.Stack.back(); Thread.Stack.pop_back()
#define Operation( OPERATOR )	PopStack( B ); Thread.Stack.back() = Thread.Stack.back() OPERATOR B; Thread.Offset += 1; break


//////////////////////////////////////////////////////////////////////
// script_Proccess::ExecuteThread() //             \author Logan Jones
/////////////////////////////////////                   \date 9/2/2002
//               
//====================================================================
// Parameters:
//  Thread_t& Thread - 
//
// Return: DWORD - 
//
DWORD script_Proccess::ExecuteThread( Thread_t& Thread )
{
	//const UINT32*	pCode = pScript->pStart;
	long			Ret = 0;

	while( Ret==0 )
		Ret = OpcodeHandlers[ *Thread.pCode - Opcode_MovePieceWithSpeed ]( *this, Thread );

	return (Ret > 0);
}
// End script_Proccess::ExecuteThread()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::UpdateAnims() //               \author Logan Jones
///////////////////////////////////                    \date 4/18/2002
//               
//====================================================================
// Parameters:
//
void script_Proccess::UpdateAnims()
{
	AnimMap_t::iterator	it = Animations.begin(),
						end= Animations.end(),
						next;
	float				fRate;

	for( ; it!=end; it=next) {

		Animation_t&	Anim = (*it).second;
		next = it;
		++next;

		switch( Anim.Type ) {
			case animation_Move:
				Anim.vCurrent.x += Anim.Velocity * (std_Time() - Anim.Last);
				if( Anim.Velocity>0 )
					if( Anim.vCurrent.x>Anim.Desired )
						pModel->SetPieceOffset( Anim.Piece, Anim.Axis, Anim.Desired ),
						Animations.erase( it );
					else Anim.Last = std_Time(),
						pModel->SetPieceOffset( Anim.Piece, Anim.Axis, Anim.vCurrent.x );
				else
					if( Anim.vCurrent.x<Anim.Desired )
						pModel->SetPieceOffset( Anim.Piece, Anim.Axis, Anim.Desired ),
						Animations.erase( it );
					else Anim.Last = std_Time(),
						pModel->SetPieceOffset( Anim.Piece, Anim.Axis, Anim.vCurrent.x );
				break;

			case animation_Turn:
				fRate = Anim.Velocity * (std_Time() - Anim.Last);
				if( acosf(Anim.vCurrent*Anim.vDesired)<=fRate )
					pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.Desired ),
					Animations.erase( it );
				else {
					if( math_Determinant(Anim.vCurrent,Anim.vDesired)>=0 )
						Anim.vCurrent.Rotate( fRate );
					else Anim.vCurrent.Rotate( -fRate );
					pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.vCurrent.Angle() * (180.0f/fPI) ),
					Anim.Last = std_Time();
				}
				/*Anim.vCurrent.Rotate( Anim.Velocity * (std_Time() - Anim.Last) );
				if( (Anim.vCurrent*Anim.vDesired)>0.99f )
					pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.Desired ),
					Animations.erase( it );
				else
					pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.vCurrent.Angle() * (180.0f/fPI) ),
					Anim.Last = std_Time();*/
				break;

			case animation_StartSpin:
				Anim.Velocity += Anim.vDesired.x * (std_Time() - Anim.Last);
				if( Anim.Velocity>=Anim.Desired )
					Anim.Velocity = Anim.Desired,
					Anim.Type = animation_Spin;
			case animation_Spin:
				Anim.vCurrent.Rotate( Anim.Velocity * (std_Time() - Anim.Last) );
				pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.vCurrent.Angle() * (180.0f/fPI) );
				Anim.Last = std_Time();
				break;

			case animation_StopSpin:
				Anim.Velocity -= Anim.vDesired.x * (std_Time() - Anim.Last);
				if( Anim.Velocity<=0 ) Animations.erase( it );
				Anim.vCurrent.Rotate( Anim.Velocity * (std_Time() - Anim.Last) );
				pModel->SetPieceAngle( Anim.Piece, Anim.Axis, Anim.vCurrent.Angle() * (180.0f/fPI) );
				Anim.Last = std_Time();
				break;
		} // end switch( Anim.Type )

		HasMoved = true;
	} // end for( it )
}
// End script_Proccess::UpdateAnims()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::ReSync() //                    \author Logan Jones
//////////////////////////////                         \date 6/11/2002
//               
//====================================================================
//
void script_Proccess::ReSync()
{
	if( HasMoved )
		pModel->SynchronizeVertices(),
		HasMoved = false;
	if( StateChanged )
		pModel->SynchronizeStates(),
		StateChanged = false;
}
// End script_Proccess::ReSync()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Proccess.cpp //
//////////////////////////////
