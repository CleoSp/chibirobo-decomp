extern "C" {
extern const char sTypeNameCThread[]     = "CThread";
extern const char sTypeNameFbMirrorEx[]  = "FbMirrorEx";
extern const char sTypeNameARPreCache[]  = "ARPreCache";
extern const char sTypeNameARCacheInfo[] = "ARCacheInfo";

extern const void* gTypeInfoCFbTexEff[];
extern const void* gTypeInfoFbMirror[];
const void* sBaseTypesFbMirrorEx[] = { gTypeInfoCFbTexEff, 0, gTypeInfoFbMirror, 0, 0 };

const void* gTypeInfoCThread[]     = { sTypeNameCThread, 0 };
const void* gTypeInfoFbMirrorEx[]  = { sTypeNameFbMirrorEx, sBaseTypesFbMirrorEx };
const void* gTypeInfoARPreCache[]  = { sTypeNameARPreCache, 0 };
const void* gTypeInfoARCacheInfo[] = { sTypeNameARCacheInfo, 0 };
}
