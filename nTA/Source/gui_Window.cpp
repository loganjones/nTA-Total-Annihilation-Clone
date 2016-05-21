// gui_Window.cpp //                               \author Logan Jones
///////////////////                                   \date 4/20/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "gui_Window.h"
#include "util_TxtParse.h"
#include "ta_GUI.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "gui_Window.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gui_Window::gui_Window():wnd_Window(),
	m_Panel( NULL )
{}
gui_Window::~gui_Window()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gui_Window::OnCreate() //                       \author Logan Jones
///////////////////////////                          \date 04-16-2003
//
//===================================================================
// Return: BOOL - 
//
BOOL gui_Window::OnCreate()
{
	m_pWndManager->SetFocus( this );

	return wnd_Window::OnCreate();
}
// End gui_Window::OnCreate()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::OnDestroy() //                      \author Logan Jones
////////////////////////////                          \date 10/13/2001
//               
//====================================================================
//
void gui_Window::OnDestroy()
{
	// Delete all our gadgets (they should already be destoyed)
	GadgetList_t::iterator			it = m_Gadgets.begin();
	GadgetList_t::const_iterator	end= m_Gadgets.end();
	for( ; it!=end; ++it)
	{
		assert( (*it)->IsCreated()==FALSE );
		delete (*it);
	}
	m_Gadgets.clear();

	// Destroy the panel
	SAFE_DESTROY( m_Panel );

	// Call the default
	wnd_Window::OnDestroy();
}
// End gui_Window::OnDestroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::OnRender() //                       \author Logan Jones
///////////////////////////                            \date 4/23/2002
//               
//====================================================================
//
void gui_Window::OnRender()
{
	if( m_Panel )
		gfx->RenderSurfaceEx( m_ScreenPosition, m_Panel );

	wnd_Window::OnRender();
}
// End gui_Window::OnRender()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gui_Window::OnKeyboardButton() //               \author Logan Jones
///////////////////////////////////                  \date 04-16-2003
//
//===================================================================
// Parameters:
//  DWORD dwButton - 
//  BOOL bDown     - 
//  DWORD dwFlags  - 
//
void gui_Window::OnKeyboardButton( DWORD dwButton, BOOL bDown, DWORD dwFlags )
{
	// Handle some keys

	wnd_Window::OnKeyboardButton( dwButton, bDown, dwFlags );
}
// End gui_Window::OnKeyboardButton()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::LoadGui() //                        \author Logan Jones
//////////////////////////                            \date 10/14/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strGuiName - 
//
// Return: BOOL - 
//
BOOL gui_Window::LoadGui( LPCTSTR strGuiName )
{
	char				GuiPath[ MAX_PATH ];
	BYTE*				pFileBuffer = NULL;
	DWORD				FileSize;
	util_TxtParse		Parser;
	Parse_Info			Info;
	BOOL				bValid;
	ta_gui_Common_t		Common;
	ta_gui_Header_t		Header;
	gui_Gadget*			Gadget;

	// Retrieve the buffer
	sprintf( GuiPath, "guis/%s.gui", strGuiName );
	FileSize = theFileTree.DumpFileToBuffer( GuiPath, &pFileBuffer );
	if( (pFileBuffer==NULL) || (FileSize==0) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to dump \'%s\' to a buffer.", GuiPath );
		return FALSE;
	}
	
	// Begin the proccess by moving to the first header and verifying it is Gadget 0
	bValid = Parser.Begin( pFileBuffer, FileSize, &Info );
	if( !bValid || Info.Type!=PARSE_Header || stricmp(Info.Value,"GADGET0")!=0 ) { delete [] pFileBuffer; return FALSE; }
	bValid = Parser.Continue( &Info );

	// Move to the block and then to the [COMMON] header
	if( !bValid || Info.Type!=PARSE_LevelUp ) { delete [] pFileBuffer; return FALSE; }
	bValid = Parser.Continue( &Info );
	if( Info.Type!=PARSE_Header || Parser.Locale()!=1 || stricmp(Info.Value,"Common")!=0 ) { delete [] pFileBuffer; return FALSE; }

	// Load the common structure
	bValid = ta_gui_LoadCommon( Parser, Common );
	if( !bValid ) { delete [] pFileBuffer; return FALSE; }

	// Load the rest of the block
	while( bValid && Parser.Locale()>0 )
	{
		if( Info.Type==PARSE_VarVal && Parser.Locale()==1 )
			STRING_SWITCH( Info.Variable )
				CASE( "totalgadgets" )	Header.NumGadgets = atoi( Info.Value );
				CASE( "Panel" )			strcpy( Header.Panel, Info.Value );
				CASE( "defaultfocus" )	strcpy( Header.Focus, Info.Value );
				CASE( "crDefault" )		strcpy( Header.crDefault, Info.Value );
				CASE( "escDefault" )	strcpy( Header.escDefault, Info.Value );
			END_STRING_SWITCH

		bValid = Parser.Continue( &Info );
	} // end while( bValid )

	// Apply the header stuff
	ApplyGuiHeader( Common, Header );

	// Load the rest of the gadgets now
	while( bValid )
	{
		if( Info.Type==PARSE_Header && Parser.Locale()==0 && strnicmp(Info.Value,"GADGET",6)==0 )
		{
			// Procceed to the block and the common section
			bValid = Parser.Continue( &Info );
			if( !bValid || Info.Type!=PARSE_LevelUp ) continue;
			bValid = Parser.Continue( &Info );
			if( Info.Type!=PARSE_Header || stricmp(Info.Value,"Common")!=0 ) continue;

			// Load the common
			bValid = ta_gui_LoadCommon( Parser, Common );

			// What type of gadget is this
			switch( Common.ID )
			{
				case GADGET_Header:
					// There shouldn't be another header, skip this gadget
					while( bValid && Parser.Locale()>0 ) bValid = Parser.Continue( &Info );
					break;

				case GADGET_Button:
				{
					ta_gui_Button	ButtonInfo;
					ZeroMemory( &ButtonInfo, sizeof(ta_gui_Button) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "text" )			strcpy( ButtonInfo.Text, Info.Value );
								CASE( "status" )		ButtonInfo.Status = atoi( Info.Value );
								CASE( "quickkey" )		ButtonInfo.QuickKey = atoi( Info.Value );
								CASE( "grayedout" )		ButtonInfo.Grayed = atoi( Info.Value );
								CASE( "stages" )		ButtonInfo.Stages = atoi( Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the button
					CreateButton( Common, ButtonInfo );
					break;
				} // end case GADGET_Button

				case GADGET_ListBox:
				{
					ta_gui_ListBox	ListBoxInfo;
					ZeroMemory( &ListBoxInfo, sizeof(ta_gui_ListBox) );
					while( bValid && Parser.Locale()>0 )
					{
						//if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
						//	STRING_SWITCH( Info.Variable )
						//	END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the list box
					CreateListBox( Common, ListBoxInfo );
					break;
				} // end case GADGET_ListBox

				case GADGET_TextEdit: {
					ta_gui_TextEdit	EditInfo;
					ZeroMemory( &EditInfo, sizeof(EditInfo) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "maxchars" )		EditInfo.MaxChars = atoi( Info.Value );
								CASE( "text" )			strcpy( EditInfo.Text, Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the slider
					CreateTextEdit( Common, EditInfo );
					break;
				} // end case GADGET_TextEdit

				case GADGET_Slider:
				{
					ta_gui_Slider	SliderInfo;
					ZeroMemory( &SliderInfo, sizeof(ta_gui_Slider) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "range" )			SliderInfo.Range = atoi( Info.Value );
								CASE( "thick" )			SliderInfo.Thickness = atoi( Info.Value );
								CASE( "knobpos" )		SliderInfo.KnobPos = atoi( Info.Value );
								CASE( "knobsize" )		SliderInfo.KnobSize = atoi( Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the slider
					CreateSlider( Common, SliderInfo );
					break;
				} // end case GADGET_Slider

				case GADGET_String:
				{
					ta_gui_String	StringInfo;
					ZeroMemory( &StringInfo, sizeof(ta_gui_String) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "text" )			strcpy( StringInfo.Text, Info.Value );
								CASE( "link" )			strcpy( StringInfo.Link, Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the string
					CreateString( Common, StringInfo );
					break;
				} // end case GADGET_String

				case GADGET_Pic:
				{
					ta_gui_Pic	PicInfo;
					ZeroMemory( &PicInfo, sizeof(ta_gui_Pic) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "hotornot" )		PicInfo.HotOrNot = atoi( Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the pic
					CreatePic( Common, PicInfo );
					break;
				} // end case GADGET_Pic

				case GADGET_Font:
				{
					ta_gui_Font	FontInfo;
					ZeroMemory( &FontInfo, sizeof(ta_gui_Font) );
					while( bValid && Parser.Locale()>0 )
					{
						if( Info.Type==Info.Type==PARSE_VarVal && Parser.Locale()==1 )
							STRING_SWITCH( Info.Variable )
								CASE( "filename" )		strcpy( FontInfo.FileName, Info.Value );
							END_STRING_SWITCH
						bValid = Parser.Continue( &Info );
					} // end load gadget

					// Create the font
					CreateFont( Common, FontInfo );
					break;
				} // end case GADGET_Font

				default:
					// The id is not one I know of, skip it
					theApp.Console.Comment( CT_EXTRA_LOAD, "Unknown gadget type, %d, with name, \'%s\', encountered while loading \'%s\'", Common.ID, Common.Name, GuiPath );
					while( bValid && Parser.Locale()>0 ) bValid = Parser.Continue( &Info );
					break;

			} // end switch( ID )
		}
		else bValid = Parser.Continue( &Info );

	} // end while( bValid )

	// Delete the buffer
	delete [] pFileBuffer;

	// Set the default focus
	if( (Gadget=GetGadget(Header.Focus)) )
		m_pWndManager->SetFocus(Gadget);

	// Return success
	return TRUE;
}
// End gui_Window::LoadGui()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::LoadPanel() //                      \author Logan Jones
////////////////////////////                           \date 4/23/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strPanel - 
//
// Return: BOOL - 
//
BOOL gui_Window::LoadPanel( LPCTSTR strPanel )
{
	m_Panel = NULL;
	return FALSE;
}
// End gui_Window::LoadPanel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::ApplyGuiHeader() //                 \author Logan Jones
/////////////////////////////////                      \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Header_t& Header - 
//
void gui_Window::ApplyGuiHeader( ta_gui_Common_t& Common, ta_gui_Header_t& Header )
{
	// Reset the dimensions
	SetWndSize( std_Size_t(Common.Width,Common.Height) );
	SetWndPos( std_Point_t(Common.XPos,Common.YPos) );

	// Load the panel (if one was given)
	if( Header.Panel[0]!='\0' )
		LoadPanel( Header.Panel );
}
// End gui_Window::ApplyGuiHeader()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreateButton() //                   \author Logan Jones
///////////////////////////////                        \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Button& Info     - 
//
void gui_Window::CreateButton( ta_gui_Common_t& Common, ta_gui_Button& Info )
{
	gadget_Button* pButton = new gadget_Button;
	if( pButton && pButton->Create( &Common, &Info, this ) )
		m_Gadgets.push_front( pButton );
}
// End gui_Window::CreateButton()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreateListBox() //                  \author Logan Jones
////////////////////////////////                       \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_ListBox& Info    - 
//
void gui_Window::CreateListBox( ta_gui_Common_t& Common, ta_gui_ListBox& Info )
{
	gadget_ListBox* pListBox = new gadget_ListBox;
	if( pListBox && pListBox->Create( &Common/*, &Info*/, this ) )
		m_Gadgets.push_front( pListBox );
}
// End gui_Window::CreateListBox()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gui_Window::CreateTextEdit() //                 \author Logan Jones
/////////////////////////////////                    \date 04-10-2003
//
//===================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_TextEdit& Info   - 
//
void gui_Window::CreateTextEdit( ta_gui_Common_t& Common, ta_gui_TextEdit& Info )
{
	gadget_TextEdit* pEdit = new gadget_TextEdit;
	if( pEdit && pEdit->Create( &Common, &Info, this ) )
		m_Gadgets.push_front( pEdit );
}
// End gui_Window::CreateTextEdit()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreateSlider() //                   \author Logan Jones
///////////////////////////////                        \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Slider& Info     - 
//
void gui_Window::CreateSlider( ta_gui_Common_t& Common, ta_gui_Slider& Info )
{
	gadget_Slider* pSlider = new gadget_Slider;
	if( pSlider && pSlider->Create( &Common, &Info, this ) )
		m_Gadgets.push_front( pSlider );
}
// End gui_Window::CreateSlider()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreateString() //                   \author Logan Jones
///////////////////////////////                        \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_String& Info     - 
//
void gui_Window::CreateString( ta_gui_Common_t& Common, ta_gui_String& Info )
{
	gadget_String* pString = new gadget_String;
	if( pString && pString->Create( &Common, &Info, this ) )
		m_Gadgets.push_front( pString );
}
// End gui_Window::CreateString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreatePic() //                      \author Logan Jones
////////////////////////////                           \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Pic& Info        - 
//
void gui_Window::CreatePic( ta_gui_Common_t& Common, ta_gui_Pic& Info )
{
	// Not yet implemented
	gadget_Pic* pPic = new gadget_Pic;
	if( pPic && pPic->Create( &Common, &Info, this ) )
		m_Gadgets.push_front( pPic );
}
// End gui_Window::CreatePic()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gui_Window::CreateFont() //                     \author Logan Jones
/////////////////////////////                          \date 4/23/2002
//               
//====================================================================
// Parameters:
//  ta_gui_Common_t& Common - 
//  ta_gui_Font& Info       - 
//
void gui_Window::CreateFont( ta_gui_Common_t& Common, ta_gui_Font& Info )
{
	// Do nothing for the default
}
// End gui_Window::CreateFont()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gui_Window.cpp //
/////////////////////////
