// gfx_Direct3D8.h //                      Author: Logan "Burn" Jones
////////////////////                                  Date: 9/11/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_DIRECT3D8_H_
#define _GFX_DIRECT3D8_H_
/////////////////////////////////////////////////////////////////////

#include <D3D8.h>
#include <D3DX8.h>


struct Surface_Vert
{
	D3DXVECTOR3	position;
	FLOAT       tu, tv;
};
#define D3DFVF_SURFACE_VERT	( D3DFVF_XYZ | D3DFVF_TEX1 )

struct Font_Vert_t
{
	D3DXVECTOR3	position;
	FLOAT       tu, tv;
};
#define D3DFVF_FONT_VERT ( D3DFVF_XYZ | D3DFVF_TEX1 )


#include "d3d_Surface.h"
#include "d3d_Font.h"
#include "d3d_Cursor.h"
#include "d3d_Terrain.h"
#include "d3d_DynamicImage.h"

//////////////////////////// gfx_Direct3D8 //////////////////////////
//
class gfx_Direct3D8 : public gfx_Interface
{
	friend 
	DWORD CALLBACK EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth );
	friend class d3d_Surface;
	friend class d3d_Terrain;
	friend class d3d_Sprite;
	friend class d3d_Model;
	friend class d3d_Model_Dynamic;
	friend class d3d_DynamicImage;

/////////////////////////////////////////////////
// Types
protected:

	struct LocalCaps_t
	{
		std_Size_t		MaxTextureSize;
	};

	struct SpriteTexItem_t;
	typedef list< SpriteTexItem_t >			SpriteTexItemList_t;


public:

	struct Palette_t
	{
		PALETTEENTRY		Entries[256];
		UINT				Index;
	};
	typedef list< Palette_t >			PaletteList_t;
	typedef list< d3d_Surface >			SurfaceList_t;
	typedef list< d3d_Font >			FontList_t;
	typedef list< d3d_Cursor >			CursorList_t;
	typedef list< d3d_DynamicImage >	DynamicImageList_t;

/////////////////////////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Initialize( std_PlatformParameters* pPlatformParams, LPCommentProc_t lpLoadProc, LPCommentProc_t lpErrorProc );

	BOOL Create( gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen );
	BOOL Destroy();


/////////////////////////////////////////////////
// General info methods

	// Mirror of the global function by the same name
	DWORD EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth );
	static INLINE DWORD EnumDisplayMode( LPDIRECT3D8 pD3D, DWORD dwIndex, std_Size* pResolution, long* pDepth );

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

	// Creates an image palette from the given entries
	BOOL CreatePalette( PALETTEENTRY* pPaletteEntries, LPPALETTE* ppPalette, BOOL bSetAsCurrent=TRUE );

	// Sets the given palette as the current image palette
	BOOL SetCurrentPalette( LPPALETTE pPalette );

	// Points the given pointer to the current image palette
	BOOL GetCurrentPalette( LPPALETTE* ppPalette );

	// Destroys a given palette
	BOOL DestroyPalette( LPPALETTE* pPalette );


/////////////////////////////////////////////////
// Surfaces

	// Creates an image surface that can be rendered to the screen
	BOOL CreateSurface( GFX_PIXEL_FORMAT pxFormat, gfx_Image_t* pImage, gfx_Surface** ppSurface );

	// Destroys a given surface
	void DestroySurface( gfx_Surface** pSurface );

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
// Sprite buffer methods

	BOOL CreateSpriteBuffer();
	void DestroySpriteBuffer();

	void FlushSpriteBuffer();


/////////////////////////////////////////////////
// Misc methods

	BOOL LoadTerrain( BYTE* pTntBuffer, gfx_Terrain** ppTerrain );
	BOOL CreateSprite( gfx_Image_t* pImage, std_Vector2 vOffset, gfx_Sprite** ppSprite );
	BOOL CreateModel( BYTE* pFileBuffer, gfx_Model** ppModel );
	BOOL CreateDynamicImage( std_Size szImage, gfx_DynamicImage** ppImage );


/////////////////////////////////////////////////////////////////////
// Low Level Interface

/////////////////////////////////////////////////
// World matrix transformation

	#define WORLD_MATRIX_STACK_DEPTH		( 32 )
	D3DXMATRIX					m_WorldMatrix[ WORLD_MATRIX_STACK_DEPTH ];
	int							m_WorldMatrixIndex;

	INLINE void dxSetWorldMatrix();
	INLINE void dxLoadIdentity();
	INLINE void dxPushMatrix();
	INLINE void dxPopMatrix();
	INLINE void dxMultMatrix( const D3DXMATRIX& matrix );
	INLINE void dxTranslatef( std_Vector3 vTranslation );
	INLINE void dxTranslatef( float fX, float fY, float fZ );
	INLINE void dxRotatef( float fAngle, float fX, float fY, float fZ );
	INLINE void dxRotateX( float fX );
	INLINE void dxRotateY( float fY );
	INLINE void dxRotateZ( float fZ );
	INLINE void dxRotate( std_Vector3 vRotate );


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

	LRESULT CreateTexture( GFX_PIXEL_FORMAT pxFormat, gfx_SubImage_t* pImage, std_Size_t& szTex, LPDIRECT3DTEXTURE8* ppTexture );

	LPCTSTR GetErrorString( LRESULT lErrorCode ) const;


/////////////////////////////////////////////////
// Data members
protected:

	std_PlatformParameters*		m_pPlatformParams;
	LPCommentProc_t				LoadComment;
	LPCommentProc_t				ErrorComment;

	LPDIRECT3D8					m_pD3D;
	LPDIRECT3DDEVICE8			m_pD3DDevice;

	LocalCaps_t					m_Caps;

	gfx_Resolution_t			m_ScreenResolution;

	LPVOID						m_TEMP;

	PaletteList_t				m_Palettes;
	UINT						m_NumPalettes;
	LPPALETTE					m_pCurrentPalette;

	SurfaceList_t				m_Surfaces;

	FontList_t					m_Fonts;
	gfx_Font*					m_ActiveFont;

	CursorList_t				m_Cursors;
	gfx_Cursor*					m_ActiveCursor;

	d3d_Terrain					m_theTerrain;

	std_Size					m_SpriteTextureSize;
	SpriteTexItemList_t			m_SpriteTextures;
	LPDIRECT3DVERTEXBUFFER8		m_SpriteBuffer;
	DWORD						m_SpriteBufferOffset;
	LPDIRECT3DTEXTURE8			m_CurrentSpriteTexture;

	DynamicImageList_t			m_DynamicImages;


/////////////////////////////////////////////////
// Type definitions
protected:

	struct SpriteTexItem_t
	{
		LPDIRECT3DTEXTURE8		pTexture;
		class util_RectFiller*	pSpace;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	gfx_Direct3D8();
	virtual ~gfx_Direct3D8();
/////////////////////////////////////////////////

}; // End class - gfx_Direct3D8
/////////////////////////////////////////////////////////////////////

// Implement inline methods here for a NON debug build
#ifndef _DEBUG
 #include "gfx_Direct3D8.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_DIRECT3D8_H_)
