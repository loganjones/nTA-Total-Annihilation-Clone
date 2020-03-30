// unit_Factory.cpp //                             \author Logan Jones
/////////////////////                                 \date 2/19/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Factory.h"
#include "game.h"
#include "snd.h"

// Derived unit_Object behaviours
#include "unit_Structure.h"
#include "unit_Ground.h"
#include "unit_Aircraft.h"
#include "unit_Seacraft.h"
#include "unit_Hovercraft.h"

// Include inline implementaions here for a debug build
#ifdef _DEBUG
 #include "unit_Factory.inl"
#endif // defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
unit_Factory::unit_Factory():m_NextID(0)
{}
unit_Factory::~unit_Factory()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::Update() //                       \author Logan Jones
///////////////////////////                            \date 2/20/2002
//               
//====================================================================
//
void unit_Factory::Update()
{
	unit_Object*				pUnit;

	// Work on paths in progress
	{
		PathsInProgress_t::iterator	it = m_PathsInPogress.begin();
		PathsInProgress_t::iterator	end= m_PathsInPogress.end();

		for(; it!=end; ++it)
		{
			pUnit = (*it).first;
			if( FindPath( std_Point(), (*it).second.Finish, 0, pUnit, (*it).second.pResult )==PATH_FOUND )
				pUnit->SetPath();
		}
	}
	// End working on paths

	// Update each unit
//	for_each( m_Units.begin(), m_Units.end(), UpdateUnit );
}
// End unit_Factory::Update()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Factory::AllocateUnitObject() //           \author Logan Jones
///////////////////////////////////////               \date 11/2/2002
//
//===================================================================
// Parameters:
//  const unit_Type* pType - 
//
// Return: unit_Object* - 
//
unit_Object* unit_Factory::AllocateUnitObject( const unit_Type* pType )
{
	switch( pType->Behaviour ) {
		case unit_Type::Structure:		return new unit_Structure( *this );
		case unit_Type::Groundcraft:	return new unit_Ground( *this );
		case unit_Type::Seacraft:		return new unit_Seacraft( *this );
		case unit_Type::Aircraft:		return new unit_Aircraft( *this );
		case unit_Type::Hovercraft:		return new unit_Hovercraft( *this );
		default: // This shouldn't happen
			assert( !"Invalid unit type behaviour." );
			return new unit_Ground( *this );
	}
}
// End unit_Factory::AllocateUnitObject()
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::SpawnUnit() //                    \author Logan Jones
//////////////////////////////                         \date 4/15/2002
//               
//====================================================================
// Parameters:
//  const unit_Type* pType   - 
//  const std_Point& ptWhere - 
//
// Return: unit_Object* - 
//
unit_Object* unit_Factory::SpawnUnit( const unit_Type* pType, const std_Point& ptWhere, class game_Player* pPlayer )
{
	unit_Object*	pUnit = AllocateUnitObject( pType );
	std_Vector3		Normal;

	pUnit->Create( ptWhere /*+ std_Point(pType->FootPrint.width*8,pType->FootPrint.height*8)*/, pType, pPlayer );
	//m_Units.push_back( pUnit );
	pUnit->m_UniqueID = m_NextID;
	m_Units[ m_NextID ] = pUnit;
	++m_NextID;

	GetTerrainNormal( pUnit->m_BaseRect.Position() / 16, pType, Normal );
    if( !pType->Upright ) {
        pUnit->m_Orientation.x = Normal.x;
		pUnit->m_Orientation.y = Normal.y;
    }

	return pUnit;
}
// End unit_Factory::SpawnUnit()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::FindPath() //                     \author Logan Jones
///////////////////////////////                         \date 3/3/2002
//               
//====================================================================
// Parameters:
//  std_Point ptStart         - 
//  std_Point ptFinish        - 
//  float fProximityToFinish  - 
//  unit_Object* pUnit        - 
//
// Return: PathFindResult_t - 
//
unit_Factory::PathFindResult_t unit_Factory::FindPath( std_Point ptStart, std_Point ptFinish, float fProximityToFinish, unit_Object* pUnit, unit_Object::Waypoints_t* pResult )
{
#ifdef VISUAL_PATHFIND
 #define VISUAL_PATHFIND_SLEEP()	pPath->count = count + 1; pPath->diagonal = !diagonal; return PATH_PROCCESSING
#else
 #define VISUAL_PATHFIND_SLEEP()
#endif
	PathFind_t*			pPath;
	SearchNode_t*		pStart;
	SearchNode_t*		pNeighbor;
	std_Point			Direction;
	float				MoveCost;
	const std_Point ptNext[8] = {	std_Point(1,0), std_Point(1,1), std_Point(0,1), std_Point(-1,1),
									std_Point(-1,0), std_Point(-1,-1), std_Point(0,-1), std_Point(1,-1) };
	const std_Vector2 vDir[8] = {	std_Vector2(1,0), std_Vector2(SQRT_OF_2/2,SQRT_OF_2/2), std_Vector2(0,1), std_Vector2(-SQRT_OF_2/2,SQRT_OF_2/2),
									std_Vector2(-1,0), std_Vector2(-SQRT_OF_2/2,-SQRT_OF_2/2), std_Vector2(0,-1), std_Vector2(SQRT_OF_2/2,-SQRT_OF_2/2) };

	// Check if the path search has already been started
	if( (pPath=GetPathInProgress(pUnit))==NULL )
	{
		// Make sure the finish point is not occupied
		if( theGame.Terrain.GetMapNode(ptFinish)->OccupationCount_Static )
			return PATH_NOT_FOUND;

		// Create an entry in the paths-in-progress list
		pPath = &(m_PathsInPogress[pUnit]);

		// Make sure the lists are empty
		pPath->NodeBank.clear();
		pPath->Open.clear();
		pPath->pResult = pResult;

		// Initialize the other members
		pPath->Finish = ptFinish;
		pPath->pProccessee = NULL;
		pPath->count = 0;
		pPath->diagonal = 0;

		// Allocate and initialize the start node
		pStart = new SearchNode_t;
		pStart->Location = ptStart;
		pStart->CostFromStart = 0;
		pStart->TotalCost = (ptFinish - ptStart).Magnitude();
		pStart->Parent = NULL;
		pStart->InOpen = TRUE;
		pStart->InClosed = FALSE;
		PQ_Push( pPath->Open, pStart );
		pPath->NodeBank[ pStart->Location ] = pStart;

	} // End PathFind initialization

	// Work until the open list is exausted
	while( !pPath->Open.empty() )
	{
		// Pop the most promising node off the open list
		if( pPath->count==0 )
			pPath->pBest = PQ_Pop( pPath->Open );
		pPath->pBest->InOpen = FALSE;

		// Check if this is the end or close enough
		if( (pPath->pBest->Location - ptFinish).MagnitudeSquared() <= sqr(fProximityToFinish) )
		{
			const std_Vector2 Offset( pUnit->m_pUnitType->FootPrint.width * 0.5f, pUnit->m_pUnitType->FootPrint.height * 0.5f );
			pPath->pResult->clear();
			//for( SearchNode_t* pNode=pPath->pBest; pNode; pNode=pNode->Parent )
			//	pPath->pResult->push_back( (Offset + pNode->Location) * 16 );

			std_Vector2 Cur,Last,Dir,LastDir;
			SearchNode_t* pNode = pPath->pBest;
			do {
				Cur = (Offset + pNode->Location) * 16;
				Dir = Cur - Last;

				if( Dir!=LastDir )
					pPath->pResult->push_back( Cur );
				
				Last = Cur;
				LastDir = Dir;
				pNode = pNode->Parent;
			} while( pNode );

			m_PathsInPogress.erase( pUnit );
			return PATH_FOUND;
		}

		// What direction are we moving
		if( pPath->pBest->Parent )
			Direction = pPath->pBest->Location - pPath->pBest->Parent->Location;
		else Direction.Set(0,0);

		// Proccess each neighbor
		for( int count=pPath->count,diagonal=pPath->diagonal; count<8; ++count,diagonal=!diagonal)
		{
			const std_Point Loc = pPath->pBest->Location + ptNext[count];
			if( (Loc.x<0 || Loc.y<0) || (Loc.x>=(theGame.Terrain.GetTerrainSize().width-1) || Loc.y>=(theGame.Terrain.GetTerrainSize().height-1)) )
				continue;

			// If we just came from this node or we failed to move to there, move on
			if( (pPath->pBest->Parent && pPath->pBest->Parent->Location==Loc) ||
				!GetMoveCost(Loc,diagonal,vDir[count],pUnit,MoveCost) )
				continue;

			// Calculate the total cost
			const float TotalCost = pPath->pBest->CostFromStart + MoveCost + (ptFinish - Loc).Magnitude();

			// Look for the node in the bank
			SearchNodeBank_t::const_iterator it = pPath->NodeBank.find( Loc );
			if( it==pPath->NodeBank.end() )
			{
				// Create a new node
				pNeighbor = new SearchNode_t;
				pNeighbor->Location = Loc;
				pNeighbor->InOpen = FALSE;
				pNeighbor->InClosed = FALSE;
				pPath->NodeBank[ pNeighbor->Location ] = pNeighbor;
			}
			else // Node was found in the bank
			{
				// Get the node from the bank
				pNeighbor = (*it).second;

				// If this node is already in a list, only proceed if the cost is less
				if( (pNeighbor->InOpen || pNeighbor->InClosed) && pNeighbor->TotalCost<=TotalCost )
					continue;
			}
			pPath->pProccessee = pNeighbor;

			// Update this node with some new info
			pNeighbor->CostFromStart = pPath->pBest->CostFromStart + MoveCost;
			pNeighbor->TotalCost = TotalCost;
			pNeighbor->Parent = pPath->pBest;
			pNeighbor->InClosed = FALSE;

			// Add this node to the open list
			if( pNeighbor->InOpen )
				PQ_Update( pPath->Open, pNeighbor );
			else
			{
				PQ_Push( pPath->Open, pNeighbor );
				pNeighbor->InOpen = TRUE;
			}

			// Let the unit render the path find progress
			VISUAL_PATHFIND_SLEEP();

		} // end for( Neighbors )
		pPath->count = 0;
		pPath->diagonal = 0;

		// Add this node to the closed list
		pPath->pBest->InClosed = TRUE;

	} // end while Open is not empty

	m_PathsInPogress.erase( pUnit );
	return PATH_NOT_FOUND;
}
// End unit_Factory::FindPath()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetTerrainNormal() //             \author Logan Jones
/////////////////////////////////////                  \date 4/12/2002
//               
//====================================================================
// Parameters:
//  const std_Point ptLoc  - 
//  const unit_Type* pType - 
//  std_Vector& vNormal3   - 
//
// Return: bool - 
//
bool unit_Factory::GetTerrainNormal( const std_Point ptLoc, const unit_Type* pType, std_Vector3& vNormal ) const
{
	game_Terrain::MapNode* pNode = theGame.Terrain.GetMapNode(ptLoc);
	const long pitch = theGame.Terrain.GetMapPitch();

	vNormal.Set(0,0,0);
	for( long y=0; y<pType->FootPrint.height; ++y)
	{
		for( long x=0; x<pType->FootPrint.width; ++x)
		{
			if( pNode[x].OccupationCount_Static || (sqr(pNode[x].Normal.x) + sqr(pNode[x].Normal.y))>sqr(pType->MaxSlope) )
				return false;
			vNormal += pNode[x].Normal;
		}
		pNode += pitch;
	}
	vNormal.Normalize();
	if( (sqr(vNormal.x) + sqr(vNormal.y))>sqr(pType->MaxSlope) )
		return false;

	return true;
}
// End unit_Factory::GetTerrainNormal()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// unit_Factory::GetMoveCost() //                  \author Logan Jones
////////////////////////////////                       \date 3/27/2002
//               
//====================================================================
// Parameters:
//  const std_Point ptLoc  - 
//  const int bDiagonal    - 
//  const std_Vector2 vDir - 
//  unit_Object* pUnit     - 
//  float& MoveCost        - 
//
// Return: BOOL - 
//
BOOL unit_Factory::GetMoveCost( const std_Point ptLoc, const int bDiagonal, const std_Vector2 vDir, unit_Object* pUnit, float& MoveCost )
{
	std_Vector3					Normal;

	const BOOL bResult = GetTerrainNormal( ptLoc, pUnit->m_pUnitType, Normal );

	const float Distance = (bDiagonal ? SQRT_OF_2 : 1.0f);
	const float Slope = (vDir.x * Normal.x) + (vDir.y * Normal.y);

	MoveCost =	Distance
				//- DirectionChange
				- (Slope * SLOPE_PENALTY)
				;
	return bResult;
}
// End unit_Factory::GetMoveCost()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Factory.cpp //
///////////////////////////
