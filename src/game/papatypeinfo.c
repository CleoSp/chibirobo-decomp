extern const char sTypeNameXObjActorPapa[];
extern const char sPapaIntpTypeName[];
extern const char sPapaActorTypeName[];

const char sPapaCJObjTypeName[] = "CJObj";
const char sPapaCXObjTypeName[] = "CXObj";
const char sPapaXObjTypeName[]  = "XObj";

const void* gPapaTypeInfoCJObj[]      = { sPapaCJObjTypeName, 0 };
const void* sPapaBaseTypesCXObj[]     = { gPapaTypeInfoCJObj, 0, 0 };
const void* gPapaTypeInfoCXObj[]      = { sPapaCXObjTypeName, sPapaBaseTypesCXObj };
const void* sPapaBaseTypesCXObjIntp[] = { gPapaTypeInfoCJObj, 0, gPapaTypeInfoCXObj, 0, 0 };
const void* gPapaTypeInfoCXObjIntp[]  = { sPapaIntpTypeName, sPapaBaseTypesCXObjIntp };
const void* sPapaBaseTypesXObj[]      = { gPapaTypeInfoCJObj, 0, gPapaTypeInfoCXObj, 0, gPapaTypeInfoCXObjIntp, 0, 0 };
const void* gPapaTypeInfoXObj[]       = { sPapaXObjTypeName, sPapaBaseTypesXObj };
const void* sPapaBaseTypesXObjActor[]
    = { gPapaTypeInfoCJObj, 0, gPapaTypeInfoCXObj, 0, gPapaTypeInfoCXObjIntp, 0, gPapaTypeInfoXObj, 0, 0 };
const void* gPapaTypeInfoXObjActor[] = { sPapaActorTypeName, sPapaBaseTypesXObjActor };
const void* sBaseTypesXObjActorPapa[]
    = { gPapaTypeInfoCJObj, 0, gPapaTypeInfoCXObj, 0, gPapaTypeInfoCXObjIntp, 0, gPapaTypeInfoXObj, 0, gPapaTypeInfoXObjActor, 0, 0 };
const void* __RTTI__13XObjActorPapa[] = { sTypeNameXObjActorPapa, sBaseTypesXObjActorPapa };
