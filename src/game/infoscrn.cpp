#include "game/particle_srt.h"
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

/**
 * @note Address: 0x8006D850
 * @note Size: 0x2A4
 */
extern "C" void fn_8006D850(InfoScrn* self)
{
	self->mFade.Reset();
	self->mWipe.Reset();
	self->_A8  = 0.0f;
	self->_AC  = 0.0f;
	self->_B0  = 0.0f;
	self->_140 = 0.0f;
	self->_144 = 0.0f;
	self->_98  = 0;
	self->_9C  = 0;
	self->_A0  = 0;
	self->_A4  = 0;
	self->_B4  = 0;
	self->_1D4 = 0.0f;
	self->_B8  = 0;
	self->_17C = 0;
	self->_180 = 0;
	fn_800650AC(&sPoint0, 0x9A);
	fn_800650AC(&sPoint1, 0x98);
	fn_800650AC(&sPoint2, 0x99);
	sPoint0._74  = FALSE;
	sPoint1._74  = FALSE;
	sPoint2._74  = FALSE;
	self->_BC    = 0;
	self->_C0    = 0.0f;
	self->_C4    = 0.0f;
	self->_C8    = 0.0f;
	const Vec v0 = { 0.0f, 0.0f, 0.0f };
	self->_128.x = v0.x;
	self->_128.y = v0.y;
	self->_128.z = v0.z;
	self->_148   = 0;
	self->_14C   = 0.0f;
	self->_150   = 0;
	self->_154   = 0;
	self->_158   = 0;
	const Vec v1 = { 0.0f, 0.0f, 0.0f };
	self->_170.x = v1.x;
	self->_170.y = v1.y;
	self->_170.z = v1.z;
	self->_16C   = 1.0f;
	self->_188   = 0;
	self->_18C   = 0.0f;
	const Vec v2 = { 0.0f, 0.0f, 0.0f };
	self->_190.x = v2.x;
	self->_190.y = v2.y;
	self->_190.z = v2.z;
	self->_19C   = NULL;
	self->_1A0   = 0;
	self->_1A4   = 0.0f;
	const Vec v3 = { 0.0f, 0.0f, 0.0f };
	self->_1A8.x = v3.x;
	self->_1A8.y = v3.y;
	self->_1A8.z = v3.z;
	self->_1B4   = 0;
	self->_1B8   = 0.0f;
	self->_1BC   = 0.0f;
	self->_1C0   = 0;
	self->_1C4   = 0;
	self->_1C8   = 0.0f;
	self->_1CC   = 0.0f;
	self->_184   = 0;
	self->_1D0   = 0;
	self->_CC    = 0.0f;
	self->_D0    = 0.0f;
	self->_D4    = 0;
	self->_D8    = 0;
	self->_DC    = 1.4f;
	f32 d        = self->_CC - self->_D0;
	for (int i = 0; i < 4; i++) {
		self->_E0[i]  = 0.0f;
		self->_F0[i]  = 0;
		self->_100[i] = 0;
		self->_104[i] = d;
		self->_114[i] = 0.0f;
	}
	self->_124 = 0;
	self->_134 = 0;
	self->_138 = 0.0f;
	self->_13C = 0.0f;
	sScale     = 1.0f;
	self->_164 = 5;
}

/**
 * @note Address: 0x8006D824
 * @note Size: 0x2C
 */
extern "C" BOOL fn_8006D824(InfoScrn* self)
{
	sPoint0._74 = FALSE;
	sPoint1._74 = FALSE;
	sPoint2._74 = FALSE;
	return TRUE;
}

/**
 * @note Address: 0x8006D5A4
 * @note Size: 0x280
 */
extern "C" void fn_8006D5A4(InfoScrn* self)
{
	self->_A8  = 0.0f;
	self->_AC  = 0.0f;
	self->_B0  = 0.0f;
	self->_140 = 0.0f;
	self->_144 = 0.0f;
	self->_98  = 0;
	self->_9C  = 0;
	self->_A0  = 0;
	self->_A4  = 0;
	self->_B4  = 0;
	self->_1D4 = 0.0f;
	self->_B8  = 0;
	self->_17C = 0;
	self->_180 = 0;
	fn_800650AC(&sPoint0, 0x9A);
	fn_800650AC(&sPoint1, 0x98);
	fn_800650AC(&sPoint2, 0x99);
	sPoint0._74  = FALSE;
	sPoint1._74  = FALSE;
	sPoint2._74  = FALSE;
	self->_BC    = 0;
	self->_C0    = 0.0f;
	self->_C4    = 0.0f;
	self->_C8    = 0.0f;
	const Vec v0 = { 0.0f, 0.0f, 0.0f };
	self->_128.x = v0.x;
	self->_128.y = v0.y;
	self->_128.z = v0.z;
	self->_148   = 0;
	self->_14C   = 0.0f;
	self->_150   = 0;
	self->_154   = 0;
	self->_158   = 0;
	const Vec v1 = { 0.0f, 0.0f, 0.0f };
	self->_170.x = v1.x;
	self->_170.y = v1.y;
	self->_170.z = v1.z;
	self->_16C   = 1.0f;
	self->_188   = 0;
	self->_18C   = 0.0f;
	const Vec v2 = { 0.0f, 0.0f, 0.0f };
	self->_190.x = v2.x;
	self->_190.y = v2.y;
	self->_190.z = v2.z;
	self->_19C   = NULL;
	self->_1A0   = 0;
	self->_1A4   = 0.0f;
	const Vec v3 = { 0.0f, 0.0f, 0.0f };
	self->_1A8.x = v3.x;
	self->_1A8.y = v3.y;
	self->_1A8.z = v3.z;
	self->_1B4   = 0;
	self->_1B8   = 0.0f;
	self->_1BC   = 0.0f;
	self->_1C0   = 0;
	self->_1C4   = 0;
	self->_1C8   = 0.0f;
	self->_1CC   = 0.0f;
	self->_184   = 0;
	self->_1D0   = 0;
	self->_CC    = 0.0f;
	self->_D0    = 0.0f;
	self->_D4    = 0;
	self->_D8    = 0;
	self->_DC    = 1.4f;
	f32 d        = self->_CC - self->_D0;
	for (int i = 0; i < 4; i++) {
		self->_E0[i]  = 0.0f;
		self->_F0[i]  = 0;
		self->_100[i] = 0;
		self->_104[i] = d;
		self->_114[i] = 0.0f;
	}
	self->_124 = 0;
	self->_134 = 0;
	self->_138 = 0.0f;
	self->_13C = 0.0f;
	sScale     = 1.0f;
	self->_164 = 5;
}

