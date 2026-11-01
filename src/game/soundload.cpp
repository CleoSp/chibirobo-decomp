#include "game/cfile.h"
#include "Version.h"
#include <string.h>
#include "game/csound.h"
#include "game/cgame.h"

struct _HSD_JObj;

// sound.cpp's rodata, the strings are reached from its start. europe drops the
// OSReport strings, japan's float tables are 4 shorter
struct SoundRodata {
#if defined(VERSION_PAL)
	u8 _000[0xAD4]; // _000
#else
#if defined(VERSION_JP)
	u8 _000[0xAD0]; // _000
#else
	u8 _000[0xAD4]; // _000
#endif
	char mRegisterMsg[0x10]; // _AD4, "Register:%s\n"
	char mFullMsg[0x54];     // _AE4
#endif
	char mSemName[0x10]; // _B38, _AD4 in europe, "sfx_sample.sem"
	char mFileName[0xC]; // _B48, "sound.cpp"
	char mSsmName[0x10]; // _B54, "sfx_sample.ssm"
};

extern "C" {
void OSReport(const char* format, ...);
void OSPanic(const char* file, int line, const char* format, ...);
extern const SoundRodata lbl_801FE2B0;
extern const char* stageSfxTable[];
extern const char lbl_8065C0C4[8]; // "%s%s"
extern const char lbl_8065C0CC[4]; // ""
extern const char lbl_8065A1B8[8]; // "Read"
extern const char lbl_8065C0D0[8]; // ">>>%s\n"
void InitAudio(void* owner, u32 heapSize, u32 voices, u32 groups);
s32 HSD_SynthSFXNewBank(s32 size);
s32 HSD_SynthSFXBankClear(s32 bank);
s32 HSD_SynthSFXGetPendingLoadCount();
s32 HSD_SynthSFXLoad(const char* name, s32 bank, void (*callback)(s32, s32), s32 arg);
BOOL fn_801E8C18(void* data);
void HSD_JObjSetDPtclCallback(void (*callback)(int, int, int, _HSD_JObj*));
void fn_8001FCBC(int sfx, int low, int high, _HSD_JObj* joint);
}

// todo: cleanup
static inline void ClearStageBank(XSound* sound)
{
	for (u32 i = 0; i < 16; ++i) {
		sound->mLoadRequests[i].mName   = NULL;
		sound->mLoadRequests[i].mResult = -1;
		sound->mLoadRequests[i].mLoaded = 0;
	}
	sound->mLoadRequestCount = 0;
	HSD_SynthSFXBankClear(sound->mStageBank);
}

// todo: cleanup, sound.cpp has the same thing as CSound::registerName
static inline void RegisterName(XSound* sound, const char* name, const SoundRodata* strings)
{
	s32 bank = sound->mStageBank;
	if (name == NULL) {
		return;
	}
	for (u32 i = 0; i < 16; ++i) {
		if (sound->mLoadRequests[i].mName != NULL && strcmp(sound->mLoadRequests[i].mName, name) == 0) {
			return;
		}
	}
	for (u32 i = 0; i < 16; ++i) {
		if (sound->mLoadRequests[i].mName == NULL) {
			++sound->mLoadRequestCount;
			sound->mLoadRequests[i].mName = name;
			sound->mLoadRequests[i].mBank = bank;
#if !defined(VERSION_PAL)
			OSReport(strings->mRegisterMsg, name);
#endif
			return;
		}
	}
#if !defined(VERSION_PAL)
	OSReport(strings->mFullMsg);
#endif
}

/**
 * @note Address: 0x8001F8D8
 * @note Size: 0xA4
 */
extern "C" void fn_8001F8D8(XSound* sound)
{
	ClearStageBank(sound);
}

/**
 * @note Address: 0x8001F97C
 * @note Size: 0x2D8
 */
// a negative stage loads the shared samples a step per call, TRUE once they're in
extern "C" BOOL fn_8001F97C(XSound* sound, s32 stage)
{
	const SoundRodata* strings = &lbl_801FE2B0;
	if (stage < 0) {
		switch (sound->mLoadState) {
		case 0:
			sound->mSampleReady = 0;
			if (((CFile*)sound->mFile)->ReadAsync(strings->mSemName, 0, 0, FALSE) >= 0) {
				++sound->mLoadState;
			} else {
#if defined(VERSION_JP)
				OSPanic(strings->mFileName, 0xED, lbl_8065C0C4, lbl_8065C0CC, lbl_8065A1B8);
#else
				OSPanic(strings->mFileName, 0xF1, lbl_8065C0C4, lbl_8065C0CC, lbl_8065A1B8);
#endif
			}
			break;
		case 1: {
			s32 result = ((CFile*)sound->mFile)->GetResult();
			if (result == 0) {
				++sound->mLoadState;
			} else if (result < 0) {
#if defined(VERSION_JP)
				OSPanic(strings->mFileName, 0xF5, lbl_8065C0C4, lbl_8065C0CC, lbl_8065A1B8);
#else
				OSPanic(strings->mFileName, 0xF9, lbl_8065C0C4, lbl_8065C0CC, lbl_8065A1B8);
#endif
			}
			break;
		}
		case 2:
			fn_801E8C18(((CFile*)sound->mFile)->mBuffer);
			HSD_SynthSFXLoad(strings->mSsmName, sound->mSampleBank, NULL, 0);
			++sound->mLoadState;
			break;
		case 3:
			if (!(bool)HSD_SynthSFXGetPendingLoadCount()) {
				sound->mLoadState   = 0;
				sound->mSampleReady = 1;
				return TRUE;
			}
			break;
		default:
			sound->mLoadState = 0;
			break;
		}
		return FALSE;
	}
	sound->mDefaultTrack = 0;
	ClearStageBank(sound);
#if !defined(VERSION_PAL)
	OSReport(lbl_8065C0D0, stageSfxTable[stage]);
#endif
	RegisterName(sound, stageSfxTable[stage], strings);
	return TRUE;
}

/**
 * @note Address: 0x8001FC54
 * @note Size: 0x68
 */
extern "C" void fn_8001FC54(XSound* sound)
{
	InitAudio(sound, 0x700000, 0x60, 0x20);
	sound->mSampleBank  = HSD_SynthSFXNewBank(0x480000);
	sound->mStageBank   = HSD_SynthSFXNewBank(0x280000);
	sound->mLoadState   = 0;
	sound->mSampleReady = 0;
	HSD_JObjSetDPtclCallback(fn_8001FCBC);
}

/**
 * @note Address: 0x8001FCBC
 * @note Size: 0x38
 */
// particle sounds from joint anims
extern "C" void fn_8001FCBC(int sfx, int low, int high, _HSD_JObj* joint)
{
	fn_8001E904((CSound*)&lbl_8025C400.mSound, sfx, 255);
}
