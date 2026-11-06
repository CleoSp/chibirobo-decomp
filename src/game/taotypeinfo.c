extern const char sTypeNameXObjActorTao[];
extern const char sTaoIntpTypeName[];
extern const char sTaoActorTypeName[];

const char sTaoCJObjTypeName[] = "CJObj";
const char sTaoCXObjTypeName[] = "CXObj";
const char sTaoXObjTypeName[]  = "XObj";

const void* gTaoTypeInfoCJObj[]      = { sTaoCJObjTypeName, 0 };
const void* sTaoBaseTypesCXObj[]     = { gTaoTypeInfoCJObj, 0, 0 };
const void* gTaoTypeInfoCXObj[]      = { sTaoCXObjTypeName, sTaoBaseTypesCXObj };
const void* sTaoBaseTypesCXObjIntp[] = { gTaoTypeInfoCJObj, 0, gTaoTypeInfoCXObj, 0, 0 };
const void* gTaoTypeInfoCXObjIntp[]  = { sTaoIntpTypeName, sTaoBaseTypesCXObjIntp };
const void* sTaoBaseTypesXObj[]      = { gTaoTypeInfoCJObj, 0, gTaoTypeInfoCXObj, 0, gTaoTypeInfoCXObjIntp, 0, 0 };
const void* gTaoTypeInfoXObj[]       = { sTaoXObjTypeName, sTaoBaseTypesXObj };
const void* sTaoBaseTypesXObjActor[] = { gTaoTypeInfoCJObj, 0, gTaoTypeInfoCXObj, 0, gTaoTypeInfoCXObjIntp, 0, gTaoTypeInfoXObj, 0, 0 };
const void* gTaoTypeInfoXObjActor[]  = { sTaoActorTypeName, sTaoBaseTypesXObjActor };
const void* sBaseTypesXObjActorTao[]
    = { gTaoTypeInfoCJObj, 0, gTaoTypeInfoCXObj, 0, gTaoTypeInfoCXObjIntp, 0, gTaoTypeInfoXObj, 0, gTaoTypeInfoXObjActor, 0, 0 };
const void* __RTTI__12XObjActorTao[] = { sTypeNameXObjActorTao, sBaseTypesXObjActorTao };
