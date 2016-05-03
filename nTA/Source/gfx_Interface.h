// gfx_Interface.h //                              \author Logan Jones
////////////////////                                  \date 9/10/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_INTERFACE_H_
#define _GFX_INTERFACE_H_
/////////////////////////////////////////////////////////////////////


// Resolution type
typedef std_Size		gfx_Resolution_t;

// Settings
enum GFX_SETTINGS
{
	TEX_POINT,
	TEX_LINEAR,
};

// Pixel format type
enum GFX_PIXEL_FORMAT
{
	PIXEL_PALETTISED,
	PIXEL_RGB,
	PIXEL_BGR,
	PIXEL_RGBA,
	PIXEL_ARGB,

	FORCE_DWORD( GFX_PIXEL_FORMAT )
};

// Image type
struct gfx_Image_t
{
	// Size of the image in pixels
	std_Size_t			Size;

	// Byte distance from pixel to pixel
	long				Stride;

	// Byte distance from line to line
	long				Pitch;

	// Actual bytes of the image
	BYTE*				pBytes;
};

// SubImage type
struct gfx_SubImage_t
{
	// Size of the image in pixels
	std_Size_t			Size;

	// Pixel offset to the upper left corner of the sub-image
	std_Point_t			Offset;

	// Pointer to the upper left corner of the sub-image
	BYTE*				pBytes;

	// Pointer to the host image
	gfx_Image_t*		pImage;
};


/////////////////////////// gfx_Interface ///////////////////////////
//
class gfx_Interface
{
public:

	typedef LRESULT (CALLBACK* LPCommentProc_t)(LPCTSTR,...);


/////////////////////////////////////////////////////////////////////
// Virtual Interface
//    All definitions are made in deriveded classes in separate DLLs.
//    This file is shared between these DLLs as well as the main project.
public:

	virtual BOOL Initialize( std_PlatformParameters* pPlatformParams, LPCommentProc_t lpLoadProc, LPCommentProc_t lpErrorProc ) = 0;

	virtual BOOL Create( gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen ) = 0;
	virtual BOOL Destroy() = 0;


/////////////////////////////////////////////////
// General methods

	// Mirror of the global function by the same name
	virtual DWORD EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth ) = 0;

	// Returns the current scree resolution of the gfx system
	virtual const gfx_Resolution_t* GetScreenResolution() const = 0;

	virtual void TEMP_DoBlueScreen() = 0;

	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;

	// Returns the amount of tri's rendered in the current scene
	INLINE DWORD GetTriangleCount() const;

	virtual void Set2DProjection( gfx_Resolution_t& resScreen, float fNear, float fFar ) = 0;

	virtual void DrawRect( std_Rect_t& rctWhere, const PALETTEENTRY& peColor ) = 0;
	virtual void DrawRect( std_RectF rctWhere, const DWORD dwColor ) = 0;
	virtual void DrawRect( std_RectF rctWhere, const BYTE byIndex ) = 0;
	virtual void DrawRect( std_Vector3 vTrans, const float fRot, std_RectF rctWhere, const DWORD dwColor ) = 0;

	virtual void DrawWireRect( std_RectF rctWhere, const DWORD dwColor ) = 0;

	virtual void DrawLine( std_Vector2 vFrom, std_Vector2 vTo, const DWORD dwColor ) = 0;

	virtual void NanolatheEffect( std_Vector2 vFrom, std_Vector2 vTo ) = 0;

	virtual void SaveScreenShot( LPCTSTR strScreenShotPath ) = 0;


/////////////////////////////////////////////////
// Game scene methods

	// Starts and ends the rendering of the game scene
	// Use these instead of the vanilla Begin/EndScene()
	// These take care of proper shadowing, terrain rendering and so forth;
	virtual void BeginGameScene() = 0;
	virtual void EndGameScene() = 0;

	// Call before rendering the game objects
	virtual void BeginObjectRendering() = 0;

	// Stops the object rendering phase and prepares to render the interface
	virtual void BeginInterfaceRendering() = 0;


