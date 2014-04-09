// std_Types.inl //                                \author Logan Jones
//////////////////                                    \date 3/24/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// pchar_less_nocase_t::operator() //              \author Logan Jones
////////////////////////////////////                   \date 3/24/2002
//               
//====================================================================
// Return: bool - 
//
bool pchar_less_nocase_t::operator()(const char* strA, const char* strB) const
{
	return (stricmp(strA,strB) < 0);
}
// End pchar_less_nocase_t::operator()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// pchar_equal_nocase_t::operator() //             \author Logan Jones
/////////////////////////////////////                  \date 3/24/2002
//               
//====================================================================
// Return: bool - 
//
bool pchar_equal_nocase_t::operator()(const char* strA, const char* strB) const
{
	return (stricmp(strA,strB) == 0);
}
// End pchar_equal_nocase_t::operator()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// pchar_less_case_t::operator() //                \author Logan Jones
//////////////////////////////////                     \date 3/24/2002
//               
//====================================================================
// Return: bool - 
//
bool pchar_less_case_t::operator()(const char* strA, const char* strB) const
{
	return (strcmp(strA,strB) < 0);
}
// End pchar_less_case_t::operator()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// pchar_equal_case_t::operator() //               \author Logan Jones
///////////////////////////////////                    \date 3/24/2002
//               
//====================================================================
// Return: bool - 
//
bool pchar_equal_case_t::operator()(const char* strA, const char* strB) const
{
	return (strcmp(strA,strB) == 0);
}
// End pchar_equal_case_t::operator()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - std_Types.inl //
////////////////////////
