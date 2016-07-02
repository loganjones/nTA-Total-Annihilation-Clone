//
//  in_GLFW.cpp
//  nTA
//
//  Created by Logan Jones on 5/3/16.
//  Copyright © 2016 Logan Jones. All rights reserved.
//

#include "app.h"
#include "input.h"
#include "in_GLFW.h"


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
in_GLFW::in_GLFW():
m_pHandler(NULL),
m_LastMouseX(0),
m_LastMouseY(0)
{}
in_GLFW::~in_GLFW()
{
    // Safety precaution
    Destroy();
}
//
/////////////////////////////////////////////////////////////////////



BOOL in_GLFW::Create( std_PlatformParameters* pPlatformParams ) {
    
    GLFWwindow *window = pPlatformParams->mainWindow;
    
    glfwSetKeyCallback(window, &KeyCallback);
    glfwSetMouseButtonCallback(window, &MouseButtonCallback);
    glfwSetCursorPosCallback(window, &CursorPositionCallback);
    
    return true;
}

void in_GLFW::Destroy() {
    
}

void in_GLFW::AcquireDevices() {
    
}

void in_GLFW::UnacquireDevices() {
    
}

void in_GLFW::SetHandler( in_Handler* pHandler ) {
    m_pHandler = pHandler;
}

//BOOL in_GLFW::CheckWindowsMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam );

BOOL in_GLFW::DumpInputToHandler( in_Handler* pHandler ) {
    return true;
}


void in_GLFW::KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods ) {
    long data = 0;
    switch (action) {
        case GLFW_PRESS: data = 0x80; break;
    }
    input.m_pHandler->HandleInput(in_Keyboard, key, data);
}

void in_GLFW::MouseButtonCallback( GLFWwindow* window, int button, int action, int mods ) {
    long data = 0;
    switch (action) {
        case GLFW_PRESS: data = 0x80; break;
    }
    input.m_pHandler->HandleInput(in_Mouse, button, data);
}

void in_GLFW::CursorPositionCallback( GLFWwindow* window, double x, double y ) {
    const long dx = x - input.m_LastMouseX;
    const long dy = y - input.m_LastMouseY;
    if (dx != 0) input.m_pHandler->HandleInput(in_Mouse, IN_MOUSE_AXIS_X, dx);
    if (dy != 0) input.m_pHandler->HandleInput(in_Mouse, IN_MOUSE_AXIS_Y, dy);
    input.m_LastMouseX = x;
    input.m_LastMouseY = y;
}


/////////////////////////////////////////////////////////////////////
// End - in_GLFW.cpp //
/////////////////////
