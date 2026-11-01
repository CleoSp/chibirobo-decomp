#include "game/cobj.h"
#include "game/cgame.h"
#include "game/cparticle.h"
#include "game/xobjs.h"
#include "game/xchr.h"
extern "C" {
#include <sysdolphin/mtx.h>
#include "game/stagedata.h"
void fn_800136B4(void* effect, Vec* position, f32 pitch, f32 yaw, f32 arg1, f32 arg2);
}

/**
 * @note Address: 0x800D3A5C
 * @note Size: 0x94
 */
extern "C" HSD_Generator* fn_800D3A5C(CGame* game, u32 bank, u32 id, s32 status, const Vec& position, BOOL keep)
{
	Vec rotation = { 0.0f, 0.0f, 0.0f };
	Vec scale    = { 1.0f, 1.0f, 1.0f };
	return ((CParticle*)game->mParticle)->CreateSRT(id | (bank << 16), status, position, scale, rotation, keep);
}

/**
 * @note Address: 0x800D3AF0
 * @note Size: 0xB4
 */
extern "C" Vec fn_800D3AF0(void* object, HSD_JObj* joint)
{
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(joint), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3BA4
 * @note Size: 0x5C
 */
extern "C" HSD_Generator* fn_800D3BA4(CGame* game, u32 bank, u32 id, s32 status, const Vec& position, const Vec& scale, BOOL keep)
{
	Vec rotation = { 0.0f, 0.0f, 0.0f };
	return ((CParticle*)game->mParticle)->CreateSRT(id | (bank << 16), status, position, scale, rotation, keep);
}

/**
 * @note Address: 0x800D3C00
 * @note Size: 0xB8
 */
extern "C" Vec fn_800D3C00(CJObj* object)
{
	Vec scale;
	HSD_MtxGetScale(HSD_JObjGetMtxPtr(object->mJObj), &scale);
	return scale;
}

/**
 * @note Address: 0x800D3CB8
 * @note Size: 0x24
 */
extern "C" s32 fn_800D3CB8(Params* stats, s16 id, s32 amount)
{
	return fn_800DC754(&stats->mInventory, id, amount);
}

/**
 * @note Address: 0x800D3CDC
 * @note Size: 0x30
 */
extern "C" bool fn_800D3CDC(XObj* object)
{
	return (u8)((object->mStatus.mFlags & 0x0040000000000000ULL) == 0);
}

/**
 * @note Address: 0x800D3D0C
 * @note Size: 0x64
 */
extern "C" XObj* fn_800D3D0C(CGame* game, s16 index)
{
	CoordInfo info(fn_8006EDB4(index));
	return fn_80132698((UnkXObjList*)game->_9694, &info, 0, TRUE);
}

/**
 * @note Address: 0x800D3D70
 * @note Size: 0x44
 */
extern "C" void fn_800D3D70(Params* stats, f32 battery)
{
	stats->mBattery = battery > 0.0f ? battery : 0.0f;
	if (stats->mBattery < 0.0f)
		stats->mBattery = 0.0f;
	if (stats->mBattery > stats->mMaxBattery)
		stats->mBattery = stats->mMaxBattery;
}

/**
 * @note Address: 0x800D3DB4
 * @note Size: 0x20
 */
extern "C" void fn_800D3DB4(CGame* game, u32 mask)
{
	game->_43D0 = 1;
	game->_43D4 = 0;
	game->_43D4 |= mask;
}

/**
 * @note Address: 0x800D3DD4
 * @note Size: 0x24
 */
extern "C" void fn_800D3DD4(CGame* game, Vec* position, f32 pitch, f32 yaw, f32 arg1, f32 arg2)
{
	fn_800136B4(game->mSubwin, position, pitch, yaw, arg1, arg2);
}

/**
 * @note Address: 0x800D3DF8
 * @note Size: 0xB8
 */
extern "C" Vec fn_800D3DF8(CJObj* object)
{
	Vec position;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(object->mJObj), &position);
	return position;
}

/**
 * @note Address: 0x800D3EB0
 * @note Size: 0x14
 */
extern "C" void fn_800D3EB0(CGame* game, f32 a, f32 b, f32 c, f32 d)
{
	game->_43E0 = a;
	game->_43E4 = b;
	game->_43E8 = c;
	game->_43EC = d;
}

/**
 * @note Address: 0x800D3EC4
 * @note Size: 0x50
 */
extern "C" void fn_800D3EC4(XObj* object)
{
	object->mAnmQueue.Clear();
}
