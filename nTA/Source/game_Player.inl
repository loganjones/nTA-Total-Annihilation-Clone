// game_Player.inl //                              \author Logan Jones
////////////////////                                  \date 1/26/2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::Consume() //                       \author Logan Jones
///////////////////////////                          \date 02-11-2003
//
//===================================================================
// Parameters:
//  ResourceType_t eWhat - 
//  float fHowMuch       - 
//
void game_Player::Consume( ResourceType_t eWhat, float fHowMuch )
{
    m_Resources[eWhat].Amount = __max( m_Resources[eWhat].Amount - fHowMuch, 0.0f );
}
// End game_Player::Consume()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::Produce() //                       \author Logan Jones
///////////////////////////                          \date 02-11-2003
//
//===================================================================
// Parameters:
//  ResourceType_t eWhat - 
//  float fHowMuch       - 
//
void game_Player::Produce( ResourceType_t eWhat, float fHowMuch )
{
	m_Resources[eWhat].Amount = min(m_Resources[eWhat].Amount + fHowMuch, m_Resources[eWhat].Max);
	m_Resources[eWhat].TotalProduced += fHowMuch;
}
// End game_Player::Produce()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::Update() //                        \author Logan Jones
//////////////////////////                            \date 1/26/2003
//
//===================================================================
// Parameters:
//  ResourceType_t eType      - Resource to update
//  UpdateResouceFlag_t eWhat - Category to update
//  float fNew                - New value
//  float fOld                - Old value
//
void game_Player::Update( ResourceType_t eType, UpdateResouceFlag_t eWhat, float fNew, float fOld )
{
	switch( eWhat ) {
		case Maximum: m_Resources[eType].Max += fNew - fOld;
			m_Resources[eWhat].Amount = min(m_Resources[eWhat].Amount, m_Resources[eWhat].Max);
			break;
		case Production: m_Resources[eType].BaseProduced += fNew - fOld; break;
		case Consumption:m_Resources[eType].TotalConsumed += fNew - fOld; break;
		default: assertEx( false, "Invalid UpdateResouceFlag_t." );
	}
}
// End game_Player::Update()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::NewJob() //                        \author Logan Jones
//////////////////////////                           \date 02-12-2003
//
//===================================================================
// Parameters:
//  unit_Object* pWorker - Job requester
//  job_Type eType       - 
//  bool bStartActive    - 
//
// Return: unit_Job* - A pointer to the new job
//
unit_Job* game_Player::NewJob( unit_Object* pWorker, job_Type eType, bool bStartActive )
{
	// Uglyness!!!!!!!!
	//return &(*(m_Jobs.insert( JobList_t::value_type(pWorker,unit_Job()) )).first).second;
	m_Jobs.push_front( unit_Job() );
	m_Jobs.front().ID = m_Jobs.begin();
	m_Jobs.front().Type = eType;
	m_Jobs.front().Worker = pWorker;
	m_Jobs.front().Active = bStartActive;
	return &m_Jobs.front();
}
// End game_Player::NewJob()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Player::KillJob() //                       \author Logan Jones
///////////////////////////                          \date 02-12-2003
//
//===================================================================
// Parameters:
//  unit_Object* pWorker - 
//
void game_Player::KillJob( unit_Object* pWorker )
{
//	m_Jobs.erase( pWorker );
}
// End game_Player::KillJob()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Player.inl //
//////////////////////////
