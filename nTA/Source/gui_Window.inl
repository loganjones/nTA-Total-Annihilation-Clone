// gui_Window.inl //                               \author Logan Jones
///////////////////                                   \date 4/22/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::GetGadget() //                      \author Logan Jones
////////////////////////////                          \date 10/14/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: gui_Gadget* - 
//
gui_Gadget* gui_Window::GetGadget( LPCTSTR strName )
{
	GadgetList_t::iterator			it = m_Gadgets.begin();
	GadgetList_t::const_iterator	end= m_Gadgets.end();

	for( ; it!=end; ++it)
		if( stricmp((*it)->GetName(),strName)==0 ) return (*it);
	return NULL;
}
// End gui_Window::GetGadget()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gui_Window.inl //
/////////////////////////
