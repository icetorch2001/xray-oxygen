// ModelPool.h: interface for the CModelPool class.
//////////////////////////////////////////////////////////////////////
#pragma once

// refs
class  dxRender_Visual;
namespace PS	{ 
	struct  SEmitter; 
};

// defs
class ECORE_API CModelPool
{
private:
	friend class CRender;

	struct str_pred 
	{	
		IC bool operator()(const shared_str& x, const shared_str& y) const
		{	return xr_strcmp(x,y)<0;}
	};
	struct ModelDef
	{
		shared_str			name;
		dxRender_Visual*		model;
        u32					refs;
        ModelDef()			{ refs=0;model=0; }
	};

	typedef xr_multimap<shared_str,dxRender_Visual*,str_pred>	POOL;
	typedef POOL::iterator										POOL_IT;
	typedef xr_map<dxRender_Visual*,shared_str>					REGISTRY;
	typedef REGISTRY::iterator									REGISTRY_IT;
private:
	xr_vector<ModelDef>			Models;				// Reference / Base
	xr_vector<dxRender_Visual*>	ModelsToDelete;		// 
	REGISTRY					Registry;			// Just pairing of pointer / Name
	POOL						Pool;				// Unused / Inactive
	BOOL						bLogging;
    BOOL						bForceDiscard;
    BOOL						bAllowChildrenDuplicate;
	std::mutex					mtPeref;
	void						Destroy	();
public:
								 CModelPool			();
	virtual 					~CModelPool			();
	dxRender_Visual*			Instance_Create		(u32 Type);
	dxRender_Visual*			Instance_Duplicate	(dxRender_Visual* V);
	dxRender_Visual*			Instance_Load		(const char* N, BOOL allow_register);
	dxRender_Visual*			Instance_Load		(const char* N, IReader* data, BOOL allow_register);
	void						Instance_Register	(const char* N, dxRender_Visual* V);
	dxRender_Visual*			Instance_Find		(const char* N);
	dxRender_Visual*			CreatePE			(PS::CPEDef* source);
	dxRender_Visual*			CreatePG			(PS::CPGDef* source);
	dxRender_Visual*			Create				(const char* name, IReader* data=0);
	dxRender_Visual*			CreateChild			(const char* name, IReader* data);
	void					Delete				(dxRender_Visual* &V, BOOL bDiscard=FALSE);
	void					Discard				(dxRender_Visual* &V, BOOL b_complete);
	void					DeleteInternal		(dxRender_Visual* &V, BOOL bDiscard=FALSE);
	void					DeleteQueue			();

	void					Logging				(BOOL bEnable)	{ bLogging=bEnable; }
	
	void					Prefetch			();
	void					ClearPool			( BOOL b_complete );

	void					dump 				();

	void					memory_stats		( u32& vb_mem_video, u32& vb_mem_system, u32& ib_mem_video, u32& ib_mem_system );
#ifdef _EDITOR    
	void					OnDeviceDestroy		();
	void 					Render				(dxRender_Visual* m_pVisual, const Fmatrix& mTransform, int priority, bool strictB2F, float m_fLOD);
	void 					RenderSingle		(dxRender_Visual* m_pVisual, const Fmatrix& mTransform, float m_fLOD);
#endif
private:
	dxRender_Visual*		TryLoadObject		(const char* N);
	dxRender_Visual*		TryLoadOgf			(const char* N);
};