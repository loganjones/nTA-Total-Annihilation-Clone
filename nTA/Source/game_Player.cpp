// game_Player.cpp //                              \author Logan Jones
////////////////////                                  \date 4/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "player.h"
#include "game_Player.h"
#include "ta_TDF.h"
#include "ta_OTA.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "game_Player.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
game_Player::game_Player()
{
	ZeroMemory( m_Resources, sizeof(m_Resources) );
}
game_Player::~game_Player()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Player::Initialize() //                    \author Logan Jones
//////////////////////////////                         \date 4/15/2002
//               
//====================================================================
// Parameters:
//  ta_ota_Schema_t* pSchema - 
//
void game_Player::Initialize( ta_ota_Schema_t* pSchema )
{
	SideData_t*			pSideData = theGame.GetSide(m_Side);
	const unit_Type*	pType;

	if( pSideData==NULL ) {/*ERROR*/}

	m_Resources[Metal].Amount = m_Resources[Metal].Max;
	m_Resources[Energy].Amount= m_Resources[Energy].Max;

	// If we haven't alredy been assigned untis, spawn the commander
    if( m_Units.empty() )
    {
		if((pType = theGame.Units.GetUnitType( pSideData->commander )))
			theGame.NotifySpawnCompleteUnit( pType, pSchema->StartPositions[m_Index], m_Index );
		else {/*ERROR*/}
    }

	//// If we haven't alredy been assigned untis, spawn the commander
	//if( m_Units.empty() )
	//	pType = theGame.Units.GetUnitType( pSideData->commander ),
	//	pUnit = theGame.Units.SpawnUnit(pType,pSchema->StartPositions[m_Index],this),
	//	pUnit->OnComplete(),
	//	m_Units.push_back( pUnit );
}
// End game_Player::Initialize()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::Update() //                        \author Logan Jones
//////////////////////////                            \date 1/26/2003
//
//===================================================================
//
void game_Player::Update()
{
}
// End game_Player::Update()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::UpdateEconomy() //                 \author Logan Jones
/////////////////////////////////                     \date 1/26/2003
//
//===================================================================
//
void game_Player::UpdateEconomy()
{
	int					i;
//	bool				Surplus = true;
	long				Surplus = 0;
	JobList_t::iterator	job,it = m_Jobs.begin(),
						end= m_Jobs.end();

	// for each resource
	for( i=0; i<RESOURCE_TYPE_COUNT; ++i) {

		// Add the amount produced thus far
		m_Resources[i].Amount = min( m_Resources[i].Amount + m_Resources[i].BaseProduced, m_Resources[i].Max );
		m_Resources[i].TotalProduced = m_Resources[i].BaseProduced;

		// Are enough resources available?
		if( m_Resources[i].Amount>=m_Resources[i].TotalConsumed )
			//Surplus = false;
			Surplus |= (1<<i);
			//m_Resources[i].Amount -= m_Resources[i].TotalConsumed;
	}

	// If economy is in surplus then subtract consumption from amount
	//if( Surplus ) for( i=0; i<RESOURCE_TYPE_COUNT; ++i)
	//	m_Resources[i].Amount -= m_Resources[i].TotalConsumed;

	// for each job
	while( it!=end )
		if( !(*it).Update(Surplus,this) )
			it = m_Jobs.erase(it);
		else ++it;
		//if( !((*(job=it++)).second).Update(Surplus,this) )
		//	m_Jobs.erase(job);
}
// End game_Player::UpdateEconomy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Player.cpp //
//////////////////////////
