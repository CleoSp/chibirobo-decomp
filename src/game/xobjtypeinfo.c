extern const char sXObjTypeNameVec[];
extern const char sXObjTypeNameCVec[];
extern const char sXObjTypeNameCJObj[];
extern const char sXObjTypeNameCXObj[];
extern const char sTypeNameXObj[];
struct XObjTypeNames {
	char cxObjIntp[12];
	char cStatus64[12];
	char coordInfo[12];
	char attrInfo[12];
	char cEvtData[12];
	char cAnmQueue[10];
};

const struct XObjTypeNames sXObjTypeNames = { "CXObjIntp", "CStatus64", "CoordInfo", "AttrInfo", "CEvtData", "CAnmQueue" };
extern const void* sXObjBaseTypesCVec[];

const void* gXObjTypeInfoVec[]        = { sXObjTypeNameVec, 0 };
const void* gXObjTypeInfoCVec[]       = { sXObjTypeNameCVec, sXObjBaseTypesCVec };
const void* gXObjTypeInfoCJObj[]      = { sXObjTypeNameCJObj, 0 };
const void* sXObjBaseTypesCXObj[]     = { gXObjTypeInfoCJObj, 0, 0 };
const void* gXObjTypeInfoCXObj[]      = { sXObjTypeNameCXObj, sXObjBaseTypesCXObj };
const void* sXObjBaseTypesCXObjIntp[] = { gXObjTypeInfoCJObj, 0, gXObjTypeInfoCXObj, 0, 0 };
const void* gXObjTypeInfoCXObjIntp[]  = { sXObjTypeNames.cxObjIntp, sXObjBaseTypesCXObjIntp };
const void* sBaseTypesXObj[]          = { gXObjTypeInfoCJObj, 0, gXObjTypeInfoCXObj, 0, gXObjTypeInfoCXObjIntp, 0, 0 };
const void* gTypeInfoXObj[]           = { sTypeNameXObj, sBaseTypesXObj };
