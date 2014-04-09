// unit_Job.cpp //                                 \author Logan Jones
/////////////////                                    \date 02-20-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "player.h"

/////////////////////////////////////////////////////////////////////
// In() //                                         \author Logan Jones
/////////                                            \date 02-12-2003
//
//===================================================================
// Parameters:
//  long A - Superset
//  long B - Supposed subset
//
// Return: bool - returns true if B is a subset of A
//
bool Subset( long A, long B )
{
	//return ((A|B) & (A^(~B))) == B;
	return (~A & B) == 0;
}
// End In()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Job::Update() //                           \author Logan Jones
///////////////////////                              \date 02-20-2003
//
//===================================================================
// Parameters:
//  long lSurplus        - 
//  game_Player* pPlayer - 
//
// Return: bool - 
//
bool unit_Job::Update( long lSurplus, game_Player* pPlayer )
{
	switch( Type ) {

		case job_Project:
			if( Subset(lSurplus,Requirements) ) {
				pPlayer->Consume( Metal, MetalCost ),
				pPlayer->Consume( Energy, EnergyCost );
				Project->AppliedTime += Worker->m_pUnitType->WorkerTime;
				Active = Project->Target!=NULL;

				if( Project->AppliedTime>=Project->TargetType->BuildTime ) {
					Worker->JobComplete( this );
					return false;
				}
			} else Active = false;
			return true;

		case job_Economy:
			if( Flags & job_UseWind )
				if( Flags & job_WindInit )
					EnergyIncome += 5,
					Flags &= ~job_WindInit;
				else EnergyIncome += 5;
			if( Worker->Active() && Subset(lSurplus,Requirements) )
				pPlayer->Consume( Metal, MetalCost ),
				pPlayer->Consume( Energy, EnergyCost ),
				pPlayer->Produce( Metal, MetalIncome ),
				pPlayer->Produce( Energy, EnergyIncome ),
				Active = true;
			else Active = false;
			return true;

		default: return false;
	} // end switch( Type )
}
// End unit_Job::Update()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Job.cpp //
///////////////////////