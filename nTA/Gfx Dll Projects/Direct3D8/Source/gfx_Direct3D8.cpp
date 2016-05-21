// gfx_Direct3D8.cpp //                    Author: Logan "Burn" Jones
//////////////////////                                Date: 9/11/2001
//
/////////////////////////////////////////////////////////////////////

#define INSTANTIATE_GLOBAL_POINTERS
#include "d3d.h"
#include "gfx_Direct3D8.h"

// Implement inline methods here for a debug build
#ifdef _DEBUG
 #include "gfx_Direct3D8.inl"
#endif // defined( _DEBUG )


//////////////////////////////////////////////////////////////////////
// RetrieveGfxInterface() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 9/11/2001
//               
//====================================================================
// Parameters:
//  gfx_Interface** ppGfxInterface - 
//
// Return: LRESULT - 
//
LRESULT WINAPI RetrieveGfxInterface(gfx_Interface** ppGfxInterface )
{
	// Prevent disaster
	if( ppGfxInterface==NULL ) return -1;

	// Allocate a derived version of the interface
	pGfxSystem = new gfx_Direct3D8;
	(*ppGfxInterface) = pGfxSystem;
	ppGfx = ppGfxInterface;

	// Return success
	return 1;
}
// End RetrieveGfxInterface()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_EnumDisplayMode() //                 Author: Logan "Burn" Jones
//////////////////////////                              Date: 8/8/2002
//               
//====================================================================
// Parameters:
//  DWORD dwIndex         - 
//  std_Size* pResolution - 
//  long* pDepth          - 
//
// Return: DWORD - 
//
DWORD CALLBACK gfx_EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth )
{
	static LPDIRECT3D8	pD3D = NULL;
	DWORD				Result;

	// Create the D3D interface if this is the first call
	if( dwIndex==0 ) pD3D = Direct3DCreate8( D3D_SDK_VERSION );

	// Call the class' static implementation
	Result = gfx_Direct3D8::EnumDisplayMode( pD3D, dwIndex, pResolution, pDepth );

	// Release the interface if the enumeration is over
	if( Result==0 ) SAFE_RELEASE( pD3D );

	return Result;
}
// End gfx_EnumDisplayMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::EnumDisplayMode() //      Author: Logan "Burn" Jones
/////////////////////////////////////                   Date: 8/8/2002
//               
//====================================================================
// Parameters:
//  DWORD dwIndex         - 
//  std_Size* pResolution - 
//  long* pDepth          - 
//
// Return: DWORD - 
//
DWORD gfx_Direct3D8::EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth )
{
	return gfx_Direct3D8::EnumDisplayMode( m_pD3D, dwIndex, pResolution, pDepth );
}
// End gfx_Direct3D8::EnumDisplayMode()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gfx_Direct3D8::gfx_Direct3D8():
// Palettes
	m_NumPalettes( 0 ),
	m_pCurrentPalette( NULL ),
// Internal Data
	m_pPlatformParams( NULL ),
	m_pD3D( NULL ),
	m_pD3DDevice( NULL ),
// Sprites
	m_SpriteTextureSize( 1024, 1024 ),
	m_SpriteBuffer( NULL ),
