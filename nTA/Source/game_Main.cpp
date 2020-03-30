// game_Main.cpp //                                \author Logan Jones
//////////////////                                    \date 3/28/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "game.h"
#include "game_Main.h"
#include "net.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "game_Main.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
game_Main::game_Main():
	m_UpdateTime(33),
	m_UpdateTimeElapsed( 0 ),
	m_InterfaceTime(50),
	m_InterfaceTimeElapsed( 0 ),
	m_EconomyTick(0),
	m_FrameCount( 0 ),
	m_FrameTime( 0 ),
	IsGameServer( true )
{
	strcpy( m_FramesPerSecond, "FPS: 0.00" );
	AssignNetMessageHandlers();

	physics_Object::m_pPhysicsSystem = &ColliderSystem;
	scene_Object::m_pSceneSystem = &ScenerySystem;
}
game_Main::~game_Main()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// game_Main::GetSide() //                         \author Logan Jones
/////////////////////////                            \date 04-28-2003
//
//===================================================================
// Parameters:
//  const char* strSide - 
//
// Return: SideData_t* - 
//
SideData_t* game_Main::GetSide( const char* strSide )
{
	for( SideList_t::iterator side = SideData.begin(); side!=SideData.end(); ++side)
		if( stricmp(strSide,(*side).name)==0 )
			return &(*side);
	return NULL;
}
// End game_Main::GetSide()
/////////////////////////////////////////////////////////////////////

int m_MoveTick = 0;
//////////////////////////////////////////////////////////////////////
// game_Main::OnFrame() //                         \author Logan Jones
/////////////////////////                              \date 3/28/2002
//               
//====================================================================
//
void game_Main::OnFrame()
{
	if( (std_Time()-m_InterfaceTimeElapsed)>=m_InterfaceTime )
	{
		m_InterfaceTimeElapsed = std_Time();
		Interface.Update();
	}

	if( (std_Time()-m_UpdateTimeElapsed)>=m_UpdateTime )
	{
		m_UpdateTimeElapsed = std_Time();

        net.SendRecv();
		ProccessNetMessages();

        if( m_EconomyTick==0 ) {
            m_EconomyTick = 30;
			for_each( Players.begin(), Players.end(), UpdatePlayerEconomy );
        }
		else --m_EconomyTick;

		// Update the speicific object systems and the object manager
		Features.Update();
		Units.Update();

		Terrain.ApplyCommands();

        if( m_MoveTick==0 ) {
            m_MoveTick=0;
            ColliderSystem.OnFrame();
        }
        else --m_MoveTick;
		ScenerySystem.Update();

		// Update players
		for_each( Players.begin(), Players.end(), UpdatePlayer );

		// Sort the visible objetcts

		// Increment the tick count
		++Tick;
	}

	Units.m_ScriptMachine.Update();

	// Render a frame!!!
	Interface.Render();

	if( (std_Time()-m_FrameTime)>500.0f )
	{
		sprintf( m_FramesPerSecond, "FPS: %.2f, Tri: %d",
			(1000.0f / (std_Time() - m_FrameTime)) * (++m_FrameCount),
			gfx->GetTriangleCount() );
		m_FrameCount = 0;
		m_FrameTime = std_Time();
	}
	else ++m_FrameCount;
}
// End game_Main::OnFrame()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::Render() //                          \author Logan Jones
////////////////////////                               \date 4/10/2002
//               
//====================================================================
//
void game_Main::Render()
{
	// Render the visable objects
	gfx->BeginObjectRendering();
//	objectManager.Render( Terrain.GetGfx()->GetViewportRect() );
	ScenerySystem.Render();
}
// End game_Main::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::Destroy() //                         \author Logan Jones
/////////////////////////                              \date 3/28/2002
//               
//====================================================================
//
void game_Main::Destroy()
{
	ColliderSystem.Destroy();
	ScenerySystem.Destroy();

	Interface.Destroy();
	Terrain.Destroy();
}
// End game_Main::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::UpdatePlayer() //                    \author Logan Jones
//////////////////////////////                         \date 4/15/2002
//               
//====================================================================
// Parameters:
//  game_Player* pPlayer - 
//
// Return: void - 
//
void WINAPI game_Main::UpdatePlayer( game_Player* pPlayer )
{
	pPlayer->Update();
}
// End game_Main::UpdatePlayer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// game_Main::UpdatePlayerEconomy() //             \author Logan Jones
/////////////////////////////////////                  \date 1/26/2003
//               
//====================================================================
// Parameters:
//  game_Player* pPlayer - 
//
// Return: void - 
//
void WINAPI game_Main::UpdatePlayerEconomy( game_Player* pPlayer )
{
	pPlayer->UpdateEconomy();
}
// End game_Main::UpdatePlayerEconomy()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - game_Main.cpp //
////////////////////////
