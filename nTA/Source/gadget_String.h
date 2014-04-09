// gadget_String.h //                              \author Logan Jones
///////////////////////                              \date 10/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _gadget_String_H_
#define _gadget_String_H_
/////////////////////////////////////////////////////////////////////


enum gadget_StringFlags
{
	string_CenterHoriz			= ( 1 << 0 ),
};


////////////////////////// gadget_String //////////////////////////
//
class gadget_String : public gui_Gadget
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, ta_gui_String* pStringInfo, wnd_Window* pParent );
	virtual BOOL Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent );

	void OnRender();

	void SetText( LPCTSTR strText );
	LPCTSTR GetText() const { return m_StringInfo.Text; }


/////////////////////////////////////////////////
// Data members
protected:

	ta_gui_String		m_StringInfo;

	DWORD				m_dwFlags;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_String();
	virtual ~gadget_String();
/////////////////////////////////////////////////

}; // End class - gadget_String
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_gadget_String_H_)
