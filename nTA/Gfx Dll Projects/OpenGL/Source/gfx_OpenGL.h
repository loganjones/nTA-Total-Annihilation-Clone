// gfx_OpenGL.h //                         Author: Logan "Burn" Jones
/////////////////                                     Date: 9/21/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_OPENGL_H_
#define _GFX_OPENGL_H_
/////////////////////////////////////////////////////////////////////

#include <GL\gl.h>
#include <GL\glext.h>
#ifdef _WIN32
 #include <GL\wglext.h>
#endif

#include "ogl_Surface.h"
#include "ogl_Font.h"
#include "ogl_Cursor.h"
#include "ogl_Terrain.h"
#include "ogl_DynamicImage.h"

///////////////////////////// gfx_OpenGL /////////////////////////////
//
class gfx_OpenGL : public gfx_Interface
{

	friend class ogl_Surface;
	friend class ogl_Model;
	friend class ogl_DynamicImage;

/////////////////////////////////////////////////
// Types
protected:

	struct Caps_t
	{
		std_Size_t		MaxTextureSize;
	};

	struct Extensions_t
	{
        BOOL        Texture_Edge_Clamp;
        BOOL        Paletted_Texture;
        BOOL        Shared_Texture_Palette;
        BOOL        Vertex_Array;
	};


public:

	struct Palette_t
	{
		PALETTEENTRY		Entries[256];
	};
	typedef list< Palette_t >			PaletteList_t;
	typedef list< ogl_Surface >			SurfaceList_t;
	typedef list< ogl_Font >			FontList_t;
	typedef list< ogl_Cursor >			CursorList_t;
	typedef list< ogl_DynamicImage >	DynamicImageList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Initialize( std_PlatformParameters* pPlatformParams, LPCommentProc_t lpLoadProc, LPCommentProc_t lpErrorProc );

	BOOL Create( gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen );
	BOOL Destroy();


/////////////////////////////////////////////////
// General info methods

	// Mirror of the global function by the same name
	DWORD EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth );

	// Returns the current scree resolution of the gfx system
	const gfx_Resolution_t* GetScreenResolution() const;

	void TEMP_DoBlueScreen();

	void BeginScene();
	void EndScene();

	void Set2DProjection( gfx_Resolution_t& resScreen, float fZNear, float fZFar );

	void DrawRect( std_Rect_t& rctWhere, const PALETTEENTRY& peColor );
	void DrawRect( std_RectF rctWhere, const DWORD dwColor );
	void DrawRect( std_RectF rctWhere, const BYTE byIndex );
	void DrawRect( std_Vector3 vTrans, const float fRot, std_RectF rctWhere, const DWORD dwColor );

	void DrawWireRect( std_RectF rctWhere, const DWORD dwColor );

	void DrawLine( std_Vector2 vFrom, std_Vector2 vTo, const DWORD dwColor );

	void NanolatheEffect( std_Vector2 vFrom, std_Vector2 vTo );

	void SaveScreenShot( LPCTSTR strScreenShotPath );


/////////////////////////////////////////////////
// Game scene methods

	// Starts and ends the rendering of the game scene
	// Use these instead of the vanilla Begin/EndScene()
	// These take care of proper shadowing, terrain rendering and so forth;
	void BeginGameScene();
	void EndGameScene();

	// Call before rendering the game objects
	void BeginObjectRendering();

	// Stops the object rendering phase and prepares to render the interface
	void BeginInterfaceRendering();


/////////////////////////////////////////////////////////////////////
// High Level Interface


/////////////////////////////////////////////////
// Palettes
public:

	// Creates an image palette from the given entries
	BOOL CreatePalette( PALETTEENTRY* pPaletteEntries, LPPALETTE* ppPalette, BOOL bSetAsCurrent=TRUE );

	// Sets the given palette as the current image palette
	BOOL SetCurrentPalette( LPPALETTE pPalette );

	// Points the given pointer to the current image palette
	BOOL GetCurrentPalette( LPPALETTE* ppPalette );

	// Destroys a given palette
	BOOL DestroyPalette( LPPALETTE* pPalette );


/////////////////////////////////////////////////
// Surface methods
public:

	// Creates an image surface that can be rendered to the screen
	BOOL CreateSurface( GFX_PIXEL_FORMAT pxFormat, gfx_Image_t* pImage, gfx_Surface** ppSurface );

	void RenderSurface( gfx_Surface* pSurface );
	void RenderSurfaceEx( std_Point_t& ptWhere, gfx_Surface* pSurface );


