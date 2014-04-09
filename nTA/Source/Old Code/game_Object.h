// game_Object.h //                        Author: Logan "Burn" Jones
//////////////////                                   Date: 12/29/2001
//
/////////////////////////////////////////////////////////////////////
#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_
/////////////////////////////////////////////////////////////////////


enum ObjectFlags_t
{
	OBJ_HAS_INFO		=	1,
	OBJ_SELECTABLE		=	2,
};

// This type descibes the ocupation level of a particular object,
// and affects it placement in the sorted scen list
enum ObjectSceneLevels_t
{
	LEVEL_Lowest,			// Lowest possible level for an object. All others are drawn on top.
	LEVEL_UnderWater,		// Objects under water, sea floor, etc
	LEVEL_Gound,			// Most common level. Anything on the ground (or at sea level)
	LEVEL_Air,				// Objects in the air
	LEVEL_Highest,			// Whatever remains. All others are drawn below
};

// This type provides a hint to what the derived object type is for a particuar object
enum ObjectTypes_t
{
	ObjType_Unit,
	ObjType_Feature,
};


//////////////////////////// game_Object ////////////////////////////
//
class game_Object
{

/////////////////////////////////////////////////
// INTERFACE
public:

	virtual BOOL Create( std_RectF& rctBounds );
	virtual void Update() = 0;
	virtual void Render( std_Vector2 vOffset ) const = 0;

	// Proccesses a movement frame and updates the velocity.
	// The default does nothing
	virtual void DoMovement(){};

	inline void SetHeight( float fHeight ){ m_Height=fHeight; m_HeightOffset.Set(0,fHeight/2.0f); }

	virtual BOOL CanCollide(){ return TRUE; }
	virtual BOOL IsFlat(){ return FALSE; };

	// Events
	virtual BOOL OnCollision( game_Object* pCollidee ){ return FALSE; };


/////////////////////////////////////////////////
// Data members
public:

	// Position stuff
	std_RectF				m_BaseRect;
	std_RectF				m_ScreenRect;
	float					m_Elevation;
	//float					m_Height;
	std_Vector3				m_Velocity;

	ObjectSceneLevels_t		m_SceneLevel;
	ObjectTypes_t			m_ObjectType;

	BYTE					m_Height;
	std_Vector2				m_HeightOffset;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	game_Object( ObjectTypes_t Type );
	virtual ~game_Object();
/////////////////////////////////////////////////

}; // End class - game_Object
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_GAME_OBJECT_H_)
