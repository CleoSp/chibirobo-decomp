#if defined(VERSION_GGTP01)
const char sTypeNameCCameraActionChase[20]          = "CCameraActionChase";
const char sChaseTypeNameCInterfaceCameraAction[28] = "CInterfaceCameraAction";
#else
const char sTypeNameCCameraActionChase[20]          = "CCameraActionChase\0"
                                                      "0";
const char sChaseTypeNameCInterfaceCameraAction[28] = "CInterfaceCameraAction\0"
                                                      "00000";
#endif

const void* sChaseTypeInfoCInterfaceCameraAction[] = { sChaseTypeNameCInterfaceCameraAction, 0 };
const void* sBaseTypesCCameraActionChase[]         = { sChaseTypeInfoCInterfaceCameraAction, 0, 0 };
const void* gTypeInfoCCameraActionChase[]          = { sTypeNameCCameraActionChase, sBaseTypesCCameraActionChase };
