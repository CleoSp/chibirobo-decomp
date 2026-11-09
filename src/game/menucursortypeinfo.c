extern const char sTypeNameMenuCursor[];
extern const char sStageTypeNameCXObjIntp[];

#if defined(VERSION_GGTP01)
const char sMenuCursorTypeNameCJObj[]  = "CJObj";
const char sMenuCursorTypeNameCXObj[]  = "CXObj";
const char sMenuCursorTypeNameXObj[]   = "XObj";
const char sMenuCursorTypeNameCursor[] = "Cursor";
#else
// US and JP pad the small-data strings with ASCII zeroes.
const char sMenuCursorTypeNameCJObj[8]  = "CJObj\0"
                                          "00";
const char sMenuCursorTypeNameCXObj[8]  = "CXObj\0"
                                          "00";
const char sMenuCursorTypeNameXObj[8]   = "XObj\0"
                                          "000";
const char sMenuCursorTypeNameCursor[8] = "Cursor\0"
                                          "0";
#endif

const void* gMenuCursorTypeInfoCJObj[]      = { sMenuCursorTypeNameCJObj, 0 };
const void* sMenuCursorBaseTypesCXObj[]     = { gMenuCursorTypeInfoCJObj, 0, 0 };
const void* gMenuCursorTypeInfoCXObj[]      = { sMenuCursorTypeNameCXObj, sMenuCursorBaseTypesCXObj };
const void* sMenuCursorBaseTypesCXObjIntp[] = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, 0 };
const void* gMenuCursorTypeInfoCXObjIntp[]  = { sStageTypeNameCXObjIntp, sMenuCursorBaseTypesCXObjIntp };
const void* sMenuCursorBaseTypesXObj[] = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, gMenuCursorTypeInfoCXObjIntp, 0, 0 };
const void* gMenuCursorTypeInfoXObj[]  = { sMenuCursorTypeNameXObj, sMenuCursorBaseTypesXObj };
const void* sMenuCursorBaseTypesCursor[]
    = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, gMenuCursorTypeInfoCXObjIntp, 0, gMenuCursorTypeInfoXObj, 0, 0 };
const void* gMenuCursorTypeInfoCursor[]      = { sMenuCursorTypeNameCursor, sMenuCursorBaseTypesCursor };
const void* sMenuCursorBaseTypesMenuCursor[] = { gMenuCursorTypeInfoCJObj,
	                                             0,
	                                             gMenuCursorTypeInfoCXObj,
	                                             0,
	                                             gMenuCursorTypeInfoCXObjIntp,
	                                             0,
	                                             gMenuCursorTypeInfoXObj,
	                                             0,
	                                             gMenuCursorTypeInfoCursor,
	                                             0,
	                                             0 };
const void* __RTTI__10MenuCursor[]           = { sTypeNameMenuCursor, sMenuCursorBaseTypesMenuCursor };

