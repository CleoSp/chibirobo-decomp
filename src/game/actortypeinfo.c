extern const char sTypeNameXObjActor[];
extern const char sActorIntpTypeName[];
const char sActorCJObjTypeName[] = "CJObj";
const char sActorCXObjTypeName[] = "CXObj";
const char sActorXObjTypeName[]  = "XObj";

const void* gActorTypeInfoCJObj[]      = { sActorCJObjTypeName, 0 };
const void* sActorBaseTypesCXObj[]     = { gActorTypeInfoCJObj, 0, 0 };
const void* gActorTypeInfoCXObj[]      = { sActorCXObjTypeName, sActorBaseTypesCXObj };
const void* sActorBaseTypesCXObjIntp[] = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, 0 };
const void* gActorTypeInfoCXObjIntp[]  = { sActorIntpTypeName, sActorBaseTypesCXObjIntp };
const void* sActorBaseTypesXObj[]      = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, gActorTypeInfoCXObjIntp, 0, 0 };
const void* gActorTypeInfoXObj[]       = { sActorXObjTypeName, sActorBaseTypesXObj };
const void* sBaseTypesXObjActor[]
    = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, gActorTypeInfoCXObjIntp, 0, gActorTypeInfoXObj, 0, 0 };
const void* gTypeInfoXObjActor[] = { sTypeNameXObjActor, sBaseTypesXObjActor };

extern void ApplyTransform__9CXObjIntpFv(void);
extern void ClearAnim__4XObjFv(void);
extern void DrawDebugStatus__9XObjActorFv(void);
extern void DrawGroundShadowAt__4XObjFPC3Vecf(void);
extern void DrawGroundShadow__4XObjFv(void);
extern void DrawObjectEffects__4XObjFv(void);
extern void DrawReflection__4XObjFPA4_f(void);
extern void DrawWithView__4XObjFPA4_f(void);
extern void ExpandShadowBounds__4XObjFP7CShadow(void);
extern void GetCameraTarget__9XObjActorFv(void);
extern void GetCollisionHeight__4XObjFv(void);
extern void GetFloorXZExtent__4XObjFv(void);
extern void GetHeadJoint__9XObjActorFv(void);
extern void GetHeadWorldPosition__9XObjActorFv(void);
extern void GetJointAnimation__4XObjFl(void);
extern void GetMaterialAnimation__4XObjFl(void);
extern void GetMaxHeadPitch__9XObjActorFv(void);
extern void GetMaxHeadYaw__9XObjActorFv(void);
extern void GetMinHeadPitch__9XObjActorFv(void);
extern void GetMinHeadYaw__9XObjActorFv(void);
extern void GetShadowRadius__4XObjFv(void);
extern void GetShapeAnimation__4XObjFl(void);
extern void HandleEventCommand__9XObjActorFlPl(void);
extern void InitializeObject__9XObjActorFUl(void);
extern void LoadAnimationArchiveAsync__4XObjFPCc(void);
extern void LoadFromPlacement__9XObjActorFRC9CoordInfo(void);
extern void OnHitFromPosition__9XObjActorFRC3Vecf(void);
extern void OnPushDisplacement__9XObjActorFRC3Vec(void);
extern void ProjectOffsetToScreen__4XObjFPC3VecPfPfPfP7CCamera(void);
extern void ProjectVisibleBounds__4XObjFPfPfP7CCameraff(void);
extern void ReadAsync__5CXObjFP11CObjArchivePCcUlUlUlUl(void);
extern void ReadCheck__9CXObjIntpFv(void);
extern void RegisterShadowObjects__4XObjFP7CShadow(void);
extern void Release__4XObjFv(void);
extern void ResetActorState__9XObjActorFv(void);
extern void ResetObjectState__9XObjActorFv(void);
extern void SetAnimRate__5CXObjFfUl(void);
extern void SetAnim__4XObjFliPvPvPvffff(void);
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
extern void SetScaleY__5CXObjFfl(void);
extern void SetScaleZ__5CXObjFfl(void);
extern void SetScale__5CXObjFRC3Vecl(void);
extern void SetScale__5CXObjFfffl(void);
extern void SetScale__5CXObjFfl(void);
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
extern void UpdateAnimationSounds__9XObjActorFf(void);
extern void UpdateObject__9XObjActorFUl(void);
extern void __dt__9XObjActorFv(void);
extern void vf100__9XObjActorFll(void);
extern void vf11C__4XObjFP7CCamera(void);
extern void vf120__4XObjFl(void);
extern void vf128__4XObjFv(void);
extern void vf12C__4XObjFlUcUcUcf(void);
extern void vf134__4XObjFf(void);
extern void vf13C__4XObjFPC3Vecll(void);
extern void vf140__4XObjFPC3Vecl(void);
extern void vf144__4XObjFll(void);
extern void vf148__9XObjActorFUl(void);
extern void vf154__9XObjActorFUl(void);
extern void vf160__9XObjActorFv(void);
extern void vf94__9XObjActorFv(void);
extern void vfA8__4XObjFUl(void);
extern void vfAC__9XObjActorFv(void);
extern void vfC4__4XObjFv(void);
extern void vfC8__9XObjActorFv(void);
extern void vfD8__4XObjFPUl(void);
extern void vfDC__4XObjFiUc(void);
extern void vfE0__4XObjFf(void);
extern void vfE4__4XObjFf(void);
extern void vfE8__4XObjFv(void);
extern void vfFC__4XObjFPC3VecPC3VecffP3VecP3Vecll(void);

