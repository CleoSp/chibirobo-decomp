const char sTypeNameXObjFree[]            = "XObjFree";
const char sFreeObjectTypeNameCXObjIntp[] = "CXObjIntp";
const char sFreeObjectTypeNameCJObj[]     = "CJObj";
const char sFreeObjectTypeNameCXObj[]     = "CXObj";
const char sFreeObjectTypeNameXObj[]      = "XObj";

const void* sFreeObjectTypeInfoCJObj[]      = { sFreeObjectTypeNameCJObj, 0 };
const void* sFreeObjectBaseTypesCXObj[]     = { sFreeObjectTypeInfoCJObj, 0, 0 };
const void* sFreeObjectTypeInfoCXObj[]      = { sFreeObjectTypeNameCXObj, sFreeObjectBaseTypesCXObj };
const void* sFreeObjectBaseTypesCXObjIntp[] = { sFreeObjectTypeInfoCJObj, 0, sFreeObjectTypeInfoCXObj, 0, 0 };
const void* sFreeObjectTypeInfoCXObjIntp[]  = { sFreeObjectTypeNameCXObjIntp, sFreeObjectBaseTypesCXObjIntp };
const void* sFreeObjectBaseTypesXObj[] = { sFreeObjectTypeInfoCJObj, 0, sFreeObjectTypeInfoCXObj, 0, sFreeObjectTypeInfoCXObjIntp, 0, 0 };
const void* sFreeObjectTypeInfoXObj[]  = { sFreeObjectTypeNameXObj, sFreeObjectBaseTypesXObj };
const void* sBaseTypesXObjFree[]
    = { sFreeObjectTypeInfoCJObj, 0, sFreeObjectTypeInfoCXObj, 0, sFreeObjectTypeInfoCXObjIntp, 0, sFreeObjectTypeInfoXObj, 0, 0 };
const void* __RTTI__8XObjFree[] = { sTypeNameXObjFree, sBaseTypesXObjFree };
