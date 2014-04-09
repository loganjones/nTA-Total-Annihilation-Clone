// gadget_ListBox.cpp //                           \author Logan Jones
//////////////////////////                           \date 10/18/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gadget_ListBox.h"
#include "gadget_Slider.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "gadget_ListBox.inl"
#endif // defined( _DEBUG )

#define INDENT					2
#define LINE_SEPARATION			2

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gadget_ListBox::gadget_ListBox():gui_Gadget(),
	m_bSortList( TRUE ),
	m_pScroller( NULL )
{
	ClearList();
}
gadget_ListBox::~gadget_ListBox()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnCreate() //                   \author Logan Jones
//////////////////////////////////                    \date 10/30/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gadget_ListBox::OnCreate()
{
	m_MaxVisibleItems = m_Size.height / (guiResources.Fonts.Standard->GetHeight() + LINE_SEPARATION);
	m_KeyDown = 0;

	return TRUE;
}
// End gadget_ListBox::OnCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnDestroy() //                  \author Logan Jones
///////////////////////////////////                   \date 10/30/2001
//               
//====================================================================
//
void gadget_ListBox::OnDestroy()
{
	ClearList();
}
// End gadget_ListBox::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnUpdate() //                   \author Logan Jones
///////////////////////////////                         \date 8/5/2002
//               
//====================================================================
//
void gadget_ListBox::OnUpdate()
{
	if( m_KeyDown && (std_Time() - m_DownTime)>=500 )
		HandleKey( m_KeyDown );
}
// End gadget_ListBox::OnUpdate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnRender() //                   \author Logan Jones
//////////////////////////////////                    \date 10/18/2001
//               
//====================================================================
//
void gadget_ListBox::OnRender()
{
//	gfx->DrawRect( std_RectF(m_ScreenPosition,m_Size), (DWORD)0x333333FF );

	const long		RowHeight = guiResources.Fonts.Standard->GetHeight() + LINE_SEPARATION;
	std_Point		Pos = m_ScreenPosition + std_Point( INDENT, RowHeight );
	const std_RectF	SelectionRect( std_Vector2(-INDENT,-RowHeight+LINE_SEPARATION), std_SizeF(m_Size.width,RowHeight) );

	// Iterate through the visible items
	if( m_ItemCount ) for( int i=m_FirstVisibleItemIndex,c=0; i<m_ItemCount && c<m_MaxVisibleItems; ++i,++c,Pos.y+=RowHeight)
	{
		// Is this item selected
		if( i==m_CurrentSelectionIndex )
			gfx->DrawRect( SelectionRect + Pos, (DWORD)0x505050FF );

		// Draw the item
		gfx->RenderString( m_ItemList[i].Name, Pos, 0xFFFFFFFF );
	}

	// Call the default
	gui_Gadget::OnRender();
}
// End gadget_ListBox::OnRender()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnWndMessage() //               \author Logan Jones
///////////////////////////////////                     \date 8/4/2002
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
DWORD gadget_ListBox::OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB )
{
	if( pSender==m_pScroller && dwMessage==gui_msg_SliderMoved )
	{
		assert( dwParamB>=0 && dwParamB<=(m_ItemCount - m_MaxVisibleItems) );
		m_FirstVisibleItemIndex = dwParamB;
		return 1;
	}

	return gui_Gadget::OnWndMessage( pSender, dwMessage, dwParamA, dwParamB );
}
// End gadget_ListBox::OnWndMessage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnMouseButton() //              \author Logan Jones
///////////////////////////////////////               \date 11/25/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  DWORD dwButton        - 
//  BOOL bDown            - 
//  DWORD dwFlags         - 
//
void gadget_ListBox::OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	const int	ItemHeight = guiResources.Fonts.Standard->GetHeight() + LINE_SEPARATION;
	const int	ItemsDown = ptCursor.y / ItemHeight;
	const int	NewItem = m_FirstVisibleItemIndex + ItemsDown;

	if( NewItem>=0 && NewItem<m_ItemCount )
	{
		int Last = m_CurrentSelectionIndex;
		m_CurrentSelectionIndex = NewItem;
		if( Last!=NewItem )
			SendMessage( m_pParent, gui_msg_ListBoxSelectionChanged, (DWORD)(LPTSTR(m_CommonData.Name)), m_CurrentSelectionIndex );
	}
}
// End gadget_ListBox::OnMouseButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnMouseWheel() //               \author Logan Jones
///////////////////////////////////                     \date 8/1/2002
//               
//====================================================================
// Parameters:
//  std_Point_t& ptCursor - 
//  long lData            - 
//  DWORD dwFlags         - 
//
void gadget_ListBox::OnMouseWheel( std_Point_t& ptCursor, long lData, DWORD dwFlags )
{
	if( lData>0 ) ShiftSelection( -4 );
	else ShiftSelection( 4 );
}
// End gadget_ListBox::OnMouseWheel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::OnKeyboardButton() //           \author Logan Jones
///////////////////////////////////////                 \date 8/1/2002
//               
//====================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void gadget_ListBox::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
#define HANDLE_KEY( theKEY ) \
	case theKEY: if( bDown ) { HandleKey( theKEY ); m_KeyDown = theKEY; m_DownTime = std_Time(); } \
				 else m_KeyDown = 0; \
				 break;

	switch( dwButton ) {
		HANDLE_KEY( IN_KEY_UP )
		HANDLE_KEY( IN_KEY_DOWN )
		HANDLE_KEY( IN_KEY_PGUP )
		HANDLE_KEY( IN_KEY_PGDOWN )
		HANDLE_KEY( IN_KEY_HOME )
		HANDLE_KEY( IN_KEY_END )
		default: gui_Gadget::OnKeyboardButton( dwButton, bDown, dwFlags );
	}
}
// End gadget_ListBox::OnKeyboardButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::AddListBoxItem() //             \author Logan Jones
////////////////////////////////////////              \date 11/25/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//  LPVOID Data     - 
//
// Return: BOOL - 
//
BOOL gadget_ListBox::AddListBoxItem( LPCTSTR strName, LPVOID Data )
{
	ListBoxItem_t			Item;

	// Sanity check
	if( strName==NULL || strlen(strName)==0 || strlen(strName)>=32  )
		return FALSE;

	// Set the item's data
	strcpy( Item.Name, strName );
	Item.Data = Data;

	// Add it to the list
	InsertItem( Item );

	return TRUE;
}
// End gadget_ListBox::AddListBoxItem()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::ClearList() //                  \author Logan Jones
///////////////////////////////////                   \date 11/25/2001
//               
//====================================================================
//
void gadget_ListBox::ClearList()
{
	m_ItemList.clear();
	m_ItemCount = 0;
}
// End gadget_ListBox::ClearList()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::SetSelection() //               \author Logan Jones
///////////////////////////////////                     \date 8/1/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strSearch - 
//
// Return: int - 
//
int gadget_ListBox::SetSelection( LPCTSTR strSearch )
{
	int						i;
	ItemList_t::iterator	it, end = m_ItemList.end();

	// Iterate until we find it (or not)
	for( it=m_ItemList.begin(),i=0; it!=end && (strcmp(strSearch,(*it).Name)!=0); ++it,++i);

	// Check if it wasn't found
	if( it==end ) return -1;

	// Set the current selection
	m_CurrentSelectionIndex = i;

	// Reset the view and return the index
	ResetView();
	return i;
}
// End gadget_ListBox::SetSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::GetCurrentSelection() //        \author Logan Jones
//////////////////////////////////////////              \date 8/8/2002
//               
//====================================================================
// Parameters:
//  LPVOID* ppData - 
//
// Return: LPCTSTR - 
//
LPCTSTR gadget_ListBox::GetCurrentSelection( LPVOID* ppData )
{
	ListBoxItem_t*	pItem;

	if( m_ItemCount )
	{
		pItem = &m_ItemList[ m_CurrentSelectionIndex ];
		if( ppData ) *ppData = pItem->Data;
		return pItem->Name;
	}
	else return NULL;
}
// End gadget_ListBox::GetCurrentSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::SetSort() //                    \author Logan Jones
/////////////////////////////////                     \date 11/25/2001
//               
//====================================================================
// Parameters:
//  BOOL bSort - 
//
void gadget_ListBox::SetSort( BOOL bSort )
{
	if( bSort )
	{
	//	if( m_bSortList==FALSE )
	//		SortList();
		m_bSortList = TRUE;
	}
	else m_bSortList = FALSE;
}
// End gadget_ListBox::SetSort()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::AttachSlider() //               \author Logan Jones
//////////////////////////////////////                \date 11/25/2001
//               
//====================================================================
// Parameters:
//  gadget_Slider* pSlider - 
//
void gadget_ListBox::AttachSlider( gadget_Slider* pSlider )
{
	if( m_pScroller = pSlider )
	{
		m_pScroller->SetBuddyWnd( this );
		ResetSliderSize();
	}
}
// End gadget_ListBox::AttachSlider()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::ResetSliderSize() //            \author Logan Jones
//////////////////////////////////////                \date 11/25/2001
//               
//====================================================================
//
void gadget_ListBox::ResetSliderSize()
{
	if( m_pScroller )
	{
		if( m_ItemCount<=m_MaxVisibleItems ) m_pScroller->SetKnobRatio( 1 );
		else
		{
			m_pScroller->SetKnobRatio( ((float)m_MaxVisibleItems) / m_ItemCount );
			m_pScroller->SetRange( 0, m_ItemCount - m_MaxVisibleItems );
		}
		ResetSliderPos();
	}
}
// End gadget_ListBox::ResetSliderSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::ResetSliderPos() //             \author Logan Jones
/////////////////////////////////////                   \date 8/4/2002
//               
//====================================================================
//
void gadget_ListBox::ResetSliderPos()
{
	if( m_pScroller ) m_pScroller->SetPosition( m_FirstVisibleItemIndex );
}
// End gadget_ListBox::ResetSliderPos()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::InsertItem() //                 \author Logan Jones
/////////////////////////////////                       \date 8/1/2002
//               
//====================================================================
// Parameters:
//  ListBoxItem_t& NewItem - 
//
void gadget_ListBox::InsertItem( ListBoxItem_t& NewItem )
{
	int							i;
	ItemList_t::iterator		it;
	ItemList_t::iterator	end = m_ItemList.end();

	// Check if we need to sort
	if( m_ItemCount==0 || !m_bSortList ) { i = m_ItemCount; it = end; }
	else // Iterate to the proper sort position
		for( it=m_ItemList.begin(),i=0; it!=end && (strcmp(NewItem.Name,(*it).Name)>0); ++it,++i);

	// Insert the item and increment the count
	it = m_ItemList.insert( it, NewItem );
	++m_ItemCount;

	// Check if this is the first item
	if( m_ItemCount==1 )
		m_FirstVisibleItemIndex = m_CurrentSelectionIndex = 0;
	else // Shift some of the stored indeces
	{
		if( i<=m_FirstVisibleItemIndex ) ++m_FirstVisibleItemIndex;
		if( i<=m_CurrentSelectionIndex ) ++m_CurrentSelectionIndex;
	}

	// Reset the size of the slider and try to center the view
	ResetSliderSize();
	MoveView( (m_CurrentSelectionIndex - m_FirstVisibleItemIndex) - (m_MaxVisibleItems / 2) );
}
// End gadget_ListBox::InsertItem()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::ResetView() //                  \author Logan Jones
////////////////////////////////                        \date 8/1/2002
//               
//====================================================================
//
void gadget_ListBox::ResetView()
{
	const int center = m_MaxVisibleItems / 2;

	// Check if the view can actually shift
	if( m_ItemCount<=m_MaxVisibleItems )
		m_FirstVisibleItemIndex = 0;
	// Check if the selection is very low in the list
	else if( (m_CurrentSelectionIndex - center)<0 )
		m_FirstVisibleItemIndex = 0;
	// Check if the selection is very high in the list
	else if( (m_CurrentSelectionIndex + center)>=m_ItemCount )
		m_FirstVisibleItemIndex = m_ItemCount - m_MaxVisibleItems;
	else // Set the selection to the center of the view
		m_FirstVisibleItemIndex = m_CurrentSelectionIndex - center;

	ResetSliderPos();
}
// End gadget_ListBox::ResetView()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::MoveView() //                   \author Logan Jones
///////////////////////////////                         \date 8/1/2002
//               
//====================================================================
// Parameters:
//  int iDist - 
//
void gadget_ListBox::MoveView( int iDist )
{
	// Check if the view can actually shift
	if( m_ItemCount<=m_MaxVisibleItems || iDist==0 ) return;

	// Move in the indicated direction
	if( iDist<0 ) // Up
	{
		m_FirstVisibleItemIndex += iDist;
		if( m_FirstVisibleItemIndex<0 ) m_FirstVisibleItemIndex = 0;
	}
	else // Down
	{
		m_FirstVisibleItemIndex += iDist;
		if( m_FirstVisibleItemIndex>(m_ItemCount - m_MaxVisibleItems) )
			m_FirstVisibleItemIndex = m_ItemCount - m_MaxVisibleItems;
	}

	ResetSliderPos();
}
// End gadget_ListBox::MoveView()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::ShiftSelection() //             \author Logan Jones
/////////////////////////////////////                   \date 8/1/2002
//               
//====================================================================
// Parameters:
//  int iDist - 
//
void gadget_ListBox::ShiftSelection( int iDist )
{
	int NewIndex = m_CurrentSelectionIndex + iDist;

	// Don't be stupid
	if( iDist>=m_ItemCount && -iDist>=m_ItemCount ) return;

	// Underflow?
	if( NewIndex<0 )
	{
		// Can we wrap
		if( m_WrapSelection )
		{
			m_CurrentSelectionIndex = m_ItemCount + NewIndex;
			ResetView();
		}
		// Is there still space at the bottom
		else if( m_CurrentSelectionIndex>0 )
		{
			MoveView( 0 - m_CurrentSelectionIndex );
			m_CurrentSelectionIndex = 0;
		}
		// Nothing has changed
		else return;
	}
	// Overflow?
	else if( NewIndex>=m_ItemCount )
	{
		// Can we wrap
		if( m_WrapSelection )
		{
			m_CurrentSelectionIndex = NewIndex - m_ItemCount;
			ResetView();
		}
		// Is there still space at the top
		else if( m_CurrentSelectionIndex<(m_ItemCount-1) )
		{
			MoveView( m_ItemCount - 1 - m_CurrentSelectionIndex );
			m_CurrentSelectionIndex = m_ItemCount - 1;
		}
		// Nothing has changed
		else return;
	}
	else // No need to wrap or stop
	{
		m_CurrentSelectionIndex = NewIndex;
		MoveView( (m_CurrentSelectionIndex - m_FirstVisibleItemIndex) - (m_MaxVisibleItems / 2) );
	}

	// Selection changed
	SendMessage( m_pParent, gui_msg_ListBoxSelectionChanged, (DWORD)(LPTSTR(m_CommonData.Name)), m_CurrentSelectionIndex );
}
// End gadget_ListBox::ShiftSelection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gadget_ListBox::HandleKey() //                  \author Logan Jones
////////////////////////////////                        \date 8/5/2002
//               
//====================================================================
// Parameters:
//  DWORD dwKey - 
//
void gadget_ListBox::HandleKey( DWORD dwKey )
{
	switch( dwKey )
	{
		case IN_KEY_UP:		ShiftSelection(-1 ); break;
		case IN_KEY_DOWN:	ShiftSelection( 1 ); break;
		case IN_KEY_PGUP:	MoveView(-m_MaxVisibleItems+1 ); break;
		case IN_KEY_PGDOWN:	MoveView( m_MaxVisibleItems-1 ); break;
		case IN_KEY_HOME:	MoveView(-999999999 ); break;
		case IN_KEY_END:	MoveView( 999999999 ); break;
	}
}
// End gadget_ListBox::HandleKey()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gadget_ListBox.cpp //
////////////////////////////////
