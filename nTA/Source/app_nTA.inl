// app_nTA.inl //                                  \author Logan Jones
////////////////                                      \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GetRenderer() //                       \author Logan Jones
///////////////////////////                             \date 8/5/2002
//               
//====================================================================
// Return: LPCTSTR - 
//
LPCTSTR app_nTA::GetRenderer() const
{
	return m_Renderer;
}
// End app_nTA::GetRenderer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GetResolution() //                     \author Logan Jones
/////////////////////////////                          \date 8/10/2002
//               
//====================================================================
// Return: std_Size - 
//
std_Size app_nTA::GetResolution() const
{
	return m_RendererResolution;
}
// End app_nTA::GetResolution()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::GetColorDepth() //                     \author Logan Jones
/////////////////////////////                          \date 8/10/2002
//               
//====================================================================
// Return: long - 
//
long app_nTA::GetColorDepth() const
{
	return m_RendererDepth;
}
// End app_nTA::GetColorDepth()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::SetRenderer() //                       \author Logan Jones
///////////////////////////                             \date 8/5/2002
//               
//====================================================================
// Parameters:
//  LPCTSTR strRenderer - 
//
void app_nTA::SetRenderer( LPCTSTR strRenderer )
{
	strcpy( m_Renderer, strRenderer );
}
// End app_nTA::SetRenderer()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::SetResolution() //                     \author Logan Jones
/////////////////////////////                          \date 8/10/2002
//               
//====================================================================
// Parameters:
//  std_Size szResolution - 
//
void app_nTA::SetResolution( std_Size szResolution )
{
	m_RendererResolution = szResolution;
}
// End app_nTA::SetResolution()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// app_nTA::SetColorDepth() //                     \author Logan Jones
/////////////////////////////                          \date 8/10/2002
//               
//====================================================================
// Parameters:
//  long lDepth - 
//
void app_nTA::SetColorDepth( long lDepth )
{
	m_RendererDepth = lDepth;
}
// End app_nTA::SetColorDepth()
//////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - app_nTA.inl //
//////////////////////
