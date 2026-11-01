#ifndef _GAME_INFOSCRN_SHARED_H
#define _GAME_INFOSCRN_SHARED_H

#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "types.h"
#include <dolphin/mtx/vec.h>

// the info screen, the hud drawn over the game: the clock, the battery, the happy points and
// the three counters that pop up in the corner. class names from RTTI, the layout from the
// methods. game/infoscrn.cpp, it sits at lbl_8025C400 + 0x8F38

// the screen overlays, see game/gxb.cpp for their methods
class CScrnColor {
public:
	f32 mColor[4];  // _00
	f32 mTarget[4]; // _10
	f32 mSpeed[4];  // _20
	// _30 vtable

	virtual ~CScrnColor(); // _08
};

class CScrnFade : public CScrnColor {
public:
	virtual void Reset(); // _0C
};

class CScrnWipe : public CScrnColor {
public:
	u8 _34[0x30]; // _34

	virtual void Reset();                       // _0C
	virtual void Update(u32 frames);            // _10
	virtual void Draw(void* game, BOOL screen); // _14
};

// one of the three counters in the corner, the static ones in game/infoscrn.cpp
class InfoPoint {
public:
	s32 mId;     // _00, 0x98 to 0x9A
	s32 mKind;   // _04
	s32 _08;     // _08
	s32 _0C;     // _0C
	s32 _10;     // _10
	f32 _14;     // _14
	s32 _18;     // _18
	f32 _1C[4];  // _1C
	Vec mPos[6]; // _2C
	u8 _74;      // _74
	u8 _75;      // _75
	u8 _76[0x2]; // _76
	s32 _78;     // _78
	f32 _7C;     // _7C
	s32 _80;     // _80
	// _84 vtable

	virtual ~InfoPoint() { } // _08
};

class InfoScrn {
public:
	CScrnFade mFade;        // _00
	CScrnWipe mWipe;        // _34
	s32 _98;                // _98
	s32 _9C;                // _9C
	s32 _A0;                // _A0
	s32 _A4;                // _A4
	f32 _A8;                // _A8
	f32 _AC;                // _AC
	f32 _B0;                // _B0
	s32 _B4;                // _B4
	s32 _B8;                // _B8
	s32 _BC;                // _BC
	f32 _C0;                // _C0
	f32 _C4;                // _C4
	f32 _C8;                // _C8
	f32 _CC;                // _CC
	f32 _D0;                // _D0
	s32 _D4;                // _D4
	s32 _D8;                // _D8
	f32 _DC;                // _DC
	f32 _E0[4];             // _E0
	s32 _F0[4];             // _F0
	u8 _100[4];             // _100
	f32 _104[4];            // _104
	f32 _114[4];            // _114
	u8 _124;                // _124
	Vec _128;               // _128
	u8 _134;                // _134
	f32 _138;               // _138
	f32 _13C;               // _13C
	f32 _140;               // _140
	f32 _144;               // _144
	s32 _148;               // _148
	f32 _14C;               // _14C
	s32 _150;               // _150
	s32 _154;               // _154
	u8 _158;                // _158
	u8 _159[0x164 - 0x159]; // _159
	s32 _164;               // _164, the battery level
	s32 _168;               // _168
	f32 _16C;               // _16C
	Vec _170;               // _170
	s32 _17C;               // _17C
	s32 _180;               // _180
	s32 _184;               // _184
	s32 _188;               // _188
	f32 _18C;               // _18C
	Vec _190;               // _190
	void* _19C;             // _19C
	s32 _1A0;               // _1A0
	f32 _1A4;               // _1A4
	Vec _1A8;               // _1A8
	s32 _1B4;               // _1B4
	f32 _1B8;               // _1B8
	f32 _1BC;               // _1BC, the timer in frames
	u8 _1C0;                // _1C0
	s32 _1C4;               // _1C4
	f32 _1C8;               // _1C8
	f32 _1CC;               // _1CC
	s32 _1D0;               // _1D0
	f32 _1D4;               // _1D4
	// _1D8 vtable

	virtual ~InfoScrn() { } // _08
};

