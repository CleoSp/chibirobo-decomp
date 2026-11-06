#include "game/cgame.h"

#pragma section RX ".text.none"

void operator delete(void* object);

extern "C" void* __vt__20CCameraActionAreaExM[];

/**
 * @note Address: 0x8001585C
 * @note Size: 0x1C
 */
__declspec(section ".text.none") bool CInterpolationNone::Start(f32 from, f32 to, f32 time)
{
	mTime  = 1.0f;
	mValue = to;
	mSpeed = 0.0f;
	return true;
}

/**
 * @note Address: 0x80015878
 * @note Size: 0x24
 */
__declspec(section ".text.none") bool CInterpolationNone::Step(f32 dt)
{
	mTime = 1.0f;
	return mTime <= 0.0f;
}

/**
 * @note Address: 0x8001589C
 * @note Size: 0x2C
 */
__declspec(section ".text.none") bool CInterpolationNone::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x800158C8
 * @note Size: 0x8
 */
__declspec(section ".text.none") f32 CInterpolationNone::Get()
{
	return mValue;
}

/**
 * @note Address: 0x800158D0
 * @note Size: 0xCC
 */
__declspec(section ".text.none") bool CVectorNoneInterpolation::Start(Vec from, Vec to, f32 time)
{
	if (time < 0.0f) {
		return false;
	}
	mAxes[0].Start(from.x, to.x, time);
	mAxes[1].Start(from.y, to.y, time);
	mAxes[2].Start(from.z, to.z, time);
	return true;
}

/**
 * @note Address: 0x8001599C
 * @note Size: 0xB4
 */
__declspec(section ".text.none") bool CVectorNoneInterpolation::Step(f32 dt)
{
	bool x = mAxes[0].Step(dt);
	bool y = mAxes[1].Step(dt);
	bool z = mAxes[2].Step(dt);
	return x && y && z;
}

/**
 * @note Address: 0x80015A50
 * @note Size: 0x2C
 */
__declspec(section ".text.none") bool CVectorNoneInterpolation::Update(u32 step, f32 dt)
{
	return Step(dt);
}

/**
 * @note Address: 0x80015A7C
 * @note Size: 0xBC
 */
__declspec(section ".text.none") Vec CVectorNoneInterpolation::Get()
{
	return CVec(mAxes[0].Get(), mAxes[1].Get(), mAxes[2].Get());
}

/**
 * @note Address: 0x80015BBC
 * @note Size: 0x98
 */
#pragma section RX ".text.cameraareadtor"
extern "C" __declspec(section ".text.cameraareadtor") CCameraActionAreaExM* __dt__20CCameraActionAreaExMFv(CCameraActionAreaExM* action,
                                                                                                           s32 flags)
{
	if (action != NULL) {
		*(void***)((u8*)action + 0x3C) = __vt__20CCameraActionAreaExM; // _3C vtable
		action->CInterfaceCameraAction::~CInterfaceCameraAction();
		if ((s16)flags > 0) {
			::operator delete(action);
		}
	}
	return action;
}
