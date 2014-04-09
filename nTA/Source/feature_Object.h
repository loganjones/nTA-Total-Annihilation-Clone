// feature_Object.h //                             \author Logan Jones
/////////////////////                                 \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FEATURE_OBJECT_H_
#define _FEATURE_OBJECT_H_
/////////////////////////////////////////////////////////////////////


struct feature_Type;

/////////////////////////// feature_Object ///////////////////////////
//
class feature_Object : public physics_Object, public scene_Object
{

/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( std_Point ptPosition, struct feature_Type* pFeatureType );

	void Animate();
	void Render( std_Vector2 vOffset ) const;
	void Render();

	BOOL CanCollide();
	BOOL IsFlat();

	const feature_Type* GetType() const {return m_pFeatureType;}


/////////////////////////////////////////////////
// Data
protected:

	feature_Type*		m_pFeatureType;

	std_Vector3			m_RenderPosition;


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	feature_Object();
	virtual ~feature_Object();
/////////////////////////////////////////////////

}; // End class - feature_Object
/////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FEATURE_OBJECT_H_)
