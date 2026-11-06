const char gHudPauseLabel[20]             = "\\B00000080 PAUSE ";
const char sHudJointRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sHudGXTexObjTypeName[12]       = "_GXTexObj";
#if defined(VERSION_GGTP01)
const char sInfoPointTypeName[16] = "InfoPoint";
#else
const char sInfoPointTypeName[16] = "InfoPoint\0"
                                    "000000";
#endif

const void* gTypeInfoInfoPoint[] = { sInfoPointTypeName, 0 };
