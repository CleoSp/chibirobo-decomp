#include "game/octamenu.h"

extern "C" {
#if defined(VERSION_JP)
extern const OXJointPath sOXMenuAnchorJointPath = { 0, 1, 0, 1, 2, 0, 1 };
extern const Vec sOXMenuAnchorOffset            = { 0.0f, 0.0f, 0.0f };
#else
f32 gOXMenuLabelOutlineX[4]     = { -1.0f, 1.0f, 1.0f, -1.0f };
f32 gOXMenuLabelOutlineY[4]     = { -1.0f, -1.0f, 1.0f, 1.0f };
u32 gOXMenuMarkDiffuseColors[2] = { 0x008E2300, 0xFF000000 };
#endif
}
