// in_DInput8.h //                                 \author Logan Jones
/////////////////                                     \date 5/3/16
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IN_MAC_H_
#define _IN_MAC_H_
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
class in_Mac
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
// Default Constructor/Deconstructor
public:
    in_Mac();
    virtual ~in_Mac();
/////////////////////////////////////////////////
    
}; // End class - in_Mac
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IN_MAC_H_)