/**
 * @note Address: 0x8006D548
 * @note Size: 0x5C
 */
extern "C" void fn_8006D548(InfoScrn* self, u32 frames)
{
	fn_80023B38(&self->mFade, frames);
	self->mWipe.Update(frames);
	fn_8006AF34(self, frames);
}

/**
 * @note Address: 0x8006D498
 * @note Size: 0xB0
 */
extern "C" void fn_8006D498(InfoScrn* self)
{
	fn_80199AB0(lbl_8025C400, 0, 0);
	fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
	fn_8019B464(GS_RENDER, 0, 3, 0);
	fn_8002390C(&self->mFade, lbl_8025C400);
	self->mWipe.Draw(lbl_8025C400, FALSE);
	fn_8006AB00(self);
}

// the loading bar, the robot walks along it
static const u8 sWalkAnim[]  = { 3, 2, 1, 0, 3, 1, 2, 0xFF };
static const u8 sStandAnim[] = { 0, 1, 2, 3, 0xFF };
static struct {
	f32 mX;          // _00
	s32 mFrame;      // _04
	const u8* mAnim; // _08
} sLoading = { 0.0f, 0, sWalkAnim };

/**
 * @note Address: 0x8006CB68
 * @note Size: 0x930
 */
// NONMATCHING: the robot's x reads the struct through a register, scratch registers
extern "C" void fn_8006CB68(InfoScrn* self)
{
	if (GAME_FLAGS & 0x40) {
		fn_8019B464(GS_RENDER, 1, 3, 0);
		if (GAME_LOAD_MAX > 0) {
			f32 fill;
			f32 max;
			f32 y;
			f32 left;
			f32 half;
			f32 tw;
			f32 th;
			f32 scale = 1.0f / GAME_LOAD_MAX;
			max       = GAME_LOAD_MAX;
			fill      = max * (GAME_LOAD_CUR * scale);
			CTexObj tex;
			if (sLoading.mAnim == sWalkAnim) {
				if (sWalkAnim[sLoading.mFrame] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[0x82 + sWalkAnim[sLoading.mFrame]], 0);
			} else {
				if (sStandAnim[sLoading.mFrame / 2] == 0xFF) {
					sLoading.mFrame = 0;
				}
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[0x86 + sStandAnim[sLoading.mFrame / 2]], 0);
			}
			tw = GXGetTexObjWidth(&tex);
			th = GXGetTexObjHeight(&tex);
			fn_8019B360(GS_RENDER, 1);
			fn_8019AFA0(GS_TEV, 2);
			half = max / 2;
			y    = 406.0f - th / 2;
			left = 320.0f - half;
			fn_80197800(lbl_8025C400, &tex, 0x808080C8, 0x808080C8, 0x808080C8, 0x808080C8, sLoading.mX + left, y, 0.0f, 0.9f * tw,
			            0.9f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(GS_TEV, 0);
			fn_80197800(lbl_8025C400, &tex, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, sLoading.mX + left, y, 0.0f, 0.8f * tw,
			            0.8f * th, 0.0f, 3.1415927f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
			fn_8019B360(GS_RENDER, 2);

			fn_8019AFA0(GS_TEV, 0);
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[117], 0);
			PSMTXScale(tex.mMtx[1], 0.25f, 1.0f, 1.0f);
			tex.mMtxDirty = TRUE;
			PSMTXTrans(tex.mMtx[0], 0.25f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f, 408.0f, 0.0f, max, 14.0f, 0.0f, 0.0f,
			            3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.5f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 322.0f - half - 4.0f, 408.0f, 0.0f, 8.0f, 14.0f,
			            0.0f, 0.0f, 3.1415927f);
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 0xC8C8C880, 4.0f + (322.0f + half), 408.0f, 0.0f, 8.0f,
			            14.0f, 0.0f, 0.0f, 3.1415927f);

			tex      = GAME_TEX->Init(&GAME_TPL->descriptorArray[118], 0);
			u16 barW = GXGetTexObjWidth(&tex);
			PSMTXScale(tex.mMtx[1], fill / barW, 1.0f, 1.0f);
			tex.mMtxDirty      = TRUE;
			static f32 sScroll = 0.0f;
			sScroll -= 0.1f;
			PSMTXTrans(tex.mMtx[0], sScroll, 0.0f, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_80197800(lbl_8025C400, &tex, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 0x9FFA1CC8, 321.0f - (max - fill) / 2, 408.0f, 0.0f, fill,
			            10.0f, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);

			sLoading.mFrame++;
			if ((sLoading.mX += 6.0f) >= max) {
				sLoading.mX = max;
				if (sLoading.mAnim == sWalkAnim) {
					sLoading.mAnim  = sStandAnim;
					sLoading.mFrame = 0;
				}
			}
		} else {
			sLoading.mX     = 0.0f;
			sLoading.mFrame = 0;
			sLoading.mAnim  = sWalkAnim;
		}
		const char* text = fn_8006DBF0(0x59);
		f32 w            = fn_801968BC(lbl_8025C400, text, 1, 16.0f);
		fn_80196610(lbl_8025C400, -1, text, 320.0f - w / 2, 386.0f, 0.0f, 16.0f, 16.0f);
	}
}

