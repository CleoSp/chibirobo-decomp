#include "game/xchr.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/hsd_dobj.h"
#include "game/csound.h"
#include "game/inventory.h"

// the part of a particle the color callback writes
struct ItemPtclColor {
	u8 _00[0x12]; // _00
	u8 mRed;      // _12
	u8 mGreen;    // _13
	u8 mBlue;     // _14
};

// the color callback in lbl_80228270
/**
 * @note Address: 0x800D47F4
 * @note Size: 0x38
 */
extern "C" s32 fn_800D47F4(ItemPtclColor* particle)
{
	// retail reads the color as one word
	u32 color        = lbl_8025C400.mPlayer->mItemTintWord;
	particle->mRed   = color >> 24;
	particle->mGreen = color >> 16;
	particle->mBlue  = color >> 8;
	return 0;
}

/**
 * @note Address: 0x800D482C
 * @note Size: 0x16C
 */
extern "C" void fn_800D482C(XAction* action)
{
	s32 path[2]     = { 0, 1 };
	HSD_JObj* joint = GetJointByPath(action->mTarget->mJObj, path, 2);
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			GXColor& color   = action->mOwner->mItemTint;
			GXColor* diffuse = &material->mat->diffuse;
			diffuse->r       = color.r;
			diffuse->g       = color.g;
			diffuse->b       = color.b;
			diffuse->a       = color.a;
			for (HSD_TObj* texture = HSD_MObjGetTObj(material); texture != NULL; texture = HSD_TObjGetNext(texture)) {
				u32 flags      = texture->flags & ~0xF0000;
				texture->flags = flags;
				texture->flags = flags | 0x60000;
			}
			HSD_MObjCompileTev(material);
		}
	}
}

/**
 * @note Address: 0x800D4998
 * @note Size: 0x1C8
 */
extern "C" void fn_800D4998(XAction* action)
{
	XObjChr* player = action->mOwner;
	if (player->mAnimId == 0x8B1 || player->mAnimId == 0x8B3) {
		player->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else if (player->mAnimId == 11) {
		player->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	action->mState              = 0;
	action->mOwner->mAttachment = 0;
	action->mTarget->mStatus.mFlags &= ~0x0200000000000000ULL;
	action->mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	action->_24    = 20;
	XObjItem* item = (XObjItem*)action->mTarget;
	s32 type       = item->GetObjFlags();
	fn_800DC9B0(&action->mOwner->mParams.mInventory, type, 1, TRUE);
	item->SetFlag();
	fn_8001E904((CSound*)&lbl_8025C400.mSound, 0x39, 255);
}
