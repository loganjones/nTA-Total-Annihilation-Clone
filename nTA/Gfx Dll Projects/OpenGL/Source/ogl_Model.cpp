// ogl_Model.cpp //                        Author: Logan "Burn" Jones
//////////////////                                    Date: 3/22/2002
//
/////////////////////////////////////////////////////////////////////

#include "ogl.h"
#include "ogl_Model.h"
#include "ogl_ModelInstance.h"
#include "../../../Source/game_Constants.h"
#include "../../../Source/ta_3DO.h"
#include "../../../Source/ta_GAF.h"
#include "../../../Source/util_RectFiller.h"


//static GLfloat M[16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1};
//static GLfloat m_PerspectiveMatrix[16] = 
//{	-1,  0,0,0,
//	 0,  1,0,0,
//	 0,-.5,1,0,
//	 0,  0,0,1 };
//static GLfloat m_ShadowMatrix[16] = 
//{	1,0,0,0,
//	0,1,0,0,
//	0,0,0,0,
//	4,0,0,1 };


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
ogl_Model::ogl_Model():
	m_PieceCount( 0 ),
	m_PieceNameBuffer( NULL ),
	m_Pieces( NULL ),
	m_Texture( NULL )
{}
ogl_Model::~ogl_Model()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_OpenGL::CreateModel() //             Author: Logan "Burn" Jones
//////////////////////////////                         Date: 3/22/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer   - 
//  gfx_Model** ppModel - 
//
// Return: BOOL - 
//
BOOL gfx_OpenGL::CreateModel( BYTE* pFileBuffer, gfx_Model** ppModel )
{
	ogl_Model*		pModel;

	// Allocate the new model
	pModel = new ogl_Model;

	// Create the model
	if( bFAILED(pModel->Create(pFileBuffer, glColorTableEXT == NULL)) )
	{
		delete pModel;
		return FALSE;
	}

	// Set the return pointer and return successfully
	*ppModel = pModel;
	return TRUE;
}
// End gfx_OpenGL::CreateModel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::Create() //                   Author: Logan "Burn" Jones
////////////////////////                               Date: 3/23/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer - 
//
// Return: BOOL - 
//
BOOL ogl_Model::Create( BYTE* pFileBuffer, BOOL NeedsPaletteConversion )
{
	DWORD				NameBufferSize = 0;
	std_Vertex3			GroundPlate[4];
	DWORD				PieceIndex = 0;
	UsedTextureList_t	Textures;

	// Sanity checks
	if( pFileBuffer==NULL )
		return FALSE;

	// Just checking
	Destroy();

	// Scan the model structure
	ScanModel( pFileBuffer, 0, NameBufferSize, GroundPlate, Textures );

	// Allocate the piece name buffer
	m_PieceNameBuffer = new char[NameBufferSize];
	NameBufferSize = 0;

	// Allocate the piece array
	m_Pieces = new Piece_t[ m_PieceCount ];

	m_NeedsPaletteConversion = NeedsPaletteConversion;

	// Assemble the texture
	if( bFAILED(AssembleTexture( Textures )) )
    {
		Destroy();
        return FALSE;
    }

	// Load the model
	LoadModel( pFileBuffer, 0, NameBufferSize, PieceIndex, glGenLists(m_PieceCount+1), Textures );

	// Create the ground plate display list
	glNewList( m_Pieces->DisplayList + m_PieceCount + 1, GL_COMPILE );
	glBegin( GL_LINE_LOOP );
	for( int n=0; n<4; ++n)
		glVertex3f( GroundPlate[n].x, GroundPlate[n].y, GroundPlate[n].z );
	glEnd();
	glEndList();

	return TRUE;
}
// End ogl_Model::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::GetPieceCount() //            Author: Logan "Burn" Jones
///////////////////////////////                        Date: 4/17/2002
//               
//====================================================================
// Return: const DWORD - 
//
const DWORD ogl_Model::GetPieceCount() const
{
	return m_PieceCount;
}
// End ogl_Model::GetPieceCount()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::GetPieceID() //               Author: Logan "Burn" Jones
////////////////////////////                           Date: 3/25/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strPiece - 
//
// Return: long - 
//
long ogl_Model::GetPieceID( LPCTSTR strPiece ) const
{
	PieceNameMap_t::const_iterator it = m_PieceNameMap.find( strPiece );
	if( it==m_PieceNameMap.end() ) return -1;
	else return (*it).second;
}
// End ogl_Model::GetPieceID()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_Model::NewInstance() //             Author: Logan "Burn" Jones
/////////////////////////////                        Date: 02-23-2003
//
//===================================================================
// Parameters:
//  class gfx_ModelInstance** ppInstance - 
//
void ogl_Model::NewInstance( class gfx_ModelInstance** ppInstance )
{
	ogl_ModelInstance*	pInst = new ogl_ModelInstance;
	*ppInstance = pInst;
	m_ModelInstances.push_front( pInst );
	pInst->Create( this, m_ModelInstances.begin() );
}
// End ogl_Model::NewInstance()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// ogl_Model::ReleaseInstance() //         Author: Logan "Burn" Jones
/////////////////////////////////                    Date: 02-23-2003
//
//===================================================================
// Parameters:
//  ogl_ModelInstance* pInstance - 
//  model_HInstance hInstance    - 
//
void ogl_Model::ReleaseInstance( ogl_ModelInstance* pInstance, model_HInstance hInstance )
{
	m_ModelInstances.erase( hInstance );
	delete pInstance;
}
// End ogl_Model::ReleaseInstance()
/////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// ogl_Model::CreateInstance() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 3/25/2002
//               
//====================================================================
// Parameters:
//  const LPVOID pCreator - 
//
void ogl_Model::CreateInstance( const LPVOID pCreator )
{
	Instance_t* pInstance = &m_Instances[ pCreator ];

	pInstance->Metrics = new PieceMetric_t[ m_PieceCount ];
	for( DWORD n=0; n<m_PieceCount; ++n) {
		pInstance->Metrics[n].Turn_cos.Set(1,1,1);
		pInstance->Metrics[n].Flags = 0;
	}
}
// End ogl_Model::CreateInstance()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::GetMetrics() //               Author: Logan "Burn" Jones
////////////////////////////                           Date: 6/11/2002
//               
//====================================================================
// Parameters:
//  const LPVOID pModelInstance - 
//
// Return: gfx_Model::PieceMetric_t* - 
//
gfx_Model::PieceMetric_t* ogl_Model::GetMetrics( const LPVOID pModelInstance )
{
	return m_Instances[ pModelInstance ].Metrics;
}
// End ogl_Model::GetMetrics()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// ogl_Model::Destroy() //                  Author: Logan "Burn" Jones
/////////////////////////                              Date: 3/22/2002
//               
//====================================================================
//
void ogl_Model::Destroy()
{
	if( m_Texture )
	{
		glDeleteTextures( 1, &m_Texture );
		m_Texture = 0;
	}
	if( m_Pieces ) glDeleteLists( m_Pieces->DisplayList, m_PieceCount+1 );
	SAFE_DELETE( m_PieceNameBuffer );
	SAFE_DELETE( m_Pieces );
	m_PieceCount = 0;
}
// End ogl_Model::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::SynchronizeVertices() //      Author: Logan "Burn" Jones
/////////////////////////////////////                  Date: 6/11/2002
//               
//====================================================================
// Parameters:
//  const LPVOID pModelInstance - 
//
void ogl_Model::SynchronizeVertices( const LPVOID pModelInstance )
{
}
// End ogl_Model::SynchronizeVertices()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::SynchronizeStates() //        Author: Logan "Burn" Jones
///////////////////////////////////                    Date: 6/11/2002
//               
//====================================================================
// Parameters:
//  const LPVOID pModelInstance - 
//
void ogl_Model::SynchronizeStates( const LPVOID pModelInstance )
{
}
// End ogl_Model::SynchronizeStates()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// ogl_Model::Render() //                   Author: Logan "Burn" Jones
////////////////////////                               Date: 3/22/2002
//               
//====================================================================
// Parameters:
//  const PieceMetric_t* pMetrics - 
//  std_Vector3 vPosition         - 
//  std_Vector3 vOrientation      - 
//  float fWaterLevel             - 
//  DWORD dwFlags                 - 
//
void ogl_Model::Render( const PieceMetric_t* pMetrics, std_Vector3 vPosition, std_Vector3 vOrientation, float fShadowOffset, float fWaterLevel, DWORD dwFlags, const LPVOID pCreator )
{
	// Convert the Orientation to degrees
	vOrientation *= 180.0f / fPI;

	// We need the z buffer to render the model correctly
	glEnable( GL_DEPTH_TEST );

	// Set the texture
	glBindTexture( GL_TEXTURE_2D, m_Texture );

	// Move to the desired poition
	glPushMatrix();
	glTranslatef( vPosition.x, vPosition.y, vPosition.z );
	{
		// Draw the model normally
		glPushMatrix();
		{
			// Rotate to the unit's perspective and its direction
			glMultMatrixf( m_PerspectiveMatrix );
			glRotatef( -vOrientation.z, 0,0,1 );

			// Do we have to worry about water coloring
			if( dwFlags & InWater )
			{
				double Plane[] = {0,0,1,-fWaterLevel};
				glEnable( GL_CLIP_PLANE0 );

				// Draw the part of the model that is above water
				glClipPlane( GL_CLIP_PLANE0, Plane );
				glPushMatrix();
				glRotatef( vOrientation.x, 1,0,0 );
				glRotatef( vOrientation.y, 0,1,0 );
				RenderPiece( m_Pieces, pMetrics, PMF_HIDE );

				// Draw the part of the model that is below water and
				// modulate the texture color with a watery color
				if( dwFlags & SubmergedVisible )
				{
					glPopMatrix();
					glPushMatrix();
					Plane[2] = -Plane[2]; Plane[3] = -Plane[3];
					glClipPlane( GL_CLIP_PLANE0, Plane );
					glRotatef( vOrientation.x, 1,0,0 );
					glRotatef( vOrientation.y, 0,1,0 );
					glColor3ub(0,0,200);
					RenderPiece( m_Pieces, pMetrics, PMF_HIDE );
				}

				glDisable( GL_CLIP_PLANE0 );
			}
			else // Not in water, just render
			{
				glPushMatrix();
				glRotatef( vOrientation.x, 1,0,0 );
				glRotatef( vOrientation.y, 0,1,0 );

				// Render the pieces
				RenderPiece( m_Pieces, pMetrics, PMF_HIDE );
			}

			// Draw the ground plate
			glDisable( GL_TEXTURE_2D );
			glColor3f(1,1,0);
			if( dwFlags & Selected ) glCallList( m_Pieces->DisplayList + m_PieceCount + 1 );
			glPopMatrix();
		}
		glPopMatrix();

		// Draw the model using the shadow matrix and without the texture
		glColor4ub( 0,0,0, 100 );
		{
			// Insert the shadow matrix
			glMultMatrixf( m_ShadowMatrix );

			// Rotate to the unit's perspective and its orientation (again)
			glMultMatrixf( m_PerspectiveMatrix );
			glRotatef( -vOrientation.z, 0,0,1 );
			glRotatef( vOrientation.x, 1,0,0 );
			glRotatef( vOrientation.y, 0,1,0 );

			// Render the pieces (again)
			RenderPiece( m_Pieces, pMetrics, PMF_HIDE | PMF_DONT_SHADE );
		}
		glColor4f(1,1,1,1);

	}
	glPopMatrix();

	// Reset some stuff we changed
	glDisable( GL_DEPTH_TEST );
	glEnable( GL_TEXTURE_2D );
}
// End ogl_Model::Render()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::RenderPiece() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 3/23/2002
//               
//====================================================================
// Parameters:
//  Piece_t* pPiece              - 
//  const PieceMetric_t* pMetric - 
//  DWORD dwSkipFlags            - 
//
void ogl_Model::RenderPiece( Piece_t* pPiece, const PieceMetric_t* pMetric, DWORD dwSkipFlags ) const
{
	// Iterate through each piece
	while( pPiece!=NULL )
	{
		if( dwSkipFlags & pMetric[pPiece->Index].Flags )
		{
			if( pPiece->pChild ) RenderPiece( pPiece->pChild, pMetric, dwSkipFlags );
		}
		else
		{
			glPushMatrix();
	//		glTranslatef( pPiece->Origin.x + pMetric[pPiece->Index].Move.x, pPiece->Origin.z + pMetric[pPiece->Index].Move.z, pPiece->Origin.y + pMetric[pPiece->Index].Move.y );
	//		glRotatef( pMetric[pPiece->Index].Turn.x, 1,0,0 );
	//		glRotatef( pMetric[pPiece->Index].Turn.z, 0,1,0 );
	//		glRotatef( pMetric[pPiece->Index].Turn.y, 0,0,1 );

			// Translate and rotate
			const std_Vector3& Origin=pPiece->Origin, &Offset=pMetric[pPiece->Index].Move;
			const std_Vector3& sin = pMetric[pPiece->Index].Turn_sin;
			const std_Vector3& cos = pMetric[pPiece->Index].Turn_cos;
			M[12]=Origin.x-Offset.x; M[13]=Origin.z-Offset.z; M[14]=Origin.y+Offset.y;
			M[0]=cos.y*cos.z; M[1]=(sin.y*cos.x)+(sin.x*cos.y*sin.z); M[2]=(sin.x*sin.y)-(cos.x*cos.y*sin.z);
			M[4]=-sin.y*cos.z; M[5]=(cos.x*cos.y)-(sin.x*sin.y*sin.z); M[6]=(sin.x*cos.y)+(cos.x*sin.y*sin.z);
			M[8]=sin.z; M[9]=-sin.x*cos.z; M[10]=cos.x*cos.z;
			glMultMatrixf( M );

			if( pPiece->Primitives )
			{
				glCallList( pPiece->DisplayList );
				((gfx_OpenGL*)pGfxSystem)->m_TriangleCount += pPiece->Primitives;
			}
			if( pPiece->pChild ) RenderPiece( pPiece->pChild, pMetric, dwSkipFlags );
			glPopMatrix();
		}
		pPiece = pPiece->pSibling;
	} // end while( pPiece )
}
// End ogl_Model::RenderPiece()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::GetPieceOrigin() //           Author: Logan "Burn" Jones
////////////////////////////////                       Date: 5/14/2002
//               
//====================================================================
// Parameters:
//  const DWORD dwPiece           - 
//  const PieceMetric_t* pMetrics - 
//  std_Vector3 vOrientation      - 
//  std_Vector3& vOrigin          - 
//
void ogl_Model::GetPieceOrigin( const DWORD dwPiece, const PieceMetric_t* pMetrics, std_Vector3 vOrientation, std_Vector3& vOrigin ) const
{
	vOrigin.Nullify();
}
// End ogl_Model::GetPieceOrigin()
//////////////////////////////////////////////////////////////////////
*/