// World matrix
	m_WorldMatrixIndex( 0 )
{}
gfx_Direct3D8::~gfx_Direct3D8()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Initialize() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 9/20/2001
//               
//====================================================================
// Parameters:
//  std_PlatformParameters* pPlatformParams - 
//  LPCommentProc_t lpLoadProc              - 
//  LPCommentProc_t lpErrorProc             - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::Initialize( std_PlatformParameters* pPlatformParams, LPCommentProc_t lpLoadProc, LPCommentProc_t lpErrorProc )
{
	// Sanity check
	assert( pPlatformParams!=NULL && lpLoadProc!=NULL && lpErrorProc!=NULL );

	// Set the member pointers
	m_pPlatformParams = pPlatformParams;
	LoadComment = lpLoadProc;
	ErrorComment= lpErrorProc;

	// Inititialization successful
	return TRUE;
}
// End gfx_Direct3D8::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Create() //               Author: Logan "Burn" Jones
////////////////////////////                           Date: 9/21/2001
//               
//====================================================================
// Parameters:
//  gfx_Resolution_t& resScreen - 
//  int iColorDepth             - 
//  BOOL bFullscreen            - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::Create( const gfx_Resolution_t& resScreen, int iColorDepth, BOOL bFullscreen )
{
	DWORD					WndStyle;
	DWORD					WndExStyle;
	RECT					WindowRect;
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;
	HRESULT					hResult;

#define CHECK_FORMAT( theFORMAT )	( SUCCEEDED(m_pD3D->CheckDeviceType( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, (theFORMAT), (theFORMAT), FALSE )) )

	// Sanity check
	if( (m_pPlatformParams==NULL)||(resScreen.width<=0)||(resScreen.height<=0)||(iColorDepth<=0) )
		return FALSE;

	// Create Direct3D
	m_pD3D = Direct3DCreate8( D3D_SDK_VERSION );
	if( m_pD3D==NULL )
	{
		ErrorComment( "Unable to create the Direct3D8 object." );
		return FALSE;
	}

	// Setup the window styles
	WndStyle = (bFullscreen) ? (WS_POPUP) : (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX);
	WndExStyle=(bFullscreen) ? (WS_EX_APPWINDOW) : (WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	// Get the window's border rect
	SetRect( &WindowRect, 0, 0, resScreen.width, resScreen.height );
	AdjustWindowRectEx( &WindowRect, WndStyle, FALSE, WndExStyle );

	// Reset the window styles
	SetWindowLong( m_pPlatformParams->hMainWindow, GWL_EXSTYLE, WndExStyle );
	SetWindowLong( m_pPlatformParams->hMainWindow, GWL_STYLE, WndStyle );
	SetWindowPos( m_pPlatformParams->hMainWindow, NULL,
		0, 0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		SWP_NOZORDER );

	// Setup D3D's present parameters
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = !bFullscreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// TODO: Un-Hardcode the D3D depth buffer to be selectable
	// For now, set it to 16 bits
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Configure the back buffer format
	if( bFullscreen )
	{
		// Set the back buffer's dimensions to the screen's resolution
		d3dpp.BackBufferWidth = resScreen.width;
		d3dpp.BackBufferHeight = resScreen.height;

		// Set the format according to the desired depth
		switch( iColorDepth )
		{
			case 32:
				if( !CHECK_FORMAT(D3DFMT_X8R8G8B8) )
					if( !CHECK_FORMAT(D3DFMT_R8G8B8) )
					{
						ErrorComment( "Cannot create device with 32 bit back buffer.", iColorDepth );
						return FALSE;
					}
					else d3dpp.BackBufferFormat = D3DFMT_R8G8B8;
				else d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
				break;

			case 16:
			default:
				if( !CHECK_FORMAT(D3DFMT_R5G6B5) )
					if( !CHECK_FORMAT(D3DFMT_X1R5G5B5) )
					{
						ErrorComment( "Cannot create device with 16 bit back buffer.", iColorDepth );
						return FALSE;
					}
					else d3dpp.BackBufferFormat = D3DFMT_X1R5G5B5;
				else d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
				break;

		} // end switch( iColorDepth )

	}
	else // not full screen (windowed)
	{
		// Retrieve the desktop's display mode and use it for ours
		m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
		d3dpp.BackBufferFormat = d3ddm.Format;
	}

	// Create the device
	if( FAILED(hResult=m_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_pPlatformParams->hMainWindow,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice )))
	{
		ErrorComment( "Failed to create the Direct3D rendering device." );
		return FALSE;
	}
	g_pd3dDevice = m_pD3DDevice;

	// Hide the windows mouse curosr for this window
	ShowCursor(FALSE);
	SetCursorPos( 0, 0 );

	// Make sure this window is visible
	ShowWindow( m_pPlatformParams->hMainWindow, SW_SHOW );
	SetForegroundWindow( m_pPlatformParams->hMainWindow );
	SetFocus( m_pPlatformParams->hMainWindow );

	// Setup some default D3D stuff
	m_ScreenResolution = resScreen;
	dxLoadIdentity();
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_FLAT );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHAREF, 0 );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	D3DDEVICE_CREATION_PARAMETERS	DevCreationParams;
	D3DADAPTER_IDENTIFIER8			AdapterIdent;
	char							DeviceType[ 16 ];
	D3DCAPS8						DevCaps;

	m_pD3DDevice->GetCreationParameters( &DevCreationParams );
	m_pD3D->GetAdapterIdentifier( DevCreationParams.AdapterOrdinal, 0, &AdapterIdent );
	switch( DevCreationParams.DeviceType ) {
		case D3DDEVTYPE_HAL: strcpy( DeviceType, "HAL" ); break;
		case D3DDEVTYPE_REF: strcpy( DeviceType, "REF" ); break;
		case D3DDEVTYPE_SW: strcpy( DeviceType, "SW" ); break;
		default: sprintf( DeviceType, "Unknown %d", (int)DevCreationParams.DeviceType ); }
	m_pD3DDevice->GetDeviceCaps( &DevCaps );

	LoadComment( "Direct3D8 adapter %s using driver %s created successfully.", AdapterIdent.Description, AdapterIdent.Driver );
	LoadComment( " Device type: %s", DeviceType );
	LoadComment( " Max Texture Size: %d x %d", DevCaps.MaxTextureWidth, DevCaps.MaxTextureHeight );
	m_Caps.MaxTextureSize.Set( DevCaps.MaxTextureWidth, DevCaps.MaxTextureHeight );

	// If the sprite texture sizze is too large, set it to the max texture size
	if( !(m_SpriteTextureSize<=m_Caps.MaxTextureSize) )
		m_SpriteTextureSize = m_Caps.MaxTextureSize;

	d3d = this;
	return TRUE;
}
// End gfx_Direct3D8::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Destroy() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 9/21/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_Direct3D8::Destroy()
{
	SAFE_RELEASE( m_pD3DDevice );
	SAFE_RELEASE( m_pD3D );
	g_pd3dDevice = NULL;

	return TRUE;
}
// End gfx_Direct3D8::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::TEMP_DoBlueScreen() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 9/21/2001
//               
//====================================================================
//
void gfx_Direct3D8::TEMP_DoBlueScreen()
{
    m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    m_pD3DDevice->BeginScene();
	m_TriangleCount = 0;
    m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}
