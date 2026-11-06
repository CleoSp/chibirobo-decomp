#include "game/readertypeinfo.h"

extern "C" {
extern ReaderTypeInfo gReaderTypeInfoCJObj;
extern ReaderTypeInfo gReaderTypeInfoCXObj;
extern ReaderTypeInfo gReaderTypeInfoCXObjIntp;

ReaderBaseList<1> sReaderBaseTypesCXObj     = { { { &gReaderTypeInfoCJObj, 0 } }, 0 };
ReaderBaseList<2> sReaderBaseTypesCXObjIntp = { { { &gReaderTypeInfoCJObj, 0 }, { &gReaderTypeInfoCXObj, 0 } }, 0 };
ReaderBaseList<3> sReaderBaseTypesXObj
    = { { { &gReaderTypeInfoCJObj, 0 }, { &gReaderTypeInfoCXObj, 0 }, { &gReaderTypeInfoCXObjIntp, 0 } }, 0 };
}