//////////////////////////////////////////////////////////////////////
// ogl_Model::ScanModel() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 3/22/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer           - 
//  DWORD dwOffset              - 
//  DWORD& dwNameBufferSize     - 
//  std_Vertex3* pGoundPlate    - 
//  UsedTextureList_t& Textures - 
//
void ogl_Model::ScanModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwNameBufferSize, std_Vertex3* pGoundPlate, UsedTextureList_t& Textures )
{
	// Retrieve the object structure
	TA_3DO_OBJECT* pObject = (TA_3DO_OBJECT*)(pFileBuffer + dwOffset);

	// Count this piece
	m_PieceCount++;

	// Iterate through each sibling
	while( pObject!=NULL )
	{
		// Add the name size to the total name size
		if(pObject->OffsetToObjectName)
			dwNameBufferSize += strlen( (LPTSTR)(pFileBuffer + pObject->OffsetToObjectName) ) + 1;

		// Scan each primitive
		TA_3DO_PRIMITIVE* pPrimitive = (TA_3DO_PRIMITIVE*)(pFileBuffer + pObject->OffsetToPrimitiveArray);
		for( DWORD count=0; count<pObject->NumberOfPrimitives; ++count,++pPrimitive)
		{
			// Check if this is the ground plate
			if( ((INT32)count)==pObject->GroundPlateIndex )
			{
				// TEMP
				//assert( pPrimitive->NumberOfVertexIndexes==4 );
				if( pPrimitive->NumberOfVertexIndexes!=4 ) continue;
				LPTA_3DO_VERTEX pVertecies = (LPTA_3DO_VERTEX)(pFileBuffer + pObject->OffsetToVertexArray);
				WORD* IndexArray = (WORD*)(pFileBuffer + pPrimitive->OffsetToVertexIndexArray);
				for( int n=3; n>=0; --n)
					pGoundPlate[n].Set( (pVertecies + IndexArray[n])->x / LINEAR_CONSTANT,
										(pVertecies + IndexArray[n])->z / LINEAR_CONSTANT,
										(pVertecies + IndexArray[n])->y / LINEAR_CONSTANT );
				continue;
			}

			// No points, lines or anything above pentagons please
			if( (pPrimitive->NumberOfVertexIndexes>5)||(pPrimitive->NumberOfVertexIndexes<3) )
				continue;

			// Mark texture
			if( pPrimitive->OffsetToTextureName )
				MarkTexture( (LPTSTR)(pFileBuffer + pPrimitive->OffsetToTextureName), 0, Textures, (LPTSTR)(pFileBuffer + pObject->OffsetToObjectName) );
			else MarkTexture( NULL, pPrimitive->Color, Textures );

		} // end for( Primitives )

		// If there is a child, scan it
		if( pObject->OffsetToChildObject!=0 )
			ScanModel( pFileBuffer, pObject->OffsetToChildObject, dwNameBufferSize, pGoundPlate, Textures );

		// If there is a sibling, procceed to scan it.
		if( pObject->OffsetToSiblingObject!=0 )
		{
			pObject = (TA_3DO_OBJECT*)(pFileBuffer + pObject->OffsetToSiblingObject);
			m_PieceCount++;
		}
		else break;

	} // end while( pObject )
}
// End ogl_Model::ScanModel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::MarkTexture() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 3/23/2002
//               
//====================================================================
// Parameters:
//  LPTSTR strTexture           - 
//  DWORD dwColor               - 
//  UsedTextureList_t& Textures - 
//
void ogl_Model::MarkTexture( LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures, LPTSTR strPiece )
{
	UsedTexture_t						NewTex;
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();

	if( strTexture && (dwColor = 255) && ((gfx_OpenGL*)pGfxSystem)->m_ModelTextures.find(strTexture)!=((gfx_OpenGL*)pGfxSystem)->m_ModelTextures.end() )
	{
		// Make sure its not already used
		for(; it!=end; ++it)
			if( (*it).Texture && stricmp(strTexture,(*it).Texture)==0 )
				return;

		LPTA_GAF_ENTRY pEntry = (LPTA_GAF_ENTRY)((gfx_OpenGL*)pGfxSystem)->m_ModelTextures[strTexture];
		BYTE* pFile = (BYTE*)(size_t)pEntry->UnKnown_2;
		LPTA_GAF_FRAME_DATA pFrame = (LPTA_GAF_FRAME_DATA)(pFile + ((LPTA_GAF_FRAME_ENTRY)( ((BYTE*)pEntry) + sizeof(TA_GAF_ENTRY) ))->OffsetToFrameData);
		NewTex.Texture = strTexture;
		NewTex.Image.Stride = 1;
		switch( pEntry->NumberOfFrames )
		{
			case 0: MarkTexture(NULL,255,Textures,strPiece); return;
			case 10: // Side color texture
			//	((gfx_OpenGL*)pGfxSystem)->LoadComment( "Piece \'%s\' uses side color texture \'%s\'(%d,%d)' on primitive", strPiece, strTexture, pFrame->Width, pFrame->Height );
			//	break;
			case 1: // Normal Texture
			default: // TEMP
				NewTex.Color = 0;
				NewTex.Image.Size.Set( pFrame->Width, pFrame->Height );
				NewTex.Image.Pitch = pFrame->Width;
				NewTex.Image.pBytes = new BYTE[ *NewTex.Image.Size ];
				ta_GAF_GetPalettedFrameBits( pFile, pFrame, NewTex.Image.pBytes );
				break;

		/*	default: // Animated texture
				NewTex.Color = 2;
				NewTex.Image.Size.Set( pFrame->Width, pFrame->Height * pEntry->NumberOfFrames );
				NewTex.Image.Pitch = pFrame->Width;
				NewTex.Image.pBytes = new BYTE[ *NewTex.Image.Size ];
				for( i=0; i<pEntry->NumberOfFrames; ++i,++pFrame)
				{
					pFrame = (LPTA_GAF_FRAME_DATA)(pFile + ((LPTA_GAF_FRAME_ENTRY)( ((BYTE*)pEntry) + sizeof(TA_GAF_ENTRY) ))[i].OffsetToFrameData);
					ta_GAF_GetPalettedFrameBits( pFile, pFrame, NewTex.Image.pBytes + (i * pFrame->Width * pFrame->Height) );
				}
				break;*/
		} // end switch( NumberOfFrames )
	}
	else // Its a color
	{
		// NOTE: This is a little hack to keep pieces that were intended to be black from being clear
		if( dwColor==0 ) dwColor = 245;

		// Make sure its not already used
		for(; it!=end; ++it)
			if( (*it).Texture==NULL && (*it).Color==dwColor )
				return;

		NewTex.Texture = NULL;
		NewTex.Color = dwColor;
		NewTex.Image.Size.Set(8,8);
		NewTex.Image.Stride = 1;
		NewTex.Image.Pitch = NewTex.Image.Size.width;
		NewTex.Image.pBytes = new BYTE[ *NewTex.Image.Size ];
		memset( NewTex.Image.pBytes, NewTex.Color, *NewTex.Image.Size );
	}

	for( it = Textures.begin(); it!=end && (*NewTex.Image.Size)<(*(*it).Image.Size); ++it);
	Textures.insert( it, NewTex );
}
// End ogl_Model::MarkTexture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::AssembleTexture() //          Author: Logan "Burn" Jones
/////////////////////////////////                      Date: 3/23/2002
//               
//====================================================================
// Parameters:
//  UsedTextureList_t& Textures - 
//
// Return: BOOL - 
//
BOOL ogl_Model::AssembleTexture( UsedTextureList_t& Textures )
{
	const std_Size						MaxTextureSize(256,256);
	std_Size							TextureSize( MaxTextureSize );
	DWORD								CurrentSize = 0;
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();
	UsedTexture_t						SubTex;
	gfx_Image_t							Reduced;
	util_RectFiller						Filler;
	gfx_Image_t*						pImage;
	std_Rect							Destination;

	const BOOL		NeedsPaletteConversion = m_NeedsPaletteConversion;
	const GLuint    GLTexInternalFormat = NeedsPaletteConversion ? GL_RGBA : GL_COLOR_INDEX8_EXT;
	const GLuint    GLTexFormat = NeedsPaletteConversion ? GL_RGBA : GL_COLOR_INDEX;
	gfx_OpenGL::LPPALETTE pPalette = NULL;

	if (NeedsPaletteConversion)
	{
		pGfxSystem->GetCurrentPalette(&pPalette);
	}

	// Calculate the current size
	for(; it!=end; ++it) CurrentSize += *(*it).Image.Size;

	// If all the textures can't fit we have to scale some down
	while( CurrentSize>(*MaxTextureSize) )
	{
		// Remove the largest
		SubTex = Textures.front();
		Textures.pop_front();
		CurrentSize -= *SubTex.Image.Size;

		// Reduce the image
		ReduceImage( SubTex.Image );

		// Add the newly reduced texture
		for( it = Textures.begin(); it!=end && (*SubTex.Image.Size)<(*(*it).Image.Size); ++it);
		Textures.insert( it, SubTex );
		CurrentSize += *SubTex.Image.Size;

	} // end while( CurrentSize is too large )

	std_Size sz;
	for( sz=TextureSize; *sz>CurrentSize; ) sz /= 2;
	TextureSize = sz * 2;

	// Allocate the texture
	gfx_Image_t TextureImage;
	TextureImage.Size = TextureSize;
	TextureImage.Stride = NeedsPaletteConversion ? 4 : 1;
	TextureImage.Pitch = TextureImage.Size.width * TextureImage.Stride;
	TextureImage.pBytes = new BYTE[ *TextureSize * TextureImage.Stride];

	// Initialize the filler
	Filler.Initialize( TextureSize.width, TextureSize.height );

	// Add each texture
	for( it = Textures.begin(); it!=end; ++it)
	{
		// Get the image and find a destination rect
		pImage = &(*it).Image;
		while( bFAILED(Filler.FindSuitableRect( pImage->Size.width, pImage->Size.height, &Destination )) )
		{
			// Crap...  reduce some more.
			ReduceImage( *pImage );
		}

		// Copy the image to the texture
		BYTE* pTex = TextureImage.pBytes + (Destination.left * TextureImage.Stride + Destination.top * TextureImage.Pitch);
		BYTE* pImg = pImage->pBytes;
		for( long y=pImage->Size.height; y>0; --y)
		{
			if (NeedsPaletteConversion)
			{
				const PALETTEENTRY *palette = static_cast<PALETTEENTRY *>(pPalette);
				for (int a = 0, b = 0, n = pImage->Size.width; a < n; ++a, b += TextureImage.Stride)
				{
					const BYTE index = pImg[a];
					const PALETTEENTRY *entry = palette + index;
					pTex[b + 0] = entry->peRed;
					pTex[b + 1] = entry->peGreen;
					pTex[b + 2] = entry->peBlue;
					pTex[b + 3] = entry->peFlags;
				}
			}
			else
			{
				memcpy( pTex, pImg, pImage->Size.width );
			}
			pTex += TextureImage.Pitch;
			pImg += pImage->Pitch;
		}

		// Set the texture coordinates
		(*it).TexCoords[0] = (float)Destination.left / (float)TextureSize.width;
		(*it).TexCoords[1] = (float)Destination.top / (float)TextureSize.height;
		(*it).TexCoords[2] = (*it).TexCoords[0];
		(*it).TexCoords[3] = (float)Destination.bottom / (float)TextureSize.height;
		(*it).TexCoords[4] = (float)Destination.right / (float)TextureSize.width;
		(*it).TexCoords[5] = (*it).TexCoords[3];
		(*it).TexCoords[6] = (*it).TexCoords[4];
		(*it).TexCoords[7] = (*it).TexCoords[1];

		// The image is no longer needed
		SAFE_DELETE( (*it).Image.pBytes );

	} // end for( Textures )

	// Create the texture
	glGenTextures( 1, &m_Texture );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GLTexInternalFormat,
		(GLsizei)TextureSize.width,
		(GLsizei)TextureSize.height,
		0,
		GLTexFormat,
		GL_UNSIGNED_BYTE,
		TextureImage.pBytes);

	//CreateBMP( (BYTE*)LockedRect.pBits, m_pHostInterface->m_Palettes.front().Entries, MaxTextureSize.width, MaxTextureSize.height );

	SAFE_DELETE_ARRAY(TextureImage.pBytes);
	return TRUE;
}
// End ogl_Model::AssembleTexture()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::ReduceImage() //              Author: Logan "Burn" Jones
/////////////////////////////                          Date: 3/24/2002
//               
//====================================================================
// Parameters:
//  gfx_Image_t& Image - 
//
void ogl_Model::ReduceImage( gfx_Image_t& Image )
{
	gfx_Image_t		Reduced;

	// Create a new image structure
	Reduced.Size = Image.Size / 2;
	assert( (Reduced.Size.width>1) && (Reduced.Size.height>1) );
	Reduced.Stride = 1;
	Reduced.Pitch = Reduced.Size.width;
	Reduced.pBytes = new BYTE[ *Reduced.Size ];

	// Reduce and discard the old one
	BYTE* pNew = Reduced.pBytes, *pOld = Image.pBytes;
	for( long h=0; h<Reduced.Size.height; ++h,pOld+=Image.Pitch)
		for( long w=0; w<Reduced.Size.width; ++w,++pNew,pOld+=2)
			*pNew = *pOld;
	delete [] Image.pBytes;
	Image = Reduced;
}
// End ogl_Model::ReduceImage()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::LoadModel() //                Author: Logan "Burn" Jones
///////////////////////////                            Date: 3/22/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer	         - 
//  DWORD dwOffset		         - 
//  DWORD& dwNameOffset	         - 
//  DWORD& dwPieceOffset         - 
//  GLuint uiListBase            - 
//  UsedTextureList_t& Textures  - 
//
void ogl_Model::LoadModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwNameOffset, DWORD& dwPieceOffset, GLuint uiListBase, UsedTextureList_t& Textures )
{
	float       TexCoords[8] = {0,0,0,1,1,1,1,0};
	int			ParentIndex = dwPieceOffset - 1;
	DWORD		TextureType;

	// Retrieve the piece structure
	TA_3DO_OBJECT* pObject = (TA_3DO_OBJECT*)(pFileBuffer + dwOffset);

	// Save a pointer to the current model piece
	Piece_t* pPiece = &m_Pieces[ dwPieceOffset ];

	// Iterate through each sibling
	while( pObject!=NULL )
	{
		// Initialize this piece
		ZeroMemory( pPiece, sizeof(Piece_t) );
		pPiece->Index = dwPieceOffset;
		pPiece->Parent = ParentIndex;

		// Set the origin
		pPiece->Origin.x = pObject->XFromParent / LINEAR_CONSTANT;
		pPiece->Origin.y = pObject->YFromParent / LINEAR_CONSTANT;
		pPiece->Origin.z = pObject->ZFromParent / LINEAR_CONSTANT;

		// Add the piece name to the name buffer
		if(pObject->OffsetToObjectName)
		{
			strcpy( m_PieceNameBuffer + dwNameOffset, (LPTSTR)(pFileBuffer + pObject->OffsetToObjectName) );
			pPiece->Name = m_PieceNameBuffer + dwNameOffset;
			for( int i=0; pPiece->Name[i]!='\0'; ++i)
				pPiece->Name[i] = toupper(pPiece->Name[i]);
			dwNameOffset += strlen(m_PieceNameBuffer + dwNameOffset) + 1;
			m_PieceNameMap[ pPiece->Name ] = dwPieceOffset;
		}

		// Are there any primitives to worry about
		if( pObject->NumberOfPrimitives>0 )
		{
			// Retrieve the vertex array for this object
			LPTA_3DO_VERTEX pVertecies = (LPTA_3DO_VERTEX)(pFileBuffer + pObject->OffsetToVertexArray);

			// Define some macros to make the stuff below look a little more sane
			#define ADD_VERTEX( TEX_U, TEX_V, INDEX )  \
				glTexCoord2f( (TEX_U), (TEX_V) ); \
				glVertex3f( \
					(pVertecies + IndexArray[INDEX])->x / LINEAR_CONSTANT, \
					(pVertecies + IndexArray[INDEX])->z / LINEAR_CONSTANT, \
					(pVertecies + IndexArray[INDEX])->y / LINEAR_CONSTANT  \
				);

			// Start compiling the display list
			pPiece->DisplayList = uiListBase + dwPieceOffset;
			glNewList( pPiece->DisplayList, GL_COMPILE );
			glBegin( GL_TRIANGLES );
		//	DWORD CurrentType = 0xFFFF;

			// Proccess each primitive
			TA_3DO_PRIMITIVE* pPrimitive = (TA_3DO_PRIMITIVE*)(pFileBuffer + pObject->OffsetToPrimitiveArray);
			for( DWORD count=0; count<pObject->NumberOfPrimitives; ++count,++pPrimitive)
			{
				// If this is the ground plate or an illegal primitive, skip it
				if( ((INT32)count)==pObject->GroundPlateIndex || pPrimitive->NumberOfVertexIndexes>5 || pPrimitive->NumberOfVertexIndexes<3 )
					continue;

				// Get the texture coordinates
		//		if( pPrimitive->OffsetToTextureName )
		//			GetTextureCoordinates( TexCoords, (LPTSTR)(pFileBuffer + pPrimitive->OffsetToTextureName), 0, Textures );
		//		else GetTextureCoordinates( TexCoords, NULL, pPrimitive->Color, Textures );
				TextureType = GetTextureCoordinates( TexCoords, (pPrimitive->OffsetToTextureName) ? (LPTSTR)(pFileBuffer + pPrimitive->OffsetToTextureName):NULL, pPrimitive->Color, Textures );
		//		if( TextureType!=CurrentType )
		//		{
		//			Part_t* pPart = pPiece->Parts;
		//		}

				// Retrieve the index array for this primitive
				WORD* IndexArray = (WORD*)(pFileBuffer + pPrimitive->OffsetToVertexIndexArray);

				// Record where we started and proccess the primitive
				switch( pPrimitive->NumberOfVertexIndexes )
				{
					case 3: // Single Triangle
						ADD_VERTEX( TexCoords[4], TexCoords[5], 2 );
						ADD_VERTEX( TexCoords[6], TexCoords[7], 1 );
						ADD_VERTEX( TexCoords[0], TexCoords[1], 0 );
						pPiece->Primitives += 1;
						break;

					case 4: // Single Quad, split into two triangles
						ADD_VERTEX( TexCoords[2],TexCoords[3], 3 );
						ADD_VERTEX( TexCoords[6],TexCoords[7], 1 );
						ADD_VERTEX( TexCoords[0],TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[2],TexCoords[3], 3 );
						ADD_VERTEX( TexCoords[4],TexCoords[5], 2 );
						ADD_VERTEX( TexCoords[6],TexCoords[7], 1 );
						pPiece->Primitives += 2;
						break;

					case 5: // Pentagon, split into three triangles
						const float 	fTopWidth = TexCoords[6] - TexCoords[0];
						const float 	fBotWidth = TexCoords[4] - TexCoords[2];
						const float 	fLeftHeight = TexCoords[3] - TexCoords[1];
						const float 	fRightHeight= TexCoords[5] - TexCoords[7];
						ADD_VERTEX( TexCoords[2] + (fBotWidth * 0.25f), TexCoords[3], 2 );
						ADD_VERTEX( TexCoords[0], TexCoords[1] + (fLeftHeight * (1/3.0f)), 1 );
						ADD_VERTEX( TexCoords[0] + (fTopWidth * 0.5f), TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[4] - (fBotWidth * 0.25f), TexCoords[5], 3 );
						ADD_VERTEX( TexCoords[2] + (fBotWidth * 0.25f), TexCoords[3], 2 );
						ADD_VERTEX( TexCoords[0] + (fTopWidth * 0.5f), TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[6], TexCoords[7] + (fRightHeight * (1/3.0f)), 4 );
						ADD_VERTEX( TexCoords[4] - (fBotWidth * 0.25f), TexCoords[5], 3 );
						ADD_VERTEX( TexCoords[0] + (fTopWidth * 0.5f), TexCoords[1], 0 );
						pPiece->Primitives += 3;
						break;

				} // end switch( pPrimitive->NumberOfVertexIndexes )

			} // end for( primitives )

			// Finished compiling the display list
			glEnd();
			glEndList();

		} // end if( there are any primitives )

		// If there is a child, load it
		if( pObject->OffsetToChildObject!=0 )
		{
			pPiece->pChild = &m_Pieces[ ++dwPieceOffset ];
			LoadModel( pFileBuffer, pObject->OffsetToChildObject, dwNameOffset, dwPieceOffset, uiListBase, Textures );
		}

		// If there is a sibling, procceed to load it.
		if( pObject->OffsetToSiblingObject!=0 )
		{
			pObject = (TA_3DO_OBJECT*)(pFileBuffer + pObject->OffsetToSiblingObject);
			pPiece = pPiece->pSibling = &m_Pieces[ ++dwPieceOffset ];
		}
		else break;

	} // end while( pObject )
}
// End ogl_Model::LoadModel()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ogl_Model::GetTextureCoordinates() //    Author: Logan "Burn" Jones
///////////////////////////////////////                Date: 3/23/2002
//               
//====================================================================
// Parameters:
//  float* pTexCoords           - 
//  LPTSTR strTexture           - 
//  DWORD dwColor               - 
//  UsedTextureList_t& Textures - 
//
// Return: DWORD - 
//
DWORD ogl_Model::GetTextureCoordinates( float* pTexCoords, LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures )
{
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();

	if( strTexture )
	{
		// Find the entry
		for(; it!=end; ++it)
			if( (*it).Texture && stricmp(strTexture,(*it).Texture)==0 )
			{
				memcpy( pTexCoords, (*it).TexCoords, sizeof(float)*8 );
				return (*it).Color;
			}

		// Texture name not found, use default color
		return GetTextureCoordinates( pTexCoords, NULL, 255, Textures );
	}
	else // Its a color
	{
		// NOTE: This is a little hack to keep pieces that were intended to be black from being clear
		if( dwColor==0 ) dwColor = 245;

		// Find the entry
		for(; it!=end; ++it)
			if( (*it).Texture==NULL && (*it).Color==dwColor )
				memcpy( pTexCoords, (*it).TexCoords, sizeof(float)*8 );

		return 0;
	}
}
// End ogl_Model::GetTextureCoordinates()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - ogl_Model.cpp //
////////////////////////