/**
 * @note Address: 0x8006CA90
 * @note Size: 0xD8
 */
extern "C" void fn_8006CA90(InfoScrn* self)
{
	if (GAME_FLAGS & 0x200000000000ULL) {
		fn_8019B464(GS_RENDER, 0, 3, 0);
		const char* text = "\\B00000080 PAUSE ";
		f32 w            = fn_801968BC(lbl_8025C400, text, 1, 24.0f);
		fn_80196610(lbl_8025C400, -1, text, 320.0f - w / 2, 200.0f, 0.0f, 24.0f, 24.0f);
		fn_8019B464(GS_RENDER, 1, 3, 0);
	}
}

/**
 * @note Address: 0x8006AB00
 * @note Size: 0x434
 */
// NONMATCHING: the bss address and the zero swap r6 and r7
extern "C" void fn_8006AB00(InfoScrn* self)
{
	if (IsHudOn() || (GAME_FLAGS & 0x200000000000ULL)) {
		fn_8006389C(&sPoint0);
		fn_8006389C(&sPoint1);
		fn_8006389C(&sPoint2);
		fn_80068D10(self);
		fn_80067928(self);
		f32 alpha = self->_18C * sScale;
		if (alpha > 0.0f) {
			CTexObj tex;
			if (self->_B4 % 32 >= 8) {
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[150], 0);
			} else {
				tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
			}
			f32 h;
			f32 w;
			f32 y;
			f32 x;
			u16 tw = GXGetTexObjWidth(&tex);
			w      = 0.8f * (1.1f * tw);
			u16 th = GXGetTexObjHeight(&tex);
			h      = 0.8f * th;
			x      = 6.0f + (w / 2 + self->_190.x);
			y      = self->_190.y - h / 2 - 6.0f;
			fn_8019AFA0(GS_TEV, 0);
			u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_80067418(self);
		fn_80066C08(self);
		fn_80066714(self);
	}
}

/**
 * @note Address: 0x80068C8C
 * @note Size: 0x84
 */
extern "C" void fn_80068C8C(f32* cur, f32* target, f32 step)
{
	Approach(cur, target, step);
}

/**
 * @note Address: 0x80068C24
 * @note Size: 0x68
 */
extern "C" void fn_80068C24(InfoScrn* self, f32* y, f32* x)
{
	f32 right = GetRight() - 64.0f - 40.0f - 9.0f;
	if (y) {
		*y = 60.0f;
	}
	if (x) {
		*x = right;
	}
}

/**
 * @note Address: 0x80068C10
 * @note Size: 0x14
 */
extern "C" void fn_80068C10(InfoScrn* self)
{
	sPoint0._74 = TRUE;
}

/**
 * @note Address: 0x80068BFC
 * @note Size: 0x14
 */
extern "C" void fn_80068BFC(InfoScrn* self)
{
	sPoint0._74 = FALSE;
}

/**
 * @note Address: 0x80068BE8
 * @note Size: 0x14
 */
extern "C" void fn_80068BE8(InfoScrn* self)
{
	sPoint1._74 = TRUE;
}

/**
 * @note Address: 0x80068BD4
 * @note Size: 0x14
 */
extern "C" void fn_80068BD4(InfoScrn* self)
{
	sPoint1._74 = FALSE;
}

/**
 * @note Address: 0x80068BC0
 * @note Size: 0x14
 */
extern "C" void fn_80068BC0(InfoScrn* self)
{
	sPoint2._74 = TRUE;
}

/**
 * @note Address: 0x80068BAC
 * @note Size: 0x14
 */
extern "C" void fn_80068BAC(InfoScrn* self)
{
	sPoint2._74 = FALSE;
}

/**
 * @note Address: 0x80068B98
 * @note Size: 0x14
 */
extern "C" void fn_80068B98(InfoScrn* self)
{
	sPoint2._78 = 2;
}

/**
 * @note Address: 0x80068B70
 * @note Size: 0x28
 */
extern "C" void fn_80068B70(InfoScrn* self)
{
	sPoint0._75 = TRUE;
	sPoint1._75 = TRUE;
	sPoint2._75 = TRUE;
}

/**
 * @note Address: 0x80068B48
 * @note Size: 0x28
 */
extern "C" void fn_80068B48(InfoScrn* self)
{
	sPoint0._75 = FALSE;
	sPoint1._75 = FALSE;
	sPoint2._75 = FALSE;
}

/**
 * @note Address: 0x80067418
 * @note Size: 0x510
 */
extern "C" void fn_80067418(InfoScrn* self)
{
	f32 alpha = self->_1A4 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 frame = self->_B4 % 8;
		if (frame < 2) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
		} else if (frame < 4) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[150], 0);
		} else if (frame < 6) {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[149], 0);
		} else {
			tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[151], 0);
		}
		f32 x;
		f32 y;
		f32 w;
		f32 h;
		u16 tw  = GXGetTexObjWidth(&tex);
		w       = 0.8f * (1.1f * tw);
		u16 th  = GXGetTexObjHeight(&tex);
		h       = 0.8f * th;
		Vec ofs = { 0.0f, 10.0f, 0.0f };
		f32 sx;
		f32 sy;
		GAME_ROBO->vf10C(&ofs, &sx, &sy, NULL, 0);
		x = 6.0f + (w / 2 + sx);
		y = sy - h / 2 - 6.0f;
		fn_8019AFA0(GS_TEV, 0);
		u32 color = (u32)(255.0f * alpha) | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, color, color, color, color, x, y, 0.0f, w, h, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);
	}
}

