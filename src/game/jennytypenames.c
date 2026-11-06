const char sTypeNameXObjActorJenny[16]      = "XObjActorJenny";
const char sJennyIntpTypeName[12]           = "CXObjIntp";
const char sJennyActorTypeName[12]          = "XObjActor";
const char sJennyEulerRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sJennyTranslationCondition[12]   = "translate";
const char sJennyStatusTypeName[12]         = "CStatus64";
const char sJennyCoordInfoTypeName[12]      = "CoordInfo";
const char sJennyAttrInfoTypeName[12]       = "AttrInfo";
const char sJennyEventDataTypeName[12]      = "CEvtData";
const char sJennyAnimQueueTypeName[10]      = "CAnmQueue";

extern const char sJennyCJObjTypeName[];
extern const char sJennyCXObjTypeName[];
extern const char sJennyXObjTypeName[];

const void* gJennyTypeInfoCJObj[]      = { sJennyCJObjTypeName, 0 };
const void* sJennyBaseTypesCXObj[]     = { gJennyTypeInfoCJObj, 0, 0 };
const void* gJennyTypeInfoCXObj[]      = { sJennyCXObjTypeName, sJennyBaseTypesCXObj };
const void* sJennyBaseTypesCXObjIntp[] = { gJennyTypeInfoCJObj, 0, gJennyTypeInfoCXObj, 0, 0 };
const void* gJennyTypeInfoCXObjIntp[]  = { sJennyIntpTypeName, sJennyBaseTypesCXObjIntp };
const void* sJennyBaseTypesXObj[]      = { gJennyTypeInfoCJObj, 0, gJennyTypeInfoCXObj, 0, gJennyTypeInfoCXObjIntp, 0, 0 };
const void* gJennyTypeInfoXObj[]       = { sJennyXObjTypeName, sJennyBaseTypesXObj };
const void* sJennyBaseTypesXObjActor[]
    = { gJennyTypeInfoCJObj, 0, gJennyTypeInfoCXObj, 0, gJennyTypeInfoCXObjIntp, 0, gJennyTypeInfoXObj, 0, 0 };
const void* gJennyTypeInfoXObjActor[] = { sJennyActorTypeName, sJennyBaseTypesXObjActor };
const void* sBaseTypesXObjActorJenny[]
    = { gJennyTypeInfoCJObj, 0, gJennyTypeInfoCXObj, 0, gJennyTypeInfoCXObjIntp, 0, gJennyTypeInfoXObj, 0, gJennyTypeInfoXObjActor, 0, 0 };
const void* __RTTI__14XObjActorJenny[] = { sTypeNameXObjActorJenny, sBaseTypesXObjActorJenny };
