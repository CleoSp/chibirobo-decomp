#include "game/particle_srt.h"
#include "game/objb_act_shared.h"

extern "C" {

static inline void PutDown(XActLift* act)
{
	XObjFlags* target;
	XObjChr* chr = act->mChr;
	if (chr->mAnimId == 0x8B1 || chr->mAnimId == 0x8B3) {
		chr->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else if (chr->mAnimId == 0xB) {
		chr->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	act->mState      = 0;
	act->mChr->_95D0 = 0;
	act->mTarget->mFlags &= ~0x0200000000000000ULL;
	act->mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	act->_24 = 0x14;
	target   = (XObjFlags*)act->mTarget;
	fn_800DC9B0(&act->mChr->mStats.mTable, target->GetKind(), 1, TRUE);
	target->SetBit(target->GetFlagNo());
	fn_8001E904(GAME_SOUND, 0x39, 0xFF);
}

/**
 * @note Address: 0x800D3A5C
 * @note Size: 0x94
 */
HSD_Generator* fn_800D3A5C(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, BOOL keep)
{
	// the dol's is game/particle_srt.h's CreateParticle out of line, its rot is that header's
	// fifth Vec. inlined into here MWCC swaps the two stack slots, so this keeps its own
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return game->mParticle.CreateSRT(id | (bank << 16), status, pos, ParticleUnitScale(), rot, keep);
}

/**
 * @note Address: 0x800D3BA4
 * @note Size: 0x5C
 */
HSD_Generator* fn_800D3BA4(UnkGame* game, s32 bank, s32 id, s32 status, const Vec& pos, const Vec& scale, BOOL keep)
{
	return CreateParticle(game->mParticle, bank, id, status, pos, scale, keep);
}

/**
 * @note Address: 0x800D3CB8
 * @note Size: 0x24
 */
void fn_800D3CB8(UnkStatus* status, s16 id, s16 n)
{
	fn_800DC754(&status->mTable, id, n);
}

/**
 * @note Address: 0x800D3CDC
 * @note Size: 0x30
 */
BOOL fn_800D3CDC(XObj* obj)
{
	return !(obj->mFlags & 0x0040000000000000ULL);
}

/**
 * @note Address: 0x800D3D0C
 * @note Size: 0x64
 */
UnkXObjReadObj* fn_800D3D0C(UnkGame* game, s16 id)
{
	return fn_80132698((UnkXObjList*)game->mObjList, &CoordInfo(fn_8006EDB4(id)), 0, TRUE);
}

/**
 * @note Address: 0x800D3D70
 * @note Size: 0x44
 */
void fn_800D3D70(UnkLimited* lim, f32 value)
{
	lim->mValue = value > 0.0f ? value : 0.0f;
	if (lim->mValue < 0.0f) {
		lim->mValue = 0.0f;
	}
	if (lim->mValue > lim->mMax) {
		lim->mValue = lim->mMax;
	}
}

/**
 * @note Address: 0x800D3DB4
 * @note Size: 0x20
 */
void fn_800D3DB4(UnkGame* game, u32 flags)
{
	game->_43D0 = 1;
	game->_43D4 = 0;
	game->_43D4 |= flags;
}

/**
 * @note Address: 0x800D3DD4
 * @note Size: 0x24
 */
void fn_800D3DD4(UnkGame* game, f32 a, f32 b)
{
	fn_800136B4(game->_042AC, a, b);
}

/**
 * @note Address: 0x800D3EB0
 * @note Size: 0x14
 */
void fn_800D3EB0(UnkGame* game, f32 a, f32 b, f32 c, f32 d)
{
	game->_43E0[0] = a;
	game->_43E0[1] = b;
	game->_43E0[2] = c;
	game->_43E0[3] = d;
}

/**
 * @note Address: 0x800D3EC4
 * @note Size: 0x50
 */
void fn_800D3EC4(XObj* obj)
{
	obj->mMsgCount = 0;
	for (int i = 0; i < 16; i++) {
		obj->mMsgs[i].mId = -1;
	}
}

/**
 * @note Address: 0x800D3AF0
 * @note Size: 0xB4
 */
Vec fn_800D3AF0(void* unused, HSD_JObj* jobj)
{
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(jobj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3C00
 * @note Size: 0xB8
 */
// NONMATCHING: the dol loads the jobj before saving r30
Vec fn_800D3C00(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(jobj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3DF8
 * @note Size: 0xB8
 */
// NONMATCHING: same as fn_800D3C00
Vec fn_800D3DF8(const CJObj* obj)
{
	HSD_JObj* jobj = obj->mJObj;
	Vec pos;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(jobj), &pos);
	return pos;
}

CVec fn_800A0978(XObjChr* chr);
}
