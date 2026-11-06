extern const char sTypeNameXObjActorToy[];
extern const char sActorToyIntpTypeName[];
extern const char sActorToyActorTypeName[];

const char sActorToyCJObjTypeName[] = "CJObj";
const char sActorToyCXObjTypeName[] = "CXObj";
const char sActorToyXObjTypeName[]  = "XObj";

const void* gActorToyTypeInfoCJObj[]      = { sActorToyCJObjTypeName, 0 };
const void* sActorToyBaseTypesCXObj[]     = { gActorToyTypeInfoCJObj, 0, 0 };
const void* gActorToyTypeInfoCXObj[]      = { sActorToyCXObjTypeName, sActorToyBaseTypesCXObj };
const void* sActorToyBaseTypesCXObjIntp[] = { gActorToyTypeInfoCJObj, 0, gActorToyTypeInfoCXObj, 0, 0 };
const void* gActorToyTypeInfoCXObjIntp[]  = { sActorToyIntpTypeName, sActorToyBaseTypesCXObjIntp };
const void* sActorToyBaseTypesXObj[]      = { gActorToyTypeInfoCJObj, 0, gActorToyTypeInfoCXObj, 0, gActorToyTypeInfoCXObjIntp, 0, 0 };
const void* gActorToyTypeInfoXObj[]       = { sActorToyXObjTypeName, sActorToyBaseTypesXObj };
const void* sActorToyBaseTypesXObjActor[]
    = { gActorToyTypeInfoCJObj, 0, gActorToyTypeInfoCXObj, 0, gActorToyTypeInfoCXObjIntp, 0, gActorToyTypeInfoXObj, 0, 0 };
const void* gActorToyTypeInfoXObjActor[] = { sActorToyActorTypeName, sActorToyBaseTypesXObjActor };
const void* sBaseTypesXObjActorToy[]     = { gActorToyTypeInfoCJObj,
	                                         0,
	                                         gActorToyTypeInfoCXObj,
	                                         0,
	                                         gActorToyTypeInfoCXObjIntp,
	                                         0,
	                                         gActorToyTypeInfoXObj,
	                                         0,
	                                         gActorToyTypeInfoXObjActor,
	                                         0,
	                                         0 };
const void* __RTTI__12XObjActorToy[]     = { sTypeNameXObjActorToy, sBaseTypesXObjActorToy };
