#include "game/stg_shared.h"

// one of the card's file slots as the save code keeps them
struct UnkCardFile {
	s32 mUsed;           // _00
	s32 mBroken;         // _04
	u8 _08[0x2C - 0x8];  // _08
	u32 mSaveTime;       // _2C, in seconds
	u8 _30[0x7C - 0x30]; // _30
	s32 _7C;             // _7C
	s32 _80;             // _80
	u8 _84[0x8C - 0x84]; // _84
	s32 _8C;             // _8C
	s32 _90;             // _90
	s32 _94;             // _94
	f32 _98;             // _98
	f32 _9C;             // _9C
	s32 _A0;             // _A0
	s32 _A4;             // _A4
	s32 _A8;             // _A8
	u8 _AC[0xC8 - 0xAC]; // _AC
	s32 mPlayTime;       // _C8, in frames
};

// the memory card code's state, GAME_SAVE
struct UnkSaveData {
	s32 mState;                   // _00
	u8 _04[0x16200 - 0x4];        // _04
	UnkCardFile mFiles[3];        // _16200
	u8 _16464[0x16A08 - 0x16464]; // _16464
	s32 _16A08;                   // _16A08, a card job is running
};
extern "C" BOOL fn_800617DC(CMenu* menu, u32 frames);

// the out of line copies of inlines MenuPC's draw was too big to take

struct UnkPlayerData {
	u8 _00[0x4]; // _00
	s32 _04;     // _04
};

/**
 * @note Address: 0x8004AB3C
 * @note Size: 0x4
 */
extern "C" void fn_8004AB3C(MenuPC* menu)
{
}

/**
 * @note Address: 0x8004B928
 * @note Size: 0x4
 */
extern "C" void fn_8004B928(MenuPC* menu)
{
}

// a placed thing on the stage select's map, negative when the slot is free
struct UnkStagePos {
	s16 mX;     // _00
	s16 mY;     // _02
	s16 mPrice; // _04
	s16 _06;    // _06
};

struct MenuShopList {
	s32 first;
	UnkStagePos entries[20];
	s32 indices[20];
	UnkStagePos previous;
	UnkStagePos next;
};
// Visible PC inventory pages, status availability, and reward rows.
MenuShopList lbl_80366A6C;
MenuShopList lbl_80366B70;
u8 lbl_80366C74[24];
u8 lbl_80366C8C[16];
static UnkStagePos lbl_80366C9C[20];
struct MenuRewardRow {
	s32 id;
	s32 count;
	s32 value;
};
MenuRewardRow lbl_80366D3C[10];
static UnkStagePos lbl_80366DB4[20];
extern "C" void fn_8003466C(MenuShopList* list);

// how many kinds of things the player has
static inline s32 CountMenuItems()
{
	s16 num;
	s16 id;
	s32 count;
	int i;
	UnkIdPair* pairs;
	count = 0;
	for (i = 255; i >= 0; i--) {
		pairs = GAME_PLAYER_IDS->mPairs;
		id    = pairs[i].mId;
		num   = pairs[i].mValue;
		if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
			count++;
		}
	}
	return count;
}

// the os time functions, game/ has no header for them
typedef s64 OSTime;
struct OSCalendarTime {
	s32 sec;  // _00
	s32 min;  // _04
	s32 hour; // _08
	s32 mday; // _0C
	s32 mon;  // _10
	s32 year; // _14
	s32 wday; // _18
	s32 yday; // _1C
	s32 msec; // _20
	s32 usec; // _24
};
extern "C" {
void OSTicksToCalendarTime(OSTime ticks, OSCalendarTime* td);
OSTime OSCalendarTimeToTicks(OSCalendarTime* td);
}

#define OS_TIMER_CLOCK (*(u32*)0x800000F8 / 4)

// the outline offsets of the flying letters, each menu has its own copy
extern "C" f32 lbl_8021238C[4];
extern "C" f32 lbl_8021239C[4];
extern "C" f32 lbl_80211E50[4];
extern "C" f32 lbl_80211E60[4];

/**
 * @note Address: 0x8005773C
 * @note Size: 0x1F8
 */
