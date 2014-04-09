// gui_Resources.h //                              \author Logan Jones
////////////////////                                  \date 4/21/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GUI_RESOURCES_H_
#define _GUI_RESOURCES_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// gui_Resources ////////////////////////////
//
class gui_Resources
{
public:
	struct ButtonImages_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	// Creates and Destroys all the resources for the gui system
	BOOL Create();
	void Destroy();

	// Returns a standard button image that is closest in size to the given value
	ButtonImages_t* GetClosestStandardButton( std_Size sz );


/////////////////////////////////////////////////
// Resources
public:

	BOOL				m_bCreated;

	BYTE*				CommonGuiBuffer;

	struct ButtonImages_t
	{
		int				StageCount;
		gfx_Surface*	Stages[ 5 ];
		gfx_Surface*	Pressed;
		gfx_Surface*	Disabled;
		void Destroy();
	};

	ButtonImages_t		StandardButton_16x16;
	ButtonImages_t		StandardButton_321x20;
	ButtonImages_t		StandardButton_120x20;
	ButtonImages_t		StandardButton_96x20;
	ButtonImages_t		StandardButton_112x20;
	ButtonImages_t		StandardButton_80x20;
	ButtonImages_t		StandardButton_96x31;
	ButtonImages_t		Checkbox;
	ButtonImages_t		StageButton_1;
	ButtonImages_t		StageButton_2;
	ButtonImages_t		StageButton_3;
	ButtonImages_t		StageButton_4;

	gfx_Surface*		m_SliderButtonUp[2];
	gfx_Surface*		m_SliderButtonDown[2];
	gfx_Surface*		m_SliderButtonLeft[2];
	gfx_Surface*		m_SliderButtonRight[2];

	gfx_Image_t			m_VerticalStrip[3];
	gfx_Image_t			m_HorizontalStrip[3];

	gfx_Image_t			m_VerticalTicker[3];
	gfx_Image_t			m_HorizontalTicker[3];
	gfx_Image_t			m_StadardTicker;

	struct Fonts_t
	{
		gfx_Font*		Standard;
		gfx_Font*		StandardSmall;
	}					Fonts;

	struct Curors_t
	{
		gfx_Cursor*		Move;
		gfx_Cursor*		Green;
		gfx_Cursor*		Select;
		gfx_Cursor*		Red;
		gfx_Cursor*		Load;
		gfx_Cursor*		Revive;
		gfx_Cursor*		Defend;
		gfx_Cursor*		Patrol;
		gfx_Cursor*		Protect;
		gfx_Cursor*		Repair;
		gfx_Cursor*		Attack;
		gfx_Cursor*		Normal;
		gfx_Cursor*		PickUp;
		gfx_Cursor*		AirStrike;
		gfx_Cursor*		Teleport;
		gfx_Cursor*		Reclamate;
		gfx_Cursor*		FindSite;
		gfx_Cursor*		Capture;
		gfx_Cursor*		Unload;
		gfx_Cursor*		HourGlass;
		gfx_Cursor*		TooFar;
		gfx_Cursor*		Path;
	}					Cursors;

	struct Sounds_t {
		LPVOID			ActivateAllStatBars;
		LPVOID			EndGameStatBar;
		LPVOID			EndGameScore;
		LPVOID			BGM;
		LPVOID			BigButton;
		LPVOID			Skirmish;
		LPVOID			Multi;
		LPVOID			SkirmishCheat;
		LPVOID			Options;
		LPVOID			Exit;
		LPVOID			SmlButton;
		LPVOID			Slider;
		LPVOID			SmallButton;
		LPVOID			SideSelect;
		LPVOID			SideSelect2;
		LPVOID			More;
		LPVOID			JoinGame;
		LPVOID			MainMenu;
		LPVOID			TreeBurn;
		LPVOID			SelectMultipleUnits;
		LPVOID			MessageArrived;
		LPVOID			CreateSquad;
		LPVOID			SelectSquad;
		LPVOID			OutOfRange;
		LPVOID			NoMetal;
		LPVOID			NoEnergy;
		LPVOID			AddBuild;
		LPVOID			NotOkToBuild;
		LPVOID			OkToBuild;
		LPVOID			OrdersButton;
		LPVOID			BuildButton;
		LPVOID			SubBuild;
		LPVOID			NextBuildMenu;
		LPVOID			SetFireOrders;
		LPVOID			SetMoveOrders;
		LPVOID			SpecialOrders;
		LPVOID			ImmediateOrders;
		LPVOID			Ally;
		LPVOID			Previous;
		LPVOID			Panel;
		LPVOID			VictoryCondition;
		// nTA specific
		LPVOID			NotYetImplemented;
	}					Sounds;


/////////////////////////////////////////////////
// Utility
protected:

	BOOL LoadFonts();
	BOOL LoadCursors();
	BOOL LoadSounds();


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gui_Resources();
	virtual ~gui_Resources();
/////////////////////////////////////////////////

}; // End class - gui_Resources
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GUI_RESOURCES_H_)
