#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/ccameraaction.h"
#include "game/globals.h"
#include "game/evt.h"
#include "game/xobjchr.h"

/**
 * @note Address: 0x8008A4FC
 * @note Size: 0x34
 */
// CJAnim::Clear
extern "C" void fn_8008A4FC(CJAnim* anim)
{
	anim->CJArchive::Clear();
	anim->mLoading = 0;
}

static inline const void* ObjectScript(CXObj* object)
{
	return ((XObj*)object)->vf128();
}

static inline void TriggerObjectEvent(CXObj* object, u32 first, u32 second)
{
	const void* script = ObjectScript(object);
	if (!fn_8009FDE4(lbl_8025C400.mPlayer)) {
		fn_8007EEA4((CEvt*)(&lbl_8025C400._5D07C), (u8*)script, (u32)object, first, second);
	}
}

/**
 * @note Address: 0x8008A530
 * @note Size: 0xD4
 */
extern "C" BOOL fn_8008A530(XObj* object, s32 unused, s32 value)
{
	if (object->mStatus.mFlags & 0x0000040000000000ULL) {
		if (value == 56)
			return FALSE;
		const void* script = ObjectScript(object);
		if (!fn_8009FDE4(lbl_8025C400.mPlayer)) {
			fn_8007EEA4((CEvt*)(&lbl_8025C400._5D07C), (u8*)script, (u32)object, (s16)value, -1);
		}
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: 0x8008A604
 * @note Size: 0xBC
 */
extern "C" void fn_8008A604(XObj* object, s32 unused, s32 value)
{
	if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		const void* script = ObjectScript(object);
		if (!fn_8009FDE4(lbl_8025C400.mPlayer)) {
			fn_8007EEA4((CEvt*)(&lbl_8025C400._5D07C), (u8*)script, (u32)object, -1, (s16)((u32)value + 3000));
		}
	}
}

/**
 * @note Address: 0x8008A6C0
 * @note Size: 0x158
 */
extern "C" void fn_8008A6C0(XObj* object, f32 value)
{
	if (object->mStatus.mFlags & 0x0000080000000000ULL) {
		if (value >= (s32)fn_8006EBC8(31) / 100.0f) {
			TriggerObjectEvent(object, -1, 2001);
		} else {
			TriggerObjectEvent(object, -1, 2000);
		}
	}
}

/**
 * @note Address: 0x8008A818
 * @note Size: 0x8
 */
extern "C" BOOL fn_8008A818(void* object)
{
	return TRUE;
}

/**
 * @note Address: 0x8008A820
 * @note Size: 0x124
 */
extern "C" void fn_8008A820(XObj* object, s32 index, u8 red, u8 green, u8 blue, f32 time)
{
	object->mColorStep[index].mTarget.x = red;
	object->mColorStep[index].mTarget.y = green;
	object->mColorStep[index].mTarget.z = blue;
	if (time == 0.0f) {
		object->mColorStep[index].mValue.x = red;
		object->mColorStep[index].mValue.y = green;
		object->mColorStep[index].mValue.z = blue;
		object->mColorStep[index].mSpeed.x = object->mColorStep[index].mSpeed.y = object->mColorStep[index].mSpeed.z = 0.0f;
	} else {
#if defined(VERSION_PAL)
		// retail reads the refresh rate for each one
		object->mColorStep[index].mSpeed.x = ((f32)red - object->mColorStep[index].mValue.x) / (lbl_80660D4C * time);
		object->mColorStep[index].mSpeed.y = ((f32)green - object->mColorStep[index].mValue.y) / (lbl_80660D4C * time);
		object->mColorStep[index].mSpeed.z = ((f32)blue - object->mColorStep[index].mValue.z) / (lbl_80660D4C * time);
#else
		object->mColorStep[index].mSpeed.x = ((f32)red - object->mColorStep[index].mValue.x) / (60.0f * time);
		object->mColorStep[index].mSpeed.y = ((f32)green - object->mColorStep[index].mValue.y) / (60.0f * time);
		object->mColorStep[index].mSpeed.z = ((f32)blue - object->mColorStep[index].mValue.z) / (60.0f * time);
#endif
	}
}
