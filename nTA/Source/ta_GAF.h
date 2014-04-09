// ta_GAF.h //                                     \author Logan Jones
/////////////                                         \date 2/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_GAF_H_
#define _TA_GAF_H_
#pragma pack(1)
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NOTE: Most of the content in this file was derived from the document
//       ta-gaf-fmt.txt by Joe D.


/////////////////////////////////////////////////////////////////////
// The header at the beginning of ever GAF or TAF
typedef struct TA_GAF_HEADER
{
    // Version is always 0x00010100 for both TA and Kingdoms
	UINT32		Version;

    // Specifies the number of entries contained in this file
	UINT32		NumberOfEntries;

    // Presumably padding, its always 0x00000000
	INT32		Padding;

    // Immediately following this header is an array of pointers to each
    // entry in this file.
    //UINT32     EntryPointers[ NumberOfEntries ];

} *LPTA_GAF_HEADER;

// The standard version for all GAFs and TAFs
#define TA_GAF_VERSION_STANDARD					( 0x00010100 )


/////////////////////////////////////////////////////////////////////
// Each entry pointer points to a structure of this type.
typedef struct TA_GAF_ENTRY
{
    // Scecifies the number of graphical frames for this entry item
	WORD		NumberOfFrames;

    // This is always 0x0001
	short		UnKnown_1;

    // This is always 0x0000
	INT32		UnKnown_2;

    // The unique name of this entry, always padded to 32 characters long with 0
	char		Name[32];

    // Immediately following an entry is an array of frame entries
    //TA_GAF_FRAME_ENTRY    FrameEntries[ NumberOfFrames ];

} *LPTA_GAF_ENTRY;


/////////////////////////////////////////////////////////////////////
// This structure provides an offset to the frame data
typedef struct TA_GAF_FRAME_ENTRY
{
    // The offset into the file at which the frame's data resides
	UINT32		OffsetToFrameData;

    // This value seems to vary by a huge margin, perhaps it contains
    // bit flags used by CAVEDOG.
	INT32		Unknown_1;

} *LPTA_GAF_FRAME_ENTRY;


/////////////////////////////////////////////////////////////////////
//
typedef struct TA_GAF_FRAME_DATA
{
    // The final width and height of the frame in pixels
	WORD		Width;
	WORD		Height;

    // The X and Y offset of the frame when displayed. Used for centering the
    // frame or other various purposes. Sometimes just ignored.
	short		XOffset;
	short		YOffset;

    // This is always 0x09
	char		Unknown_1;

    // The compression flag for this frame
	BYTE		CompressionMethod;

    // Specifies the amount of subframes associated with this frame
	WORD		NumberOfSubFrames;

    // This is always 0x00000000
	INT32		Unknown_2;

    // If there are no sub frames, this points to the pixel data.
    // If there are sub frames, this points to an array of offsets to
    // the sb frame data structures.
	UINT32		OffsetToFrameData;

    // This seems to be another value that holds bit flags
	INT32		Unknown_3;

} *LPTA_GAF_FRAME_DATA;

// This flag inndicates that the frame is uncompressed. OffsetToFrameData points
// to an array of Width x Height bytes.
#define TA_GAF_FRAME_NOT_COMPRESSED						( 0 )

// This flag inndicates that the frame is compressed using the compression
// scheme used for TA and Kingdoms
#define TA_GAF_FRAME_COMPRESSED_TA						( 1 )

// This flag inndicates that the frame is compressed using the compression
// scheme used for Kingdoms TAF files ending in "*_4444.TAF"
#define TA_GAF_FRAME_COMPRESSED_TAK1					( 4 )

// This flag inndicates that the frame is compressed using the compression
// scheme used for Kingdoms TAF files ending in "*_1555.TAF"
#define TA_GAF_FRAME_COMPRESSED_TAK2					( 5 )


#pragma pack()
/////////////////////////////////////////////////////////////////////
// GAF utility functions


