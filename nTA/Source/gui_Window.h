// gui_Window.h //                                 \author Logan Jones
/////////////////                                     \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GUI_WINDOW_H_
#define _GUI_WINDOW_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// gui_Window /////////////////////////////
//
class gui_Window : public wnd_Window
{
	typedef list< gui_Gadget* >	GadgetList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Loads and processes the given gui file
	BOOL LoadGui( LPCTSTR strGuiName );

	// Retrieves a pointer to the desired gadget
	INLINE gui_Gadget* GetGadget( LPCTSTR strName );

	// Default load panel method. Returns FALSE
	virtual BOOL LoadPanel( LPCTSTR strPanel );


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();
	void OnDestroy();

	void OnRender();

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility

	virtual void ApplyGuiHeader( ta_gui_Common_t& Common, ta_gui_Header_t& Header );
	virtual void CreateButton( ta_gui_Common_t& Common, ta_gui_Button& Info );
	virtual void CreateListBox( ta_gui_Common_t& Common, ta_gui_ListBox& Info );
	virtual void CreateTextEdit( ta_gui_Common_t& Common, ta_gui_TextEdit& Info );
	virtual void CreateSlider( ta_gui_Common_t& Common, ta_gui_Slider& Info );
	virtual void CreateString( ta_gui_Common_t& Common, ta_gui_String& Info );
	virtual void CreatePic( ta_gui_Common_t& Common, ta_gui_Pic& Info );
	virtual void CreateFont( ta_gui_Common_t& Common, ta_gui_Font& Info );


/////////////////////////////////////////////////
// Data members
protected:

	// List of all gadgets allocated by this class
	GadgetList_t		m_Gadgets;

	// Background panel for the window
	gfx_Surface*		m_Panel;

	// Gadgets invoked on key commands
	gui_Gadget*			m_EscDefault;
	gui_Gadget*			m_CrDefault;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gui_Window();
	virtual ~gui_Window();
/////////////////////////////////////////////////

}; // End class - gui_Window
/////////////////////////////////////////////////////////////////////


// Include INLINE implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "gui_Window.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GUI_WINDOW_H_)
