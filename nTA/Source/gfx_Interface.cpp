// gfx_Interface.cpp //                            \author Logan Jones
//////////////////////                                \date 3/19/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "gfx.h"
#include "game.h"
#include "ta_GAF.h"


// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "gfx_Interface.inl"
#endif // defined( _DEBUG )


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateSurfaceFromGAF() //        \author Logan Jones
//////////////////////////////////////////             \date 10/9/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  LPCTSTR strImageName    - 
//  int iFrameNumber        - 
//  gfx_Surface** ppSurface - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateSurfaceFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, gfx_Surface** ppSurface )
{
    LPTA_GAF_HEADER         Header;
    DWORD*                  EntryPointers;
    LPTA_GAF_ENTRY          ImageEntry = NULL;
    LPTA_GAF_FRAME_ENTRY    FrameEntries;
    LPTA_GAF_FRAME_DATA     FrameData;
	gfx_Image_t				Image;

    // Sanity check
    assert( pFileBuffer );

    // Get the header and entry pointers
	Header = (LPTA_GAF_HEADER)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer + sizeof(TA_GAF_HEADER));

    // If an image name was not given, just use the first image that
    // occurs in the GAF
    if( strImageName==NULL )
    {
        ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[0]);
        FrameEntries = (LPTA_GAF_FRAME_ENTRY)(pFileBuffer + (EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
    }
    else // Find the appropriate image entry
    {
        
        FrameEntries = NULL;
	    for( DWORD x=0; x<Header->NumberOfEntries; x++)
	    {
		    ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[x]);
		    if( stricmp(ImageEntry->Name,strImageName)==0 )
		    {
			    FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer + (EntryPointers[x]+sizeof(TA_GAF_ENTRY)));
			    break;
		    }
	    }

        // Check to see if the image entry was found
	    if( FrameEntries==NULL || ImageEntry==NULL )
	    {
            // Image was not found
		    return FALSE;
	    }

    } // end else strImageName!=NULL

    // Check for an erraneous frame number or the default
    if( iFrameNumber<=0 )
    {
        // Set to the first frame
        iFrameNumber = 1;
    }
    // Check for an over bounds frame number
    else if( iFrameNumber>ImageEntry->NumberOfFrames )
    {
        // Set to the last frame;
        iFrameNumber = ImageEntry->NumberOfFrames;
    }

    // Get the frame data
    FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber-1].OffsetToFrameData);

    // Get the dimensions ans allocate the image buffer
	Image.pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

    // Get the frame bits
    ta_GAF_GetPalettedImageBits(
        pFileBuffer + FrameData->OffsetToFrameData,
        FrameData->Width,
        FrameData->Height,
        FrameData->CompressionMethod,
        Image.pBytes );

	Image.Size.Set( FrameData->Width, FrameData->Height );
	Image.Pitch = Image.Size.width;
	Image.Stride = 1;
	BOOL hRet = CreateSurface( PIXEL_PALETTISED, &Image, ppSurface );
	delete [] Image.pBytes;
	return hRet;
}
// End gfx_Interface::CreateSurfaceFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateSurfaceFromGAF() //        \author Logan Jones
//////////////////////////////////////////              \date 4/1/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  LPCTSTR strImageName    - 
//  int iFrameNumber        - 
//  std_Size_t& szDesired   - 
//  gfx_Surface** ppSurface - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateSurfaceFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, const std_Size_t& szDesired, gfx_Surface** ppSurface )
{
    LPTA_GAF_HEADER         Header;
    DWORD*                  EntryPointers;
    LPTA_GAF_ENTRY          ImageEntry = NULL;
    LPTA_GAF_FRAME_ENTRY    FrameEntries;
    LPTA_GAF_FRAME_DATA     FrameData;
	gfx_Image_t				Image;

    // Sanity check
    assert( pFileBuffer );

    // Get the header and entry pointers
	Header = (LPTA_GAF_HEADER)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer + sizeof(TA_GAF_HEADER));

    // If an image name was not given, just use the first image that
    // occurs in the GAF
    if( strImageName==NULL )
    {
        ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[0]);
        FrameEntries = (LPTA_GAF_FRAME_ENTRY)(pFileBuffer + (EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
    }
    else // Find the appropriate image entry
    {
        
        FrameEntries = NULL;
	    for( DWORD x=0; x<Header->NumberOfEntries; x++)
	    {
		    ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[x]);
		    if( stricmp(ImageEntry->Name,strImageName)==0 )
		    {
			    FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer + (EntryPointers[x]+sizeof(TA_GAF_ENTRY)));
			    break;
		    }
	    }

        // Check to see if the image entry was found
	    if( FrameEntries==NULL || ImageEntry==NULL )
	    {
            // Image was not found
		    return FALSE;
	    }

    } // end else strImageName!=NULL

    // Check for an erraneous frame number or the default
    if( iFrameNumber<=0 )
    {
        // Set to the first frame
        iFrameNumber = 1;
    }
    // Check for an over bounds frame number
    else if( iFrameNumber>ImageEntry->NumberOfFrames )
    {
        // Set to the last frame;
        iFrameNumber = ImageEntry->NumberOfFrames;
    }

    // Get the frame data
    FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber-1].OffsetToFrameData);

    // Get the dimensions ans allocate the image buffer
	BYTE* pFrame = new BYTE[ FrameData->Width*FrameData->Height ];

    // Get the frame bits
    ta_GAF_GetPalettedImageBits(
        pFileBuffer + FrameData->OffsetToFrameData,
        FrameData->Width,
        FrameData->Height,
        FrameData->CompressionMethod,
        pFrame );

	BYTE* pTmp = pFrame;
	BYTE* pImg = Image.pBytes = new BYTE[*szDesired];
	for( long y=0; y<szDesired.height; ++y)
	{
		memcpy( pImg, pTmp, szDesired.width );
		pTmp += FrameData->Width;
		pImg += szDesired.width;
	}
	delete [] pFrame;

	Image.Size = szDesired;
	Image.Pitch = Image.Size.width;
	Image.Stride = 1;
	BOOL hRet = CreateSurface( PIXEL_PALETTISED, &Image, ppSurface );
	delete [] Image.pBytes;
	return hRet;
}
// End gfx_Interface::CreateSurfaceFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateSurfacesFromGAF() //       \author Logan Jones
///////////////////////////////////////////           \date 10/17/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  LPCTSTR strImageName    - 
//  int iStartFrame         - 
//  int iFrameCount         - 
//  gfx_Surface** ppSurface - 
//
// Return: DWORD - 
//
DWORD gfx_Interface::CreateSurfacesFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iStartFrame, int iFrameCount, gfx_Surface** ppSurface )
{
    LPTA_GAF_HEADER         Header;
    DWORD*                  EntryPointers;
    LPTA_GAF_ENTRY          ImageEntry = NULL;
    LPTA_GAF_FRAME_ENTRY    FrameEntries;
    LPTA_GAF_FRAME_DATA     FrameData;
	gfx_Image_t				Image;
	int						iFrameNumber;
	int						iEndFrame;
	int						NumCreated;

    // Sanity check
    if( pFileBuffer==NULL ) return 0;

    // Get the header and entry pointers
	Header = (LPTA_GAF_HEADER)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer + sizeof(TA_GAF_HEADER));

    // If an image name was not given, just use the first image that
    // occurs in the GAF
    if( strImageName==NULL )
    {
        ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[0]);
        FrameEntries = (LPTA_GAF_FRAME_ENTRY)(pFileBuffer + (EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
    }
    else // Find the appropriate image entry
    {
        
        FrameEntries = NULL;
	    for( DWORD x=0; x<Header->NumberOfEntries; x++)
	    {
		    ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[x]);
		    if( stricmp(ImageEntry->Name,strImageName)==0 )
		    {
			    FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer + (EntryPointers[x]+sizeof(TA_GAF_ENTRY)));
			    break;
		    }
	    }

        // Check to see if the image entry was found
	    if( FrameEntries==NULL || ImageEntry==NULL )
	    {
            // Image was not found
		    return FALSE;
	    }

    } // end else strImageName!=NULL

    // Check for an erraneous frame number or the default
    if( iStartFrame<=0 )
    {
        // Set to the first frame
        iStartFrame = 1;
    }
    // Check for an over bounds frame number
	else if( iStartFrame>ImageEntry->NumberOfFrames )
	{
        // Set to the last frame;
		iStartFrame = ImageEntry->NumberOfFrames;
	}

	// Check if there are enough frames
    if( (iStartFrame + iFrameCount - 1)>ImageEntry->NumberOfFrames )
    {
		// Work to the last frame
        iFrameCount = ImageEntry->NumberOfFrames - iStartFrame + 1;
    }

	// Iterate from the first desired frame until iFrameCount have been created
	iFrameNumber = iStartFrame;
	iEndFrame = iStartFrame + iFrameCount - 1;
	for( NumCreated=0; iFrameNumber<=iEndFrame; ++iFrameNumber )
	{

		// Get the frame data
		FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber-1].OffsetToFrameData);

		// Get the dimensions ans allocate the image buffer
		Image.pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

		// Get the frame bits
	//	ta_GAF_GetPalettedImageBits(
	//		pFileBuffer + FrameData->OffsetToFrameData,
	//		FrameData->Width,
	//		FrameData->Height,
	//		FrameData->CompressionMethod,
	//		Image.pBytes );
		ta_GAF_GetPalettedFrameBits(
			pFileBuffer,
			FrameData,
			Image.pBytes );

		Image.Size.Set( FrameData->Width, FrameData->Height );
		Image.Pitch = Image.Size.width;
		Image.Stride = 1;
		if( bFAILED(CreateSurface( PIXEL_PALETTISED, &Image, &ppSurface[NumCreated] )) )
			iFrameNumber = iEndFrame + 1;
		else ++NumCreated;

		delete [] Image.pBytes;

	} // end for( iFrameCount )

	return NumCreated;
}
// End gfx_Interface::CreateSurfacesFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateFontFromGAF() //           \author Logan Jones
///////////////////////////////////////               \date 10/10/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer   - 
//  LPCTSTR strFontName - 
//  gfx_Font** ppFont   - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateFontFromGAF( BYTE* pFileBuffer, LPCTSTR strFontName, gfx_Font** ppFont )
{
	TA_GAF_HEADER*				Header;
	DWORD*						EntryPointers;
	TA_GAF_ENTRY*				FontEntry = NULL;
	TA_GAF_FRAME_ENTRY*			FrameEntries;
	TA_GAF_FRAME_DATA*			FrameData;
    BYTE						FontTexture[256*256];
	BYTE*						RawFrame;
	DWORD						Size;
	gfx_Interface::FontEntry	FontEntries[ 256 ];
	gfx_Image_t					Image;

	// Get the header and entry pointers
	Header = (TA_GAF_HEADER*)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer+sizeof(TA_GAF_HEADER));

	// If a font name was not given, use the first entry
	if( strFontName==NULL )
	{
		FontEntry = (TA_GAF_ENTRY*)(pFileBuffer+EntryPointers[0]);
		FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer+(EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
	}
	else
    // Try to find the given font
	{
		FrameEntries = NULL;
		for( DWORD num=0; num<Header->NumberOfEntries; num++)
		{
			FontEntry = (TA_GAF_ENTRY*)(pFileBuffer+EntryPointers[num]);
			if( stricmp(FontEntry->Name,strFontName)==0 )
			{
				FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer+(EntryPointers[num]+sizeof(TA_GAF_ENTRY)));
				break;
			}
		}

		// Check if the font was found
		if( FrameEntries==NULL )
		{
			// Not found, fail out
			return 0;
		}

	} // end if( strFontName!=NULL )
    
    // TODO: Cleanup font loading procedure
	memset( FontTexture, 0, sqr(256) );
    DWORD x,y;
	BYTE* pMap;
	BYTE* MapOffset = FontTexture;
	//BYTE RowCount=0;
	for( WORD FrameCount=0,num=0; FrameCount<FontEntry->NumberOfFrames; FrameCount++,num++ )
	{
		if( num==16 )
		{
			MapOffset += (256*15);
			num = 0;
		}
		FrameData = (TA_GAF_FRAME_DATA*)(pFileBuffer+FrameEntries[FrameCount].OffsetToFrameData);
		Size = FrameData->Width * FrameData->Height;

		// Check if this is the space frame
		if( FrameCount==' ' )
		{
			RawFrame = (BYTE*)(pFileBuffer+FrameData->OffsetToFrameData);
			pMap = MapOffset;
			for( y=FrameData->Height; y>0; y--)
			{
				memset( pMap, 0, FrameData->Width );
				pMap += 256;
				RawFrame += FrameData->Width;
			}
		}
		else if( FrameData->CompressionMethod ) // Decompress frame if needed
		{
			pMap = MapOffset;
			BYTE* pLine;
			WORD LineLength;
			//DWORD Rawindex=0;
			DWORD Texindex=0;
			BYTE mask;
			BYTE color;
			WORD count;
			DWORD Offset = FrameData->OffsetToFrameData;
			for( WORD linecount=0; linecount<FrameData->Height; linecount++)
			{
				pLine = pMap;
				LineLength = *((WORD*)(pFileBuffer+Offset)); Offset += sizeof(WORD);
				RawFrame = (BYTE*)(pFileBuffer+Offset); Offset += LineLength;
				for( WORD n=0; n<(LineLength); )
				{
					mask = RawFrame[n++];
					if((mask & 0x01) == 0x01)
					{
						count = (mask >> 1);
						//ZeroMemory( pLine, count/**sizeof(DWORD)*/ );
						pLine += count;
					}
					else if((mask & 0x02) == 0x02)
					{
						color = RawFrame[n++];
						count = (mask >> 2) + 1;
						for( WORD k=0; k<count; k++)
							pLine[ Texindex+k ] = color;
						pLine += count;
					}
					else
					{
						count = (mask >> 2) + 1;
						for( WORD k=0; k<count; k++)
							pLine[ Texindex+k ] = RawFrame[n+k];
						n += count;
						pLine += count;
					}
				} // End for( line length )
				pMap += 256;
			} // End for( num lines )
		}
		else // Frame is not compressed
		{
			RawFrame = (BYTE*)(pFileBuffer+FrameData->OffsetToFrameData);
			pMap = MapOffset;
			for( y=FrameData->Height; y>0; y--)
			{
				for( x=0; x<FrameData->Width; x++)
				{
					pMap[x] = RawFrame[x];
				}
				pMap += 256;
				RawFrame += FrameData->Width;
			}
		}
		MapOffset += 16;

		FontEntries[ FrameCount ].Size.Set( FrameData->Width, FrameData->Height );
		FontEntries[ FrameCount ].Offset.Set( FrameData->XOffset, FrameData->YOffset );
	}

    Image.pBytes = FontTexture;
	Image.Pitch = 256;
	Image.Stride = 1;
	Image.Size = std_Size_t( 256, 256 );
    return CreateFont( FontEntries, std_Size_t(16,16), PIXEL_PALETTISED, &Image, ppFont );
}
// End gfx_Interface::CreateFontFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateCursorFromGAF() //         \author Logan Jones
/////////////////////////////////////////             \date 10/28/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer     - 
//  LPCTSTR strCursorName - 
//  gfx_Cursor** ppCursor - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateCursorFromGAF( BYTE* pFileBuffer, LPCTSTR strCursorName, gfx_Cursor** ppCursor )
{
	TA_GAF_HEADER*				Header;
	DWORD*						EntryPointers;
	TA_GAF_ENTRY*				GafEntry = NULL;
	TA_GAF_FRAME_ENTRY*			FrameEntries;
	TA_GAF_FRAME_DATA*			FrameData;
	gfx_Interface::CursorFrame*	CursorFrames;
	long						iFrameNumber;
	BOOL						bReturn = TRUE;

	// Get the header and entry pointers
	Header = (TA_GAF_HEADER*)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer+sizeof(TA_GAF_HEADER));

	// If a font name was not given, use the first entry
	if( strCursorName==NULL )
	{
		GafEntry = (TA_GAF_ENTRY*)(pFileBuffer+EntryPointers[0]);
		FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer+(EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
	}
	else
    // Try to find the given font
	{
		FrameEntries = NULL;
		for( DWORD num=0; num<Header->NumberOfEntries; num++)
		{
			GafEntry = (TA_GAF_ENTRY*)(pFileBuffer+EntryPointers[num]);
			if( stricmp(GafEntry->Name,strCursorName)==0 )
			{
				FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer+(EntryPointers[num]+sizeof(TA_GAF_ENTRY)));
				break;
			}
		}

		// Check if the font was found
		if( FrameEntries==NULL )
		{
			// Not found, fail out
			return 0;
		}

	} // end if( strFontName!=NULL )

	if( GafEntry->NumberOfFrames==0 )
		return FALSE;

	// Allocate the cursor frame entries
	CursorFrames = new gfx_Interface::CursorFrame[ GafEntry->NumberOfFrames ];

	// Iterate from the first to the last frame
	for( iFrameNumber=0; iFrameNumber<GafEntry->NumberOfFrames; ++iFrameNumber )
	{

		// Get the frame data
		FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber].OffsetToFrameData);

		// Get the dimensions ans allocate the image buffer
		CursorFrames[iFrameNumber].Image.pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

		// Get the frame bits
		ta_GAF_GetPalettedImageBits(
			pFileBuffer + FrameData->OffsetToFrameData,
			FrameData->Width,
			FrameData->Height,
			FrameData->CompressionMethod,
			CursorFrames[iFrameNumber].Image.pBytes );

		CursorFrames[iFrameNumber].Image.Size.Set( FrameData->Width, FrameData->Height );
		CursorFrames[iFrameNumber].Image.Pitch = CursorFrames[iFrameNumber].Image.Size.width;
		CursorFrames[iFrameNumber].Image.Stride = 1;
		CursorFrames[iFrameNumber].HotSpot.Set( FrameData->XOffset, FrameData->YOffset );

	} // end for( NumberOfFrames )

	if( bFAILED(CreateCursor( GafEntry->Name, GafEntry->NumberOfFrames, CursorFrames, ppCursor )) )
		bReturn = FALSE;

	// Iterate from the first to the last frame and delete the images
	for( iFrameNumber=0; iFrameNumber<GafEntry->NumberOfFrames; ++iFrameNumber )
		delete [] CursorFrames[iFrameNumber].Image.pBytes;
	delete [] CursorFrames;

	return bReturn;
}
// End gfx_Interface::CreateCursorFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateCursorsFromGAF() //        \author Logan Jones
//////////////////////////////////////////            \date 10/28/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  gfx_Cursor*** pppCursor - 
//
// Return: DWORD - 
//
DWORD gfx_Interface::CreateCursorsFromGAF( BYTE* pFileBuffer, gfx_Cursor*** pppCursor )
{
	TA_GAF_HEADER*				Header;
	DWORD*						EntryPointers;
	TA_GAF_ENTRY*				GafEntry;
	TA_GAF_FRAME_ENTRY*			FrameEntries;
	TA_GAF_FRAME_DATA*			FrameData;
	gfx_Interface::CursorFrame*	CursorFrames;
	gfx_Cursor**				ppCursor;
	long						iFrameNumber;
	DWORD						NumCreated = 0;

	// Get the header and entry pointers
	Header = (TA_GAF_HEADER*)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer+sizeof(TA_GAF_HEADER));

	(*pppCursor) = new gfx_Cursor*[ Header->NumberOfEntries ];
	ppCursor = (*pppCursor);

	FrameEntries = NULL;
	for( DWORD num=0; num<Header->NumberOfEntries; ++num)
	{
		GafEntry = (TA_GAF_ENTRY*)(pFileBuffer+EntryPointers[num]);
		FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer+(EntryPointers[num]+sizeof(TA_GAF_ENTRY)));

		if( GafEntry->NumberOfFrames==0 )
			continue;

		// Allocate the cursor frame entries
		CursorFrames = new gfx_Interface::CursorFrame[ GafEntry->NumberOfFrames ];

		// Iterate from the first to the last frame
		for( iFrameNumber=0; iFrameNumber<GafEntry->NumberOfFrames; ++iFrameNumber )
		{

			// Get the frame data
			FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber].OffsetToFrameData);

			// Get the dimensions ans allocate the image buffer
			CursorFrames[iFrameNumber].Image.pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

			// Get the frame bits
			ta_GAF_GetPalettedImageBits(
				pFileBuffer + FrameData->OffsetToFrameData,
				FrameData->Width,
				FrameData->Height,
				FrameData->CompressionMethod,
				CursorFrames[iFrameNumber].Image.pBytes );

			CursorFrames[iFrameNumber].Image.Size.Set( FrameData->Width, FrameData->Height );
			CursorFrames[iFrameNumber].Image.Pitch = CursorFrames[iFrameNumber].Image.Size.width;
			CursorFrames[iFrameNumber].Image.Stride = 1;
			CursorFrames[iFrameNumber].HotSpot.Set( FrameData->XOffset, FrameData->YOffset );

		} // end for( NumberOfFrames )

		if( bFAILED(CreateCursor( GafEntry->Name, GafEntry->NumberOfFrames, CursorFrames, &ppCursor[iFrameNumber] )) )
			ppCursor[iFrameNumber] = NULL;
		else ++NumCreated;

		// Iterate from the first to the last frame and delete the images
		for( iFrameNumber=0; iFrameNumber<GafEntry->NumberOfFrames; ++iFrameNumber )
			delete [] CursorFrames[iFrameNumber].Image.pBytes;
		delete [] CursorFrames;

	} // end for( NumberOfEntries )

	return NumCreated;
}
// End gfx_Interface::CreateCursorsFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::RetrieveImageFromGAF() //        \author Logan Jones
//////////////////////////////////////////            \date 10/26/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer    - 
//  LPCTSTR strImageName - 
//  int iFrameNumber     - 
//  gfx_Image_t* pImage  - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::RetrieveImageFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iFrameNumber, gfx_Image_t* pImage )
{
    LPTA_GAF_HEADER         Header;
    DWORD*                  EntryPointers;
    LPTA_GAF_ENTRY          ImageEntry = NULL;
    LPTA_GAF_FRAME_ENTRY    FrameEntries;
    LPTA_GAF_FRAME_DATA     FrameData;

    // Sanity check
    assert( pFileBuffer && pImage );

    // Get the header and entry pointers
	Header = (LPTA_GAF_HEADER)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer + sizeof(TA_GAF_HEADER));

    // If an image name was not given, just use the first image that
    // occurs in the GAF
    if( strImageName==NULL )
    {
        ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[0]);
        FrameEntries = (LPTA_GAF_FRAME_ENTRY)(pFileBuffer + (EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
    }
    else // Find the appropriate image entry
    {
        
        FrameEntries = NULL;
	    for( DWORD x=0; x<Header->NumberOfEntries; x++)
	    {
		    ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[x]);
		    if( stricmp(ImageEntry->Name,strImageName)==0 )
		    {
			    FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer + (EntryPointers[x]+sizeof(TA_GAF_ENTRY)));
			    break;
		    }
	    }

        // Check to see if the image entry was found
	    if( FrameEntries==NULL )
	    {
            // Image was not found
		    return FALSE;
	    }

    } // end else strImageName!=NULL

    // Check for an erraneous frame number or the default
    if( iFrameNumber<=0 )
    {
        // Set to the first frame
        iFrameNumber = 1;
    }
    // Check for an over bounds frame number
    else if( iFrameNumber>ImageEntry->NumberOfFrames )
    {
        // Set to the last frame;
        iFrameNumber = ImageEntry->NumberOfFrames;
    }

    // Get the frame data
    FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber-1].OffsetToFrameData);

    // Get the dimensions ans allocate the image buffer
	pImage->pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

    // Get the frame bits
    ta_GAF_GetPalettedImageBits(
        pFileBuffer + FrameData->OffsetToFrameData,
        FrameData->Width,
        FrameData->Height,
        FrameData->CompressionMethod,
        pImage->pBytes );

	// Set the image info
	pImage->Size.Set( FrameData->Width, FrameData->Height );
	pImage->Pitch = pImage->Size.width;
	pImage->Stride = 1;

	return TRUE;
}
// End gfx_Interface::RetrieveImageFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::RetrieveImagesFromGAF() //       \author Logan Jones
///////////////////////////////////////////           \date 10/26/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer    - 
//  LPCTSTR strImageName - 
//  int iStartFrame      - 
//  int iFrameCount      - 
//  gfx_Image_t* pImage  - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::RetrieveImagesFromGAF( BYTE* pFileBuffer, LPCTSTR strImageName, int iStartFrame, int iFrameCount, gfx_Image_t* pImage )
{
    LPTA_GAF_HEADER         Header;
    DWORD*                  EntryPointers;
    LPTA_GAF_ENTRY          ImageEntry = NULL;
    LPTA_GAF_FRAME_ENTRY    FrameEntries;
    LPTA_GAF_FRAME_DATA     FrameData;
	int						iFrameNumber;
	int						iEndFrame;
	int						NumCreated;

    // Sanity check
    assert( pFileBuffer && pImage );

    // Get the header and entry pointers
	Header = (LPTA_GAF_HEADER)pFileBuffer;
	EntryPointers = (DWORD*)(pFileBuffer + sizeof(TA_GAF_HEADER));

    // If an image name was not given, just use the first image that
    // occurs in the GAF
    if( strImageName==NULL )
    {
        ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[0]);
        FrameEntries = (LPTA_GAF_FRAME_ENTRY)(pFileBuffer + (EntryPointers[0]+sizeof(TA_GAF_ENTRY)));
    }
    else // Find the appropriate image entry
    {
        
        FrameEntries = NULL;
	    for( DWORD x=0; x<Header->NumberOfEntries; x++)
	    {
		    ImageEntry = (LPTA_GAF_ENTRY)(pFileBuffer + EntryPointers[x]);
		    if( stricmp(ImageEntry->Name,strImageName)==0 )
		    {
			    FrameEntries = (TA_GAF_FRAME_ENTRY*)(pFileBuffer + (EntryPointers[x]+sizeof(TA_GAF_ENTRY)));
			    break;
		    }
	    }

        // Check to see if the image entry was found
	    if( FrameEntries==NULL )
	    {
            // Image was not found
		    return FALSE;
	    }

    } // end else strImageName!=NULL

    // Check for an erraneous frame number or the default
    if( iStartFrame<=0 )
    {
        // Set to the first frame
        iStartFrame = 1;
    }
    // Check for an over bounds frame number
	else if( iStartFrame>ImageEntry->NumberOfFrames )
	{
        // Set to the last frame;
		iStartFrame = ImageEntry->NumberOfFrames;
	}

	// Check if there are enough frames
    if( (iStartFrame + iFrameCount - 1)>ImageEntry->NumberOfFrames )
    {
		// Work to the last frame
        iFrameCount = ImageEntry->NumberOfFrames - iStartFrame + 1;
    }

	// Iterate from the first desired frame until iFrameCount have been created
	iFrameNumber = iStartFrame;
	iEndFrame = iStartFrame + iFrameCount - 1;
	for( NumCreated=0; iFrameNumber<=iEndFrame; ++iFrameNumber,++pImage )
	{

		// Get the frame data
		FrameData = (LPTA_GAF_FRAME_DATA)(pFileBuffer + FrameEntries[iFrameNumber-1].OffsetToFrameData);

		// Get the dimensions ans allocate the image buffer
		pImage->pBytes = new BYTE[ FrameData->Width*FrameData->Height ];

		// Get the frame bits
		ta_GAF_GetPalettedImageBits(
			pFileBuffer + FrameData->OffsetToFrameData,
			FrameData->Width,
			FrameData->Height,
			FrameData->CompressionMethod,
			pImage->pBytes );

		pImage->Size.Set( FrameData->Width, FrameData->Height );
		pImage->Pitch = pImage->Size.width;
		pImage->Stride = 1;
		++NumCreated;

	} // end for( iFrameCount )

	return NumCreated;
}
// End gfx_Interface::RetrieveImagesFromGAF()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::ListModelTextures() //           \author Logan Jones
///////////////////////////////////////                \date 3/22/2002
//               
//====================================================================
// Return: BOOL - 
//
BOOL gfx_Interface::ListModelTextures()
{
	BOOL							bWorkingTextureFiles;
	app_FileTree::HFIND				hFind;
	app_FileTree::FindData_t		FindInfo;
	BYTE*							FileBuffer;
    LPTA_GAF_HEADER					Header;
    DWORD*							EntryPointers;
    LPTA_GAF_ENTRY					ImageEntry;

	// Iterate through each gaf in the textures directory
	hFind = theFileTree.Find( app_FileTree::FILES_WITH_EXTENSIONS, "gaf", "textures", &FindInfo );
	bWorkingTextureFiles = hFind!=NULL;
	while( bWorkingTextureFiles )
	{
		// Get the file and its header and entry pointers
		theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(FindInfo.hFile), &FileBuffer );
		Header = (LPTA_GAF_HEADER)FileBuffer;
		EntryPointers = (DWORD*)(FileBuffer + sizeof(TA_GAF_HEADER));

		// Iterate through each entry
		for( DWORD i=0; i<Header->NumberOfEntries; i++)
	    {
			// Add this texture to the map
		    ImageEntry = (LPTA_GAF_ENTRY)(FileBuffer + EntryPointers[i]);
			//TextureMap_t::iterator it = m_ModelTextures.find(ImageEntry->Name);
			//if( it!=m_ModelTextures.end() );
            
            // TODO: Casting a pointer to a 32bit int is *dangerous* ; FIX THIS
            ImageEntry->UnKnown_2 = (INT32)(size_t)FileBuffer;
            
			m_ModelTextures[ ImageEntry->Name ] = (BYTE*)ImageEntry;
	    }

		// Add the file buffer to the list and find next file
		m_ModelTextureFiles.push_back( FileBuffer );
		bWorkingTextureFiles = theFileTree.ContinueFind( hFind, &FindInfo );

	} // end while( gaf files )

	theFileTree.CloseFind( hFind );

	return TRUE;
}
// End gfx_Interface::ListModelTextures()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::DestroyModelTextureList() //     \author Logan Jones
/////////////////////////////////////////////          \date 3/22/2002
//               
//====================================================================
//
void gfx_Interface::DestroyModelTextureList()
{
	m_ModelTextures.clear();
	while( !m_ModelTextureFiles.empty() )
	{
		delete [] m_ModelTextureFiles.back();
		m_ModelTextureFiles.pop_back();
	}
}
// End gfx_Interface::DestroyModelTextureList()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_Interface.cpp //
////////////////////////////
