// sys_macOS.mm //                                \author Logan Jones
//////////////////                                     \date 9/9/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#define INSTANTIATE_SYS_INTERFACE_POINTER
#include "sys.h"

#include "input.h"

#import <Cocoa/Cocoa.h>


//////////////////////////////////////////////////////////////////////
// sys_App::Create() //                            \author Logan Jones
//////////////////////                                 \date 9/24/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSplashPath - 
//
// Return: BOOL - 
//
BOOL sys_App::Create( LPCTSTR strSplashPath )
{
    // TODO: Some macOS specific setup goes here.

	return TRUE;
}
// End sys_App::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Destroy() //                           \author Logan Jones
///////////////////////                                \date 9/24/2001
//               
//====================================================================
//
void sys_App::Destroy()
{
    // TODO: Some macOS specific teardown goes here.
}
// End sys_App::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::Quit() //                              \author Logan Jones
////////////////////                                   \date 9/10/2001
// Description:  Quit is called to safely terminate this run of the app.
//               
//====================================================================
//
void sys_App::Quit()
{
    [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}
// End sys_App::Quit()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::ErrorBox() //                          \author Logan Jones
////////////////////////                               \date 9/29/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strTitle    - 
//  LPCTSTR strErrorMsg - 
//  ...                 - 
//
// Return: BOOL - 
//
BOOL sys_App::ErrorBox( LPCTSTR strTitle, LPCTSTR strErrorMsg, ... )
{
    @autoreleasepool {
        va_list va_ArgList;
        va_start( va_ArgList, strErrorMsg );
        NSString *message = [[NSString alloc] initWithFormat:[[NSString alloc] initWithUTF8String:strErrorMsg] arguments:va_ArgList];
        va_end( va_ArgList );
        
        NSString *title = [[NSString alloc] initWithUTF8String:strTitle];
        
        NSAlert *alert = [[NSAlert alloc] init];
        alert.messageText = title;
        alert.informativeText = message;
        [alert runModal];
    }
	return TRUE;
}
// End sys_App::ErrorBox()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// sys_App::CheckForSysMessage() //                \author Logan Jones
//////////////////////////////////                     \date 9/10/2001
// Description:  Checks the windows message queue.
//               
//====================================================================
// Return: BOOL - Returns TRUE if a message was found and handled, FALSE otherwise
//
BOOL sys_App::CheckForSysMessage()
{

	// No message was found, return FALSE
	return FALSE;
}
// End sys_App::CheckForSysMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - sys_macOS.mm //
////////////////////////