/////////////////////////////////////////////////
// Palettes

	// Palette handle
	typedef LPVOID		LPPALETTE;

	// Creates an image palette from the given entries
	virtual BOOL CreatePalette( PALETTEENTRY* pPaletteEntries, LPPALETTE* ppPalette, BOOL bSetAsCurrent=TRUE ) = 0;

	// Sets the given palette as the current image palette
	virtual BOOL SetCurrentPalette( LPPALETTE pPalette ) = 0;

	// Points the given pointer to the current image palette
	virtual BOOL GetCurrentPalette( LPPALETTE* ppPalette ) = 0;

	// Destroys a given palette
	virtual BOOL DestroyPalette( LPPALETTE* pPalette ) = 0;


/////////////////////////////////////////////////
// Surface methods

	// Creates an image surface that can be rendered to the screen
	virtual BOOL CreateSurface( GFX_PIXEL_FORMAT pxFormat, gfx_Image_t* pImage, gfx_Surface** ppSurface ) = 0;

	INLINE BOOL CreateSurfaceFromPCX( BYTE* pFileBuffer, DWORD dwFileBufferSize, DWORD dwFlags, gfx_Surface** ppSurface );
	INLINE BOOL CreateSurfaceFromPCX( BYTE* pFileBuffer, DWORD dwFileBufferSize, DWORD dwFlags, std_Size_t& szDesired, gfx_Surface** ppSurface );

	BOOL CreateSurfaceFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, gfx_Surface** ppSurface );
	BOOL CreateSurfaceFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, std_Size_t& szDesired, gfx_Surface** ppSurface );
	DWORD CreateSurfacesFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iStartFrame, int iFrameCount, gfx_Surface** ppSurface );

	// Destroys a given surface
	//virtual void DestroySurface( gfx_Surface** pSurface ) = 0;

	virtual void RenderSurface( gfx_Surface* pSurface ) = 0;
	virtual void RenderSurfaceEx( std_Point_t& ptWhere, gfx_Surface* pSurface ) = 0;


/////////////////////////////////////////////////
// Font methods

	struct FontEntry
	{
		std_Size_t		Size;
		std_Point_t		Offset;
	};

	// Creates a gfx font from a given image buffer
	virtual BOOL CreateFont( FontEntry* pFontEntries, std_Size_t szFontDist, GFX_PIXEL_FORMAT pxSrcFormat, gfx_Image_t* pImage, gfx_Font** ppFont, BOOL bSetAsCurrent=TRUE ) = 0;
	BOOL CreateFontFromGAF( BYTE* pFileBuffer, LPCTSTR strFontName, gfx_Font** ppFont );
	INLINE BOOL CreateFontFromFNT( BYTE* pFileBuffer, gfx_Font** ppFont );

	// Sets a given font as the current
	virtual BOOL SetCurrentFont( gfx_Font* pFont ) = 0;

	// Renders a string using the active font
	virtual void RenderString( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL ) = 0;
	virtual DWORD RenderString( LPCTSTR strToRender, DWORD dwLength, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL ) = 0;
	
	virtual void RenderStringRight( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont=NULL ) = 0;

	virtual void RenderStringAt( std_Point_t& ptWhere, LPCTSTR strToRender ) = 0;
	virtual void RenderStringCenteredAt( std_Point_t& ptWhere, LPCTSTR strToRender, BOOL bCenterHorizontal=TRUE, BOOL bCenterVertical=TRUE ) = 0;


/////////////////////////////////////////////////
// Cursor methods

	struct CursorFrame
	{
		gfx_Image_t		Image;
		std_Point_t		HotSpot;
	};

	// Creates a gfx cursor from a given cursor frame entry array
	virtual BOOL CreateCursor( LPCTSTR strName, DWORD dwNumFrames, CursorFrame* pFrames, gfx_Cursor** ppCursor, BOOL bSetAsCurrent=TRUE ) = 0;
	BOOL CreateCursorFromGAF( BYTE* pFileBuffer, LPCTSTR strCursorName, gfx_Cursor** ppCursor );
	DWORD CreateCursorsFromGAF( BYTE* pFileBuffer, gfx_Cursor*** pppCursor );

	// Sets a given cursor as the current
	virtual BOOL SetCurrentCursor( gfx_Cursor* pCursor ) = 0;

	// Sets a cursor with the given name as the current
	virtual BOOL SetCurrentCursor( LPCTSTR strName ) = 0;

	// Updates the animation frame of the current cursor
	virtual void UpdateCursor() = 0;

	// Renders the current cursor
	virtual void RenderCursor( std_Vector2 vWhere ) = 0;


/////////////////////////////////////////////////
// Terrain methods

	virtual BOOL LoadTerrain( BYTE* pTntBuffer, gfx_Terrain** ppTerrain ) = 0;