static inline s16 MenuInventoryId(s32 index)
{
	return ((UnkIdPair*)((u8*)GAME_PLAYER_IDS + 0x9688))[index].mId;
}
static inline void MarkMenuInventory()
{
	for (int i = 255; i >= 0; i--) {
		fn_8006E44C(MenuInventoryId(i), 4);
	}
}

// empties one entry of the status table
static inline void ClearStatusEntry(UnkStatusEntry* e)
{
	e->mId            = -1;
	e->_02            = -1;
	e->mStatus.mFlags = 0;
}

static inline void ClearStatusTable(UnkStatusTable* table)
{
	table->_00 = 0;
	for (int i = 0; i < 256; i++) {
		ClearStatusEntry(&table->mEntries[i]);
	}
	for (int i = 0; i < 4; i++) {
		ClearStatusEntry(&table->_C04[i]);
		ClearStatusEntry(&table->_C34[i]);
	}
}

extern "C" f32 lbl_8065B1A0;
extern "C" void fn_80068C8C(f32*, f32*, f32);

static inline void ResetPCText(MenuPC* menu, s32 step)
{
	menu->mLastStep    = menu->mStep;
	menu->mStep        = step;
	menu->mSubStep     = 0;
	menu->mStepTime    = 0;
	menu->mTextX       = 210.0f;
	menu->mTextY       = 129.0f;
	menu->mTextTargetX = menu->mTextX;
	menu->mTextTargetY = menu->mTextY;
	switch (menu->mStep) {
	case 3:
	case 7:
	case 10:
		menu->mTextY       = 121.0f;
		menu->mTextTargetY = menu->mTextY;
		break;
	}
	menu->mTextIndex = 0;
	menu->_378       = 0;
	menu->_37C       = 0;
	char space[3]    = "\x81\x40";
	for (int i = 0; i < 40; i++)
		menu->mText[i] = *(u16*)space;
	menu->mTextEnd = 0;
	for (int i = 0; i < 40; i++) {
		menu->mGlyphX[i]           = 640.0f;
		menu->mGlyphFlags[i]       = 0;
		*(u16*)menu->mGlyphText[i] = *(u16*)space;
		menu->mGlyphText[i][2]     = 0;
	}
	menu->_364   = 0;
	menu->_368   = 0;
	menu->_36C   = 0;
	menu->_370   = 0;
	menu->_374   = 0;
	lbl_8065B1A0 = 640.0f;
	memset(lbl_803620A8, 0, 0x200);
}

extern "C" void fn_8004BBC0(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 2);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	}
}
extern "C" void fn_8004B92C(MenuPC* menu)
{
	CPad pad(0);
	if (pad.mPad->mTrigger & 0x200) {
		*(s32*)(lbl_8025C400 + 0x43D0) = 0;
		ResetPCText(menu, 2);
		fn_8001E904(GAME_SOUND, 0x86, 0xFF);
	}
}
extern "C" void fn_8004D584(MenuPC* menu, u32 frames)
{
	switch (menu->mSubStep) {
	case 0:
		fn_8001E904(GAME_SOUND, 0x30008, 0xFF);
		menu->mSubStep++;
		break;
	case 1:
		break;
	}
	fn_80068C8C(&menu->mProgress, &menu->mProgressTarget, menu->mStepSpeed * frames);
	if (menu->mProgress >= 1.0f)
		ResetPCText(menu, 2);
}

extern "C" void fn_8004D85C(MenuPC* menu)
{
	if (*((u8*)menu + 0x3A0)) {
		ResetPCText(menu, 3);
	} else {
		ResetPCText(menu, 1);
		menu->mProgress       = 0.0f;
		menu->mProgressTarget = 1.0f;
		switch (rand() % 2) {
		case 0:
			menu->mStepSpeed = 0.04f;
			break;
		case 1:
			menu->mStepSpeed = 0.08f;
			break;
		}
	}
}

extern "C" s32 lbl_80367A2C[];

static inline s16 MenuHistoryValue(int i)
{
	return lbl_80366C9C[i].mY;
}

static inline void RestorePCHistory(MenuPC* menu)
{
	if (menu->mHistoryCount > 0) {
		for (int i = 0; i < menu->mHistoryCount; i++) {
			lbl_80367A2C[menu->mHistoryStart + i + menu->mHistoryCount] = MenuHistoryValue(i);
		}
		lbl_80367A2C[196] = menu->_384;
	}
}

