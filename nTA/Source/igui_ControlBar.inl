// igui_ControlBar.inl //                          \author Logan Jones
////////////////////////                              \date 11/6/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// igui_ControlBar::SetButtonImages() //           \author Logan Jones
///////////////////////////////////////               \date 11/6/2002
//
//===================================================================
// Parameters:
//  gadget_Button* pButton - 
//
void igui_ControlBar::SetButtonImages( gadget_Button* pButton )
{
	LPCTSTR					Name = pButton->GetName();
	SurfaceMap_t::iterator	it = m_Surfaces.find( Name );	
	gfx_Surface**			pSurfaces;

	// Have surfaces by this name already
	// been assocciated with this name
	if( it==m_Surfaces.end() )
	{
		// No. Assocciate them now
		pSurfaces = m_Surfaces[ Name ].Surfaces;
		ZeroMemory( pSurfaces, sizeof(gfx_Surface*) * 6 );
		if( gfx->CreateSurfacesFromGAF(
							guiResources.CommonGuiBuffer,
                            Name, 1, 3, pSurfaces)==0 ) {
            m_Surfaces.erase( Name );
			pSurfaces = NULL;
        }
	}
	else pSurfaces = ((*it).second).Surfaces;

	// Strip the ARM or COR prefix from the name
	if( strnicmp("ARM",Name,3)==0 || strnicmp("COR",Name,3)==0 )
		pButton->SetName( Name + 3 );

	// Set the images
	if( pSurfaces==NULL ) pButton->SetButtonImages( NULL, NULL, NULL );
	else pButton->SetButtonImages( pSurfaces, pSurfaces[1], pSurfaces[2] );
}
// End igui_ControlBar::SetButtonImages()
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// End - igui_ControlBar.inl //
//////////////////////////////
