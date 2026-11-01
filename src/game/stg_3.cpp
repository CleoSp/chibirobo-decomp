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
extern MenuShopList lbl_80366B70;
static UnkStagePos lbl_80366C9C[20];
struct MenuRewardRow {
	s32 id;
	s32 count;
	s32 value;
};
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

// the letters of a line flying in, each with a blob behind it and a dark outline
/**
 * @note Address: 0x800373D4
 * @note Size: 0x5A4
 */
// NONMATCHING: the float temporaries take other saved registers
extern "C" void fn_800373D4(MenuSave* menu)
{
	if (menu->mGlyphs.mActive) {
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B464(GS_RENDER, 1, 7, 0);
		fn_8019AFA0(GS_TEV, 0);
		CTexObj blob = GAME_TEX->Init(&GAME_TPL->descriptorArray[108], 0);
		u32 alpha;
		f32 t;
		f32 s0;
		f32 bob;
		f32 rot;
		f32 s1;
		f32 s2;
		f32 scale;
		f32 x;
		f32 y;
		f32 pad;
		u32 color = menu->mGlyphs.mColor & 0xFFFFFF00;
		f32 zero  = 0.0f;
		for (int i = 0; i < menu->mGlyphs.mCount; i++) {
			MenuGlyph* glyph = &menu->mGlyphs.mGlyphs[i];
			t                = glyph->mX;
			if (!(t <= 0.0f)) {
				CFont font;
				s32 width;
				CTexObj tex = *fn_8019E8F4(&font, glyph->mChar, &width);
				s0          = sin(glyph->mRotX);
				bob         = 4.0f * s0;
				rot         = sin(0.7f * glyph->mRotY);
				s1          = sin(3.1415927f * t);
				s2          = sin(1.5707964f * t);
				scale       = 0.2f * s1 + s2;
				x           = 24.0f + (48.0f * i + (320.0f - 48.0f * menu->mGlyphs.mCount * 0.5f));
				alpha       = 255.0f * t;
				rot         = -(0.017453292f * (10.0f * rot));
				y           = 64.0f + bob;
				u32 c       = alpha | 0xFFFFFF00;
				fn_80197800(lbl_8025C400, &blob, c, c, c, c, x, y, 0.0f, 20.0f * (2.0f * scale), 20.0f * (2.0f * scale), 0.0f, -zero, rot);
				scale = 20.0f * (1.7f * scale);
				pad   = (width / 24.0f) * scale;
				pad   = scale - pad;
				for (int j = 0; j < 4; j++) {
					fn_80197800(lbl_8025C400, &tex, alpha, alpha, alpha, alpha, pad / 2 + (x + lbl_8021238C[j]),
					            bob + (64.0f + lbl_8021239C[j]), 0.0f, scale, scale, 0.0f, -zero, rot);
				}
				c = color | alpha;
				fn_80197800(lbl_8025C400, &tex, c, c, c, c, pad / 2 + x, y, 0.0f, scale, scale, 0.0f, -zero, rot);
			}
		}
		fn_8019AFA0(GS_TEV, 4);
		fn_8019B464(GS_RENDER, 0, 7, 0);
		fn_80199AB0(lbl_8025C400, 0, 0);
	}
}

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
extern "C" s32 fn_800351F4(void*, s32 id, f32 x, f32 y, f32 size, f32 alpha)
{
	s32 lines = 0;
	u32 color = (u32)(255.0f * alpha);
	switch (id) {
	case 1:
		DrawMenuHelpLine(0xad, y, size, color);
		DrawMenuHelpLine(0xae, (y + size), size, color);
		lines = 2;
		break;
	case 2:
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0xdf, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0xdf, y, size, color);
			DrawMenuHelpLine(0xe0, (y + size), size, color);
			lines = 2;
		}
		break;
	case 3:
		DrawMenuHelpLine(0xb7, y, size, color);
		lines = 1;
		break;
	case 8:
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0x85, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0x85, y, size, color);
			DrawMenuHelpLine(0x86, (y + size), size, color);
			lines = 2;
		}
		break;
	case 0xe:
		DrawMenuHelpLine(0xee, y, size, color);
		lines = 1;
		break;
	case 4:
		DrawMenuHelpLine(0xb2, y, size, color);
		DrawMenuHelpLine(0xb3, (y + size), size, color);
		DrawMenuHelpLine(0xb4, (2.0f * size + y), size, color);
		lines = 3;
		break;
	case 5:
		DrawMenuHelpLine(0x76, y, size, color);
		DrawMenuHelpLine(0x77, (y + size), size, color);
		DrawMenuHelpLine(0x78, (2.0f * size + y), size, color);
		lines = 4;
		break;
	case 6:
		DrawMenuHelpLine(0x7b, y, size, color);
		DrawMenuHelpLine(0x7c, (y + size), size, color);
		lines = 3;
		break;
	case 7:
		DrawMenuHelpLine(0x80, y, size, color);
		DrawMenuHelpLine(0x81, (y + size), size, color);
		DrawMenuHelpLine(0x82, (2.0f * size + y), size, color);
		lines = 3;
		break;
	case 9:
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0x8a, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0x8a, y, size, color);
			DrawMenuHelpLine(0x8b, (y + size), size, color);
			lines = 2;
		}
		break;
	case 10:
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0x8f, y, size, color);
			DrawMenuHelpLine(0x90, (y + size), size, color);
			lines = 2;
		} else {
			DrawMenuHelpLine(0x8f, y, size, color);
			lines = 1;
		}
		lines = lines + 1;
		break;
	case 0xb:
		DrawMenuHelpLine(0xcb, y, size, color);
		DrawMenuHelpLine(0xcc, (y + size), size, color);
		lines = 3;
		break;
	case 0x10:
		DrawMenuHelpLine(0x71, y, size, color);
		DrawMenuHelpLine(0x72, (y + size), size, color);
		lines = 2;
		break;
	case 0xc:
		DrawMenuHelpLine(0xf8, y, size, color);
		DrawMenuHelpLine(0xf9, (y + size), size, color);
		DrawMenuHelpLine(0xfa, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0xfb, (3.0f * size + y), size, color);
		lines = 4;
		break;
	case 0x11:
		DrawMenuHelpLine(0xfd, y, size, color);
		DrawMenuHelpLine(0xfe, (y + size), size, color);
		DrawMenuHelpLine(0xff, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0x100, (3.0f * size + y), size, color);
		lines = 4;
		break;
	case 0xd:
		DrawMenuHelpLine(0xda, y, size, color);
		DrawMenuHelpLine(0xdb, (y + size), size, color);
		DrawMenuHelpLine(0xdc, (2.0f * size + y), size, color);
		DrawMenuHelpLine(0xdd, (3.0f * size + y), size, color);
		DrawMenuHelpLine(0xde, (4.0f * size + y), size, color);
		lines = 5;
	}
	return lines;
}
extern "C" s32 fn_800362C4(void*, s32 id, u8 retry, f32 x, f32 y, f32 size, f32 alpha)
{
	s32 lines = 0;
	u32 color = (u32)(255.0f * alpha);
	switch (id) {
	case -128:
	case -14:
	case -12:
	case -11:
	case -10:
	case -8:
	case -7:
	case -4:
	case -1:
	case 0:
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0xC6, y, size, color);
			lines = 1;
		} else {
			DrawMenuHelpLine(0xC6, y, size, color);
			DrawMenuHelpLine(0xC7, y + size, size, color);
			DrawMenuHelpLine(0xC8, 2.0f * size + y, size, color);
			lines = 3;
		}
		break;
	case -5:
		DrawMenuHelpLine(0xF3, y, size, color);
		DrawMenuHelpLine(0xF4, y + size, size, color);
		lines = 2;
		break;
	case -3: {
		s32 n;
		if (lbl_8025C400[0x3318] == 6u) {
			DrawMenuHelpLine(0x67, y, size, color);
			DrawMenuHelpLine(0x68, y + (2.0f + size), size, color);
			DrawMenuHelpLine(0x69, 2.0f * (2.0f + size) + y, size, color);
			n = 3;
		} else {
			DrawMenuHelpLine(0x67, y, size, color);
			DrawMenuHelpLine(0x68, y + (2.0f + size), size, color);
			DrawMenuHelpLine(0x69, 2.0f * (2.0f + size) + y, size, color);
			DrawMenuHelpLine(0x6A, 3.0f * (2.0f + size) + y, size, color);
			n = 4;
		}
		lines = n;
		break;
	}
	case -9:
		DrawMenuHelpLine(0xA3, y, size, color);
		DrawMenuHelpLine(0xA4, y + size, size, color);
		DrawMenuHelpLine(0xA5, 2.0f * size + y, size, color);
		DrawMenuHelpLine(0xA6, 3.0f * size + y, size, color);
		lines = 4;
		break;
	case -2:
		if (retry) {

			if (lbl_8025C400[0x3318] == 6u) {
				DrawMenuHelpLine(0x6C, y, size, color);
				lines = 1;
			} else {
				DrawMenuHelpLine(0x6C, y, size, color);
				DrawMenuHelpLine(0x6D, y + size, size, color);
				DrawMenuHelpLine(0x6E, 2.0f * size + y, size, color);
				DrawMenuHelpLine(0x6F, 3.0f * size + y, size, color);
				lines = 4;
			}
		} else {
			DrawMenuHelpLine(0xD0, y, size, color);
			DrawMenuHelpLine(0xD1, y + size, size, color);
			DrawMenuHelpLine(0xD2, 2.0f * size + y, size, color);
			DrawMenuHelpLine(0xD3, 3.0f * size + y, size, color);
			lines = 4;
		}
		break;

	case -13:
	case -6:
		break;
	}
	return lines;
}
