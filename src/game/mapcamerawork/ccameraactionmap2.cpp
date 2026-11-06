#define DEFER_CVEC_DESTRUCTOR
#include "Dolphin/mtx.h"
#include "game/ut.h"
#include "Version.h"

extern "C" {
#if defined(VERSION_JP)
u8 lbl_805C0190[0x134];
#else
u8 lbl_805C0190[0x140];
#endif
CVec lbl_805C02DC;
CVec lbl_805C02F8;
u8 lbl_805C0308[0x18];
CVec lbl_805C032C;
CVec lbl_805C0348;
CVec lbl_805C0364;
Vec sPreviousMapOrbitAngle;
CVec sMapCameraTestEye;
CVec sMapCameraTestCenter;
}
