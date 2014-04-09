// gui_Gadget.h //                                 \author Logan Jones
/////////////////                                     \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GUI_GADGET_H_
#define _GUI_GADGET_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// gui_Gadget /////////////////////////////
//
class gui_Gadget : public wnd_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, wnd_Window* pParent );

	// Derived render method
	void Render();

	// Accessor methods for the gadget name
	INLINE LPCTSTR GetName() const;
	INLINE void SetName( LPCTSTR strName );

	virtual void OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

	ta_gui_Common_t		m_CommonData;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gui_Gadget();
	virtual ~gui_Gadget();
/////////////////////////////////////////////////

}; // End class - gui_Gadget
/////////////////////////////////////////////////////////////////////


// Include INLINE implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "gui_Gadget.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GUI_GADGET_H_)
