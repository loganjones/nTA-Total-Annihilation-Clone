// ogl_Font.cpp //                         Author: Logan "Burn" Jones
/////////////////                                    Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Font.h"


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateFont() //              Author: Logan "Burn" Jones
/////////////////////////////                         Date: 10/10/2001
//               
//====================================================================
// Parameters:
//  FontEntry* pFontEntries      - 
//  std_Size_t szFontDist        - 
//  GFX_PIXEL_FORMAT pxSrcFormat - 
//  gfx_Image_t* pImage          - 
//  gfx_Font** ppFont            - 
//  BOOL bSetAsCurrent           - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateFont( FontEntry* pFontEntries, std_Size_t szFontDist, GFX_PIXEL_FORMAT pxSrcFormat, gfx_Image_t* pImage, gfx_Font** ppFont, BOOL bSetAsCurrent )
{
#define TEX_COORD_U( T )		( (float)(T) / pImage->Size.width )
#define TEX_COORD_V( T )		( (float)(T) / pImage->Size.height )
    GLuint					GLTexInternalFormat;
    GLuint					GLTexFormat;
	ogl_Font				NewFont;
	FontEntry*				pEntry;
	std_Size_t				MaxFontSize;
	long					TotalHeight;
	std_Point_t				TexOffset;
	std_Vector2				Origin;
	std_SizeF				Size;

	// Check the pixel format
	switch( pxSrcFormat )
    {

		case PIXEL_PALETTISED:
			GLTexInternalFormat = GL_COLOR_INDEX8_EXT;
			GLTexFormat = GL_COLOR_INDEX;
			break;

		case PIXEL_RGB:
			GLTexInternalFormat = GL_RGB;
			GLTexFormat = GL_RGB;
			break;

		case PIXEL_RGBA:
			GLTexInternalFormat = GL_RGBA;
			GLTexFormat = GL_RGBA;
			break;

		default: // This is our error condition; not supposed to happen
			ErrorComment( "ERROR: Invalid surface creation type, %d.", pxSrcFormat );
			assertEx( 0, "Invalid surface creation type." );
			return FALSE;

    } // end switch( pxSrcFormat )

	glGenTextures( 1, &NewFont.m_Texture );
    glBindTexture( GL_TEXTURE_2D, NewFont.m_Texture );
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D( GL_TEXTURE_2D,
				  0,
				  GLTexInternalFormat,
				  pImage->Size.width,
				  pImage->Size.height,
				  0,
				  GLTexFormat,
				  GL_UNSIGNED_BYTE,
				  pImage->pBytes );
	
	// Calculate the max font size
	MaxFontSize.width = pImage->Size.width / szFontDist.width;
	MaxFontSize.height= pImage->Size.height/ szFontDist.height;

	NewFont.m_CharLists = glGenLists(256);
	pEntry = pFontEntries;
	TexOffset = std_Point_t(0,0);
	NewFont.m_Height = TotalHeight = 0;
	for( long y=0,iIndex=0; y<szFontDist.height; ++y)
	{
		TexOffset.x = 0;
		for( long x=0; x<szFontDist.width; ++x,++pEntry,++iIndex)
		{
			float w = pEntry->Size.width;
			float h = pEntry->Size.height;
			int ix= pEntry->Offset.x;
			int iy= h-pEntry->Offset.y;
			//Origin.Set( pEntry->Offset.x, pEntry->Size.height - pEntry->Offset.y );
			//SET_VERTEX( Origin.x, Origin.y, 0, TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y) );
			//SET_VERTEX( Origin.x, Origin.y - pEntry->Size.height, 0, TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );
			//SET_VERTEX( Origin.x + pEntry->Size.width, Origin.y, 0, TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y) );
			//SET_VERTEX( Origin.x + pEntry->Size.width, Origin.y - pEntry->Size.height, 0, TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );
			glNewList( NewFont.m_CharLists + iIndex, GL_COMPILE );
				glBegin( GL_TRIANGLE_STRIP );
					glTexCoord2f( TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y) );
					glVertex2i( ix, iy-h );
					glTexCoord2f( TEX_COORD_U(TexOffset.x), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );
					glVertex2i( ix, iy );
					glTexCoord2f( TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y) );
					glVertex2i( ix+w, iy-h );
					glTexCoord2f( TEX_COORD_U(TexOffset.x+pEntry->Size.width), TEX_COORD_V(TexOffset.y+pEntry->Size.height) );
					glVertex2i( ix+w, iy );
				glEnd();
				glTranslatef( w, 0, 0 );
			glEndList();

			TotalHeight += pEntry->Size.height;

			TexOffset.x += MaxFontSize.width;
		}

		TexOffset.y += MaxFontSize.height;
	}


	// Set the rest of the font's data and add it to the list
	memcpy( NewFont.m_Entries, pFontEntries, sizeof(NewFont.m_Entries) );
	NewFont.m_pHost = &m_Fonts;
	NewFont.m_Height = TotalHeight / *szFontDist;
	m_Fonts.push_front( NewFont );
	*ppFont = &m_Fonts.front();
	m_Fonts.front().m_HostIt = m_Fonts.begin();

	// Set the new font as the current one if desired
	if( bSetAsCurrent )
		SetCurrentFont( *ppFont );

	// Return success
	return TRUE;
}
// End gfx_OpenGL::CreateFont()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Font::ogl_Font():
	m_Texture( 0 ),
	m_CharLists( 0 ),
	m_Height( 0 ),
	m_pHost( NULL )
{}
ogl_Font::~ogl_Font()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Font::Destroy() //                   Author: Logan "Burn" Jones
////////////////////////                              Date: 10/13/2001
//               
//====================================================================
//
void ogl_Font::Destroy()
{
	if( m_pHost )
	{
		glDeleteLists( m_CharLists, 256 );
		glDeleteTextures( 1, &m_Texture );
		list< ogl_Font >* pList = m_pHost;
		m_pHost = NULL;
		m_pHost->erase( m_HostIt );
	}
}
// End ogl_Font::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Font::GetHeight() //                 Author: Logan "Burn" Jones
//////////////////////////                            Date: 10/24/2001
//               
//====================================================================
// Return: long - 
//
long ogl_Font::GetHeight() const
{
	return (m_Height);
}
// End ogl_Font::GetHeight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::SetCurrentFont() //          Author: Logan "Burn" Jones
/////////////////////////////////                     Date: 10/22/2001
//               
//====================================================================
// Parameters:
//  gfx_Font* pFont - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::SetCurrentFont( gfx_Font* pFont )
{
	return (m_ActiveFont = pFont)!=NULL;
}
// End gfx_OpenGL::SetCurrentFont()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderString() //            Author: Logan "Burn" Jones
///////////////////////////////                       Date: 10/10/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender - 
//
void gfx_OpenGL::RenderString( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	const ogl_Font* Font = (pFont==NULL) ? static_cast<ogl_Font*>(m_ActiveFont) : static_cast<ogl_Font*>(pFont);

	// Call each display list corresponding to the characters given
    glPushMatrix();
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glTranslatef(vWhere.x,vWhere.y,vWhere.z);
		glBindTexture( GL_TEXTURE_2D, Font->m_Texture );
		glListBase( Font->m_CharLists );
		glCallLists( strlen(strToRender), GL_BYTE, strToRender );
	glColor4f(1,1,1,1);
	glPopMatrix();
	m_TriangleCount += strlen(strToRender) * 2;
}
// End gfx_OpenGL::RenderString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderString() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 5/10/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender      - 
//  DWORD dwLength           - 
//  const std_Vector3 vWhere - 
//  const DWORD dwColor      - 
//  gfx_Font* pFont          - 
//
// Return: DWORD - 
//
DWORD gfx_OpenGL::RenderString( LPCTSTR strToRender, DWORD dwLength, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
#define Elmt( POS )		((dwColor >> (24 - (POS << 3) )) & 0xFF)
	const ogl_Font* Font = (pFont==NULL) ? static_cast<ogl_Font*>(m_ActiveFont) : static_cast<ogl_Font*>(pFont);
	DWORD Width = 0;

	// Call each display list corresponding to the characters given
    glPushMatrix();
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glTranslatef(vWhere.x,vWhere.y,vWhere.z);
		glBindTexture( GL_TEXTURE_2D, Font->m_Texture );
		glListBase( Font->m_CharLists );
		glCallLists( dwLength, GL_BYTE, strToRender );
		for( DWORD n=0; n<dwLength; ++n )
			Width += Font->m_Entries[strToRender[n]].Size.width;
	glColor4f(1,1,1,1);
	glPopMatrix();
	m_TriangleCount += dwLength * 2;
	return Width;
}
// End gfx_OpenGL::RenderString()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderStringRight() //       Author: Logan "Burn" Jones
////////////////////////////////////                   Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strToRender      - 
//  const std_Vector3 vWhere - 
//  const DWORD dwColor      - 
//  gfx_Font* pFont          - 
//
void gfx_OpenGL::RenderStringRight( LPCTSTR strToRender, const std_Vector3 vWhere, const DWORD dwColor, gfx_Font* pFont )
{
	const ogl_Font* Font = (pFont==NULL) ? static_cast<ogl_Font*>(m_ActiveFont) : static_cast<ogl_Font*>(pFont);

	// Call each display list corresponding to the characters given
    glPushMatrix();
	glColor4ub( Elmt(0), Elmt(1), Elmt(2), Elmt(3) );
	glTranslatef(vWhere.x,vWhere.y,vWhere.z);
	glBindTexture( GL_TEXTURE_2D, Font->m_Texture );
	long Len = strlen( strToRender );
	for( long n=Len-1; n>=0; --n)
	{
		glTranslatef( -Font->m_Entries[strToRender[n]].Size.width, 0, 0 );
		glCallList( strToRender[n] + Font->m_CharLists );
		glTranslatef( -Font->m_Entries[strToRender[n]].Size.width, 0, 0 );
	}
	glColor4f(1,1,1,1);
	glPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_OpenGL::RenderStringRight()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderStringAt() //          Author: Logan "Burn" Jones
/////////////////////////////////                     Date: 10/14/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere - 
//  LPCTSTR strToRender  - 
//
void gfx_OpenGL::RenderStringAt( std_Point_t& ptWhere, LPCTSTR strToRender )
{
	const ogl_Font* pFont = static_cast<ogl_Font*>(m_ActiveFont);

	// Call each display list corresponding to the characters given
    glPushMatrix();
		glTranslatef( ptWhere.x, ptWhere.y, 0 );
		glBindTexture( GL_TEXTURE_2D, pFont->m_Texture );
		glListBase( pFont->m_CharLists );
		glCallLists( strlen(strToRender), GL_BYTE, strToRender );
	glPopMatrix();
	m_TriangleCount += strlen(strToRender) * 2;
}
// End gfx_OpenGL::RenderStringAt()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderStringCenteredAt() //  Author: Logan "Burn" Jones
/////////////////////////////////////////             Date: 10/14/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere   - 
//  LPCTSTR strToRender    - 
//  BOOL bCenterHorizontal - 
//  BOOL bCenterVertical   - 
//
void gfx_OpenGL::RenderStringCenteredAt( std_Point_t& ptWhere, LPCTSTR strToRender, BOOL bCenterHorizontal, BOOL bCenterVertical )
{
	long			n, Len, TotalWidth, MaxHeight,H,H1,H2;
	const ogl_Font* pFont = static_cast<ogl_Font*>(m_ActiveFont);

	Len = strlen( strToRender );
	for( n=0,TotalWidth=0,MaxHeight=H1=H2=0; n<Len; ++n)
	{
		const FontEntry* pEntry = pFont->m_Entries + strToRender[n];

		TotalWidth += pEntry->Size.width;

		H =  pEntry->Size.height - (pEntry->Size.height - pEntry->Offset.y);
		if( H>H1 ) H1 = H;
		H =  pEntry->Size.height - pEntry->Offset.y;
		if( H>H2 ) H2 = H;
	}

	if( bCenterHorizontal )
		ptWhere.x -= TotalWidth / 2;

	if( bCenterVertical )
		//ptWhere.y += MaxHeight / 2;
		//ptWhere.y += (H1 + H2) / 2;
		ptWhere.y += pFont->m_Height / 2;

	// Call each display list corresponding to the characters given
    glPushMatrix();
		glTranslatef( ptWhere.x, ptWhere.y, 0 );
		glBindTexture( GL_TEXTURE_2D, pFont->m_Texture );
		glListBase( pFont->m_CharLists );
		glCallLists( strlen(strToRender), GL_BYTE, strToRender );
	glPopMatrix();
	m_TriangleCount += Len * 2;
}
// End gfx_OpenGL::RenderStringCenteredAt()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Font.cpp //
///////////////////////
