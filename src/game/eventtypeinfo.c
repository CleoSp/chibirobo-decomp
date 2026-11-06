const char sTypeNameCEvtTimer[12]           = "CEvtTimer";
const char sEventTypeNameCStatus64[12]      = "CStatus64";
const char sEventTypeNameAttrInfo[12]       = "AttrInfo";
const char sEventTypeNameCEvtData[12]       = "CEvtData";
const char sEventTypeNameCAnmQueue[12]      = "CAnmQueue";
const char sEventTypeNameCCameraMemento[16] = "CCameraMemento";
const char sTypeNameCEvtQueueEx[12]         = "CEvtQueueEx";
const char sTypeNameCEvtQueue[12]           = "CEvtQueue";
const char sTypeNameCEvtVars[12]            = "CEvtVars";
const char sEventTypeNameEVT_VARS[12]       = "EVT_VARS";
const char sTypeNameCEvtFlags[12]           = "CEvtFlags";
#if defined(VERSION_GGTP01)
const char sEventTypeNameEVT_FLAGS[16] = "EVT_FLAGS";
#else
const char sEventTypeNameEVT_FLAGS[16] = "EVT_FLAGS\0"
                                         "000000";
#endif

#if defined(VERSION_GGTP01)
const char sEventTypeNameCVec[8] = "CVec";
const char sEventTypeNameVec[8]  = "Vec";
#else
const char sEventTypeNameCVec[8] = "CVec\0"
                                   "000";
const char sEventTypeNameVec[8]  = "Vec\0"
                                   "0000";
#endif
extern const void* sEventBaseTypesCVec[];
const void* gEventTypeInfoVec[]  = { sEventTypeNameVec, 0 };
const void* gEventTypeInfoCVec[] = { sEventTypeNameCVec, sEventBaseTypesCVec };

extern const void* sBaseTypesCEvtQueueEx[];
extern const void* sBaseTypesCEvtVars[];
extern const void* sBaseTypesCEvtFlags[];
const void* gTypeInfoCEvtTimer[]   = { sTypeNameCEvtTimer, 0 };
const void* gTypeInfoCEvtQueue[]   = { sTypeNameCEvtQueue, 0 };
const void* gTypeInfoCEvtQueueEx[] = { sTypeNameCEvtQueueEx, sBaseTypesCEvtQueueEx };
const void* gTypeInfoEVT_VARS[]    = { sEventTypeNameEVT_VARS, 0 };
const void* gTypeInfoCEvtVars[]    = { sTypeNameCEvtVars, sBaseTypesCEvtVars };
const void* gTypeInfoEVT_FLAGS[]   = { sEventTypeNameEVT_FLAGS, 0 };
const void* gTypeInfoCEvtFlags[]   = { sTypeNameCEvtFlags, sBaseTypesCEvtFlags };
#pragma section RW ".sdata" \
                   ".sbss"
#if defined(VERSION_GGTP01)
__declspec(section ".sdata") char sEventTypeInfoAlignment[4] = { 0 };
#else
__declspec(section ".sdata") char sEventTypeInfoAlignment[4] = { '0', '0', '0', '0' };
#endif

extern void __dt__11CEvtQueueExFv(void);
extern void __dt__9CEvtQueueFv(void);
extern void __dt__8CEvtVarsFv(void);
extern void __dt__9CEvtFlagsFv(void);
const void* sBaseTypesCEvtQueueEx[] = { gTypeInfoCEvtQueue, 0, 0 };
const void* __vt__11CEvtQueueEx[]   = { gTypeInfoCEvtQueueEx, 0, __dt__11CEvtQueueExFv };
const void* __vt__9CEvtQueue[]      = { gTypeInfoCEvtQueue, 0, __dt__9CEvtQueueFv };
const void* sBaseTypesCEvtVars[]    = { gTypeInfoEVT_VARS, 0, 0 };
const void* __vt__8CEvtVars[]       = { gTypeInfoCEvtVars, 0, __dt__8CEvtVarsFv };
const void* sBaseTypesCEvtFlags[]   = { gTypeInfoEVT_FLAGS, 0, 0 };
const void* __vt__9CEvtFlags[]      = { gTypeInfoCEvtFlags, 0, __dt__9CEvtFlagsFv };
