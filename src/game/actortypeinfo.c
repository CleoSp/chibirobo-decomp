extern const char sTypeNameXObjActor[];
extern const char sActorIntpTypeName[];
const char sActorCJObjTypeName[] = "CJObj";
const char sActorCXObjTypeName[] = "CXObj";
const char sActorXObjTypeName[]  = "XObj";

const void* gActorTypeInfoCJObj[]      = { sActorCJObjTypeName, 0 };
const void* sActorBaseTypesCXObj[]     = { gActorTypeInfoCJObj, 0, 0 };
const void* gActorTypeInfoCXObj[]      = { sActorCXObjTypeName, sActorBaseTypesCXObj };
const void* sActorBaseTypesCXObjIntp[] = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, 0 };
const void* gActorTypeInfoCXObjIntp[]  = { sActorIntpTypeName, sActorBaseTypesCXObjIntp };
const void* sActorBaseTypesXObj[]      = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, gActorTypeInfoCXObjIntp, 0, 0 };
const void* gActorTypeInfoXObj[]       = { sActorXObjTypeName, sActorBaseTypesXObj };
const void* sBaseTypesXObjActor[]
    = { gActorTypeInfoCJObj, 0, gActorTypeInfoCXObj, 0, gActorTypeInfoCXObjIntp, 0, gActorTypeInfoXObj, 0, 0 };
const void* gTypeInfoXObjActor[] = { sTypeNameXObjActor, sBaseTypesXObjActor };
