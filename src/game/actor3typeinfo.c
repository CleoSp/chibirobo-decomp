const char sTypeNameXObjActor3[]          = "XObjActor3";
const char sActor3IntpTypeName[]          = "CXObjIntp";
const char sActor3NoQuaternionCondition[] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sActor3CJObjTypeName[]         = "CJObj";
const char sActor3CXObjTypeName[]         = "CXObj";
const char sActor3XObjTypeName[]          = "XObj";

const void* gActor3TypeInfoCJObj[]      = { sActor3CJObjTypeName, 0 };
const void* sActor3BaseTypesCXObj[]     = { gActor3TypeInfoCJObj, 0, 0 };
const void* gActor3TypeInfoCXObj[]      = { sActor3CXObjTypeName, sActor3BaseTypesCXObj };
const void* sActor3BaseTypesCXObjIntp[] = { gActor3TypeInfoCJObj, 0, gActor3TypeInfoCXObj, 0, 0 };
const void* gActor3TypeInfoCXObjIntp[]  = { sActor3IntpTypeName, sActor3BaseTypesCXObjIntp };
const void* sActor3BaseTypesXObj[]      = { gActor3TypeInfoCJObj, 0, gActor3TypeInfoCXObj, 0, gActor3TypeInfoCXObjIntp, 0, 0 };
const void* gActor3TypeInfoXObj[]       = { sActor3XObjTypeName, sActor3BaseTypesXObj };
const void* sBaseTypesXObjActor3[]
    = { gActor3TypeInfoCJObj, 0, gActor3TypeInfoCXObj, 0, gActor3TypeInfoCXObjIntp, 0, gActor3TypeInfoXObj, 0, 0 };
const void* __RTTI__10XObjActor3[] = { sTypeNameXObjActor3, sBaseTypesXObjActor3 };
