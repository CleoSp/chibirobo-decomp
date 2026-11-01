#include "game/xchr.h"
#include <stdlib.h>
extern "C" {
#include <sysdolphin/aobj.h>
#include <sysdolphin/fobj.h>
#include "game/video.h"
}

// todo: cleanup
static inline u8* AnimationHead(HSD_JObj* joint)
{
	HSD_AObj* animation = joint->aobj;
	HSD_FObj* frame     = animation ? animation->fobj : NULL;
	return frame ? frame->ad_head : NULL;
}

// loops the four idle joints' own anims, restarting them together after a random pause
/**
 * @note Address: 0x800A9DBC
 * @note Size: 0x2B4
 */
extern "C" void fn_800A9DBC(XObjChr* object, u32 elapsed)
{
	XObjChrIdleJoint* entries = object->_9500;
	if (!entries[0].mJObj)
		return;
	if (!entries[1].mJObj)
		return;
	if (!entries[2].mJObj)
		return;
	if (!entries[3].mJObj)
		return;
	if (object->_9530 > 0) {
		if ((object->_9530 -= elapsed) < 0)
			object->_9530 = 0;
	}

	u8* current[4];
	for (s32 i = 0; i < 4; ++i)
		current[i] = AnimationHead(entries[i].mJObj);
	if (current[0] == entries[0]._08 && current[1] == entries[1]._08 && current[2] == entries[2]._08 && current[3] == entries[3]._08) {
		s32 restarted           = 0;
		XObjChrIdleJoint* entry = entries;
		for (s32 i = 0; i < 4; ++i, ++entry) {
			if (object->_9530 == 0) {
				HSD_AObj* animation = entry->mJObj->aobj;
				if (animation == NULL) {
					HSD_JObjAddAnim(entry->mJObj, entry->mAnimJoint, NULL, NULL);
					HSD_JObjReqAnim(entry->mJObj, 0.0f);
					++restarted;
					entry->_08 = entry->mJObj->aobj->fobj->ad_head;
				} else if (HSD_AObjGetFlags(animation) & 0x44000000) {
					HSD_JObjReqAnim(entry->mJObj, 0.0f);
					++restarted;
				}
			}
			object->Push(entry->mJObj, 0x200, 1.0f);
		}
		if (restarted > 0) {
#if defined(VERSION_PAL)
			s32 fps       = lbl_80660D4C;
			object->_9530 = 0.5f * fps + rand() % fps;
#else
			object->_9530 = 30.0f + rand() % 60;
#endif
		}
	} else {
		entries[0]._08 = NULL;
		entries[1]._08 = NULL;
		entries[2]._08 = NULL;
		entries[3]._08 = NULL;
	}
}
