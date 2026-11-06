// The US and Japanese string pool uses ASCII zeroes for alignment.
#if defined(VERSION_GGTP01)
const char sBootEulerCondition[40]       = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sBootTranslationCondition[12] = "translate";
const char gBootLogoTexturePath[16]      = "tpl/logo%s.tpl";
#else
const char sBootEulerCondition[40]       = "!(jobj->flags & JOBJ_USE_QUATERNION)\0"
                                           "000";
const char sBootTranslationCondition[12] = "translate\0"
                                           "00";
#if defined(VERSION_GGTJ01_01)
const char gBootLogoTexturePath[16] = "tpl/logo.tpl\0"
                                      "000";
#else
const char gBootLogoTexturePath[16] = "tpl/logo%s.tpl\0"
                                      "0";
#endif
#endif
