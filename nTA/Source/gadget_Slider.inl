// gadget_Slider.inl //                            \author Logan Jones
//////////////////////                                \date 5/17/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetRange() //                    \author Logan Jones
/////////////////////////////////                     \date 10/25/2001
//               
//====================================================================
// Parameters:
//  long lMin - 
//  long lMax - 
//
void gadget_Slider::SetRange( long lMin, long lMax )
{
	// Assume Min is less than max
	assert( lMin<lMax );

	// Set the values
	m_MinRange = lMin;
	m_MaxRange = lMax;

	// Reset the tick freq if the control is in ticker mode
	if( m_Mode==MODE_TICKER )
		SetTickFreq( m_TickFreq );
}
// End gadget_Slider::SetRange()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetPosition() //                 \author Logan Jones
/////////////////////////////////                      \date 5/17/2002
//               
//====================================================================
// Parameters:
//  long lPos - 
//
void gadget_Slider::SetPosition( long lPos )
{
	float fPos = float(lPos - m_MinRange) / (m_MaxRange - m_MinRange);
	long lNewPos = long(fPos * (m_Knob_MaxPos - m_Knob_MinPos)) + m_Knob_MinPos;
	SetKnobPosition( lNewPos );
}
// End gadget_Slider::SetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::GetPosition() //                 \author Logan Jones
/////////////////////////////////                      \date 5/17/2002
//               
//====================================================================
// Return: long - 
//
long gadget_Slider::GetPosition() const
{
	return (long((float(m_Knob_CurrentPos - m_Knob_MinPos) / (m_Knob_MaxPos - m_Knob_MinPos)) * (m_MaxRange - m_MinRange)) + m_MinRange);
}
// End gadget_Slider::GetPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetTickFreq() //                 \author Logan Jones
/////////////////////////////////                      \date 5/17/2002
//               
//====================================================================
// Parameters:
//  long lFreq - 
//
void gadget_Slider::SetTickFreq( long lFreq )
{
	const long Range = m_MaxRange - m_MinRange;
	const long NumTicks = (Range / lFreq) + ((Range % lFreq)==0 ? 0:1) + 1;

	assert( lFreq<=Range );

	// Fill the tick array
	m_TickArray.resize( NumTicks );
	m_PosArray.resize( NumTicks );
	for( long n=0; n<(NumTicks-1); ++n)
		m_TickArray[n] = m_MinRange + n*lFreq,
		m_PosArray[n] = m_Knob_MinPos + long((float(m_TickArray[n] - m_MinRange) / Range) * (m_Knob_MaxPos - m_Knob_MinPos));
	m_TickArray.back() = m_MaxRange;
	m_PosArray.back() = m_Knob_MaxPos;

	// Save the freq and set ticker mode
	m_TickFreq = lFreq;
	m_Mode = MODE_TICKER;
}
// End gadget_Slider::SetTickFreq()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetKnobRatio() //                \author Logan Jones
//////////////////////////////////                      \date 8/4/2002
//               
//====================================================================
// Parameters:
//  float fRatio - 
//
void gadget_Slider::SetKnobRatio( float fRatio )
{
	long	NewLength = (m_Knob_MaxPos - m_Knob_MinPos + m_Knob_Length) * fRatio;

	if( NewLength==m_Knob_Length )
		return;

	DestroyKnob();
	m_Knob_Length = NewLength;
	CreateKnob();
}
// End gadget_Slider::SetKnobRatio()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetBuddyWnd() //                 \author Logan Jones
/////////////////////////////////                       \date 8/4/2002
//               
//====================================================================
// Parameters:
//  wnd_Window* pWnd - 
//
void gadget_Slider::SetBuddyWnd( wnd_Window* pWnd )
{
	m_pBuddyWnd = pWnd;
}
// End gadget_Slider::SetBuddyWnd()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SetKnobPosition() //             \author Logan Jones
/////////////////////////////////////                  \date 5/17/2002
//               
//====================================================================
// Parameters:
//  long lPos - 
//
void gadget_Slider::SetKnobPosition( long lPos )
{
	if( m_bVertical )
		m_Knob_ImageRect.Shift( 0, lPos - m_Knob_CurrentPos );
	else // if( !m_bVertical )
		m_Knob_ImageRect.Shift( lPos - m_Knob_CurrentPos, 0 );

	m_Knob_CurrentPos = lPos;
}
// End gadget_Slider::SetKnobPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::MoveKnobNear() //                \author Logan Jones
//////////////////////////////////                     \date 5/17/2002
//               
//====================================================================
// Parameters:
//  long lWhere - 
//
void gadget_Slider::MoveKnobNear( long lWhere )
{
	if( m_Mode==MODE_TICKER )
	{
		const long Size = m_PosArray.size();
		long Dist = 1000, i=0, Closest=1000; 
		for( long n=0; n<Size; ++n)
		{
			Dist = abs( m_PosArray[n] - lWhere );
			if( Dist<Closest )
			{
				Closest = Dist;
				i = n;
			}
		}
		lWhere = m_PosArray[i];
		if(lWhere != m_Knob_CurrentPos)
		{
			SetKnobPosition( lWhere );
			SendKnobMovedMessage();
		}
	}
	else // m_Mode==MODE_SLIDER
	{
		if( lWhere<m_Knob_MinPos ) lWhere = m_Knob_MinPos;
		else if( lWhere>m_Knob_MaxPos ) lWhere = m_Knob_MaxPos;
		if(lWhere != m_Knob_CurrentPos)
		{
			SetKnobPosition( lWhere );
			SendKnobMovedMessage();
		}
	}
}
// End gadget_Slider::MoveKnobNear()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::MoveKnobFoward() //              \author Logan Jones
////////////////////////////////////                   \date 5/17/2002
//               
//====================================================================
//
void gadget_Slider::MoveKnobFoward()
{
	long NewPosition = m_Knob_CurrentPos;

	if( m_Mode==MODE_TICKER )
	{
		if( m_Knob_CurrentPos!=m_Knob_MaxPos )
		{
			const long Size = m_PosArray.size();
			for( long n=0; n<Size && m_PosArray[n]!=m_Knob_CurrentPos; ++n);
			assert( n+1<Size );
			SetKnobPosition( m_PosArray[n+1] );
			SendKnobMovedMessage();
		}
	}
	else // m_Mode==MODE_SLIDER
	{
		if(++NewPosition <= m_Knob_MaxPos)
		{
			SetKnobPosition( NewPosition );
			SendKnobMovedMessage();
		}
	}
}
// End gadget_Slider::MoveKnobFoward()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::MoveKnobBackward() //            \author Logan Jones
//////////////////////////////////////                 \date 5/17/2002
//               
//====================================================================
//
void gadget_Slider::MoveKnobBackward()
{
	long NewPosition = m_Knob_CurrentPos;

	if( m_Mode==MODE_TICKER )
	{
		if( m_Knob_CurrentPos!=m_Knob_MinPos )
		{
			const long Size = m_PosArray.size();
			for( long n=0; n<Size && m_PosArray[n]!=m_Knob_CurrentPos; ++n);
			assert( n-1>=0 );
			SetKnobPosition( m_PosArray[n-1] );
			SendKnobMovedMessage();
		}
	}
	else // m_Mode==MODE_SLIDER
	{
		if(--NewPosition >= m_Knob_MinPos)
		{
			SetKnobPosition( NewPosition );
			SendKnobMovedMessage();
		}
	}
}
// End gadget_Slider::MoveKnobBackward()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_Slider::SendKnobMovedMessage() //        \author Logan Jones
//////////////////////////////////////////              \date 8/4/2002
//               
//====================================================================
//
void gadget_Slider::SendKnobMovedMessage()
{
	SendMessage( (m_pBuddyWnd) ? m_pBuddyWnd : m_pParent, gui_msg_SliderMoved, (DWORD)(LPTSTR(m_CommonData.Name)), GetPosition() );
}
// End gadget_Slider::SendKnobMovedMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_Slider.inl //
////////////////////////////
