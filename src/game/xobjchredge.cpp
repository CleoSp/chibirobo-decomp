#include "game/xobj.h"
#include <math.h>
#include "game/gmath.h"

extern "C" {
extern Vec lbl_80378258;
extern Vec lbl_8037824C;
}

static inline Vec FirstEdgePoint(const XObj* object, s32 index, f32 height)
{
	Vec point = lbl_80378258;
	point.x   = object->mMoveTranslate.x + object->mEdges[index].mFrom.x;
	point.y   = height;
	point.z   = object->mMoveTranslate.z + object->mEdges[index].mFrom.z;
	return point;
}

static inline Vec SecondEdgePoint(const XObj* object, s32 index, f32 height)
{
	Vec point = lbl_8037824C;
	point.x   = object->mMoveTranslate.x + object->mEdges[index].mTo.x;
	point.y   = height;
	point.z   = object->mMoveTranslate.z + object->mEdges[index].mTo.z;
	return point;
}

/**
 * @note Address: 0x800B1EC0
 * @note Size: 0x144
 */
extern "C" f32 fn_800B1EC0(const XObj* object, Vec* point, s32 index)
{
	Vec first  = FirstEdgePoint(object, index, point->y);
	Vec second = SecondEdgePoint(object, index, point->y);
	CVec direction;
	fn_8018E824(point, &first, &second, &direction);
	f32 angle = atan2(direction.x, direction.z);
	angle     = WrapAngle(angle);
	return angle;
}
