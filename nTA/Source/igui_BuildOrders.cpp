// igui_BuildOrders.cpp //                         \author Logan Jones
/////////////////////////                            \date 02-09-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "igui.h"
#include "igui_BuildOrders.h"
#include "unit.h"
#include "game.h"

/////////////////////////////////////////////////////////////////////
// igui_BuildOrders::Add() //                      \author Logan Jones
////////////////////////////                         \date 02-09-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType - 
//  int iAmount            - 
//
void igui_BuildOrders::Add( const unit_Type* pType, int iAmount )
{
	// First build order?
	if( m_BuildOrders.empty() )
		m_BuildOrders.push_back(Entry_t()),
		//m_BuildOrders.back().Name = strType,
		m_BuildOrders.back().Type = pType,
		m_BuildOrders.back().Amount = iAmount,
		m_Unit->RequestUnit( pType );//theGame.Units.GetUnitType(strType) );

	// Is this build order already at the back of the list
	//else if( stricmp(m_BuildOrders.back().Name,strType)==0 )
	else if( m_BuildOrders.back().Type==pType )
		m_BuildOrders.back().Amount += iAmount;

	// Just add a new entry to the back
	else m_BuildOrders.push_back(Entry_t()),
		//m_BuildOrders.back().Name = strType,
		m_BuildOrders.back().Type = pType,
		m_BuildOrders.back().Amount = iAmount;
}
// End igui_BuildOrders::Add()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_BuildOrders::Sub() //                      \author Logan Jones
////////////////////////////                         \date 02-09-2003
//
//===================================================================
// Parameters:
//  const unit_Type* pType - 
//  int iAmount            - 
//
void igui_BuildOrders::Sub( const unit_Type* pType, int iAmount )
{
	BuildOrderList_t::iterator	it = m_BuildOrders.begin(),
								end= m_BuildOrders.end();

	for(; it!=end && iAmount; )
		//if( stricmp((*it).Name,strType)==0 )
		if( (*it).Type==pType )
			if( (*it).Amount>iAmount )
				(*it).Amount -= iAmount,
				iAmount = 0;
			//else if( it==m_BuildOrders.begin() )
			else 
				iAmount -= (*it).Amount,
				it = m_BuildOrders.erase(it);
		else ++it;
}
// End igui_BuildOrders::Sub()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_BuildOrders::Amount() //                   \author Logan Jones
///////////////////////////////                      \date 02-09-2003
//
//===================================================================
// Parameters:
//  LPCTSTR strName - 
//
// Return: int - 
//
int igui_BuildOrders::Amount( LPCTSTR strName ) const
{
	int	Amount = 0;
	BuildOrderList_t::const_iterator	it = m_BuildOrders.begin(),
										end= m_BuildOrders.end();

	for(; it!=end; ++it )
		if( stricmp((*it).Type->UnitName,strName)==0 )
			if( (*it).Amount>0 )
				Amount += (*it).Amount;
			else return -1;

	return Amount;
}
// End igui_BuildOrders::Amount()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_BuildOrders::PopTop() //                   \author Logan Jones
///////////////////////////////                      \date 03-01-2003
//
//===================================================================
//
void igui_BuildOrders::PopTop()
{
	if( m_BuildOrders.empty() ) return;

	if( --m_BuildOrders.front().Amount==0 )
		m_BuildOrders.pop_front();

	if( m_BuildOrders.empty() )
		m_Interface->KillBuildOrders( this );
}
// End igui_BuildOrders::PopTop()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_BuildOrders.cpp //
///////////////////////////////