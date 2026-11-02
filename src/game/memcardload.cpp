#include "Dolphin/mtx.h"
#include "game/ccard.h"
#include "game/memcard.h"

static inline BOOL ReadFailed(Memcard* card)
{
	card->mCard.Close();
	card->mCard.Unmount();
	return TRUE;
}

/**
 * @note Address: 0x80087508
 * @note Size: 0x34C
 */
extern "C" BOOL fn_80087508(Memcard* card)
{
	card->mState     = 12;
	SaveHeader* save = &card->mFile.mData.mHeader;
	for (s32 copy = 0; copy < 2; ++copy) {
		card->mCard.Read(card->mBuffer, 0x6000, copy * 0x6000);
		card->LogResult();
		if (card->mCard.mResult != CARD_RESULT_READY)
			return ReadFailed(card);
		if (card->mFile.mData.mHeader.mSignature != SAVE_SIGNATURE) {
			if (copy == 1) {
				card->mCard.mResult = -128;
				return ReadFailed(card);
			}
			continue;
		}
		u32 checksum = 0;
		for (u32 i = 0; i < sizeof(SaveData); ++i)
			checksum += ((u8*)save)[i];
		if (checksum != card->mFile.mChecksum) {
			if (copy == 1) {
				card->mCard.mResult = -128;
				return ReadFailed(card);
			}
			continue;
		}
		if (!IsValidSave(save)) {
			if (copy == 1) {
				card->mCard.mResult = -128;
				return ReadFailed(card);
			}
			continue;
		}
		break;
	}
	return FALSE;
}
