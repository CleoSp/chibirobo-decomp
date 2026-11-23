#include "Dolphin/vec.h"
#include "game/memcard.h"

/**
 * @note Address: 0x80087508
 * @note Size: 0x34C
 */
BOOL Memcard::ReadSaveFile()
{
	mState         = 12;
	s32 offset     = 0;
	s32 copy       = 0;
	SaveData* save = &mFile.mData;
	for (; copy < 2; ++copy, offset += sizeof(SaveFile)) {
		if (ReadSaveCopy(&mFile, offset) != CARD_RESULT_READY)
			return FailSaveRead(TRUE);
		if (!mFile.mData.HasValidSignature()) {
			if (copy == 1) {
				mCard.mResult = CARD_RESULT_FATAL_ERROR;
				return FailSaveRead(TRUE);
			}
			continue;
		}
		u32 checksum = save->CalculateChecksum();
		if (checksum != mFile.mChecksum) {
			if (copy == 1) {
				mCard.mResult = CARD_RESULT_FATAL_ERROR;
				return FailSaveRead(TRUE);
			}
			continue;
		}
		if (!ValidateSaveHeader(&save->mHeader)) {
			if (copy == 1) {
				mCard.mResult = CARD_RESULT_FATAL_ERROR;
				return FailSaveRead(TRUE);
			}
			continue;
		}
		break;
	}
	return FALSE;
}
