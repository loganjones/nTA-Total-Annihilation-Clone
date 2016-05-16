// gfx_OpenGL.cpp //                       Author: Logan "Burn" Jones
///////////////////                                   Date: 9/21/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "gfx_OpenGL.h"

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
	pGfxSystem = new gfx_OpenGL;
	(*ppGfxInterface) = pGfxSystem;
	ppGfx = ppGfxInterface;

	// Return success
	return 1;
}
// End RetrieveGfxInterface()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
gfx_OpenGL::gfx_OpenGL():
// Platform
	m_pPlatformParams( NULL ),
// Palettes
	m_pCurrentPalette( NULL ),
#ifdef _WIN32
	m_hDC( NULL ),
	m_hRC( NULL ),
#endif
// Initialize all extension functions to NULL
    glColorTableEXT( NULL )
{}
gfx_OpenGL::~gfx_OpenGL()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::EnumDisplayMode() //         Author: Logan "Burn" Jones
//////////////////////////////////                      Date: 8/8/2002
//               
//====================================================================
// Parameters:
//  DWORD dwIndex         - 
//  std_Size* pResolution - 
//  long* pDepth          - 
//
// Return: DWORD - 
//
DWORD gfx_OpenGL::EnumDisplayMode( DWORD dwIndex, std_Size* pResolution, long* pDepth )
{
	return gfx_EnumDisplayMode( dwIndex, pResolution, pDepth );
}
// End gfx_OpenGL::EnumDisplayMode()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Initialize() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 9/21/2001
//               
//====================================================================
// Parameters:
//  std_PlatformParameters* pPlatformParams - 
//  LPCommentProc_t lpLoadProc              - 
//  LPCommentProc_t lpErrorProc             - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::Initialize( std_PlatformParameters* pPlatformParams, LPCommentProc_t lpLoadProc, LPCommentProc_t lpErrorProc )
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
// End gfx_OpenGL::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::PostCreate() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 9/27/2001
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_OpenGL::PostCreate()
{
	GLint	iNum;

	// Setup some default OpenGL stuff
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );
	glDisable( GL_LIGHTING );
    glEnable(GL_CULL_FACE);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_NOTEQUAL, 0.0f );
    glEnable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );
	glPolygonMode(GL_FRONT,GL_FILL);

	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &iNum );
	m_Caps.MaxTextureSize.Set( iNum, iNum );

	LoadComment( "OpenGL device created successfully." );
	LoadComment( " Vendor  : %s", (LPTSTR)glGetString(GL_VENDOR) );
	LoadComment( " Renderer: %s", (LPTSTR)glGetString(GL_RENDERER) );
	LoadComment( " Version : %s", (LPTSTR)glGetString(GL_VERSION) );
	LoadComment( " Max Texture Size: %d x %d", iNum, iNum );

	return TRUE;
}
// End gfx_OpenGL::PostCreate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::GetScreenResolution() //     Author: Logan "Burn" Jones
//////////////////////////////////////                 Date: 10/4/2001
//               
//====================================================================
// Return: const gfx_Resolution_t* - 
//
const gfx_Resolution_t* gfx_OpenGL::GetScreenResolution() const
{
	return &m_ScreenResolution;
}
// End gfx_OpenGL::GetScreenResolution()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::BeginScene() //              Author: Logan "Burn" Jones
/////////////////////////////                         Date: 10/11/2001
//               
//====================================================================
//
void gfx_OpenGL::BeginScene()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	m_TriangleCount = 0;
}
// End gfx_OpenGL::BeginScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::EndScene() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/11/2001
//               
//====================================================================
//
void gfx_OpenGL::EndScene()
{
#ifdef _WIN32
	SwapBuffers(m_hDC);
#endif
}
// End gfx_OpenGL::EndScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CheckForExtensionString() // Author: Logan "Burn" Jones
//////////////////////////////////////////             Date: 10/8/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strExtension - The extension string to be queried
//
// Return: BOOL - TRUE if the extension is present, FALSE otherwise
//
BOOL gfx_OpenGL::CheckForExtensionString( LPCTSTR strExtension )
{
    LPCTSTR     ExtensionString;
    LPCTSTR     PossibleOccurance;
    LPCTSTR     EndOfOccurance;

    // Check if the given string is valid
	if( (strchr(strExtension,' ')!=NULL)||(strExtension==NULL)||(strExtension[0]=='\0') )
		return FALSE;

    // Get the extensions string
    ExtensionString = (LPCTSTR)glGetString( GL_EXTENSIONS );

    // Loop until the extension is found or not found
	while( TRUE )
	{
        // Look for strExtension in ExtensionString
		PossibleOccurance = strstr( ExtensionString, strExtension );

        // Check to see if it was found
		if( PossibleOccurance==NULL )
			return FALSE;

		EndOfOccurance = PossibleOccurance + strlen(strExtension);

        // Check if the found string is actually a sub-string
        if( (PossibleOccurance==ExtensionString)||((PossibleOccurance-1)[0]==' ') )
        {
            if( (EndOfOccurance[0]==' ')||(EndOfOccurance[0]=='\0') )
            {
                return TRUE;
            }
        }

		ExtensionString = EndOfOccurance + 1;

    } // end while( true )

	return FALSE;
}
// End gfx_OpenGL::CheckForExtensionString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::Set2DProjection() //         Author: Logan "Burn" Jones
//////////////////////////////////                    Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  gfx_Resolution_t& resScreen - 
//  float fZNear                - 
//  float fZFar                 - 
//
void gfx_OpenGL::Set2DProjection( const gfx_Resolution_t& resScreen, float fZNear, float fZFar )
{
    // Switch to the projection matrix
    glMatrixMode(GL_PROJECTION);

    // Load the identity
	glLoadIdentity();

    // Multiply by the orthographic matrix
    // with the origin at the top left
	glOrtho( 0, resScreen.width, resScreen.height, 0, fZNear, fZFar );

    // Switch back to the model view matrix
	glMatrixMode(GL_MODELVIEW);
}
// End gfx_OpenGL::Set2DProjection()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawRect() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/13/2001
//               
//====================================================================
// Parameters:
//  std_Rect_t& rctWhere        - 
//  const PALETTEENTRY& peColor - 
//
void gfx_OpenGL::DrawRect( const std_Rect_t& rctWhere, const PALETTEENTRY& peColor )
{
	glDisable( GL_TEXTURE_2D );
	glColor3ubv( (GLubyte*)&peColor );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2i( (GLint)rctWhere.left, (GLint)rctWhere.top );
		glVertex2i( (GLint)rctWhere.left, (GLint)rctWhere.bottom );
		glVertex2i( (GLint)rctWhere.right, (GLint)rctWhere.top );
		glVertex2i( (GLint)rctWhere.right, (GLint)rctWhere.bottom );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	m_TriangleCount += 2;
}
// End gfx_OpenGL::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawRect() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 1/26/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_OpenGL::DrawRect( std_RectF rctWhere, const DWORD dwColor )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	glDisable( GL_TEXTURE_2D );
	//glColor4ubv( (GLubyte*)&dwColor );
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2f( rctWhere.left, rctWhere.top );
		glVertex2f( rctWhere.left, rctWhere.bottom );
		glVertex2f( rctWhere.right, rctWhere.top );
		glVertex2f( rctWhere.right, rctWhere.bottom );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	m_TriangleCount += 2;
}
// End gfx_OpenGL::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawRect() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere - 
//  const BYTE byIndex - 
//
void gfx_OpenGL::DrawRect( std_RectF rctWhere, const BYTE byIndex )
{
	if( m_pCurrentPalette )
	{
		PALETTEENTRY* pe = static_cast<Palette_t*>(m_pCurrentPalette)->Entries + byIndex;
		glColor4ub( pe->peRed, pe->peGreen, pe->peBlue, pe->peFlags );
	}
	else glColor4f(1,1,1,1);

	glDisable( GL_TEXTURE_2D );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2f( rctWhere.left, rctWhere.top );
		glVertex2f( rctWhere.left, rctWhere.bottom );
		glVertex2f( rctWhere.right, rctWhere.top );
		glVertex2f( rctWhere.right, rctWhere.bottom );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	m_TriangleCount += 2;
}
// End gfx_OpenGL::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawRect() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 2/27/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vTrans  - 
//  const float fRot    - 
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_OpenGL::DrawRect( std_Vector3 vTrans, const float fRot, std_RectF rctWhere, const DWORD dwColor )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	glPushMatrix();
	glTranslatef(vTrans.x,vTrans.y,vTrans.z);
	glRotatef(0,0,1,fRot);
	glDisable( GL_TEXTURE_2D );
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glBegin( GL_TRIANGLE_STRIP );
		glVertex2f( rctWhere.left, rctWhere.top );
		glVertex2f( rctWhere.left, rctWhere.bottom );
		glVertex2f( rctWhere.right, rctWhere.top );
		glVertex2f( rctWhere.right, rctWhere.bottom );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	glPopMatrix();
	m_TriangleCount += 2;
}
// End gfx_OpenGL::DrawRect()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawWireRect() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/20/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctWhere  - 
//  const DWORD dwColor - 
//
void gfx_OpenGL::DrawWireRect( std_RectF rctWhere, const DWORD dwColor )
{
	glDisable( GL_TEXTURE_2D );
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glBegin( GL_LINE_LOOP );
		glVertex2f( rctWhere.left, rctWhere.top );
		glVertex2f( rctWhere.left, rctWhere.bottom );
		glVertex2f( rctWhere.right, rctWhere.bottom );
		glVertex2f( rctWhere.right, rctWhere.top );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	m_TriangleCount += 4;
}
// End gfx_OpenGL::DrawWireRect()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gfx_OpenGL::DrawLine() //               Author: Logan "Burn" Jones
///////////////////////////                           Date: 1/24/2003
//
//===================================================================
// Parameters:
//  std_Vector2 vFrom   - 
//  std_Vector2 vTo     - 
//  const DWORD dwColor - 
//
void gfx_OpenGL::DrawLine( std_Vector2 vFrom, std_Vector2 vTo, const DWORD dwColor )
{
	glDisable( GL_TEXTURE_2D );
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glBegin( GL_LINES );
		glVertex2f( vFrom.x, vFrom.y );
		glVertex2f( vTo.x, vTo.y );
	glEnd();
	glColor4f(1,1,1,1);
	glEnable( GL_TEXTURE_2D );
	m_TriangleCount += 1;
}
// End gfx_OpenGL::DrawLine()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// gfx_OpenGL::NanolatheEffect() //        Author: Logan "Burn" Jones
//////////////////////////////////                   Date: 02-02-2003
//
//===================================================================
// Parameters:
//  std_Vector2 vFrom - 
//  std_Vector2 vTo   - 
//
void gfx_OpenGL::NanolatheEffect( std_Vector2 vFrom, std_Vector2 vTo )
{
}
// End gfx_OpenGL::NanolatheEffect()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::BeginGameScene() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 4/10/2002
//               
//====================================================================
//
void gfx_OpenGL::BeginGameScene()
{
	m_TriangleCount = 0;
}
// End gfx_OpenGL::BeginGameScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::EndGameScene() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 4/10/2002
//               
//====================================================================
//
void gfx_OpenGL::EndGameScene()
{
#ifdef _WIN32
	SwapBuffers(m_hDC);
#endif
}
// End gfx_OpenGL::EndGameScene()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::BeginObjectRendering() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 4/10/2002
//               
//====================================================================
//
void gfx_OpenGL::BeginObjectRendering()
{
	// Enable the depth buffer
	glEnable( GL_DEPTH_TEST );

	// Render the terrain with a depth test that always passes and without the aplha test and
    glDisable(GL_ALPHA_TEST);
	glDisable( GL_BLEND );
	glDepthFunc( GL_ALWAYS );
	m_theTerrain.Render();
    glEnable(GL_ALPHA_TEST);

	// Disable the depth buffer (it will be enabled while rendering models)
	glDisable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );

	// Enable blending for shadows and other stuff
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}
// End gfx_OpenGL::BeginObjectRendering()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::BeginInterfaceRendering() // Author: Logan "Burn" Jones
//////////////////////////////////////////             Date: 4/10/2002
//               
//====================================================================
//
void gfx_OpenGL::BeginInterfaceRendering()
{
}
// End gfx_OpenGL::BeginInterfaceRendering()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_OpenGL.cpp //
/////////////////////////
