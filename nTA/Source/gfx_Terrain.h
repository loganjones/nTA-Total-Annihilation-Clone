// gfx_Terrain.h //                                \author Logan Jones
//////////////////                                     \date 2/6/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _GFX_TERRAIN_H_
#define _GFX_TERRAIN_H_
/////////////////////////////////////////////////////////////////////


//////////////////////////// gfx_Terrain ////////////////////////////
//
class gfx_Terrain
{

/////////////////////////////////////////////////////////////////////
// Virtual Interface
public:

	// Destroys this terrain interface
	virtual void Destroy() = 0;

	// Renders the terrain image
	virtual void Render() const = 0;

	// Initializes the viewport with image data from the given center point and size
	virtual void InitializeViewport( std_Point ptCenterPosition, std_Size szViewport ) = 0;

	// Scrolls the viewport to a position centered at the given point
	virtual void CenterViewAt( std_Point ptCenterPosition ) = 0;

	// Scrolls the view
	virtual void Scroll( std_Point ptScroll ) = 0;
	virtual void ScrollHorizontaly( long lX, BOOL EarlyOutIsOK=TRUE ) = 0;
	virtual void ScrollVerticaly( long lY, BOOL EarlyOutIsOK=TRUE ) = 0;

	// Sets and returns the viewport size respectively
	virtual BOOL SetViewportSize( std_Size szViewport ) = 0;
	virtual std_Size GetViewportSize() const = 0;

	// Returns a rect with the current terrain viewport
	virtual std_Rect GetViewportRect() const = 0;

	// Returns a rect with the dimensions of the entire map
	virtual std_Rect GetWorldRect() const = 0;


}; // End class - gfx_Terrain
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GFX_TERRAIN_H_)
