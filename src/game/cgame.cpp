#include "game/particle_srt.h"
#include "game/cgame_shared.h"

/**
 * @note Address: 0x80007880
 * @note Size: 0x6BC
 */
CGame::~CGame()
{
}
extern "C" {
int strncmp(const char* a, const char* b, unsigned long n);
CTexObj* fn_8019E8F4(CFont* font, const char* str, s32* width);
void fn_80197800(CGs* gs, CTexObj* tex, u32 color0, u32 color1, u32 color2, u32 color3, f32 x, f32 y, f32 z, f32 w, f32 h, f32 rx, f32 ry,
                 f32 rz);
}
extern "C" f32 lbl_8020FC10[4];
extern "C" f32 lbl_8020FC20[4];
extern "C" {
void fn_8019B3E8(CGsRenderMode* mode, u8 enable, u8 value);
void fn_80198614(CGs* gs, f32 x0, f32 y0, f32 z0, u32 color0, f32 x1, f32 y1, f32 z1, u32 color1, f32 x2, f32 y2, f32 z2, u32 color2,
                 f32 x3, f32 y3, f32 z3, u32 color3);
}

// the player, only the objects it points at
struct UnkPlayer : public XObj {
	u8 _988[0x9E0 - 0x988]; // _988
	XObj* mShadowObj;       // _9E0, the one its shadow height comes from
	u8 _9E4[0xA48 - 0x9E4]; // _9E4
	s32 _A48;               // _A48, the action, 0 and 4 are idle
	u8 _A4C[0xA64 - 0xA4C]; // _A4C
	XObj* _A64;             // _A64
	u8 _A68[0xA94 - 0xA68]; // _A68
	XObj* _A94;             // _A94
	u8 _A98[0xAC4 - 0xA98]; // _A98
	s32 _AC4;               // _AC4
	u8 _AC8[0xB14 - 0xAC8]; // _AC8
	s32 _B14;               // _B14
	u8 _B18[0xB68 - 0xB18]; // _B18
	s32 _B68;               // _B68
	u8 _B6C[0xB84 - 0xB6C]; // _B6C
	XObj* _B84;             // _B84
	u8 _B88[0xBAC - 0xB88]; // _B88
	s32 _BAC;               // _BAC
	u8 _BB0[0xBC8 - 0xBB0]; // _BB0
	XObj* _BC8;             // _BC8
	u8 _BCC[0xBEC - 0xBCC]; // _BCC
	s32 _BEC;               // _BEC
	u8 _BF0[0xC08 - 0xBF0]; // _BF0
	XObj* _C08;             // _C08
	u8 _C0C[0xC44 - 0xC0C]; // _C0C
	s32 _C44;               // _C44

	bool IsIdle() const
	{
		bool idle = false;
		if ((_A48 == 0 || _A48 == 4) && _AC4 == 0 && _B14 == 0 && _B68 == 0 && _BAC == 0 && _BEC == 0 && _C44 == 0) {
			idle = true;
		}
		return idle;
	}

	XObj* GetShadowObj()
	{
		if (mShadowObj) {
			return mShadowObj;
		}
		return NULL;
	}
};

// the current stage's settings
struct UnkStageInfo {
	u8 _00[0x6];        // _00
	s16 mCameraMode;    // _06
	u8 mMirrorFade;     // _08
	u8 _09[0x10 - 0x9]; // _09
	u8 mFlags;          // _10, bit 0 keeps the shadows at full strength
};

struct UnkStageRef {
	UnkStageInfo* mInfo; // _00
};
extern "C" {
UnkStageRef* fn_8006F4B8(void);
void JObjClearMObjFlags(HSD_JObj* jobj, u32 flags);
void HSD_CObjSetInterest(HSD_CObj* cobj, Vec* interest);
void HSD_CObjSetEyePosition(HSD_CObj* cobj, Vec* eye);
void HSD_ShadowDeleteObject(HSD_Shadow* shadow, HSD_JObj* jobj);
void fn_8000CBF4(HSD_JObj* jobj, f32* limits);
}

