#include "game/hsd_jobj.h"
#include "game/ut.h"
extern "C" {
#include <sysdolphin/mtx.h>
}

extern "C" {
void BlendJointPose(HSD_JObj* a, HSD_JObj* b, HSD_JObj* out, f32 t);
void CopyJointPoseAsQuaternion(HSD_JObj* src, HSD_JObj* dst);
}

/**
 * @note Address: 0x8018F6EC
 * @note Size: 0x2C8
 */
// one joint of the copy, the rotation ends up a quaternion
void CopyJointPoseAsQuaternion(HSD_JObj* src, HSD_JObj* dst)
{
	Vec v;
	Quaternion q;

	HSD_JObjGetTranslation(src, &v);
	HSD_JObjSetTranslate(dst, &v);
	HSD_JObjGetScale(src, &v);
	HSD_JObjSetScale(dst, &v);
	if (src->flags & USE_QUATERNION) {
		HSD_JObjGetRotation(src, &q);
	} else {
		HSD_JObjGetRotation(src, (Quaternion*)&v);
		EulerToQuat(&v, &q);
	}
	HSD_JObjSetRotation(dst, &q);
}

/**
 * @note Address: 0x8018F658
 * @note Size: 0x94
 */
// copies the pose of a whole tree onto another
void CopyJointPoseTree(HSD_JObj* src, HSD_JObj* dst)
{
	HSD_JObj* s;
	HSD_JObj* d;

	if (src == NULL || dst == NULL) {
		return;
	}
	CopyJointPoseAsQuaternion(src, dst);
	if (src->flags & INSTANCE) {
		return;
	}
	for (s = src->child, d = dst->child; s != NULL && d != NULL; s = s->next, d = d->next) {
		if (s == NULL || d == NULL) {
			break;
		}
		CopyJointPoseTree(s, d);
	}
}

/**
 * @note Address: 0x8018F14C
 * @note Size: 0x50C
 */
// one joint of the blend, translate and scale linear, rotation slerped
void BlendJointPose(HSD_JObj* a, HSD_JObj* b, HSD_JObj* out, f32 t)
{
	f32 s = 1.0f - t;
	Vec va;
	Vec vb;
	Vec v;
	Quaternion q;
	Quaternion qa;
	Quaternion qb;

	HSD_JObjGetTranslation(a, &va);
	HSD_JObjGetTranslation(b, &vb);
	v.x = va.x * s + vb.x * t;
	v.y = va.y * s + vb.y * t;
	v.z = va.z * s + vb.z * t;
	HSD_JObjSetTranslate(out, &v);

	HSD_JObjGetScale(a, &va);
	HSD_JObjGetScale(b, &vb);
	v.x = va.x * s + vb.x * t;
	v.y = va.y * s + vb.y * t;
	v.z = va.z * s + vb.z * t;
	HSD_JObjSetScale(out, &v);

	// the euler reads reuse v, w lands past it again
	if (a->flags & USE_QUATERNION) {
		HSD_JObjGetRotation(a, &qa);
	} else {
		HSD_JObjGetRotation(a, (Quaternion*)&v);
		EulerToQuat(&v, &qa);
	}
	if (b->flags & USE_QUATERNION) {
		HSD_JObjGetRotation(b, &qb);
	} else {
		HSD_JObjGetRotation(b, (Quaternion*)&v);
		EulerToQuat(&v, &qb);
	}
	// take the short way round
	if ((qa.x - qb.x) * (qa.x - qb.x) + (qa.y - qb.y) * (qa.y - qb.y) + (qa.z - qb.z) * (qa.z - qb.z) + (qa.w - qb.w) * (qa.w - qb.w)
	    > (qa.x + qb.x) * (qa.x + qb.x) + (qa.y + qb.y) * (qa.y + qb.y) + (qa.z + qb.z) * (qa.z + qb.z) + (qa.w + qb.w) * (qa.w + qb.w)) {
		qb.x = -qb.x;
		qb.y = -qb.y;
		qb.z = -qb.z;
		qb.w = -qb.w;
	}
	QuatSlerp(&qa, &qb, &q, t);
	HSD_JObjSetRotation(out, &q);
	out->flags |= USE_QUATERNION;
}

/**
 * @note Address: 0x8018EEC8
 * @note Size: 0x284
 */
// blends the poses of two trees into a third, t from a to b
void BlendJointTreePoses(HSD_JObj* a, HSD_JObj* b, HSD_JObj* out, f32 t)
{
	HSD_JObj* ca;
	HSD_JObj* cb;
	HSD_JObj* cout;

	if (a == NULL || b == NULL || out == NULL) {
		return;
	}
	BlendJointPose(a, b, out, t);
	if (a->flags & INSTANCE) {
		return;
	}
	for (ca = a->child, cb = b->child, cout = out->child; ca != NULL; ca = ca->next, cb = cb->next, cout = cout->next) {
		if (ca == NULL || cb == NULL || cout == NULL) {
			break;
		}
		BlendJointTreePoses(ca, cb, cout, t);
	}
}
