// igui_ResourceBar.h //                           \author Logan Jones
///////////////////////                                \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_RESOURCEBAR_H_
#define _IGUI_RESOURCEBAR_H_
/////////////////////////////////////////////////////////////////////


////////////////////////// igui_ResourceBar //////////////////////////
//
class igui_ResourceBar : public wnd_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Initialize( struct SideData_t* pSideData );

	void Render();


/////////////////////////////////////////////////
// Event Handlers

	void OnCursorMove( std_Point& ptCursor, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members

	BYTE				m_MetalColor;
	BYTE				m_EnergyColor;

	gfx_Surface*		m_Logo;
	std_RectF			m_LogoRect;

	std_Rect			m_MetalBar;
	std_Rect			m_EnergyBar;

	std_Point			m_Metal_0;
	std_Point			m_Metal_Max;
	std_Point			m_Metal_Num;
	std_Point			m_Metal_Produced;
	std_Point			m_Metal_Consumed;
	char				m_Metal_Produced_String[32];
	char				m_Metal_Consumed_String[32];

	std_Point			m_Energy_0;
	std_Point			m_Energy_Max;
	std_Point			m_Energy_Num;
	std_Point			m_Energy_Produced;
	std_Point			m_Energy_Consumed;
	char				m_Energy_Produced_String[32];
	char				m_Energy_Consumed_String[32];

	float				m_Test;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_ResourceBar();
	virtual ~igui_ResourceBar();
/////////////////////////////////////////////////

}; // End class - igui_ResourceBar
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_RESOURCEBAR_H_)
