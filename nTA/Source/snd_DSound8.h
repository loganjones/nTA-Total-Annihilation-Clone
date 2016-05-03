// snd_DSound8.h //                                \author Logan Jones
//////////////////                                    \date 9/27/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SND_DSOUND8_H_
#define _SND_DSOUND8_H_
/////////////////////////////////////////////////////////////////////


#include <dsound.h>
typedef LPVOID	LPSOUND;

//////////////////////////// snd_DSound8 ////////////////////////////
//
class snd_DSound8
{
//	struct Sound_t
//	{
//		char					Name[ 32 ];
//		LPDIRECTSOUNDBUFFER		pDSBuffer;
//	};

	//typedef list< Sound_t >	SoundList_t;
	typedef unordered_map< LPVOID, LPDIRECTSOUNDBUFFER >	SoundList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( std_PlatformParameters* pPlatformParams );
	void Destroy();

	BOOL PlaySound( LPCTSTR strSound );
	BOOL PlaySound( LPVOID hSoundFile );
	BOOL PlayAmbientSound( LPCTSTR strSound, LPSOUND* ppAmbientSound );
	BOOL PlayAmbientSound( LPVOID hSoundFile, LPSOUND* ppAmbientSound );
	void StopSound( LPSOUND* ppAmbientSound );


/////////////////////////////////////////////////
// Data members
protected:

	LPDIRECTSOUND8		m_lpDS;
	SoundList_t			m_ActiveSounds;


/////////////////////////////////////////////////
// Utility methods
protected:

	void TrimSounds();
	void DestroyAllSounds();
	//SoundList_t::iterator FindSound( LPVOID hSoundFile );

#undef FOURCC_RIFF
	enum FOUR_CC_t
	{
		FOURCC_RIFF				= 0x46464952,
		FOURCC_WAVE				= 0x45564157,
		FOURCC_FMT				= 0x20746D66,
		FOURCC_DATA				= 0x61746164,
		FOURCC_FORCE32			= 0xFFFFFFFF
	};


	struct RIFF_CHUNK_INFO_t
	{
		FOUR_CC_t		fccID;
		UINT32			Size;
		FOUR_CC_t		fccType;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	snd_DSound8();
	virtual ~snd_DSound8();
/////////////////////////////////////////////////

}; // End class - snd_DSound8
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SND_DSOUND8_H_)
