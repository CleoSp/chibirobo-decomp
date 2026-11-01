#include "game/xchr.h"
#include "game/cgame.h"
#include "game/infoscrn.h"

extern "C" {
s32 fn_800E4AAC(XObj* target, const Vec* position);
void fn_800A0014(XObjChr* chr, BOOL start, BOOL cancel, f32 value);
}

// pool 9 objects decide for themselves whether chibi robo can reach them
/**
 * @note Address: 0x8009FC60
 * @note Size: 0x64
 */
extern "C" s32 fn_8009FC60(XObjChr* chr, XObj* target)
{
	if ((s16)target->GetPool() == 9)
		return fn_800E4AAC(target, &chr->mMoveTranslate);
	return 1;
}

// resets the plug, keeping its rotation while it's carried
/**
 * @note Address: 0x8009FCC4
 * @note Size: 0x120
 */
extern "C" void fn_8009FCC4(XObjChr* chr)
{
	if (chr->mPlug.mStatus.mFlags & 0x0400000000000000ULL) {
		Vec rotation = (Vec&)chr->mPlug.mMoveRotate;
		chr->mPlug.mStatus.mFlags &= ~0x0400000000000000ULL;
		chr->mPlug.vf98();
		chr->mPlug.vfA4(10);
		XObj* plug = &chr->mPlug;
		plug->SetRot(rotation, 0);
		chr->mPlug.mStatus.mFlags |= 0x0400000000000000ULL;
	} else {
		chr->mPlug.vf98();
		chr->mPlug.vfA4(10);
	}
}

// todo: cleanup
static inline u8 BatteryDead(XObjChr* chr)
{
	return (u8)(chr->mParams.mBattery <= 0.0f && fn_800665E0((InfoScrn*)lbl_8025C400.mInfoScrn));
}

// todo: cleanup
static inline u8 BatteryDeadReady(XObjChr* chr)
{
	return (u8)(BatteryDead(chr) && chr->_158A4 != 0);
}

// todo: cleanup
static inline u8 SuppressEvent(XObjChr* chr)
{
	return (u8)((lbl_8025C400.mStatus.mFlags & 0x40000000ULL) && BatteryDeadReady(chr));
}

// todo: cleanup
static inline u8 IsTumbling(XObjChr* chr)
{
	u8 result = 0;
	if (chr->mTumble.mState == 4 || chr->mTumble.mState == 5 || chr->mTumble.mState == 6)
		result = 1;
	return result;
}

// todo: cleanup, xobjchr.h has it as void*
/**
 * @note Address: 0x8009FDE4
 * @note Size: 0xF8
 */
extern "C" BOOL fn_8009FDE4(void* object)
{
	XObjChr* chr = (XObjChr*)object;
	u8 blocked   = 0;
	if (SuppressEvent(chr) && IsTumbling(chr))
		blocked = 1;
	return blocked;
}

// out of battery
/**
 * @note Address: 0x8009FEDC
 * @note Size: 0x64
 */
extern "C" BOOL fn_8009FEDC(XObjChr* chr)
{
	return BatteryDead(chr);
}

// XObjChr::vf138, hit from a position. flashes what he carries instead when he lifts something
/**
 * @note Address: 0x8009FF40
 * @note Size: 0xC0
 */
extern "C" void fn_8009FF40(XObjChr* chr, f32 strength)
{
	if (fn_800D4D54(&chr->mLift)) {
		chr->mLift.mTarget->vf12C(1, 150, 150, 0, 0.0f);
		chr->mLift.mTarget->vf12C(1, 0, 0, 0, 0.2f);
	} else if (chr->_A34 <= 0) {
		fn_800A0014(chr, TRUE, TRUE, strength);
	}
}

// still flashing from a hit
/**
 * @note Address: 0x800A0000
 * @note Size: 0x14
 */
extern "C" u8 fn_800A0000(XObjChr* chr)
{
	return chr->_A34 > 0;
}
