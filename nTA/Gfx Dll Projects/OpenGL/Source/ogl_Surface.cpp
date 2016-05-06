// ogl_Surface.cpp //                      Author: Logan "Burn" Jones
////////////////////                                 Date: 10/13/2001
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Surface.h"


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateSurface() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 10/9/2001
//               
//====================================================================
// Parameters:
//  GFX_PIXEL_FORMAT pxFormat - 
//  gfx_Image_t* pImage       - 
//  gfx_Surface** ppSurface   - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateSurface( GFX_PIXEL_FORMAT pxFormat, gfx_Image_t* pImage, gfx_Surface** ppSurface )
{
    GLuint          GLTexInternalFormat;
    GLuint          GLTexFormat;
	std_Size_t		TextureSize;
    BYTE*           pTempTexture;
    BYTE*           pTemp;
    BYTE*           pTile;
	ogl_Surface		NewSurface;

    // Sanity checks
    assert( pImage && (pImage->Size.width>0) && (pImage->Size.height>0 ) && ppSurface );

	// Check the pixel format
	switch( pxFormat )
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
			ErrorComment( "ERROR: Invalid surface creation type, %d.", pxFormat );
			assertEx( 0, "Invalid surface creation type." );
			return FALSE;

    } // end switch( pxFormat )

	// Calculate the required texture size
	TextureSize.Set( math_NextPowerOfTwo(pImage->Size.width), math_NextPowerOfTwo(pImage->Size.height) );
	NewSurface.m_ImageSize = pImage->Size;

    // Check to see if the surface needs to be chopped up
    if( TextureSize <= m_Caps.MaxTextureSize )
    {
        // Check to see if a temporary texture buffer needs to be created
        if( pImage->Size==TextureSize )
            pTempTexture = pImage->pBytes;
        else
        {
            // Create the buffer and fill it with the image
            pTempTexture = new BYTE[ *TextureSize * pImage->Stride ];
			if( pTempTexture==NULL ) return FALSE;
            pTemp = pTempTexture;
			BYTE* pImg = pImage->pBytes;
            for( int count=pImage->Size.height; count>0; --count)
            {
                memcpy( pTemp, pImg, pImage->Size.width * pImage->Stride );
			    pTemp += (TextureSize.width * pImage->Stride);
			    pImg += (pImage->Size.width * pImage->Stride);
            }
        }

        // Create the texture
        glGenTextures( 1, &NewSurface.m_Textures[0] );
        glBindTexture( GL_TEXTURE_2D, NewSurface.m_Textures[0] );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GLTexInternalFormat,
            TextureSize.width,
            TextureSize.height,
            0,
            GLTexFormat,
            GL_UNSIGNED_BYTE,
            pTempTexture );

        // Delete the temporary texture buffer if we have to
        if( pImage->Size!=TextureSize )
            delete [] pTempTexture;

        // Create the display list
		NewSurface.m_Ratio.Set( float(pImage->Size.width)/TextureSize.width, float(pImage->Size.height)/TextureSize.height );
        NewSurface.m_DisplayList = glGenLists(1);
		glNewList( NewSurface.m_DisplayList, GL_COMPILE );
			glBindTexture( GL_TEXTURE_2D, NewSurface.m_Textures[0] );
			glBegin( GL_TRIANGLE_STRIP );
				glTexCoord2f(0,0);
				glVertex2i( 0, 0 );
				glTexCoord2f( 0, NewSurface.m_Ratio.height );
				glVertex2i( 0, pImage->Size.height );
				glTexCoord2f( NewSurface.m_Ratio.width, 0 );
				glVertex2i( pImage->Size.width, 0 );
				glTexCoord2f( NewSurface.m_Ratio.width, NewSurface.m_Ratio.height );
				glVertex2i( pImage->Size.width, pImage->Size.height );
			glEnd();
		glEndList();

		NewSurface.m_Tiles.Set( 1, 1 );
		NewSurface.m_pHost = &m_Surfaces;
		m_Surfaces.push_front( NewSurface );
		*ppSurface = &m_Surfaces.front();
		m_Surfaces.front().m_HostIt = m_Surfaces.begin();

        // Return success
        return TRUE;

    } // end if( TextureSize <= MaxTextureSize )

    // Create a temporary texture buffer
    pTempTexture = new BYTE[ *m_Caps.MaxTextureSize * pImage->Stride ];

    // Calculate the number of texture tiles needed
	NewSurface.m_Tiles.width = (pImage->Size.width / m_Caps.MaxTextureSize.width) + ( ((pImage->Size.width % m_Caps.MaxTextureSize.width)==0) ? 0:1 );
	NewSurface.m_Tiles.height = (pImage->Size.height / m_Caps.MaxTextureSize.height) + ( ((pImage->Size.height % m_Caps.MaxTextureSize.height)==0) ? 0:1 );

    // Generate the tiles
    glGenTextures( *NewSurface.m_Tiles, NewSurface.m_Textures );

    // We'll need these
    WORD		Width, Height;
	WORD		WidthOnEdge, HeightOnEdge;
	DWORD		TexWidthOnEdge, TexHeightOnEdge;
	WidthOnEdge = pImage->Size.width -((NewSurface.m_Tiles.width-1)*m_Caps.MaxTextureSize.width );
	HeightOnEdge= pImage->Size.height-((NewSurface.m_Tiles.height-1)*m_Caps.MaxTextureSize.height);
	TexWidthOnEdge = math_NextPowerOfTwo( WidthOnEdge );
	TexHeightOnEdge= math_NextPowerOfTwo( HeightOnEdge);

    // Iterate through each column
    for( int y=0,z=0; y<NewSurface.m_Tiles.height; y++)
	{
        // Is this tile on the bottom row
		if( (y+1)==NewSurface.m_Tiles.height )
		{
            // Yes it is, use the edge height values
			Height = HeightOnEdge;
			TextureSize.height = TexHeightOnEdge;
		}
		else
		{
            // No it's not, use the max height values
			Height = m_Caps.MaxTextureSize.height;
			TextureSize.height = m_Caps.MaxTextureSize.height;
		}

        // Iterate through the tiles on this row
		for( int x=0; x<NewSurface.m_Tiles.width; x++,++z)
		{
            // Is this tile on the right edge
			if( (x+1)==NewSurface.m_Tiles.width )
			{
                // Yes it is, use the edge width values
				Width = WidthOnEdge;
				TextureSize.width = TexWidthOnEdge;
			}
			else
			{
                // No it's not, use the max width values
				Width = m_Caps.MaxTextureSize.width;
				TextureSize.width = m_Caps.MaxTextureSize.width;
			}

			pTile = pImage->pBytes + ( (y*m_Caps.MaxTextureSize.height*(pImage->Size.width*pImage->Stride)) + (x*m_Caps.MaxTextureSize.width*pImage->Stride) );
			pTemp = pTempTexture;
			for( int n=Height; n>0; n--)
			{
				memcpy( pTemp, pTile, Width*pImage->Stride);
				pTemp += (TextureSize.width*pImage->Stride);
				pTile += (pImage->Size.width*pImage->Stride);
			}
			
            // Create the texture
			glBindTexture( GL_TEXTURE_2D, NewSurface.m_Textures[ z ] );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GLTexInternalFormat,
                TextureSize.width,
                TextureSize.height,
                0,
                GLTexFormat,
                GL_UNSIGNED_BYTE,
                pTempTexture );
			
        } // end for( Number of tiles in this row )

    } // end for( Number of tile rows )

    // Create the display list
    NewSurface.m_DisplayList = glGenLists(1);
	glNewList( NewSurface.m_DisplayList, GL_COMPILE );

    // Iterate through each column, again
	for( int y=0,z=0; y<NewSurface.m_Tiles.height; y++)
	{
		// Is this tile on the bottom row
		if( (y+1)==NewSurface.m_Tiles.height )
		{
            // Yes it is, use the edge height values
			Height = HeightOnEdge;
			TextureSize.height = TexHeightOnEdge;
		}
		else
		{
            // No it's not, use the max height values
			Height = m_Caps.MaxTextureSize.height;
			TextureSize.height = m_Caps.MaxTextureSize.height;
		}

        // Iterate through the tiles on this row, again
		for( int x=0; x<NewSurface.m_Tiles.width; x++,++z)
		{
			// Is this tile on the right edge
			if( (x+1)==NewSurface.m_Tiles.width )
			{
                // Yes it is, use the edge width values
				Width = WidthOnEdge;
				TextureSize.width = TexWidthOnEdge;
			}
			else
			{
                // No it's not, use the max width values
				Width = m_Caps.MaxTextureSize.width;
				TextureSize.width = m_Caps.MaxTextureSize.width;
			}

            // Draw this tile
			glBindTexture( GL_TEXTURE_2D, NewSurface.m_Textures[ z ] );
			glBegin( GL_TRIANGLE_STRIP );
				glTexCoord2f(0,0);
				glVertex2i( (x*m_Caps.MaxTextureSize.width), (y*m_Caps.MaxTextureSize.height) );
				glTexCoord2f(0,GLfloat(Height)/TextureSize.height);
				glVertex2i( (x*m_Caps.MaxTextureSize.width), (y*m_Caps.MaxTextureSize.height)+Height );
				glTexCoord2f(GLfloat(Width)/TextureSize.width,0);
				glVertex2i( (x*m_Caps.MaxTextureSize.width)+Width, (y*m_Caps.MaxTextureSize.height) );
				glTexCoord2f(GLfloat(Width)/TextureSize.width,GLfloat(Height)/TextureSize.height);
				glVertex2i( (x*m_Caps.MaxTextureSize.width)+Width, (y*m_Caps.MaxTextureSize.height)+Height );
			glEnd();

		} // end for( Number of tiles in this row )

	} // end for( Number of tile rows )

    // Finish compiling the list
	glEndList();

    // Delete the temporary texture buffer
    delete [] pTempTexture;

	NewSurface.m_Ratio.Set(1,1);
	NewSurface.m_pHost = &m_Surfaces;
	m_Surfaces.push_front( NewSurface );
	*ppSurface = &m_Surfaces.front();
	m_Surfaces.front().m_HostIt = m_Surfaces.begin();

    // Return success
    return TRUE;
}
// End gfx_OpenGL::CreateSurface()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Surface::ogl_Surface():
	m_DisplayList( 0 )
{
	ZeroMemory( m_Textures, sizeof(m_Textures) );
}
ogl_Surface::~ogl_Surface()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::Destroy() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/13/2001
//               
//====================================================================
//
void ogl_Surface::Destroy()
{
}
// End ogl_Surface::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::GetSize() //                Author: Logan "Burn" Jones
///////////////////////////                           Date: 10/13/2001
//               
//====================================================================
// Return: const std_Size_t& - 
//
const std_Size_t& ogl_Surface::GetSize() const
{
	return m_ImageSize;
}
// End ogl_Surface::GetSize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderSurface() //           Author: Logan "Burn" Jones
////////////////////////////////                      Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  gfx_Surface* pSurface - 
//
void gfx_OpenGL::RenderSurface( gfx_Surface* pSurface )
{
	const ogl_Surface* Surface = static_cast<ogl_Surface*>(pSurface);

	glCallList( Surface->m_DisplayList );
	m_TriangleCount += *Surface->m_Tiles * 2;
}
// End gfx_OpenGL::RenderSurface()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::RenderSurfaceEx() //         Author: Logan "Burn" Jones
//////////////////////////////////                    Date: 10/12/2001
//               
//====================================================================
// Parameters:
//  std_Point_t& ptWhere  - 
//  gfx_Surface* pSurface - 
//
void gfx_OpenGL::RenderSurfaceEx( const std_Point_t& ptWhere, gfx_Surface* pSurface )
{
	const ogl_Surface* Surface = static_cast<ogl_Surface*>(pSurface);

	glPushMatrix();
	glTranslatef( ptWhere.x, ptWhere.y, 0 );
	glCallList( Surface->m_DisplayList  );
	glPopMatrix();
	m_TriangleCount += *Surface->m_Tiles * 2;
}
// End gfx_OpenGL::RenderSurfaceEx()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
//
void ogl_Surface::Render()
{
	glCallList( m_DisplayList );
	static_cast<gfx_OpenGL*>(pGfxSystem)->m_TriangleCount += *m_Tiles * 2;
}
// End ogl_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPos - 
//
void ogl_Surface::Render( std_Vector3 vPos )
{
	glPushMatrix();
	glTranslatef( vPos.x, vPos.y, vPos.z );
	glCallList( m_DisplayList  );
	glPopMatrix();
	static_cast<gfx_OpenGL*>(pGfxSystem)->m_TriangleCount += *m_Tiles * 2;
}
// End ogl_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_Vector3 vPos - 
//  std_SizeF sz     - 
//
void ogl_Surface::Render( std_Vector3 vPos, std_SizeF sz )
{
	glBindTexture( GL_TEXTURE_2D, m_Textures[0] );
	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f(0,0);
		glVertex3f( vPos.x, vPos.y, vPos.z );
		glTexCoord2f( 0, m_Ratio.height );
		glVertex3f( vPos.x, vPos.y + sz.height, vPos.z );
		glTexCoord2f( m_Ratio.width, 0 );
		glVertex3f( vPos.x + sz.width, vPos.y, vPos.z);
		glTexCoord2f( m_Ratio.width, m_Ratio.height );
		glVertex3f( vPos.x + sz.width, vPos.y + sz.height, vPos.z );
	glEnd();
	static_cast<gfx_OpenGL*>(pGfxSystem)->m_TriangleCount += 2;
}
// End ogl_Surface::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Surface::Render() //                 Author: Logan "Burn" Jones
//////////////////////////                             Date: 5/15/2002
//               
//====================================================================
// Parameters:
//  std_RectF rctPos - 
//
void ogl_Surface::Render( std_RectF rctPos )
{
	glBindTexture( GL_TEXTURE_2D, m_Textures[0] );
	glBegin( GL_TRIANGLE_STRIP );
		glTexCoord2f(0,0);
		glVertex2f( rctPos.left, rctPos.top );
		glTexCoord2f( 0, m_Ratio.height );
		glVertex2f( rctPos.left, rctPos.bottom );
		glTexCoord2f( m_Ratio.width, 0 );
		glVertex2f( rctPos.right, rctPos.top );
		glTexCoord2f( m_Ratio.width, m_Ratio.height );
		glVertex2f( rctPos.right, rctPos.bottom );
	glEnd();
	static_cast<gfx_OpenGL*>(pGfxSystem)->m_TriangleCount += 2;
}
// End ogl_Surface::Render()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Surface.cpp //
//////////////////////////
