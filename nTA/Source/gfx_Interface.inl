// gfx_Interface.inl //                            \author Logan Jones
//////////////////////                                \date 3/19/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::GetTriangleCount() //            \author Logan Jones
//////////////////////////////////////                 \date 3/19/2002
//               
//====================================================================
// Return: DWORD - 
//
DWORD gfx_Interface::GetTriangleCount() const
{
	return m_TriangleCount;
}
// End gfx_Interface::GetTriangleCount()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateSurfaceFromPCX() //        \author Logan Jones
//////////////////////////////////////////             \date 10/5/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  DWORD dwFileBufferSize  - 
//  DWORD dwFlags           - 
//  gfx_Surface** ppSurface - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateSurfaceFromPCX( BYTE* pFileBuffer, DWORD dwFileBufferSize, DWORD dwFlags, gfx_Surface** ppSurface )
{
    // The PCX file header
    struct PCX_HEADER
    {
    	BYTE			Manufacturer;
    	BYTE			Version;
    	BYTE			Encoding;
    	BYTE			BitsPerPicel;
    	struct
    	{	UINT16	XMin;
    		UINT16	YMin;
    		UINT16	XMax;
    		UINT16	YMax;
    	}				Window;
    	UINT16			HDpi;
    	UINT16			VDpi;
    	BYTE			Colormap[48];
    	BYTE			Reserved;
    	BYTE			NPlanes;
    	UINT16			BytesPerLine;
    	UINT16			PaletteInfo;
    	UINT16			HscreenSize;
    	UINT16			VscreenSize;
    	BYTE			Filler[54];
    };

    PCX_HEADER* pHeader;
	gfx_Image_t	Image;
    BYTE*       pPCXPalette;
    BYTE*       pPCXImage;
    WORD        x,y;
    BOOL        bRet;

    // Sanity check
    assert( pFileBuffer );

    // Get the pcx header
    pHeader = (PCX_HEADER*)pFileBuffer;

    // Get the image dimensions
    Image.Size.width = pHeader->Window.XMax - pHeader->Window.XMin + 1;
	Image.Size.height= pHeader->Window.YMax - pHeader->Window.YMin + 1;

    // Get the palette
	pPCXPalette = pFileBuffer + (dwFileBufferSize - 769);

    // Make sure its valid
	if( *(pPCXPalette)!=0x0C )
        return FALSE;
    else pPCXPalette++;

    // Get a pointer to the PCX image data
    pPCXImage = pFileBuffer + 128;

    // Allocate a buffer to hold the raw decompressed image
	BYTE* RawImg = new BYTE[ pHeader->BytesPerLine*Image.Size.height ];

    // Decompress the PCX data
	BYTE* Raw = RawImg;
	BYTE byte; int count;
	for( y=0; y<Image.Size.height; y++,Raw+=pHeader->BytesPerLine)
	{
		for( x=0; x<Image.Size.width; )
		{
			byte = *pPCXImage;
            pPCXImage++;
			if( 0xC0==(0xC0&byte) )
			{
		  		count = 0x3F & byte;
				byte = *pPCXImage;
                pPCXImage++;
				while(count--) Raw[x++] = byte;
			}
			else Raw[x++] = byte;
		}
	}

	if( dwFlags & GFX_FORCE_CURRENT_PALETTE )
	{
		Image.pBytes = RawImg;
		Image.Pitch = Image.Size.width;
		Image.Stride = 1;		
		bRet = CreateSurface( PIXEL_PALETTISED, &Image, ppSurface );
		delete [] RawImg;
	}
	else
	{
        BYTE* FinalImage = new BYTE[ *Image.Size * 3 ];
        BYTE* pRaw = RawImg;
        BYTE* pFinal = FinalImage;
        for( y=0; y<Image.Size.height; y++)
        {
            for( x=0; x<Image.Size.width; x++,pFinal+=3)
            {
                pFinal[0] = *(pPCXPalette+((*(pRaw+x))*3)+0);
                pFinal[1] = *(pPCXPalette+((*(pRaw+x))*3)+1);
                pFinal[2] = *(pPCXPalette+((*(pRaw+x))*3)+2);
            }
            pRaw += pHeader->BytesPerLine;
        }
        delete [] RawImg;
		Image.pBytes = FinalImage;
		Image.Stride = 3;		
		Image.Pitch = Image.Size.width * 3;
		bRet = CreateSurface( PIXEL_RGB, &Image, ppSurface );
        delete [] FinalImage;
	}

	return bRet;
}
// End gfx_Interface::CreateSurfaceFromPCX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateSurfaceFromPCX() //        \author Logan Jones
//////////////////////////////////////////             \date 10/9/2001
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer       - 
//  DWORD dwFileBufferSize  - 
//  DWORD dwFlags           - 
//  std_Size_t& szDesired   - 
//  gfx_Surface** ppSurface - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateSurfaceFromPCX( BYTE* pFileBuffer, DWORD dwFileBufferSize, DWORD dwFlags, const std_Size_t& szDesired, gfx_Surface** ppSurface )
{
    // The PCX file header
    struct PCX_HEADER
    {
    	BYTE			Manufacturer;
    	BYTE			Version;
    	BYTE			Encoding;
    	BYTE			BitsPerPicel;
    	struct
    	{	UINT16	XMin;
    		UINT16	YMin;
    		UINT16	XMax;
    		UINT16	YMax;
    	}				Window;
    	UINT16			HDpi;
    	UINT16			VDpi;
    	BYTE			Colormap[48];
    	BYTE			Reserved;
    	BYTE			NPlanes;
    	UINT16			BytesPerLine;
    	UINT16			PaletteInfo;
    	UINT16			HscreenSize;
    	UINT16			VscreenSize;
    	BYTE			Filler[54];
    };

    PCX_HEADER* pHeader;
	gfx_Image_t	Image;
    BYTE*       pPCXPalette;
    BYTE*       pPCXImage;
    long        x,y;
	BYTE*		RawImg,*pRaw;
	BYTE*		FinalImage,*pFinal;
    BOOL        bRet;

    // Sanity check
    assert( pFileBuffer );

    // Get the pcx header
    pHeader = (PCX_HEADER*)pFileBuffer;

    // Get the image dimensions
    Image.Size.width = pHeader->Window.XMax - pHeader->Window.XMin + 1;
	Image.Size.height= pHeader->Window.YMax - pHeader->Window.YMin + 1;

    // Get the palette
	pPCXPalette = pFileBuffer + (dwFileBufferSize - 769);

    // Make sure its valid
	if( *(pPCXPalette)!=0x0C )
        return FALSE;
    else pPCXPalette++;

    // Get a pointer to the PCX image data
    pPCXImage = pFileBuffer + 128;

    // Allocate a buffer to hold the raw decompressed image
	RawImg = new BYTE[ pHeader->BytesPerLine*Image.Size.height ];

    // Decompress the PCX data
	BYTE* Raw = RawImg;
	BYTE byte; int count;
	for( y=0; y<Image.Size.height; y++,Raw+=pHeader->BytesPerLine)
	{
		for( x=0; x<Image.Size.width; )
		{
			byte = *pPCXImage;
            pPCXImage++;
			if( 0xC0==(0xC0&byte) )
			{
		  		count = 0x3F & byte;
				byte = *pPCXImage;
                pPCXImage++;
				while(count--) Raw[x++] = byte;
			}
			else Raw[x++] = byte;
		}
	}

	if( dwFlags & GFX_FORCE_CURRENT_PALETTE )
	{
		FinalImage = new BYTE[ *szDesired ];
        pRaw = RawImg;
        pFinal = FinalImage;
        for( y=0; y<szDesired.height; ++y)
        {
            for( x=0; x<szDesired.width; ++x,++pFinal)
				*pFinal = pRaw[ x ] ;
            pRaw += pHeader->BytesPerLine;
        }
        delete [] RawImg;
		Image.pBytes = FinalImage;
		Image.Size = szDesired;
		Image.Pitch = Image.Size.width;
		Image.Stride = 1;		
		bRet = CreateSurface( PIXEL_PALETTISED, &Image, ppSurface );
		delete [] FinalImage;
	}
	else
	{
		FinalImage = new BYTE[ *szDesired * 3 ];
        pRaw = RawImg;
        pFinal = FinalImage;
        for( y=0; y<szDesired.height; ++y)
        {
            for( x=0; x<szDesired.width; ++x,pFinal+=3)
            {
				const BYTE* pPalEntry = pPCXPalette + (pRaw[ x ] * 3);
                pFinal[0] = pPalEntry[ 0 ];
                pFinal[1] = pPalEntry[ 1 ];
                pFinal[2] = pPalEntry[ 2 ];
            }
            pRaw += pHeader->BytesPerLine;
        }
        delete [] RawImg;
		Image.pBytes = FinalImage;
		Image.Size = szDesired;
		Image.Pitch = Image.Size.width * 3;
		Image.Stride = 3;		
		bRet = CreateSurface( PIXEL_RGB, &Image, ppSurface );
        delete [] FinalImage;
	}

	return bRet;
}
// End gfx_Interface::CreateSurfaceFromPCX()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::CreateFontFromFNT() //           \author Logan Jones
///////////////////////////////////////                \date 4/19/2002
//               
//====================================================================
// Parameters:
//  BYTE* pFileBuffer - 
//  gfx_Font** ppFont - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::CreateFontFromFNT( BYTE* pFileBuffer, gfx_Font** ppFont )
{
	WORD						LineCount;
	WORD*						pIndexBuffer;
    BYTE						FontTexture[256*256];
	BYTE*						pEntry;
	gfx_Interface::FontEntry	FontEntries[ 256 ];
	gfx_Image_t					Image;

	// Get the line count for each font and the index buffer for the font data
	LineCount = *((WORD*)pFileBuffer);
	pIndexBuffer = (WORD*)(pFileBuffer + 4);

	memset( FontTexture, 0, sqr(256) );
	int		Line,Col,Bit,Byte;
	BYTE*	pMap;
	BYTE*	MapOffset = FontTexture;
	for( int i=0,num=0; i<256; ++i,++num )
	{
		if( num==16 )
		{
			MapOffset += (256*15);
			num = 0;
		}

		if( pIndexBuffer[i] )
		{
			pMap = MapOffset;
			pEntry = pFileBuffer + pIndexBuffer[i];
			for( Line=0,Col=0,Bit=7,Byte=1; Line<LineCount; )
			{
				if( pEntry[Byte]&(1<<Bit) ) pMap[Col]=255; else pMap[Col]=0;
				if( ++Col==pEntry[0] )
				{
					pMap += 256;
					Col = 0;
					++Line;
				}
				if( Bit==0 ) {Bit = 7; ++Byte;}
				else --Bit;
			}

			FontEntries[ i ].Size.Set( *pEntry, LineCount );
			FontEntries[ i ].Offset.Set( 0, LineCount-3 );
		}

		MapOffset += 16;
	}

    Image.pBytes = FontTexture;
	Image.Pitch = 256;
	Image.Stride = 1;
	Image.Size = std_Size_t( 256, 256 );
	return CreateFont( FontEntries, std_Size_t(16,16), PIXEL_PALETTISED, &Image, ppFont );
}
// End gfx_Interface::CreateFontFromFNT()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// gfx_Interface::BitBlt() //                      \author Logan Jones
////////////////////////////                          \date 10/26/2001
//               
//====================================================================
// Parameters:
//  std_Point_t ptDestStart - 
//  gfx_Image_t* pDestImage - 
//  gfx_Image_t* pSrcImage  - 
//  std_Rect_t* pSrcRect    - 
//
// Return: BOOL - 
//
BOOL gfx_Interface::BitBlt( std_Point_t ptDestStart, gfx_Image_t* pDestImage, gfx_Image_t* pSrcImage, std_Rect_t* pSrcRect )
{
	std_Rect_t		SrcRect( 0, 0, pSrcImage->Size );
	BYTE*			pStart;
	BYTE*			pSrc;

	if( pSrcRect==NULL )
		pSrcRect = &SrcRect;

	pStart = pDestImage->pBytes + ( (ptDestStart.x * pDestImage->Stride) + (ptDestStart.y * pDestImage->Pitch) );
	pSrc = pSrcImage->pBytes + ( (pSrcRect->left * pSrcImage->Stride) + (pSrcRect->top * pSrcImage->Pitch) );
	const long CopyWidth = pSrcRect->Width() * pSrcImage->Stride;

	for( long y=pSrcRect->bottom; y>pSrcRect->top; --y)
	{
		// Copy the line
		memcpy( pStart, pSrc, CopyWidth );

		// Move to the next line
		pStart += pDestImage->Pitch;
		pSrc += pSrcImage->Pitch;
	}

	return TRUE;
}
// End gfx_Interface::BitBlt()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - gfx_Interface.inl //
////////////////////////////