/**
 * @note Address: 0x80066C08
 * @note Size: 0x810
 */
extern "C" void fn_80066C08(InfoScrn* self)
{
	f32 alpha = self->_1B8 * sScale;
	CTexObj tex;
	if (alpha > 0.0f) {
		u32 a      = 255.0f * alpha;
		u32 top    = a | 0xEFEBC500;
		u32 bottom = a | 0xEDA83600;
		f32 clock  = FramesToClock(self->_1BC);
		f32 step;
		f32 w;
		f32 h;
		f32 smallW;
		f32 smallH;
		f32 value = 10000.0f * (s32)(clock / 6000.0f) + (s32)clock % 6000;
		tex       = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
		u16 tw    = GXGetTexObjWidth(&tex);
		w         = 1.8f * tw;
		u16 th    = GXGetTexObjHeight(&tex);
		h         = 1.8f * (th / 10);
		tw        = GXGetTexObjWidth(&tex);
		smallW    = 0.7f * (1.8f * tw);
		th        = GXGetTexObjHeight(&tex);
		smallH    = 0.7f * (1.8f * (th / 10));
		PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
		tex.mMtxDirty = TRUE;
		step          = 7.0f * w;
		s32 digit;
		for (s32 i = 0; i < 6; i++) {
			f32 div = 1.0f;
			if (i > 0) {
				for (s32 j = 0; j < i; j++) {
					div *= 10.0f;
				}
			}
			digit = (s32)(value / div);
			digit %= 10;
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * digit, 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(GS_TEV, 0);
			f32 ofs = 0.0f;
			if (i == 0) {
				ofs = -7.2f;
			} else if (i == 1) {
				ofs = -1.8f;
			} else if (i == 4 || i == 5) {
				ofs = -9.0f;
			}
			if (i == 5 && digit == 0) {
				continue;
			}
			if (i < 2) {
				fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 75.6f, 0.0f,
				            smallW, smallH, 0.0f, 0.0f, 0.0f);
			} else {
				fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, ofs + (step + (372.0f - i * (w - 3.6f)) - 14.4f), 72.0f, 0.0f, w,
				            h, 0.0f, 0.0f, 0.0f);
			}
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_8019AFA0(GS_TEV, 4);
		tex        = GAME_TEX->Init(&GAME_TPL->descriptorArray[129], 0);
		u16 colonW = GXGetTexObjWidth(&tex);
		u16 colonH = GXGetTexObjHeight(&tex);
		fn_8019AFA0(GS_TEV, 0);
		fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, 472.8f, 72.9f, 0.0f, 0.6f * (1.8f * colonW), 0.6f * (1.8f * colonH), 0.0f,
		            0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);
	}
}

/**
 * @note Address: 0x80066BFC
 * @note Size: 0xC
 */
extern "C" void fn_80066BFC(InfoScrn* self)
{
	self->_1C0 = TRUE;
}

/**
 * @note Address: 0x80066BF0
 * @note Size: 0xC
 */
extern "C" void fn_80066BF0(InfoScrn* self)
{
	self->_1C0 = FALSE;
}

/**
 * @note Address: 0x80066B9C
 * @note Size: 0x54
 */
extern "C" void fn_80066B9C(InfoScrn* self, s32 time)
{
	self->_1BC = 60.0f * (time / 100.0f);
	if (self->_1BC >= 359999.0f) {
		self->_1BC = 359999.0f;
	}
}

/**
 * @note Address: 0x80066A84
 * @note Size: 0x118
 */
extern "C" s32 fn_80066A84(InfoScrn* self)
{
	return FramesToClock(self->_1BC);
}

/**
 * @note Address: 0x8006697C
 * @note Size: 0x108
 */
// NONMATCHING: the three int to float conversions land in other registers
extern "C" f32 fn_8006697C(f32 t)
{
	f32 sec  = t / 60.0f;
	s32 hund = ((s32)t % 60) * 100 / 60;
	if (t >= 359999.0f) {
		hund = 99;
	}
	f32 min = sec / 60.0f;
	f32 ret = hund;
	ret += 6000.0f * (s32)min + ((s32)sec % 60) * 100;
	return ret;
}

/**
 * @note Address: 0x80066714
 * @note Size: 0x268
 */
extern "C" void fn_80066714(InfoScrn* self)
{
	f32 alpha = self->_1C8;
	if (alpha > 0.0f) {
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_8019B464(GS_RENDER, 1, 7, 1);
		fn_8019B464(GS_RENDER, 1, 7, 0);
		u32 a = 255.0f * alpha;

		static f32 sScroll = 0.0f;
		if (!MENU_ACTIVE) {
			sScroll += 1.0f;
			if (sScroll >= 4.0f) {
				sScroll = 0.0f;
			}
		}

		fn_8019B56C(GS_RENDER, 3, 4, 5, 15);
		fn_8019B34C(GS_RENDER, 12, 5);
		u32 color = a | 0x30303000;
		for (s32 y = 0; y < 448; y += 4.0f) {
			fn_801992BC(lbl_8025C400, 0, color, color, 0.0f, y + sScroll, 0.0f, 640.0f, y + sScroll, 0.0f);
		}
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_8019B34C(GS_RENDER, 12, 5);
		for (int i = 9; i > 0; i--) {
			sHistory[i] = sHistory[i - 1];
		}
		sHistory[0] = 0.0f;
		fn_8019B34C(GS_RENDER, 12, 5);
	}
}