//////////////////////////////////////////////////////////////////////
// ta_GAF_GetEntryPointer() //                     \author Logan Jones
/////////////////////////////                          \date 6/18/2001
// Description:  
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer    - Pointer to a buffer containing the GAF file
//  LPCTSTR strEntryName - The name of the desired entry or NULL if the first
//                         is desired
//
// Return: inline UINT32 - Returns the offset in the file at which the entry
//                        is located, or returns 0 if the entry does not exist
//
inline UINT32 ta_GAF_GetEntryPointer( BYTE* pFileBuffer, LPCTSTR strEntryName=NULL )
{
	LPTA_GAF_HEADER				pHeader;
	UINT32*						pEntryPointers;
	LPTA_GAF_ENTRY				pEntry;
	UINT32						index;

	// Sanity check
	assert( pFileBuffer );

	// Get the header and the entry pointers
	pHeader = (LPTA_GAF_HEADER)pFileBuffer;
	pEntryPointers = (UINT32*)(pFileBuffer + sizeof(TA_GAF_HEADER));

	// Check which entry is desired
	if( strEntryName==NULL )
	{
		// Just return the first entry pointer
		return pEntryPointers[0];
	}
	else
	{
		// Iterate through each entry
		for( index=0; index<pHeader->NumberOfEntries; index++)
		{
			// Get the entry from the file
			pEntry = (LPTA_GAF_ENTRY)(pFileBuffer + pEntryPointers[index]);

			// Check if the is the desired entry
			if( stricmp( pEntry->Name, strEntryName )==0 )
				return pEntryPointers[index];

		} // end for( NumberOfEntries )
	}

	// Entry not found
	return 0;
}
// End ta_GAF_GetEntryPointer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ta_GAF_GetPalettedImageBits() //                \author Logan Jones
//////////////////////////////////                     \date 3/26/2001
// Description:  Fills the destination buffer with the given frame data.
//               Does NOT process subframes.
//====================================================================
// Parameters:
//  BYTE* pFileBuffer  - Pointer to a buffer holding the compressed image
//  WORD wWidth        - Width of the image in pixels
//  WORD wHeight       - Height of the image in pixels
//  BYTE bfCompression - Compression flag
//  BYTE* pImageBits   - Destination buffer for the decompressed image
//  BYTE byClearIndex  - Color index to use for the transparent pixels ( if any )
//
// Return: BOOL - TRUE for success, FALSE for failure
//
inline BOOL ta_GAF_GetPalettedImageBits( BYTE* pFileBuffer, WORD wWidth, WORD wHeight, BYTE bfCompression, BYTE* pImageBits, BYTE byClearIndex=0 )
{
    WORD        LineLength;
    BYTE*       pLineBits;
	UINT32       Texindex=0;
	BYTE        ByteMask;
	BYTE        ColorIndex;
    WORD        linecount;
	WORD        count;
    WORD        pos;
    //INT32        k;

    // Sanity checks
    assert( pFileBuffer&&pImageBits );

    // Proceed according to the compression flag
    switch( bfCompression )
    {

    case TA_GAF_FRAME_NOT_COMPRESSED:
        // Image is not compessed, the file data is just raw pixel indeces
        // Just copy from one buffer to the other
        memcpy( pImageBits, pFileBuffer, wWidth*wHeight );
        break;
    // end case TA_GAF_FRAME_NOT_COMPRESSED

    case TA_GAF_FRAME_COMPRESSED_TA:
        // Decompress into pImageBits one line at a time
	    for( linecount=0; linecount<wHeight; linecount++)
	    {
            // Get the length of the compressed line
		    LineLength = *((WORD*)pFileBuffer);
			assert( LineLength<=(wWidth*2) );
		    pLineBits = pFileBuffer + sizeof(WORD);
            
		    for( pos=0; pos<(LineLength); )
		    {
                // The first byte is the mask
			    ByteMask = pLineBits[pos++];

                // Check for transparent pixels
			    if((ByteMask & 0x01) == 0x01)
			    {
				    count = (ByteMask >> 1);
                    memset( pImageBits + Texindex, byClearIndex, count );
				    Texindex += count;
			    }
                // Check for a color run
			    else if((ByteMask & 0x02) == 0x02)
			    {
				    ColorIndex = pLineBits[pos++];
				    count = (ByteMask >> 2) + 1;
                    memset( pImageBits + Texindex, ColorIndex, count );
		    		Texindex += count;
	    		}
    			else // Just take the next count pixels
			    {
			    	count = (ByteMask >> 2) + 1;
                    memcpy( pImageBits + Texindex, pLineBits + pos, count );
    				pos += count;
				    Texindex += count;
			    }

		    } // End for( line length )

            // Move to the next line;
            pFileBuffer += (sizeof(WORD) + LineLength);

    	} // End for( num lines )
        break;
    // end case TA_GAF_FRAME_COMPRESSED_TA

    case TA_GAF_FRAME_COMPRESSED_TAK1:
        // Not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK1

    case TA_GAF_FRAME_COMPRESSED_TAK2:
        // not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK2

    default:
        // Unknown compression type
        assert( !"Encountered unknown GAF compression type" );
        return FALSE;

    } // end switch( bfCompression )

    return TRUE;
}
// End ta_GAF_GetPalettedImageBits()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ta_GAF_InsertPalettedImageBits() //             \author Logan Jones
/////////////////////////////////////                  \date 6/25/2001
// Description:  Inserts the specified frame data into a given image
//               buffer
//====================================================================
// Parameters:
//  BYTE* pFileBuffer              - Pointer to a buffer holding the GAF file
//  LPTA_GAF_FRAME_DATA pSrcFrame  - Pointer to the source frame data structure
//  LPTA_GAF_FRAME_DATA pDestFrame - Pointer to the destination frame data structure
//  BYTE* pDestBits                - Destination buffer for the decompressed image
//
// Return: BOOL - TRUE for success, FALSE for failure
//
inline BOOL ta_GAF_InsertPalettedImageBits( BYTE* pFileBuffer, LPTA_GAF_FRAME_DATA pSrcFrame, LPTA_GAF_FRAME_DATA pDestFrame, BYTE* pDestBits )
{
	BYTE*		pDest;
	WORD		LineLength;
	BYTE*		pLineBits;
	UINT32		Texindex;
	BYTE		ByteMask;
	BYTE		ColorIndex;
	WORD		linecount;
	WORD		count;
	WORD		pos;

    // Sanity checks
    assert( pFileBuffer && pSrcFrame && pDestFrame && pDestBits );

	// Move to the offset within the destination image to start copying to
	pDest = pDestBits + (((pDestFrame->YOffset - pSrcFrame->YOffset) * pDestFrame->Width) + (pDestFrame->XOffset - pSrcFrame->XOffset));

    // Proceed according to the compression flag
    switch( pSrcFrame->CompressionMethod )
    {

    case TA_GAF_FRAME_NOT_COMPRESSED:
        // Image is not compessed, the file data is just raw pixel indeces
        // Just copy from one buffer to the other
		pFileBuffer = pFileBuffer + pSrcFrame->OffsetToFrameData;
		for( linecount=0; linecount<pSrcFrame->Width; linecount++)
		{
			memcpy( pDest, pFileBuffer, pSrcFrame->Width );
			pFileBuffer += pSrcFrame->Width;
			pDest += pDestFrame->Width;
		}
        break;
    // end case TA_GAF_FRAME_NOT_COMPRESSED

    case TA_GAF_FRAME_COMPRESSED_TA:
		//
		pFileBuffer = pFileBuffer + pSrcFrame->OffsetToFrameData;

        // Decompress into pImageBits one line at a time
	    for( linecount=0; linecount<pSrcFrame->Height; linecount++)
	    {
            // Get the length of the compressed line
		    LineLength = *((WORD*)pFileBuffer);
		    pLineBits = pFileBuffer + sizeof(WORD);

			// A new line has started
			Texindex = 0;
            
		    for( pos=0; pos<(LineLength); )
		    {

                // The first byte is the mask
			    ByteMask = pLineBits[pos++];

                // Check for transparent pixels
			    if((ByteMask & 0x01) == 0x01)
			    {
				    count = (ByteMask >> 1);
				    Texindex += count;
			    }
                // Check for a color run
			    else if((ByteMask & 0x02) == 0x02)
			    {
				    ColorIndex = pLineBits[pos++];
				    count = (ByteMask >> 2) + 1;
                    memset( pDest + Texindex, ColorIndex, count );
		    		Texindex += count;
	    		}
    			else // Just take the next count pixels
			    {
			    	count = (ByteMask >> 2) + 1;
                    memcpy( pDest + Texindex, pLineBits + pos, count );
    				pos += count;
				    Texindex += count;
			    }

		    } // End for( line length )

			// Move the destination image to the next line
			pDest += pDestFrame->Width;

            // Move to the next line;
            pFileBuffer += (sizeof(WORD) + LineLength);

    	} // End for( num lines )

        break;
    // end case TA_GAF_FRAME_COMPRESSED_TA

    case TA_GAF_FRAME_COMPRESSED_TAK1:
        // Not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK1

    case TA_GAF_FRAME_COMPRESSED_TAK2:
        // not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK2

    default:
        // Unknown compression type
        assert( !"Encountered unknown GAF compression type" );
        return FALSE;

    } // end switch( bfCompression )

    return TRUE;
}
// End ta_GAF_InsertPalettedImageBits()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ta_GAF_GetPalettedFrameBits() //                \author Logan Jones
//////////////////////////////////                     \date 6/25/2001
// Description:  Fills the given buffer with the final results of the
//               given frame. Use this function instead of 
//               ta_GAF_GetPalettedImageBits() if there is a possibility
//               that the given frame has subframes
//====================================================================
// Parameters:
//  BYTE* pFileBuffer              - Pointer to a buffer holding the GAF file
//  LPTA_GAF_FRAME_DATA pFrameData - Pointer to the frame data structure
//  BYTE* pImageBits               - Destination buffer for the decompressed image
//  BYTE byClearIndex              - Color index to use for the transparent pixels ( if any )
//
// Return: BOOL - TRUE for success, FALSE for failure
//
inline BOOL ta_GAF_GetPalettedFrameBits( BYTE* pFileBuffer, LPTA_GAF_FRAME_DATA pFrameData, BYTE* pImageBits, BYTE byClearIndex=0 )
{
    WORD        LineLength;
    BYTE*       pLineBits;
	UINT32       Texindex=0;
	BYTE        ByteMask;
	BYTE        ColorIndex;
    WORD        linecount;
	WORD        count;
    WORD        pos;
    //INT32        k;

    // Sanity checks
    assert( pFileBuffer&&pFrameData&&pImageBits );

	// Check if there are subframes
	if( pFrameData->NumberOfSubFrames>0 )
	{
		// Clear the image to transparent
		memset( pImageBits, byClearIndex, pFrameData->Width*pFrameData->Height );

		// Get the offsets to the subframes
		UINT32* SubFramePointer = (UINT32*)(pFileBuffer + pFrameData->OffsetToFrameData);

		// Insert each subframe into the image
		for( count=pFrameData->NumberOfSubFrames; count>0; count--,SubFramePointer++)
			ta_GAF_InsertPalettedImageBits(
				pFileBuffer,
				(LPTA_GAF_FRAME_DATA)(pFileBuffer + *SubFramePointer),
				pFrameData,
				pImageBits );

	} // end if( NumberOfSubFrames )
	else
    // Proceed according to the compression flag
    switch( pFrameData->CompressionMethod )
    {

    case TA_GAF_FRAME_NOT_COMPRESSED:
        // Image is not compessed, the file data is just raw pixel indeces
        // Just copy from one buffer to the other
        memcpy( pImageBits, pFileBuffer + pFrameData->OffsetToFrameData, pFrameData->Width*pFrameData->Height );
        break;
    // end case TA_GAF_FRAME_NOT_COMPRESSED

    case TA_GAF_FRAME_COMPRESSED_TA:
        // Decompress into pImageBits one line at a time
		pFileBuffer = pFileBuffer + pFrameData->OffsetToFrameData;
	    for( linecount=0; linecount<pFrameData->Height; linecount++)
	    {
            // Get the length of the compressed line
		    LineLength = *((WORD*)pFileBuffer);
			assert( LineLength<=(pFrameData->Width*2) );
		    pLineBits = pFileBuffer + sizeof(WORD);
            
		    for( pos=0; pos<(LineLength); )
		    {
                // The first byte is the mask
			    ByteMask = pLineBits[pos++];

                // Check for transparent pixels
			    if((ByteMask & 0x01) == 0x01)
			    {
				    count = (ByteMask >> 1);
                    memset( pImageBits + Texindex, byClearIndex, count );
				    Texindex += count;
			    }
                // Check for a color run
			    else if((ByteMask & 0x02) == 0x02)
			    {
				    ColorIndex = pLineBits[pos++];
				    count = (ByteMask >> 2) + 1;
                    memset( pImageBits + Texindex, ColorIndex, count );
		    		Texindex += count;
	    		}
    			else // Just take the next count pixels
			    {
			    	count = (ByteMask >> 2) + 1;
                    memcpy( pImageBits + Texindex, pLineBits + pos, count );
    				pos += count;
				    Texindex += count;
			    }

		    } // End for( line length )

            // Move to the next line;
            pFileBuffer += (sizeof(WORD) + LineLength);

    	} // End for( num lines )
        break;
    // end case TA_GAF_FRAME_COMPRESSED_TA

    case TA_GAF_FRAME_COMPRESSED_TAK1:
        // Not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK1

    case TA_GAF_FRAME_COMPRESSED_TAK2:
        // not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK2

    default:
        // Unknown compression type
        assert( !"Encountered unknown GAF compression type" );
        return FALSE;

    } // end switch( pFrameData->CompressionMethod )

    return TRUE;
}
// End ta_GAF_GetPalettedFrameBits()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// ta_GAF_GetImageBits() //                        \author Logan Jones
//////////////////////////                             \date 3/26/2001
// Description:  
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer  - Pointer to a buffer holding the compressed image
//  WORD wWidth        - Width of the image in pixels
//  WORD wHeight       - Height of the image in pixels
//  BYTE bfCompression - Compression flag
//  UINT32* pImageBits  - Destination buffer for the decompressed image
//  UINT32* pPalette    - Pointer to a buffer containing the image palette
//  UINT32 dwClearColor - Color value to use for the transparent pixels ( if any )
//
// Return: BOOL - TRUE for success, FALSE for failure
//
inline BOOL ta_GAF_GetImageBits( BYTE* pFileBuffer, WORD wWidth, WORD wHeight, BYTE bfCompression, UINT32* pImageBits, UINT32* pPalette, UINT32 dwClearColor=0 )
{
    WORD        LineLength;
    BYTE*       pLineBits;
	UINT32       Texindex=0;
	BYTE        ByteMask;
	BYTE        ColorIndex;
    WORD        linecount;
	WORD        count;
    WORD        pos;
    INT32        k;

    // Sanity checks
    assert( pFileBuffer&&pImageBits&&pPalette );

    // Proceed according to the compression flag
    switch( bfCompression )
    {

    case TA_GAF_FRAME_NOT_COMPRESSED:
        // Image is not compessed, the file data is just raw pixel indeces
        // Just copy from one buffer to the other
        for( k=((wWidth*wHeight)-1); k>=0; k--)
            pImageBits[k] = pPalette[ pFileBuffer[k] ];
        break;
    // end case TA_GAF_FRAME_NOT_COMPRESSED

    case TA_GAF_FRAME_COMPRESSED_TA:
        // Decompress into pImageBits one line at a time
	    for( linecount=0; linecount<wHeight; linecount++)
	    {
            // Get the length of the compressed line
		    LineLength = *((WORD*)pFileBuffer);
		    pLineBits = pFileBuffer + sizeof(WORD);
            
		    for( pos=0; pos<(LineLength); )
		    {
                // The first byte is the mask
			    ByteMask = pLineBits[pos++];

                // Check for transparent pixels
			    if((ByteMask & 0x01) == 0x01)
			    {
				    count = (ByteMask >> 1);
                    for( k=0; k<count; k++)
                        pImageBits[ Texindex+k ] = dwClearColor;
				    Texindex += count;
			    }
                // Check for a color run
			    else if((ByteMask & 0x02) == 0x02)
			    {
				    ColorIndex = pLineBits[pos++];
				    count = (ByteMask >> 2) + 1;
				    for( k=0; k<count; k++)
			    		pImageBits[ Texindex+k ] = pPalette[ ColorIndex ];
		    		Texindex += count;
	    		}
    			else // Just take the next count pixels
			    {
			    	count = (ByteMask >> 2) + 1;
		    		for( k=0; k<count; k++)
	    				pImageBits[ Texindex+k ] = pPalette[ pLineBits[pos+k] ];
    				pos += count;
				    Texindex += count;
			    }

		    } // End for( line length )

            // Move to the next line;
            pFileBuffer += (sizeof(WORD) + LineLength);

    	} // End for( num lines )
        break;
    // end case TA_GAF_FRAME_COMPRESSED_TA

    case TA_GAF_FRAME_COMPRESSED_TAK1:
        // Not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK1

    case TA_GAF_FRAME_COMPRESSED_TAK2:
        // not implemented yet, proceed to default
    // end case TA_GAF_FRAME_COMPRESSED_TAK2

    default:
        // Unknown compression type
        assert( !"Encountered unknown GAF compression type!" );
        return FALSE;

    } // end switch( bfCompression )

    return TRUE;
}
// End ta_GAF_GetImageBits()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_TA_GAF_H_)
