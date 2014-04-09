// player_Local.cpp //                             \author Logan Jones
/////////////////////                                 \date 4/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "player.h"
#include "player_Local.h"
#include "gfx.h"
#include "ta_OTA.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
player_Local::player_Local():game_Player()
{}
player_Local::~player_Local()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// player_Local::Initialize() //                   \author Logan Jones
///////////////////////////////                        \date 4/15/2002
//               
//====================================================================
// Parameters:
//  ta_ota_Schema_t* pSchema - 
//
void player_Local::Initialize( ta_ota_Schema_t* pSchema )
{
	game_Player::Initialize(pSchema);
	theGame.Interface.Initialize( this, pSchema->StartPositions[m_Index], theGame.GetSide(m_Side) );
}
// End player_Local::Initialize()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - player_Local.cpp //
///////////////////////////
