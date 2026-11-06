const char sTypeNameCCameraActionScroll2[24]         = "CCameraActionScroll2";
const char sScrollTypeNameCInterfaceCameraAction[24] = "CInterfaceCameraAction";
#if defined(VERSION_GGTP01)
const char sScrollTypeNameGXTexObj[12] = "_GXTexObj";
#else
const char sScrollTypeNameGXTexObj[12] = "_GXTexObj\0"
                                         "00";
#endif

const void* sScrollTypeInfoCInterfaceCameraAction[] = { sScrollTypeNameCInterfaceCameraAction, 0 };
const void* sBaseTypesCCameraActionScroll2[]        = { sScrollTypeInfoCInterfaceCameraAction, 0, 0 };
const void* gTypeInfoCCameraActionScroll2[]         = { sTypeNameCCameraActionScroll2, sBaseTypesCCameraActionScroll2 };

#if defined(VERSION_GGTP01)
const char sScrollTypeNameVec[8]      = "Vec";
const void* sScrollTypeInfoGXTexObj[] = { sScrollTypeNameGXTexObj, 0 };
const void* sScrollTypeInfoVec[]      = { sScrollTypeNameVec, 0 };
#endif