/////////////////////////////////////////////////
// Fonts

	// Creates a gfx font from a given image buffer
	BOOL CreateFont( FontEntry* pFontEntries, std_Size_t szFontDist, GFX_PIXEL_FORMAT pxSrcFormat, gfx_Image_t* pImage, gfx_Font** ppFont, BOOL bSetAsCurrent=TRUE );

	// Sets a given font as the current
	BOOL SetCurrentFont( gfx_Font* pFont );

	// Renders a string using the desired font
	void RenderString( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL );
	DWORD RenderString( LPCTSTR strToRender, DWORD dwLength, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL );
	void RenderStringRight( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL );

	// Renders a string using the active font at the desired location
	void RenderStringAt( std_Point_t& ptWhere, LPCTSTR strToRender );

	void RenderStringCenteredAt( std_Point_t& ptWhere, LPCTSTR strToRender, BOOL bCenterHorizontal, BOOL bCenterVertical );


/////////////////////////////////////////////////
// Cursors

	// Creates a gfx cursor from a given cursor frame entry array
	BOOL CreateCursor( LPCTSTR strName, DWORD dwNumFrames, CursorFrame* pFrames, gfx_Cursor** ppCursor, BOOL bSetAsCurrent );

	// Sets a given cursor as the current
	BOOL SetCurrentCursor( gfx_Cursor* pCursor );

	// Sets a cursor with the given name as the current
	BOOL SetCurrentCursor( LPCTSTR strName );

	// Updates the animation frame of the current cursor
	void UpdateCursor();

	// Renders the current cursor
	void RenderCursor( std_Vector2 vWhere );


/////////////////////////////////////////////////
// Misc

	BOOL LoadTerrain( BYTE* pTntBuffer, gfx_Terrain** ppTerrain );
	BOOL CreateSprite( gfx_Image_t* pImage, std_Vector2 vOffset, gfx_Sprite** ppSprite );
	BOOL CreateModel( BYTE* pFileBuffer, gfx_Model** ppModel );
	BOOL CreateDynamicImage( std_Size szImage, gfx_DynamicImage** ppImage );


/////////////////////////////////////////////////
// Low level

	void PushMatrix();
	void PopMatrix();
	void LoadIdentity();
	void Translate( const std_Vector3 vTranslate );
	void Rotate( const std_Vector3 vRotate );
	void RotateX( const float fRot );
	void RotateY( const float fRot );
	void RotateZ( const float fRot );


/////////////////////////////////////////////////
// Utility methods
protected:

	// Assigns all the extension function pointers
	// Platform dependent
	BOOL LoadExtensions();

	// Checks the currently attachecd OpenGL driver for a specified extension string
	BOOL CheckForExtensionString( LPCTSTR strExtension );

	// Called at the end of Create() if it is successful. Use for
	// post creation loading ie. extensions, etc
	BOOL PostCreate();


/////////////////////////////////////////////////
// Data members
protected:

	std_PlatformParameters*		m_pPlatformParams;
	LPCommentProc_t				LoadComment;
	LPCommentProc_t				ErrorComment;

	Caps_t						m_Caps;
	Extensions_t				m_Extensions;

	gfx_Resolution_t			m_ScreenResolution;

	LPVOID						m_TEMP;

	PaletteList_t				m_Palettes;
	LPPALETTE					m_pCurrentPalette;
	
	SurfaceList_t				m_Surfaces;

	FontList_t					m_Fonts;
	gfx_Font*					m_ActiveFont;

	CursorList_t				m_Cursors;
	gfx_Cursor*					m_ActiveCursor;

	ogl_Terrain					m_theTerrain;

	DynamicImageList_t			m_DynamicImages;


/////////////////////////////////////////////////////////////////////
// OpenGL extension function pointers
public:

    PFNGLCOLORTABLEEXTPROC              glColorTableEXT;


/////////////////////////////////////////////////
// Windows specific data members and methods
#ifdef _WIN32
protected:

    // Handle for the device context
	HDC				m_hDC;

    // Handle for the rendering context
	HGLRC			m_hRC;

#endif // End Windows specific
/////////////////////////////////////////////////


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gfx_OpenGL();
	virtual ~gfx_OpenGL();
/////////////////////////////////////////////////

}; // End class - gfx_OpenGL
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_OPENGL_H_)
