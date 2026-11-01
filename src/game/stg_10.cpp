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

// the out of line copies of inlines MenuPC's draw was too big to take

struct UnkPlayerData {
	u8 _00[0x4]; // _00
	s32 _04;     // _04
};

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
extern "C" void fn_8003466C(MenuShopList* list);
extern "C" void fn_80034EAC(UnkStatusTable* table);

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

/**
 * @note Address: 0x80058C7C
 * @note Size: 0x6AC
 */
// throwing things away: pick the thing, then how many, then confirm
// NONMATCHING: the item counts and the drop call arguments take other registers
extern "C" void fn_80058C7C(MenuStatus* menu)
{
	CPad pad(0);
	s32 count = CountMenuItems();
	switch (menu->mStepTime) {
	case 0:
		if (count > 1) {
			if (PAD_REPEAT(pad) & 0x40001) {
				if (--menu->_520 < 0) {
					menu->_520 = count - 1;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			} else if (PAD_REPEAT(pad) & 0x80002) {
				if (++menu->_520 > count - 1) {
					menu->_520 = 0;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			menu->mLastStep = menu->mStep;
			menu->mStep     = 4;
			menu->mNextStep = 2;
			menu->mStepTime = 0;
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			for (int i = 0; i < 4; i++) {
				menu->mTabs2[i]._00 = 0.0f;
				if (menu->_674) {
					menu->mTabs2[i].mX = 198.0f + i * 108 - 8.0f;
				} else {
					menu->mTabs2[i].mX = 252.0f + i * 108;
				}
			}
			menu->_5E0 = 758.0f;
		} else if ((pad.mPad->mTrigger & 0x100) && lbl_80365E08.mEntries[menu->_520]._02 >= 1) {
			menu->_570 = 336.0f;
			menu->_580 = 336.0f;
			menu->_534 = 1;
			menu->mStepTime++;
		}
		break;
	case 1: {
		s32 num = lbl_80365E08.mEntries[menu->_520]._02;
		if (num >= 1) {
			if (PAD_REPEAT(pad) & 0x40001) {
				if (--menu->_534 < 1) {
					menu->_534 = num;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			} else if (PAD_REPEAT(pad) & 0x80002) {
				if (++menu->_534 > num) {
					menu->_534 = 1;
				}
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
			if (pad.mPad->mTrigger & 0x100) {
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->mStepTime++;
			} else if (pad.mPad->mTrigger & 0x200) {
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				menu->_570      = 725.0f;
				menu->_580      = 937.0f;
				menu->mStepTime = 0;
			}
		}
		break;
	}
	case 2:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (menu->_530 > 0) {
				menu->_530--;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->_530 < 1) {
				menu->_530++;
				fn_8001E904(GAME_SOUND, 0x87, 0xFF);
			}
		}
		if (pad.mPad->mTrigger & 0x200) {
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mStepTime = 1;
		} else if (pad.mPad->mTrigger & 0x100) {
			switch (menu->_530 == 0) {
			default:
				UnkStatusEntry* entry = &lbl_80365E08.mEntries[menu->_520];
				fn_800DC754(GAME_PLAYER_IDS->mPairs, entry->mId, menu->_534);
				menu->_570      = 725.0f;
				menu->_580      = 937.0f;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				menu->_520 = 0;
				fn_80034EAC(&lbl_80365E08);
				menu->_5E0 = 360.0f - (CountMenuItems() - 1) * 54;
				if (CountMenuItems() == 0) {
					menu->_5E0 = 360.0f;
				}
				break;
			case false:
				fn_8001E904(GAME_SOUND, 0x86, 0xFF);
				menu->mStepTime = 1;
				break;
			}
		}
		break;
	}
}

/**
 * @note Address: 0x8005A290
 * @note Size: 0x110C
 */
// the status menu's tabs: pick one and open its page, or close the menu
// NONMATCHING: the item counts take other saved registers
extern "C" void fn_8005A290(MenuStatus* menu, u32 frames)
{
	CPad pad(0);
	switch (menu->mStepTime) {
	case 0:
		if (PAD_REPEAT(pad) & 0x40001) {
			if (--menu->mSelect < 0) {
				if (menu->_674) {
					menu->mSelect = 3;
				} else {
					menu->mSelect = 2;
				}
			}
			char* msg  = fn_8006DBF0((menu->mSelect >= 4 ? 0 : sStatusTabMsg[menu->mSelect]) + 0x17);
			menu->_684 = 1;
			menu->_690 = msg;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		} else if (PAD_REPEAT(pad) & 0x80002) {
			if (menu->_674) {
				if (++menu->mSelect > 3) {
					menu->mSelect = 0;
				}
			} else if (++menu->mSelect > 2) {
				menu->mSelect = 0;
			}
			char* msg  = fn_8006DBF0((menu->mSelect >= 4 ? 0 : sStatusTabMsg[menu->mSelect]) + 0x17);
			menu->_684 = 1;
			menu->_690 = msg;
			fn_8001E904(GAME_SOUND, 0x87, 0xFF);
		}
		if (pad.mPad->mTrigger & 0x1200) {
			menu->_554 = -200.0f;
			for (int i = 0; i < 4; i++) {
				menu->mTabs[i].mX = -102.0f;
			}
			fn_8001E904(GAME_SOUND, 0x86, 0xFF);
			menu->mStepTime++;
			menu->mWait = frames * 4;
			for (int i = 0; i < 20; i++) {
				menu->mGlyphs.mGlyphs[i].mY = 0.0f;
			}
		} else if (pad.mPad->mTrigger & 0x100) {
			switch (menu->mSelect) {
			case 0: {
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 2;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				for (int i = 0; i < 4; i++) {
					menu->mTabs2[i]._00 = 758.0f;
					if (menu->_674) {
						menu->mTabs2[i].mX = 198.0f + i * 108 - 8.0f;
					} else {
						menu->mTabs2[i].mX = 252.0f + i * 108;
					}
				}
				s32 count = CountMenuItems();
				for (int i = 0; i < 4; i++) {
					menu->mTabs2[i].mX = 0.0f;
				}
				menu->_5E0 = 360.0f - (count - 1) * 51;
				if (count == 0) {
					menu->_5E0 = 360.0f;
				}
				menu->_5E0 = 207.0f;
				u32 color  = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x18), color);
				s16 id     = lbl_80365E08.mEntries[menu->_520].mId;
				menu->_524 = menu->_520;
				if (id >= 0 && CountMenuItems() > 0) {
					char* msg  = fn_8006E510(id);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					menu->_684 = 1;
					menu->_690 = NULL;
				}
				break;
			}
			case 1: {
				menu->_5F0      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 9;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x1A), color);
				if (lbl_80366C8C[menu->_528]) {
					char* msg  = fn_8006DC8C(menu->_528);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					char* msg  = fn_8006DBF0(0xE);
					menu->_684 = 1;
					menu->_690 = msg;
				}
				break;
			}
			case 2: {
				menu->_610      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 5;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x19), color);
				menu->_684 = 1;
				menu->_690 = NULL;
				break;
			}
			case 3: {
				menu->_600      = 360.0f;
				menu->mLastStep = menu->mStep;
				menu->mStep     = 4;
				menu->mNextStep = 8;
				menu->mStepTime = 0;
				fn_8001E904(GAME_SOUND, 0x88, 0xFF);
				for (int i = 0; i < 4; i++) {
					menu->mTabs[i].mX = -102.0f;
				}
				u32 color = (u32)(255.0f * menu->mAlpha) | 0xFFFFFF00;
				menu->mGlyphs.Set(fn_8006DBF0(0x1B), color);
				if (lbl_80366C74[menu->_52C]) {
					char* msg  = fn_8006DDB8(menu->_52C);
					menu->_684 = 1;
					menu->_690 = msg;
				} else {
					char* msg  = fn_8006DBF0(0xE);
					menu->_684 = 1;
					menu->_690 = msg;
				}
				break;
			}
			default:
				fn_8001E904(GAME_SOUND, 0x147, 0xFF);
				break;
			}
		}
		break;
	case 2:
		break;
	case 1:
		if (--menu->mWait < 0) {
			GAME_FLAGS &= ~2ULL;
			menu->Close();
			sMenuMain.StopMainFade();
			fn_8001E660(GAME_SOUND, 0xFFFF0004);
			if (menu->_664) {
				fn_8001E904(GAME_SOUND, menu->_664, menu->_668);
			}
			menu->mStepTime++;
		}
		break;
	}
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
