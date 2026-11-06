#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/ut.h"
extern "C" {
#include <sysdolphin/mobj.h>
#include <sysdolphin/tobj.h>
}

/**
 * @note Address: 0x801905D4
 * @note Size: 0x30C
 */
#pragma push
#pragma defer_codegen off
#pragma inline_depth(4)
#pragma inline_max_size(1024)
extern "C" HSD_AObj* FindMaterialOrTextureAnimationInJointTree(HSD_JObj* jobj)
{
	HSD_AObj* aobj = NULL;
	for (; jobj != NULL; jobj = jobj->child) {
		for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
			HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
			if (mobj != NULL) {
				if ((aobj = mobj->aobj) != NULL) {
					return aobj;
				}
				HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
				if (tobj != NULL && (aobj = tobj->aobj) != NULL) {
					return aobj;
				}
			}
		}
		aobj = FindMaterialOrTextureAnimationInJointTree(jobj->next);
		if (aobj != NULL) {
			break;
		}
	}
	return aobj;
}
#pragma pop
