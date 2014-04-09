// igui_MiniMap.h //                               \author Logan Jones
///////////////////                                   \date 5/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_MINIMAP_H_
#define _IGUI_MINIMAP_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// igui_MiniMap ////////////////////////////
//
class igui_MiniMap : public wnd_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Render();
	void Update();


/////////////////////////////////////////////////
// Event Handlers

	BOOL OnCreate();
	void OnDestroy();

	void OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );
	void OnCursorMove( std_Point_t& ptCursor, DWORD dwFlags );

	void OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

	gfx_DynamicImage*		m_MiniMapImage;
	BYTE*					m_MiniMapBuffer;
	std_Size				m_MiniMapBufferSize;
	std_Rect				m_MiniMapScreenRect;

	std_Size				m_WorldSize;
	std_Rect				m_ViewRect;

	BOOL					m_bScrolling;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_MiniMap();
	virtual ~igui_MiniMap();
/////////////////////////////////////////////////

}; // End class - igui_MiniMap
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_MINIMAP_H_)
