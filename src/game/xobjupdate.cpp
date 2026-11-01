#include "game/xobjs.h"

static inline void StepColor(f32& current, f32& target, f32& speed, u32 frames)
{
	if (current != target) {
		current += speed * frames;
		if (speed > 0.0f && current >= target) {
			current = target;
			speed   = 0.0f;
		}
		if (speed < 0.0f && current <= target) {
			current = target;
			speed   = 0.0f;
		}
	}
}

/**
 * @note Address: 0x80090C84
 * @note Size: 0x250
 */
extern "C" void fn_80090C84(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		for (s32 i = 0; i < 2; ++i) {
			XObjColorStep& color = object->mColorStep[i];
			StepColor(color.mValue.x, color.mTarget.x, color.mSpeed.x, frames);
			StepColor(color.mValue.y, color.mTarget.y, color.mSpeed.y, frames);
			StepColor(color.mValue.z, color.mTarget.z, color.mSpeed.z, frames);
		}
		object->mColors[1] = ((u32)object->mColorStep[0].mValue.x << 24) | ((u32)object->mColorStep[0].mValue.y << 16)
		                   | ((u32)object->mColorStep[0].mValue.z << 8);
		object->mColors[0] = ((u32)object->mColorStep[1].mValue.x << 24) | ((u32)object->mColorStep[1].mValue.y << 16)
		                   | ((u32)object->mColorStep[1].mValue.z << 8);
	}
}

/**
 * @note Address: 0x80090ED4
 * @note Size: 0x2EC
 */
extern "C" void fn_80090ED4(XObj* object, u32 frames)
{
	if (object->mJObj != NULL) {
		object->vfA8(frames);
		if (object->mStatus.mFlags & 0x1000000000000000ULL) {
			if (object->mMoveTranslate.y + object->_794.y > object->_790) {
				object->mMoveTranslate.mTarget.y = object->_790;
				f32 speed                        = 0.15f + object->mMoveTranslate.mSpeed.y;
				if (speed > 10.0f)
					speed = 10.0f;
				object->mMoveTranslate.mSpeed.y = speed;
			} else {
				object->mMoveTranslate.mSpeed.y = 0.0f;
				object->mMoveTranslate.x += object->_794.x;
				object->mMoveTranslate.z += object->_794.z;
				object->mMoveTranslate.y = object->_790;
				object->_794.x           = 0.0f;
				object->_794.y           = 0.0f;
				object->_794.z           = 0.0f;
				object->SetRotationX(0.0f, 0);
				object->SetRotationZ(0.0f, 0);
				object->mStatus.mFlags &= ~0x1000000000000000ULL;
				if ((s16)object->GetPool() == 21) {
					object->mStatus.mFlags |= 0x2000000000000000ULL;
				} else if ((s16)object->GetPool() == 22) {
					if (object->mStatus.mFlags & 0x04000000ULL)
						object->mStatus.mFlags |= 0x2000000000000000ULL;
				}
			}
		}
		object->CXObjIntp::StepRotate(frames);
		object->CXObjIntp::StepTranslate(frames);
		if (object->CXObjIntp::StepScale(frames)) {
			fn_8008C8A4((XObj*)object);
			fn_8008D220((XObj*)object);
		}
		object->UpdateAnim(frames);
		HSD_JObjSetupMatrix(object->mJObj);
	}
	if ((object->mStatus.mFlags & 0x0010000000000000ULL) || object->mMoveTranslate.y != object->_264.y) {
		fn_8008C8A4((XObj*)object);
	}
	if (object->mEvt._04 != NULL)
		CEvt_Update(&object->mEvt, frames);
}
