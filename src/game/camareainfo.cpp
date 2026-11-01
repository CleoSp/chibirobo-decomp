#include "game/ccameraaction.h"

void operator delete(void* object);
extern "C" void* __vt__11CamAreaInfo[];

// todo: cleanup, IsCameraOff/IsDisabled don't match here
static inline bool HasAreaFlag(const CamAreaInfo& info, u16 mask)
{
	return (info.GetRecord()->_2E & mask) ? true : false;
}

/**
 * @note Address: 0x80015CC8
 * @note Size: 0x64
 */
extern "C" BOOL fn_80015CC8(CCameraActionAreaExM* action, s32 index)
{
	if (HasAreaFlag(CamAreaInfo(&lbl_805A054C._24[index]), 4)) {
		return TRUE;
	}
	return FALSE;
}

// todo: weak copy
/**
 * @note Address: 0x80015D2C
 * @note Size: 0x48
 */
extern "C" CamAreaInfo* fn_80015D2C(CamAreaInfo* info, s32 flags)
{
	if (info != NULL) {
		*(void***)info = __vt__11CamAreaInfo;
		if ((s16)flags > 0) {
			::operator delete(info);
		}
	}
	return info;
}

/**
 * @note Address: 0x80015D74
 * @note Size: 0x64
 */
extern "C" BOOL fn_80015D74(CCameraActionAreaExM* action, s32 index)
{
	if (HasAreaFlag(CamAreaInfo(&lbl_805A054C._1C[index]), 0x80)) {
		return FALSE;
	}
	return TRUE;
}
