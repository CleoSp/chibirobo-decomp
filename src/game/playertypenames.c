const char sPlayerTypeNameCoordInfo[]               = "CoordInfo";
const char sPlayerTypeNameGXTexObj[]                = "_GXTexObj";
const char sPlayerTypeNameCXObjIntp[]               = "CXObjIntp";
const char sPlayerTypeNameCStatus64[]               = "CStatus64";
const char sPlayerTypeNameAttrInfo[]                = "AttrInfo";
const char sPlayerTypeNameCEvtData[]                = "CEvtData";
const char sPlayerTypeNameCAnmQueue[]               = "CAnmQueue";
const char sPlayerTypeNameCInterfaceInterpolation[] = "CInterfaceInterpolation";
const char sPlayerTypeNameCScrnColor[]              = "CScrnColor";
const char sPlayerTypeNameCPtclOne[]                = "CPtclOne";
const char sTypeNameXActLift[]                      = "XActLift";
const char sTypeNameXActHang[]                      = "XActHang";
const char sTypeNameXActJump[]                      = "XActJump";
const char sTypeNameXActLadder[]                    = "XActLadder";
const char sTypeNameXActRope[]                      = "XActRope";
const char sTypeNameXActPush[]                      = "XActPush";
const char sTypeNameXActTumble[]                    = "XActTumble";
const char sTypeNameXActCopter[]                    = "XActCopter";
const char sTypeNameXActPopper[]                    = "XActPopper";
const char sTypeNameXActRadar[]                     = "XActRadar";
const char sTypeNameCJointRot[]                     = "CJointRot";

#if defined(VERSION_GGTP01)
const char sPlayerParameterName[16] = "PARAM_CHR";
#else
const char sPlayerParameterName[16] = "PARAM_CHR\0"
                                      "000000";
#endif
