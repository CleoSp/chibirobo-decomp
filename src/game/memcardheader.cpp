#include "Dolphin/vec.h"
#include "game/memcard.h"

/**
 * @note Address: 0x80087190
 * @note Size: 0x378
 */
BOOL Memcard::ReadSavePreview(SaveFile* file)
{
	mState = 12;
	for (s32 copy = 0; copy < 2; ++copy) {
		if (ReadSaveCopy(file, copy * (s32)sizeof(SaveFile)) != CARD_RESULT_READY)
			return FailSaveRead(FALSE);
		SaveData* save = &file->mData;
		if (!save->HasValidSignature())
			mCard.mResult = CARD_RESULT_FATAL_ERROR;
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			if (copy == 1)
				return FailSaveRead(FALSE);
			continue;
		}
		u32 checksum = save->CalculateChecksum();
		if (checksum != file->mChecksum)
			mCard.mResult = CARD_RESULT_FATAL_ERROR;
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			if (copy == 1)
				return FailSaveRead(FALSE);
			continue;
		}
		if (!ValidateSaveHeader(&save->mHeader)) {
			if (copy == 1) {
				mCard.mResult = CARD_RESULT_FATAL_ERROR;
				return FailSaveRead(FALSE);
			}
			continue;
		}
		break;
	}
	return FALSE;
}