// an object that is drawn into the shadow, or onto it with the mask
static inline bool IsShadowObj(XObj* obj, u64 mask)
{
	bool on = false;
	if ((obj->_228.mFlags & 1) && (obj->_228.mFlags & 0x10000) && (obj->_228.mFlags & mask)) {
		on = true;
	}
	return on;
}
extern "C" void fn_8000CBF4(HSD_JObj* jobj, f32* limits);
extern "C" {
s32 DVDGetDriveStatus(void);
BOOL DVDCheckDisk(void);
void fn_801CDC34(void);
void AXQuit(void);
void AIReset(void);
void* HSD_VISetUserGXDrawDoneCallback(void* cb);
void GXDrawDone(void);
void GXAbortFrame(void);
void OSResetSystem(int reset, u32 code, BOOL forceMenu);
}

/**
 * @note Address: 0x800153BC
 * @note Size: 0x170
 */
// NONMATCHING: the pad pointer and the counter swap registers
// CBase::Update, keeps the rumble off while the disc can't be read and watches for the reset
// button or B X start held for a second
// CBase::Update, CGame::Update has it inlined
static inline void BaseUpdate(CBase* base, u32 frames)
{
	base->mLastDriveStatus = base->mDriveStatus;
	base->mDriveStatus     = DVDGetDriveStatus();
	base->mDiskIn          = DVDCheckDisk();
	switch (base->mDriveStatus) {
	case -1:
	case 4:
	case 5:
	case 6:
	case 11:
		if (base->mDriveStatus != base->mLastDriveStatus) {
			CPadEx* pad         = &base->mPad;
			base->mRumblePaused = TRUE;
			for (s32 i = 0; i < 4; i++) {
				if (pad->mPad[i].mConnected) {
					HSD_PadRumbleOffN(pad->mPad[i].mPort);
				}
			}
			HSD_PadRumblePauseAll();
		}
		break;
	case 0:
		if (base->mLastDriveStatus != 0 && base->mRumblePaused) {
			base->mRumblePaused = FALSE;
			fn_801CDC34();
		}
		break;
	}

	if (!base->mReset) {
		if (HSD_PadGetResetSwitch()) {
			base->mResetTimer = 0;
			base->mReset      = TRUE;
		} else if ((base->mPad.mPad[0].mButton & 0xFFFF) == 0x1600) {
			if ((base->mResetTimer += base->mFrames) > 60) {
				base->mResetTimer = 0;
				base->mReset      = TRUE;
			}
		} else if (!base->mReset) {
			base->mResetTimer = 0;
		}
	}
}

/**
 * @note Address: 0x800152A0
 * @note Size: 0x60
 */
// stops the sound and the video and resets
inline void ResetSystem(u32 code)
{
	AXQuit();
	AIReset();
	HSD_VISetUserGXDrawDoneCallback(NULL);
	GXDrawDone();
	GXAbortFrame();
	VISetBlack(TRUE);
	VIFlush();
	VIWaitForRetrace();
	OSResetSystem(code, 0, 0);
}

extern "C" void GXProject(f32 x, f32 y, f32 z, const Mtx mtx, const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);

// a menu, only as far as its inline methods show it. name from RTTI
class CMenu {
public:
	s32 mState;             // _00
	u8 _04[0x8];            // _04
	BOOL mOpen;             // _0C
	u8 _10[0x138 - 0x10];   // _10
	s32 _138;               // _138
	s32 _13C;               // _13C
	u8 _140[0x148 - 0x140]; // _140
	// _148 vtable

	virtual ~CMenu();     // _08
	virtual void vf0C();  // _0C
	virtual void vf10();  // _10
	virtual void vf14();  // _14
	virtual void vf18();  // _18
	virtual void Open();  // _1C
	virtual void vf20();  // _20
	virtual void vf24();  // _24
	virtual void vf28();  // _28
	virtual void Reset(); // _2C
};

