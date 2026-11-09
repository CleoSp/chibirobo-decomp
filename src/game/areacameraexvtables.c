extern void GetCameraFlags__22CInterfaceCameraActionFv(void);
extern void GetFollowDivisor__22CInterfaceCameraActionFv(void);
extern void GetTarget__22CInterfaceCameraActionFv(void);
extern void Get__18CInterpolationNoneFv(void);
extern void Get__24CVectorNoneInterpolationFv(void);
extern void Init__22CInterfaceCameraActionFv(void);
extern void PreviousUpdate__22CInterfaceCameraActionFv(void);
extern void SetAngleZ__22CInterfaceCameraActionFf(void);
extern void SetAngle__22CInterfaceCameraActionFRC3Vec(void);
extern void SetAngle__22CInterfaceCameraActionFfff(void);
extern void SetCamera__22CInterfaceCameraActionFP7CCamera(void);
extern void SetDistance__22CInterfaceCameraActionFf(void);
extern void SetEye__22CInterfaceCameraActionFRC3Vec(void);
extern void SetEye__22CInterfaceCameraActionFfff(void);
extern void SetFov__22CInterfaceCameraActionFf(void);
extern void SetInterest__22CInterfaceCameraActionFRC3Vec(void);
extern void SetInterest__22CInterfaceCameraActionFfff(void);
extern void Start__18CInterpolationNoneFfff(void);
extern void Start__24CVectorNoneInterpolationF3Vec3Vecf(void);
extern void Step__18CInterpolationNoneFf(void);
extern void Step__24CVectorNoneInterpolationFf(void);
extern void Update__18CInterpolationNoneFUlf(void);
extern void Update__24CVectorNoneInterpolationFUlf(void);
extern void __dt__11CamAreaInfoFv(void);
extern void __dt__18CInterpolationNoneFv(void);
extern void __dt__20CCameraActionAreaExMFv(void);
extern void __dt__24CVectorNoneInterpolationFv(void);
extern const void* gTypeInfoCCameraActionAreaExM[];
extern const void* gTypeInfoCInterfaceCameraAction[];
extern const void* gTypeInfoCInterfaceInterpolation[];
extern const void* gTypeInfoCInterfaceVectorInterpolation[];
extern const void* gTypeInfoCInterpolationNone[];
extern const void* gTypeInfoCVectorNoneInterpolation[];
extern const void* gTypeInfoCamAreaInfo[];
extern void vf14__22CInterfaceCameraActionFv(void);
extern void vf20__22CInterfaceCameraActionFv(void);
extern void vf24__22CInterfaceCameraActionFv(void);
extern void vf28__22CInterfaceCameraActionFv(void);
extern void vf50__22CInterfaceCameraActionFv(void);
extern void vf5C__22CInterfaceCameraActionFv(void);
extern void vf64__20CCameraActionAreaExMFv(void);
extern void vf68__20CCameraActionAreaExMFv(void);
extern void vf6C__20CCameraActionAreaExMFv(void);
extern void vf70__20CCameraActionAreaExMFl(void);
extern void vf74__20CCameraActionAreaExMF3Vec(void);
extern void vf78__20CCameraActionAreaExMFl(void);
extern void vf7C__20CCameraActionAreaExMFl(void);

const void* sBaseTypesCInterpolationNone[]       = { gTypeInfoCInterfaceInterpolation, 0, 0 };
const void* __vt__18CInterpolationNone[]         = { gTypeInfoCInterpolationNone,  0,
	                                                 __dt__18CInterpolationNoneFv, Start__18CInterpolationNoneFfff,
	                                                 Step__18CInterpolationNoneFf, Update__18CInterpolationNoneFUlf,
	                                                 Get__18CInterpolationNoneFv };
const void* sBaseTypesCVectorNoneInterpolation[] = { gTypeInfoCInterfaceVectorInterpolation, 0, 0 };
const void* __vt__24CVectorNoneInterpolation[]   = { gTypeInfoCVectorNoneInterpolation,  0,
	                                                 __dt__24CVectorNoneInterpolationFv, Start__24CVectorNoneInterpolationF3Vec3Vecf,
	                                                 Step__24CVectorNoneInterpolationFf, Update__24CVectorNoneInterpolationFUlf,
	                                                 Get__24CVectorNoneInterpolationFv };
const void* sBaseTypesCCameraActionAreaExM[]     = { gTypeInfoCInterfaceCameraAction, 0, 0 };
const void* __vt__20CCameraActionAreaExM[]       = { gTypeInfoCCameraActionAreaExM,
	                                                 0,
	                                                 __dt__20CCameraActionAreaExMFv,
	                                                 PreviousUpdate__22CInterfaceCameraActionFv,
	                                                 0,
	                                                 vf14__22CInterfaceCameraActionFv,
	                                                 SetCamera__22CInterfaceCameraActionFP7CCamera,
	                                                 Init__22CInterfaceCameraActionFv,
	                                                 vf20__22CInterfaceCameraActionFv,
	                                                 vf24__22CInterfaceCameraActionFv,
	                                                 vf28__22CInterfaceCameraActionFv,
	                                                 SetEye__22CInterfaceCameraActionFfff,
	                                                 SetInterest__22CInterfaceCameraActionFfff,
	                                                 SetAngle__22CInterfaceCameraActionFfff,
	                                                 SetEye__22CInterfaceCameraActionFRC3Vec,
	                                                 SetInterest__22CInterfaceCameraActionFRC3Vec,
	                                                 SetAngle__22CInterfaceCameraActionFRC3Vec,
	                                                 SetDistance__22CInterfaceCameraActionFf,
	                                                 SetFov__22CInterfaceCameraActionFf,
	                                                 SetAngleZ__22CInterfaceCameraActionFf,
	                                                 vf50__22CInterfaceCameraActionFv,
	                                                 GetCameraFlags__22CInterfaceCameraActionFv,
	                                                 GetFollowDivisor__22CInterfaceCameraActionFv,
	                                                 vf5C__22CInterfaceCameraActionFv,
	                                                 GetTarget__22CInterfaceCameraActionFv,
	                                                 vf64__20CCameraActionAreaExMFv,
	                                                 vf68__20CCameraActionAreaExMFv,
	                                                 vf6C__20CCameraActionAreaExMFv,
	                                                 vf70__20CCameraActionAreaExMFl,
	                                                 vf74__20CCameraActionAreaExMF3Vec,
	                                                 vf78__20CCameraActionAreaExMFl,
	                                                 vf7C__20CCameraActionAreaExMFl };
const void* __vt__11CamAreaInfo[]                = { gTypeInfoCamAreaInfo, 0, __dt__11CamAreaInfoFv };