#include "game/ctpl.h"
#include "game/ccameraaction.h"
#include <string.h>
#include <math.h>

// InfoScrn, the hud. the class names are from RTTI, the methods have no names in the dol, so
// they're extern "C" under their address

extern "C" u8 lbl_8025C400[];
#define GAME_FLAGS    (*(u64*)(lbl_8025C400 + 0x1B10))
#define GAME_4174     (*(f32*)(lbl_8025C400 + 0x4174))
#define GAME_417C     (*(f32*)(lbl_8025C400 + 0x417C))
#define GAME_PLAYER   (*(u8**)(lbl_8025C400 + 0x96BC)) // chibi robo's object
#define GAME_ROBO     (*(XObj**)(lbl_8025C400 + 0x96BC))
#define GAME_44       (*(u8**)(lbl_8025C400 + 0x44))
#define GS_RENDER     (*(void**)(lbl_8025C400 + 0x290))
#define GS_TEV        (*(void**)(lbl_8025C400 + 0x294))
#define GAME_331C     (*(s32*)(lbl_8025C400 + 0x331C))
#define GAME_3320     (*(s32*)(lbl_8025C400 + 0x3320))
#define GAME_3324     (*(s32*)(lbl_8025C400 + 0x3324))
#define GAME_TPL      (*(TPLPalette**)(lbl_8025C400 + 0x3D4C)) // the system textures
#define GAME_TEX      ((CTexObj*)(lbl_8025C400 + 0x3D6C))
#define MENU_ACTIVE   (*(BOOL*)(lbl_8025C400 + 0x8580)) // some menu of the MenuEx is open
#define GAME_EVT      (*(void**)(lbl_8025C400 + 0x5D080))
#define GAME_SOUND    (lbl_8025C400 + 0xE7E24)
#define GAME_LOAD_MAX (*(s32*)(lbl_8025C400 + 0xA860))
#define GAME_LOAD_CUR (*(s32*)(lbl_8025C400 + 0xA864))

// the right edge the corner counters line up on
static inline f32 GetRight()
{
	return (GAME_417C / 2 + GAME_4174 >= 418.0f) ? 418.0f : GAME_417C / 2 + GAME_4174;
}

