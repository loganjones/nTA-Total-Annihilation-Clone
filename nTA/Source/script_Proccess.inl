// script_Proccess.inl //                          \author Logan Jones
////////////////////////                               \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::RunModule() //                 \author Logan Jones
////////////////////////////////                       \date 3/24/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strModule     - 
//
// Return: BOOL - 
//
BOOL script_Proccess::RunModule( LPCTSTR strModule )
{
	Thread_t				NewThread;

	// Get the module
	NewThread.Start = pScript->GetModule( strModule );
	if( NewThread.Start==0xFFFFFFFF ) return FALSE;

	// Initialize the new thread
	NewThread.Offset = NewThread.Start;
	NewThread.pCode = pScript->pStart + NewThread.Offset;
	NewThread.Locals.clear();
	NewThread.pParent = NULL;
	NewThread.Mask = 0;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// Add the new thread and return
	Threads.push_back( NewThread );
	return TRUE;
}
// End script_Proccess::RunModule()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::Run() //                       \author Logan Jones
//////////////////////////////////                     \date 3/24/2002
//               
//====================================================================
// Parameters:
//  Proccess_t& Proccess - 
//
void script_Proccess::Run()
{
//	UpdateAnims();

	ThreadList_t::iterator			it = Threads.begin();
	ThreadList_t::const_iterator	end= Threads.end();
	for(; it!=end; )
	{
		if( (*it).Sleep<std_Time() )
		//	if( Execute( *it )==0 )
			if( ExecuteThread( *it )==0 )
			{
				if( (*it).ThreadComplete ) *(*it).ThreadComplete = TRUE;
				it = Threads.erase( it );
			}
			else ++it;
		else ++it;
	}
}
// End script_Proccess::Run()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::Animate() //                   \author Logan Jones
///////////////////////////////                        \date 6/11/2002
//               
//====================================================================
//
void script_Proccess::Animate()
{
	UpdateAnims();
	ReSync();
}
// End script_Proccess::Animate()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::IsAnimating() //               \author Logan Jones
///////////////////////////////////                    \date 6/11/2002
//               
//====================================================================
// Return: bool - 
//
bool script_Proccess::IsAnimating()
{
	return !Animations.empty();
}
// End script_Proccess::IsAnimating()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::StartScript() //               \author Logan Jones
//////////////////////////////////                     \date 3/24/2002
//               
//====================================================================
// Parameters:
//  UINT32 uiModule      - 
//  Thread_t& Thread     - 
//
void script_Proccess::StartScript( UINT32 uiModule, Thread_t& Thread )
{
	Thread_t				NewThread;
	long					NumArg;

	// Initialize the new thread
	NewThread.Start = NewThread.Offset = pScript->Modules[uiModule];
	NewThread.Locals.clear();
	NewThread.pParent = NULL;
	NewThread.Mask = Thread.Mask;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// Proccess the arguments (if there are any)
	NumArg = pScript->pStart[Thread.Offset + 2];
	for( ; NumArg>0; --NumArg,++NewThread.Start,++NewThread.Offset)
	{
		NewThread.Locals.push_back( Thread.Stack.back() );
		Thread.Stack.pop_back();
	}
	NewThread.pCode = pScript->pStart + NewThread.Offset;

	// Add the new thread
	Threads.push_back( NewThread );
}
// End script_Proccess::StartScript()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// script_Proccess::CallScript() //                \author Logan Jones
/////////////////////////////////                      \date 3/24/2002
//               
//====================================================================
// Parameters:
//  UINT32 uiModule      - 
//  Thread_t& Thread     - 
//
void script_Proccess::CallScript( UINT32 uiModule, Thread_t& Thread )
{
	Thread_t				NewThread;
	long					NumArg;

	// Initialize the new thread
	NewThread.Start = NewThread.Offset = pScript->Modules[uiModule];
	NewThread.Locals.clear();
	NewThread.pParent = &Thread;
	NewThread.Mask = Thread.Mask;
	NewThread.Sleep = 0;
	NewThread.ThreadComplete = NULL;

	// Proccess the arguments (if there are any)
	NumArg = pScript->pStart[Thread.Offset + 2];
	for( ; NumArg>0; --NumArg,++NewThread.Start,++NewThread.Offset)
	{
		NewThread.Locals.push_back( Thread.Stack.back() );
		Thread.Stack.pop_back();
	}
	NewThread.pCode = pScript->pStart + NewThread.Offset;

	// Make it so the parent thread will never run
	Thread.Sleep = 0xFFFFFFFF;

	// Add the new thread
	Threads.push_back( NewThread );
}
// End script_Proccess::CallScript()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - script_Proccess.inl //
//////////////////////////////
