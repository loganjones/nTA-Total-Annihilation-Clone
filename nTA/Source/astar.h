// astar.h //                                      \author Logan Jones
////////////                                         \date 06-22-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _A_STAR_H_
#define _A_STAR_H_
/////////////////////////////////////////////////////////////////////


class astar_Base
{
	virtual void Run() = 0;
};

/////////////////////////// astar_Machine ///////////////////////////
//
template <class STORAGE_TYPE, class GOAL_TYPE, class MAP_TYPE>
class astar_Machine: public astar_Base, public STORAGE_TYPE
{
	// The class supplied for the STORAGE_TYPE template parameter is assumed
	// to have the following methods and members:
	//		node_type		m_WorkingNode;		// Current node being proccessed
	//		node_type		m_Neighbor;			// Last neighbor node proccessed
	//		node_type BestNode();				// Returns the best node in the lists
	//		void Open( node_type n );			// Adds node n to open list
	//		void Close( node_type n );			// Adds node n to closed list


/////////////////////////////////////////////////
// INTERFACE
public:

	astar_Machine( MAP_TYPE& MapInterface ):m_Map(MapInterface){}

	virtual void Run();


/////////////////////////////////////////////////
// Data members
private:

	// 
	GOAL_TYPE		m_Goal;

	MAP_TYPE&		m_Map;
	//		void InitNeighbors( node_type n );	// Prepare the neighbors of n
	//		bool GetNeighbor( node_type& n );	// Return a neighbor in n or false if none


}; // End class - astar_Machine
/////////////////////////////////////////////////////////////////////


template <class STORAGE_TYPE, class GOAL_TYPE, class MAP_TYPE>
void astar_Machine<STORAGE_TYPE,GOAL_TYPE,MAP_TYPE>::Run()
{
	STORAGE_TYPE::tile_type		Neighbor;
	float						Cost;

	do {

		// Get cheapest node in open list
		m_WorkingNode = BestNode();

		// Check for goal
		if( m_Goal.IsComplete(m_WorkingNode) )
			break;

		// Check neighbors
		for( m_Map.InitNeighbors(m_WorkingNode); m_Map.GetNeighbor(Neighbor); ) {

			if( !m_Goal.GetTileCost(Neighbor,Cost) )
				continue;

			Open( Neighbor, Cost, m_WorkingNode );
		}

		// Add n to closed list
		Close( m_WorkingNode );

	} while( !m_Goal.TimeToBreak() );
}


/////////////////////////////////////////////////////////////////////
#endif // !defined(_A_STAR_H_)
