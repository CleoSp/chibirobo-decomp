#include "game/xobjattachment.h"
#include "game/objectfloorquery.h"
#include "game/cgame.h"
#include "game/cvec.h"

/**
 * @note Address: 0x800D9B10
 * @note Size: 0x44C
 */
extern "C" f32 FindPlugCordFloor(XObjPlugCord* cord, Vec* pos, f32 radius, f32 height)
{
	XObjMap* map = (XObjMap*)gGame.mXObjs.mPools[3].mObjects;
	if (!FindMapFloor(map, pos, &cord->mFloorHit, NULL)) {
		cord->mFloorHit.mTriangle = -1;
		cord->mFloorHit.mTrigon   = -1;
		cord->mFloorHit.mHeight   = pos->y;
	}
	f32 highest = cord->mFloorHit.mHeight;
	CVec contact, push;
	for (int i = 0; i < gGame.mXObjs.mObjectCount; ++i) {
		XObj* object = gGame.GetObject(i);
		if (!(object->mStatus.mFlags & 1) || !(object->mStatus.mFlags & 0x10000) || (object->mStatus.mFlags & 0x100000))
			continue;
		f32 dx = object->mMoveTranslate.x - pos->x;
		f32 dz = object->mMoveTranslate.z - pos->z;
		if (sqrtf(dx * dx + dz * dz) > 100.0f + (radius + object->_6F8))
			continue;
		s32 floor = 0;
		while (floor != -1) {
			if (ProbeObjectFloor(object, CVec(pos->x, pos->y, pos->z), CVec(pos->x, pos->y, pos->z), radius, height, &push, &contact,
			                     &floor)
			    & 1) {
				f32 y = object->mFloorHeights[object->_274];
				if (__fpclassifyf(y) != FP_NAN && pos->y + 1.0f >= y && y >= highest)
					highest = y;
			}
		}
	}
	if (highest > cord->mFloorHit.mHeight)
		cord->mFloorHit.mHeight = highest;
	return cord->mFloorHit.mHeight;
}

extern "C" {
extern const f32 lbl_8065D954; // 1.0f
extern Vec gPlugFloorOffsetTemplate, gPlugFloorExtentTemplate;
}
static inline Vec FloorOffset(XObj* obj, int i)
{
	Vec v = gPlugFloorOffsetTemplate;
	v.x   = obj->mFloors[i]._04[0] / 100.0f;
	v.y   = obj->mFloors[i]._04[1] / 100.0f;
	v.z   = obj->mFloors[i]._04[2] / 100.0f;
	return v;
}
static inline Vec FloorExtent(XObj* obj, int i)
{
	Vec v = gPlugFloorExtentTemplate;
	v.x   = obj->mFloors[i]._14[0] / 100.0f;
	v.y   = obj->mFloors[i]._14[1] / 100.0f;
	v.z   = obj->mFloors[i]._14[2] / 100.0f;
	return v;
}
/**
 * @note Address: 0x800D9F5C
 * @note Size: 0x574
 */
#pragma push
// Retail rounds the floor rotation before adding the object rotation.
#pragma fp_contract off
extern "C" BOOL PushPlugCordPointFromObject(CVec* pos, XObj* object, f32 radius)
{
	static f32 verticalMargin = lbl_8065D954;
	if (!(object->mStatus.mFlags & 0x0200000000000000ULL))
		return FALSE;
	if (object->mStatus.mFlags & 0x0c00000000000000ULL)
		return FALSE;
	if (object->mStatus.mFlags & 0x4000000000000000ULL)
		return FALSE;
	CVec point, edge;
	point.x     = pos->x;
	point.y     = pos->y;
	point.z     = pos->z;
	point.y     = 0.0f;
	BOOL pushed = FALSE;
	for (int i = 0; object->mFloors[i].mId != -1; ++i) {
		CVec origin(object->mMoveTranslate);
		Vec centerOffset = object->mMoveScale * FloorOffset(object, i);
		Vec size         = object->mMoveScale * FloorExtent(object, i);
		f32 angle        = object->mMoveRotate.y + DEG2RAD * object->mFloors[i]._10;
		switch (object->mFloors[i].mId) {
		case 0:
		case 1: {
			Mtx translate, local, rotate, scale, world;
			CVec center(0, 0, 0);
			PSMTXTrans(translate, origin.x, origin.y, origin.z);
			PSMTXTrans(local, centerOffset.x, centerOffset.y, centerOffset.z);
			PSMTXScale(scale, size.x, size.y, size.z);
			PSMTXRotRad(rotate, 'y', angle);
			PSMTXConcat(translate, rotate, world);
			PSMTXConcat(world, local, world);
			PSMTXConcat(world, scale, world);
			PSMTXMultVec(world, &center, &center);
			f32 centerY = center.y;
			center.y    = 0.0f;
			f32 reach   = radius + size.x;
			if (PSVECDistance(&center, &point) < reach) {
				CVec direction = (point - center).Normalize();
				edge.Set(center + direction * reach);
				if (pos->y <= centerY + size.y + verticalMargin && pos->y >= centerY - size.y - verticalMargin) {
					pos->x = edge.x;
					pos->z = edge.z;
					pushed = TRUE;
				}
			}
			break;
		}
		}
	}
	return pushed;
}

#pragma pop
