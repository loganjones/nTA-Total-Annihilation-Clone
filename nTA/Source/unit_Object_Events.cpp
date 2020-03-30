// unit_Object_Events.cpp //                       \author Logan Jones
///////////////////////////                           \date 3/27/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "gfx.h"
#include "snd.h"
#include "player.h"


/////////////////////////////////////////////////////////////////////
// unit_Object::OnComplete() //                    \author Logan Jones
//////////////////////////////                       \date 02-14-2003
//
//===================================================================
// Parameters:
//   - 
//
void unit_Object::OnComplete()
{
	// Update the player's resource pool and unconditional production
	m_pPlayer->Update( Metal, Maximum, m_pUnitType->MetalStorage );
	m_pPlayer->Update( Energy, Maximum, m_pUnitType->EnergyStorage );
	m_pPlayer->Update( Metal, Production, m_pUnitType->MetalMake );
	m_pPlayer->Update( Energy, Production, m_pUnitType->EnergyMake );

	// Does this unit contribute to the economy
    if( m_pUnitType->MetalUse || m_pUnitType->EnergyUse || m_pUnitType->MakesMetal || m_pUnitType->MetalExtractionRate || m_pUnitType->WindGenerator ) {
        m_ProductionJob = m_pPlayer->NewJob( this, job_Economy );
        m_ProductionJob->MetalIncome = m_pUnitType->MetalUse<0 ? -m_pUnitType->MetalUse : 0;
        m_ProductionJob->EnergyIncome= m_pUnitType->EnergyUse<0? -m_pUnitType->EnergyUse: 0;
        m_ProductionJob->MetalCost = m_pUnitType->MetalUse>0 ? m_pUnitType->MetalUse : 0;
        m_ProductionJob->EnergyCost= m_pUnitType->EnergyUse>0? m_pUnitType->EnergyUse: 0;
        m_ProductionJob->Requirements = (m_pUnitType->MetalUse>0 ? (1<<Metal):0) | (m_pUnitType->EnergyUse>0 ? (1<<Energy):0);
        m_ProductionJob->Flags = m_pUnitType->WindGenerator ? job_UseWind|job_WindInit : 0;
		m_ProductionJob->MetalIncome += m_pUnitType->MakesMetal + m_pUnitType->MetalExtractionRate * 223 * 12;
    }
    
	// If the unit can extract metal then set the speed
    if( m_pUnitType->MetalExtractionRate ) {
		m_Script->SetSpeed( 223 );
    }

	// If the unit can activate, then activate according to the startup state
    if( m_pUnitType->Abilities & unit_Type::OnOffable ) {
        if( m_pUnitType->InitialActivationOrder ) { Activate(); }
        else { OnActivate(); Deactivate(); }
    }
    else { Activate(false); }

	m_Cloaked = m_pUnitType->InitialCloakOrder;
}
// End unit_Object::OnComplete()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnActivate() //                    \author Logan Jones
//////////////////////////////                       \date 03-03-2003
//
//===================================================================
//
void unit_Object::OnActivate()
{
	// Update economy consumption values
    if( m_ProductionJob ) {
        m_pPlayer->Update( Metal, Consumption, m_ProductionJob->MetalCost );
		m_pPlayer->Update( Energy, Consumption, m_ProductionJob->EnergyCost );
    }
}
// End unit_Object::OnActivate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnDeactivate() //                  \author Logan Jones
////////////////////////////////                     \date 03-03-2003
//
//===================================================================
//
void unit_Object::OnDeactivate()
{
	// Update economy consumption values
    if( m_ProductionJob ) {
        m_pPlayer->Update( Metal, Consumption, 0, m_ProductionJob->MetalCost );
		m_pPlayer->Update( Energy, Consumption, 0, m_ProductionJob->EnergyCost );
    }
}
// End unit_Object::OnDeactivate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::ReadyToWork() //                   \author Logan Jones
///////////////////////////////                      \date 02-13-2003
//
//===================================================================
//
void unit_Object::ReadyToWork()
{
	NotifySpawnNewUnit( m_PrimaryJob->Project->TargetType, m_PrimaryJob->Project->Location );
}
// End unit_Object::ReadyToWork()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::JobComplete() //                   \author Logan Jones
///////////////////////////////                      \date 02-27-2003
//
//===================================================================
// Parameters:
//  unit_Job* pJob - 
//
void unit_Object::JobComplete( unit_Job* pJob )
{
	// Update the resouce pool
	m_pPlayer->Update( Metal, Consumption, 0, m_PrimaryJob->MetalCost );
	m_pPlayer->Update( Energy,Consumption, 0, m_PrimaryJob->EnergyCost);

	// Is this the project initiator
    if( pJob->Project->WorkerCount==1 ) {
        pJob->Project->Target->OnComplete();
		// SendMessage( pJob->Project->Target, unit_msg_AreYouClear );
		// Give initial order; like get the hell out of the way
        pJob->Project->Target->m_AttachedProject = NULL;
		delete pJob->Project;
    }
	else --pJob->Project->WorkerCount;

	// TEMP
	m_PrimaryJob = NULL;

	if( m_BuildOrders ) m_BuildOrders->PopTop();
	//else NextOrder
	m_Script->StopBuilding();

	// TEMP
	YardClear();
}
// End unit_Object::JobComplete()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::YardClear() //                     \author Logan Jones
/////////////////////////////                        \date 03-01-2003
//
//===================================================================
//
void unit_Object::YardClear()
{
	if( m_BuildOrders )
		RequestUnit( m_BuildOrders->Top() );
	else if( m_pUnitType->Behaviour==unit_Type::Structure )
		m_Script->Deactivate();
}
// End unit_Object::YardClear()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::OnSelect() //                      \author Logan Jones
////////////////////////////                           \date 5/20/2002
//               
//====================================================================
// Parameters:
//  bool bSelected - 
//  bool bSingle   - 
//
void unit_Object::OnSelect( bool bSelected, bool bSingle )
{
	if( bSelected ) {
		m_bSelected = TRUE;
		m_Model->SetRenderFlag( model_Selected );
		if( bSingle ) sound.PlaySound( m_pUnitType->pSounds->Select1 );
	}
    else {
        m_bSelected = FALSE;
		m_Model->ClearRenderFlag( model_Selected );
    }
}
// End unit_Object::OnSelect()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::OnNewBuildOrder() //               \author Logan Jones
///////////////////////////////////                  \date 02-10-2003
//
//===================================================================
//
void unit_Object::OnNewBuildOrder()
{
//	assert( m_BuildOrders!=NULL );
//	m_Script->Activate();
}
// End unit_Object::OnNewBuildOrder()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::PathFinished() //                  \author Logan Jones
////////////////////////////////                       \date 5/13/2002
//               
//====================================================================
// Parameters:
//  Order_t* pOrder - 
//
void unit_Object::PathFinished( Order_t* pOrder )
{
/*	switch( pOrder->Type )
	{
		case Order_t::Move:
			m_OrderQueue.pop_front();
			StopMoving();
			sound.PlaySound( m_pUnitType->pSounds->Arrived1 );
			if( m_OrderQueue.empty() ) m_CurrentTask = TASK_Nothing;
			else m_CurrentTask = TASK_Pathing;
			break;

		case Order_t::Build:
			StopMoving();
			//m_CurrentTask = TASK_Building;
			m_Manager.SpawnUnit( pOrder->Specifics.pUnitType, pOrder->Goal, m_pPlayer );
			m_OrderQueue.pop_front();
			if( m_OrderQueue.empty() ) m_CurrentTask = TASK_Nothing;
			else m_CurrentTask = TASK_Pathing;
			break;
	} // end switch( pOrder->Type )*/
}
// End unit_Object::PathFinished()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Object_Events.cpp //
/////////////////////////////////
