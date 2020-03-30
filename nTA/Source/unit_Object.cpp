// unit_Object.cpp //                              \author Logan Jones
////////////////////                                 \date 12/29/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Object.h"
#include "game.h"
#include "gfx.h"
#include "snd.h"
#include "object.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "unit_Object.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
unit_Object::unit_Object( unit_Factory& Manager ):
	physics_Object( OBJ_Unit ),
	scene_Object( OBJ_Unit ),
	m_Manager( Manager )
{}
unit_Object::~unit_Object()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Create() //                        \author Logan Jones
//////////////////////////                             \date 2/24/2002
//               
//====================================================================
// Parameters:
//  const std_Point& ptPosition - 
//  const unit_Type* pUnitType - 
//
// Return: BOOL - 
//
BOOL unit_Object::Create( const std_Point& ptPosition, const unit_Type* pUnitType, game_Player* pPlayer )
{
	m_BaseRect.Set( ptPosition, pUnitType->FootPrint * 16 );
	m_Orientation.Set( 0, 0, fPI/2 );
	m_Direction.Set( 0, 1 );

	// Set the type and host player
	m_pUnitType = pUnitType;
	m_pPlayer = pPlayer;

	// Create an model instance for this unit
	m_pUnitType->Model->NewInstance( &m_Model );

	// Create the script proccess and run the create module
	if( bFAILED(theGame.Units.m_ScriptMachine.CreateProccess( m_pUnitType->pScript, this, m_Model, &m_Script )) )
	{
		//Destroy();
		return FALSE;
	}

	// Initialize the model verts and states
	m_Model->SynchronizeVertices();
	m_Model->SynchronizeStates();

	// If the unit can build let the initial menu be athe first build menu
	m_LastMenu = (m_pUnitType->Abilities & unit_Type::CanBuild) ? 1 : 0;
	m_LastBuildPage = 1;

	// Set the initial orders
	m_FireOrder = m_pUnitType->InitialFireOrder;
	m_MoveOrder = m_pUnitType->InitialMoveOrder;
	m_Cloaked = FALSE;
	m_Activation = FALSE;

	// Initialize
	m_BuildOrders = NULL;
	m_bSelected = FALSE;
	m_ReadyToBuild = 0;
	m_PrimaryJob = m_ProductionJob = NULL;
	m_AttachedProject = NULL;

	// Call the create script and the creation event
	m_Script->Create();
	OnCreate();

	// Attach to physics and scene systems
	m_SceneSortKey = m_VisibleRect.top;
	AddToPhysicsSystem();
	AttachToScene();

	return TRUE;
}
// End unit_Object::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Update() //                        \author Logan Jones
//////////////////////////                              \date 3/3/2002
//               
//====================================================================
//
void unit_Object::Update()
{
	if( !m_Script->IsAnimating() )
		MakeAnimator( false );
/*
	Order_t*	pOrder = m_OrderQueue.empty() ? NULL : &m_OrderQueue.front();

	switch( m_CurrentTask )
	{
		case TASK_Nothing:
		case TASK_BeingBuilt:
			break;

		case TASK_Pathing:
			if( pOrder && !pOrder->Waypoints.empty() && m_BaseRect.PointInRect(pOrder->Waypoints.back()) )
			{
				pOrder->Waypoints.pop_back();
				if( pOrder->Waypoints.empty() )
					PathFinished( pOrder );
				else ComputeMotion();
			}
			break;
	} // end switch( m_CurrentTask )*/
}
// End unit_Object::Update()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Animate() //                       \author Logan Jones
///////////////////////////                            \date 6/11/2002
//               
//====================================================================
//
void unit_Object::Animate()
{
	m_Script->Animate();
}
// End unit_Object::Animate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Render() //                        \author Logan Jones
//////////////////////////                              \date 2/2/2002
//               
//====================================================================
// Parameters:
//   std_Vector2 vOffset - 
//
void unit_Object::Render( std_Vector2 vOffset ) const
{
	//const std_RectF ScreenRect( m_BaseRect - vOffset/* - m_HeightOffset*/ );
	//const DWORD Flags = (m_bSelected ? gfx_Model::Selected : 0) |
	//					(m_Elevation < theGame.Terrain.GetSeaLevel() ? gfx_Model::InWater : 0) |
	//					(TRUE ? gfx_Model::SubmergedVisible : 0);

/*	m_pUnitType->Model->Render(
		m_Script->GetPieces(),
		ScreenRect.Center(),
		m_MoveInfo.GetOrientation() + std_Vector3(0,0,fPI/2),
		theGame.Terrain.GetSeaLevel() - m_Elevation,
		Flags,
		this );*/

#ifdef VISUAL_PATHFIND
	unit_Factory::PathFind_t* pPath = m_Manager.GetPathInProgress( (unit_Object*)((DWORD)this) );
	if( pPath!=NULL )
	{
		std_RectF		rct;
		unit_Factory::SearchNodeBank_t::iterator	it = pPath->NodeBank.begin();
		unit_Factory::SearchNodeBank_t::iterator	end= pPath->NodeBank.end();
		for( ; it!=end; ++it )
		{
			unit_Factory::SearchNode_t*	pNode = ((*it).second);
			rct.Set( pNode->Location * 16, 16, 16 );
			if( pNode==pPath->pBest )
				gfx->DrawRect( rct - vOffset, 0xFFFFFF77 );
			else if( pNode==pPath->pProccessee )
				gfx->DrawRect( rct - vOffset, 0xFFFF0077 );
			else if( pNode->InOpen )
				gfx->DrawRect( rct - vOffset, 0x0000FF77 );
			else if( pNode->InClosed )
				gfx->DrawRect( rct - vOffset, 0xFF000077 );
			else
				gfx->DrawRect( rct - vOffset, 0x00000077 );
		}
	}
#endif
#ifdef VISUAL_PATHFIND_RESULT
	/*if( !m_OrderQueue.empty() )
	{
		const Order_t* pOrder = &m_OrderQueue.front();
		Waypoints_t::const_iterator	it = pOrder->Waypoints.begin();
		Waypoints_t::const_iterator	end= pOrder->Waypoints.end();
		for( ; it!=end; ++it)
			gfx->DrawRect( std_RectF( (*it) - vOffset - std_Vector2( 0, theGame.Terrain.GetHeight((*it)/16) * 0.5f ), std_SizeF(16,16)), DWORD(0xFFFFFF99) );
	}*/
#endif
}
// End unit_Object::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::Render() //                        \author Logan Jones
//////////////////////////                             \date 5/29/2002
//               
//====================================================================
//
void unit_Object::Render()
{
	//const DWORD Flags = (m_bSelected ? gfx_Model::Selected : 0) |
	//					(m_Elevation < theGame.Terrain.GetSeaLevel() ? gfx_Model::InWater : 0) |
	//					(TRUE ? gfx_Model::SubmergedVisible : 0);

	//m_pUnitType->Model->Render(
	//	m_Script->GetPieces(),
	//	m_VisibleRect.Center(),
	//	m_MoveInfo.GetOrientation() + std_Vector3(0,0,fPI/2),
	//	0,
	//	theGame.Terrain.GetSeaLevel() - m_Elevation,
	//	Flags,
	//	this );

	m_Model->Render( m_VisibleRect.Center(), m_Orientation + std_Vector3(0,0,fPI/2) );

	if( m_PrimaryJob && m_PrimaryJob->Active ) {
		gfx_ModelPiece*	pFrom= m_Script->QueryNanoPiece();
		gfx_ModelPiece*	pTo  = m_PrimaryJob->Project->Target->m_Script->SweetSpot();
		std_Vector3		From = pFrom->ScreenPosition();
		std_Vector2		To = pTo->ScreenPosition();
		gfx->DrawLine( m_VisibleRect.Center() + From, m_PrimaryJob->Project->Target->m_VisibleRect.Center() + To, 0xFFFFFFFF );
	}
}
// End unit_Object::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::SetPath() //                       \author Logan Jones
///////////////////////////                             \date 3/5/2002
//               
//====================================================================
//
void unit_Object::SetPath()
{
//	m_OrderQueue.front().Waypoints.pop_back();
	ComputeMotion();
//	Move( m_pUnitType->MaxSpeed );
//	MakeMover();
	MakeMeDynamic();
}
// End unit_Object::SetPath()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::ComputeMotion() //                 \author Logan Jones
/////////////////////////////////                      \date 2/24/2002
//               
//====================================================================
//
void unit_Object::ComputeMotion()
{
//	std_Vector2		Dir( m_Path.front() - m_BaseRect.Center() );
	std_Vector2		Dir( Waypoint() - m_BaseRect.Center() );

	Dir.Normalize();
//	Turn( Dir );
	//m_Direction = m_Path.front() - m_BaseRect.Center();
	//m_Direction.Normalize();
}
// End unit_Object::ComputeMotion()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::DoJobStep() //                     \author Logan Jones
/////////////////////////////                        \date 02-12-2003
//
//===================================================================
// Parameters:
//  unit_Job* pJob - 
//  bool bStep     - 
//
void unit_Object::DoJobStep( unit_Job* pJob, bool bStep )
{
	// TEMP
	//if( bStep )
	//	if( ((pJob->Project->AppliedTime+=pJob->Worker->m_pUnitType->WorkerTime)>=pJob->Project->TargetType->BuildTime) )
	//	{
	//		if( pJob->Project->WorkerCount==1 ) delete pJob->Project;
	//		m_Project = NULL;
	//		m_pPlayer->KillJob( this ),
	//		m_Script->Deactivate(),
	//		m_pPlayer->Update( Metal, Consumption, 0, pJob->Project->TargetType->BuildCostMetal /  ),
	//		m_pPlayer->Update( Energy,Consumption, 0, pJob->EnergyCost);
	//	}
}
// End unit_Object::DoJobStep()
/////////////////////////////////////////////////////////////////////

