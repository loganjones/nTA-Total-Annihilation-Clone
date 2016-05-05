// snd.h //                                        \author Logan Jones
//////////                                            \date 2/11/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SND_H_
#define _SND_H_
/////////////////////////////////////////////////////////////////////


// Include the base code
#include "sys.h"

// Define the sound system (platform dependent)
#ifdef _WIN32
 #include "snd_DSound8.h"
 typedef snd_DSound8			snd_Interface;
#elif __APPLE__
 #include "snd_Mac.h"
 typedef snd_Mac                snd_Interface;
#else
#  error "No sound interface known for this platform."
#endif

// Global snd_Interface object
// NOTE: Define 'INSTANTIATE_SOUND_INTERFACE' in some source file
//       to instantiate the global snd_Interface object
#ifdef INSTANTIATE_SOUND_INTERFACE
	snd_Interface			sound;
#else
	extern snd_Interface	sound;
#endif


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SND_H_)