// the microphone in each memory card slot, class unknown. the game only uses slot B
struct UnkSlot {
	s32 _00;       // _00, -3 once the mic is pulled out
	u32 mDeviceID; // _04
	s32 mState;    // _08, 0 probing, 1 mounting, 2 waiting for the mount, 3 mounted
	void* mWork;   // _0C, the mount's work area
};
extern "C" UnkSlot lbl_8020FC3C[];
extern "C" s32 lbl_80659ECC;
extern "C" {
s32 MICProbeEx(s32 chan);
s32 MICMountAsync(s32 chan, void* work, s32 size, void (*detach)(s32), void (*attach)(s32, s32));
s32 MICGetResultCode(s32 chan);
s32 MICGetDeviceID(s32 chan, u32* id);
s32 MICUnmount(s32 chan);
s32 M2SGetActiveChannel(void);
s32 M2SGetSamplesLeft(void);
void M2SGetSamples(s16** samples, s32 count);
void M2SAdvanceBuffer(s32 count);
void* memset(void* dst, int val, u32 size);
void DCFlushRange(void* addr, u32 size);
extern void* __OSCurrHeap;
void* OSAllocFromHeap(void* heap, u32 size);
void OSFreeToHeap(void* heap, void* ptr);
}

extern "C" s32 MICReset(s32 chan);

extern "C" s32 lbl_8020FC5C[3];
extern "C" s32 lbl_8020FC68[3];
extern "C" s32 lbl_8065B158;
extern "C" s32 lbl_8065B15C;
extern "C" s16* lbl_8065B160[2];
extern "C" u8 lbl_8065B168;
extern "C" u32 lbl_8065B16C[2];
extern "C" void (*lbl_8065B174)(void);
extern "C" {
u32 CheckStageFlag(s32 stage, u32 mask);
BOOL M2SSetActiveChannel(s32 chan);
BOOL M2SSetMode(s32 mode);
BOOL M2SSetPrerecordSamples(s32 milliseconds);
void M2SOpen(void);
void M2SClose(void);
void M2SStart(void);
void M2SStop(void);
s32 MICSetGain(s32 chan, s32 gain);
s32 MICStart(s32 chan);
s32 MICGetButton(s32 chan, u32* button);
s64 OSGetTime(void);
}
extern "C" BOOL lbl_8065B148;
extern "C" s64 lbl_8065B150;

// os.h's, which this file can't include
#define OSMillisecondsToTicks(msec) ((msec) * ((*(u32*)0x800000F8 / 4) / 1000))

// whether the stage takes the mic, which ended up true either way
static inline BOOL IsMicStage()
{
	if (CheckStageFlag(lbl_8025C400.mStage, 2)) {
		return TRUE;
	}
	return TRUE;
}
extern "C" {
void OSPanic(const char* file, int line, const char* message, ...);
BOOL OSCreateThread(void* thread, void* (*func)(void*), void* param, void* stack, u32 stackSize, s32 priority, u16 attr);
s32 OSResumeThread(void* thread);
}

// the thread the movie plays on and its stack
struct MovieThread {
	u8 mThread[0x318]; // _000, an OSThread
	void* mStack;      // _318
	u32 mStackSize;    // _31C
	u8 _320[0x8];      // _320
};
extern "C" MovieThread lbl_80360AE8;
extern "C" u8 lbl_80360E10[0x1000];

// what the command line can set
struct CmdLineArgs {
	s32 mStage;   // _00
	s32 mSlot;    // _04, the card slot, 0 for A
	u8 _08[0x18]; // _08
};
extern "C" CmdLineArgs lbl_8020FBC0;
extern "C" s32 fn_8006F654(const char* name);
extern "C" {
void fn_80023EE0(void* scrn, f32 time, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1, u8 a1);
void fn_800236EC(CScrnRoboeyes* eyes, s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9,
                 f32 arg10, f32 arg11);
void fn_80023438(void* scrn, f32 x0, f32 y0, f32 x1, f32 y1, f32 x2, f32 y2, f32 time, u8 r0, u8 g0, u8 b0, u8 a0, u8 r1, u8 g1, u8 b1,
                 u8 a1, s32 mode);
void fn_800242A8(void* step, f32 r0, f32 g0, f32 b0, f32 r1, f32 g1, f32 b1, f32 time);
s32 fn_8006E7BC(void);
s32 fn_80087A40(void* card);
void fn_80061364(void* menu);
void fn_8007F040(CEvt* evt, s32 a, s32 b);
void fn_800821DC(CMsg* msg);
void CMsg_ResetPos(CMsg* msg);
void fn_8006F544(void);
void fn_801328C8(XOBJS* objs);
void psKillAllParticle(void);
void fn_8001E11C(void* sound, BOOL pause);
void fn_8001E630(void* sound, s32 track);
void fn_8001F8D8(void* sound);
void fn_80066190(void* info, s32 a, s32 b);
void fn_80030CE8(void* title);
void fn_80031510(void* menu);
void fn_8006D5A4(void* info);
s32 GetStageNum(void);
u8* fn_8006DFC4(s32 index);
int rand(void);
}
extern "C" GXColor lbl_80659EC0;

