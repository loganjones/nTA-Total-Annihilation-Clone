// feature_Factory.h //                            \author Logan Jones
//////////////////////                                \date 2/12/2002
/// \file
/// \brief ...
/////////////////////////////////////////////////////////////////////
#ifndef _FEATURE_FACTORY_H_
#define _FEATURE_FACTORY_H_
/////////////////////////////////////////////////////////////////////

class gfx_Sprite;
typedef vector< gfx_Sprite*	>	SpriteList_t;

// Feature structure: Most everything identifying a particular feature goes here
struct feature_Type
{
	string				Name;
	string				Description;
	std_Size			Footprint;
	long				Height;

	DWORD				Flags;

	gfx_Sprite*			Image;
	std_Rect			ImageRect;
	gfx_Sprite*			Shadow;
	std_Rect			ShadowRect;

	SpriteList_t		Anim_Burn;
	DWORD				FeatureBurnt;
	SpriteList_t		Anim_Reclaim;
	DWORD				FeatureReclaimed;
	SpriteList_t		Anim_Die;
	DWORD				FeatureDead;

	std_Point			ViewOffset;
	std_Size			ViewSize;
};

enum feature_Type_Flags
{
	FTF_Animating		= 1,
	FTF_Flamable		= 2,
	FTF_Reclaimable		= 4,
	FTF_Blocking		= 8,
	FTF_Geothermal		= 16,
	FTF_Indestructible	= 32,
	FTF_Permanent		= 64,
	FTF_NoDisplayInfo	= 128,
};


#include "game_LoadFacilitator.h"

////////////////////////// feature_Factory //////////////////////////
//
class feature_Factory
{

/////////////////////////////////////////////////
// Type declarations
protected:

	struct FeatureToLoad_t
	{
		char				Name[ 32 ];
		int					SearchAdded;
	};
	typedef unordered_map< DWORD, FeatureToLoad_t >	FeatureToLoadList_t;

	typedef feature_Type						FeatureType_t;
	typedef unordered_map< DWORD, FeatureType_t >	FeatureTypeMap_t;

	struct FeatureFileInfo_t;


/////////////////////////////////////////////////
// INTERFACE
public:

	BOOL Create( BYTE* pTntFileBuffer, LPTSTR strWorld, game_LoadFacilitator Load );
	void Destroy();

	void Update(){}


/////////////////////////////////////////////////
// Data members
protected:

	FeatureToLoadList_t			m_FeaturesToLoad;
	DWORD						m_ToLoadCount;
	BOOL						m_NewFeatureAdded;
	int							m_LoadPass;
	BYTE*						m_CurrentAnimFile;
	char						m_CurrentAnimFileName[32];

	FeatureTypeMap_t			m_FeatureTypes;


/////////////////////////////////////////////////
// Utility methods
protected:

	INLINE BOOL IsInFeatureList( LPCTSTR strSearch );
	INLINE void AddToLoadList( LPCTSTR strNew, int iAddedWhen );
	INLINE BOOL IsInToLoadList( LPCTSTR strSearch );

	BOOL AddHardcodedFeatureTypes();
	BOOL ProcessDirectory( LPTSTR strDirectory, game_LoadFacilitator Load );
	BOOL ProcessTDF( void* hFile, LPCTSTR strHostDirectory, LPCTSTR strFileName, game_LoadFacilitator Load );
	BOOL AddFeatureType( LPCTSTR strName, FeatureFileInfo_t* pFeatureFileInfo );
	BOOL CreateFeatureImages( FeatureFileInfo_t* pFeatureFileInfo, FeatureType_t* pNewFeatureType );
	BOOL ExtractImage( LPTSTR strName, BYTE* pGafBuffer, struct gfx_Image_t* pImage, BYTE** ppFrameData );
	BOOL SpawnFeature( LPTSTR strType, long x, long y, BYTE z );
	INLINE FeatureType_t* GetFeatureType( LPTSTR strType );


/////////////////////////////////////////////////
// Type definitions
protected:


/////////////////////////////////////////////////
// Feature file info
protected:

	struct FeatureFileInfo_t
	{
		char			World[32];
		char			Description[64];
		char			Category[32];
		
		int				FootPrintX;
		int				FootPrintZ;
		int				Height;
		
		char			Object[32];
		char			FileName[32];
		char			SeqName[32];
		char			SeqNameShad[32];
		char			SeqNameReclamate[32];
		char			FeatureReclamate[32];
		char			SeqNameBurn[32];
		char			FeatureBurnt[32];
		char			SeqNameDie[32];
		char			FeatureDead[32];
		
		int				Energy;
		int				Metal;
		
		int				BurnMin;
		int				BurnMax;
		int				SparkTime;
		int				SpreadChance;
		char			BurnWeapon[32];
		
		int				AnimTrans;
		int				ShadTrans;
		int				Animating;
		int				Flamable;
		int				Reclaimable;
		int				Reproduce;
		int				ReproduceArea;
		int				Blocking;
		int				HitDensity;
		int				Damage;
		int				Geothermal;
		int				Indestructible;
		int				NoDisplayInfo;
		int				Permanent;
	};


/////////////////////////////////////////////////
// Default Constructor/Deconstructor
public:
	feature_Factory();
	virtual ~feature_Factory();
/////////////////////////////////////////////////

}; // End class - feature_Factory
/////////////////////////////////////////////////////////////////////


// Include inline implementaions here for a NON-debug build
#ifndef _DEBUG
 #include "feature_Factory.inl"
#endif // !defined( _DEBUG )


/////////////////////////////////////////////////////////////////////
#endif // !defined(_FEATURE_FACTORY_H_)