extern void ApplyTransform__9CXObjIntpFv(void);
extern void ClearAnim__4XObjFv(void);
extern void DrawDebugStatus__4XObjFv(void);
extern void DrawGroundShadowAt__4XObjFPC3Vecf(void);
extern void DrawGroundShadow__4XObjFv(void);
extern void DrawObjectEffects__4XObjFv(void);
extern void DrawReflection__4XObjFPA4_f(void);
extern void DrawWithView__4XObjFPA4_f(void);
extern void ExpandShadowBounds__4XObjFP7CShadow(void);
extern void GetCameraTarget__4XObjFv(void);
extern void GetCollisionHeight__4XObjFv(void);
extern void GetFloorXZExtent__4XObjFv(void);
extern void GetJointAnimation__4XObjFl(void);
extern void GetMaterialAnimation__4XObjFl(void);
extern void GetShadowRadius__4XObjFv(void);
extern void GetShapeAnimation__4XObjFl(void);
extern void HandleEventCommand__4XObjFlPl(void);
extern void InitializeObject__6CursorFUl(void);
extern void LoadAnimationArchiveAsync__4XObjFPCc(void);
extern void LoadFromPlacement__6CursorFRC9CoordInfo(void);
extern void OnHitFromPosition__4XObjFRC3Vecf(void);
extern void OnPushDisplacement__4XObjFRC3Vec(void);
extern void ProjectOffsetToScreen__4XObjFPC3VecPfPfPfP7CCamera(void);
extern void ProjectVisibleBounds__4XObjFPfPfP7CCameraff(void);
extern void ReadAsync__5CXObjFP11CObjArchivePCcUlUlUlUl(void);
extern void ReadCheck__9CXObjIntpFv(void);
extern void RegisterShadowObjects__4XObjFP7CShadow(void);
extern void Release__4XObjFv(void);
extern void ResetObjectState__10MenuCursorFv(void);
extern void SetAnimRate__5CXObjFfUl(void);
extern void SetAnim__6CursorFliPvPvPvffff(void);
extern void SetMoveDir__5CXObjFPC3Vecf(void);
extern void SetPos__5CXObjFRC3Vecl(void);
extern void SetPos__5CXObjFfffl(void);
extern void SetRot__5CXObjFRC3Vecl(void);
extern void SetRot__5CXObjFfffl(void);
extern void SetRotateY__5CXObjFff(void);
extern void SetRotationX__5CXObjFfl(void);
extern void SetRotationY__5CXObjFfl(void);
extern void SetRotationZ__5CXObjFfl(void);
extern void SetRotation__5CJObjFRC3Vecl(void);
extern void SetRotation__5CJObjFfffl(void);
extern void SetScaleX__5CXObjFfl(void);
extern void SetScaleY__6CursorFfl(void);
extern void SetScaleZ__5CXObjFfl(void);
extern void SetScale__6CursorFRC3Vecl(void);
extern void SetScale__6CursorFfffl(void);
extern void SetScale__6CursorFfl(void);
extern void SetTranslateX__5CXObjFfl(void);
extern void SetTranslateY__5CXObjFfl(void);
extern void SetTranslateZ__5CXObjFfl(void);
extern void SetTranslate__5CJObjFRC3Vecl(void);
extern void SetTranslate__5CJObjFfffl(void);
extern void StepRotate__9CXObjIntpFUl(void);
extern void StepScale__9CXObjIntpFUl(void);
extern void StepTranslate__9CXObjIntpFUl(void);
extern void TestFloorCrossing__4XObjFRC3VecRC3Vec(void);
extern void UpdateAnim__9CXObjIntpFUl(void);
extern void UpdateObject__10MenuCursorFUl(void);
extern const void* __RTTI__3Vec[];
extern void __dt__10MenuCursorFv(void);
extern const void* gStageTypeInfoCVec[];
extern void vf100__4XObjFll(void);
extern void vf11C__4XObjFP7CCamera(void);
extern void vf120__4XObjFl(void);
extern void vf128__4XObjFv(void);
extern void vf12C__4XObjFlUcUcUcf(void);
extern void vf134__4XObjFf(void);
extern void vf13C__4XObjFPC3Vecll(void);
extern void vf140__4XObjFPC3Vecl(void);
extern void vf144__4XObjFll(void);
extern void vf94__4XObjFv(void);
extern void vfA8__4XObjFUl(void);
extern void vfAC__6CursorFv(void);
extern void vfC4__4XObjFv(void);
extern void vfC8__4XObjFv(void);
extern void vfD8__4XObjFPUl(void);
extern void vfDC__4XObjFiUc(void);
extern void vfE0__4XObjFf(void);
extern void vfE4__4XObjFf(void);
extern void vfE8__4XObjFv(void);
extern void vfFC__4XObjFPC3VecPC3VecffP3VecP3Vecll(void);

