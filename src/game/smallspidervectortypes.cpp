extern "C" {
extern const char sSmallSpiderVecTypeName[]  = "Vec";
extern const char sSmallSpiderCVecTypeName[] = "CVec";

const void* gSmallSpiderVecTypeInfo[]   = { sSmallSpiderVecTypeName, 0 };
const void* sSmallSpiderCVecBaseTypes[] = { gSmallSpiderVecTypeInfo, 0, 0 };
const void* gSmallSpiderCVecTypeInfo[]  = { sSmallSpiderCVecTypeName, sSmallSpiderCVecBaseTypes };
}
