// unit_Object_Handlers.cpp //                     \author Logan Jones
/////////////////////////////                        \date 10/18/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "player.h"
#include "snd.h"


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestStop() //                 \author Logan Jones
/////////////////////////////////                    \date 10/18/2002
//
//===================================================================
//
void unit_Object::OnRequestStop()
{
/*	// NOTE: Old code; to be removed	
		// Clear the queue
		m_OrderQueue.clear();

		// Stop everything
		StopMoving();
		m_CurrentTask = TASK_Nothing;
	// END NOTE
*/
	// Stop everything
//	StopMoving();

	m_Orders.Stop();
}
// End unit_Object::OnRequestStop()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestMoveTo() //               \author Logan Jones
///////////////////////////////////                  \date 10/18/2002
//
//===================================================================
// Parameters:
//  const std_Point& ptWhere - 
//  bool bEnqueue            - 
//
void unit_Object::OnRequestMoveTo( const std_Point& ptWhere, bool bEnqueue )
{
	// Add the task
	if( MakeMeDynamic() )
		m_Script->RunModule( "StartMoving" );
//	if( !m_MoveInfo.IsMoving ) m_Script->RunModule( "StartMoving" );
//	m_MoveInfo.IsMoving |= unit_MoveInfo::MI_MOVING;

/*
	// NOTE: Old code; to be removed
		// Clear the queue if desired
		if( !bEnqueue ) m_OrderQueue.clear();

		// Add a new order and get a pointer to it
		m_OrderQueue.push_back( Order_t() );
		Order_t* pOrder = &m_OrderQueue.back();

		// Fill out the order
		pOrder->Type = Order_t::Move;
		pOrder->Waypoints.clear();
		if( !bEnqueue ) m_CurrentTask = TASK_Pathing;

		// Find a path
		if( theGame.Units.FindPath(m_BaseRect.Position() / 16, ptWhere / 16, 0, this, &pOrder->Waypoints)==unit_Factory::PATH_FOUND )
			SetPath();
*/

		sound.PlaySound( m_pUnitType->pSounds->Ok1 );
	// END NOTE

	m_Orders.MoveTo( ptWhere, NULL, bEnqueue );
}
// End unit_Object::OnRequestMoveTo()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestBuild() //                \author Logan Jones
//////////////////////////////////                   \date 10/18/2002
//
//===================================================================
// Parameters:
//  const unit_Type* pType   - 
//  const std_Point& ptWhere - 
//  bool bEnqueue            - 
//
void unit_Object::OnRequestBuild( const unit_Type* pType, const std_Point& ptWhere, bool bEnqueue )
{
	// NOTE: Old code; to be removed
/*		const unit_Type* pWhat = theGame.Units.GetUnitType(dwType);
		const float DistanceSqr = (m_BaseRect.Center() - (ptWhere + std_Point(pWhat->FootPrint.width*8,pWhat->FootPrint.height*8))).MagnitudeSquared();

		// Clear the queue if desired
		if( !bEnqueue ) m_OrderQueue.clear();

		// Add a new order and get a pointer to it
		m_OrderQueue.push_back( Order_t() );
		Order_t* pOrder = &m_OrderQueue.back();

		// Fill out the order
		pOrder->Type = Order_t::Build;
		pOrder->Waypoints.clear();
		pOrder->Goal = ptWhere;
		pOrder->Specifics.pUnitType = pWhat;
		if( !bEnqueue ) m_CurrentTask = TASK_Pathing;

		// Find a path if the buildee is beyond our range
		if( DistanceSqr > sqr(m_pUnitType->BuildDistance) )
		{
			if( theGame.Units.FindPath(m_BaseRect.Position() / 16, ptWhere / 16, m_pUnitType->BuildDistance / 16.0f, this, &pOrder->Waypoints)==unit_Factory::PATH_FOUND )
				SetPath();
			sound.PlaySound( m_pUnitType->pSounds->Ok1 );
		}
*/	// END NOTE

	// TEMP
		//if( m_pUnitType->Behaviour==unit_Type::Structure )
		//	theGame.Units.SpawnUnit( pType, m_BaseRect.Center(), m_pPlayer );
		//else theGame.Units.SpawnUnit( pType, ptWhere, m_pPlayer );
		NotifyPrepareToBuild( pType, ptWhere );
	// END TEMP

	m_Orders.Build( pType, ptWhere, NULL, bEnqueue );
}
// End unit_Object::OnRequestBuild()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestActivate() //             \author Logan Jones
/////////////////////////////////////                \date 10/18/2002
//
//===================================================================
// Parameters:
//  bool bActivate - 
//
void unit_Object::OnRequestActivate( bool bActivate )
{
	// NOTE: Old code; to be removed
		if( bActivate && !m_Activation )
		{
			sound.PlaySound( m_pUnitType->pSounds->Activate );
			Activate();
		}
		else if( !bActivate && m_Activation )
		{
			sound.PlaySound( m_pUnitType->pSounds->Deactivate );
			Deactivate();
		}
	// END NOTE
}
// End unit_Object::OnRequestActivate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestAttack() //               \author Logan Jones
///////////////////////////////////                  \date 01-31-2003
//
//===================================================================
// Parameters:
//  const std_Point& ptWhere - 
//  bool bEnqueue            - 
//
void unit_Object::OnRequestAttack( const std_Point& ptWhere, bool bEnqueue )
{
}
// End unit_Object::OnRequestAttack()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnRequestUnit() //                 \author Logan Jones
/////////////////////////////////                    \date 02-18-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType - 
//
void unit_Object::OnRequestUnit( const unit_Type* pType )
{
//	if( m_ReadyToBuild ) NotifySpawnNewUnit( pType, std_Point(0,0) );//m_BaseRect.Center() );
/*	else*/ NotifyPrepareToBuild( pType, std_Point(0,0) );//)m_BaseRect.Center() );
}
// End unit_Object::OnRequestUnit()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnNotifyPrepareToBuild() //        \author Logan Jones
//////////////////////////////////////////           \date 02-16-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType   - 
//  const std_Point& ptWhere - 
//
void unit_Object::OnNotifyPrepareToBuild( const unit_Type* pType, const std_Point& ptWhere )
{
	// Initiate the job+project
	m_PrimaryJob = m_pPlayer->NewJob( this, job_Project );
	m_PrimaryJob->Project = new unit_Project( pType, ptWhere );

	// Have the script get ready to build (if it isn't already)
	if( m_ReadyToBuild )
		NotifySpawnNewUnit( pType, ptWhere );
	else if( m_pUnitType->Behaviour==unit_Type::Structure )
		m_Script->Activate();
	else {
		std_Vector2 Pos = m_BaseRect.Center();
		std_Vector2 Where = ptWhere + std_Point(pType->FootPrint.width*8,pType->FootPrint.height*8);
		std_Vector2 Dir = math_Normalize(Where - Pos); 
		float Dot = m_Direction * Dir;
		float Angle = acosf(Dot) * (180.0f/fPI);
		m_Script->StartBuilding( (Dir.x<0) ? -Angle : Angle,0);
	}
}
// End unit_Object::OnNotifyPrepareToBuild()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnNotifySpawnNewUnit() //          \author Logan Jones
////////////////////////////////////////             \date 02-16-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType   - 
//  const std_Point& ptWhere - 
//
void unit_Object::OnNotifySpawnNewUnit( const unit_Type* pType, const std_Point& ptWhere )
{
	// Fill out the job
	m_PrimaryJob->Active = true;
	m_PrimaryJob->Flags = job_Originator;
	m_PrimaryJob->MetalCost = pType->BuildCostMetal * m_pUnitType->WorkerTime / pType->BuildTime;
	m_PrimaryJob->EnergyCost= pType->BuildCostEnergy* m_pUnitType->WorkerTime / pType->BuildTime;
	m_PrimaryJob->Requirements = (pType->BuildCostMetal>0 ? (1<<Metal):0) | (pType->BuildCostEnergy>0 ? (1<<Energy):0);
	m_pPlayer->Update( Metal, Consumption, m_PrimaryJob->MetalCost );
	m_pPlayer->Update( Energy,Consumption, m_PrimaryJob->EnergyCost);

	// Let the script do something
	if( m_pUnitType->Behaviour==unit_Type::Structure )
		m_Script->StartBuilding( 0, 0 );

	// Spawn the unit
	if( ptWhere.IsNull() )
		m_PrimaryJob->Project->Target = m_Manager.SpawnUnit( pType, m_BaseRect.Center() + m_Script->QueryBuildInfo()->Position(), m_pPlayer );
	else m_PrimaryJob->Project->Target = m_Manager.SpawnUnit( pType, ptWhere, m_pPlayer );

	m_PrimaryJob->Project->Target->m_AttachedProject = m_PrimaryJob->Project;
}
// End unit_Object::OnNotifySpawnNewUnit()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Object_Handlers.cpp //
///////////////////////////////////