// End gfx_Direct3D8::TEMP_DoBlueScreen()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::GetScreenResolution() //  Author: Logan "Burn" Jones
/////////////////////////////////////////              Date: 10/4/2001
//               
//====================================================================
// Return: const gfx_Resolution_t* - 
//
const gfx_Resolution_t* gfx_Direct3D8::GetScreenResolution() const
{
	return &m_ScreenResolution;
}
// End gfx_Direct3D8::GetScreenResolution()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::BeginScene() //           Author: Logan "Burn" Jones
////////////////////////////////                      Date: 10/11/2001
//               
//====================================================================
//
void gfx_Direct3D8::BeginScene()
{
    m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    m_pD3DDevice->BeginScene();
	m_TriangleCount = 0;
}
// End gfx_Direct3D8::BeginScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::EndScene() //             Author: Logan "Burn" Jones
//////////////////////////////                        Date: 10/11/2001
//               
//====================================================================
//
void gfx_Direct3D8::EndScene()
{
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}
// End gfx_Direct3D8::EndScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::Set2DProjection() //      Author: Logan "Burn" Jones
/////////////////////////////////////                 Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  gfx_Resolution_t& resScreen - 
//  float fZNear                - 
//  float fZFar                 - 
//
void gfx_Direct3D8::Set2DProjection( const gfx_Resolution_t& resScreen, float fZNear, float fZFar )
{
	D3DXMATRIX Matrix;

	// Set up the projection matrix
	D3DXMatrixOrthoOffCenterRH( &Matrix, 0, resScreen.width, resScreen.height, 0, fZNear, fZFar );

	// Set it to the device
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &Matrix );
}
// End gfx_Direct3D8::Set2DProjection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawRect() //             Author: Logan "Burn" Jones
//////////////////////////////                        Date: 10/13/2001
//               
//====================================================================
// Parameters:
//  std_Rect_t& rctWhere        - 
//  const PALETTEENTRY& peColor - 
//
void gfx_Direct3D8::DrawRect( const std_Rect_t& rctWhere, const PALETTEENTRY& peColor )
{
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[4];

	Verts[0].color = Verts[1].color = Verts[2].color = Verts[3].color = D3DCOLOR_RGBA( peColor.peRed, peColor.peGreen, peColor.peBlue, peColor.peFlags );
	Verts[0].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );
	Verts[1].position = D3DXVECTOR3( rctWhere.left, rctWhere.bottom, 0 );
	Verts[2].position = D3DXVECTOR3( rctWhere.right, rctWhere.top, 0 );
	Verts[3].position = D3DXVECTOR3( rctWhere.right, rctWhere.bottom, 0 );

	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Rect_Vert) );
	m_TriangleCount += 2;
}
// End gfx_Direct3D8::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawRect() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 1/26/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_Direct3D8::DrawRect( std_RectF rctWhere, const DWORD dwColor )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[4];

	Verts[0].color = Verts[1].color = Verts[2].color = Verts[3].color = D3DCOLOR_RGBA( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	Verts[0].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );
	Verts[1].position = D3DXVECTOR3( rctWhere.left, rctWhere.bottom, 0 );
	Verts[2].position = D3DXVECTOR3( rctWhere.right, rctWhere.top, 0 );
	Verts[3].position = D3DXVECTOR3( rctWhere.right, rctWhere.bottom, 0 );

	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Rect_Vert) );
	m_TriangleCount += 2;
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );
}
// End gfx_Direct3D8::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawRect() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere - 
//  const BYTE byIndex - 
//
void gfx_Direct3D8::DrawRect( std_RectF rctWhere, const BYTE byIndex )
{
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[4];
	DWORD Color;

	if( m_pCurrentPalette )
	{
		PALETTEENTRY* pe = static_cast<Palette_t*>(m_pCurrentPalette)->Entries + byIndex;
		Color = D3DCOLOR_RGBA( pe->peRed, pe->peGreen, pe->peBlue, pe->peFlags );
	}
	else Color = 0xFFFFFFFF;
	
	Verts[0].color = Verts[1].color = Verts[2].color = Verts[3].color = Color;
	Verts[0].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );
	Verts[1].position = D3DXVECTOR3( rctWhere.left, rctWhere.bottom, 0 );
	Verts[2].position = D3DXVECTOR3( rctWhere.right, rctWhere.top, 0 );
	Verts[3].position = D3DXVECTOR3( rctWhere.right, rctWhere.bottom, 0 );

	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Rect_Vert) );
	m_TriangleCount += 2;
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );
}
// End gfx_Direct3D8::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawRect() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 2/27/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vTrans  - 
//  const float fRot    - 
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_Direct3D8::DrawRect( std_Vector3 vTrans, const float fRot, std_RectF rctWhere, const DWORD dwColor )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[4];

	Verts[0].color = Verts[1].color = Verts[2].color = Verts[3].color = D3DCOLOR_RGBA( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	Verts[0].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );
	Verts[1].position = D3DXVECTOR3( rctWhere.left, rctWhere.bottom, 0 );
	Verts[2].position = D3DXVECTOR3( rctWhere.right, rctWhere.top, 0 );
	Verts[3].position = D3DXVECTOR3( rctWhere.right, rctWhere.bottom, 0 );

	dxPushMatrix();
	dxTranslatef( vTrans.x, vTrans.y, vTrans.z );
	dxRotateZ( fRot );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, Verts, sizeof(Rect_Vert) );
	dxPopMatrix();
	m_TriangleCount += 2;
}
// End gfx_Direct3D8::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawWireRect() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 5/20/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_Direct3D8::DrawWireRect( std_RectF rctWhere, const DWORD dwColor )
{
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[5];

	Verts[0].color = Verts[1].color = Verts[2].color = Verts[3].color = Verts[4].color = D3DCOLOR_RGBA( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	Verts[0].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );
	Verts[1].position = D3DXVECTOR3( rctWhere.left, rctWhere.bottom, 0 );
	Verts[2].position = D3DXVECTOR3( rctWhere.right, rctWhere.bottom, 0 );
	Verts[3].position = D3DXVECTOR3( rctWhere.right, rctWhere.top, 0 );
	Verts[4].position = D3DXVECTOR3( rctWhere.left, rctWhere.top, 0 );

	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 4, Verts, sizeof(Rect_Vert) );
	m_TriangleCount += 4;
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );
}
// End gfx_Direct3D8::DrawWireRect()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::DrawLine() //            Author: Logan "Burn" Jones
//////////////////////////////                        Date: 1/24/2003
//
//===================================================================
// Parameters:
//  std_Vector2 vFrom   - 
//  std_Vector2 vTo     - 
//  const DWORD dwColor - 
//
void gfx_Direct3D8::DrawLine( std_Vector2 vFrom, std_Vector2 vTo, const DWORD dwColor )
{
	struct Rect_Vert
	{
		D3DXVECTOR3	position;
		D3DCOLOR    color;
	}	Verts[2];

	Verts[0].color = Verts[1].color = D3DCOLOR_RGBA( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	Verts[0].position = D3DXVECTOR3( vFrom.x, vFrom.y, 0 );
	Verts[1].position = D3DXVECTOR3( vTo.x, vTo.y, 0 );

	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	dxSetWorldMatrix();
	m_pD3DDevice->SetVertexShader( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	m_pD3DDevice->SetTexture( 0, NULL );
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINESTRIP, 1, Verts, sizeof(Rect_Vert) );
	m_TriangleCount += 1;
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );
}
// End gfx_Direct3D8::DrawLine()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::NanolatheEffect() //     Author: Logan "Burn" Jones
/////////////////////////////////////                Date: 02-02-2003
//
//===================================================================
// Parameters:
//  std_Vector2 vFrom - 
//  std_Vector2 vTo   - 
//
void gfx_Direct3D8::NanolatheEffect( std_Vector2 vFrom, std_Vector2 vTo )
{
}
// End gfx_Direct3D8::NanolatheEffect()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::SaveScreenShot() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strScreenShotPath - 
//
void gfx_Direct3D8::SaveScreenShot( LPCTSTR strScreenShotPath )
{
    BITMAPFILEHEADER	FileHeader;
    BITMAPINFOHEADER	InfoHeader;
	LPDIRECT3DSURFACE8	BackBuffer;
	D3DSURFACE_DESC		BufferDesc;
	LPDIRECT3DSURFACE8	BackBufferCopy;
	LPDIRECT3DSURFACE8	SceneBuffer;
	char				FilePath[ MAX_PATH ];
	D3DLOCKED_RECT		SceneRect;

//	m_pD3DDevice->CreateImageSurface( m_ScreenResolution.width, m_ScreenResolution.height, D3DFMT_A8R8G8B8, &SceneBuffer );
//	m_pD3DDevice->GetFrontBuffer( SceneBuffer );

	// Get the back buffer
	m_pD3DDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &BackBuffer );
	BackBuffer->GetDesc( &BufferDesc );

	// Create a surface just like the back buffer and copy it
	m_pD3DDevice->CreateImageSurface( BufferDesc.Width, BufferDesc.Height, BufferDesc.Format, &BackBufferCopy );
	m_pD3DDevice->CopyRects( BackBuffer, NULL, 0, BackBufferCopy, NULL );
	BackBuffer->GetDesc( &BufferDesc );
	SAFE_RELEASE( BackBuffer );

	// Create a surface for the scene and copy it from the back buffer copy
	m_pD3DDevice->CreateImageSurface( BufferDesc.Width, BufferDesc.Height, D3DFMT_A8R8G8B8, &SceneBuffer );
	D3DXLoadSurfaceFromSurface( SceneBuffer, NULL, NULL, BackBufferCopy, NULL, NULL, D3DX_FILTER_NONE, 0xFF000000 );
	SAFE_RELEASE( BackBufferCopy );

	// Initialize the bmp file header
    ZeroMemory( &FileHeader, sizeof(BITMAPFILEHEADER) );
    FileHeader.bfType = 0x4D42;
    FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    FileHeader.bfSize = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPINFOHEADER) + (*m_ScreenResolution * 4);

	// Initialize the bmp info header
    ZeroMemory( &InfoHeader, sizeof(BITMAPINFOHEADER) );
    InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    InfoHeader.biWidth = m_ScreenResolution.width;
    InfoHeader.biHeight = m_ScreenResolution.height;
    InfoHeader.biPlanes = 1;
    InfoHeader.biBitCount = 32;
    InfoHeader.biCompression = BI_RGB;
    InfoHeader.biSizeImage = *m_ScreenResolution * 4;

	// Open the file and write the headers to it
	sprintf( FilePath, "%s.bmp", strScreenShotPath );
    void* hFile = fio_OpenFile( FilePath, fio_WRITE );
    fio_WriteFile( hFile, &FileHeader, sizeof(BITMAPFILEHEADER) );
    fio_WriteFile( hFile, &InfoHeader, sizeof(BITMAPINFOHEADER) );

	// Lock the scene surface and weite its data to the file
	if( FAILED(SceneBuffer->LockRect( &SceneRect, NULL, 0 )) )
		assertEx( 0, "Surface not lockable" );
	BYTE* pSrcBit = (BYTE*)SceneRect.pBits + ((*m_ScreenResolution - m_ScreenResolution.width) * 4);
	for( DWORD y=0; y<m_ScreenResolution.height; ++y,pSrcBit-=(m_ScreenResolution.width*4))
		fio_WriteFile( hFile, pSrcBit, m_ScreenResolution.width * 4 );
	SceneBuffer->UnlockRect();
	SAFE_RELEASE( SceneBuffer );

	// Close the file
    fio_CloseFile( hFile );
}
// End gfx_Direct3D8::SaveScreenShot()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::BeginGameScene() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 4/10/2002
//               
//====================================================================
//
void gfx_Direct3D8::BeginGameScene()
{
    m_pD3DDevice->BeginScene();
	m_TriangleCount = 0;
}
// End gfx_Direct3D8::BeginGameScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::EndGameScene() //         Author: Logan "Burn" Jones
//////////////////////////////////                     Date: 4/10/2002
//               
//====================================================================
//
void gfx_Direct3D8::EndGameScene()
{
	m_pD3DDevice->EndScene();
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}
// End gfx_Direct3D8::EndGameScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::BeginObjectRendering() // Author: Logan "Burn" Jones
//////////////////////////////////////////             Date: 4/10/2002
//               
//====================================================================
//
void gfx_Direct3D8::BeginObjectRendering()
{
	// Enable the depth buffer
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

	// Render the terrain with a depth test that always passes and without the aplha test and
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_ALWAYS );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	m_theTerrain.Render();
	m_pD3DDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );

	// Disable the depth buffer (it will be enabled while rendering models)
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );

	// Enable alpha blending
	m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	m_pD3DDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

	m_SpriteBufferOffset = 0;
	m_CurrentSpriteTexture = NULL;
}
// End gfx_Direct3D8::BeginObjectRendering()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::BeginInterfaceRendering() // Author: Logan "Burn" Jones
/////////////////////////////////////////////          Date: 4/10/2002
//               
//====================================================================
//
void gfx_Direct3D8::BeginInterfaceRendering()
{
	FlushSpriteBuffer();
}
// End gfx_Direct3D8::BeginInterfaceRendering()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::GetErrorString() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 10/4/2001
//               
//====================================================================
// Parameters:
//  LRESULT lErrorCode - 
//
// Return: LPCTSTR - 
//
LPCTSTR gfx_Direct3D8::GetErrorString( LRESULT lErrorCode ) const
{
#ifdef _DEBUG
	// Whats the error
	switch( lErrorCode )
	{
		case D3D_OK: return "D3D_OK";
		case D3DERR_CONFLICTINGRENDERSTATE: return "D3DERR_CONFLICTINGRENDERSTATE";
		case D3DERR_CONFLICTINGTEXTUREFILTER : return "D3DERR_CONFLICTINGTEXTUREFILTER";
		case D3DERR_CONFLICTINGTEXTUREPALETTE : return "D3DERR_CONFLICTINGTEXTUREPALETTE";
		case D3DERR_DEVICELOST : return "D3DERR_DEVICELOST";
		case D3DERR_DEVICENOTRESET : return "D3DERR_DEVICENOTRESET";
		case D3DERR_DRIVERINTERNALERROR : return "D3DERR_DRIVERINTERNALERROR";
		case D3DERR_INVALIDCALL : return "D3DERR_INVALIDCALL";
		case D3DERR_INVALIDDEVICE : return "D3DERR_INVALIDDEVICE";
		case D3DERR_MOREDATA : return "D3DERR_MOREDATA";
		case D3DERR_NOTAVAILABLE : return "D3DERR_NOTAVAILABLE";
		case D3DERR_NOTFOUND : return "D3DERR_NOTFOUND";
		case D3DERR_OUTOFVIDEOMEMORY : return "D3DERR_OUTOFVIDEOMEMORY";
		case D3DERR_TOOMANYOPERATIONS : return "D3DERR_TOOMANYOPERATIONS";
		case D3DERR_UNSUPPORTEDALPHAARG : return "D3DERR_UNSUPPORTEDALPHAARG";
		case D3DERR_UNSUPPORTEDALPHAOPERATION : return "D3DERR_UNSUPPORTEDALPHAOPERATION";
		case D3DERR_UNSUPPORTEDCOLORARG : return "D3DERR_UNSUPPORTEDCOLORARG";
		case D3DERR_UNSUPPORTEDCOLOROPERATION : return "D3DERR_UNSUPPORTEDCOLOROPERATION";
		case D3DERR_UNSUPPORTEDFACTORVALUE : return "D3DERR_UNSUPPORTEDFACTORVALUE";
		case D3DERR_UNSUPPORTEDTEXTUREFILTER : return "D3DERR_UNSUPPORTEDTEXTUREFILTER";
		case D3DERR_WRONGTEXTUREFORMAT : return "D3DERR_WRONGTEXTUREFORMAT";
		case E_FAIL : return "E_FAIL";
		case E_INVALIDARG : return "E_INVALIDARG";
		//case E_INVALIDCALL : return "E_INVALIDCALL";
		case E_OUTOFMEMORY : return "E_OUTOFMEMORY";
		//case S_OK : return "S_OK";

		default: return "UNKNOWN_ERROR_CODE";

	} // end switch( lErrorCode )
#else
	return "D3DERR";
#endif
}
// End gfx_Direct3D8::GetErrorString()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_Direct3D8.cpp //
////////////////////////////
