// igui_InfoBar.h //                               \author Logan Jones
///////////////////                                    \date 4/1/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_INFOBAR_H_
#define _IGUI_INFOBAR_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// igui_InfoBar ////////////////////////////
//
class igui_InfoBar : public wnd_Window
{

/////////////////////////////////////////////////
// INTERFACE
public:

	void Initialize( struct SideData_t* pSideData );

	void Render();

	void DisplayObjectInfo( class scene_Object* pObject );
	void DisplayBuildInfo( const struct unit_Type* pType );


/////////////////////////////////////////////////
// Event Handlers

	void OnCursorMove( std_Point& ptCursor, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

	enum DiplayingWhat_t
	{
		DISP_Nothing,
		DISP_BuildInfo,
		DISP_UnitInfo,
		DISP_FeatureInfo,
	}				m_DisplayingWhat;

	union {
		const class feature_Object*		Feature;
		const class unit_Object*		Unit;
		const struct unit_Type*			UnitType;
	}				m_What;

	char			m_Line1[ 256 ];
	char			m_Line2[ 256 ];


	std_Point			m_Name;
	std_Point			m_Description;
	std_Point			m_UnitName;
	std_Point			m_UnitName2;
	std_Point			m_Status;

	std_Point			m_Metal_Produced;
	std_Point			m_Metal_Consumed;
	std_Point			m_Energy_Produced;
	std_Point			m_Energy_Consumed;

	std_Rect			m_DamageBar;
	std_Rect			m_DamageBar2;

	gfx_Surface*		m_Logo;
	std_RectF			m_LogoRect;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_InfoBar();
	virtual ~igui_InfoBar();
/////////////////////////////////////////////////

}; // End class - igui_InfoBar
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_INFOBAR_H_)
