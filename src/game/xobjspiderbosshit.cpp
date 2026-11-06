#include "game/xobjspiderboss.h"
#include "game/xchr.h"
#include "game/cgame.h"
#include "game/cparticle.h"

#pragma section RX ".text.spiderbosshitflag"

/**
 * @note Address: 0x801051A0
 * @note Size: 0x14
 */
__declspec(section ".text.spiderbosshitflag") s32 XObjSpiderBoss::vf100(s32 unused, s32 value)
{
	mSpiderStatus.mFlags |= 1;
	return value;
}

/**
 * @note Address: 0x801051B4
 * @note Size: 0x1A8
 */
// hit from pos, only once chibi robo has the item that does it
void XObjSpiderBoss::OnHitFromPosition(const Vec& pos, f32 strength)
{
	u32 flags = mSpiderStatus.mFlags;
	if (flags & 2) {
		return;
	}
	if (flags & 0x10) {
		return;
	}
	if (flags & 0x8000) {
		return;
	}
	XObjChr* player = gGame.GetPlayer();
	if (GetAttachmentCountById(&player->mAttachments, 2) < 2) {
		return;
	}
	XObj::OnHitFromPosition(pos, strength);
	mSpiderStatus.mFlags |= 2;
	f32 x               = pos.x;
	f32 y               = pos.y;
	f32 z               = pos.z;
	Vec hit             = { 0.0f, 0.0f, 0.0f };
	hit.x               = x;
	hit.y               = y;
	hit.z               = z;
	mHitPos             = hit;
	mHitStrength        = strength;
	CParticle* particle = &gGame.mParticle;
	particle->Kill(0xB3);
	Vec scale = { 1.0f, 1.0f, 1.0f };
	particle->CreateSRT(0xB3, 1, pos, scale, (Vec) { 0, mTargetRotY, 0 }, TRUE);
	XSound* sound = &gGame.mSound;
	if (!IsGameSoundPlaying(sound, 0x127)) {
		PlayGameSound(sound, 0x127, 255); // npc_spider_bakuhatsu
	}
}