const void* __vt__10MenuCursor[] = { __RTTI__10MenuCursor,
	                                 0,
	                                 __dt__10MenuCursorFv,
	                                 SetTranslateX__5CXObjFfl,
	                                 SetTranslateY__5CXObjFfl,
	                                 SetTranslateZ__5CXObjFfl,
	                                 SetScaleX__5CXObjFfl,
	                                 SetScaleY__6CursorFfl,
	                                 SetScaleZ__5CXObjFfl,
	                                 SetRotationX__5CXObjFfl,
	                                 SetRotationY__5CXObjFfl,
	                                 SetRotationZ__5CXObjFfl,
	                                 SetTranslate__5CJObjFRC3Vecl,
	                                 SetTranslate__5CJObjFfffl,
	                                 SetScale__6CursorFRC3Vecl,
	                                 SetScale__6CursorFfffl,
	                                 SetRotation__5CJObjFRC3Vecl,
	                                 SetRotation__5CJObjFfffl,
	                                 SetAnimRate__5CXObjFfUl,
	                                 ReadAsync__5CXObjFP11CObjArchivePCcUlUlUlUl,
	                                 ReadCheck__9CXObjIntpFv,
	                                 Release__4XObjFv,
	                                 StepRotate__9CXObjIntpFUl,
	                                 StepTranslate__9CXObjIntpFUl,
	                                 StepScale__9CXObjIntpFUl,
	                                 SetPos__5CXObjFRC3Vecl,
	                                 SetPos__5CXObjFfffl,
	                                 SetScale__6CursorFfl,
	                                 SetRot__5CXObjFRC3Vecl,
	                                 SetRot__5CXObjFfffl,
	                                 SetMoveDir__5CXObjFPC3Vecf,
	                                 SetRotateY__5CXObjFff,
	                                 SetAnim__6CursorFliPvPvPvffff,
	                                 ClearAnim__4XObjFv,
	                                 UpdateAnim__9CXObjIntpFUl,
	                                 ApplyTransform__9CXObjIntpFv,
	                                 InitializeObject__6CursorFUl,
	                                 vf94__4XObjFv,
	                                 ResetObjectState__10MenuCursorFv,
	                                 LoadFromPlacement__6CursorFRC9CoordInfo,
	                                 LoadAnimationArchiveAsync__4XObjFPCc,
	                                 UpdateObject__10MenuCursorFUl,
	                                 vfA8__4XObjFUl,
	                                 vfAC__6CursorFv,
	                                 DrawWithView__4XObjFPA4_f,
	                                 DrawReflection__4XObjFPA4_f,
	                                 DrawGroundShadow__4XObjFv,
	                                 DrawGroundShadowAt__4XObjFPC3Vecf,
	                                 DrawObjectEffects__4XObjFv,
	                                 vfC4__4XObjFv,
	                                 vfC8__4XObjFv,
	                                 DrawDebugStatus__4XObjFv,
	                                 RegisterShadowObjects__4XObjFP7CShadow,
	                                 ExpandShadowBounds__4XObjFP7CShadow,
	                                 vfD8__4XObjFPUl,
	                                 vfDC__4XObjFiUc,
	                                 vfE0__4XObjFf,
	                                 vfE4__4XObjFf,
	                                 vfE8__4XObjFv,
	                                 GetCameraTarget__4XObjFv,
	                                 GetJointAnimation__4XObjFl,
	                                 GetMaterialAnimation__4XObjFl,
	                                 GetShapeAnimation__4XObjFl,
	                                 vfFC__4XObjFPC3VecPC3VecffP3VecP3Vecll,
	                                 vf100__4XObjFll,
	                                 TestFloorCrossing__4XObjFRC3VecRC3Vec,
	                                 GetShadowRadius__4XObjFv,
	                                 ProjectOffsetToScreen__4XObjFPC3VecPfPfPfP7CCamera,
	                                 GetFloorXZExtent__4XObjFv,
	                                 GetCollisionHeight__4XObjFv,
	                                 ProjectVisibleBounds__4XObjFPfPfP7CCameraff,
	                                 vf11C__4XObjFP7CCamera,
	                                 vf120__4XObjFl,
	                                 HandleEventCommand__4XObjFlPl,
	                                 vf128__4XObjFv,
	                                 vf12C__4XObjFlUcUcUcf,
	                                 OnPushDisplacement__4XObjFRC3Vec,
	                                 vf134__4XObjFf,
	                                 OnHitFromPosition__4XObjFRC3Vecf,
	                                 vf13C__4XObjFPC3Vecll,
	                                 vf140__4XObjFPC3Vecl,
	                                 vf144__4XObjFll,
	                                 __RTTI__3Vec,
	                                 0,
	                                 gStageTypeInfoCVec,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0,
	                                 0 };
