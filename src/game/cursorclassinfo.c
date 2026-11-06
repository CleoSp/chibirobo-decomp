#if defined(VERSION_GGTP01)
const char sCursorTypeNameCXObjIntp[12] = "CXObjIntp";
const char sTypeNameCursor[8]           = "Cursor";
const char sCursorTypeNameCJObj[8]      = "CJObj";
const char sCursorTypeNameCXObj[8]      = "CXObj";
const char sCursorTypeNameXObj[8]       = "XObj";
#else
const char sCursorTypeNameCXObjIntp[12] = "CXObjIntp\0"
                                          "00";
const char sTypeNameCursor[8]           = "Cursor\0"
                                          "0";
const char sCursorTypeNameCJObj[8]      = "CJObj\0"
                                          "00";
const char sCursorTypeNameCXObj[8]      = "CXObj\0"
                                          "00";
const char sCursorTypeNameXObj[8]       = "XObj\0"
                                          "000";
#endif

extern const void* sCursorBaseTypesCXObj[];
extern const void* sCursorBaseTypesCXObjIntp[];
extern const void* sCursorBaseTypesXObj[];
extern const void* sBaseTypesCursor[];

const void* gCursorTypeInfoCJObj[]     = { sCursorTypeNameCJObj, 0 };
const void* gCursorTypeInfoCXObj[]     = { sCursorTypeNameCXObj, sCursorBaseTypesCXObj };
const void* gCursorTypeInfoCXObjIntp[] = { sCursorTypeNameCXObjIntp, sCursorBaseTypesCXObjIntp };
const void* gCursorTypeInfoXObj[]      = { sCursorTypeNameXObj, sCursorBaseTypesXObj };
const void* gTypeInfoCursor[]          = { sTypeNameCursor, sBaseTypesCursor };

const void* sCursorBaseTypesCXObj[]     = { gCursorTypeInfoCJObj, 0, 0 };
const void* sCursorBaseTypesCXObjIntp[] = { gCursorTypeInfoCJObj, 0, gCursorTypeInfoCXObj, 0, 0 };
const void* sCursorBaseTypesXObj[]      = { gCursorTypeInfoCJObj, 0, gCursorTypeInfoCXObj, 0, gCursorTypeInfoCXObjIntp, 0, 0 };
const void* sBaseTypesCursor[]
    = { gCursorTypeInfoCJObj, 0, gCursorTypeInfoCXObj, 0, gCursorTypeInfoCXObjIntp, 0, gCursorTypeInfoXObj, 0, 0 };