// a lead byte of a two byte shift jis character
static inline bool IsSJISByte(char c)
{
	return ((u8)c >= 0x81 && (u8)c <= 0x9F) || ((u8)c >= 0xE0 && (u8)c <= 0xFC);
}

static inline bool IsSJISLead(char c)
{
	return IsSJISByte(c) ? true : false;
}

// resets a screen color to nothing at once
#define CLEAR_WIPE(wipe) fn_800236EC((CScrnRoboeyes*)(wipe), 0, 0, 0, 0, 1, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f)
extern "C" {
char* strtok(char* str, const char* delim);
int strcmp(const char* a, const char* b);
}
extern "C" {
s32 HSD_DevComIsBusy(s32 id);
void HSD_PadReset(void);
BOOL fn_80131D2C(void);
void fn_80088848(void* card);
void fn_8001E5C0(void* sound, u32 mask, s32 a);
void fn_8001E504(void* sound, u32 mask);
void* fn_8006F3EC(void);
void fn_8007EEA4(CEvt* evt, void* script, s32 a, s32 b, s32 c);
void CEvt_Update(CEvt* evt, u32 frames);
void CMsg_Update(CMsg* msg, u32 frames);
void fn_8001F46C(void* sound, u32 frames);
void fn_80023B38(void* scrn, u32 frames);
void fn_800240EC(void* step, u32 frames);
void fn_80027528(void* logo, u32 frames);
void fn_8002E8D0(void* title, u32 frames);
void fn_80031460(void* menu, u32 frames);
void fn_8006D548(void* info, u32 frames);
void fn_80014DA4(FbMirror* mirror, u32 frames);
void fn_80011270(CGame* game);
}
extern "C" {
void fn_8001146C(CGame* game);
void fn_80011270(CGame* game);
BOOL fn_80132B98(XOBJS* objs);
void* fn_8006F3CC(void);
void* fn_8006F3BC(void);
void fn_80141754(CInterfaceCameraAction* action);
void fn_800236EC(CScrnRoboeyes* eyes, s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, f32 arg9,
                 f32 arg10, f32 arg11);
void fn_801BA3AC(HSD_MObj* mobj, u8 r, u8 g, u8 b);
HSD_TObj* HSD_MObjGetTObj(HSD_MObj* mobj);
}

static inline HSD_JObj* GetChild(HSD_JObj* jobj)
{
	return jobj == NULL ? NULL : jobj->child;
}

// the last joint down the first children
static inline HSD_JObj* GetLeaf(HSD_JObj* jobj)
{
	while (GetChild(jobj) != NULL) {
		jobj = GetChild(jobj);
	}
	return jobj;
}
extern "C" void fn_80011270(CGame* game);
extern "C" void fn_8001146C(CGame* game);

// copies what was drawn into the effect's texture
static inline void CopyToTexture(CFbTexEff* effect)
{
	fn_8019B648(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B5D0(lbl_8025C400.mGs.mCurRenderMode, 1);
	fn_8019B464(lbl_8025C400.mGs.mCurRenderMode, 0, 7, 1);
	SetupAll(TRUE);
	if (effect->mBuffer != NULL) {
		u16 width, height;
		if (effect->_18) {
			width  = effect->mWidth << 1;
			height = effect->mHeight << 1;
		} else {
			width  = effect->mWidth;
			height = effect->mHeight;
		}
		GXSetTexCopySrc(0, 0, width, height);
		GXSetTexCopyDst(effect->mWidth, effect->mHeight, effect->mFormat, effect->_18);
		GXCopyTex(effect->mBuffer, GX_TRUE);
		GXPixModeSync();
		InvalidateAll();
	}
}
extern "C" {
void PSMTXReflect(Mtx m, const Vec* p, const Vec* n);
void PSMTXTranspose(const Mtx src, Mtx xPose);
}
extern "C" void fn_80014DA4(FbMirror* mirror, u32 frames);
