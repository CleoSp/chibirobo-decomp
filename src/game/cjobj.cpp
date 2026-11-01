#include "game/cjobj.h"
#include "game/cjobjset.h"

// todo: weak copy, CJObj setters out of cjobj.h

/**
 * @note Address: 0x80017244
 * @note Size: 0xC4
 */
extern "C" void fn_80017244(CJObj* object, f32 x, s32 force)
{
	object->CJObj::SetTranslateX(x, force);
}

/**
 * @note Address: 0x80017308
 * @note Size: 0xC4
 */
extern "C" void fn_80017308(CJObj* object, f32 y, s32 force)
{
	object->CJObj::SetTranslateY(y, force);
}

/**
 * @note Address: 0x800173CC
 * @note Size: 0xC4
 */
extern "C" void fn_800173CC(CJObj* object, f32 z, s32 force)
{
	object->CJObj::SetTranslateZ(z, force);
}

/**
 * @note Address: 0x80017490
 * @note Size: 0xC4
 */
extern "C" void fn_80017490(CJObj* object, f32 x, s32 force)
{
	object->CJObj::SetScaleX(x, force);
}

/**
 * @note Address: 0x80017554
 * @note Size: 0xC4
 */
extern "C" void fn_80017554(CJObj* object, f32 y, s32 force)
{
	object->CJObj::SetScaleY(y, force);
}

/**
 * @note Address: 0x80017618
 * @note Size: 0xC4
 */
extern "C" void fn_80017618(CJObj* object, f32 z, s32 force)
{
	object->CJObj::SetScaleZ(z, force);
}

/**
 * @note Address: 0x800176DC
 * @note Size: 0x19C
 */
extern "C" void fn_800176DC(CJObj* object, f32 x, s32 force)
{
	object->CJObj::SetRotationX(x, force);
}

/**
 * @note Address: 0x80017878
 * @note Size: 0x19C
 */
extern "C" void fn_80017878(CJObj* object, f32 y, s32 force)
{
	object->CJObj::SetRotationY(y, force);
}

/**
 * @note Address: 0x80017A14
 * @note Size: 0x19C
 */
extern "C" void fn_80017A14(CJObj* object, f32 z, s32 force)
{
	object->CJObj::SetRotationZ(z, force);
}

/**
 * @note Address: 0x80017BB0
 * @note Size: 0x16C
 */
extern "C" void fn_80017BB0(CJObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->CJObj::SetTranslate(x, y, z, force);
}

/**
 * @note Address: 0x80017D1C
 * @note Size: 0x14C
 */
extern "C" void fn_80017D1C(CJObj* object, const Vec& position, s32 force)
{
	object->CJObj::SetTranslate(position, force);
}

/**
 * @note Address: 0x80017E68
 * @note Size: 0x168
 */
extern "C" void fn_80017E68(CJObj* object, f32 x, f32 y, f32 z, s32 force)
{
	object->CJObj::SetScale(x, y, z, force);
}

/**
 * @note Address: 0x80017FD0
 * @note Size: 0x148
 */
extern "C" void fn_80017FD0(CJObj* object, const Vec& scale, s32 force)
{
	object->CJObj::SetScale(scale, force);
}