static const char* sAscii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
static const char* sWide  = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
                            "\x81\x69\x81\x6A\x81\x96\x81\x7B\x81\x43\x81\x5B\x81\x44\x81\x5E"
                            "\x82\x4F\x82\x50\x82\x51\x82\x52\x82\x53\x82\x54\x82\x55\x82\x56"
                            "\x82\x57\x82\x58\x81\x46\x81\x47\x81\x83\x81\x81\x81\x84\x81\x48"
                            "\x81\x97\x82\x60\x82\x61\x82\x62\x82\x63\x82\x64\x82\x65\x82\x66"
                            "\x82\x67\x82\x68\x82\x69\x82\x6A\x82\x6B\x82\x6C\x82\x6D\x82\x6E"
                            "\x82\x6F\x82\x70\x82\x71\x82\x72\x82\x73\x82\x74\x82\x75\x82\x76"
                            "\x82\x77\x82\x78\x82\x79\x81\x6D\x81\x8F\x81\x6E\x81\x4F\x81\x51"
                            "\x81\x65\x82\x81\x82\x82\x82\x83\x82\x84\x82\x85\x82\x86\x82\x87"
                            "\x82\x88\x82\x89\x82\x8A\x82\x8B\x82\x8C\x82\x8D\x82\x8E\x82\x8F"
                            "\x82\x90\x82\x91\x82\x92\x82\x93\x82\x94\x82\x95\x82\x96\x82\x97"
                            "\x82\x98\x82\x99\x82\x9A\x81\x6F\x81\x62\x81\x70";

/**
 * @note Address: 0x80066630
 * @note Size: 0xE4
 */
extern "C" s32 fn_80066630(char* text)
{
	char buf[0x200];
	char* out = buf;
	char* dst;
	char* p;
	s32 count = 0;
	for (p = text, dst = out; *p != '\0'; p++) {
		BOOL found = FALSE;
		for (s32 i = 0; sAscii[i] != '\0'; i++) {
			if (*p == sAscii[i]) {
				strncpy(dst, &sWide[i * 2], 2)[2] = '\0';
				found                             = TRUE;
				count++;
				dst += 2;
				break;
			}
		}
		if (!found) {
			strncpy(dst, p, 1)[1] = '\0';
			dst++;
		}
	}
	strcpy(text, out);
	return count;
}

/**
 * @note Address: 0x800665FC
 * @note Size: 0x34
 */
extern "C" BOOL fn_800665FC(InfoScrn* self)
{
	return self->_1D4 == 10.0f * *(f32*)(GAME_PLAYER + 0x965C);
}

/**
 * @note Address: 0x800665E0
 * @note Size: 0x1C
 */
extern "C" BOOL fn_800665E0(InfoScrn* self)
{
	return self->_1D4 <= 0.0f;
}

/**
 * @note Address: 0x8006656C
 * @note Size: 0x74
 */
extern "C" void fn_8006656C(InfoScrn* self, f32 x)
{
	for (int i = 0; i < 5; i++) {
		self->_E0[i]  = 1.0f;
		self->_100[i] = 0;
		self->_104[i] = 0.0f;
		self->_114[i] = 0.0f;
	}
	self->_D0 = 10.0f * x;
	self->_CC = self->_D0;
}

/**
 * @note Address: 0x800664C4
 * @note Size: 0xA8
 */
extern "C" void fn_800664C4(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_BC = 1;
		} else {
			self->_BC = 2;
		}
		break;
	case 1:
		if (on) {
			self->_BC = 3;
		} else {
			self->_BC = 4;
		}
		break;
	case 2:
		if (on) {
			self->_C8 = -52.0f;
		} else {
			self->_C8 = 0.0f;
		}
		break;
	case 3:
		if (on) {
			self->_BC = 5;
		} else {
			self->_BC = 6;
		}
		break;
	}
}

/**
 * @note Address: 0x80066250
 * @note Size: 0x274
 */
// NONMATCHING: the item loop's counter is in r7, not r5
extern "C" void fn_80066250(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_148 = 1;
		} else {
			self->_148 = 2;
		}
		break;
	case 1:
		if (on) {
			self->_148 = 3;
		} else {
			self->_148 = 4;
		}
		break;
	case 2:
		self->_164 = GetBatteryLevel();
		break;
	case 3:
		self->_148 = 5;
		self->_168 = GetBatteryLevel();
		break;
	case 4:
		if (on) {
			self->_148 = 7;
		} else {
			self->_148 = 8;
		}
		break;
	}
}

/**
 * @note Address: 0x800661E4
 * @note Size: 0x6C
 */
extern "C" void fn_800661E4(InfoScrn* self, void* obj, u8 on, s32 mode)
{
	self->_19C = NULL;
	switch (mode) {
	case 0:
		if (on) {
			self->_188 = 1;
			self->_19C = obj;
		} else {
			self->_188 = 2;
			self->_19C = NULL;
		}
		break;
	case 1:
		if (on) {
			self->_188 = 3;
			self->_19C = obj;
		} else {
			self->_188 = 4;
			self->_19C = NULL;
		}
		break;
	}
}

/**
 * @note Address: 0x80066190
 * @note Size: 0x54
 */
