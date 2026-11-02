#ifndef _GAME_MEMCARD_H
#define _GAME_MEMCARD_H

#include "types.h"
#include "game/savepreview.h"

#include "game/ccard.h"
// savedata.h needs a Vec header first
#include "game/savedata.h"

class Memcard;

// what a card job's thread gets
struct MemcardJob {
	Memcard* mCard; // _00
	s32 mChannel;   // _04
	s32 mFile;      // _08, the save slot
};

// the memory card side of saving, class name from RTTI. CGame keeps the one at +0xE8400
class Memcard {
public:
	s32 mState;                // _00, what the card thread is doing
	u8 _04[0x20 - 0x04];       // _04
	CCard mCard;               // _20
	u8 _A044[0xA068 - 0xA044]; // _A044
	CARDStat mStat;            // _A068
	u8 _A0D4[0xA200 - 0xA0D4]; // _A0D4
	union {
		u8 mBuffer[0xC000]; // _A200, the file, two SaveFile copies
		SaveFile mFile;     // _A200
	};
	SavePreviewRecord mSaves[2][5]; // _16200, the save headers read off each slot's card
	u32 _169F8;                     // _169F8
	u32 _169FC;                     // _169FC
	s32 _16A00;                     // _16A00, the save file's two words menus show
	s32 _16A04;                     // _16A04
	BOOL mBusy;                     // _16A08, a job's thread is running
	BOOL mFound;                    // _16A0C, the scan found a save
	MemcardJob mJob;                // _16A10
	BOOL mLog;                      // _16A1C, print card results
	// _16A20 vtable

	virtual ~Memcard() { } // _08

	// todo: cleanup, every card job inlines these
	inline void LogResult()
	{
		if (mLog)
			mCard.GetResultString(mCard.mResult);
	}
	inline BOOL Probe(s32 channel)
	{
		s32 memorySize, sectorSize;
		mState = 1;
		mCard.ProbeEx(channel, &memorySize, &sectorSize);
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY)
			return TRUE;
		if (sectorSize != 0x2000) {
			mCard.mResult = CARD_RESULT_WRONGDEVICE;
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL Check(BOOL unmount)
	{
		mState = 5;
		mCard.Check();
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			if (unmount)
				mCard.Unmount();
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL Mount(s32 channel, BOOL unmount)
	{
		mState = 2;
		mCard.Mount(channel, NULL);
		LogResult();
		switch (mCard.mResult) {
		case CARD_RESULT_READY:
		case CARD_RESULT_BROKEN:
			if (Check(unmount))
				return TRUE;
			break;
		}
		if (mCard.mResult != CARD_RESULT_READY) {
			switch (mCard.mResult) {
			case CARD_RESULT_BROKEN:
			case CARD_RESULT_ENCODING:
				if (unmount)
					mCard.Unmount();
				return TRUE;
			}
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL Delete(const char* name)
	{
		mState = 15;
		mCard.Delete(name);
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			mCard.Unmount();
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL Format()
	{
		mState = 6;
		mCard.Format();
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			mCard.Unmount();
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL OpenStatus(char* name, CARDStat* stat)
	{
		mState = 8;
		mCard.Open(name);
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY)
			return TRUE;
		mState = 10;
		mCard.GetStatus(stat);
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			mCard.Close();
			return TRUE;
		}
		return FALSE;
	}
	inline BOOL Close()
	{
		mState = 9;
		mCard.CloseEx();
		LogResult();
		if (mCard.mResult != CARD_RESULT_READY) {
			mCard.Unmount();
			return TRUE;
		}
		return FALSE;
	}
};
STATIC_ASSERT(sizeof(Memcard) == 0x16A24);

extern "C" {
// the card steps, TRUE when one failed. mCard.mResult says why
BOOL fn_80087190(Memcard* card, struct SaveFile* file);
BOOL fn_80087508(Memcard* card);
BOOL fn_80087854(Memcard* card, char* name, BOOL create);
BOOL fn_80088F20(Memcard* card, s32 channel);
BOOL fn_8008933C(Memcard* card, s32 channel);
BOOL fn_80089D3C(Memcard* card, s32 channel, s32 file);
// "CHIBIROBO(%d...", a slot's file name
extern const char lbl_802091D8[];
// the card jobs, each runs on the card thread and returns FALSE while one is still busy
BOOL fn_80088B4C(Memcard* card, s32 channel, s32 file, BOOL logResults);
BOOL fn_80088E78(Memcard* card, s32 channel, BOOL logResults);
BOOL fn_80089890(Memcard* card, s32 channel, BOOL logResults);
BOOL fn_80089C90(Memcard* card, s32 channel, s32 file, BOOL logResults);
BOOL fn_8008A1F4(Memcard* card, s32 channel, s32 file, BOOL logResults);

// the card error messages
extern const char lbl_801FF4B8[];
}

#endif
