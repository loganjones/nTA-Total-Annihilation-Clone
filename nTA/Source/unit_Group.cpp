// unit_Group.cpp //                               \author Logan Jones
///////////////////                                    \date 5/2/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "unit.h"
#include "unit_Group.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
unit_Group::unit_Group()
{}
unit_Group::~unit_Group()
{}
//
/////////////////////////////////////////////////////////////////////


void unit_Group::OnRequestStop(){}
void unit_Group::OnRequestMoveTo( const std_Point& ptWhere, bool bEnqueue ){}
void unit_Group::OnRequestBuild( const unit_Type* pType, const std_Point& ptWhere, bool bEnqueue ){}
void unit_Group::OnRequestActivate( bool bActivate ){}
void unit_Group::OnRequestAttack( const std_Point& ptWhere, bool bEnqueue ){}
void unit_Group::OnRequestUnit( const unit_Type* pType ){}
void unit_Group::OnNotifyPrepareToBuild( const unit_Type* pType, const std_Point& ptWhere ){}
void unit_Group::OnNotifySpawnNewUnit( const unit_Type* pType, const std_Point& ptWhere ){}


/////////////////////////////////////////////////////////////////////
// End - unit_Group.cpp //
/////////////////////////