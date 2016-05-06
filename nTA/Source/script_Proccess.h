// script_Proccess.h //                            \author Logan Jones
//////////////////////                                 \date 5/4/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _SCRIPT_PROCCESS_H_
#define _SCRIPT_PROCCESS_H_
/////////////////////////////////////////////////////////////////////


#define DEFINE_OPCODE_HANDLER( NAME, CODE ) \
	static long CALLBACK handler_##NAME( script_Proccess& Proccess, Thread_t& Thread );
#define IMPLEMENT_OPCODE_HANDLER( NAME ) \
	long CALLBACK script_Proccess::handler_##NAME( script_Proccess& Proccess, Thread_t& Thread )
#define ASSOCCIATE_INSTRUCTION_TO_OPCODE_HANDLER( NAME, CODE ) \
	OpcodeHandlers[ CODE - UINT32(Opcode_MovePieceWithSpeed) ] = handler_##NAME;

#define MOVE_ANIM_INDEX( PIECE, AXIS )	( ((PIECE) << 4) + 0 + (AXIS) )
#define TURN_ANIM_INDEX( PIECE, AXIS )	( ((PIECE) << 4) + 3 + (AXIS) )
#define SPIN_ANIM_INDEX( PIECE, AXIS )	( ((PIECE) << 4) + 3 + (AXIS) )

class gfx_ModelPiece;

////////////////////////// script_Proccess //////////////////////////
//
class script_Proccess
{
	friend class script_Machine;

/////////////////////////////////////////////////
// Type declarations
protected:

	struct Thread_t;
	typedef list< Thread_t >	ThreadList_t;
	typedef vector< long >		LocalList_t;
	typedef list< long >		Stack_t;

	struct Animation_t;
	typedef map< UINT32, Animation_t >		AnimMap_t;

	typedef long (CALLBACK* LPOpcodeHandler_t)(script_Proccess&,Thread_t&);
	typedef LPOpcodeHandler_t*		OpcodeHandlerMap_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	// Adds the specified module code as a new thread to the proccess
	INLINE BOOL RunModule( LPCTSTR strModule );

	INLINE void Animate();
	INLINE bool IsAnimating();


/////////////////////////////////////////////////
// Various functions that call a script to do something or other (implemented in script_Functions.cpp)
public:

	void Create();
	gfx_ModelPiece* SweetSpot();
	long AimFrom( const int iWeapon, long& lPiece );
	long AimWeapon( const int iWeapon, long lHeading, long lPitch, BOOL* pDoneAiming );
	long QueryWeapon( const int iWeapon, long& lPiece );
	long FireWeapon( const int iWeapon );
	gfx_ModelPiece* QueryNanoPiece();
	gfx_ModelPiece* QueryBuildInfo();
	void StartBuilding( float fHeading, float fPitch );
	void StopBuilding();
	void Activate();
	void Deactivate();
	void Killed( long lSeverity, long& lCorpseType );

	// Set the spin speed for metal extractors
	void SetSpeed( float fSpeed );


/////////////////////////////////////////////////
// Opcode Handlers
protected:

	static bool AssociateOpcodeHandlers();
	static void DestroyOpcodeHandlers();

	// Prototype some handler functions from the instructions defined in ta_COB_Instructions.h
	#define INSTRUCTION_MACRO		DEFINE_OPCODE_HANDLER
	#include "ta_COB_Instructions.h"
	#undef INSTRUCTION_MACRO


/////////////////////////////////////////////////
// Utility Methods
protected:

	// Executes a frames worth of code
	INLINE void Run();

	// Proccesses any piece animations
	void UpdateAnims();

	// Executes the thread code in one run. Used for queries, etc
	long QuickExecute( Thread_t& Thread );

	// Executes the thread code. Proccesses every thing.
	DWORD Execute( Thread_t& Thread );

	DWORD ExecuteThread( Thread_t& Thread );

	// Called by another thread, StartScript() adds a new thread to the proccess
	INLINE void StartScript( UINT32 uiModule, Thread_t& Thread );

	// Called by another thread, CallScript() holds execution of the parent until this new thread is complete
	INLINE void CallScript( UINT32 uiModule, Thread_t& Thread );

	// Syncs the model with whats been happening in the script
	void ReSync();

	// Animation instructions called by the threads
	//void Move( UINT32 uiPiece, UINT32 uiAxis, float fWhere, float fSpeed );
	//void Turn( UINT32 uiPiece, UINT32 uiAxis, float fWhere, float fSpeed );
	//void StartSpin( UINT32 uiPiece, UINT32 uiAxis, float fSpeed, float fAcceleration );
	//void StopSpin( UINT32 uiPiece, UINT32 uiAxis, float fDeceleration );
	//INLINE BOOL IsMoving( UINT32 uiPiece, UINT32 uiAxis );
	//INLINE BOOL IsTurning( UINT32 uiPiece, UINT32 uiAxis );


/////////////////////////////////////////////////
// Data members
protected:

	unit_Object*				pUnit;
	gfx_ModelInstance*			pModel;
	AnimMap_t					Animations;
	long*						Statics;
	script_Code*				pScript;
	ThreadList_t				Threads;

	bool						HasMoved;
	bool						StateChanged;

	static OpcodeHandlerMap_t	OpcodeHandlers;
	static Thread_t				NewThread;


/////////////////////////////////////////////////
// Type definitions
protected:

	struct Thread_t
	{
		LocalList_t					Locals;
		Stack_t						Stack;
		UINT32						Start;
		UINT32						Offset;
		UINT32*						pCode;
		DWORD						Sleep;
		DWORD						Mask;
		BOOL*						ThreadComplete;
		Thread_t*					pParent;
	};

	enum AnimType {
		animation_Move,
		animation_Turn,
		animation_StartSpin,
		animation_Spin,
		animation_StopSpin,
	};

	struct Animation_t
	{
		AnimType			Type;
		UINT32				Piece;
		UINT32				Axis;
		std_Time_t			Last;
		float				Velocity;
		float				Desired;
		std_Vector2			vCurrent;
		std_Vector2			vDesired;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	script_Proccess();
	virtual ~script_Proccess();
/////////////////////////////////////////////////

}; // End class - script_Proccess
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "script_Proccess.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_SCRIPT_PROCCESS_H_)
