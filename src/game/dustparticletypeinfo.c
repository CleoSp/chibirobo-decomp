const char sTypeNameCPtclDust3[]     = "CPtclDust3";
const char sTypeNameCPtclOne[]       = "CPtclOne";
const char sDustTypeNameCPtclDust2[] = "CPtclDust2";
#if defined(VERSION_GGTP01)
const char sDustTypeNameCPtclDust[16] = "CPtclDust";
#else
const char sDustTypeNameCPtclDust[16] = "CPtclDust\0"
                                        "000000";
#endif

extern const void* sBaseTypesCPtclDust3[];
extern const void* sBaseTypesCPtclDust2[];

const void* gTypeInfoCPtclOne[]   = { sTypeNameCPtclOne, 0 };
const void* gTypeInfoCPtclDust3[] = { sTypeNameCPtclDust3, sBaseTypesCPtclDust3 };
const void* gTypeInfoCPtclDust2[] = { sDustTypeNameCPtclDust2, sBaseTypesCPtclDust2 };
