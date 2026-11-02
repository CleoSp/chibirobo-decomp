#include "Dolphin/mtx.h"
#include "game/ccard.h"
#include "game/memcard.h"
#include "game/cthread.h"
#include <stdio.h>

/**
 * @note Address: 0x80088848
 * @note Size: 0x58
 */
extern "C" void fn_80088848(Memcard* card)
{
	if (card->mBusy && OSIsThreadTerminated(&lbl_8036AC10.mThread))
		card->mBusy = FALSE;
}

/**
 * @note Address: 0x800888A0
 * @note Size: 0x278
 */
extern "C" BOOL fn_800888A0(Memcard* card, s32 channel, s32 file)
{
	if (card->Probe(channel))
		return TRUE;
	if (card->Mount(channel, TRUE))
		return TRUE;
	char name[128];
	sprintf(name, lbl_802091D8, file);
	if (card->Delete(name))
		return TRUE;
	card->mState = 3;
	card->mCard.UnmountEx();
	card->LogResult();
	if (card->mCard.mResult != CARD_RESULT_READY)
		return TRUE;
	card->mState = 0;
	return FALSE;
}

/**
 * @note Address: 0x80088B18
 * @note Size: 0x34
 */
extern "C" void* fn_80088B18(void* arg)
{
	MemcardJob* job = (MemcardJob*)arg;
	fn_800888A0(job->mCard, job->mChannel, job->mFile);
	return NULL;
}

/**
 * @note Address: 0x80088B4C
 * @note Size: 0xAC
 */
extern "C" BOOL fn_80088B4C(Memcard* card, s32 channel, s32 file, BOOL log)
{
	if (card->mBusy)
		return FALSE;
	card->mLog          = log;
	card->mJob.mCard    = card;
	card->mJob.mChannel = channel;
	card->mJob.mFile    = file;
	lbl_8036AC10.Start(fn_80088B18, &card->mJob, lbl_8036AF38, sizeof(lbl_8036AF38));
	card->mBusy = TRUE;
	return TRUE;
}

/**
 * @note Address: 0x80088BF8
 * @note Size: 0x250
 */
extern "C" BOOL fn_80088BF8(Memcard* card, s32 channel)
{
	if (card->Probe(channel))
		return TRUE;
	if (card->Mount(channel, FALSE)) {
		switch (card->mCard.mResult) {
		case CARD_RESULT_BROKEN:
		case CARD_RESULT_ENCODING:
			if (card->Format())
				return TRUE;
			break;
		default:
			card->mCard.Unmount();
			return TRUE;
		}
	}
	card->mState = 3;
	card->mCard.UnmountEx();
	card->LogResult();
	if (card->mCard.mResult != CARD_RESULT_READY)
		return TRUE;
	card->mState = 0;
	return FALSE;
}

/**
 * @note Address: 0x80088E48
 * @note Size: 0x30
 */
extern "C" void* fn_80088E48(void* arg)
{
	MemcardJob* job = (MemcardJob*)arg;
	fn_80088BF8(job->mCard, job->mChannel);
	return NULL;
}

/**
 * @note Address: 0x80088E78
 * @note Size: 0xA8
 */
extern "C" BOOL fn_80088E78(Memcard* card, s32 channel, BOOL log)
{
	if (card->mBusy)
		return FALSE;
	card->mLog          = log;
	card->mJob.mCard    = card;
	card->mJob.mChannel = channel;
	lbl_8036AC10.Start(fn_80088E48, &card->mJob, lbl_8036AF38, sizeof(lbl_8036AF38));
	card->mBusy = TRUE;
	return TRUE;
}
