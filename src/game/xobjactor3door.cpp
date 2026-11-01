#include "game/xobjactor.h"
#include "game/xobjs.h"
#include "game/cgame.h"
#include "game/infoscrn.h"

static inline void ForwardRequest(XObjActor3* object, u8 enabled)
{
	CGame* game  = &lbl_8025C400;
	u8* selected = game->mPools[2].mObjects + (u32)object->_258 * game->mPools[2].mStride;
	fn_800661E4((InfoScrn*)game->mInfoScrn, (s32)selected, enabled, 1);
}

/**
 * @note Address: 0x801151E0
 * @note Size: 0xBC
 */
extern "C" void fn_801151E0(XObjActor3* object)
{
	switch (object->_B94) {
	case 1:
		ForwardRequest(object, 1);
		object->_B94 = 0;
		break;
	case 2:
		ForwardRequest(object, 0);
		object->_B94 = 0;
		break;
	}
}

/**
 * @note Address: 0x8011529C
 * @note Size: 0x4
 */
extern "C" void fn_8011529C(XObjActor3* object)
{
}
