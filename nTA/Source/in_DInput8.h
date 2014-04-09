// in_DInput8.h //                                 \author Logan Jones
/////////////////                                     \date 9/29/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IN_DINPUT8_H_
#define _IN_DINPUT8_H_
/////////////////////////////////////////////////////////////////////


#include <basetsd.h>
#define DIRECTINPUT_VERSION         0x0800
#include <dinput.h>
#include <dinputd.h>


enum in_Devices
{
	in_Keyboard,
	in_Mouse,

	FORCE_DWORD(in_Devices)
};

class in_Handler { public: virtual BOOL HandleInput(in_Devices,DWORD,long) = 0; };

///////////////////////////// in_DInput8 ////////////////////////////
//
class in_DInput8
{

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( std_PlatformParameters* pPlatformParams );
	void Destroy();

	void AcquireDevices();
	void UnacquireDevices();

	void SetHandler( in_Handler* pHandler );
	BOOL CheckWindowsMessage( UINT uiMsg, WPARAM wParam, LPARAM lParam );
	BOOL DumpInputToHandler( in_Handler* pHandler );


/////////////////////////////////////////////////
// Utility methods
protected:

	LRESULT CreateDevice( REFGUID rguid, LPDIRECTINPUTDEVICE8* lplpDirectInputDevice, LPCDIDATAFORMAT lpdf, std_PlatformParameters* pPlatformParams, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

    // Pointer to the Direct Input object
    LPDIRECTINPUT8			m_lpDI;

    // Default keyboard device
    LPDIRECTINPUTDEVICE8	m_lpDefaultKeyboard;

    // Default mouse device
	LPDIRECTINPUTDEVICE8	m_lpDefaultMouse;

	in_Handler*				m_pHandler;

	std_Point				m_CursorPos;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	in_DInput8();
	virtual ~in_DInput8();
/////////////////////////////////////////////////

}; // End class - in_DInput8
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IN_DINPUT8_H_)
