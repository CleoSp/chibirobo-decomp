#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
#include "game/ut.h"

/**
 * @note Address: 0x801920A8 in Japan, stripped elsewhere
 * @note Size: 0x40
 */
// drops a jobj's own dobjs
#pragma push
#pragma inline_depth(8)
#pragma inline_max_size(1024)
extern "C" void RemoveJointDisplayObjectsJP(HSD_JObj* jobj)
{
	if (jobj->u.dobj != NULL) {
		HSD_DObjRemoveAll(jobj->u.dobj);
		jobj->u.dobj = NULL;
	}
}

/**
 * @note Address: 0x80191E90 in Japan, stripped elsewhere
 * @note Size: 0x218
 */
// drops every dobj in a jobj tree, instances keep their children's
#pragma defer_codegen off
extern "C" void RemoveJointTreeDisplayObjects(HSD_JObj* jobj)
{
	RemoveJointDisplayObjectsJP(jobj);
	if (!(jobj->flags & INSTANCE)) {
		for (HSD_JObj* child = jobj->child; child != NULL; child = child->next) {
			RemoveJointTreeDisplayObjects(child);
		}
	}
}
#pragma pop
