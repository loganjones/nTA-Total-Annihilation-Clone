// in_GLFW.h //                                   \author Logan Jones
//////////////                                          \date 6/26/16
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IN_GLFW_H_
#define _IN_GLFW_H_
/////////////////////////////////////////////////////////////////////

#include "std_Platform.h"

enum in_Devices : DWORD
{
    in_Keyboard,
    in_Mouse,
};

class in_Handler { public: virtual BOOL HandleInput(in_Devices,DWORD,long) = 0; };

/////////////////////////////// in_Mac //////////////////////////////
//
class in_GLFW
{
    
/////////////////////////////////////////////////
// INTERFACE
public:
    
    BOOL Create( std_PlatformParameters* pPlatformParams );
    void Destroy();
    
    void AcquireDevices();
    void UnacquireDevices();
    
    void SetHandler( in_Handler* pHandler );
    //BOOL CheckWindowsMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam );
    BOOL DumpInputToHandler( in_Handler* pHandler );
    
    
/////////////////////////////////////////////////
// Utility methods
protected:
    
    static void KeyCallback( GLFWwindow* window, int key, int scancode, int action, int mods );
    static void MouseButtonCallback( GLFWwindow* window, int button, int action, int mods );
    static void CursorPositionCallback( GLFWwindow* window, double x, double y );
    
    
/////////////////////////////////////////////////
// Data members
protected:
    
    in_Handler*				m_pHandler;
    
    double                  m_LastMouseX, m_LastMouseY;
    
    
/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
    in_GLFW();
    virtual ~in_GLFW();
/////////////////////////////////////////////////
    
}; // End class - in_GLFW
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IN_GLFW_H_)
