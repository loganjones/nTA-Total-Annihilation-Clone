// fe_Options.h //                                 \author Logan Jones
/////////////////                                    \date 10/17/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FE_OPTIONS_H_
#define _FE_OPTIONS_H_
/////////////////////////////////////////////////////////////////////


///////////////////////////// fe_Options /////////////////////////////
//
class fe_Options : public fe_Wnd
{

	struct RenderMode_t;
	typedef list< RenderMode_t >		ModeList_t;
	typedef map< string, ModeList_t >	RenderList_t;

/////////////////////////////////////////////////
// INTERFACE
public:


/////////////////////////////////////////////////
// Event Handlers

	virtual BOOL OnCreate();
	virtual void OnDestroy();

	virtual void OnUpdate();

	virtual DWORD OnWndMessage( wnd_Window* pSender, DWORD dwMessage, DWORD dwParamA, DWORD dwParamB );


/////////////////////////////////////////////////
// Utility methods
protected:

	void LoadSounds();
	void LoadInterface();
	void LoadVisuals();
	void LoadMusic();

	void SetModes( LPCTSTR strRenderer, long Depth, std_Size szDesiredSelection );


/////////////////////////////////////////////////
// Data members
protected:

	fe_Wnd			m_Sounds;
	fe_Wnd			m_Interface;
	fe_Wnd			m_Visuals;
	fe_Wnd			m_Music;

	gfx_Surface*	m_pSoundPanel;
	gfx_Surface*	m_pInterfacePanel;
	gfx_Surface*	m_pVisualPanel;
	gfx_Surface*	m_pMusicPanel;

	fe_Wnd*			m_pActiveWnd;

	gfx_Surface*	m_CDPrevImages[ 3 ];
	gfx_Surface*	m_CDStopImages[ 3 ];
	gfx_Surface*	m_CDPlayImages[ 3 ];
	gfx_Surface*	m_CDNextImages[ 3 ];

	gadget_ListBox*	m_RendererBox;
	gadget_ListBox*	m_ResolutionBox;
	gadget_Button*	m_ColorDepthButton;
	RenderList_t	m_Renderers;

	int				m_RendererSettingsChanged;

private:
	struct RenderMode_t
	{
		std_Size		Resolution;
		long			Depth;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	fe_Options();
	virtual ~fe_Options();
/////////////////////////////////////////////////

}; // End class - fe_Options
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FE_OPTIONS_H_)