extern "C" void fn_80066190(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_1A0 = 1;
		} else {
			self->_1A0 = 2;
		}
		break;
	case 1:
		if (on) {
			self->_1A0 = 3;
		} else {
			self->_1A0 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x8006613C
 * @note Size: 0x54
 */
extern "C" void fn_8006613C(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_1B4 = 1;
		} else {
			self->_1B4 = 2;
		}
		break;
	case 1:
		if (on) {
			self->_1B4 = 3;
		} else {
			self->_1B4 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x800660E8
 * @note Size: 0x54
 */
extern "C" void fn_800660E8(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_1C4 = 1;
		} else {
			self->_1C4 = 2;
		}
		break;
	case 1:
		if (on) {
			self->_1C4 = 3;
		} else {
			self->_1C4 = 4;
		}
		break;
	}
}

/**
 * @note Address: 0x800660E4
 * @note Size: 0x4
 */
extern "C" void fn_800660E4(InfoScrn* self, s32 delta)
{
}

/**
 * @note Address: 0x800660E0
 * @note Size: 0x4
 */
extern "C" void fn_800660E0(InfoScrn* self, s32 level)
{
}

/**
 * @note Address: 0x80065E44
 * @note Size: 0x29C
 */
extern "C" void fn_80065E44(u32 color, f32 x, f32 y, f32 w, f32 h, f32 rx, f32 ry)
{
	Vec pos[65];
	f32 right;
	f32 bottom;
	f32 sx;
	f32 sy;
	sx     = 9.0f * rx;
	sy     = 9.0f * ry;
	right  = x + w;
	bottom = y + h;
	for (int i = 0; i < 65; i++) {
		f32 cx;
		f32 cy;
		f32 s;
		f32 angle = 0.09817477f * i - 1.5707964f;
		s         = sin(angle);
		f32 c     = -(f32)cos(angle);
		if ((i >= 0 && i < 16) || i == 64) {
			cx = x;
			cy = y;
		} else if ((i >= 16 && i < 32) || i == 64) {
			cx = right;
			cy = y;
		} else if ((i >= 32 && i < 48) || i == 64) {
			cx = right;
			cy = bottom;
		} else {
			cx = x;
			cy = bottom;
		}
		pos[i].x = s * sx + cx - w / 2;
		pos[i].y = c * sy + cy - h / 2;
		pos[i].z = 0.0f;
	}
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, color, 65);
	fn_8019B464(GS_RENDER, 1, 7, 0);
}

