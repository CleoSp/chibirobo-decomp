#ifndef _DOLPHIN_THP_H
#define _DOLPHIN_THP_H

#include "types.h"

// the public part of the sdk thp decoder, the bodies are in src/dolphin/thp

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

BOOL THPInit(void);
s32 THPVideoDecode(void* file, void* tileY, void* tileU, void* tileV, void* work);
u32 THPAudioDecode(s16* audioBuffer, u8* audioFrame, s32 flag);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif
