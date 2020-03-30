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


/////////////////////////////////////////////////////////////////////
// End - sys_macOS.mm //
////////////////////////