extern "C" {
void fn_80198614(void* gs, u32 c0, u32 c1, u32 c2, u32 c3, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3,
                 f32 y3, f32 z3);
void fn_80198258(void* gs, s32 mode, u32 c0, u32 c1, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
}

/**
 * @note Address: 0x800652C0
 * @note Size: 0xB84
 */
// NONMATCHING: vertex temporaries and floating-point register allocation
extern "C" void fn_800652C0(u32 color, u32 textureColor, f32 x, f32 y, f32 width, f32 height, f32 alpha, f32 scaleX, f32 scaleY)
{
	f32 angle  = 4.712389f * (1.0f - alpha);
	f32 sx     = 4.0f * (f32)sin(angle);
	f32 sy     = 4.0f * (f32)sin(angle);
	f32 left   = x - width / 2.0f - 40.0f - sx;
	f32 right  = sx + (40.0f + (x + width / 2.0f));
	f32 top    = y - 8.0f - sy;
	f32 bottom = sy + (8.0f + (y + height));
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198614(lbl_8025C400, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, left - 8.0f, top - 8.0f, -1.0f, 9.0f + (8.0f + right),
	            top - 8.0f, -1.0f, 9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f);
	Vec pos[10] = {
		(Vec) { x, y, 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (1.0f + top), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (1.0f + top), 0.0f },
		(Vec) { (right - 1.0f), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (right - 1.0f), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (bottom - 1.0f), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (bottom - 1.0f), 0.0f },
		(Vec) { (1.0f + left), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
	};
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, color, 10);
	fn_8019B464(GS_RENDER, 1, 1, 1);
	fn_80198258(lbl_8025C400, 16, color, color, (8.0f + (1.0f + left)), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, ((right - 1.0f) - 8.0f), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, (8.0f + (1.0f + left)), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, ((right - 1.0f) - 8.0f), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	u32 shadow = (u32)(64.0f * alpha);
	for (int i = 0; i < 10; i++) {
		pos[i].x += 9.0f;
		pos[i].y += 9.0f;
	}
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, shadow, 10);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	CTexObj tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[114], 0);
	f32 tw      = GXGetTexObjWidth(&tex);
	f32 th      = GXGetTexObjHeight(&tex);
	f32 w       = right - left - 128.0f;
	if (w < 0.0f)
		w = 0.0f;
	f32 h = bottom - top - 64.0f;
	if (h < 0.0f)
		h = 0.0f;
	f32 tiles[8][8] = {
		{ 0.0f, 0.0f, .5f, .5f, (32.0f + left - 3.0f), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ .5f, 0.0f, 1.0f / tw, .5f, x, (8.0f + ((16.0f + top) - 10.0f)), (6.0f + w), 32.0f },
		{ .5f, 0.0f, .5f, .5f, (3.0f + (right - 32.0f)), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ 0.0f, .5f, .5f, 1.0f / th, (32.0f + left - 3.0f), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ .5f, .5f, .5f, 1.0f / th, (3.0f + (right - 32.0f)), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ 0.0f, .5f, .5f, .5f, (32.0f + left - 3.0f), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
		{ .5f, .5f, 1.0f / tw, .5f, x, (10.0f + (bottom - 16.0f)), (6.0f + w), 32.0f },
		{ .5f, .5f, .5f, .5f, (3.0f + (right - 32.0f)), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
	};
	fn_8019AFA0(GS_TEV, 0);
	fn_8019B464(GS_RENDER, 1, 1, 1);
	for (int i = 0; i < 8; i++) {
		PSMTXTrans(tex.mMtx[0], tiles[i][0], tiles[i][1], 0.0f);
		tex.mMtxDirty = TRUE;
		PSMTXScale(tex.mMtx[1], tiles[i][2], tiles[i][3], 1.0f);
		tex.mMtxDirty = TRUE;
		fn_80197800(lbl_8025C400, &tex, textureColor, textureColor, textureColor, textureColor, tiles[i][4], tiles[i][5], 1.0f, tiles[i][6],
		            tiles[i][7], 0.0f, 0.0f, 0.0f);
	}
	fn_8019AFA0(GS_TEV, 4);
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198614(lbl_8025C400, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, left - 8.0f, top - 8.0f, -1.0f, 9.0f + (8.0f + right),
	            top - 8.0f, -1.0f, 9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f);
	fn_8019B464(GS_RENDER, 1, 3, 0);
}

/**
 * @note Address: 0x800650AC
 * @note Size: 0x214
 */
extern "C" void fn_800650AC(InfoPoint* self, s32 id)
{
	self->mId = id;
	self->_18 = 0;
	self->_08 = 0;
	self->_0C = 0;
	self->_10 = 0;
	self->_14 = 0.0f;
	for (int i = 0; i < 6; i++) {
		self->mPos[i].x = -220.0f;
	}
	for (int i = 0; i < 4; i++) {
		self->_1C[i] = 0.0f;
	}
	self->_74 = 0;
	self->_75 = 0;
	switch (self->mId) {
	case 0x9A:
		self->mKind     = self->mId;
		self->mPos[0].y = GetRight() - 64.0f - 40.0f;
		self->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case 0x98:
		self->mKind     = self->mId;
		self->mPos[0].y = GetRight() - 64.0f;
		self->mPos[1].y = GetRight() - 64.0f;
		break;
	case 0x99:
		self->mKind     = self->mId;
		self->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		self->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}
	self->_78 = 0;
	self->_7C = 0.0f;
	self->_80 = 0;
}

/**
 * @note Address: 0x800643A4
 * @note Size: 0xD08
 */
extern "C" void fn_800643A4(InfoPoint* self, u32 frames)
{
	switch (self->mId) {
	case 0x9A:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x964C);
		break;
	case 0x98:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x9660);
		break;
	case 0x99:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x9664);
		break;
	}

	bool changed = false;
	if (self->_1C[1] - self->_1C[0]) {
		changed = true;
	}
	if (IsMenu3Open()) {
		self->_1C[0]    = self->_1C[1];
		self->_1C[2]    = self->_1C[3];
		self->mPos[1].x = -220.0f;
		self->mPos[0].x = -220.0f;
		self->mPos[3].x = -220.0f;
		self->mPos[2].x = -220.0f;
		self->mPos[5].x = -220.0f;
		self->mPos[4].x = -220.0f;
		self->_08       = 0;
		changed         = false;
	} else {
		f32 step = (self->_1C[1] - self->_1C[0]) / 4;
		if (step < 0.0f) {
			step = -step;
		}
		f32 speed = step;
		if (step <= 1.0f) {
			speed = 1.0f;
		}
		Approach(&self->_1C[0], &self->_1C[1], speed);
		Approach(&self->_1C[2], &self->_1C[3], 1.0f);
	}
	Approach(&self->mPos[0].x, &self->mPos[1].x, ApproachStep(self->mPos[0].x, self->mPos[1].x));
	Approach(&self->mPos[2].x, &self->mPos[3].x, ApproachStep(self->mPos[2].x, self->mPos[3].x));
	Approach(&self->mPos[4].x, &self->mPos[5].x, ApproachStep(self->mPos[4].x, self->mPos[5].x));

	bool empty = false;
	if (0.0f == *(f32*)(GAME_PLAYER + 0xA30)) {
		empty = true;
	}
	if (self->_75) {
		empty = false;
	}
	switch (self->mId) {
	case 0x9A:
		self->mPos[0].y = GetRight() - 64.0f - 40.0f;
		self->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case 0x98:
		self->mPos[0].y = GetRight() - 64.0f;
		self->mPos[1].y = GetRight() - 64.0f;
		break;
	case 0x99:
		self->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		self->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}

	switch (self->_08) {
	case 0:
		if (IsMenu3Open()) {
			break;
		}
		if (empty) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		} else if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		}
		break;
	case 1:
		self->_10++;
		if (self->_10 > 6) {
			self->mPos[3].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 2:
		self->_10++;
		if (self->_10 > 6) {
			self->mPos[5].x = 60.0f;
			self->_08++;
		}
		break;
	case 3:
		if (!empty) {
			self->mPos[1].x = -220.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 4:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[5].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 5:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[3].x = -220.0f;
				self->_08       = 0;
			}
		}
		break;
	case 10:
		self->_10++;
		if (self->_10 > 3) {
			self->mPos[3].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 11:
		self->_10++;
		if (self->_10 > 3) {
			self->mPos[5].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 12:
		if (empty) {
			self->_08 = 3;
		} else {
			self->_10++;
			if (changed) {
				self->_10 = 0;
			}
			if (self->_10 > 90) {
				self->mPos[1].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 13:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[5].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 14:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[3].x = -220.0f;
				self->_08       = 0;
			}
		}
		break;
	}

	if (self->_74) {
		self->mPos[1].x = 60.0f;
		self->mPos[3].x = 60.0f;
		self->mPos[5].x = 60.0f;
		self->_10       = 0;
		self->_08       = 3;
	}

	switch (self->_0C) {
	case 0:
		self->_14 = 1.0f;
		break;
	case 1:
		self->_14 = 1.0f;
		break;
	case 2:
		self->_14 = 0.0f;
		break;
	case 3:
		self->_14 += 0.08f * frames;
		if (self->_14 >= 1.0f) {
			self->_14 = 1.0f;
		}
		if (GAME_EVT) {
			self->_0C = 4;
		}
		break;
	case 4:
		self->_14 -= 0.08f * frames;
		if (self->_14 < 0.0f) {
			self->_14 = 0.0f;
		}
		if (!GAME_EVT) {
			self->_0C = 3;
		}
		break;
	}

	self->_7C = 1.0f;
	switch (self->mId) {
	case 0x99:
		if (fn_80051C60()) {
			self->_78 = 0;
			self->_18 = 0;
			self->_7C = 1.0f;
			break;
		}
		switch (self->_78) {
		case 0:
			if (self->_1C[1] >= 300.0f) {
				self->_78++;
				self->_18 = 0;
			}
			break;
		case 1:
			self->_18 += frames;
			self->_7C = sin(0.017453292f * ((self->_18 * 6) % 360));
			if (self->_7C > 0.0f) {
				fn_8001E904(GAME_SOUND, 0x183, 0xFF);
				self->_78++;
			}
			break;
		case 2:
			self->_18 += frames;
			self->_7C = sin(0.017453292f * ((self->_18 * 6) % 360));
			if (self->_1C[1] < 300.0f) {
				self->_78 = 0;
			}
			break;
		case 3:
			if (self->_1C[1] < 300.0f) {
				self->_78 = 0;
			}
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8006389C
 * @note Size: 0xB08
 */
// NONMATCHING: float registers
extern "C" void fn_8006389C(InfoPoint* self)
{
	f32 alpha = self->_14 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 len = 1;
		f32 x0  = self->mPos[0].x;
		f32 y0  = self->mPos[0].y;
		f32 x2  = self->mPos[2].x;
		f32 x4  = self->mPos[4].x;
		while (true) {
			if ((s32)(self->_1C[0] / Pow10(len)) == 0) {
				break;
			}
			len++;
		}

		Vec pos[65];
		f32 cy = y0 - 7.2f;
		for (int i = 0; i < 65; i++) {
			f32 s;
			f32 angle = 0.09817477f * i - 1.5707964f;
			s         = sin(angle);
			f32 c     = -(f32)cos(angle);
			pos[i].x  = x0 + s * 14.4f;
			pos[i].y  = cy + c * 14.4f;
			pos[i].z  = 0.0f;
		}
		f32 a255 = 255.0f * alpha;
		u32 a    = a255;
		fn_8019B34C(GS_RENDER, 0x12, 5);
		fn_801990EC(lbl_8025C400, pos, a, 65);
		fn_8019B34C(GS_RENDER, 12, 5);

		tex          = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
		u16 tw       = GXGetTexObjWidth(&tex);
		f32 lineY    = cy + 14.4f;
		self->_1C[3] = len * (1.2f * tw - 3.6f) + 34.0f;
		pos[0].x     = x0;
		pos[0].y     = lineY;
		pos[1].x     = x0 + self->_1C[2];
		pos[1].y     = lineY;
		fn_8019B34C(GS_RENDER, 0x12, 5);
		fn_801990EC(lbl_8025C400, pos, a, 2);
		fn_8019B34C(GS_RENDER, 12, 5);

		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[155], 0);
		fn_8019AFA0(GS_TEV, 0);
		u16 ew = GXGetTexObjWidth(&tex);
		u16 eh = GXGetTexObjHeight(&tex);
		u32 c  = a | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, c, c, c, c, pos[1].x, pos[1].y, 0.0f, 1.2f * ew, 1.2f * eh, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);

		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[self->mKind], 0);
		fn_8019AFA0(GS_TEV, 0);
		f32 iconW = 0.8f * GXGetTexObjWidth(&tex);
		f32 iconY = y0 - 9.0f;
		f32 iconH = 0.8f * GXGetTexObjHeight(&tex);
		if (self->mId == 0x9A) {
			u32 ic = (u32)(a255 * self->_7C) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, ic, ic, ic, ic, x4, 4.0f + iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		} else {
			u32 ic = (u32)(a255 * self->_7C) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, ic, ic, ic, ic, x4, iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		}

		tex    = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
		f32 dx = x2 - 12.0f;
		f32 dy = y0 - 4.0f;
		f32 w  = 1.2f * GXGetTexObjWidth(&tex);
		f32 h  = 1.2f * (GXGetTexObjHeight(&tex) / 10);
		PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
		u8 da         = (s32)a255;
		f32 step      = 7.0f * w;
		f32 base      = len * (w - 3.6f) - 117.6f;
		u32 blueT     = da | 0xCECEFF00;
		u32 blueB     = da | 0xE037EE00;
		u32 goldT     = da | 0xEFEBC500;
		u32 goldB     = da | 0xEDA83600;
		u32 pinkB     = da | 0x5959FF00;
		tex.mMtxDirty = TRUE;
		for (int i = 0; i < 7; i++) {
			s32 v = (s32)(self->_1C[0] / Pow10(i));
			if (i != 0 && v == 0) {
				continue;
			}
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * (v % 10), 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(GS_TEV, 0);
			u32 top;
			u32 bottom;
			switch (self->mId) {
			case 0x9A:
				top    = blueT;
				bottom = blueB;
				break;
			case 0x98:
				top    = goldT;
				bottom = goldB;
				break;
			case 0x99:
				top    = blueT;
				bottom = pinkB;
				break;
			}
			fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, base + (step + (dx - i * (w - 3.6f))), dy - 2.0f, 0.0f, w, h, 0.0f,
			            0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_8019AFA0(GS_TEV, 4);
	}
}