extern "C" void fn_8004CD38(MenuPC* menu)
{
	CPad pad(0);
	if (PAD_REPEAT(pad) & 0x10008) {
		menu->mChoice--;
		if (menu->mChoice < 0)
			menu->mChoice = menu->mChoiceCount - 1;
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	} else if (PAD_REPEAT(pad) & 0x20004) {
		menu->mChoice++;
		if (menu->mChoice > menu->mChoiceCount - 1)
			menu->mChoice = 0;
		fn_8001E904(GAME_SOUND, 0x87, 0xFF);
	} else if (pad.mPad->mTrigger & 0x100) {
		s32 step = menu->mChoices[menu->mChoice];
		if (step == 7) {
			if (menu->mCanSelect) {
				ResetPCText(menu, 7);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			} else {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			}
		} else if (step >= 0) {
			ResetPCText(menu, step);
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
	} else if (pad.mPad->mTrigger & 0x200) {
		ResetPCText(menu, 11);
		*(f32*)((u8*)menu + 0x1A4) = 0.0f;
		RestorePCHistory(menu);
	}
}

static inline s32 MenuPower10(s32 exponent)
{
	s32 result = 1;
	for (s32 i = 0; i < exponent; i++)
		result *= 10;
	return result;
}
static inline s32 MenuDigit(s32 value, s32 digit)
{
	if (digit == 0)
		return value % 10;
	return (value / MenuPower10(digit)) % 10;
}
struct MenuPlayerBalances {
	u8 _00[0x9660];
	s32 money;
	s32 scrap;
	void SetMoney(s32 value)
	{
		s32& balance = money;
		balance      = value > 0 ? value : 0;
		if (balance < 0)
			balance = 0;
		if (balance > 99999)
			balance = 99999;
	}
	void SetScrap(s32 value)
	{
		scrap        = value > 0 ? value : 0;
		s32& balance = scrap;
		if (balance < 0)
			balance = 0;
		if (balance > 99999)
			balance = 99999;
	}
};
static inline s32 MenuGetMoney()
{
	return ((MenuPlayerBalances*)GAME_PLAYER_IDS)->money;
}
static inline s32 MenuGetScrap()
{
	return ((MenuPlayerBalances*)GAME_PLAYER_IDS)->scrap;
}
static inline void SetPCBalance(s32& balance, s32 value)
{
	balance = value > 0 ? value : 0;
	if (balance < 0)
		balance = 0;
	if (balance > 99999)
		balance = 99999;
}
static inline void MenuSetMoney(s32 value)
{
	SetPCBalance(((MenuPlayerBalances*)GAME_PLAYER_IDS)->money, value);
}

static inline void DepositPCAmount(MenuPC* menu)
{
	s32 deposit = menu->mAmount;
	s32 money   = MenuGetMoney();
	money -= deposit;
	menu->_384 += deposit;
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetMoney(money);
	*(f32*)((u8*)menu + 0x410) = money;
}

static inline void RecyclePCAmount(MenuPC* menu)
{
	s32 scrap = MenuGetScrap() - menu->mAmount;
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetScrap(scrap);
	s32 money = MenuGetMoney() + menu->mAmount * *(f32*)((u8*)menu + 0x42C);
	((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetMoney(money);
	*(f32*)((u8*)menu + 0x418) = scrap;
	*(f32*)((u8*)menu + 0x410) = money;
}
// NONMATCHING: transaction balance registers differ.
extern "C" void fn_8004A188(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mSubStep) {
	case 0:
		if (PAD_REPEAT(pad) & 0x80002) {
			menu->mMoneyDigit--;
			if (menu->mMoneyDigit < 0)
				menu->mMoneyDigit = 4;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x40001) {
			menu->mMoneyDigit++;
			if (menu->mMoneyDigit > 4)
				menu->mMoneyDigit = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			s32 value = 0;
			for (s32 i = 0; i < 7; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mMoneyDigit]--;
			if (digits[menu->mMoneyDigit] < 0)
				digits[menu->mMoneyDigit] = 9;
			for (s32 i = 0; i < 7; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x10008) {
			s32 value = 0;
			for (s32 i = 0; i < 7; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mMoneyDigit]++;
			if (digits[menu->mMoneyDigit] > 9)
				digits[menu->mMoneyDigit] = 0;
			for (s32 i = 0; i < 7; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > MenuGetMoney()) {
				menu->mAmount = MenuGetMoney();
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				return;
			}
			menu->mSubStep = 1;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	case 1:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				DepositPCAmount(menu);
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mSubStep = 2;
				break;
			}
			case 1:
				menu->mSubStep = 0;
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	}
}

static inline void MenuSetScrap(s32 value)
{
	MenuPlayerBalances* player = (MenuPlayerBalances*)GAME_PLAYER_IDS;
	s32& balance               = player->scrap;
	balance                    = value > 0 ? value : 0;
	if (balance < 0)
		balance = 0;
	if (balance > 99999)
		balance = 99999;
}
// NONMATCHING: transaction balance registers differ.
extern "C" void fn_80049508(MenuPC* menu)
{
	CPad pad(0);
	s32 digits[7] = { 0, 0, 0, 0, 0, 0, 0 };
	switch (menu->mSubStep) {
	case 0:
		menu->mYesNo = -1;
		if (PAD_REPEAT(pad) & 0x80002) {
			menu->mScrapDigit--;
			if (menu->mScrapDigit < 0)
				menu->mScrapDigit = 4;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x40001) {
			menu->mScrapDigit++;
			if (menu->mScrapDigit > 4)
				menu->mScrapDigit = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			s32 value = 0;
			for (s32 i = 0; i < 5; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mScrapDigit]--;
			if (digits[menu->mScrapDigit] < 0)
				digits[menu->mScrapDigit] = 9;
			for (s32 i = 0; i < 5; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x10008) {
			s32 value = 0;
			for (s32 i = 0; i < 5; i++) {
				if (i == 0)
					digits[i] = menu->mAmount % 10;
				else {
					s32 power = 1;
					for (s32 j = 0; j < i; j++)
						power *= 10;
					digits[i] = (menu->mAmount / power) % 10;
				}
			}
			digits[menu->mScrapDigit]++;
			if (digits[menu->mScrapDigit] > 9)
				digits[menu->mScrapDigit] = 0;
			for (s32 i = 0; i < 5; i++) {
				s32 power = 1;
				if (i > 0) {
					for (s32 j = 0; j < i; j++)
						power *= 10;
				}
				value += power * digits[i];
			}
			menu->mAmount = value;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mAmount > MenuGetScrap()) {
				menu->mAmount = MenuGetScrap();
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
				return;
			}
			if (menu->mAmount == 0) {
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
				return;
			}
			menu->mSubStep = 1;
			menu->mYesNo   = 0;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
		}
		break;
	case 1:
		if (PAD_REPEAT(pad) & 0x10008) {
			if (menu->mYesNo > 0) {
				menu->mYesNo--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x20004) {
			if (menu->mYesNo < 1) {
				menu->mYesNo++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mYesNo) {
			case 0: {
				RecyclePCAmount(menu);
				menu->mYesNo = 2;
				fn_8001E904(GAME_SOUND, 0x30009, 0xFF);
				menu->mSubStep = 2;
				break;
			}
			case 1:
				menu->mSubStep = 0;
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				break;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			menu->mSubStep = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep = 0;
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
		}
		break;
	}
}
static inline Vec MenuObjectVector(XObj* obj, int first)
{
	s32* params = (s32*)obj->_238;
	Vec value   = lbl_80362048[0];
	value.x     = params[first] / 100.0f;
	value.y     = params[first + 1] / 100.0f;
	value.z     = params[first + 2] / 100.0f;
	return value;
}

extern "C" void fn_8003466C(MenuShopList*);

static inline s32 CountPCShopItems(UnkStagePos* table)
{
	s16 x;
	s16 y;
	int i;
	s32 count;
	count = 0;
	for (i = 0; i < 20; i++) {
		x = table[i].mX;
		y = table[i].mY;
		if (x >= 0 && y >= 0)
			count++;
	}
	return count;
}
static inline s32 PCShopMaxPage(UnkStagePos*& table)
{
	s16 x, y;
	int i;
	s32 pages = 0;
	for (i = 0; i < 20; table++, i++) {
		x = table->mX;
		y = table->mY;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
static inline s32 ResetPCShopPage(MenuPC* menu, UnkStagePos*& table)
{
	s16 x, y;
	int i;
	s32 pages      = 0;
	i              = 0;
	menu->mSubStep = 0;
	for (; i < 20; table++, i++) {
		x = table->mX;
		y = table->mY;
		if (x >= 0 && y >= 0)
			pages++;
	}
	pages -= 8;
	if (pages < 1)
		pages = 1;
	return pages;
}
#define PC_FLOAT(menu, off) (*(f32*)((u8*)(menu) + (off)))
#define PC_INT(menu, off)   (*(s32*)((u8*)(menu) + (off)))
static inline s16 PCShopEntryId(s32 index)
{
	return lbl_80366B70.entries[index].mX;
}
extern "C" void fn_8004AB40(MenuPC* menu, u32 frames)
{
	s32 count;
	CPad pad(0);
	UnkStagePos* table = lbl_80366DB4;
	count              = CountPCShopItems(table);
	fn_80068C8C(&menu->mPanelAlpha, &menu->mPanelTarget, 0.08f * frames);
	switch (menu->mSubStep) {
	case 0:
		if (count > 1) {
			if (menu->mShopSelection == 0) {
				if (PAD_REPEAT(pad) & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						fn_8001E904(GAME_SOUND, 0x87, 0xFF);
					}
				}
				if (PAD_REPEAT(pad) & 0x10008) {
					CountPCShopItems(table);
					if (lbl_80366B70.first > 0) {
						menu->mScrollTarget = 17.0f;
						menu->mSubStep      = 20;
						break;
					}
				}
			} else if (menu->mShopSelection == 8) {
				if (PAD_REPEAT(pad) & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(GAME_SOUND, 0x87, 0xFF);
				}
				if (PAD_REPEAT(pad) & 0x20004) {
					if (lbl_80366B70.first < PCShopMaxPage(table) - 1) {
						menu->mScrollTarget = -17.0f;
						menu->mSubStep      = 10;
						break;
					}
				}
			} else {
				if (PAD_REPEAT(pad) & 0x10008) {
					if (--menu->mShopSelection < 0)
						menu->mShopSelection = 19;
					fn_8001E904(GAME_SOUND, 0x87, 0xFF);
				}
				if (PAD_REPEAT(pad) & 0x20004) {
					s32 next = menu->mShopSelection + 1;
					if (next <= 19) {
						menu->mShopSelection = next;
						s16 id               = lbl_80366B70.entries[menu->mShopSelection].mX;
						if (id >= 0)
							fn_8001E904(GAME_SOUND, 0x87, 0xFF);
					}
				}
			}
		}
		if (PCShopEntryId(menu->mShopSelection) < 0) {
			do {
				menu->mShopSelection--;
				s16 id = lbl_80366B70.entries[menu->mShopSelection].mX;
				if (id >= 0)
					break;
			} while (menu->mShopSelection != 0);
		}
		if (pad.mPad->mTrigger & 0x100) {
			if (PCShopEntryId(menu->mShopSelection) >= 0 && lbl_80366B70.entries[menu->mShopSelection].mY > 0) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mPanelTarget = 1.0f;
				menu->mSubStep++;
				menu->mYesNo = 0;
				break;
			}
			fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			break;
		}
		if (pad.mPad->mTrigger & 0x200) {
			ResetPCText(menu, 2);
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 1:
		if (PAD_REPEAT(pad) & 0x10008) {
			menu->mYesNo--;
			if (menu->mYesNo < 0)
				menu->mYesNo = 1;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x20004) {
			menu->mYesNo++;
			if (menu->mYesNo > 1)
				menu->mYesNo = 0;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (pad.mPad->mTrigger & 0x100) {
			if (menu->mYesNo == 0) {
				UnkStagePos* entry;
				s32 item  = lbl_80366B70.indices[menu->mShopSelection];
				s32 scrap = MenuGetScrap();
				entry     = &lbl_80366DB4[item];
				s16 price = entry->mPrice;
				if (price <= scrap) {
					scrap -= price;
					((MenuPlayerBalances*)GAME_PLAYER_IDS)->SetScrap(scrap);
					menu->mDisplayScrap = scrap;
					menu->mYesNo        = 2;
					fn_8001E904(GAME_SOUND, 0x30009, 0xFF);
					lbl_80367A2C[menu->mCatalogBase + entry->mX] = 1;
					menu->_430                                   = item;
					s16& quantity                                = entry->mY;
					s16 value                                    = quantity;
					s32 remaining                                = value;
					if (value > 0)
						remaining = value - 1;
					quantity = remaining;
					menu->mSubStep++;
					return;
				}
				fn_8001E904(GAME_SOUND, 0x85, 0xFF);
			} else {
				menu->mSubStep     = 0;
				menu->mPanelTarget = 0.0f;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_39C = 1.0f;
			}
		} else if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mSubStep     = 0;
			menu->mPanelTarget = 0.0f;
		}
		break;
	case 2:
		if (pad.mPad->mTrigger & 0x300) {
			menu->mSubStep     = 0;
			menu->mPanelTarget = 0.0f;
			fn_8001E904(GAME_SOUND, 0x88, 0xFF);
			fn_8003466C(&lbl_80366B70);
			ResetPCText(menu, 11);
			PC_FLOAT(menu, 0x1A4) = 0.0f;
			RestorePCHistory(menu);
		}
		break;
	case 10:
		if (menu->mScroll == menu->mScrollTarget) {
			if (lbl_80366B70.first < ResetPCShopPage(menu, table) - 1) {
				lbl_80366B70.first++;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
			menu->mScroll       = 0.0f;
			menu->mScrollTarget = 0.0f;
		}
		break;
	case 20:
		if (menu->mScroll == menu->mScrollTarget) {
			menu->mSubStep = 0;
			CountPCShopItems(table);
			if (lbl_80366B70.first > 0) {
				lbl_80366B70.first--;
				fn_8003466C(&lbl_80366B70);
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
			menu->mScroll       = 0.0f;
			menu->mScrollTarget = 0.0f;
		}
		break;
	}
}

static inline void PCSlide(f32& value, f32& target, f32 speed)
{
	if (value > target) {
		if ((value -= speed) < target)
			value = target;
	}
	if (value < target) {
		if ((value += speed) > target)
			value = target;
	}
}
static inline void PCEase(f32& value, f32& target)
{
	f32 speed = target - value;
	f32 rate  = 0.25f;
	speed     = speed * rate;
	if (speed < 0.0f)
		speed = -speed;
	if (speed < 1.0f)
		speed = 1.0f;
	PCSlide(value, target, speed);
}
static inline void PCEaseCall(f32& value, f32& target)
{
	f32 speed = target - value;
	f32 rate  = 0.25f;
	rate      = speed * rate;
	if (rate < 0.0f)
		rate = -rate;
	if (rate <= 1.0f)
		rate = 1.0f;
	fn_80068C8C(&value, &target, rate);
}
static inline XObj* PCWorldObject(s32 index)
{
	return (XObj*)(*(u8**)(lbl_8025C400 + 0x984C) + index * (*(s32*)(lbl_8025C400 + 0x985C)));
}
static inline s32 PCWorldObjectId(XObj* obj)
{
	s32* ident = (s32*)obj->_244;
	return ident ? *ident : -1;
}
static inline f32 PCCheckedEnd(HSD_AObj* aobj)
{
	if (!aobj)
		__assert("aobj.h", 0xAB, "aobj");
	return *(f32*)((u8*)aobj + 0xC);
}
static inline f32 PCCheckedFrame(HSD_AObj* aobj)
{
	if (!aobj)
		__assert("aobj.h", 0x95, "aobj");
	return *(f32*)((u8*)aobj + 4);
}
static inline f32 PCAnimEnd(XObj* obj)
{
	HSD_AObj* a = GetAObj(obj->mJObj);
	return a ? PCCheckedEnd(a) : 0.0f;
}
static inline f32 PCAnimFrame(XObj* obj)
{
	HSD_AObj* a = GetAObj(obj->mJObj);
	return a ? PCCheckedFrame(a) : 0.0f;
}
typedef void (MenuPC::*PCStepHandler)(u32);
extern "C" PCStepHandler lbl_80212120[];
extern "C" BOOL fn_8004DD1C(MenuPC* menu, u32 frames)
{
	if (fn_800617DC(menu, frames) == 4) {
		PCEase(PC_FLOAT(menu, 0x198), PC_FLOAT(menu, 0x1A0));
		PCEase(menu->_19C, PC_FLOAT(menu, 0x1A4));
		PCSlide(menu->mTextX, menu->mTextTargetX, 6.0f * frames);
		PCSlide(menu->mTextY, menu->mTextTargetY, 6.0f * frames);
		fn_80068C8C(&menu->mScroll, &menu->mScrollTarget, 17.0f);
		PCEaseCall(menu->_40C, menu->mDisplayMoney);
		PCEaseCall(menu->_414, menu->mDisplayScrap);
		PCEaseCall(menu->_424, menu->_428);
		PC_FLOAT(menu, 0x44C) -= 0.008f * frames;
		if (menu->mStep > 1) {
			switch (menu->mStepTime) {
			case 0:
				switch (menu->mStep) {
				case 3:
				case 7:
				case 10:
					menu->mTextTargetX = 140.0f;
					break;
				case 4:
					menu->mTextTargetX = 160.0f;
					break;
				default:
					menu->mTextTargetX = 180.0f;
					break;
				}
				menu->mStepTime++;
				break;
			case 1:
				if (menu->mTextX == menu->mTextTargetX) {
					menu->mTextY       = 129.0f;
					menu->mTextTargetY = 301.0f;
					switch (menu->mStep) {
					case 3:
					case 7:
					case 10:
						menu->mTextY -= 8.0f;
						menu->mTextTargetY -= 8.0f;
						break;
					}
					menu->mStepTime++;
				}
				break;
			case 2:
				if (menu->mTextY == menu->mTextTargetY) {
					switch (menu->mStep) {
					case 3:
					case 7:
					case 10:
						menu->mTextX       = 140.0f;
						menu->mTextTargetX = 469.0f;
						break;
					case 4:
						menu->mTextX       = 160.0f;
						menu->mTextTargetX = 449.0f;
						break;
					default:
						menu->mTextX       = 210.0f;
						menu->mTextTargetX = 429.0f;
						break;
					}
					menu->mStepTime++;
				}
				break;
			case 3:
				if (menu->mTextX == menu->mTextTargetX)
					menu->mStepTime++;
				break;
			case 4:
				break;
			}
		}
		switch (menu->_36C) {
		case 0:
			menu->_370 = 0;
			break;
		case 1:
			menu->_370 = 1;
			break;
		case 2:
			menu->_370 = 2;
			break;
		case 3:
			menu->_370 = 6;
			break;
		case 4:
			menu->_370 = 7;
			break;
		case 5:
			menu->_370 = 8;
			break;
		case 6:
			menu->_370 = 9;
			break;
		case 7:
			menu->_370 = 2;
			break;
		case 8:
			menu->_370 = 10;
			break;
		default:
			menu->_370 = 0;
			break;
		}
		if (menu->_374 == 1) {
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = PCWorldObject(i);
				switch (PCWorldObjectId(obj)) {
				case 120:
					obj->SetAnim(menu->_370, 0, 0, 0, 0, 0.0f, frames, 1.0f, 10.0f);
					break;
				}
			}
			menu->_374 = 0;
		}
		switch (menu->_36C) {
		case 1:
		case 2:
		case 6:
		case 8:
			for (s32 i = 0; i < 288; i++) {
				XObj* obj = PCWorldObject(i);
				switch (PCWorldObjectId(obj)) {
				case 120: {
					f32 end     = PCAnimEnd(obj);
					f32 current = PCAnimFrame(obj);
					if (end - 2.0f <= current) {
						menu->_36C++;
						menu->_374 = 1;
					}
				} break;
				}
			}
			break;
		}
		(menu->*lbl_80212120[menu->mStep])(frames);
	}
	return menu->IsOpen();
}

typedef void (MenuSave::*SaveStepHandler)(u32);
extern "C" SaveStepHandler lbl_802122B4[];

static inline f32 MenuTextCenter(f32 w, f32 factor, f32 center)
{
	return center - w * factor;
}
static inline void DrawMenuHelpLine(s32 id, f32 y, f32 size, u32 color)
{
	char* text = fn_8006DBF0(id);
	f32 width  = fn_801968BC(lbl_8025C400, text, 1, size);
	fn_80196610(lbl_8025C400, color, text, MenuTextCenter(width, 0.5f, 320.0f), y, 0.0f, size, size);
}
