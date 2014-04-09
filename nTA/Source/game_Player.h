// game_Player.h //                                \author Logan Jones
//////////////////                                    \date 4/14/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_PLAYER_H_
#define _GAME_PLAYER_H_
/////////////////////////////////////////////////////////////////////


// Valid resource types
enum ResourceType_t {
	Metal,
	Energy,
	RESOURCE_TYPE_COUNT,
};

// Data pertaining to a player's particular resource allotment
struct ResourceData_t {
	float	Amount;			// Total amount in reserves
	float	Max;			// Maximum size for Amount
	float	BaseProduced;	// Base production
	float	TotalProduced;	// Total production
	float	BaseConsumed;	// Base consumption
	float	TotalConsumed;	// Total consumption
};

// Flag value for Update*() to indicate what value is being altered
enum UpdateResouceFlag_t {
	Maximum,
	Production,
	Consumption,
};


//////////////////////////// game_Player ////////////////////////////
//
class game_Player
{
	friend class game_Main;
	typedef list< unit_Object* >				UnitList_t;
	typedef list< unit_Job >					JobList_t;

/////////////////////////////////////////////////
// INTERFACE
public:

	virtual void Initialize( struct ta_ota_Schema_t* pSchema );

	virtual void Update();

	void UpdateEconomy();

	// Comsume/Produce a specified amount of a resource
	INLINE void Consume( ResourceType_t eWhat, float fHowMuch );
	INLINE void Produce( ResourceType_t eWhat, float fHowMuch );

	// Update a resources base Production/Consumption value
	INLINE void Update( ResourceType_t eType, UpdateResouceFlag_t eWhat, float fNew, float fOld=0 );

	INLINE unit_Job* NewJob( unit_Object* pWorker, job_Type eType, bool bStartActive=false );

	INLINE void KillJob( unit_Object* pWorker );


/////////////////////////////////////////////////
// Accessors
public:

	void SetSide( LPCTSTR strSide, DWORD dwColor ) {strcpy(m_Side,strSide); m_Color=dwColor;}
	LPCTSTR SideName() const {return m_Side;}
	DWORD Color() const {return m_Color;}

	const ResourceData_t& Resource( ResourceType_t eWhat ) const {return m_Resources[eWhat];}


/////////////////////////////////////////////////
// Data members
protected:

	UnitList_t				m_Units;
	char					m_Side[ 16 ];
	DWORD					m_Color;
	char					m_Name[32];
	DWORD					m_Index;

	ResourceData_t			m_Resources[ RESOURCE_TYPE_COUNT ];

	JobList_t				m_Jobs;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	game_Player();
	virtual ~game_Player();
/////////////////////////////////////////////////

}; // End class - game_Player
/////////////////////////////////////////////////////////////////////

// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "game_Player.inl"
#endif // !defined( _DEBUG )

/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_PLAYER_H_)
