// gadget_Pic.h //                                 \author Logan Jones
/////////////////                                    \date 04-30-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GADGET_PIC_H_
#define _GADGET_PIC_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// gadget_Pic /////////////////////////////
//
class gadget_Pic : public gui_Gadget
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// New create method (calls old one)
	virtual BOOL Create( ta_gui_Common_t* pCommonData, ta_gui_Pic* pPicInfo, wnd_Window* pParent );
	virtual BOOL Create( const std_Point_t& ptPos, const std_Size_t& szDim, wnd_Window* pParent );

	void SetFrames( gfx_Surface** pFrames, DWORD dwFrameCount=1 );
	void SetFrame( DWORD dwFrame ) {if(dwFrame<m_FrameCount) m_Frame=dwFrame;}
	DWORD Frame() const {return m_Frame;}

	void SetText( LPCTSTR strText ){m_Text=strText;}
	LPCTSTR GetText() const {return m_Text.c_str();}

	void LoopFrames( bool bLoop=true ) {m_Loop=bLoop;}
	void FrameSize( bool bFrameSize=true );
	void Animate( bool bAnimate=true ) {if(m_FrameCount>1) m_Animating=bAnimate;}

	BOOL OnCreate();
	void OnDestroy();
	void OnUpdate();
	void OnRender();
	void OnMouseButton( std_Point_t& ptCursor, DWORD dwButton, BOOL bDown, DWORD dwFlags );


/////////////////////////////////////////////////
// Data members
protected:

	ta_gui_Pic			m_PicInfo;

	bool				m_Animating;
	bool				m_Loop;
	bool				m_Resize;

	DWORD				m_FrameCount;
	DWORD				m_Frame;

	gfx_Surface**		m_Frames;

	string				m_Text;

	DWORD				m_ButtonStates;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gadget_Pic();
	virtual ~gadget_Pic();
/////////////////////////////////////////////////

}; // End class - gadget_Pic
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GADGET_PIC_H_)
