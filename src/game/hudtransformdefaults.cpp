#include <dolphin/mtx/mtxtypes.h>

extern "C" {
#include "game/hudtransformdefaults.inc"

// InitInfoScreen and ResetGameHudState each copy four zero vectors into the HUD.
extern const Vec sHudInitialVectors[4] = {};
extern const Vec sHudResetVectors[4]   = {};
}
