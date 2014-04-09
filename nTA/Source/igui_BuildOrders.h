// igui_BuildOrders.h //                           \author Logan Jones
///////////////////////                              \date 02-09-2003
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _IGUI_BUILDORDERS_H_
#define _IGUI_BUILDORDERS_H_
/////////////////////////////////////////////////////////////////////

struct unit_Type;
////////////////////////// igui_BuildOrders /////////////////////////
//
class igui_BuildOrders
{
	friend class game_UserInterface;

/////////////////////////////////////////////////
// INTERFACE
public:

	void Add( const unit_Type* pType, int iAmount );
	void Sub( const unit_Type* pType, int iAmount );

	int Amount( LPCTSTR strName ) const;

	const unit_Type* Top() const {return m_BuildOrders.front().Type;}
	void PopTop();


/////////////////////////////////////////////////
// Data types
private:

	struct Entry_t {
		const unit_Type*	Type;
		int					Amount;
	};

	typedef list< Entry_t >		BuildOrderList_t;


/////////////////////////////////////////////////
// Data types
private:

	class unit_Object*					m_Unit;
	class game_UserInterface*			m_Interface;
	list<igui_BuildOrders>::iterator	m_ID;

	BuildOrderList_t					m_BuildOrders;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	igui_BuildOrders(unit_Object* pUnit,game_UserInterface* pInterface):m_Unit(pUnit),m_Interface(pInterface){}
	virtual ~igui_BuildOrders(){}
/////////////////////////////////////////////////

}; // End class - igui_BuildOrders
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_IGUI_BUILDORDERS_H_)
