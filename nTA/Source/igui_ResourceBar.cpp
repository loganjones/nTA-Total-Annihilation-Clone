// igui_ResourceBar.cpp //                         \author Logan Jones
/////////////////////////                              \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_ResourceBar.h"
#include "game.h"
#include "gfx.h"
#include "ta_TDF.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
igui_ResourceBar::igui_ResourceBar():wnd_Window()
{}
igui_ResourceBar::~igui_ResourceBar()
{}
//
/////////////////////////////////////////////////////////////////////

float Test2;
//////////////////////////////////////////////////////////////////////
// igui_ResourceBar::Initialize() //               \author Logan Jones
///////////////////////////////////                    \date 5/15/2002
//               
//====================================================================
// Parameters:
//  SideData_t* pSideData - 
//
void igui_ResourceBar::Initialize( SideData_t* pSideData )
{
	BYTE*		FileBuf;
	std_Point	FontSize( 0, theInterface->Font()->GetHeight() - 1 );

	m_MetalColor = pSideData->metalcolor;
	m_EnergyColor = pSideData->energycolor;

	theFileTree.DumpFileToBuffer( "anims/logos.gaf", &FileBuf );
	gfx->CreateSurfaceFromGAF( FileBuf, "32xLogos", theInterface->User()->Color() + 1, &m_Logo );
	delete [] FileBuf;
	m_LogoRect = pSideData->Header.logo - m_ScreenPosition;

	m_MetalBar = pSideData->Header.metalbar - m_ScreenPosition;
	m_EnergyBar = pSideData->Header.energybar - m_ScreenPosition;

	m_Metal_0 = pSideData->Header.metal0.Position() - m_ScreenPosition + FontSize;
	m_Metal_Max = pSideData->Header.metalmax.Position() - m_ScreenPosition + FontSize;
	m_Metal_Num = pSideData->Header.metalnum.Position() - m_ScreenPosition + FontSize;
	m_Metal_Produced = pSideData->Header.metalproduced.Position() - m_ScreenPosition + FontSize;
	m_Metal_Consumed = pSideData->Header.metalconsumed.Position() - m_ScreenPosition + FontSize;
	sprintf( m_Metal_Produced_String, "%.1f", 0.0f );
	sprintf( m_Metal_Consumed_String, "%.1f", 0.0f );

	m_Energy_0 = pSideData->Header.energy0.Position() - m_ScreenPosition + FontSize;
	m_Energy_Max = pSideData->Header.energymax.Position() - m_ScreenPosition + FontSize;
	m_Energy_Num = pSideData->Header.energynum.Position() - m_ScreenPosition + FontSize;
	m_Energy_Produced = pSideData->Header.energyproduced.Position() - m_ScreenPosition + FontSize;
	m_Energy_Consumed = pSideData->Header.energyconsumed.Position() - m_ScreenPosition + FontSize;
	sprintf( m_Energy_Produced_String, "%.1f", 0.0f );
	sprintf( m_Energy_Consumed_String, "%.1f", 0.0f );
	m_Test = 0;
	Test2 = 0;
}
// End igui_ResourceBar::Initialize()
//////////////////////////////////////////////////////////////////////


inline float Clamp( float fValue, float fMin, float fMax )
{
	return fValue<fMin ? fMin : (fValue>fMax ? fMax:fValue);
}


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


//////////////////////////////////////////////////////////////////////
// igui_ResourceBar::Render() //                   \author Logan Jones
///////////////////////////////                         \date 4/1/2002
//               
//====================================================================
//
void igui_ResourceBar::Render()
{
	char					NumString[32];
	const ResourceData_t*	pMetal = &theInterface->User()->Resource(Metal);
	const ResourceData_t*	pEnergy= &theInterface->User()->Resource(Energy);
	const float				MetalAmount = math_IIRFilter( Clamp(pMetal->Amount + pMetal->TotalProduced - pMetal->TotalConsumed,0,pMetal->Max), 0.2f, m_Test) + 0.5f;
	const float				EnergyAmount= math_IIRFilter( Clamp(pEnergy->Amount+ pEnergy->TotalProduced- pEnergy->TotalConsumed,0,pEnergy->Max), 0.2f, Test2)+ 0.5f;
	const long				MetalWidth = (MetalAmount / pMetal->Max) * m_MetalBar.Width();
	const long				EnergyWidth= (EnergyAmount / pEnergy->Max)*m_EnergyBar.Width();

	gfx->PushMatrix();
	gfx->Translate( m_ScreenPosition );

	gfx->RenderSurface( theInterface->Surface(TopPanel) );
	for( long x=m_Size.width-512,n=1; x>0; x-=512,++n)
		gfx->RenderSurfaceEx( std_Point(512*n,0), theInterface->Surface(BottomPanel) );

	m_Logo->Render( m_LogoRect );

	gfx->DrawRect( std_Rect(m_MetalBar.left, m_MetalBar.top, m_MetalBar.left + MetalWidth, m_MetalBar.bottom), m_MetalColor );
	gfx->DrawRect( std_Rect(m_EnergyBar.left, m_EnergyBar.top, m_EnergyBar.left + EnergyWidth, m_EnergyBar.bottom), m_EnergyColor );

	gfx->RenderString( "0", m_Metal_0, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderStringRight( ltoa(pMetal->Max,NumString,10), m_Metal_Max, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderString( ltoa(MetalAmount,NumString,10), m_Metal_Num, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderString( ftoa(pMetal->TotalProduced,NumString), m_Metal_Produced, 0x53DF4FFF, theInterface->Font() );
	gfx->RenderString( ftoa(pMetal->TotalConsumed,NumString), m_Metal_Consumed, 0xFF4700FF, theInterface->Font() );

	gfx->RenderString( "0", m_Energy_0, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderStringRight( ltoa(pEnergy->Max,NumString,10), m_Energy_Max, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderString( ltoa(EnergyAmount,NumString,10), m_Energy_Num, 0xFFFFFFFF, theInterface->Font() );
	gfx->RenderString( ltoa(pEnergy->TotalProduced,NumString,10), m_Energy_Produced, 0x53DF4FFF, theInterface->Font() );
	gfx->RenderString( ltoa(pEnergy->TotalConsumed,NumString,10), m_Energy_Consumed, 0xFF4700FF, theInterface->Font() );

	gfx->PopMatrix();
}
// End igui_ResourceBar::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// igui_ResourceBar::OnCursorMove() //             \author Logan Jones
/////////////////////////////////////                  \date 4/27/2002
//               
//====================================================================
// Parameters:
//  std_Point& ptCursor - 
//  DWORD dwFlags       - 
//
void igui_ResourceBar::OnCursorMove( std_Point& ptCursor, DWORD dwFlags )
{
	gfx->SetCurrentCursor( guiResources.Cursors.Normal );
	wnd_Window::OnCursorMove( ptCursor, dwFlags );
}
// End igui_ResourceBar::OnCursorMove()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_ResourceBar.cpp //
///////////////////////////////
