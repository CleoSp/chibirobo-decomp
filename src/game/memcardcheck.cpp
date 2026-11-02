#include <dolphin/mtx/vec.h>
#include "game/savedata.h"

static inline BOOL IsValidStat(SavePreviewRecord* slot)
{
	if (slot->mStat.length != 0xC000)
		return FALSE;
	if (slot->mStat.bannerFormat != 2)
		return FALSE;
	if (slot->mStat.iconAddr != 0x40)
		return FALSE;
	if (slot->mStat.iconFormat != 2)
		return FALSE;
	if (slot->mStat.iconSpeed != 3)
		return FALSE;
	if (slot->mStat.commentAddr != 0)
		return FALSE;
	if (slot->mStat.offsetBanner != 0x40)
		return FALSE;
	if (slot->mStat.offsetIcon[0] != 0x1840)
		return FALSE;
	if (slot->mStat.offsetData != 0x2040)
		return FALSE;
	return TRUE;
}

/**
 * @note Address: 0x80086F24
 * @note Size: 0x26C
 */
extern "C" BOOL fn_80086F24(SavePreviewRecord* slot)
{
	if (!IsValidStat(slot))
		return FALSE;
	return IsValidSave(&slot->mHeader) != FALSE;
}
