const char sTypeNameXObjMap[]      = "XObjMap";
const char sMapTypeNameCXObjIntp[] = "CXObjIntp";
const char sMapTypeNameCJObj[]     = "CJObj";
const char sMapTypeNameCXObj[]     = "CXObj";
const char sMapTypeNameXObj[]      = "XObj";

const void* gMapTypeInfoCJObj[]      = { sMapTypeNameCJObj, 0 };
const void* sMapBaseTypesCXObj[]     = { gMapTypeInfoCJObj, 0, 0 };
const void* gMapTypeInfoCXObj[]      = { sMapTypeNameCXObj, sMapBaseTypesCXObj };
const void* sMapBaseTypesCXObjIntp[] = { gMapTypeInfoCJObj, 0, gMapTypeInfoCXObj, 0, 0 };
const void* gMapTypeInfoCXObjIntp[]  = { sMapTypeNameCXObjIntp, sMapBaseTypesCXObjIntp };
const void* sMapBaseTypesXObj[]      = { gMapTypeInfoCJObj, 0, gMapTypeInfoCXObj, 0, gMapTypeInfoCXObjIntp, 0, 0 };
const void* gMapTypeInfoXObj[]       = { sMapTypeNameXObj, sMapBaseTypesXObj };
const void* sBaseTypesXObjMap[]      = { gMapTypeInfoCJObj, 0, gMapTypeInfoCXObj, 0, gMapTypeInfoCXObjIntp, 0, gMapTypeInfoXObj, 0, 0 };
const void* __RTTI__7XObjMap[]       = { sTypeNameXObjMap, sBaseTypesXObjMap };
