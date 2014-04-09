// snd_DSound8.cpp //                              \author Logan Jones
////////////////////                                  \date 9/27/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "app.h"
#include "snd.h"
#include "snd_DSound8.h"


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
snd_DSound8::snd_DSound8():
	m_lpDS( NULL )
{}
snd_DSound8::~snd_DSound8()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::Create() //                        \author Logan Jones
//////////////////////////                             \date 9/27/2001
//               
//====================================================================
// Parameters:
//  std_PlatformParameters* pPlatformParams - 
//
// Return: BOOL - 
//
BOOL snd_DSound8::Create( std_PlatformParameters* pPlatformParams )
{
	if( FAILED(DirectSoundCreate8(NULL, &m_lpDS, NULL)) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to create the DirectSound8 interface object." );
		return FALSE;
	}

	if( FAILED(m_lpDS->SetCooperativeLevel(pPlatformParams->hMainWindow,DSSCL_PRIORITY)) )
	{
		theApp.Console.Comment( CT_ERROR, "ERROR: Failed to set the desired coop level for the sound interface." );
		return FALSE;
	}

	return TRUE;
}
// End snd_DSound8::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::Destroy() //                       \author Logan Jones
///////////////////////////                            \date 9/27/2001
//               
//====================================================================
//
void snd_DSound8::Destroy()
{
	DestroyAllSounds();
	SAFE_RELEASE( m_lpDS );
}
// End snd_DSound8::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::PlaySound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSound - 
//
// Return: BOOL - 
//
BOOL snd_DSound8::PlaySound( LPCTSTR strSound )
{
	char				Path[ 128 ];

	sprintf( Path, "sounds/%s.wav", strSound );
	return PlaySound( theFileTree.OpenFile( Path ) );		
}
// End snd_DSound8::PlaySound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::PlaySound() //                     \author Logan Jones
/////////////////////////////                          \date 4/15/2002
//               
//====================================================================
// Parameters:
//  LPVOID hSoundFile - 
//
// Return: BOOL - 
//
BOOL snd_DSound8::PlaySound( LPVOID hSoundFile )
{
	BOOL				bReturn = FALSE;
	BYTE*				pBuf;
	RIFF_CHUNK_INFO_t*	pInfo;
	WAVEFORMATEX		WaveFormat;
	BYTE*				pData;
	DSBUFFERDESC		dsbd;
	LPDIRECTSOUNDBUFFER	NewSound;
	LPVOID				lpLockedBuf;
	DWORD				SizeLocked;

	BEGIN_CODE_BLOCK

		if( hSoundFile==NULL ) return FALSE;

		SoundList_t::iterator it = m_ActiveSounds.find( hSoundFile );
		if( it!=m_ActiveSounds.end() )
		{
			((*it).second)->GetStatus( &SizeLocked );
			if( !(SizeLocked & DSBSTATUS_PLAYING) )
			{
				((*it).second)->Play( 0, 0, 0 );
				return TRUE;
			}
		}
		else TrimSounds();

		theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(hSoundFile), &pBuf );

		pInfo = (RIFF_CHUNK_INFO_t*)pBuf;
		assert( (pInfo->fccID==FOURCC_RIFF)&&(pInfo->fccType==FOURCC_WAVE) );

		pInfo = (RIFF_CHUNK_INFO_t*)(pBuf + sizeof(RIFF_CHUNK_INFO_t));
		assert( (pInfo->fccID==FOURCC_FMT)&&(pInfo->Size>=16) );

		memcpy( &WaveFormat, pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8), sizeof(WAVEFORMATEX) );
		WaveFormat.cbSize = 0;
		assert( WaveFormat.wFormatTag==WAVE_FORMAT_PCM );

		pData = pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8 + pInfo->Size + 8);
		pInfo = (RIFF_CHUNK_INFO_t*)(pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8 + pInfo->Size));

		ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
		dsbd.dwSize          = sizeof(DSBUFFERDESC);
		dsbd.dwFlags         = 0;
		dsbd.dwBufferBytes   = pInfo->Size;
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.lpwfxFormat     = &WaveFormat;

		if( FAILED(m_lpDS->CreateSoundBuffer( &dsbd, &NewSound, NULL )) )
		{
			EXIT_CODE_BLOCK
		}

		if( FAILED(NewSound->Lock( 0, pInfo->Size, &lpLockedBuf, &SizeLocked, NULL, NULL, 0 )) )
		{
			EXIT_CODE_BLOCK
		}

		memcpy( lpLockedBuf, pData, SizeLocked );
		NewSound->Unlock( lpLockedBuf, SizeLocked, NULL, 0 );

		NewSound->Play( 0, 0, 0 );

		m_ActiveSounds[ hSoundFile ] = NewSound;

		bReturn = TRUE;

	END_CODE_BLOCK

	delete [] pBuf;
	return bReturn;
}
// End snd_DSound8::PlaySound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::PlayAmbientSound() //              \author Logan Jones
////////////////////////////////////                  \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSound        - 
//  LPSOUND* ppAmbientSound - 
//
// Return: BOOL - 
//
BOOL snd_DSound8::PlayAmbientSound( LPCTSTR strSound, LPSOUND* ppAmbientSound )
{
	char				Path[ 128 ];

	sprintf( Path, "sounds/%s.wav", strSound );
	return PlayAmbientSound( theFileTree.OpenFile(Path), ppAmbientSound );
}
// End snd_DSound8::PlayAmbientSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::PlayAmbientSound() //              \author Logan Jones
////////////////////////////////////                   \date 4/22/2002
//               
//====================================================================
// Parameters:
//  LPVOID hSoundFile       - 
//  LPSOUND* ppAmbientSound - 
//
// Return: BOOL - 
//
BOOL snd_DSound8::PlayAmbientSound( LPVOID hSoundFile, LPSOUND* ppAmbientSound )
{
	BOOL				bReturn = FALSE;
	BYTE*				pBuf;
	RIFF_CHUNK_INFO_t*	pInfo;
	WAVEFORMATEX		WaveFormat;
	BYTE*				pData;
	DSBUFFERDESC		dsbd;
	LPDIRECTSOUNDBUFFER	NewSound;
	LPVOID				lpLockedBuf;
	DWORD				SizeLocked;

	BEGIN_CODE_BLOCK

		if( hSoundFile==NULL ) return FALSE;

		SoundList_t::iterator it = m_ActiveSounds.find( hSoundFile );
		if( it!=m_ActiveSounds.end() )
		{
			((*it).second)->Play( 0, 0, DSBPLAY_LOOPING );
			return TRUE;
		}
		else TrimSounds();

		theFileTree.DumpFileToBuffer( static_cast<app_FileTree::HFILEENTRY>(hSoundFile), &pBuf );

		pInfo = (RIFF_CHUNK_INFO_t*)pBuf;
		assert( (pInfo->fccID==FOURCC_RIFF)&&(pInfo->fccType==FOURCC_WAVE) );

		pInfo = (RIFF_CHUNK_INFO_t*)(pBuf + sizeof(RIFF_CHUNK_INFO_t));
		assert( (pInfo->fccID==FOURCC_FMT)&&(pInfo->Size>=16) );

		memcpy( &WaveFormat, pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8), sizeof(WAVEFORMATEX) );
		WaveFormat.cbSize = 0;
		assert( WaveFormat.wFormatTag==WAVE_FORMAT_PCM );

		pData = pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8 + pInfo->Size + 8);
		pInfo = (RIFF_CHUNK_INFO_t*)(pBuf + (sizeof(RIFF_CHUNK_INFO_t) + 8 + pInfo->Size));

		ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
		dsbd.dwSize          = sizeof(DSBUFFERDESC);
		dsbd.dwFlags         = 0;
		dsbd.dwBufferBytes   = pInfo->Size;
		dsbd.guid3DAlgorithm = GUID_NULL;
		dsbd.lpwfxFormat     = &WaveFormat;

		if( FAILED(m_lpDS->CreateSoundBuffer( &dsbd, &NewSound, NULL )) )
		{
			EXIT_CODE_BLOCK
		}

		if( FAILED(NewSound->Lock( 0, pInfo->Size, &lpLockedBuf, &SizeLocked, NULL, NULL, 0 )) )
		{
			EXIT_CODE_BLOCK
		}

		memcpy( lpLockedBuf, pData, SizeLocked );
		NewSound->Unlock( lpLockedBuf, SizeLocked, NULL, 0 );

		NewSound->Play( 0, 0, DSBPLAY_LOOPING );

		m_ActiveSounds[hSoundFile] = NewSound;
		*ppAmbientSound = hSoundFile;

		bReturn = TRUE;

	END_CODE_BLOCK

	delete [] pBuf;
	return bReturn;
}
// End snd_DSound8::PlayAmbientSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::StopSound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPSOUND* ppAmbientSound - 
//
void snd_DSound8::StopSound( LPSOUND* ppAmbientSound )
{
	m_ActiveSounds[*ppAmbientSound]->Stop();
}
// End snd_DSound8::StopSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::TrimSounds() //                    \author Logan Jones
//////////////////////////////                        \date 10/17/2001
//               
//====================================================================
//
void snd_DSound8::TrimSounds()
{
	SoundList_t::iterator	it = m_ActiveSounds.begin();
	SoundList_t::iterator	end = m_ActiveSounds.end();
	DWORD					Status;

	while( it!=end )
	{
		((*it).second)->GetStatus( &Status );
		if( !(Status & DSBSTATUS_PLAYING) )
		{
			SAFE_RELEASE( ((*it).second) );
			SoundList_t::iterator	trim = it++;
			m_ActiveSounds.erase( trim );
		}
		else ++it;
	}
}
// End snd_DSound8::TrimSounds()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_DSound8::DestroyAllSounds() //              \author Logan Jones
////////////////////////////////////                  \date 10/17/2001
//               
//====================================================================
//
void snd_DSound8::DestroyAllSounds()
{
	SoundList_t::iterator	it = m_ActiveSounds.begin();
	SoundList_t::iterator	end = m_ActiveSounds.end();

	for( ; it!=end; ++it)
		SAFE_RELEASE( ((*it).second) );
	m_ActiveSounds.clear();
}
// End snd_DSound8::DestroyAllSounds()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// snd_DSound8::FindSound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: SoundList_t::iterator - 
//
snd_DSound8::SoundList_t::iterator snd_DSound8::FindSound( LPVOID hSoundFile )
{
	return m_ActiveSounds.find( hSoundFile );
}
// End snd_DSound8::FindSound()
//////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// End - snd_DSound8.cpp //
/////////////////////////////////
