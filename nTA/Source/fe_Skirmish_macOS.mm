// fe_Skirmish_macOS.mm //                        \author Logan Jones
//////////////////////////                            \date 3/29/2020
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "fe.h"
#include "fe_Skirmish.h"
#include "gadget_Button.h"
#include "gadget_String.h"
#include "game.h"
#import <Foundation/Foundation.h>


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::QuickStart() //                    \author Logan Jones
//////////////////////////////                         \date 3/29/2020
//               
//====================================================================
// Return: BOOL - 
//
BOOL fe_Skirmish::QuickStart()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
    // Get the skirmish settings
    theGame.Settings.Difficulty = [defaults integerForKey:@"SkirmishDifficulty"];
    theGame.Settings.CommanderDeath = [defaults integerForKey:@"SkirmishCommanderDeath"];
    theGame.Settings.LineOfSight = [defaults integerForKey:@"SkirmishLineOfSight"];
    theGame.Settings.LOSType = [defaults integerForKey:@"SkirmishLOSType"];
    theGame.Settings.Location = [defaults integerForKey:@"SkirmishLocation"];
    theGame.Settings.Mapping = [defaults integerForKey:@"SkirmishMapping"];
    NSString *map = [defaults stringForKey:@"SkirmishMap"] ?: @"Coast To Coast";
    [map getCString:theGame.Settings.Map maxLength:sizeof(theGame.Settings.Map) encoding:NSASCIIStringEncoding];

    // Get the player settings
    // TODO: Use the actual settings
    theGame.SetPlayerCount( 2 );
    theGame.SetPlayer( 0, PT_LOCAL_USER, "CORE", 0, 1000, 1000 );
    theGame.SetPlayer( 1, PT_AI, "ARM", 1, 1000, 1000 );

    return TRUE;
}
// End fe_Skirmish::QuickStart()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::LoadSettings() //                  \author Logan Jones
////////////////////////////////                       \date 3/29/2020
//               
//====================================================================
//
void fe_Skirmish::LoadSettings()
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSInteger value;
    
    value = [defaults integerForKey:@"SkirmishDifficulty"];
    ((gadget_Button*)GetGadget("Difficulty"))->SetStage( (DWORD)value );
    
    value = [defaults integerForKey:@"SkirmishCommanderDeath"];
    ((gadget_Button*)GetGadget("CommanderDeath"))->SetStage( (DWORD)value );
    
    value = [defaults integerForKey:@"SkirmishLineOfSight"];
    ((gadget_Button*)GetGadget("LineOfSight"))->SetStage( (DWORD)value );
    
//    value = [defaults integerForKey:@"SkirmishLOSType"];
//    ((gadget_Button*)GetGadget("CommanderDeath"))->SetStage( (DWORD)Value );
    
    value = [defaults integerForKey:@"SkirmishLocation"];
    ((gadget_Button*)GetGadget("StartLocation"))->SetStage( (DWORD)value );
    
    value = [defaults integerForKey:@"SkirmishMapping"];
    ((gadget_Button*)GetGadget("Mapping"))->SetStage( (DWORD)value );
    
    NSString *map = [defaults stringForKey:@"SkirmishMap"] ?: @"Coast To Coast";
    ((gadget_String*)GetGadget("MapName"))->SetText( [map UTF8String] );
}
// End fe_Skirmish::LoadSettings()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// fe_Skirmish::SaveSettings() //                  \author Logan Jones
////////////////////////////////                       \date 3/29/2020
//               
//====================================================================
//
void fe_Skirmish::SaveSettings()
{
    theGame.Settings.Difficulty = ((gadget_Button*)GetGadget("Difficulty"))->GetStage();
    theGame.Settings.CommanderDeath = ((gadget_Button*)GetGadget("CommanderDeath"))->GetStage();
    theGame.Settings.LineOfSight = ((gadget_Button*)GetGadget("LineOfSight"))->GetStage();
//    theGame.Settings.LOSType = ((gadget_Button*)GetGadget("Difficulty"))->GetStage();
    theGame.Settings.Location = ((gadget_Button*)GetGadget("StartLocation"))->GetStage();
    theGame.Settings.Mapping = ((gadget_Button*)GetGadget("Mapping"))->GetStage();
    strcpy( theGame.Settings.Map, ((gadget_String*)GetGadget("MapName"))->GetText() );

    // Get the player settings
    // TODO: Use the actual settings
    theGame.SetPlayerCount( 2 );
    theGame.SetPlayer( 0, PT_LOCAL_USER, "CORE", 0, 1000, 1000 );
    theGame.SetPlayer( 1, PT_AI, "ARM", 1, 1000, 1000 );
}
// End fe_Skirmish::SaveSettings()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - fe_Skirmish_macOS.mm //
////////////////////////////////
