#include "Dolphin/mtx.h"
#include "game/ccard.h"
#include "game/memcard.h"
#include "game/cthread.h"
#include <stdio.h>

/**
 * @note Address: 0x80089860
 * @note Size: 0x30
 */
extern "C" void* fn_80089860(void* arg)
{
	MemcardJob* job = (MemcardJob*)arg;
	fn_8008933C(job->mCard, job->mChannel);
	return NULL;
}

/**
 * @note Address: 0x80089890
 * @note Size: 0xEC
 */
extern "C" BOOL fn_80089890(Memcard* card, s32 channel, BOOL log)
{
	if (card->mBusy)
		return FALSE;
	card->mLog = log;
	for (u32 i = 0; i < 5; ++i)
		card->mSaves[0][i].mState = 0;
	for (u32 i = 0; i < 5; ++i)
		card->mSaves[1][i].mState = 0;
	card->mJob.mCard    = card;
	card->mJob.mChannel = channel;
	lbl_8036AC10.Start(fn_80089860, &card->mJob, lbl_8036AF38, sizeof(lbl_8036AF38));
	card->mBusy = TRUE;
	return TRUE;
}

/**
 * @note Address: 0x8008997C
 * @note Size: 0x2E0
 */
extern "C" BOOL fn_8008997C(Memcard* card, s32 channel, s32 file)
{
	if (card->Probe(channel))
		return TRUE;
	if (card->Mount(channel, TRUE))
		return TRUE;
	char name[128];
	sprintf(name, lbl_802091D8, file);
	if (fn_80087854(card, name, FALSE))
		return TRUE;
	if (fn_80087508(card))
		return TRUE;
	if (card->Close())
		return TRUE;
	card->mState = 3;
	card->mCard.UnmountEx();
	card->LogResult();
	if (card->mCard.mResult != CARD_RESULT_READY)
		return TRUE;
	card->mState = 0;
	// retail converts the save time and drops it
	OSCalendarTime calendar;
	OSTicksToCalendarTime(OSSecondsToTicks((OSTime)card->mStat.time), &calendar);
	return FALSE;
}

/**
 * @note Address: 0x80089C5C
 * @note Size: 0x34
 */
extern "C" void* fn_80089C5C(void* arg)
{
	MemcardJob* job = (MemcardJob*)arg;
	fn_8008997C(job->mCard, job->mChannel, job->mFile);
	return NULL;
}

/**
 * @note Address: 0x80089C90
 * @note Size: 0xAC
 */
extern "C" BOOL fn_80089C90(Memcard* card, s32 channel, s32 file, BOOL log)
{
	if (card->mBusy)
		return FALSE;
	card->mLog          = log;
	card->mJob.mCard    = card;
	card->mJob.mChannel = channel;
	card->mJob.mFile    = file;
	lbl_8036AC10.Start(fn_80089C5C, &card->mJob, lbl_8036AF38, sizeof(lbl_8036AF38));
	card->mBusy = TRUE;
	return TRUE;
}
