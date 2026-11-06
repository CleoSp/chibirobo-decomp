const char sTypeNameXObjActorMama[16]      = "XObjActorMama";
const char sMamaIntpTypeName[12]           = "CXObjIntp";
const char sMamaActorTypeName[12]          = "XObjActor";
const char sMamaEulerRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sMamaTranslationCondition[12]   = "translate";
const char sMamaStatusTypeName[12]         = "CStatus64";
const char sMamaCoordInfoTypeName[12]      = "CoordInfo";
const char sMamaAttrInfoTypeName[12]       = "AttrInfo";
const char sMamaEventDataTypeName[12]      = "CEvtData";
const char sMamaAnimQueueTypeName[10]      = "CAnmQueue";

extern const char sMamaCJObjTypeName[];
extern const char sMamaCXObjTypeName[];
extern const char sMamaXObjTypeName[];

const void* gMamaTypeInfoCJObj[]      = { sMamaCJObjTypeName, 0 };
const void* sMamaBaseTypesCXObj[]     = { gMamaTypeInfoCJObj, 0, 0 };
const void* gMamaTypeInfoCXObj[]      = { sMamaCXObjTypeName, sMamaBaseTypesCXObj };
const void* sMamaBaseTypesCXObjIntp[] = { gMamaTypeInfoCJObj, 0, gMamaTypeInfoCXObj, 0, 0 };
const void* gMamaTypeInfoCXObjIntp[]  = { sMamaIntpTypeName, sMamaBaseTypesCXObjIntp };
const void* sMamaBaseTypesXObj[]      = { gMamaTypeInfoCJObj, 0, gMamaTypeInfoCXObj, 0, gMamaTypeInfoCXObjIntp, 0, 0 };
const void* gMamaTypeInfoXObj[]       = { sMamaXObjTypeName, sMamaBaseTypesXObj };
const void* sMamaBaseTypesXObjActor[]
    = { gMamaTypeInfoCJObj, 0, gMamaTypeInfoCXObj, 0, gMamaTypeInfoCXObjIntp, 0, gMamaTypeInfoXObj, 0, 0 };
const void* gMamaTypeInfoXObjActor[] = { sMamaActorTypeName, sMamaBaseTypesXObjActor };
const void* sBaseTypesXObjActorMama[]
    = { gMamaTypeInfoCJObj, 0, gMamaTypeInfoCXObj, 0, gMamaTypeInfoCXObjIntp, 0, gMamaTypeInfoXObj, 0, gMamaTypeInfoXObjActor, 0, 0 };
const void* __RTTI__13XObjActorMama[] = { sTypeNameXObjActorMama, sBaseTypesXObjActorMama };
