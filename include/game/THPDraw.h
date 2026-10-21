#ifndef _GAME_THPDRAW_H
#define _GAME_THPDRAW_H

#include "types.h"

// the sdk's THPDraw gx helpers as the game has them. the sdk's THPGXYuv2RgbDraw is split
// into the texture load, the vertex format and the quad, see THPSimple.h

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

void THPGXRestore(void);
void THPGXYuv2RgbSetup(BOOL alpha);
void THPGXLoadYuvTex(u8* yData, u8* uData, u8* vData, s16 textureWidth, s16 textureHeight);
void THPGXSetupVtx(void);
void THPGXDrawQuad(s16 x, s16 y, s16 polygonWidth, s16 polygonHeight);

#ifdef __cplusplus
}
#endif // ifdef __cplusplus

#endif
