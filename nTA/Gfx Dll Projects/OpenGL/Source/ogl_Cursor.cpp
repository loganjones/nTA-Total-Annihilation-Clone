// ogl_Cursor.cpp //                       Author: Logan "Burn" Jones
///////////////////                                  Date: 10/27/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Cursor.h"

//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateCursor() //            Author: Logan "Burn" Jones
///////////////////////////////                       Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName       - 
//  DWORD dwNumFrames     - 
//  CursorFrame* pFrames  - 
//  gfx_Cursor** ppCursor - 
//  BOOL bSetAsCurrent    - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateCursor( LPCTSTR strName, DWORD dwNumFrames, CursorFrame* pFrames, gfx_Cursor** ppCursor, BOOL bSetAsCurrent )
{
	ogl_Cursor		NewCursor;
	std_Size_t		TexSize;
	BYTE			TempBuffer[ sqr(256) ];
	BYTE*			pImg;
	BYTE*			pTmp;

	strcpy( NewCursor.m_Name, strName );
	NewCursor.m_pTextures = new GLuint[ dwNumFrames ];
	glGenTextures( dwNumFrames, NewCursor.m_pTextures );
	NewCursor.m_ListBase = glGenLists( dwNumFrames );

// TODO: Combine all the cursor frames into one texture

	for( DWORD num=0; num<dwNumFrames; ++num)
	{
		TexSize.width = math_NextPowerOfTwo( (UINT32)pFrames[num].Image.Size.width );
		TexSize.height= math_NextPowerOfTwo( (UINT32)pFrames[num].Image.Size.height);

		ZeroMemory( TempBuffer, sqr(256) );
		pImg = pFrames[num].Image.pBytes;
		pTmp = TempBuffer;
		for( long y=0; y<pFrames[num].Image.Size.height; ++y)
		{
			memcpy( pTmp, pImg, pFrames[num].Image.Size.width );
			pImg += pFrames[num].Image.Pitch;
			pTmp += TexSize.width;
		}

		glBindTexture( GL_TEXTURE_2D, NewCursor.m_pTextures[num] );
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D( GL_TEXTURE_2D,
					  0,
					  GL_COLOR_INDEX8_EXT,
					  (GLsizei)TexSize.width,
					  (GLsizei)TexSize.height,
					  0,
					  GL_COLOR_INDEX,
					  GL_UNSIGNED_BYTE,
					  TempBuffer );

		glNewList( NewCursor.m_ListBase + num, GL_COMPILE );
			glBindTexture( GL_TEXTURE_2D, NewCursor.m_pTextures[num] );
			glBegin( GL_TRIANGLE_STRIP );
				glTexCoord2f( 0, 0 );
				glVertex2i( 0 - (GLint)pFrames[num].HotSpot.x, 0 - (GLint)pFrames[num].HotSpot.y );
				glTexCoord2f( 0 , float(pFrames[num].Image.Size.height) / TexSize.height );
				glVertex2i( 0 - (GLint)pFrames[num].HotSpot.x, (GLint)(pFrames[num].Image.Size.height - pFrames[num].HotSpot.y) );
				glTexCoord2f( float(pFrames[num].Image.Size.width) / TexSize.width, 0 );
				glVertex2i( (GLint)(pFrames[num].Image.Size.width - pFrames[num].HotSpot.x), 0 - (GLint)pFrames[num].HotSpot.y );
				glTexCoord2f( float(pFrames[num].Image.Size.width) / TexSize.width , float(pFrames[num].Image.Size.height) / TexSize.height );
				glVertex2i( (GLint)(pFrames[num].Image.Size.width - pFrames[num].HotSpot.x), (GLint)(pFrames[num].Image.Size.height - pFrames[num].HotSpot.y) );
			glEnd();
		glEndList();

	} // end for( dwNumFrames )


	// Set the rest of the cursor's data and add it to the list
	NewCursor.m_NumFrames = dwNumFrames;
	NewCursor.m_CurrentFrame = 0;
	NewCursor.m_pHost = &m_Cursors;
	m_Cursors.push_front( NewCursor );
	*ppCursor = &m_Cursors.front();
	m_Cursors.front().m_HostIt = m_Cursors.begin();

	// Set the new font as the current one if desired
	if( bSetAsCurrent )
		SetCurrentCursor( *ppCursor );

	// Return success
	return TRUE;
}
// End gfx_OpenGL::CreateCursor()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Cursor::ogl_Cursor()
{}
ogl_Cursor::~ogl_Cursor()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Cursor::Destroy() //                 Author: Logan "Burn" Jones
//////////////////////////                            Date: 10/27/2001
//               
//====================================================================
//
void ogl_Cursor::Destroy()
{
	if( m_pHost )
	{
		glDeleteLists( m_ListBase, m_NumFrames );
		glDeleteTextures( m_NumFrames, m_pTextures );
		SAFE_DELETE_ARRAY( m_pTextures );
		m_pHost->erase( m_HostIt );
		m_pHost = NULL;
	}
}
// End ogl_Cursor::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::SetCurrentCursor() //        Author: Logan "Burn" Jones
///////////////////////////////////                   Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  gfx_Cursor* pCursor - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::SetCurrentCursor( gfx_Cursor* pCursor )
{
	return (m_ActiveCursor = pCursor)!=NULL;
}
// End gfx_OpenGL::SetCurrentCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::SetCurrentCursor() //        Author: Logan "Burn" Jones
///////////////////////////////////                   Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::SetCurrentCursor( LPCTSTR strName )
{
	CursorList_t::iterator		it = m_Cursors.begin();
	CursorList_t::iterator		end = m_Cursors.end();

	for( ; it!=end; ++it )
	{
		if( stricmp((*it).m_Name,strName)==0 )
		{
			m_ActiveCursor = &(*it);
			return TRUE;
		}
	}

	// If we get here, we have failed
	return FALSE;
}
// End gfx_OpenGL::SetCurrentCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::UpdateCursor() //            Author: Logan "Burn" Jones
///////////////////////////////                       Date: 10/27/2001
//               
//====================================================================
//
void gfx_OpenGL::UpdateCursor()
{
	if( m_ActiveCursor )
	{
		ogl_Cursor* pCur = static_cast<ogl_Cursor*>(m_ActiveCursor);
		++pCur->m_CurrentFrame;
		if( pCur->m_CurrentFrame>=pCur->m_NumFrames )
			pCur->m_CurrentFrame = 0;
	}
}
// End gfx_OpenGL::UpdateCursor()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderCursor() //            Author: Logan "Burn" Jones
///////////////////////////////                       Date: 10/27/2001
//               
//====================================================================
// Parameters:
//  std_Vector2 vWhere - 
//
void gfx_OpenGL::RenderCursor( std_Vector2 vWhere )
{
	ogl_Cursor* pCur = static_cast<ogl_Cursor*>(m_ActiveCursor);

	glPushMatrix();
	glTranslatef( vWhere.x, vWhere.y, 0 );
	glCallList( pCur->m_ListBase + pCur->m_CurrentFrame );
	glPopMatrix();
	m_TriangleCount += 2;
}
// End gfx_OpenGL::RenderCursor()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Cursor.cpp //
/////////////////////////
