#include "game/cgame.h"
#include "game/ccard.h"
#include <stdio.h>

extern "C" char lbl_80378000[];

/**
 * @note Address: 0x80088F20
 * @note Size: 0x338
 */
extern "C" BOOL fn_80088F20(Memcard* card, s32 channel)
{
	BOOL found = FALSE;
	if (card->Probe(channel))
		return TRUE;
	if (card->Mount(channel, TRUE))
		return TRUE;
	for (s32 file = 0; file < 10; ++file) {
		sprintf(lbl_80378000, lbl_802091D8, file);
		if (!card->OpenStatus(lbl_80378000, &card->mStat)) {
			card->mState = 9;
			card->mCard.CloseEx();
			card->LogResult();
			if (card->mCard.mResult == CARD_RESULT_READY) {
				found = TRUE;
				if (!(lbl_8025C400.mStatus.mFlags & 0x4000ULL))
					break;
			}
		}
	}
	card->mState = 3;
	card->mCard.UnmountEx();
	card->LogResult();
	if (card->mCard.mResult != CARD_RESULT_READY)
		return TRUE;
	card->mFound = found;
	card->mState = 0;
	return FALSE;
}
