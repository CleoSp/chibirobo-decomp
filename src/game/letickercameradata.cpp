#include "types.h"

extern "C" {
#if !defined(VERSION_PAL)
extern const char sLetickerEyeXZFormat[]          = "%4.2f %4.2f\n";
extern const char sLetickerTurnAngleFormat[]      = "CCameraActionLeticker::Update %4.2f %4.2f %d\n";
extern const char sLetickerUninitializedMessage[] = "レチッカーズが初期化されてません\n";
extern const char sLetickerLevelEntryFormat[]     = "m_level_table[%d]=%d\n";
extern const char sLetickerCameraResetMessage[]   = "CCameraActionLeticker::Reset\n";
#endif
extern const f32 sLetickerLeftTurnSpeed  = -0.08726646f;
extern const f32 sLetickerRightTurnSpeed = 0.08726646f;
}
