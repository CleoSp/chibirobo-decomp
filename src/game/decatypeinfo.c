extern const char sTypeNameXObjActorDeca[];
extern const char sDecaIntpTypeName[];
extern const char sDecaActorTypeName[];

const char sDecaCJObjTypeName[] = "CJObj";
const char sDecaCXObjTypeName[] = "CXObj";
const char sDecaXObjTypeName[]  = "XObj";

const void* gDecaTypeInfoCJObj[]      = { sDecaCJObjTypeName, 0 };
const void* sDecaBaseTypesCXObj[]     = { gDecaTypeInfoCJObj, 0, 0 };
const void* gDecaTypeInfoCXObj[]      = { sDecaCXObjTypeName, sDecaBaseTypesCXObj };
const void* sDecaBaseTypesCXObjIntp[] = { gDecaTypeInfoCJObj, 0, gDecaTypeInfoCXObj, 0, 0 };
const void* gDecaTypeInfoCXObjIntp[]  = { sDecaIntpTypeName, sDecaBaseTypesCXObjIntp };
const void* sDecaBaseTypesXObj[]      = { gDecaTypeInfoCJObj, 0, gDecaTypeInfoCXObj, 0, gDecaTypeInfoCXObjIntp, 0, 0 };
const void* gDecaTypeInfoXObj[]       = { sDecaXObjTypeName, sDecaBaseTypesXObj };
const void* sDecaBaseTypesXObjActor[]
    = { gDecaTypeInfoCJObj, 0, gDecaTypeInfoCXObj, 0, gDecaTypeInfoCXObjIntp, 0, gDecaTypeInfoXObj, 0, 0 };
const void* gDecaTypeInfoXObjActor[] = { sDecaActorTypeName, sDecaBaseTypesXObjActor };
const void* sBaseTypesXObjActorDeca[]
    = { gDecaTypeInfoCJObj, 0, gDecaTypeInfoCXObj, 0, gDecaTypeInfoCXObjIntp, 0, gDecaTypeInfoXObj, 0, gDecaTypeInfoXObjActor, 0, 0 };
const void* __RTTI__13XObjActorDeca[] = { sTypeNameXObjActorDeca, sBaseTypesXObjActorDeca };