const void* __vt__9XObjActor[] = { gTypeInfoXObjActor,
	                               0,
	                               __dt__9XObjActorFv,
	                               SetTranslateX__5CXObjFfl,
	                               SetTranslateY__5CXObjFfl,
	                               SetTranslateZ__5CXObjFfl,
	                               SetScaleX__5CXObjFfl,
	                               SetScaleY__5CXObjFfl,
	                               SetScaleZ__5CXObjFfl,
	                               SetRotationX__5CXObjFfl,
	                               SetRotationY__5CXObjFfl,
	                               SetRotationZ__5CXObjFfl,
	                               SetTranslate__5CJObjFRC3Vecl,
	                               SetTranslate__5CJObjFfffl,
	                               SetScale__5CXObjFRC3Vecl,
	                               SetScale__5CXObjFfffl,
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
	                               SetScale__5CXObjFfl,
	                               SetRot__5CXObjFRC3Vecl,
	                               SetRot__5CXObjFfffl,
	                               SetMoveDir__5CXObjFPC3Vecf,
	                               SetRotateY__5CXObjFff,
	                               SetAnim__4XObjFliPvPvPvffff,
	                               ClearAnim__4XObjFv,
	                               UpdateAnim__9CXObjIntpFUl,
	                               ApplyTransform__9CXObjIntpFv,
	                               InitializeObject__9XObjActorFUl,
	                               vf94__9XObjActorFv,
	                               ResetObjectState__9XObjActorFv,
	                               LoadFromPlacement__9XObjActorFRC9CoordInfo,
	                               LoadAnimationArchiveAsync__4XObjFPCc,
	                               UpdateObject__9XObjActorFUl,
	                               vfA8__4XObjFUl,
	                               vfAC__9XObjActorFv,
	                               DrawWithView__4XObjFPA4_f,
	                               DrawReflection__4XObjFPA4_f,
	                               DrawGroundShadow__4XObjFv,
	                               DrawGroundShadowAt__4XObjFPC3Vecf,
	                               DrawObjectEffects__4XObjFv,
	                               vfC4__4XObjFv,
	                               vfC8__9XObjActorFv,
	                               DrawDebugStatus__9XObjActorFv,
	                               RegisterShadowObjects__4XObjFP7CShadow,
	                               ExpandShadowBounds__4XObjFP7CShadow,
	                               vfD8__4XObjFPUl,
	                               vfDC__4XObjFiUc,
	                               vfE0__4XObjFf,
	                               vfE4__4XObjFf,
	                               vfE8__4XObjFv,
	                               GetCameraTarget__9XObjActorFv,
	                               GetJointAnimation__4XObjFl,
	                               GetMaterialAnimation__4XObjFl,
	                               GetShapeAnimation__4XObjFl,
	                               vfFC__4XObjFPC3VecPC3VecffP3VecP3Vecll,
	                               vf100__9XObjActorFll,
	                               TestFloorCrossing__4XObjFRC3VecRC3Vec,
	                               GetShadowRadius__4XObjFv,
	                               ProjectOffsetToScreen__4XObjFPC3VecPfPfPfP7CCamera,
	                               GetFloorXZExtent__4XObjFv,
	                               GetCollisionHeight__4XObjFv,
	                               ProjectVisibleBounds__4XObjFPfPfP7CCameraff,
	                               vf11C__4XObjFP7CCamera,
	                               vf120__4XObjFl,
	                               HandleEventCommand__9XObjActorFlPl,
	                               vf128__4XObjFv,
	                               vf12C__4XObjFlUcUcUcf,
	                               OnPushDisplacement__9XObjActorFRC3Vec,
	                               vf134__4XObjFf,
	                               OnHitFromPosition__9XObjActorFRC3Vecf,
	                               vf13C__4XObjFPC3Vecll,
	                               vf140__4XObjFPC3Vecl,
	                               vf144__4XObjFll,
	                               vf148__9XObjActorFUl,
	                               0,
	                               0,
	                               vf154__9XObjActorFUl,
	                               0,
	                               ResetActorState__9XObjActorFv,
	                               vf160__9XObjActorFv,
	                               UpdateAnimationSounds__9XObjActorFf,
	                               GetHeadJoint__9XObjActorFv,
	                               GetHeadWorldPosition__9XObjActorFv,
	                               GetMaxHeadYaw__9XObjActorFv,
	                               GetMinHeadYaw__9XObjActorFv,
	                               GetMaxHeadPitch__9XObjActorFv,
	                               GetMinHeadPitch__9XObjActorFv };
