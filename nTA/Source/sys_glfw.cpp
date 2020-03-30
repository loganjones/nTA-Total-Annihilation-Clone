//
//  sys_glfw.cpp
//  nTA
//
//  Created by Logan Jones on 5/21/16.
//  Copyright © 2016 Logan Jones. All rights reserved.
//

#include <stdio.h>
#include <GLFW/glfw3.h>
#include "sys.h"


int main( void )
{
    /* Init GLFW */
    if( !glfwInit() )
        exit( EXIT_FAILURE );
    
    assertEx( sys!=NULL, "sys hasn't been assigned yet! BAD BAD BAD" );
    sys->Run();
    
    glfwTerminate();
    return sys->GetExitCode();
}


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
    GLFWwindow *window = glfwCreateWindow( 640, 480, m_PlatformParameters.MainWindowName, NULL, NULL );
    if (!window)
    {
        ErrorBox( "Error -> sys_App::Create()", "Failed to create window!" );
        return FALSE;
    }
    
    m_PlatformParameters.mainWindow = window;
    glfwSetWindowUserPointer(window, this);
    
//    glfwSetFramebufferSizeCallback(window, reshape);
//    glfwSetKeyCallback(window, key_callback);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
//    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval( 1 );
    
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
    if( m_PlatformParameters.mainWindow )
    {
        glfwDestroyWindow( m_PlatformParameters.mainWindow );
        m_PlatformParameters.mainWindow = NULL;
    }
}
// End sys_App::Destroy()
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
    // GLFW uses PollEvents() to check & process platform specific events
    // that have been queued up by the system.
    // This will run the callbacks (window, input, etc) associated with those events.
    glfwPollEvents();
    
    // Check if the main window has been given a close message
    // and proceed to ShutDown if so.
    if (glfwWindowShouldClose(m_PlatformParameters.mainWindow)) {
        ChangeState( State_ShutDown );
        return TRUE;
    }
    
    // No message was found, return FALSE
    return FALSE;
}
// End sys_App::CheckForSysMessage()
//////////////////////////////////////////////////////////////////////
