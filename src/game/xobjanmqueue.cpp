#include "game/hsd_jobj.h"
#include "game/ccameraaction.h"

static inline BOOL HasQueuedAnimation(XObj* object)
{
	for (s32 i = 0; i < 16; ++i) {
		s32 index = (object->mAnmQueue.mNext + i) % 16;
		if (object->mAnmQueue.mEntry[index].mId != -1)
			return TRUE;
	}
	return FALSE;
}

static inline void PopAnimation(XObj* object, s32* id, u32* frame, s32* rate)
{
	for (s32 i = 0; i < 16; ++i) {
		s32 index = (object->mAnmQueue.mNext + i) % 16;
		if (object->mAnmQueue.mEntry[index].mId != -1) {
			*id                                 = object->mAnmQueue.mEntry[index].mId;
			*frame                              = object->mAnmQueue.mEntry[index].mFrame;
			*rate                               = object->mAnmQueue.mEntry[index].mRate;
			object->mAnmQueue.mEntry[index].mId = -1;
			object->mAnmQueue.mNext             = (index + 1) % 16;
			break;
		}
	}
}

/**
 * @note Address: 0x800908C0
 * @note Size: 0x3C4
 */
extern "C" void fn_800908C0(XObj* object, Mtx view)
{
	HSD_AObj* animation = GetAObj(object->mJObj);
	if (animation != NULL && (animation->flags & (AOBJ_NO_ANIM | AOBJ_REWINDED)) && HasQueuedAnimation(object)) {
		s32 id;
		u32 frame;
		s32 rate;
		PopAnimation(object, &id, &frame, &rate);
		if (id & 0xFFFF0000) {
			s32 jointId             = id & 0xFFFF;
			s32 materialId          = id >> 16;
			void* jointAnimation    = object->vfF0(jointId);
			void* materialAnimation = object->vfF4(materialId);
			void* shapeAnimation    = object->vfF8(jointId);
			object->SetAnim(0, TRUE, jointAnimation, materialAnimation, shapeAnimation, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
			if (jointId >= 1000 || materialId >= 1000)
				object->mStatus.mFlags |= 0x0008000000000000ULL;
		} else {
			object->SetAnim(id, FALSE, NULL, NULL, NULL, (f32)frame, rate / 100.0f, 0.0f, 10.0f);
		}
		object->UpdateAnim(1);
	}
	if (!(object->mStatus.mFlags & 0x800ULL)) {
		if (object->mJObj != NULL)
			HSD_JObjDispAll(object->mJObj, view, 7, 0);
	}
}
