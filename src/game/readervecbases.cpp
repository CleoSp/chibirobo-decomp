#include "game/readertypeinfo.h"

extern "C" {
extern ReaderTypeInfo gReaderTypeInfoVec;

ReaderBaseList<1> sReaderBaseTypesCVec = { { { &gReaderTypeInfoVec, 0 } }, 0 };
}
