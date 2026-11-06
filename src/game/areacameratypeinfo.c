extern const char sTypeNameCCameraActionArea[];
extern const char sAreaTypeNameCInterfaceCameraAction[];
extern const char sAreaTypeNameCCameraActionAreaExM[];

const void* gAreaTypeInfoCInterfaceCameraAction[] = { sAreaTypeNameCInterfaceCameraAction, 0 };
const void* sAreaBaseTypesCCameraActionAreaExM[]  = { gAreaTypeInfoCInterfaceCameraAction, 0, 0 };
const void* gAreaTypeInfoCCameraActionAreaExM[]   = { sAreaTypeNameCCameraActionAreaExM, sAreaBaseTypesCCameraActionAreaExM };
const void* sBaseTypesCCameraActionArea[]         = { gAreaTypeInfoCInterfaceCameraAction, 0, gAreaTypeInfoCCameraActionAreaExM, 0, 0 };
const void* __RTTI__17CCameraActionArea[]         = { sTypeNameCCameraActionArea, sBaseTypesCCameraActionArea };
