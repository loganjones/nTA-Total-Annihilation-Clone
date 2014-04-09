// wnd_Manager.inl //                              \author Logan Jones
////////////////////                                   \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::SetCursorSpeed() //                \author Logan Jones
//////////////////////////////////                      \date 4/1/2002
//               
//====================================================================
// Parameters:
//  const float fSpeed - 
//
void wnd_Manager::SetCursorSpeed( const float fSpeed )
{
	m_CursorSpeed = fSpeed;
}
// End wnd_Manager::SetCursorSpeed()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::GetCursorPosition() //             \author Logan Jones
/////////////////////////////////////                   \date 4/2/2002
//               
//====================================================================
// Return: const std_Point - 
//
const std_Point wnd_Manager::GetCursorPosition() const
{
	return m_Cursor;
}
// End wnd_Manager::GetCursorPosition()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// wnd_Manager::ProccessMessage() //               \author Logan Jones
///////////////////////////////////                    \date 4/21/2002
//               
//====================================================================
// Parameters:
//  const Message_t& Message - 
//
void wnd_Manager::ProccessMessage( const Message_t& Message )
{
	Message.Dest->OnWndMessage( Message.Src, Message.Message, Message.ParamA, Message.ParamB );
}
// End wnd_Manager::ProccessMessage()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - wnd_Manager.inl //
//////////////////////////
