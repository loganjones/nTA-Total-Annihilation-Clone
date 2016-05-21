// gadget_ListBox.h //                             \author Logan Jones
/////////////////////                                \date 10/18/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _gadget_ListBox_H_
#define _gadget_ListBox_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// gadget_ListBox //////////////////////////
//
class gadget_ListBox : public gui_Gadget
{
	struct ListBoxItem_t
	{
		char			Name[32];
		LPVOID			Data;
	};

	typedef vector< ListBoxItem_t >	ItemList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL AddListBoxItem( LPCTSTR strName, LPVOID Data=NULL );
	void ClearList();

	int SetSelection( LPCTSTR strSearch );
	LPCTSTR GetCurrentSelection( LPVOID* ppData=NULL );

	void SetSort( BOOL bSort=TRUE );

	void AttachSlider( class gadget_Slider* pSlider );

	INLINE bool IsEmpty() const;


/////////////////////////////////////////////////
// Data members
protected:

	ItemList_t				m_ItemList;
	DWORD					m_ItemCount;

	int						m_MaxVisibleItems;

	int						m_FirstVisibleItemIndex;
	int						m_CurrentSelectionIndex;

	bool					m_bSortList;
	bool					m_WrapSelection;

	DWORD					m_KeyDown;
	std_Time_t				m_DownTime;

	class gadget_Slider*	m_pScroller;


/////////////////////////////////////////////////
// Event Handlers
public:

	BOOL OnCreate();
	void OnDestroy();

	void OnUpdate();
	void OnRender();

	DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, Param_t dwParamA, Param_t dwParamB );

	void OnMouseButton( const std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );
	void OnMouseWheel( const std_Point_t& ptCursor, long lData, DWORD dwFlags );
	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Utility methods
protected:

	void InsertItem( ListBoxItem_t& NewItem );

	void ResetView();
	void MoveView( int iDist );

	void ShiftSelection( int iDist );

	void ResetSliderSize();
	void ResetSliderPos();

	void HandleKey( DWORD dwKey );


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_ListBox();
	virtual ~gadget_ListBox();
/////////////////////////////////////////////////

}; // End class - gadget_ListBox
/////////////////////////////////////////////////////////////////////


// Include INLINE implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "gadget_ListBox.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_gadget_ListBox_H_)
