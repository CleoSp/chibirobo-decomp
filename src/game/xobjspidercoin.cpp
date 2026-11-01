#include "game/xobjspider.h"
#include "game/xobjflags.h"
#include "game/cgame.h"
#include "game/hsd_cobj.h"
#include <math.h>
#include "game/gmath.h"
#include "game/csound.h"

// a coin a spider knocked loose, it hops up and drops to mTarget
struct CoinBounce {
	XObj* mCoin; // _00
	Vec mPos;    // _04
	s32 mPhase;  // _10
	s32 mCount;  // _14
	Vec mTarget; // _18
};
STATIC_ASSERT(sizeof(CoinBounce) == 0x24);

extern "C" {
extern CoinBounce lbl_8037E640[11];
extern s32 lbl_8065B2C4;
}

/**
 * @note Address: 0x800EA40C
 * @note Size: 0x148
 */
extern "C" void fn_800EA40C(XObjSpider* spider)
{
	if (spider->mStatus.mFlags & 1ULL) {
		if (spider->mStatus.mFlags & 0x10000ULL) {
			HSD_JObj* joint = spider->mJObj;
			s32 id          = spider->mAnimId;
			HSD_AObj* aobj  = GetAObj(joint);
			f32 frame       = aobj != NULL ? HSD_AObjGetCurrentFrame(aobj) : 0.0f;
			spider->mSfx.Update(id, frame, fn_8001DF00((CSound*)&lbl_8025C400.mSound, &spider->mMoveTranslate, 0));
		}
	}
}

/**
 * @note Address: 0x800EA554
 * @note Size: 0x22C
 */
extern "C" void fn_800EA554(Vec* position, Vec* target, s32 anim)
{
	if (lbl_8065B2C4 >= 11)
		return;
	u32 stride = lbl_8025C400.mPools[4].mStride;
	u8* pool   = lbl_8025C400.mPools[4].mObjects;
	u32 offset = 0;
	for (s32 i = 0; i < 96; ++i, offset += stride) {
		XObjCoin* coin = (XObjCoin*)(pool + offset);
		if (coin->mStatus.mFlags & 1ULL) {
			if (coin->GetKind() == 0x104) {
				if ((coin->mStatus.mFlags & 1ULL) && !coin->mBouncing) {
					coin->mBouncing                   = 1;
					lbl_8037E640[lbl_8065B2C4].mPhase = 0;
					lbl_8037E640[lbl_8065B2C4].mCount += 2;
					lbl_8037E640[lbl_8065B2C4].mPos.x    = position->x;
					lbl_8037E640[lbl_8065B2C4].mPos.y    = position->y + 0.2f;
					lbl_8037E640[lbl_8065B2C4].mPos.z    = position->z;
					lbl_8037E640[lbl_8065B2C4].mTarget.x = target->x;
					lbl_8037E640[lbl_8065B2C4].mTarget.y = target->y;
					lbl_8037E640[lbl_8065B2C4].mTarget.z = target->z;
					lbl_8037E640[lbl_8065B2C4].mCoin     = coin;
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateX(position->x, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateY(position->y + 0.2f, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->SetTranslateZ(position->z, 0);
					lbl_8037E640[lbl_8065B2C4].mCoin->mStatus.mFlags |= 0x100000ULL;
					lbl_8037E640[lbl_8065B2C4].mCoin->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					++lbl_8065B2C4;
					break;
				}
			}
		}
	}
}

/**
 * @note Address: 0x800EA780
 * @note Size: 0x21C
 */
extern "C" void fn_800EA780(void* unused, u32 frames)
{
	f32 fall = 0.8f * (f32)frames;
	for (s32 i = 0; i < 11; ++i) {
		CoinBounce* entry = &lbl_8037E640[i];
		XObj* coin        = entry->mCoin;
		if (coin != NULL) {
			s32 phase   = entry->mPhase;
			s32 counter = entry->mCount;
			f32 targetY = entry->mTarget.y;
			f32 x       = entry->mPos.x;
			f32 y       = entry->mPos.y;
			f32 z       = entry->mPos.z;
			switch (phase) {
			case 0:
				coin->mStatus.mFlags |= 0x10000ULL;
				++phase;
				break;
			case 1: {
				++counter;
				f32 height = 10.0f * (f32)sin(DEG2RAD * (f32)((counter * 16) % 360)) + 0.2f;
				if (height < 0.2f) {
					coin->SetTranslateY(y + 0.2f, 0);
					++phase;
				} else {
					coin->SetTranslateY(y + height, 0);
				}
				break;
			}
			case 2:
				y -= fall;
				if (y <= targetY) {
					coin->mStatus.mFlags &= ~0x100000ULL;
					y = targetY;
					++phase;
				}
				coin->SetTranslateY(y + 0.2f, 0);
				break;
			case 3:
				break;
			}
			entry->mPhase = phase;
			entry->mCount = counter;
			entry->mPos.x = x;
			entry->mPos.y = y;
			entry->mPos.z = z;
		}
	}
}

/**
 * @note Address: 0x800EA99C
 * @note Size: 0x1BC
 */
extern "C" void fn_800EA99C(void* unused)
{
	lbl_8065B2C4 = 0;
	for (s32 i = 0; i < 11; ++i) {
		lbl_8037E640[i].mCoin  = NULL;
		lbl_8037E640[i].mPhase = 0;
		lbl_8037E640[i].mCount = 0;
	}
	for (s32 i = 0; i < 96; ++i) {
		u32 stride     = lbl_8025C400.mPools[4].mStride;
		u8* pool       = lbl_8025C400.mPools[4].mObjects;
		XObjCoin* coin = (XObjCoin*)(pool + i * stride);
		if (coin->GetKind() == 0x104)
			coin->mBouncing = 0;
	}
}
