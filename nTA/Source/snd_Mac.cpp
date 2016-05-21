// snd_Mac.cpp //                              \author Logan Jones
////////////////////                                  \date 9/27/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "app.h"
#include "snd.h"
#include "snd_Mac.h"


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
snd_Mac::snd_Mac()
{}
snd_Mac::~snd_Mac()
{
	Destroy();
}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::Create() //                        \author Logan Jones
//////////////////////////                             \date 9/27/2001
//               
//====================================================================
// Parameters:
//  std_PlatformParameters* pPlatformParams - 
//
// Return: BOOL - 
//
BOOL snd_Mac::Create( std_PlatformParameters* pPlatformParams )
{
	return TRUE;
}
// End snd_Mac::Create()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::Destroy() //                       \author Logan Jones
///////////////////////////                            \date 9/27/2001
//               
//====================================================================
//
void snd_Mac::Destroy()
{
}
// End snd_Mac::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::PlaySound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSound - 
//
// Return: BOOL - 
//
BOOL snd_Mac::PlaySound( LPCTSTR strSound )
{
    return TRUE;
}
// End snd_Mac::PlaySound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::PlaySound() //                     \author Logan Jones
/////////////////////////////                          \date 4/15/2002
//               
//====================================================================
// Parameters:
//  LPVOID hSoundFile - 
//
// Return: BOOL - 
//
BOOL snd_Mac::PlaySound( LPVOID hSoundFile )
{
    return TRUE;
}
// End snd_Mac::PlaySound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::PlayAmbientSound() //              \author Logan Jones
////////////////////////////////////                  \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strSound        - 
//  LPSOUND* ppAmbientSound - 
//
// Return: BOOL - 
//
BOOL snd_Mac::PlayAmbientSound( LPCTSTR strSound, LPSOUND* ppAmbientSound )
{
    return TRUE;
}
// End snd_Mac::PlayAmbientSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::PlayAmbientSound() //              \author Logan Jones
////////////////////////////////////                   \date 4/22/2002
//               
//====================================================================
// Parameters:
//  LPVOID hSoundFile       - 
//  LPSOUND* ppAmbientSound - 
//
// Return: BOOL - 
//
BOOL snd_Mac::PlayAmbientSound( LPVOID hSoundFile, LPSOUND* ppAmbientSound )
{
    return TRUE;
}
// End snd_Mac::PlayAmbientSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::StopSound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPSOUND* ppAmbientSound - 
//
void snd_Mac::StopSound( LPSOUND* ppAmbientSound )
{
}
// End snd_Mac::StopSound()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::TrimSounds() //                    \author Logan Jones
//////////////////////////////                        \date 10/17/2001
//               
//====================================================================
//
void snd_Mac::TrimSounds()
{
}
// End snd_Mac::TrimSounds()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// snd_Mac::DestroyAllSounds() //              \author Logan Jones
////////////////////////////////////                  \date 10/17/2001
//               
//====================================================================
//
void snd_Mac::DestroyAllSounds()
{
}
// End snd_Mac::DestroyAllSounds()
//////////////////////////////////////////////////////////////////////

/*
//////////////////////////////////////////////////////////////////////
// snd_Mac::FindSound() //                     \author Logan Jones
/////////////////////////////                         \date 10/17/2001
//               
//====================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: SoundList_t::iterator - 
//
snd_Mac::SoundList_t::iterator snd_Mac::FindSound( LPVOID hSoundFile )
{
	return m_ActiveSounds.find( hSoundFile );
}
// End snd_Mac::FindSound()
//////////////////////////////////////////////////////////////////////
*/

/////////////////////////////////////////////////////////////////////
// End - snd_Mac.cpp //
/////////////////////////////////
