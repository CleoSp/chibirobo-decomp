#include "game/csound.h"

extern "C" {
extern const s32 sLetickerPlayerFacingJointPath[3] = {};
extern const s32 sLetickerStackDrawJointPath[8]    = {};
extern const s32 sLetickerStackTopJointPath[9]     = {};

extern const s32 sLetickerRideJointPath[2]          = { 0, 9 };
extern const s32 sLetickerFrontApproachJointPath[2] = { 0, 2 };
extern const s32 sLetickerBackApproachJointPath[2]  = { 0, 6 };
extern const s32 sLetickerLeftApproachJointPath[2]  = { 0, 4 };
extern const s32 sLetickerRightApproachJointPath[2] = { 0, 8 };

AnimSfxEvent sLetickerAnimSfxEvents[] = {
	{ 7, 10.0f, -1.0f, 1, 0x190, 0xFF, 0, 0, 0 },
	{ -1 },
};
}