extern "C" {
f32 fn_801968BC(void* gs, const char* text, s32 a, f32 size);
void fn_80196610(void* gs, u32 color, const char* fmt, f32 x, f32 y, f32 z, f32 sx, f32 sy, ...);
void fn_8019B464(void* mode, s32 enable, s32 func, s32 update);
void fn_8019B34C(void* mode, s32 a, s32 b);
void fn_8019B56C(void* mode, s32 type, s32 src, s32 dst, s32 op);
void fn_80199AB0(void* gs, s32 a, s32 b);
void fn_801992BC(void* gs, s32 mode, u32 c0, u32 c1, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1);
void fn_80198D08(void* gs, s32 prim, void* mtx, Vec* pos, u32 color, s32 count);
void fn_8019AFA0(void* tev, s32 mode);
u16 GXGetTexObjWidth(const GXTexObj* obj);
u16 GXGetTexObjHeight(const GXTexObj* obj);
void PSMTXTrans(Mtx m, f32 x, f32 y, f32 z);
void PSMTXScale(Mtx m, f32 x, f32 y, f32 z);
void fn_80197800(void* gs, CTexObj* tex, u32 c0, u32 c1, u32 c2, u32 c3, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
void fn_801990EC(void* gs, Vec* pos, u32 color, u16 count);
void fn_8006389C(InfoPoint* self);
void fn_800650AC(InfoPoint* self, s32 id);
void fn_80066714(InfoScrn* self);
void fn_80066C08(InfoScrn* self);
void fn_80067418(InfoScrn* self);
void fn_80067928(InfoScrn* self);
void fn_80068D10(InfoScrn* self);
void fn_8006AB00(InfoScrn* self);
void fn_8006AF34(InfoScrn* self, u32 frames);
bool fn_80051C60();
const char* fn_8006DBF0(s32 id);
void fn_8019B360(void* mode, s32 cull);
void fn_8001E904(void* sound, s32 id, s32 volume);
}

static f32 sScale = 1.0f;

// moves cur toward target by step, not while the game is paused
static inline void Approach(f32* cur, f32* target, f32 step)
{
	if (GAME_FLAGS & 0x200000000000ULL) {
		return;
	}
	if (*cur > *target && (*cur -= step) < *target) {
		*cur = *target;
	}
	if (*cur < *target && (*cur += step) > *target) {
		*cur = *target;
	}
}

// a quarter of the way there each frame, at least 1
static inline f32 ApproachStep(f32 cur, f32 target)
{
	f32 step = (target - cur) / 4;
	if (step < 0.0f) {
		step = -step;
	}
	if (step <= 1.0f) {
		step = 1.0f;
	}
	return step;
}

// a menu of the MenuEx, game/stg.h has it as CMenu
class InfoMenu {
public:
	u8 _00[0x148]; // _00
	// _148 vtable

	virtual void vf08();
	virtual void vf0C();
	virtual void vf10();
	virtual void vf14();
	virtual void vf18();
	virtual void vf1C();
	virtual void vf20();
	virtual BOOL IsOpen(); // _24
};
#define GAME_MENU(i) (((InfoMenu**)(lbl_8025C400 + 0x8584))[i])

// the menu in the fourth slot is open
static inline BOOL IsMenu3Open()
{
	return GAME_MENU(3) ? GAME_MENU(3)->IsOpen() : FALSE;
}

// frames to a clock reading, minutes * 6000 + seconds * 100 + hundredths
static inline f32 FramesToClock(f32 t)
{
	f32 sec  = t / 60.0f;
	s32 hund = ((s32)t % 60) * 100 / 60;
	if (t >= 359999.0f) {
		hund = 99;
	}
	f32 min = sec / 60.0f;
	return hund + (6000.0f * (s32)min + ((s32)sec % 60) * 100);
}

// 10 to the n
static inline f32 Pow10(int n)
{
	f32 p = 1.0f;
	for (int i = 0; i < n; i++) {
		p *= 10.0f;
	}
	return p;
}

// one slot of chibi robo's item list
struct InfoItem {
	s16 mId;  // _00
	s16 mNum; // _02
};
#define PLAYER_ITEMS(player) ((InfoItem*)((player) + 0x9688)) // 256 of them

// how much the battery holds, from the best battery upgrade chibi robo has
static inline s32 GetBatteryLevel()
{
	for (int i = 0; i < 256; i++) {
		InfoItem* item = &PLAYER_ITEMS(GAME_PLAYER)[i];
		s16 id         = item->mId;
		s16 num        = item->mNum;
		if (id >= 0 && num > 0) {
			switch (id) {
			case 0x57:
				return 5;
			case 0x59:
				return 10;
			case 0x5A:
				return 15;
			}
		}
	}
	return 5;
}

// the three counters, their __sinit and destructor follow this file's code
static u8 sUnk[0x140];
static InfoPoint sPoint1;
static InfoPoint sPoint0;
static InfoPoint sPoint2;
static f32 sHistory[10];

extern "C" {
void fn_8002390C(CScrnColor* scrn, void* game);
void fn_80023B38(CScrnColor* scrn, u32 frames);
void fn_8006AB00(InfoScrn* self);
void fn_8006AF34(InfoScrn* self, u32 frames);
bool fn_80051C60();
const char* fn_8006DBF0(s32 id);
void fn_8019B360(void* mode, s32 cull);
void fn_8001E904(void* sound, s32 id, s32 volume);
}

// an event is running that keeps the hud up
static inline bool IsEventHud()
{
	bool on = false;
	if ((GAME_331C != 0 || GAME_3320 != 0) && GAME_3324 == 0) {
		on = true;
	}
	return on;
}

// the hud is drawn in the field, outside of events
static inline bool IsHudOn()
{
	bool on = false;
	if ((GAME_FLAGS & 1) && !(GAME_FLAGS & 0x22) && !IsEventHud()) {
		on = true;
	}
	return on;
}

#endif
