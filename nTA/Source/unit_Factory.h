// unit_Factory.h //                               \author Logan Jones
///////////////////                                   \date 2/19/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_FACTORY_H_
#define _UNIT_FACTORY_H_
/////////////////////////////////////////////////////////////////////


// Unit structures
#include "unit_SoundCategory.h"
#include "game_LoadFacilitator.h"

//////////////////////////// unit_Factory ////////////////////////////
//
class unit_Factory
{
	friend unit_Type;
	friend class unit_Object;
	friend class game_Main;

/////////////////////////////////////////////////
// External Types
public:

	struct PathFind_t;
    enum PathFindResult_t : short;


/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( game_LoadFacilitator Load );
	void Destroy();

	void ProccessSchema( struct ta_ota_Schema_t* pSchema );

	void Update();

	INLINE unit_Interface* GetUnitInterface( UINT32 uiUniqueID );

	INLINE const unit_Type* GetUnitType( LPCTSTR strType ) const;
	INLINE const unit_Type* GetUnitType( DWORD dwKey ) const;

	unit_Object* SpawnUnit( const unit_Type* pType, const std_Point& ptWhere, class game_Player* pPlayer );

	INLINE bool CheckUnitRect( const unit_Type* pType, const std_Point& ptWhere ) const;

	PathFindResult_t FindPath( std_Point ptStart, std_Point ptFinish, float fProximityToFinish, unit_Object* pUnit, unit_Object::Waypoints_t* pResult );

	INLINE PathFind_t* GetPathInProgress( unit_Object* pObject );


/////////////////////////////////////////////////
// Internal Types
protected:

	typedef list< unit_Type >				UnitTypeList_t;
//	typedef unordered_map< DWORD, unit_Type* >	UnitTypeMap_t;
	typedef unordered_map< DWORD, unit_Type >	UnitTypeMap_t;
	typedef unordered_map< UINT32, unit_Object*>	UnitMap_t;

	typedef list< unit_SoundCategory >		SoundCategoryList_t;
	typedef unordered_map< string, unit_SoundCategory* >	SoundCategoryMap_t;

	struct SearchNode_t;

	//typedef list< SearchNode_t* >			PathFindOpenList_t;
	typedef vector< SearchNode_t* >			PathFindOpenList_t;

	struct PointHash_t { size_t operator()( const std_Point& pt ) const { return (pt.x + (pt.y * 4096)); } };
	struct PointEql_t { bool operator()( const std_Point& ptA, const std_Point& ptB ) const { return (ptA == ptB)==TRUE; } };
	typedef unordered_map< std_Point, SearchNode_t*, PointHash_t, PointEql_t >	SearchNodeBank_t;

	typedef map< unit_Object*, PathFind_t >	PathsInProgress_t;


/////////////////////////////////////////////////
// Data members
protected:

	SoundCategoryList_t			m_SoundCategories;
	SoundCategoryMap_t			m_SoundCategoryMap;

//	UnitTypeList_t				m_UnitTypes;
//	UnitTypeMap_t				m_UnitTypeTable;
	UnitTypeMap_t				m_UnitTypes;

	UnitMap_t					m_Units;
	UINT32						m_NextID;

	PathsInProgress_t			m_PathsInPogress;

	script_Machine				m_ScriptMachine;


/////////////////////////////////////////////////
// Utility Methods
protected:

	BOOL GetSoundCategories();

	bool LoadUnitType( void* hFile );
	void ProccessFBI( void* hFile, unit_Type& Type, LPTSTR strObjectName );
	void ProccessDownloads();

	bool GetTerrainNormal( const std_Point ptLoc, const unit_Type* pType, std_Vector3& vNormal ) const;

	BOOL GetMoveCost( const std_Point ptLoc, const int bDiagonal, const std_Vector2 vDir, unit_Object* pUnit, float& MoveCost );

	unit_Object* AllocateUnitObject( const unit_Type* pType );

	// Priority queue methods used in FindPath()
	INLINE void PQ_Push( PathFindOpenList_t& PQ, SearchNode_t* pNode );
	INLINE SearchNode_t* PQ_Pop( PathFindOpenList_t& PQ );
	INLINE void PQ_Update( PathFindOpenList_t& PQ, SearchNode_t* pNode );

	static INLINE void UpdateUnit( unit_Object* pUnit );


/////////////////////////////////////////////////
// Internal structures
public:

	struct PathFind_t
	{
		SearchNodeBank_t			NodeBank;
		PathFindOpenList_t			Open;
		unit_Object::Waypoints_t*	pResult;

		std_Point					Finish;
		SearchNode_t*				pBest;
		SearchNode_t*				pProccessee;
		int							count;
		int							diagonal;
	};

    enum PathFindResult_t : short
	{
		PATH_NOT_FOUND,
		PATH_PROCCESSING,
		PATH_FOUND,
	};

protected:
	struct SearchNode_t
	{
		std_Point			Location;
		float				CostFromStart;
		float				TotalCost;
		SearchNode_t*		Parent;
		BOOL				InOpen;
		BOOL				InClosed;
	};

	struct GreaterSearchNode_t
	{
		bool operator()( SearchNode_t* pA, SearchNode_t* pB ) const
		{ return (pA->TotalCost > pB->TotalCost); }
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	unit_Factory();
	virtual ~unit_Factory();
/////////////////////////////////////////////////

}; // End class - unit_Factory
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "unit_Factory.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_FACTORY_H_)
