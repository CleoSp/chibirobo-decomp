const char sTypeNameXObjDust[]  = "XObjDust";
const char sTypeNameXObjFlags[] = "XObjFlags";
const char sTypeNameXObjHBox[]  = "XObjHBox";
const char sTypeNameXObjCoin[]  = "XObjCoin";
const char sTypeNameXObjItem[]  = "XObjItem";

extern const void* sFreeObjectTypeInfoCJObj[];
extern const void* sFreeObjectTypeInfoCXObj[];
extern const void* sFreeObjectTypeInfoCXObjIntp[];
extern const void* sFreeObjectTypeInfoXObj[];
extern const void* sBaseTypesXObjHBox[];
extern const void* sBaseTypesXObjCoin[];
extern const void* sBaseTypesXObjItem[];

const void* sBaseTypesXObjFlags[]
    = { sFreeObjectTypeInfoCJObj, 0, sFreeObjectTypeInfoCXObj, 0, sFreeObjectTypeInfoCXObjIntp, 0, sFreeObjectTypeInfoXObj, 0, 0 };
const void* __RTTI__9XObjFlags[] = { sTypeNameXObjFlags, sBaseTypesXObjFlags };
const void* sBaseTypesXObjDust[] = { sFreeObjectTypeInfoCJObj,
	                                 0,
	                                 sFreeObjectTypeInfoCXObj,
	                                 0,
	                                 sFreeObjectTypeInfoCXObjIntp,
	                                 0,
	                                 sFreeObjectTypeInfoXObj,
	                                 0,
	                                 __RTTI__9XObjFlags,
	                                 0,
	                                 0 };
const void* __RTTI__8XObjDust[]  = { sTypeNameXObjDust, sBaseTypesXObjDust };
const void* __RTTI__8XObjHBox[]  = { sTypeNameXObjHBox, sBaseTypesXObjHBox };
const void* __RTTI__8XObjCoin[]  = { sTypeNameXObjCoin, sBaseTypesXObjCoin };
const void* __RTTI__8XObjItem[]  = { sTypeNameXObjItem, sBaseTypesXObjItem };
