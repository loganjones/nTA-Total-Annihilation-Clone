// ta_3DO.h //                                     \author Logan Jones
/////////////                                         \date 2/22/2001
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _TA_3DO_H_
#define _TA_3DO_H_
#pragma pack(1)
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
// NOTE: Most of the content in this file was derived from the document
//       ta-3DO-fmt.txt by Dan Melchione.


// TODO: Prototype 3DO utility functions


/////////////////////////////////////////////////////////////////////
// A 3DO contains a tree of 3D objects used in the composition of
// a model. Each object uses this structure to collect its data. The
// parent object is at the head of the file, every other object can
// be obtained from the parent.
typedef struct TA_3DO_OBJECT
{
    // Version of the 3DO file. Should always be 0x00000001
    UINT32		Version;

    // Specifies the number of unique vertecies for this object
    UINT32		NumberOfVertexes;

    // Specifies the number of unique primitives for this object
    UINT32		NumberOfPrimitives;

    // The primitive index for the model's ground plate.
    // If this is -1, there is no ground plate primitive in this object
    INT32		GroundPlateIndex;

    // The translation offset of this object from its parent
    INT32		XFromParent;
    INT32		YFromParent;
    INT32		ZFromParent;

    // The offset in the file at which this object's name resides
    // The name is a NULL terminated character string
    UINT32		OffsetToObjectName;

    // This is always 0x00000000
    UINT32		Unknown_1;

    // The offset in the file at which this object's vertex array resides
    // This array consists of TA_3DO_VERTEX[ NumberOfVertexes ]
    UINT32		OffsetToVertexArray;

    // The offset in the file at which this object's primitive array resides
    // This array consists of TA_3DO_PRIMITIVE[ NumberOfPrimitives ]
    UINT32		OffsetToPrimitiveArray;

    // The offset in the file to an object that shares its parent with this one.
    // If this is 0, there is no sibling.
    UINT32		OffsetToSiblingObject;

    // The offset in the file to an object that has this object as its parent.
    // If this is 0, there is no child.
    UINT32		OffsetToChildObject;

} *LPTA_3DO_OBJECT;

// The standard version for all 3DO files
#define TA_3DO_VERSION_STANDARD             1


/////////////////////////////////////////////////////////////////////
// The stucture of each vertex used by the objects
typedef struct TA_3DO_VERTEX
{
    // The respective coordinates for the vertex
	INT32		x;
	INT32		y;
	INT32		z;

} *LPTA_3DO_VERTEX;


/////////////////////////////////////////////////////////////////////
// The stucture of each vertex used by the objects
typedef struct TA_3DO_PRIMITIVE
{
    // If there is no texture, this specifies the primitive's color.
    UINT32		Color;

    // Specifies the number of vertecies used by the primitive
    UINT32		NumberOfVertexIndexes;

    // This is always 0
    INT32		Unknown_1;

    // The offset in the file at which this primitive's vertex index array resides.
    // This array consists of WORD[ NumberOfVertexIndexes ]
    UINT32		OffsetToVertexIndexArray;

    // The offset in the file at which this primitive's texture name resides
    // The name is a NULL terminated character string
    // If this is 0, there is no texture, there is a color value in Color
    UINT32		OffsetToTextureName;

    // "Cavedog(tm) specific data used for their editor", Dan Melchione.
    INT32		CavedogSpecific_1;
    INT32		CavedogSpecific_2;
    INT32		CavedogSpecific_3;

} *LPTA_3DO_PRIMITIVE_3DO;


/////////////////////////////////////////////////////////////////////
#pragma pack()
#endif // !defined(_TA_3DO_H_)
