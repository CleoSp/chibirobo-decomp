#include "game/xobj.h"
#include "game/xobjread.h"

/**
 * @note Address: 0x80131C6C
 * @note Size: 0xC0
 */
// any read still waiting
extern "C" BOOL vf1C__8XObjReadFv(XObjRead* reader)
{
	for (s32 i = 0; i < 8; ++i) {
		if (reader->mReqs[i].mObj != NULL) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x80131D2C
 * @note Size: 0x280
 */
extern "C" bool fn_80131D2C()
{
	return vf1C__8XObjReadFv(&lbl_80380008) || vf1C__8XObjReadFv(&lbl_803803A8) || vf1C__8XObjReadFv(&lbl_80380748);
}

/**
 * @note Address: 0x80131FAC
 * @note Size: 0x94
 */
// XObjReadAnm::vf18, reads the object's anim archive
extern "C" void vf18__11XObjReadAnmFP11XObjReadReq(XObjReadAnm* reader, XObjReadReq* req)
{
	XObj* object     = (XObj*)req->mObj;
	const char* path = (const char*)req->mInfo;
	while (!object->vfA0(path)) {
		OSYieldThread();
	}
	req->mObj = NULL;
	object->mStatus.mFlags &= ~0x0080000000000000ULL;
}
