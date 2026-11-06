// US and JP use ASCII zeroes in string alignment padding.
#if defined(VERSION_GGTP01)
#define PAD "\0"
#else
#define PAD "0"
#endif

const char sTypeNameCJObj[8]               = "CJObj\0" PAD PAD;
const char sTypeNameCXObj[8]               = "CXObj\0" PAD PAD;
const char sObjectJObjHeader[8]            = "jobj.h\0" PAD;
const char sObjectJObjCondition[8]         = "jobj\0" PAD PAD PAD;
const char sObjectScaleCondition[8]        = "scale\0" PAD PAD;
const char sObjectRotationCondition[8]     = "rotate\0" PAD;
const float sJointRotationFullTurn         = 6.2831855f;
const float sObjectInterpolationZero       = 0.0f;
const float sJointRotationHalfTurn         = 3.1415927f;
const float sJointRotationSnapThreshold    = 0.001f;
const float sJointRotationSpeedFactor      = 0.1f;
const float sJointRotationMinimumSpeed     = 0.01f;
const double sObjectUnsignedConversionBias = 4503599627370496.0;
const char sObjectPanicFormat[8]           = "%s%s\0" PAD PAD PAD;
const char sObjectEmptyMessage[4]          = "\0" PAD PAD PAD;
const float sObjectInterpolationOne        = 1.0f;
const char sXCObjClassName[8]              = "x_cobj\0" PAD;
const char sObjectReadErrorFormat[8]       = "%s(%d)\n";

// CJArchive::ReadCheck addresses its diagnostic strings relative to this base.
const unsigned char sObjectRodataBase[36]     = { 0 };
const char sTypeNameCXObjIntp[12]             = "CXObjIntp";
const char sObjectNoQuaternionCondition[40]   = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sObjectTranslationCondition[12]    = "translate";
const char sObjectHaltMessage[16]             = "\n!!!HALT!!!\n";
const char sPathCobjCpp[12]                   = "cobj.cpp";
const char sXCObjLibraryName[16]              = "x_cobj_class";
const char sObjectArchiveParseErrorFormat[52] = "ObjArchive::%s() ... error on ArchiveParse - %s\n";
#if defined(VERSION_GGTP01)
const char sTypeNameCJArchive[] = "CJArchive";
#else
const char sTypeNameCJArchive[12] = "CJArchive\0" PAD PAD;
#endif

char sObjectArchiveParseName[8] = "Parse\0" PAD PAD;
char sJointRotationInitName[8]  = "Init\0" PAD PAD PAD;
extern const void* sBaseTypesCXObj[];
extern const void* sBaseTypesCXObjIntp[];
const void* __RTTI__5CJObj[]     = { sTypeNameCJObj, 0 };
const void* __RTTI__5CXObj[]     = { sTypeNameCXObj, sBaseTypesCXObj };
const void* __RTTI__9CXObjIntp[] = { sTypeNameCXObjIntp, sBaseTypesCXObjIntp };
const void* __RTTI__9CJArchive[] = { sTypeNameCJArchive, 0 };
