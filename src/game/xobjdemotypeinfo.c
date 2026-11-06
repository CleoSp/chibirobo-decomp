const char sTypeNameXObjDemo[12]      = "XObjDemo";
const char sDemoTypeNameCXObjIntp[12] = "CXObjIntp";
#if defined(VERSION_GGTP01)
const char sDemoTypeNameCJObj[] = "CJObj";
const char sDemoTypeNameCXObj[] = "CXObj";
const char sDemoTypeNameXObj[]  = "XObj";
#else
// These editions pad the small-data strings with ASCII zeroes.
const char sDemoTypeNameCJObj[8] = "CJObj\0"
                                   "00";
const char sDemoTypeNameCXObj[8] = "CXObj\0"
                                   "00";
const char sDemoTypeNameXObj[8]  = "XObj\0"
                                   "000";
#endif

const void* gDemoTypeInfoCJObj[]      = { sDemoTypeNameCJObj, 0 };
const void* sDemoBaseTypesCXObj[]     = { gDemoTypeInfoCJObj, 0, 0 };
const void* gDemoTypeInfoCXObj[]      = { sDemoTypeNameCXObj, sDemoBaseTypesCXObj };
const void* sDemoBaseTypesCXObjIntp[] = { gDemoTypeInfoCJObj, 0, gDemoTypeInfoCXObj, 0, 0 };
const void* gDemoTypeInfoCXObjIntp[]  = { sDemoTypeNameCXObjIntp, sDemoBaseTypesCXObjIntp };
const void* sDemoBaseTypesXObj[]      = { gDemoTypeInfoCJObj, 0, gDemoTypeInfoCXObj, 0, gDemoTypeInfoCXObjIntp, 0, 0 };
const void* gDemoTypeInfoXObj[]       = { sDemoTypeNameXObj, sDemoBaseTypesXObj };
const void* sBaseTypesXObjDemo[] = { gDemoTypeInfoCJObj, 0, gDemoTypeInfoCXObj, 0, gDemoTypeInfoCXObjIntp, 0, gDemoTypeInfoXObj, 0, 0 };
const void* __RTTI__8XObjDemo[]  = { sTypeNameXObjDemo, sBaseTypesXObjDemo };
