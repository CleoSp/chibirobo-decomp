extern const char sTypeNameMenuCursor[];
extern const char sStageTypeNameCXObjIntp[];

#if defined(VERSION_GGTP01)
const char sMenuCursorTypeNameCJObj[]  = "CJObj";
const char sMenuCursorTypeNameCXObj[]  = "CXObj";
const char sMenuCursorTypeNameXObj[]   = "XObj";
const char sMenuCursorTypeNameCursor[] = "Cursor";
#else
// US and JP pad the small-data strings with ASCII zeroes.
const char sMenuCursorTypeNameCJObj[8]  = "CJObj\0"
                                          "00";
const char sMenuCursorTypeNameCXObj[8]  = "CXObj\0"
                                          "00";
const char sMenuCursorTypeNameXObj[8]   = "XObj\0"
                                          "000";
const char sMenuCursorTypeNameCursor[8] = "Cursor\0"
                                          "0";
#endif

const void* gMenuCursorTypeInfoCJObj[]      = { sMenuCursorTypeNameCJObj, 0 };
const void* sMenuCursorBaseTypesCXObj[]     = { gMenuCursorTypeInfoCJObj, 0, 0 };
const void* gMenuCursorTypeInfoCXObj[]      = { sMenuCursorTypeNameCXObj, sMenuCursorBaseTypesCXObj };
const void* sMenuCursorBaseTypesCXObjIntp[] = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, 0 };
const void* gMenuCursorTypeInfoCXObjIntp[]  = { sStageTypeNameCXObjIntp, sMenuCursorBaseTypesCXObjIntp };
const void* sMenuCursorBaseTypesXObj[] = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, gMenuCursorTypeInfoCXObjIntp, 0, 0 };
const void* gMenuCursorTypeInfoXObj[]  = { sMenuCursorTypeNameXObj, sMenuCursorBaseTypesXObj };
const void* sMenuCursorBaseTypesCursor[]
    = { gMenuCursorTypeInfoCJObj, 0, gMenuCursorTypeInfoCXObj, 0, gMenuCursorTypeInfoCXObjIntp, 0, gMenuCursorTypeInfoXObj, 0, 0 };
const void* gMenuCursorTypeInfoCursor[]      = { sMenuCursorTypeNameCursor, sMenuCursorBaseTypesCursor };
const void* sMenuCursorBaseTypesMenuCursor[] = { gMenuCursorTypeInfoCJObj,
	                                             0,
	                                             gMenuCursorTypeInfoCXObj,
	                                             0,
	                                             gMenuCursorTypeInfoCXObjIntp,
	                                             0,
	                                             gMenuCursorTypeInfoXObj,
	                                             0,
	                                             gMenuCursorTypeInfoCursor,
	                                             0,
	                                             0 };
const void* __RTTI__10MenuCursor[]           = { sTypeNameMenuCursor, sMenuCursorBaseTypesMenuCursor };
