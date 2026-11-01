#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/evt.h"
#include "game/xobjactor.h"

extern "C" {
void fn_800242A8(void* effect, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 time);
extern XObjActorToy::StateFunc lbl_8022CFC8;
extern XObjActorToy::StateFunc lbl_8022CFD4;
}

static inline void Shake(XObjActorToy* object)
{
	fn_800242A8(lbl_8025C400.mQuake, object->mCmd[1], object->mCmd[2], object->mCmd[3], 0.0f, 0.0f, 0.0f, object->mCmd[4] / 100.0f);
}

/**
 * @note Address: 0x8011E8DC
 * @note Size: 0xD0
 */
extern "C" void fn_8011E8DC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		Shake(object);
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011E9AC
 * @note Size: 0x140
 */
extern "C" void fn_8011E9AC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		object->mCmd[0] = 0;
		break;
	case 1:
		Shake(object);
		object->mCmd[0] = 0;
		break;
	case 2:
		switch (object->mCmd[1]) {
		case 0:
			object->_2098.mFlags |= 1;
			object->mCmd[0] = 0;
			break;
		case 1:
			object->_2098.mFlags &= ~1U;
			object->mCmd[0] = 0;
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8011EAEC
 * @note Size: 0xAC
 */
extern "C" void fn_8011EAEC(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		object->_A4C   = object->mCmd[1];
		object->mState = lbl_8022CFD4;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 2:
		object->_A4C = -1;
		if (object->_A50 != NULL)
			object->_A50->Release();
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011EB98
 * @note Size: 0x80
 */
extern "C" void fn_8011EB98(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		object->_2098.mFlags |= 8;
		object->mCmd[0] = 0;
		break;
	case 2:
		if (object->mStatus.mFlags & 0x10000ULL)
			lbl_80367A2C[object->mCmd[1]] = 1;
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011EC18
 * @note Size: 0x130
 */
extern "C" void fn_8011EC18(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 1:
		object->mCmd[0] = 0;
		break;
	case 2:
		if (object->mMoveTranslate.y <= 2.0f)
			lbl_80367A2C[object->mCmd[1]] = 1;
		object->mCmd[0] = 0;
		break;
	case 3:
		if (object->_2098.mFlags & 0x10)
			lbl_80367A2C[object->mCmd[1]] = 1;
		object->mCmd[0] = 0;
		break;
	case 4:
		object->_A4C   = object->mCmd[1];
		object->mState = lbl_8022CFC8;
		fn_8010706C(object);
		object->mCmd[0] = 0;
		break;
	case 5:
		object->_A4C = -1;
		if (object->_A50 != NULL)
			object->_A50->Release();
		object->mCmd[0] = 0;
		break;
	}
}

/**
 * @note Address: 0x8011ED48
 * @note Size: 0x144
 */
extern "C" void fn_8011ED48(XObjActorToy* object)
{
	switch (object->mCmd[0]) {
	case 0:
		break;
	case 1:
		switch (object->mCmd[1]) {
		case 0:
			object->_2098.mFlags &= ~1U;
			object->_984 = 0.0f;
			object->_990 = 0;
			object->SetMoveDir(CVec(object->mMoveTranslate.x, object->mMoveTranslate.y, object->mMoveTranslate.z), 0.0f);
			break;
		case 1:
			object->_2098.mFlags |= 1;
			object->_9C8.x = object->mCmd[2] / 100.0f;
			object->_9C8.y = object->mCmd[3] / 100.0f;
			object->_9C8.z = object->mCmd[4] / 100.0f;
			object->_990   = 1;
			break;
		}
		object->mCmd[0] = 0;
		break;
	}
}
