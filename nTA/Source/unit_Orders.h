// unit_Orders.h //                                \author Logan Jones
//////////////////                                   \date 02-02-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _UNIT_ORDERS_H_
#define _UNIT_ORDERS_H_
/////////////////////////////////////////////////////////////////////

// Order types to add to the order queue
enum OrderType_t {
	order_Stop,
	order_Move,		// Move order with a destination and a path
	order_Build,	// Build order with a type and amount
	order_Attack,	// Attack order with a destination
};


//////////////////////////// unit_Order /////////////////////////////
//
struct unit_Order
{
	OrderType_t			Type;

	std_Point			Waypoint;

	union OrderData_t{
		void*					Path;
		const struct unit_Type*	Type;
		class unit_Object*		Unit;
	}					Data;


	unit_Order( OrderType_t eType ):Type(eType),Waypoint(std_Point(0,0)){}
	unit_Order( OrderType_t eType, const std_Point& ptWaypoint ):Type(eType),Waypoint(ptWaypoint){}

}; // End struct - unit_Order
/////////////////////////////////////////////////////////////////////


//////////////////////////// unit_Orders ////////////////////////////
//
class unit_Orders
{

/////////////////////////////////////////////////
// INTERFACE
public:

	// No orders?
	bool Empty() const {return m_OrderList.empty();}

	// Get the current order
	const unit_Order& Front() const {return m_OrderList.front();}

	// Next order; return true if the list is NOT empty
	bool Next() {m_OrderList.pop_front(); return !m_OrderList.empty();}


/////////////////////////////////////////////////
// Order adding methods
public:

	// Add a stop order (and reset the queue)
	void Stop();

	// Add a move order with its assocciated waypoint and path
	void MoveTo( const std_Point& ptWaypoint, void* hPath, bool bAdd=true );

	// Add a unit build order with its assocciated waypoint and path
	void Build( const unit_Type* pType, const std_Point& ptWaypoint, void* hPath, bool bAdd=true );


/////////////////////////////////////////////////
// Data members
private:

	// List of orders
	list< unit_Order >		m_OrderList;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	unit_Orders();
	virtual ~unit_Orders();
/////////////////////////////////////////////////

}; // End class - unit_Orders
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_UNIT_ORDERS_H_)
