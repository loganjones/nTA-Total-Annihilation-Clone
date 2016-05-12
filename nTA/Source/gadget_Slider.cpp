// gadget_Slider.cpp //                            \author Logan Jones
/////////////////////////                            \date 10/18/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_Slider.h"
#include "gadget_Button.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "gadget_Slider.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_Slider::gadget_Slider():gui_Gadget(),
	m_pBuddyWnd( NULL )
{}
gadget_Slider::~gadget_Slider()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::Create() //                      \author Logan Jones
///////////////////////////////                       \date 10/18/2001
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t* pCommonData - 
//  ta_gui_Slider* pSliderInfo   - 
//  wnd_Window* pParent          - 
//
// Return: BOOL - 
//
BOOL gadget_Slider::Create( ta_gui_Common_t* pCommonData, ta_gui_Slider* pSliderInfo, wnd_Window* pParent )
{
	// Sanity check
	assert( pSliderInfo );

	// Call the default Create() method
	if( bFAILED(gui_Gadget::Create(pCommonData,pParent)) )
	{
		return FALSE;
	}

	// Copy the common data
	memcpy( &m_SliderInfo, pSliderInfo, sizeof(m_SliderInfo) );

	// Set the orientation
	m_bVertical = (m_Size.height>m_Size.width) ? TRUE : FALSE;

	// Create the foward and back buttons for the ticker
	if( bFAILED(CreateButtons()) )
	{
		return FALSE;
	}

	// Create the background strip
	if( bFAILED(CreateStrip()) )
	{
		return FALSE;
	}
	
	// Get the initial length of the knob from the info
	m_Knob_Length = m_SliderInfo.KnobSize;

	// Create the knob
	if( bFAILED(CreateKnob()) )
	{
		return FALSE;
	}

	// Initialize some misc stuff
	m_Mode = MODE_SLIDER;
	m_bDraggingKnob = FALSE;

	// Initialize the knob position and the control range
	SetKnobPosition( m_Knob_MinPos );

	SetRange( 0, m_SliderInfo.Range );
	m_pBuddyWnd = NULL;

	return TRUE;
}
// End gadget_Slider::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnDestroy() //                   \author Logan Jones
//////////////////////////////////                    \date 10/25/2001
//               
//====================================================================
//
void gadget_Slider::OnDestroy()
{
	DestroyStrip();
	DestroyKnob();

	// Call the default
	gui_Gadget::OnDestroy();
}
// End gadget_Slider::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnUpdate() //                    \author Logan Jones
/////////////////////////////////                     \date 10/25/2001
//               
//====================================================================
//
void gadget_Slider::OnUpdate()
{
	if( m_FowardButton.IsDown() )
	{
		if( (std_Time() - m_DownTime) > 500 )
			MoveKnobFoward();
	}
	else if( m_BackButton.IsDown() )
	{
		if( (std_Time() - m_DownTime) > 500 )
			MoveKnobBackward();
	}

	gui_Gadget::OnUpdate();
}
// End gadget_Slider::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnRender() //                    \author Logan Jones
/////////////////////////////////                     \date 10/18/2001
//               
//====================================================================
//
void gadget_Slider::OnRender()
{
	gfx->RenderSurfaceEx( m_ScreenPosition + m_Strip_ImagePos, m_Strip_Image );
	gfx->RenderSurfaceEx( m_ScreenPosition + m_Knob_ImageRect.Position(), m_Knob_Image );

	gui_Gadget::OnRender();
}
// End gadget_Slider::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnWndMessage() //                \author Logan Jones
/////////////////////////////////////                 \date 10/25/2001
//               
//====================================================================
// Parameters:
//  wnd_Window* pSender - 
//  DWORD dwMessage     - 
//  DWORD dwParamA      - 
//  DWORD dwParamB      - 
//
// Return: DWORD - 
//
DWORD gadget_Slider::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB )
{
	LPTSTR				strSender = (LPTSTR)dwParamA;

	if( dwMessage==gui_msg_ButtonDown )
	{
		STRING_SWITCH( strSender )

			CASE( "FOWARD" )
				MoveKnobFoward();
				m_DownTime = std_Time();
				return 1;

			CASE( "BACK" )
				MoveKnobBackward();
				m_DownTime = std_Time();
				return 1;

		END_STRING_SWITCH

	} // end if( dwMessage==msg_ButtonPressed )

	return gui_Gadget::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End gadget_Slider::OnWndMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnCursorMove() //                \author Logan Jones
/////////////////////////////////////                 \date 10/25/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwFlags         - 
//
void gadget_Slider::OnCursorMove( const std_Point_t& ptCursor, DWORD dwFlags )
{
	if( m_bDraggingKnob && m_pWndManager->GetCapture()==this )
		if( m_bVertical ) MoveKnobNear( ptCursor.y );
		else MoveKnobNear( ptCursor.x );
}
// End gadget_Slider::OnCursorMove()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::OnMouseButton() //               \author Logan Jones
//////////////////////////////////////                \date 10/25/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void gadget_Slider::OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// Check if this is the primary button
	if( dwButton==1 )
	{
		// What happened
		if( bDown )
		{
			// Check if the the button was pressed over the knob
			if( m_Knob_ImageRect.PointInRect(ptCursor) )
			{
				m_bDraggingKnob = TRUE;
			}
			else // button was pressed over the strip
			{
				m_bDraggingKnob = TRUE;
				if( m_bVertical ) MoveKnobNear( ptCursor.y );
				else MoveKnobNear( ptCursor.x );
			}

			SetCapture();
		}
		else // Button 1 was released
		{
			m_bDraggingKnob = FALSE;
			ReleaseCapture();
		}
	} // end if( dwButton==1 )
	else gui_Gadget::OnMouseButton( ptCursor, dwButton, bDown, dwFlags );
}
// End gadget_Slider::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::CreateButtons() //               \author Logan Jones
//////////////////////////////////////                \date 11/25/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gadget_Slider::CreateButtons()
{
	ta_gui_Common_t			FowardButtonCommonData;
	ta_gui_Common_t			BackButtonCommonData;
	ta_gui_Button			ButtonData;

	// Initialize the button data
	ZeroMemory( &ButtonData, sizeof(ButtonData) );
	ZeroMemory( &FowardButtonCommonData, sizeof(FowardButtonCommonData) );
	ZeroMemory( &BackButtonCommonData, sizeof(BackButtonCommonData) );
	FowardButtonCommonData.Active = BackButtonCommonData.Active = 1;
	strcpy( FowardButtonCommonData.Name, "FOWARD" );
	strcpy( BackButtonCommonData.Name, "BACK" );

	if( m_bVertical )
	{
		// Setup the button common data structures
		BackButtonCommonData.XPos = (m_Size.width / 2) - (guiResources.m_SliderButtonUp[0]->GetSize().width / 2);
		BackButtonCommonData.YPos = 0;
		BackButtonCommonData.Width = BackButtonCommonData.Height = 1;
		FowardButtonCommonData.XPos = (m_Size.width / 2) - (guiResources.m_SliderButtonDown[0]->GetSize().width / 2);
		FowardButtonCommonData.YPos = m_Size.height - guiResources.m_SliderButtonDown[0]->GetSize().height;
		FowardButtonCommonData.Width = FowardButtonCommonData.Height = 1;

		// Create the buttons and assosiate them with their images
		m_FowardButton.Create( &FowardButtonCommonData, &ButtonData, this );
		m_FowardButton.SetButtonImages( guiResources.m_SliderButtonDown, guiResources.m_SliderButtonDown[1], guiResources.m_SliderButtonDown[1] );
		m_BackButton.Create( &BackButtonCommonData, &ButtonData, this );
		m_BackButton.SetButtonImages( guiResources.m_SliderButtonUp, guiResources.m_SliderButtonUp[1], guiResources.m_SliderButtonUp[1] );
	}
	else // if( !m_bVertical )
	{
		// Setup the button common data structures
		BackButtonCommonData.XPos = 0;
		BackButtonCommonData.YPos = (m_Size.height / 2) - (guiResources.m_SliderButtonLeft[0]->GetSize().height / 2);
		BackButtonCommonData.Width = BackButtonCommonData.Height = 1;
		FowardButtonCommonData.XPos = m_Size.width - guiResources.m_SliderButtonRight[0]->GetSize().width;
		FowardButtonCommonData.YPos = (m_Size.height / 2) - (guiResources.m_SliderButtonRight[0]->GetSize().height / 2);
		FowardButtonCommonData.Width = BackButtonCommonData.Height = 1;

		// Create the buttons and assosiate them with their images
		m_FowardButton.Create( &FowardButtonCommonData, &ButtonData, this );
		m_FowardButton.SetButtonImages( guiResources.m_SliderButtonRight, guiResources.m_SliderButtonRight[1], guiResources.m_SliderButtonRight[1] );
		m_BackButton.Create( &BackButtonCommonData, &ButtonData, this );
		m_BackButton.SetButtonImages( guiResources.m_SliderButtonLeft, guiResources.m_SliderButtonLeft[1], guiResources.m_SliderButtonLeft[1] );
	} // end else // if( !m_bVertical )

	return TRUE;
}
// End gadget_Slider::CreateButtons()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::CreateStrip() //                 \author Logan Jones
/////////////////////////////////                     \date 11/25/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gadget_Slider::CreateStrip()
{
	gfx_Image_t				StripImage;
	long					StripLength;

	if( m_bVertical )
	{
		// Get the length of the major side of the strip
		StripLength = m_FowardButton.GetWndPos().y - (m_BackButton.GetWndPos().y + m_BackButton.GetWndSize().height);

		// Setup the image structure
		StripImage.Size.Set( guiResources.m_VerticalStrip[0].Size.width, StripLength );
		StripImage.Stride = guiResources.m_VerticalStrip[0].Stride;
		StripImage.Pitch = StripImage.Stride * StripImage.Size.width;
		StripImage.pBytes = new BYTE[ *StripImage.Size ];

		// Setup the image
		gfx_Interface::BitBlt( std_Point_t(0,0), &StripImage, &guiResources.m_VerticalStrip[0] );
		const long IncY = guiResources.m_VerticalStrip[1].Size.height;
		const long StopY = StripImage.Size.height - guiResources.m_VerticalStrip[2].Size.height;
		for( long y=guiResources.m_VerticalStrip[0].Size.height; y<StopY; y+=IncY)
			gfx_Interface::BitBlt( std_Point_t(0,y), &StripImage, &guiResources.m_VerticalStrip[1] );
		gfx_Interface::BitBlt( std_Point_t(0,StopY), &StripImage, &guiResources.m_VerticalStrip[2] );

		// Create the surface
		gfx->CreateSurface( PIXEL_PALETTISED, &StripImage, &m_Strip_Image );
		m_Strip_ImagePos.Set( (m_Size.width / 2) - (guiResources.m_VerticalStrip[0].Size.width / 2), guiResources.m_SliderButtonUp[0]->GetSize().height );
		delete [] StripImage.pBytes;
	}
	else // if( !m_bVertical )
	{
		// Get the length of the major side of the strip
		StripLength = m_FowardButton.GetWndPos().x - (m_BackButton.GetWndPos().x + m_BackButton.GetWndSize().width);

		// Setup the image structure
		StripImage.Size.Set( StripLength, guiResources.m_HorizontalStrip[0].Size.height );
		StripImage.Stride = guiResources.m_HorizontalStrip[0].Stride;
		StripImage.Pitch = StripImage.Stride * StripImage.Size.width;
		StripImage.pBytes = new BYTE[ *StripImage.Size ];

		// Setup the image
		gfx_Interface::BitBlt( std_Point_t(0,0), &StripImage, &guiResources.m_HorizontalStrip[0] );
		const long IncX = guiResources.m_HorizontalStrip[1].Size.width;
		const long StopX = StripImage.Size.width - guiResources.m_HorizontalStrip[2].Size.width;
		for( long x=guiResources.m_HorizontalStrip[0].Size.height; x<StopX; x+=IncX)
			gfx_Interface::BitBlt( std_Point_t(x,0), &StripImage, &guiResources.m_HorizontalStrip[1] );
		gfx_Interface::BitBlt( std_Point_t(StopX,0), &StripImage, &guiResources.m_HorizontalStrip[2] );

		// Create the surface
		gfx->CreateSurface( PIXEL_PALETTISED, &StripImage, &m_Strip_Image );
		m_Strip_ImagePos.Set( guiResources.m_SliderButtonLeft[0]->GetSize().width, (m_Size.height / 2) - (guiResources.m_HorizontalStrip[0].Size.height / 2) );
		delete [] StripImage.pBytes;
	} // end else // if( !m_bVertical )

	return TRUE;
}
// End gadget_Slider::CreateStrip()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::DestroyStrip() //                \author Logan Jones
//////////////////////////////////                    \date 11/25/2001
//               
//====================================================================
//
void gadget_Slider::DestroyStrip()
{
	SAFE_DESTROY( m_Strip_Image );
}
// End gadget_Slider::DestroyStrip()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::CreateKnob() //                  \author Logan Jones
///////////////////////////////////                   \date 11/25/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gadget_Slider::CreateKnob()
{
	gfx_Image_t		KnobImage;

	if( m_bVertical )
	{
		// Set the min and max positions and the rect for the knob
		m_Knob_MinPos = m_BackButton.GetWndPos().y + m_BackButton.GetWndSize().height + 2 + (m_Knob_Length / 2);
		m_Knob_MaxPos = m_FowardButton.GetWndPos().y - m_Knob_Length - 2 + (m_Knob_Length / 2);
		m_Knob_ImageRect.Set(
			std_Point((m_Size.width / 2) - (guiResources.m_VerticalTicker[0].Size.width / 2), m_Knob_CurrentPos - (m_Knob_Length / 2)),
			std_Size(10, m_Knob_Length) );

		// Check if the knob is the standard size
		if( m_Knob_ImageRect.Size()==guiResources.m_StadardTicker.Size )
			gfx->CreateSurface( PIXEL_PALETTISED, &guiResources.m_StadardTicker, &m_Knob_Image );
		else // its not, make a new knob image
		{
			// Setup the image structure
			KnobImage.Size = m_Knob_ImageRect.Size();
			KnobImage.Stride = guiResources.m_StadardTicker.Stride;
			KnobImage.Pitch = KnobImage.Stride * KnobImage.Size.width;
			KnobImage.pBytes = new BYTE[ *KnobImage.Size ];

			// Setup the image
			gfx_Interface::BitBlt( std_Point_t(0,0), &KnobImage, &guiResources.m_VerticalTicker[0] );
			const long IncY = guiResources.m_VerticalTicker[1].Size.height;
			const long StopY = KnobImage.Size.height - guiResources.m_VerticalTicker[2].Size.height;
			for( long y=guiResources.m_VerticalTicker[0].Size.height; y<StopY; y+=IncY)
				gfx_Interface::BitBlt( std_Point_t(0,y), &KnobImage, &guiResources.m_VerticalTicker[1] );
			gfx_Interface::BitBlt( std_Point_t(0,StopY), &KnobImage, &guiResources.m_VerticalTicker[2] );

			// Create the surface
			gfx->CreateSurface( PIXEL_PALETTISED, &KnobImage, &m_Knob_Image );
			delete [] KnobImage.pBytes;
		}
	}
	else // if( !m_bVertical )
	{
		// Set the min and max positions and the rect for the knob
		m_Knob_MinPos = m_BackButton.GetWndPos().x + m_BackButton.GetWndSize().width + 2 + (m_Knob_Length / 2);
		m_Knob_MaxPos = m_FowardButton.GetWndPos().x - m_Knob_Length - 2 + (m_Knob_Length / 2);
		m_Knob_ImageRect.Set(
			std_Point(m_Knob_CurrentPos - (m_Knob_Length / 2), (m_Size.height / 2) - (guiResources.m_HorizontalTicker[0].Size.height / 2)),
			std_Size(m_Knob_Length, 10) );

		// Check if the knob is the standard size
		if( m_Knob_ImageRect.Size()==guiResources.m_StadardTicker.Size )
			gfx->CreateSurface( PIXEL_PALETTISED, &guiResources.m_StadardTicker, &m_Knob_Image );
		else // its not, make a new knob image
		{
			// Setup the image structure
			KnobImage.Size = m_Knob_ImageRect.Size();
			KnobImage.Stride = guiResources.m_StadardTicker.Stride;
			KnobImage.Pitch = KnobImage.Stride * KnobImage.Size.width;
			KnobImage.pBytes = new BYTE[ *KnobImage.Size ];

			// Setup the image
			gfx_Interface::BitBlt( std_Point_t(0,0), &KnobImage, &guiResources.m_HorizontalTicker[0] );
			const long IncX = guiResources.m_HorizontalTicker[1].Size.width;
			const long StopX = KnobImage.Size.width - guiResources.m_HorizontalTicker[2].Size.width;
			for( long x=guiResources.m_HorizontalTicker[0].Size.width; x<StopX; x+=IncX)
				gfx_Interface::BitBlt( std_Point_t(x,0), &KnobImage, &guiResources.m_HorizontalTicker[1] );
			gfx_Interface::BitBlt( std_Point_t(StopX,0), &KnobImage, &guiResources.m_HorizontalTicker[2] );

			// Create the surface
			gfx->CreateSurface( PIXEL_PALETTISED, &KnobImage, &m_Knob_Image );
			delete [] KnobImage.pBytes;
		}
	} // end else // if( !m_bVertical )

	return TRUE;
}
// End gadget_Slider::CreateKnob()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::DestroyKnob() //                 \author Logan Jones
////////////////////////////////////                  \date 11/25/2001
//               
//====================================================================
//
void gadget_Slider::DestroyKnob()
{
	SAFE_DESTROY( m_Knob_Image );
}
// End gadget_Slider::DestroyKnob()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_Slider.cpp //
///////////////////////////////
