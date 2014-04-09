// unit_Orders.cpp //                              \author Logan Jones
////////////////////                                 \date 02-02-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "unit_Orders.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
unit_Orders::unit_Orders()
{}
unit_Orders::~unit_Orders()
{}
//
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Orders::Stop() //                          \author Logan Jones
////////////////////////                             \date 03-24-2003
//
//===================================================================
//
void unit_Orders::Stop()
{
	m_OrderList.clear();
	m_OrderList.push_back( unit_Order(order_Stop) );
}
// End unit_Orders::Stop()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Orders::MoveTo() //                        \author Logan Jones
//////////////////////////                           \date 03-24-2003
//
//===================================================================
// Parameters:
//  const std_Point& ptWaypoint - 
//  void* hPath                 - 
//  bool bAdd                   - 
//
void unit_Orders::MoveTo( const std_Point& ptWaypoint, void* hPath, bool bAdd )
{
	if( !bAdd ) m_OrderList.clear();
	m_OrderList.push_back( unit_Order(order_Move,ptWaypoint) );
	m_OrderList.back().Data.Path = hPath;
}
// End unit_Orders::MoveTo()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// unit_Orders::Build() //                         \author Logan Jones
/////////////////////////                            \date 03-24-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType      - 
//  const std_Point& ptWaypoint - 
//  void* hPath                 - 
//  bool bAdd                   - 
//
void unit_Orders::Build( const unit_Type* pType, const std_Point& ptWaypoint, void* hPath, bool bAdd )
{
	if( !bAdd ) m_OrderList.clear();
	m_OrderList.push_back( unit_Order(order_Move,ptWaypoint) );
	m_OrderList.back().Data.Path = hPath;
	m_OrderList.push_back( unit_Order(order_Build,ptWaypoint) );
	m_OrderList.back().Data.Type = pType;
}
// End unit_Orders::Build()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - unit_Orders.cpp //
//////////////////////////