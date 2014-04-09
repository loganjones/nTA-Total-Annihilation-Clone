// util_RectFiller.cpp //                          \author Logan Jones
////////////////////////                               \date 7/2/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////

#include "std.h"
#include "util_RectFiller.h"

/////////////////////////////////////////////////////////////////////
// Default Construction/Destruction
//
util_RectFiller::util_RectFiller()
{
	// TODO: Add INITIALIZATION
}
util_RectFiller::~util_RectFiller()
{}
//
/////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_RectFiller::Initialize() //                \author Logan Jones
//////////////////////////////////                      \date 7/2/2001
// Description:  
//               
//====================================================================
// Parameters:
//  long lWidth  - 
//  long lHeight - 
//
void util_RectFiller::Initialize( long lWidth, long lHeight )
{
	m_Rects = new RECT_NODE;
	m_Rects->theRect.Set( 0, 0, lWidth, lHeight );
	//SetRect( &m_Rects->theRect, 0, 0, lWidth, lHeight );
	m_Rects->next = NULL;
}
// End util_RectFiller::Initialize()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_RectFiller::Destroy() //                   \author Logan Jones
///////////////////////////////                         \date 7/2/2001
// Description:  
//               
//====================================================================
//
void util_RectFiller::Destroy()
{
	RECT_NODE* pRectNode = m_Rects;
	RECT_NODE* pDel;
	while( pRectNode )
	{
		pDel = pRectNode;
		pRectNode = pRectNode->next;
		delete pDel;
	}
	m_Rects = NULL;
}
// End util_RectFiller::Destroy()
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// util_RectFiller::FindSuitableRect() //          \author Logan Jones
////////////////////////////////////////                \date 7/2/2001
// Description:  
//               
//====================================================================
// Parameters:
//  long lWidth             - 
//  long lHeight            - 
//  std_Rect* pSuitableRect - 
//
// Return: BOOL - 
//
BOOL util_RectFiller::FindSuitableRect( long lWidth, long lHeight, std_Rect* pSuitableRect )
{
	// Iterate through each rect
	RECT_NODE*		pRectNode = m_Rects, *pPrev=NULL;
	for( ; pRectNode; pRectNode=pRectNode->next )
	{
		// Check if this rect is large enough
		if( (lWidth<=(pRectNode->theRect.right-pRectNode->theRect.left)) &&
			(lHeight<=(pRectNode->theRect.bottom-pRectNode->theRect.top)) )
			// It is; break out of the search
			break;
		pPrev = pRectNode;
	} // end for( pRectNode )

	// Check if the search failed to find a rect
	if( pRectNode==NULL )
		return FALSE;

	// Set the returning rect
	pSuitableRect->left = pRectNode->theRect.left;
    pSuitableRect->top = pRectNode->theRect.top;
    pSuitableRect->right = pSuitableRect->left + lWidth;
    pSuitableRect->bottom = pSuitableRect->top + lHeight;

	// Less space is available, set the rects accordingly
	if( (lWidth==(pRectNode->theRect.right-pRectNode->theRect.left)) &&
		(lHeight==(pRectNode->theRect.bottom-pRectNode->theRect.top)) )
	{
		// The rect exactly fits; delete it
		if( pPrev==NULL )
		{
			m_Rects = pRectNode->next;
			delete pRectNode;
		}
		else
		{
			pPrev->next = pRectNode->next;
			delete pRectNode;
		}
	}
	else if( lWidth==(pRectNode->theRect.right-pRectNode->theRect.left) )
	{
		pRectNode->theRect.top += lHeight;
	}
	else if( lHeight==(pRectNode->theRect.bottom-pRectNode->theRect.top) )
	{
		pRectNode->theRect.left += lWidth;
	}
	else
	{
        RECT_NODE* SplitNode = new RECT_NODE;
        SplitNode->theRect.left = pRectNode->theRect.left + lWidth;
        SplitNode->theRect.top = pRectNode->theRect.top;
        SplitNode->theRect.right = pRectNode->theRect.right;
        SplitNode->theRect.bottom= SplitNode->theRect.top + lHeight;
        pRectNode->theRect.top += lHeight;
        if( pPrev==NULL )
        {
            SplitNode->next = pRectNode;
            m_Rects = SplitNode;
        }
        else
        {
            pPrev->next = SplitNode;
            SplitNode->next = pRectNode;
        }
	}

	return TRUE;
}
// End util_RectFiller::FindSuitableRect()
//////////////////////////////////////////////////////////////////////