/////////////////////////////////////////////////
// Sprite methods

	virtual BOOL CreateSprite( gfx_Image_t* pImage, std_Vector2 vOffset, gfx_Sprite** ppSprite ) = 0;


/////////////////////////////////////////////////
// Model methods
public:

	BOOL ListModelTextures();
	void DestroyModelTextureList();

	virtual BOOL CreateModel( BYTE* pFileBuffer, gfx_Model** ppModel ) = 0;


/////////////////////////////////////////////////
// Dynamic Image methods
public:

	virtual BOOL CreateDynamicImage( std_Size szImage, gfx_DynamicImage** ppImage ) = 0;


/////////////////////////////////////////////////////////////////////
// Low Level Interface

/////////////////////////////////////////////////
// General

	virtual void PushMatrix() = 0;
	virtual void PopMatrix() = 0;
	virtual void LoadIdentity() = 0;
	virtual void Translate( const std_Vector3 vTranslate ) = 0;
	virtual void Rotate( const std_Vector3 vRotate ) = 0;
	virtual void RotateX( const float fRot ) = 0;
	virtual void RotateY( const float fRot ) = 0;
	virtual void RotateZ( const float fRot ) = 0;
/*
	virtual void SetClearColor( const PALETTEENTRY& peClearColor ) = 0;
	virtual void SetClearColor( const float fR, const float fG, const float fB, const float fA ) = 0;
	virtual void SetClearColor( const float* vfClearColor ) = 0;

	virtual void SetClearDepth( const float fDepthValue ) = 0;
*/

/////////////////////////////////////////////////
// Vertex arrays
/*
	BOOL VA_Create( DWORD dwNumVerts, DWORD dwFormat, DWORD dwUsage, LPVERTEXARRAY* ppVertexArray );
	BOOL VA_Lock( LPVERTEXARRAY lpVertexArray, DWORD dwOffset, DWORD dwSize, LPVOID* ppVertData );
	BOOL VA_Unlock();
*/

/////////////////////////////////////////////////
// Misc static utility methods
public:

	static BOOL RetrieveImageFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, gfx_Image_t* pImage );
	static BOOL RetrieveImagesFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iStartFrame, int iFrameCount, gfx_Image_t* pImage );

	static INLINE BOOL BitBlt( std_Point_t ptDestStart, gfx_Image_t* pDestImage, gfx_Image_t* pSrcImage, std_Rect_t* pSrcRect=NULL );


/////////////////////////////////////////////////////////////////////
// Base class members
protected:

	// Current triangle count of the scene
	DWORD			m_TriangleCount;

	// Settings for the interface
	struct gfx_Settings_t
	{
		DWORD			FeatureTextureMethod;
		DWORD			TerrainTextureMethod;
	}				m_Settings;

	typedef list< BYTE* >		FileBufferList_t;
	FileBufferList_t			m_ModelTextureFiles;

	//struct streq { bool operator()(const char* strA, const char* strB) const { return strcmp(strA,strB)==0; } };
	//typedef unordered_map< char*, BYTE*, hash<char*>, streq >	TextureMap_t;
	struct strless { bool operator()(const char* strA, const char* strB) const { return strcmp(strA,strB)<0; } };
	typedef map< char*, BYTE*, strless >	TextureMap_t;
	TextureMap_t				m_ModelTextures;


}; // End class - gfx_Interface
/////////////////////////////////////////////////////////////////////



#include <string>
#include <list>

// Define some types for the functions used to communicate between the DLLs
typedef LRESULT (CALLBACK* LPRetrieveGfxInterface)(gfx_Interface**);
typedef DWORD (CALLBACK* LPEnumDisplayMode)( DWORD, std_Size*, long* );


// DLL only stuff
#ifdef _USRDLL

	// Prototype the functions used to access the derived interface
	LRESULT WINAPI RetrieveGfxInterface(gfx_Interface** ppGfxInterface );
	DWORD CALLBACK gfx_EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth );

	// Provide some external pointers
	extern gfx_Interface*		pGfxSystem;
	extern gfx_Interface**		ppGfx;

// App only stuff
#else

	// Include inline implementaions here for a NON-debug build
	#ifndef _DEBUG
		#include "gfx_Interface.inl"
	#endif // !defined( _DEBUG )

#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_INTERFACE_H_)
