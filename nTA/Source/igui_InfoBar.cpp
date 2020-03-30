// igui_InfoBar.cpp //                             \author Logan Jones
/////////////////////                                  \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_InfoBar.h"
#include "game.h"
#include "gfx.h"
#include "ta_TDF.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_InfoBar::igui_InfoBar():wnd_Window(),
	m_DisplayingWhat( DISP_Nothing )
{
}
igui_InfoBar::~igui_InfoBar()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_InfoBar::Initialize() //                   \author Logan Jones
///////////////////////////////                        \date 5/15/2002
//               
//====================================================================
// Parameters:
//  struct SideData_t* pSideData - 
//
void igui_InfoBar::Initialize( struct SideData_t* pSideData )
{
	const std_Point	FontSize( 0, theInterface->Font()->GetHeight() - 1 );
	const std_Point RelativePosition( m_ScreenPosition.x, 480 - m_Size.height );

	m_Name = pSideData->Footer.name.Position() - RelativePosition + FontSize;
	m_Description = pSideData->Footer.description.Position() - RelativePosition + FontSize;
	m_UnitName = pSideData->Footer.unitname.Position() - RelativePosition + FontSize;
	m_UnitName2= pSideData->Footer.unitname2.Position() - RelativePosition + FontSize;
	m_Status = pSideData->Footer.missiontext.Position() - RelativePosition + FontSize;

	m_Metal_Produced = pSideData->Footer.unitmetalmake.Position() - RelativePosition + FontSize;
	m_Metal_Consumed = pSideData->Footer.unitmetaluse.Position() - RelativePosition + FontSize;
	m_Energy_Produced = pSideData->Footer.unitenergymake.Position() - RelativePosition + FontSize;
	m_Energy_Consumed = pSideData->Footer.unitenergyuse.Position() - RelativePosition + FontSize;

	m_DamageBar = pSideData->Footer.damagebar - RelativePosition + FontSize;
	m_DamageBar2= pSideData->Footer.damagebar2- RelativePosition + FontSize;
	m_LogoRect = pSideData->Footer.logo2 - RelativePosition + FontSize;
}
// End igui_InfoBar::Initialize()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ftoa() //                                       \author Logan Jones
///////////                                           \date 1/26/2003
//
//===================================================================
// Parameters:
//  float fValue  - 
//  char* pBuffer - 
//
// Return: const char* - 
//
inline const char* ftoa( float fValue, char* pBuffer )
{
	sprintf( pBuffer, "%.1f", fValue );
	return pBuffer;
}
// End ftoa()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// DrawHealthBar() //                              \author Logan Jones
////////////////////                                 \date 03-06-2003
//
//===================================================================
// Parameters:
//  const std_Rect& rctBar - 
//  const float fHealth    - 
//
inline void DrawHealthBar( const std_Rect& rctBar, const float fHealth )
{
	std_Rect	Left( rctBar.left, rctBar.top, rctBar.left + rctBar.Width() * fHealth, rctBar.bottom ),
				Right( Left.right + 1, rctBar.top, rctBar.right, rctBar.bottom );
	gfx->DrawRect( Left, (DWORD)0x53DF4FFF );
	gfx->DrawRect( Right, (DWORD)0xFF4700FF );
}
// End DrawHealthBar()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_InfoBar::Render() //                       \author Logan Jones
///////////////////////////                             \date 4/1/2002
//               
//====================================================================
//
void igui_InfoBar::Render()
{
	gfx->PushMatrix();
	gfx->Translate( m_ScreenPosition );
	
		// Draw the bar
		for( long x=m_Size.width,n=0; x>0; x-=512,++n)
			gfx->RenderSurfaceEx( std_Point(512,0) * n, theInterface->Surface(BottomPanel)  );

	switch( m_DisplayingWhat )
	{
		case DISP_FeatureInfo:
			gfx->RenderString( m_Line1, m_Name, 0xFFFFFFFF, theInterface->Font() );
			break;

		case DISP_UnitInfo: {
			static float	fMetalProduction,fMetalConsumtion,fEnergyProduction,fEnergyConsumtion;
			m_What.Unit->EconomicActivity(fMetalProduction,fMetalConsumtion,fEnergyProduction,fEnergyConsumtion);
			m_Line2[0] = '+';
			gfx->RenderString( ftoa(fMetalProduction,m_Line2+1)-1, m_Metal_Produced, 0x53DF4FFF, theInterface->Font() );
			gfx->RenderString( ltoa(fEnergyProduction,m_Line2+1,10)-1, m_Energy_Produced, 0x53DF4FFF, theInterface->Font() );
			m_Line2[0] = '-';
			gfx->RenderString( ftoa(fMetalConsumtion,m_Line2+1)-1, m_Metal_Consumed, 0xFF4700FF, theInterface->Font() );
			gfx->RenderString( ltoa(fEnergyConsumtion,m_Line2+1,10)-1, m_Energy_Consumed, 0xFF4700FF, theInterface->Font() );
			gfx->RenderString( "Idle", m_Status, 0xFFFFFFFF, theInterface->Font() );
			gfx->RenderString( m_Line1, m_UnitName, 0xFFFFFFFF, theInterface->Font() );
			DrawHealthBar( m_DamageBar, m_What.Unit->AbsoluteHealth() );
            if( m_What.Unit->m_PrimaryJob && m_What.Unit->m_PrimaryJob->Project->Target ) {
                DrawHealthBar( m_DamageBar2, m_What.Unit->m_PrimaryJob->Project->Target->AbsoluteHealth() );
				gfx->RenderString( m_What.Unit->m_PrimaryJob->Project->TargetType->Name, m_UnitName2, 0xFFFFFFFF, theInterface->Font() );
            }
			break;
		}

		case DISP_BuildInfo:
			gfx->RenderString( m_Line1, m_Name, 0xFFFFFFFF, theInterface->Font() );
			gfx->RenderString( m_Line2, m_Description, 0xFFFFFFFF, theInterface->Font() );
			break;
            
        case DISP_Nothing:
            break;
	}

	gfx->PopMatrix();
}
// End igui_InfoBar::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_InfoBar::DisplayObjectInfo() //            \author Logan Jones
//////////////////////////////////////                 \date 4/20/2002
//               
//====================================================================
// Parameters:
//  class scene_Object* pObject - 
//
void igui_InfoBar::DisplayObjectInfo( class scene_Object* pObject )
{
	if( pObject==NULL ) m_DisplayingWhat = DISP_Nothing;
	else if( pObject->GetSceneryType()==OBJ_Feature )
	{
		m_What.Feature = static_cast<feature_Object*>(pObject);
		sprintf( m_Line1, "%s", m_What.Feature->GetType()->Name.c_str() );
		m_DisplayingWhat = DISP_FeatureInfo;
	}
	else if( pObject->GetSceneryType()==OBJ_Unit )
	{
		m_What.Unit = static_cast<unit_Object*>(pObject);
		sprintf( m_Line1, "%s", m_What.Unit->m_pUnitType->Name );
		m_DisplayingWhat = DISP_UnitInfo;
	}
	else m_DisplayingWhat = DISP_Nothing;
}
// End igui_InfoBar::DisplayObjectInfo()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_InfoBar::DisplayBuildInfo() //             \author Logan Jones
/////////////////////////////////////                   \date 5/1/2002
//               
//====================================================================
// Parameters:
//  const unit_Type* pType - 
//
void igui_InfoBar::DisplayBuildInfo( const unit_Type* pType )
{
	assert( pType );
	sprintf( m_Line1, "%s:   M: %d E: %d", pType->Name, pType->BuildCostMetal, pType->BuildCostEnergy );
	sprintf( m_Line2, "%s", pType->Description );
	m_What.UnitType = pType;
	m_DisplayingWhat = DISP_BuildInfo;
}
// End igui_InfoBar::DisplayBuildInfo()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_InfoBar::OnCursorMove() //                 \author Logan Jones
/////////////////////////////////                      \date 4/27/2002
//               
//====================================================================
// Parameters:
//  std_Point& ptCursor - 
//  DWORD dwFlags       - 
//
void igui_InfoBar::OnCursorMove( std_Point& ptCursor, DWORD dwFlags )
{
	gfx->SetCurrentCursor( guiResources.Cursors.Normal );
	wnd_Window::OnCursorMove( ptCursor, dwFlags );
}
// End igui_InfoBar::OnCursorMove()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_InfoBar.cpp //
///////////////////////////
