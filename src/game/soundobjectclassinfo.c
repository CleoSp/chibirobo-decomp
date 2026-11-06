extern const char sSoundObjectTypeNameCJObj[];
extern const char sSoundObjectTypeNameCXObj[];
extern const char sSoundObjectTypeNameCXObjIntp[];
extern const char sSoundObjectTypeNameXObj[];
extern const char sTypeNameXObjSound[];

const void* sSoundObjectTypeInfoCJObj[]      = { sSoundObjectTypeNameCJObj, 0 };
const void* sSoundObjectBaseTypesCXObj[]     = { sSoundObjectTypeInfoCJObj, 0, 0 };
const void* sSoundObjectTypeInfoCXObj[]      = { sSoundObjectTypeNameCXObj, sSoundObjectBaseTypesCXObj };
const void* sSoundObjectBaseTypesCXObjIntp[] = { sSoundObjectTypeInfoCJObj, 0, sSoundObjectTypeInfoCXObj, 0, 0 };
const void* sSoundObjectTypeInfoCXObjIntp[]  = { sSoundObjectTypeNameCXObjIntp, sSoundObjectBaseTypesCXObjIntp };
const void* sSoundObjectBaseTypesXObj[]
    = { sSoundObjectTypeInfoCJObj, 0, sSoundObjectTypeInfoCXObj, 0, sSoundObjectTypeInfoCXObjIntp, 0, 0 };
const void* sSoundObjectTypeInfoXObj[] = { sSoundObjectTypeNameXObj, sSoundObjectBaseTypesXObj };
const void* sBaseTypesXObjSound[]
    = { sSoundObjectTypeInfoCJObj, 0, sSoundObjectTypeInfoCXObj, 0, sSoundObjectTypeInfoCXObjIntp, 0, sSoundObjectTypeInfoXObj, 0, 0 };
const void* __RTTI__9XObjSound[] = { sTypeNameXObjSound, sBaseTypesXObjSound };
