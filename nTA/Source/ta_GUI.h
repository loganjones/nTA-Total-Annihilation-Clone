// ta_GUI.h //                                     \author Logan Jones
/////////////                                        \date 10/14/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_GUI_H_
#define _TA_GUI_H_
/////////////////////////////////////////////////////////////////////


// Gadget types
enum ta_gui_GadgetTypes_t
{
	GADGET_Header		= 0,
	GADGET_Button		= 1,
	GADGET_ListBox		= 2,
	GADGET_TextEdit		= 3,
	GADGET_Slider		= 4,
	GADGET_String		= 5,
	GADGET_Pic			= 6,
	GADGET_Font			= 7,

	FORCE_DWORD( gadget )
};


// Common data
// Every entry in a gui file has a common section
struct ta_gui_Common_t
{
	ta_gui_GadgetTypes_t	ID;
	UINT32					Assoc;
	char					Name[32];
	INT32					XPos;
	INT32					YPos;
	UINT32					Width;
	UINT32					Height;
	UINT32					Attribs;
	UINT32					Active;
	UINT32					CommonAttribs;
};

// Gadget type 0
// This data is always obtained from gadget 0
struct ta_gui_Header_t
{
	UINT32		NumGadgets;
	char		Panel[32];
	char		Focus[32];
	char		crDefault[32];
	char		escDefault[32];
};


// Gadget type 1
// Data given for a button entry in a gui file
struct ta_gui_Button
{
	UINT32		Status;
	char		Text[ 128 ];
	UINT32		QuickKey;
	UINT32		Grayed;
	UINT32		Stages;
};


// Gadget type 2
// Data given for a list box entry in a gui file
struct ta_gui_ListBox
{
};


// Gadget type 3
// Data given for a text edit entry in a gui file
struct ta_gui_TextEdit
{
	UINT32		MaxChars;
	char		Text[ 128 ];
};


// Gadget type 4
// Data given for a slider entry in a gui file
struct ta_gui_Slider
{
	UINT32		Range;
	UINT32		Thickness;
	UINT32		KnobPos;
	UINT32		KnobSize;
};


// Gadget type 5
// Data given for a string entry in a gui file
struct ta_gui_String
{
	char		Text[ 128 ];
	char		Link[ 128 ];
};


// Gadget type 6
// Data given for a pic entry in a gui file
struct ta_gui_Pic
{
	UINT32		HotOrNot;
};


// Gadget type 7
// Data given for a font entry in a gui file
struct ta_gui_Font
{
	char		FileName[ 32 ];
};


// Load the common structure from the given parser
BOOL ta_gui_LoadCommon( class util_TxtParse& Parser, ta_gui_Common_t& Common );


/////////////////////////////////////////////////////////////////////
#endif // !defined(_TA_GUI_H_)
