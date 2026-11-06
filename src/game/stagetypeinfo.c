#if defined(VERSION_GGTP01)
const char sTypeNameOSModuleHeader[16] = "OSModuleHeader";
#else
const char sTypeNameOSModuleHeader[16] = "OSModuleHeader\0"
                                         "0";
#endif

const char sTypeNameGlobals[8] = "Globals";
const char sTypeNameCModule[8] = "CModule";
const char sTypeNameEnvInfo[8] = "EnvInfo";
#if defined(VERSION_GGTP01)
const char sTypeNameStage[8] = "Stage";
#else
const char sTypeNameStage[8] = "Stage\0"
                               "00";
#endif

#if defined(VERSION_GGTJ01_01)
extern const char sGlobalResourcePath[];
const char* gGlobalResourceFileName = sGlobalResourcePath;
#else
extern const char sPathGlobalsBin[];
const char* gGlobalResourceFileName = sPathGlobalsBin;
#endif
const void* gTypeInfoGlobals[]        = { sTypeNameGlobals, 0 };
const void* gTypeInfoOSModuleHeader[] = { sTypeNameOSModuleHeader, 0 };
extern const void* sBaseTypesCModule[];
const void* gTypeInfoCModule[] = { sTypeNameCModule, sBaseTypesCModule };
const void* gTypeInfoEnvInfo[] = { sTypeNameEnvInfo, 0 };
const void* gTypeInfoStage[]   = { sTypeNameStage, 0 };

extern void __dt__7GlobalsFv(void);
extern void __dt__7CModuleFv(void);
extern void __dt__7EnvInfoFv(void);
extern void __dt__5StageFv(void);
const void* __vt__7Globals[]    = { gTypeInfoGlobals, 0, __dt__7GlobalsFv };
const void* sBaseTypesCModule[] = { gTypeInfoOSModuleHeader, 0, 0 };
const void* __vt__7CModule[]    = { gTypeInfoCModule, 0, __dt__7CModuleFv };
const void* __vt__7EnvInfo[]    = { gTypeInfoEnvInfo, 0, __dt__7EnvInfoFv };
const void* __vt__5Stage[]      = { gTypeInfoStage, 0, __dt__5StageFv };
