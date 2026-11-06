extern "C" {
extern const char sTypeNameXObjLeticker[]      = "XObjLeticker";
extern const char sLetickerTypeNameCXObjIntp[] = "CXObjIntp";
extern const char sLetickerTypeNameCJObj[]     = "CJObj";
extern const char sLetickerTypeNameCXObj[]     = "CXObj";
extern const char sLetickerTypeNameXObj[]      = "XObj";

const void* gLetickerTypeInfoCJObj[]      = { sLetickerTypeNameCJObj, 0 };
const void* sLetickerBaseTypesCXObj[]     = { gLetickerTypeInfoCJObj, 0, 0 };
const void* gLetickerTypeInfoCXObj[]      = { sLetickerTypeNameCXObj, sLetickerBaseTypesCXObj };
const void* sLetickerBaseTypesCXObjIntp[] = { gLetickerTypeInfoCJObj, 0, gLetickerTypeInfoCXObj, 0, 0 };
const void* gLetickerTypeInfoCXObjIntp[]  = { sLetickerTypeNameCXObjIntp, sLetickerBaseTypesCXObjIntp };
const void* sLetickerBaseTypesXObj[]      = { gLetickerTypeInfoCJObj, 0, gLetickerTypeInfoCXObj, 0, gLetickerTypeInfoCXObjIntp, 0, 0 };
const void* gLetickerTypeInfoXObj[]       = { sLetickerTypeNameXObj, sLetickerBaseTypesXObj };
const void* sBaseTypesXObjLeticker[]
    = { gLetickerTypeInfoCJObj, 0, gLetickerTypeInfoCXObj, 0, gLetickerTypeInfoCXObjIntp, 0, gLetickerTypeInfoXObj, 0, 0 };
const void* __RTTI__12XObjLeticker[] = { sTypeNameXObjLeticker, sBaseTypesXObjLeticker };
}
