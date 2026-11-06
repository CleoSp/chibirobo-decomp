extern "C" {
extern const char sTypeNameXObjArmy[]      = "XObjArmy";
extern const char sArmyTypeNameCXObjIntp[] = "CXObjIntp";
extern const char sArmyTypeNameCJObj[]     = "CJObj";
extern const char sArmyTypeNameCXObj[]     = "CXObj";
extern const char sArmyTypeNameXObj[]      = "XObj";

const void* gArmyTypeInfoCJObj[]      = { sArmyTypeNameCJObj, 0 };
const void* sArmyBaseTypesCXObj[]     = { gArmyTypeInfoCJObj, 0, 0 };
const void* gArmyTypeInfoCXObj[]      = { sArmyTypeNameCXObj, sArmyBaseTypesCXObj };
const void* sArmyBaseTypesCXObjIntp[] = { gArmyTypeInfoCJObj, 0, gArmyTypeInfoCXObj, 0, 0 };
const void* gArmyTypeInfoCXObjIntp[]  = { sArmyTypeNameCXObjIntp, sArmyBaseTypesCXObjIntp };
const void* sArmyBaseTypesXObj[]      = { gArmyTypeInfoCJObj, 0, gArmyTypeInfoCXObj, 0, gArmyTypeInfoCXObjIntp, 0, 0 };
const void* gArmyTypeInfoXObj[]       = { sArmyTypeNameXObj, sArmyBaseTypesXObj };
const void* sBaseTypesXObjArmy[] = { gArmyTypeInfoCJObj, 0, gArmyTypeInfoCXObj, 0, gArmyTypeInfoCXObjIntp, 0, gArmyTypeInfoXObj, 0, 0 };
const void* __RTTI__8XObjArmy[]  = { sTypeNameXObjArmy, sBaseTypesXObjArmy };
}
