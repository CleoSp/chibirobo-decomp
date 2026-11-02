#include "game/cgame.h"
#include "game/cthread.h"

/**
 * @note Address: 0x80089258
 * @note Size: 0x30
 */
extern "C" void* fn_80089258(void* arg)
{
	MemcardJob* job = (MemcardJob*)arg;
	fn_80088F20(job->mCard, job->mChannel);
	return NULL;
}

/**
 * @note Address: 0x80089288
 * @note Size: 0xB4
 */
extern "C" BOOL fn_80089288(Memcard* card, s32 channel, BOOL log)
{
	if (card->mBusy)
		return FALSE;
	card->mLog          = log;
	card->mFound        = FALSE;
	card->mJob.mCard    = card;
	card->mJob.mChannel = channel;
	lbl_8036AC10.Start(fn_80089258, &card->mJob, lbl_8036AF38, sizeof(lbl_8036AF38));
	card->mBusy = TRUE;
	return TRUE;
}
