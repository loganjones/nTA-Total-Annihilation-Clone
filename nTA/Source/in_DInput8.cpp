// in_DInput8.cpp //                               \author Logan Jones
///////////////////                                   \date 9/29/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "app.h"
#include "input.h"
#include "in_DInput8.h"

#define STANDARD_BUFFER_SIZE		( 32 )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
in_DInput8::in_DInput8():
    m_lpDI(NULL),
    m_lpDefaultKeyboard(NULL),
    m_lpDefaultMouse(NULL)
{}
in_DInput8::~in_DInput8()
{
    // Safety precaution
    Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::Create() //                         \author Logan Jones
/////////////////////////                              \date 9/29/2001
//               
//====================================================================
// Parameters:
//  std_PlatformParameters* pPlatformParams - 
//
// Return: BOOL - 
//
BOOL in_DInput8::Create( std_PlatformParameters* pPlatformParams )
{
	LRESULT			lr;
	DIPROPDWORD     dipdw;

    // Create the Direct Input Object
	if( FAILED(DirectInput8Create(
		pPlatformParams->hInstance,
		DIRECTINPUT_VERSION, 
		IID_IDirectInput8,
		(void**)&m_lpDI,
		NULL)) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the DirectInput8 interface object." );
		return FALSE;
	}

	// Create the default keyboard device
	if( FAILED(lr=CreateDevice(
		GUID_SysKeyboard,
		&m_lpDefaultKeyboard,
		&c_dfDIKeyboard,
		pPlatformParams,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE )) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the default keyboard device." );
		return FALSE;
	}

	// Create the default mouse device
	if( FAILED(lr=CreateDevice(
		GUID_SysMouse,
		&m_lpDefaultMouse,
		&c_dfDIMouse2,
		pPlatformParams,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE )) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the default mouse device." );
		return FALSE;
	}

    // Setup the data buffer size
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj        = 0;
    dipdw.diph.dwHow        = DIPH_DEVICE;
    dipdw.dwData            = STANDARD_BUFFER_SIZE;

	// Set this property
	m_lpDefaultKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
	m_lpDefaultMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

	// Acquire the devices
	AcquireDevices();

	RECT					WindowRect;
	GetWindowRect( GetDesktopWindow(), &WindowRect );
	m_CursorPos.Set( (WindowRect.right - WindowRect.left) / 2, (WindowRect.bottom - WindowRect.top) / 2 );
	SetCursorPos( m_CursorPos.x, m_CursorPos.y );

	return TRUE;
}
// End in_DInput8::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::Destroy() //                        \author Logan Jones
//////////////////////////                             \date 9/29/2001
//               
//====================================================================
//
void in_DInput8::Destroy()
{
	UnacquireDevices();

	SAFE_RELEASE( m_lpDefaultKeyboard );
	SAFE_RELEASE( m_lpDefaultMouse );
	SAFE_RELEASE( m_lpDI );
}
// End in_DInput8::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::CreateDevice() //                   \author Logan Jones
///////////////////////////////                        \date 9/29/2001
//               
//====================================================================
// Parameters:
//  REFGUID rguid                               - 
//  LPDIRECTINPUTDEVICE8* lplpDirectInputDevice - 
//  LPCDIDATAFORMAT lpdf                        - 
//  std_PlatformParameters* pPlatformParams     - 
//  DWORD dwFlags                               - 
//
// Return: LRESULT - 
//
LRESULT in_DInput8::CreateDevice( REFGUID rguid, LPDIRECTINPUTDEVICE8* lplpDirectInputDevice, LPCDIDATAFORMAT lpdf, std_PlatformParameters* pPlatformParams, DWORD dwFlags )
{
	LRESULT			lr;

	// Create the device
	if( FAILED(lr=m_lpDI->CreateDevice(
		rguid,
		lplpDirectInputDevice,
		NULL )) )
	{
		return lr;
	}

	// Set the data format
	if( FAILED(lr=(*lplpDirectInputDevice)->SetDataFormat(lpdf)) )
	{
		return lr;
	}

	// Set the coop level
	if( FAILED(lr=(*lplpDirectInputDevice)->SetCooperativeLevel(
		pPlatformParams->hMainWindow,
		dwFlags )) )
	{
		return lr;
	}

	// Return success
	return lr;
}
// End in_DInput8::CreateDevice()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::AcquireDevices() //                 \author Logan Jones
/////////////////////////////////                      \date 9/29/2001
//               
//====================================================================
//
void in_DInput8::AcquireDevices()
{
#define SAFE_ACQUIRE( LPDEVICE )	if( LPDEVICE ) (LPDEVICE)->Acquire()

	SAFE_ACQUIRE( m_lpDefaultKeyboard );
	SAFE_ACQUIRE( m_lpDefaultMouse );

#undef SAFE_ACQUIRE
}
// End in_DInput8::AcquireDevices()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::UnacquireDevices() //               \author Logan Jones
///////////////////////////////////                    \date 9/29/2001
//               
//====================================================================
//
void in_DInput8::UnacquireDevices()
{
#define SAFE_UNACQUIRE( LPDEVICE )	if( LPDEVICE ) (LPDEVICE)->Unacquire()

	SAFE_UNACQUIRE( m_lpDefaultKeyboard );
	SAFE_UNACQUIRE( m_lpDefaultMouse );

#undef SAFE_UNACQUIRE
}
// End in_DInput8::UnacquireDevices()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::SetHandler() //                     \author Logan Jones
/////////////////////////////                           \date 4/7/2002
//               
//====================================================================
// Parameters:
//  in_Handler* pHandler - 
//
void in_DInput8::SetHandler( in_Handler* pHandler )
{
	m_pHandler = pHandler;
}
// End in_DInput8::SetHandler()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::CheckWindowsMessage() //            \author Logan Jones
//////////////////////////////////////                  \date 4/7/2002
//               
//====================================================================
// Parameters:
//  UINT uiMsg    - 
//  WPARAM wParam - 
//  LPARAM lParam - 
//
// Return: BOOL - 
//
BOOL in_DInput8::CheckWindowsMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam )
{
	// What do we have here
	switch( uiMsg )
	{
		case WM_MOUSEMOVE:
		{
			std_Point	NewCursorPos( LOWORD(lParam), HIWORD(lParam) );
			std_Point	Change( NewCursorPos - m_CursorPos );

	//		if(Change.x) m_pHandler->HandleInput(in_Mouse,IN_MOUSE_AXIS_X,Change.x);
	//		if(Change.y) m_pHandler->HandleInput(in_Mouse,IN_MOUSE_AXIS_Y,Change.y);
	//		SetCursorPos( m_CursorPos.x, m_CursorPos.y );	
			break;
		}
	} // end switch( uiMsg )

	// We did nothing
	return FALSE;
}
// End in_DInput8::CheckWindowsMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// in_DInput8::DumpInputToProc() //                \author Logan Jones
//////////////////////////////////                    \date 10/15/2001
//               
//====================================================================
// Parameters:
//  in_Handler* pHandler - 
//
// Return: BOOL - 
//
BOOL in_DInput8::DumpInputToHandler( in_Handler* pHandler )
{
	DIDEVICEOBJECTDATA      DeviceObjectData[ STANDARD_BUFFER_SIZE ];
    DWORD                   NumElements;
    HRESULT                 hResult;
    DWORD                   count;

    // Get the device data from the mouse
	if( m_lpDefaultMouse!=NULL )
	{
		NumElements = STANDARD_BUFFER_SIZE;
		hResult = m_lpDefaultMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), DeviceObjectData, &NumElements, 0 );
		switch( hResult )
		{
			case DI_BUFFEROVERFLOW:
			//	assertEx(0,"Overflow");
			//	break;
			case DI_OK:
				for( count=0; count<NumElements; ++count)
					pHandler->HandleInput( in_Mouse, DeviceObjectData[count].dwOfs, DeviceObjectData[count].dwData );
				break;
			case DIERR_INPUTLOST:
				AcquireDevices();
				break;
			//default:
			//	assert(0);
		}
	}

    // Get the device data from the mouse
	if( m_lpDefaultKeyboard!=NULL )
	{
		NumElements = STANDARD_BUFFER_SIZE;
		hResult = m_lpDefaultKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), DeviceObjectData, &NumElements, 0 );
		switch( hResult )
		{
			case DI_BUFFEROVERFLOW:
			//	assertEx(0,"Overflow");
			//	break;
			case DI_OK:
				for( count=0; count<NumElements; ++count)
					pHandler->HandleInput( in_Keyboard, DeviceObjectData[count].dwOfs, DeviceObjectData[count].dwData );
				break;
			case DIERR_INPUTLOST:
				AcquireDevices();
				break;
			//default:
			//	assert(0);
		}
	}

	return TRUE;
}
// End in_DInput8::DumpInputToProc()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - in_DInput8.cpp //
/////////////////////////
