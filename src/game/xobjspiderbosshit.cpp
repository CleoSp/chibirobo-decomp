#include "game/xobjspiderboss.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cparticle.h"

/**
 * @note Address: 0x801051A0
 * @note Size: 0x14
 */
extern "C" s32 fn_801051A0(XObjSpiderBoss* spider, s32 unused, s32 value)
{
	spider->mSpiderStatus.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x801051B4
 * @note Size: 0x1A8
 */
// hit from pos, only once chibi robo has the item that does it
extern "C" void fn_801051B4(XObjSpiderBoss* spider, const Vec* pos, f32 strength)
{
	u32 flags = spider->mSpiderStatus.mFlags;
	if (flags & 2) {
		return;
	}
	if (flags & 0x10) {
		return;
	}
	if (flags & 0x8000) {
		return;
	}
	XObjChr* player = lbl_8025C400.GetPlayer();
	if (fn_8009C1BC(&player->mAttachments, 2) < 2) {
		return;
	}
	fn_8008A6C0(spider, pos, strength);
	spider->mSpiderStatus.mFlags |= 2;
	f32 x                = pos->x;
	f32 y                = pos->y;
	f32 z                = pos->z;
	Vec hit              = { 0.0f, 0.0f, 0.0f };
	hit.x                = x;
	hit.y                = y;
	hit.z                = z;
	spider->mHitPos      = hit;
	spider->mHitStrength = strength;
	CParticle* particle  = &lbl_8025C400.mParticle;
	particle->Kill(0xB3);
	Vec rot   = { 0.0f, 0.0f, 0.0f };
	rot.y     = spider->mTargetRotY;
	Vec scale = { 1.0f, 1.0f, 1.0f };
	particle->CreateSRT(0xB3, 1, *pos, scale, rot, TRUE);
	XSound* sound = &lbl_8025C400.mSound;
	if (!fn_8001E6C8(sound, 0x127)) {
		fn_8001E904(sound, 0x127, 255); // npc_spider_bakuhatsu
	}
}
