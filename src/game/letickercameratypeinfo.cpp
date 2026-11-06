extern "C" {
extern const char sTypeNameCCameraActionLeticker[]          = "CCameraActionLeticker";
extern const char sLetickerTypeNameCInterfaceCameraAction[] = "CInterfaceCameraAction";
extern const char sLetickerTypeNameCCameraActionLadr[]      = "CCameraActionLadr";
const void* gLetickerTypeInfoCInterfaceCameraAction[]       = { sLetickerTypeNameCInterfaceCameraAction, 0 };
const void* sLetickerBaseTypesCCameraActionLadr[]           = { gLetickerTypeInfoCInterfaceCameraAction, 0, 0 };
const void* gLetickerTypeInfoCCameraActionLadr[]            = { sLetickerTypeNameCCameraActionLadr, sLetickerBaseTypesCCameraActionLadr };
const void* sBaseTypesCCameraActionLeticker[] = { gLetickerTypeInfoCInterfaceCameraAction, 0, gLetickerTypeInfoCCameraActionLadr, 0, 0 };
const void* __RTTI__21CCameraActionLeticker[] = { sTypeNameCCameraActionLeticker, sBaseTypesCCameraActionLeticker };
}
