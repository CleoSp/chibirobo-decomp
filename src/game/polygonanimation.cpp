#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/ut.h"

/**
 * @note Address: 0x801902C4
 * @note Size: 0x2E0
 */
#pragma push
#pragma defer_codegen off
#pragma inline_depth(5)
#pragma inline_max_size(1024)
extern "C" HSD_AObj* FindPolygonAnimationInJointTree(HSD_JObj* jobj)
{
	HSD_AObj* aobj = NULL;
	for (; jobj != NULL; jobj = jobj->child) {
		for (HSD_DObj* dobj = HSD_JObjGetDObj(jobj); dobj != NULL; dobj = HSD_DObjGetNext(dobj)) {
			HSD_PObj* pobj = HSD_DObjGetPObj(dobj);
			if (pobj != NULL && (aobj = pobj->aobj) != NULL) {
				return aobj;
			}
		}
		if ((aobj = FindPolygonAnimationInJointTree(jobj->next)) != NULL) {
			break;
		}
	}
	return aobj;
}
#pragma pop