static long _yard_open = 0;
/////////////////////////////////////////////////////////////////////
// unit_Object::SetUnitValue() //                  \author Logan Jones
////////////////////////////////                     \date 02-13-2003
//
//===================================================================
// Parameters:
//  const long& lUnitValueID - 
//  long lDesiredValue       - 
//
void unit_Object::SetUnitValue( const long& lUnitValueID, long lDesiredValue )
{
	switch( lUnitValueID )
	{
		case INBUILDSTANCE:
			if( (m_ReadyToBuild=lDesiredValue) ) ReadyToWork();
			break;

		case BUSY:
			break;

		case YARD_OPEN:
			_yard_open = lDesiredValue;

		case BUGGER_OFF:
		case ARMORED:
			break;

		default:
			break;
	}
}
// End unit_Object::SetUnitValue()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Object::GetUnitValue() //                  \author Logan Jones
////////////////////////////////                       \date 4/18/2002
//               
//====================================================================
// Parameters:
//  const long lUnitValueID - 
//
// Return: const long - 
//
const long unit_Object::GetUnitValue( const long lUnitValueID ) const
{
	switch( lUnitValueID )
	{
		case ACTIVATION:
			return m_Activation;
		case STANDINGMOVEORDERS:
			return m_MoveOrder;
		case STANDINGFIREORDERS:
			return m_FireOrder;
		case HEALTH:
			return 0;
		case INBUILDSTANCE:
			return m_ReadyToBuild;
		case BUSY:
		case PIECE_XZ:
		case PIECE_Y:
		case UNIT_XZ:
		case UNIT_Y:
		case UNIT_HEIGHT:
		case GROUND_HEIGHT:
		case BUILD_PERCENT_LEFT:
			return 0;

		case YARD_OPEN:
			return _yard_open;

		case BUGGER_OFF:
		case ARMORED:
			return 0;
		case IN_WATER:
			return theGame.Terrain.GetElevation(m_BaseRect.Center()) < theGame.Terrain.GetSeaLevel();
		case CURRENT_SPEED:
			return long(m_Velocity.Magnitude() * LINEAR_CONSTANT);
		case VETERAN_LEVEL:
			return 0;

		default:
			return 0;
	}
}
// End unit_Object::GetUnitValue()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::AbsoluteHealth() //                \author Logan Jones
//////////////////////////////////                   \date 03-06-2003
//
//===================================================================
// Return: float - 
//
float unit_Object::AbsoluteHealth() const
{
	if( m_AttachedProject )
		return m_AttachedProject->AppliedTime / m_pUnitType->BuildTime;
	else return 1;
}
// End unit_Object::AbsoluteHealth()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::EconomicActivity() //              \author Logan Jones
////////////////////////////////////                 \date 03-03-2003
//
//===================================================================
// Parameters:
//  float& fMetalProduction  - 
//  float& fMetalConsumtion  - 
//  float& fEnergyProduction - 
//  float& fEnergyConsumtion - 
//
void unit_Object::EconomicActivity( float& fMetalProduction, float& fMetalConsumtion, float& fEnergyProduction, float& fEnergyConsumtion ) const
{
	fMetalProduction = m_pUnitType->MetalMake;
	fEnergyProduction= m_pUnitType->EnergyMake;

	fMetalConsumtion = m_PrimaryJob ? m_PrimaryJob->MetalCost : 0;
	fEnergyConsumtion= m_PrimaryJob ? m_PrimaryJob->EnergyCost: 0;

	if( m_ProductionJob ) {
        if( m_ProductionJob->Active ) {
            fMetalProduction += m_ProductionJob->MetalIncome;
			fEnergyProduction+= m_ProductionJob->EnergyIncome;
        }
        if( Active() ) {
            fMetalConsumtion += m_ProductionJob->MetalCost;
			fEnergyConsumtion+= m_ProductionJob->EnergyCost;
        }
	}
}
// End unit_Object::EconomicActivity()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::DoMovement() //                    \author Logan Jones
//////////////////////////////                       \date 05-15-2003
//
//===================================================================
// Return: bool - return true if unit is still alive
//
bool unit_Object::DoMovement()
{
	// Save the old base rect
	m_MovementBox = m_BaseRect;

	// Steering
		// Accumulate steering vector

	// Locomotion
		// Apply steering vector
		// Update move state

	Locomotion( Steering() );

	// Make a movement box that encompasses the old and new position
	m_MovementBox.Encompass( m_BaseRect );

	// Validation
	// Net
	return false;
}
// End unit_Object::DoMovement()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Object::Steering() //                      \author Logan Jones
////////////////////////////                         \date 05-21-2003
//
//===================================================================
// Return: std_Vector2 - 
//
std_Vector2 unit_Object::Steering()
{
	std_Vector2 target = Waypoint();
	std_Vector2 position = m_BaseRect.Center();
	const std_Vector2	Offset = target - position;
	//return ((math_Normalize(Offset) * m_pUnitType->MaxSpeed) - m_Velocity);

	const float			Distance = Offset.Magnitude();
	const float			SlowingDistance = sqr(m_pUnitType->MaxSpeed) / m_pUnitType->BrakeRate;
	const float			RampedSpeed = m_pUnitType->MaxSpeed * Distance / SlowingDistance;
	const float			ClippedSpeed = min( RampedSpeed, m_pUnitType->MaxSpeed );
	const std_Vector2	DesiredVelocity = Offset * (ClippedSpeed / Distance);
	return (DesiredVelocity - m_Velocity);
}
// End unit_Object::Steering()
/////////////////////////////////////////////////////////////////////

/*
/////////////////////////////////////////////////////////////////////
// unit_Object::Locomotion() //                    \author Logan Jones
//////////////////////////////                       \date 05-21-2003
//
//===================================================================
// Parameters:
//  const std_Vector2& vSteering - 
//
void unit_Object::Locomotion( const std_Vector2& vSteering )
{
	switch( m_pUnitType->Behaviour ) {
		case unit_Type::Structure:
			break;

		case unit_Type::Groundcraft:
			break;

		case unit_Type::Seacraft:
			break;

		case unit_Type::Aircraft:
			break;

		case unit_Type::Hovercraft:
			break;

		default: // This shouldn't happen
			assert( !"Invalid unit type behaviour." );
	}
}
// End unit_Object::Locomotion()
/////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// End - unit_Object.cpp //
//////////////////////////
