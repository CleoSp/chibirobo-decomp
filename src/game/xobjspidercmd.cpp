#include "game/xobjspider.h"
#include "game/cgame.h"

/**
 * @note Address: 0x800F1730
 * @note Size: 0x3E0
 */
extern "C" void fn_800F1730(XObjSpider* spider)
{
	switch (spider->mCommand) {
	case 1:
		spider->SetState(3);
		spider->mCommand = 0;
		break;
	case 2:
		if (spider->mFlags.mFlags & 0x10000)
			lbl_8025C400._5D08C = 1;
		else
			lbl_8025C400._5D08C = 0;
		spider->mCommand = 0;
		break;
	case 3:
		if (spider->mFlags.mFlags & 0x80000)
			lbl_8025C400._5D08C = 1;
		else
			lbl_8025C400._5D08C = 0;
		spider->mCommand = 0;
		break;
	case 4:
		spider->mFlags.mFlags |= 0x00200000;
		spider->mCommand = 0;
		break;
	case 5:
		spider->mFlags.mFlags |= 0x00400000;
		spider->mCommand = 0;
		break;
	case 6:
		spider->mStatus.mFlags |= 0x00100000ULL;
		spider->mCommand = 0;
		break;
	case 7:
		if (spider->mArgs[0] == 1)
			spider->mFlags.mFlags &= ~0x08000000U;
		else
			spider->mFlags.mFlags |= 0x08000000;
		spider->mCommand = 0;
		spider->mArgs[0] = 0;
		break;
	case 8:
		fn_800F38F4(spider);
		spider->mCommand = 0;
		break;
	case 9:
		spider->mHome.x          = spider->mArgs[0] / 100.0f;
		spider->mHome.y          = spider->mArgs[1] / 100.0f;
		spider->mHome.z          = spider->mArgs[2] / 100.0f;
		spider->mStartPos.x      = spider->mHome.x;
		spider->mStartPos.y      = spider->mHome.y;
		spider->mStartPos.z      = spider->mHome.z;
		spider->mMoveTranslate.x = spider->mHome.x;
		spider->mMoveTranslate.y = spider->mHome.y + (f32)spider->_260;
		spider->mMoveTranslate.z = spider->mHome.z;
		spider->mMoveTranslate.SetTarget(spider->mMoveTranslate.x, spider->mMoveTranslate.y, spider->mMoveTranslate.z);
		spider->mCommand = 0;
		break;
	case 10:
		spider->mAreaPos.x = spider->mArgs[0] / 100.0f;
		spider->mAreaPos.y = spider->mArgs[1] / 100.0f;
		spider->mAreaPos.z = spider->mArgs[2] / 100.0f;
		spider->mAreaWidth = (f32)spider->mArgs[3];
		spider->mAreaDepth = (f32)spider->mArgs[4];
		spider->mCommand   = 0;
		break;
	case 11:
		spider->mCommand = 0;
		break;
	case 12:
		spider->mCommand = 0;
		break;
	case 13:
		spider->mFlags.mFlags |= 0x10000000;
		spider->mCommand = 0;
		break;
	case 14:
		spider->mEffectId = spider->mArgs[0];
		spider->mEffectOn = 1;
		spider->mCommand  = 0;
		break;
	case 15:
		spider->mEffectOn = 0;
		spider->mEffectId = -1;
		if (spider->mEffect != NULL)
			spider->mEffect->Release();
		spider->mCommand = 0;
		break;
	case 16:
		spider->mCommand = 0;
		break;
	case 17:
		spider->mCommand = 0;
		break;
	case 18:
		spider->mCommand = 0;
		break;
	case 19:
		spider->SetState(5);
		spider->mCommand = 0;
		break;
	}
}
