// d3d_Model.cpp //                        Author: Logan "Burn" Jones
//////////////////                                    Date: 3/22/2002
//
/////////////////////////////////////////////////////////////////////

#include "d3d.h"
#include "d3d_Model.h"
#include "d3d_ModelInstance.h"
#include "../../../Source/game_Constants.h"
#include "../../../Source/ta_3DO.h"
#include "../../../Source/ta_GAF.h"
#include "../../../Source/util_RectFiller.h"

// Implement inline methods here for a debug build
#ifdef _DEBUG
 #include "d3d_Model.inl"
#endif // defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
// gfx_Direct3D8::CreateModel() //         Author: Logan "Burn" Jones
/////////////////////////////////                     Date: 3/22/2002
//               
//===================================================================
// Parameters:
//  BYTE* pFileBuffer   - 
//  gfx_Model** ppModel - 
//
// Return: BOOL - 
//
BOOL gfx_Direct3D8::CreateModel( BYTE* pFileBuffer, gfx_Model** ppModel )
{
	d3d_Model*		pModel;

	// Allocate the new model
	pModel = new d3d_Model( this );

	// Create the model
	if( bFAILED(pModel->Create(pFileBuffer)) )
	{
		delete pModel;
		return FALSE;
	}

	// Set the return pointer and return successfully
	*ppModel = pModel;
	return TRUE;
}
// End gfx_Direct3D8::CreateModel()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::Create() //                  Author: Logan "Burn" Jones
////////////////////////                               Date: 6/8/2002
//               
//===================================================================
// Parameters:
//  BYTE* pFileBuffer - 
//
// Return: BOOL - 
//
BOOL d3d_Model::Create( BYTE* pFileBuffer )
{
	DWORD					PieceIndex = 0;
	std_Vertex3				GroundPlate[4];
	model_Vertex*	pVertexBuffer;
	DWORD					VertexIndex = 0;
	WORD*					pIndexBuffer;
	DWORD					IndexIndex = 0;
	UsedTextureList_t		Textures;

	// Sanity checks
	if( m_pInterface==NULL || pFileBuffer==NULL )
		return FALSE;

	// Just checking
	Destroy();

	// Scan the model structure
	m_PieceCount = m_VertexCount = m_TriangleCount = m_GoundPlateSize = 0;
	ScanModel( pFileBuffer, 0, GroundPlate, Textures );

	// Allocate the piece array
	m_Pieces = new Piece_t[ m_PieceCount ];

	// Assemble the texture
	if( bFAILED(AssembleTexture( Textures )) )
    {
		Destroy();
        return FALSE;
    }

	// Lock the buffers
	pVertexBuffer = m_RawVertexArray = new model_Vertex[ m_VertexCount ];
	pIndexBuffer = m_RawIndexArray = new WORD[ (m_TriangleCount * 3) + m_GoundPlateSize ];

	// Load the model
	m_GoundPlateIndex = m_TriangleCount * 3;
	LoadModel( pFileBuffer, 0, PieceIndex,
		pVertexBuffer, VertexIndex,
		pIndexBuffer, IndexIndex,
		Textures );

	return TRUE;
}
// End d3d_Model::Create()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::Destroy() //                 Author: Logan "Burn" Jones
/////////////////////////                              Date: 6/8/2002
//               
//===================================================================
//
void d3d_Model::Destroy()
{
	SAFE_DELETE( m_Pieces );
	SAFE_DELETE( m_RawVertexArray );
	SAFE_DELETE( m_RawIndexArray );
}
// End d3d_Model::Destroy()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::GetPieceCount() //           Author: Logan "Burn" Jones
///////////////////////////////                       Date: 4/17/2002
//               
//===================================================================
// Return: const DWORD - 
//
const DWORD d3d_Model::GetPieceCount() const
{
	return m_PieceCount;
}
// End d3d_Model::GetPieceCount()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::GetPieceID() //              Author: Logan "Burn" Jones
////////////////////////////                           Date: 6/8/2002
//               
//===================================================================
// Parameters:
//  LPTSTR strPiece - 
//
// Return: long - 
//
long d3d_Model::GetPieceID( LPCTSTR strPiece ) const
{
	PieceNameHash_t::const_iterator it = m_PieceNames.find( std_NameHash(strPiece) );
	if( it==m_PieceNames.end() ) return -1;
	else return (*it).second;
}
// End d3d_Model::GetPieceID()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::NewInstance() //             Author: Logan "Burn" Jones
/////////////////////////////                        Date: 02-23-2003
//
//===================================================================
// Parameters:
//  class gfx_ModelInstance** ppInstance - 
//
void d3d_Model::NewInstance( class gfx_ModelInstance** ppInstance )
{
	d3d_ModelInstance*	pInst = new d3d_ModelInstance;
	*ppInstance = pInst;
	m_ModelInstances.push_front( pInst );
	pInst->Create( this, m_ModelInstances.begin() );
}
// End d3d_Model::NewInstance()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::ReleaseInstance() //         Author: Logan "Burn" Jones
/////////////////////////////////                    Date: 02-23-2003
//
//===================================================================
// Parameters:
//  gfx_ModelInstance* pInstance - 
//  model_HInstance hInstance    - 
//
void d3d_Model::ReleaseInstance( gfx_ModelInstance* pInstance, model_HInstance hInstance )
{
	m_ModelInstances.erase( hInstance );
	delete pInstance;
}
// End d3d_Model::ReleaseInstance()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::ScanModel() //               Author: Logan "Burn" Jones
///////////////////////////                            Date: 6/8/2002
//               
//===================================================================
// Parameters:
//  BYTE* pFileBuffer           - 
//  DWORD dwOffset              - 
//  std_Vertex3* pGoundPlate    - 
//  UsedTextureList_t& Textures - 
//
void d3d_Model::ScanModel( BYTE* pFileBuffer, DWORD dwOffset, std_Vertex3* pGoundPlate, UsedTextureList_t& Textures )
{
	// Retrieve the object structure
	TA_3DO_OBJECT* pObject = (TA_3DO_OBJECT*)(pFileBuffer + dwOffset);

	// Count this piece
	++m_PieceCount;

	// Iterate through each sibling
	while( pObject )
	{
		// Add the name to the list
		if( pObject->OffsetToObjectName )
			m_PieceNames[ std_NameHash((LPTSTR)(pFileBuffer + pObject->OffsetToObjectName)) ] = m_PieceCount - 1;

		// Scan each primitive
		TA_3DO_PRIMITIVE* pPrimitive = (TA_3DO_PRIMITIVE*)(pFileBuffer + pObject->OffsetToPrimitiveArray);
		for( DWORD count=0; count<pObject->NumberOfPrimitives; ++count,++pPrimitive)
		{
			// Check if this is the ground plate
/*			if( ((INT32)count)==pObject->GroundPlateIndex )
			{
				// TEMP
				assert( pPrimitive->NumberOfVertexIndexes==4 );
				LPTA_3DO_VERTEX pVertecies = (LPTA_3DO_VERTEX)(pFileBuffer + pObject->OffsetToVertexArray);
				WORD* IndexArray = (WORD*)(pFileBuffer + pPrimitive->OffsetToVertexIndexArray);
				for( int n=3; n>=0; --n)
					pGoundPlate[n].Set( (pVertecies + IndexArray[n])->x / LINEAR_CONSTANT,
										(pVertecies + IndexArray[n])->z / LINEAR_CONSTANT,
										(pVertecies + IndexArray[n])->y / LINEAR_CONSTANT );
				continue;
			}
*/
			// No points, lines or anything above pentagons please
			if( (pPrimitive->NumberOfVertexIndexes>5)||(pPrimitive->NumberOfVertexIndexes<3) )
				continue;
			else
			{
				m_VertexCount += pPrimitive->NumberOfVertexIndexes;
				if( ((INT32)count)==pObject->GroundPlateIndex )
				{
					assert( m_GoundPlateSize==0 );
					m_GoundPlateSize = pPrimitive->NumberOfVertexIndexes + 1;
				}
				else m_TriangleCount+= pPrimitive->NumberOfVertexIndexes - 2;
			}

			// Mark the texture that is given
			if( pPrimitive->OffsetToTextureName )
				MarkTexture( (LPTSTR)(pFileBuffer + pPrimitive->OffsetToTextureName), 0, Textures );
			else MarkTexture( NULL, pPrimitive->Color, Textures );

		} // end for( Primitives )

		// If there is a child, scan it
		if( pObject->OffsetToChildObject!=0 )
			ScanModel( pFileBuffer, pObject->OffsetToChildObject, pGoundPlate, Textures );

		// If there is a sibling, procceed to scan it.
		if( pObject->OffsetToSiblingObject!=0 )
		{
			pObject = (TA_3DO_OBJECT*)(pFileBuffer + pObject->OffsetToSiblingObject);
			++m_PieceCount;
		}
		else break;

	} // end while( pObject )
}
// End d3d_Model::ScanModel()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::MarkTexture() //             Author: Logan "Burn" Jones
/////////////////////////////                         Date: 3/23/2002
//               
//===================================================================
// Parameters:
//  LPTSTR strTexture           - 
//  DWORD dwColor               - 
//  UsedTextureList_t& Textures - 
//
void d3d_Model::MarkTexture( LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures )
{
	UsedTexture_t						NewTex;
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();

	if( strTexture && (dwColor = 255) && m_pInterface->m_ModelTextures.find(strTexture)!=m_pInterface->m_ModelTextures.end() )
	{
		// Make sure its not already used
		for(; it!=end; ++it)
			if( (*it).Texture && stricmp(strTexture,(*it).Texture)==0 )
				return;

		LPTA_GAF_ENTRY pEntry = (LPTA_GAF_ENTRY)m_pInterface->m_ModelTextures[strTexture];
		BYTE* pFile = (BYTE*)pEntry->UnKnown_2;
		LPTA_GAF_FRAME_DATA pFrame = (LPTA_GAF_FRAME_DATA)(pFile + ((LPTA_GAF_FRAME_ENTRY)( ((BYTE*)pEntry) + sizeof(TA_GAF_ENTRY) ))->OffsetToFrameData);

		NewTex.Texture = strTexture;
		NewTex.Color = 0;
		NewTex.Image.Size.Set( pFrame->Width, pFrame->Height );
		NewTex.Image.Stride = 1;
		NewTex.Image.Pitch = pFrame->Width;
		NewTex.Image.pBytes = new BYTE[ *NewTex.Image.Size ];
		ta_GAF_GetPalettedFrameBits( pFile, pFrame, NewTex.Image.pBytes );
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
// End d3d_Model::MarkTexture()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::AssembleTexture() //         Author: Logan "Burn" Jones
/////////////////////////////////                     Date: 3/23/2002
//               
//===================================================================
// Parameters:
//  UsedTextureList_t& Textures - 
//
// Return: BOOL - 
//
BOOL d3d_Model::AssembleTexture( UsedTextureList_t& Textures )
{
	const std_Size						MaxTextureSize(256,256);
	std_Size							TextureSize( MaxTextureSize );
	DWORD								CurrentSize = 0;
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();
	UsedTexture_t						SubTex;
	gfx_Image_t							Reduced;
	D3DLOCKED_RECT						LockedRect;
	util_RectFiller						Filler;
	gfx_Image_t*						pImage;
	std_Rect							Destination;

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

	// Create the texture and lock it
	m_pInterface->m_pD3DDevice->CreateTexture( TextureSize.width, TextureSize.height, 1, 0, D3DFMT_P8, D3DPOOL_MANAGED, &m_PrimaryTexture );
	m_PrimaryTexture->LockRect( 0, &LockedRect, NULL, 0 );

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
		BYTE* pTex = ((BYTE*)LockedRect.pBits) + (Destination.left + Destination.top * LockedRect.Pitch);
		BYTE* pImg = pImage->pBytes;
		for( long y=pImage->Size.height; y>0; --y)
		{
			memcpy( pTex, pImg, pImage->Size.width );
			pTex += LockedRect.Pitch;
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

	// Unlock the texture and return
	m_PrimaryTexture->UnlockRect( 0 );
	return TRUE;
}
// End d3d_Model::AssembleTexture()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::ReduceImage() //             Author: Logan "Burn" Jones
/////////////////////////////                         Date: 3/24/2002
//               
//===================================================================
// Parameters:
//  gfx_Image_t& Image - 
//
void d3d_Model::ReduceImage( gfx_Image_t& Image )
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
// End d3d_Model::ReduceImage()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::LoadModel() //               Author: Logan "Burn" Jones
///////////////////////////                           Date: 3/22/2002
//               
//===================================================================
// Parameters:
//  BYTE* pFileBuffer	         - 
//  DWORD dwOffset		         - 
//  DWORD& dwPieceOffset         - 
//  model_Vertex* pVertexBuffer -
//  DWORD& dwVertexIndex         - 
//  WORD* pIndexBuffer           - 
//  DWORD& dwIndexIndex          - 
//  UsedTextureList_t& Textures  - 
//
void d3d_Model::LoadModel( BYTE* pFileBuffer, DWORD dwOffset, DWORD& dwPieceOffset, model_Vertex* pVertexBuffer, DWORD& dwVertexIndex, WORD* pIndexBuffer, DWORD& dwIndexIndex, UsedTextureList_t& Textures )
{
	float       TexCoords[8] = {0,0,0,1,1,1,1,0};
	int			ParentIndex = dwPieceOffset - 1;

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
//		pPiece->Parent = ParentIndex;

		// Set the origin
		pPiece->Origin.x = pObject->XFromParent / LINEAR_CONSTANT;
		pPiece->Origin.y = pObject->YFromParent / LINEAR_CONSTANT;
		pPiece->Origin.z = pObject->ZFromParent / LINEAR_CONSTANT;

		// Are there any primitives to worry about
		if( pObject->NumberOfPrimitives>0 )
		{
			// Remember where we started
			pPiece->VertexOffset = dwVertexIndex;
			pPiece->IndexOffset = dwIndexIndex;

			// Retrieve the vertex array for this object
			LPTA_3DO_VERTEX pVertecies = (LPTA_3DO_VERTEX)(pFileBuffer + pObject->OffsetToVertexArray);

			// Define some macros to make the stuff below look a little more sane
			#define ADD_VERTEX( TEX_U, TEX_V, INDEX )  \
				pVertexBuffer[dwVertexIndex].tu_0 = (TEX_U); \
				pVertexBuffer[dwVertexIndex].tv_0 = (TEX_V); \
				pVertexBuffer[dwVertexIndex].diffuse = D3DCOLOR_RGBA(0,0,0,100); \
				pVertexBuffer[dwVertexIndex].specular= D3DCOLOR_XRGB(0,0,200); \
				pVertexBuffer[dwVertexIndex].position.x  = (pVertecies + IndexArray[INDEX])->x / LINEAR_CONSTANT; \
				pVertexBuffer[dwVertexIndex].position.y  = (pVertecies + IndexArray[INDEX])->z / LINEAR_CONSTANT; \
				pVertexBuffer[dwVertexIndex].position.z  = (pVertecies + IndexArray[INDEX])->y / LINEAR_CONSTANT; \
				++dwVertexIndex;
			#define ADD_INDEX( wINDEX ) pIndexBuffer[dwIndexIndex++] = ((WORD)(wINDEX));

			// Proccess each primitive
			TA_3DO_PRIMITIVE* pPrimitive = (TA_3DO_PRIMITIVE*)(pFileBuffer + pObject->OffsetToPrimitiveArray);
			for( DWORD count=0; count<pObject->NumberOfPrimitives; ++count,++pPrimitive)
			{
				// If this is the ground plate or an illegal primitive, skip it
				if( /*((INT32)count)==pObject->GroundPlateIndex ||*/ pPrimitive->NumberOfVertexIndexes>5 || pPrimitive->NumberOfVertexIndexes<3 )
					continue;

				// Get the texture coordinates
				if( pPrimitive->OffsetToTextureName )
					GetTextureCoordinates( TexCoords, (LPTSTR)(pFileBuffer + pPrimitive->OffsetToTextureName), 0, Textures );
				else GetTextureCoordinates( TexCoords, NULL, pPrimitive->Color, Textures );

				// Retrieve the index array for this primitive
				WORD* IndexArray = (WORD*)(pFileBuffer + pPrimitive->OffsetToVertexIndexArray);

				// Record where we started and proccess the primitive
				DWORD StartVertex = dwVertexIndex;
				switch( pPrimitive->NumberOfVertexIndexes )
				{
					case 3: // Single Triangle
						ADD_VERTEX( TexCoords[0], TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[6], TexCoords[7], 1 );
						ADD_VERTEX( TexCoords[4], TexCoords[5], 2 );
						if( ((INT32)count)==pObject->GroundPlateIndex )
							SetGroundPlate( StartVertex, 3, pVertexBuffer, pIndexBuffer );
						else {
							ADD_INDEX( StartVertex+2 );
							ADD_INDEX( StartVertex+1 );
							ADD_INDEX( StartVertex );
						}
						break;

					case 4: // Single Quad, split into two triangles
						ADD_VERTEX( TexCoords[0],TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[6],TexCoords[7], 1 );
						ADD_VERTEX( TexCoords[2],TexCoords[3], 3 );
						ADD_VERTEX( TexCoords[4],TexCoords[5], 2 );
						if( ((INT32)count)==pObject->GroundPlateIndex )
							SetGroundPlate( StartVertex, 4, pVertexBuffer, pIndexBuffer );
						else {
							ADD_INDEX( StartVertex+2 );
							ADD_INDEX( StartVertex+1 );
							ADD_INDEX( StartVertex );
							ADD_INDEX( StartVertex+2 );
							ADD_INDEX( StartVertex+3 );
							ADD_INDEX( StartVertex+1 );
						}
						break;

					case 5: // Pentagon, split into three triangles
						const float 	fTopWidth = TexCoords[6] - TexCoords[0];
						const float 	fBotWidth = TexCoords[4] - TexCoords[2];
						const float 	fLeftHeight = TexCoords[3] - TexCoords[1];
						const float 	fRightHeight= TexCoords[5] - TexCoords[7];
						ADD_VERTEX( TexCoords[0] + (fTopWidth * 0.5f), TexCoords[1], 0 );
						ADD_VERTEX( TexCoords[0], TexCoords[1] + (fLeftHeight * (1/3.0f)), 1 );
						ADD_VERTEX( TexCoords[2] + (fBotWidth * 0.25f), TexCoords[3], 2 );
						ADD_VERTEX( TexCoords[4] - (fBotWidth * 0.25f), TexCoords[5], 3 );
						ADD_VERTEX( TexCoords[6], TexCoords[7] + (fRightHeight * (1/3.0f)), 4 );
						if( ((INT32)count)==pObject->GroundPlateIndex )
							SetGroundPlate( StartVertex, 5, pVertexBuffer, pIndexBuffer );
						else {
							ADD_INDEX( StartVertex+2 );
							ADD_INDEX( StartVertex+1 );
							ADD_INDEX( StartVertex );
							ADD_INDEX( StartVertex+3 );
							ADD_INDEX( StartVertex+2 );
							ADD_INDEX( StartVertex );
							ADD_INDEX( StartVertex+4 );
							ADD_INDEX( StartVertex+3 );
							ADD_INDEX( StartVertex );
						}
						break;

				} // end switch( pPrimitive->NumberOfVertexIndexes )

			} // end for( primitives )

			// Record where we ended
			pPiece->VertexCount = dwVertexIndex - pPiece->VertexOffset;
			pPiece->IndexCount = dwIndexIndex - pPiece->IndexOffset;
//			pPiece->Primitives = (dwIndexIndex - pPiece->IndexOffset) / 3;

		} // end if( there are any primitives )

		// If there is a child, load it
		if( pObject->OffsetToChildObject!=0 )
		{
			pPiece->pChild = &m_Pieces[ ++dwPieceOffset ];
			LoadModel( pFileBuffer, pObject->OffsetToChildObject, dwPieceOffset, pVertexBuffer, dwVertexIndex, pIndexBuffer, dwIndexIndex, Textures );
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
// End d3d_Model::LoadModel()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::GetTextureCoordinates() //   Author: Logan "Burn" Jones
///////////////////////////////////////               Date: 3/23/2002
//               
//===================================================================
// Parameters:
//  float* pTexCoords           - 
//  LPTSTR strTexture           - 
//  DWORD dwColor               - 
//  UsedTextureList_t& Textures - 
//
void d3d_Model::GetTextureCoordinates( float* pTexCoords, LPTSTR strTexture, DWORD dwColor, UsedTextureList_t& Textures )
{
	UsedTextureList_t::iterator			it = Textures.begin();
	UsedTextureList_t::const_iterator	end = Textures.end();

	if( strTexture )
	{
		// Find the entry
		for(; it!=end; ++it)
			if( (*it).Texture && stricmp(strTexture,(*it).Texture)==0 )
				memcpy( pTexCoords, (*it).TexCoords, sizeof(float)*8 );
	}
	else // Its a color
	{
		// NOTE: This is a little hack to keep pieces that were intended to be black from being clear
		if( dwColor==0 ) dwColor = 245;

		// Find the entry
		for(; it!=end; ++it)
			if( (*it).Texture==NULL && (*it).Color==dwColor )
				memcpy( pTexCoords, (*it).TexCoords, sizeof(float)*8 );
	}
}
// End d3d_Model::GetTextureCoordinates()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// d3d_Model::SetGroundPlate() //          Author: Logan "Burn" Jones
////////////////////////////////                      Date: 6/11/2002
//               
//===================================================================
// Parameters:
//  DWORD dwStartIndex          - 
//  DWORD dwNumVerts            - 
//  model_Vertex* pVertexBuffer - 
//  WORD* pIndexBuffer          - 
//
void d3d_Model::SetGroundPlate( DWORD dwStartIndex, DWORD dwNumVerts, model_Vertex* pVertexBuffer, WORD* pIndexBuffer )
{
	for( DWORD n=0; n<dwNumVerts; ++n)
		pVertexBuffer[dwStartIndex + n].diffuse = D3DCOLOR_XRGB(255,255,0),
		pVertexBuffer[dwStartIndex + n].specular = D3DCOLOR_XRGB(255,255,0);

	m_GoundPlateVertex = dwStartIndex;

	switch( dwNumVerts )
	{
		case 3:
			pIndexBuffer[ m_GoundPlateIndex ] = dwStartIndex;
			pIndexBuffer[m_GoundPlateIndex+1] = dwStartIndex + 1;
			pIndexBuffer[m_GoundPlateIndex+2] = dwStartIndex + 2;
			pIndexBuffer[m_GoundPlateIndex+3] = dwStartIndex;
			break;
		case 4:
			pIndexBuffer[ m_GoundPlateIndex ] = dwStartIndex;
			pIndexBuffer[m_GoundPlateIndex+1] = dwStartIndex + 1;
			pIndexBuffer[m_GoundPlateIndex+2] = dwStartIndex + 3;
			pIndexBuffer[m_GoundPlateIndex+3] = dwStartIndex + 2;
			pIndexBuffer[m_GoundPlateIndex+4] = dwStartIndex;
			break;
		case 5:
			pIndexBuffer[ m_GoundPlateIndex ] = dwStartIndex;
			pIndexBuffer[m_GoundPlateIndex+1] = dwStartIndex + 1;
			pIndexBuffer[m_GoundPlateIndex+2] = dwStartIndex + 2;
			pIndexBuffer[m_GoundPlateIndex+3] = dwStartIndex + 3;
			pIndexBuffer[m_GoundPlateIndex+4] = dwStartIndex + 4;
			pIndexBuffer[m_GoundPlateIndex+5] = dwStartIndex;
			break;
	}
}
// End d3d_Model::SetGroundPlate()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - d3d_Model.cpp //
////////////////////////
