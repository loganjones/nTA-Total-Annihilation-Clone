// fe_MS_PlayerSelection.cpp //                    \author Logan Jones
//////////////////////////////                       \date 05-06-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "net.h"
#include "game.h"
#include "gadget_Button.h"
#include "gadget_String.h"
#include "gadget_Pic.h"
#include "fe_MS_PlayerSelection.h"
#include "fe_MultiplayerSetup.h"


/////////////////////////////////////////////////////////////////////
// fe_MS_PlayerSelection::Create() //              \author Logan Jones
////////////////////////////////////                 \date 05-06-2003
//
//===================================================================
// Parameters:
//  const char* strName          - 
//  UINT32 uiIndex               - 
//  UINT32 uiNetID               - 
//  const char* strSide          - 
//  UINT8 uiColor                - 
//  fe_MultiplayerSetup* pParent - 
//
// Return: bool - 
//
bool fe_MS_PlayerSelection::Create( const char* strName, UINT32 uiIndex, UINT32 uiNetID, const char* strSide, UINT8 uiColor, fe_MultiplayerSetup* pParent )
{
	std_Point	pos = pParent->OpenSlot();

	NameDisp = new gadget_String;
	NameDisp->Create( pos, std_Size(96,20), (wnd_Window*)pParent );
	NameDisp->SetText( strName );
	sprintf( Name, "Player%d", uiIndex );
	NameDisp->SetName( Name );

	SideSel = new gadget_Button;
	SideSel->Create(std_Point(pos.x+284,pos.y), std_Size(96,20), (wnd_Window*)pParent );
	sprintf( Name, "#%.4uSide", uiIndex );
	SideSel->SetName( Name );
	if( strSide==NULL ) Side=theGame.SideData.begin(),SideSel->SetText((*Side).name);
	else if( stricmp(strSide,"WATCH")==0 ) Side=theGame.SideData.end(),SideSel->SetText("WATCH");
	else {
		Side = theGame.SideData.begin();
		for( ;Side!=theGame.SideData.end() && stricmp((*Side).name,strSide)!=0;++Side);
		SideSel->SetText( Side==theGame.SideData.end() ? "---":strSide );
	}

	Color = uiColor;
	ColorSel = new gadget_Pic;
	ColorSel->Create(std_Point(pos.x+170,pos.y), std_Size(16,16), (wnd_Window*)pParent );
	ColorSel->SetText( "0.1" );
	ColorSel->SetFrames( pParent->Logos(), 10 );
	ColorSel->SetFrame( Color );
	sprintf( Name, "#%.4uColor", uiIndex );
	ColorSel->SetName( Name );

	Ready = new gadget_Button;
	Ready->Create(std_Point(pos.x+470,pos.y+2), std_Size(16,16), (wnd_Window*)pParent );
	Ready->SetButtonImages( guiResources.Checkbox.Stages, guiResources.Checkbox.Pressed, guiResources.Checkbox.Disabled, 2 );
	sprintf( Name, "#%.4uReady", uiIndex );
	Ready->SetName( Name );

	strcpy( Name, NameDisp->GetText() ),
	//NameDisp->SetText( NameDisp->GetName() );
	NetID = uiNetID;
	Valid = 0xFACEBAAC;
	return true;
}
// End fe_MS_PlayerSelection::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MS_PlayerSelection::Destroy() //             \author Logan Jones
/////////////////////////////////////                \date 05-06-2003
//
//===================================================================
//
void fe_MS_PlayerSelection::Destroy( fe_MultiplayerSetup* pParent )
{
	if( Valid==0xFACEBAAC ) {
		pParent->Replace( Color, NameDisp->GetWndPos() );
		SAFE_DESTROY_AND_DELETE( NameDisp );
		SAFE_DESTROY_AND_DELETE( SideSel );
		SAFE_DESTROY_AND_DELETE( ColorSel );
		Valid = 0;
	}
}
// End fe_MS_PlayerSelection::Destroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// fe_MS_PlayerSelection::Send() //                \author Logan Jones
//////////////////////////////////                   \date 05-06-2003
//
//===================================================================
// Parameters:
//  UINT32 uiTo     - 
//  UINT32 uiIndex  - 
//  UINT32 uiPrefix - 
//
void fe_MS_PlayerSelection::Send( UINT32 uiTo, UINT32 uiIndex, UINT32 uiPrefix )
{
	if(uiPrefix) net.Write().Write(uiTo,4) << uiPrefix;
	net.Write().Write(uiTo,6 + strlen(Name) + strlen(SideSel->GetText()))
		<< uiIndex
		<< (UINT8)(uiTo==NetID)
		<< Name
		<< SideSel->GetText()
		<< Color;
}
// End fe_MS_PlayerSelection::Send()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_MS_PlayerSelection.cpp //
////////////////////////////////////