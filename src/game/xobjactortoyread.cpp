#include "game/xobjactor.h"
#include "game/stagedata.h"
#include "game/xobjs.h"

extern "C" {
extern u8 lbl_8022C7E0[];
}

static inline void PlaceChild(XObj* child, const Vec& position)
{
	child->mStatus.mFlags &= ~0x10000ULL;
	child->SetPos(position, 0);
}

/**
 * @note Address: 0x80122978
 * @note Size: 0x64
 */
extern "C" void fn_80122978(XObjActorToy* object)
{
	object->mFirst.Release();
	object->mSecond.Release();
	fn_800911C0(object);
	object->mLoad = 0;
	object->_1F90 = 0;
}

/**
 * @note Address: 0x801229DC
 * @note Size: 0x1E8
 */
extern "C" bool fn_801229DC(XObjActorToy* object, const CoordInfo& info)
{
	switch (object->GetKind()) {
	case 15:
		switch (object->mLoad) {
		case 0:
			if (!fn_80107F18(object, info))
				break;
			++object->mLoad;
		case 1:
			if (!object->mFirst.vf9C(CoordInfo(GetSpawnData(42))))
				break;
			PlaceChild(&object->mFirst, object->mMoveTranslate);
			++object->mLoad;
		case 2:
			if (!object->mSecond.vf9C(CoordInfo(GetSpawnData(43))))
				break;
			PlaceChild(&object->mSecond, object->mMoveTranslate);
			++object->mLoad;
			break;
		}
		if (object->mLoad > 2) {
			object->mLoad = 0;
			return true;
		}
		return false;
	default:
		return fn_80107F18(object, info) != 0;
	}
}

/**
 * @note Address: 0x80122BC4
 * @note Size: 0x174
 */
extern "C" void fn_80122BC4(XObjActorToy* object)
{
	const u8* definitions = lbl_8022C7E0;
	fn_801081F0(object);
	object->mLoad = 0;
	object->_1F90 = 0;
	object->mFirst.vf98();
	object->mSecond.vf98();
	AnimSfxEvent*& table = object->mAnimSfx.mEvents;
	switch (object->GetKind()) {
	case 0x6F:
		table = (AnimSfxEvent*)(definitions + 0x374);
		break;
	case 0x110:
		table = (AnimSfxEvent*)(definitions + 0x68);
		break;
	case 0x117:
		table = (AnimSfxEvent*)(definitions + 0x50C);
		break;
	case 0x159:
		table = (AnimSfxEvent*)(definitions + 0x4DC);
		break;
	case 0x1FB:
		table = (AnimSfxEvent*)(definitions + 0x62C);
		break;
	case 0x1FC:
		table = (AnimSfxEvent*)(definitions + 0x3EC);
		break;
	case 0x25E:
		table = (AnimSfxEvent*)(definitions + 0x74C);
		break;
	case 0x317:
		table = (AnimSfxEvent*)(definitions + 0x68C);
		break;
	case 0x26C:
	case 0x485:
		table = (AnimSfxEvent*)(definitions + 0x4AC);
		break;
	default:
		table = NULL;
		break;
	}
}

/**
 * @note Address: 0x80122D38
 * @note Size: 0x60
 */
extern "C" void fn_80122D38(XObjActorToy* object)
{
	fn_80108014(object);
	object->mLoad = 0;
	object->_1F90 = 0;
	object->mFirst.vf94();
	object->mSecond.vf94();
}

/**
 * @note Address: 0x80122D98
 * @note Size: 0x74
 */
extern "C" void fn_80122D98(XObjActorToy* object, s32 argument)
{
	fn_801083F0(object, argument);
	object->mLoad = 0;
	object->_1F90 = 0;
	object->mFirst.vf90(argument);
	object->mSecond.vf90(argument);
}

/**
 * @note Address: 0x80122E0C
 * @note Size: 0x24
 */
extern "C" void fn_80122E0C(XObjActorJenny* object)
{
	object->mState = NULL;
}

/**
 * @note Address: 0x80122E30
 * @note Size: 0x58
 */
extern "C" void fn_80122E30(XObjActorJenny* object, u32 frames)
{
	if (object->mState != NULL)
		(object->*object->mState)(frames);
}
