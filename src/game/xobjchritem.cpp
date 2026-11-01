#include "game/xchr.h"
#include "game/stagedata.h"

extern "C" {
extern s32 lbl_802262F4[];
extern const char* lbl_802262D0[];
void fn_800B7D54(XObjChr* chr);
}

static inline BOOL LoadItem(XObjChr* chr)
{
	if (chr->mSuit == 0)
		return TRUE;
	if (chr->mSuitLoad == 0)
		return TRUE;
	switch (chr->mSuitLoad) {
	case 1: {
		XObj* item = &chr->mSuitObj[chr->mSuit];
		s32 id     = chr->mSuit == 7 ? lbl_802262F4[chr->mSuitSpecial] : chr->mSuit + 25;
		if (!item->vf9C(CoordInfo(GetSpawnData(id))))
			break;
		HSD_JObj** joint = &chr->mSuitJoint[chr->mSuit];
		if (*joint == NULL) {
			s32 path[4] = { 0, 1, 0, 0 };
			*joint      = GetJointByPath(item->mJObj, path, 4);
		}
		chr->mSuitLoad++;
	}
	case 2: {
		const char* path = lbl_802262D0[chr->mSuit];
		BOOL ready       = path != NULL ? fn_8008A38C(&chr->mSuitAnim, path) : TRUE;
		if (ready) {
			chr->mSuitLoad = 0;
			return TRUE;
		}
		break;
	}
	}
	return FALSE;
}

/**
 * @note Address: 0x800B8EC4
 * @note Size: 0x270
 */
// loads the item he's asked to hold, true once it's ready
extern "C" BOOL fn_800B8EC4(XObjChr* chr)
{
	if (chr->mSuitLoad == 0)
		chr->mSuitLoad = 1;
	if (chr->mSuit != chr->mSuitRequest) {
		chr->mSuitChanged = TRUE;
		chr->mSuit        = chr->mSuitRequest;
	}
	if (LoadItem(chr)) {
		chr->mSuitChanged = FALSE;
		fn_800B7D54(chr);
		return TRUE;
	}
	return FALSE;
}
