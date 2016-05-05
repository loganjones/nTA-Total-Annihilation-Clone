//
//  in_Mac.cpp
//  nTA
//
//  Created by Logan Jones on 5/3/16.
//  Copyright © 2016 Logan Jones. All rights reserved.
//

#include "app.h"
#include "input.h"
#include "in_Mac.hpp"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
in_Mac::in_Mac()
{}
in_Mac::~in_Mac()
{
    // Safety precaution
    Destroy();
}
//
/////////////////////////////////////////////////////////////////////



BOOL in_Mac::Create( std_PlatformParameters* pPlatformParams ) {
    return true;
}

void in_Mac::Destroy() {
    
}

void in_Mac::AcquireDevices() {
    
}

void in_Mac::UnacquireDevices() {
    
}

void in_Mac::SetHandler( in_Handler* pHandler ) {
    
}

//BOOL in_Mac::CheckWindowsMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam );

BOOL in_Mac::DumpInputToHandler( in_Handler* pHandler ) {
    return true;
}



/////////////////////////////////////////////////////////////////////
// End - in_Mac.cpp //
/////////////////////
