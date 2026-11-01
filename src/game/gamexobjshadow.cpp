#include "game/xobj.h"
#include "game/cshadow.h"
#include "game/hsd_dobj.h"
extern "C" {
#include <sysdolphin/mtx.h>
}
#include <math.h>

// what fn_8000CBF4 gets passed while it walks a model
struct ShadowHeightRange {
	f32 mCeiling; // _00, joints above it don't take the shadow
	f32 mHighest; // _04, the highest joint under it, NAN before the first
};

/**
 * @note Address: 0x8000CB4C
 * @note Size: 0x74
 */
// XObj's vfD4, a square shadow the size of vf108
extern "C" void fn_8000CB4C(XObj* object, CShadow* shadow)
{
	f32 radius   = object->vf108();
	Vec position = object->mMoveTranslate;
	shadow->AddRect(&position, radius, -radius, -radius, radius);
}

/**
 * @note Address: 0x8000CBC0
 * @note Size: 0x8
 */
// XObj's vf108, the shadow radius
extern "C" f32 fn_8000CBC0(XObj* object)
{
	return 40.0f;
}

/**
 * @note Address: 0x8000CBC8
 * @note Size: 0x2C
 */
// XObj's vfD0, the model itself casts the shadow
extern "C" void fn_8000CBC8(XObj* object, CShadow* shadow)
{
	HSD_ShadowAddObject(shadow->mShadow, (HSD_Obj*)object->mJObj);
}

/**
 * @note Address: 0x8000CBF4
 * @note Size: 0x1A8
 */
// a joint walk callback, flags the joint's materials to take the shadow if it's low enough
extern "C" void fn_8000CBF4(HSD_JObj* joint, ShadowHeightRange* heights)
{
	Vec position;
	HSD_MtxGetTranslate(HSD_JObjGetMtxPtr(joint), &position);
	BOOL enabled = position.y <= heights->mCeiling;
	if (enabled) {
		if (__fpclassifyf(heights->mHighest) == FP_NAN)
			heights->mHighest = position.y;
		else if (heights->mHighest < position.y)
			heights->mHighest = position.y;
	}
	HSD_DObj* object = HSD_JObjGetDObj(joint);
	while (object != NULL) {
		HSD_MObj* material = HSD_DObjGetMObj(object);
		if (material != NULL) {
			if (enabled)
				HSD_MObjSetFlags(material, RENDER_SHADOW);
			else
				HSD_MObjClearFlags(material, RENDER_SHADOW);
		}
		object = HSD_DObjGetNext(object);
	}
}

/**
 * @note Address: 0x8000CD9C
 * @note Size: 0x4
 */
// XObj's vfC0
extern "C" void fn_8000CD9C(XObj* object)
{
}
