#include "Version.h"
#include "game/cgame.h"
#include "game/csound.h"
#include "game/soundtracks.h"
#include "game/xsound.h"
#include <string.h>
extern "C" {
extern char sSoundReadOperation[5];
void InitAudio(void* owner, u32 heapSize, u32 voices, u32 groups);
s32 HSD_SynthSFXNewBank(s32 size);
void HSD_JObjSetJSoundCallback(void (*callback)(s32));
void PlayGameSfxAtFullVolume(s32 sfx);
s32 HSD_SynthSFXBankClear(s32 bank);
s32 HSD_SynthSFXLoad(const char* path, s32 bank, void (*callback)(s32, s32), s32 arg);
BOOL LoadSoundSampleMetadata(void* data);
void OSReport(const char*, ...);
void OSPanic(const char*, int, const char*, ...);
}

extern "C" {

// The loader addresses filenames and diagnostics from one MWCC constant pool.
#define SOUND_BANK_READER
#include "game/soundbankpaths.inc"
} // extern "C"

// drops every sfx name waiting to load and empties the stage bank
static inline void ClearStageBank(XSound* sound)
{
	sound->mStageRequests.Clear();
	sound->mLoadRequestCount = 0;
	HSD_SynthSFXBankClear(sound->mStageBank);
}

#include "game/soundbankregister.inl"

/**
 * @note Address: 0x8001F8D8
 * @note Size: 0xA4
 */
extern "C" void ClearStageSoundBank(XSound* sound)
{
	ClearStageBank(sound);
}

/**
 * @note Address: 0x8001F97C
 * @note Size: 0x2D8
 */
// a negative stage loads the shared samples a step per call, TRUE once they're in
extern "C" BOOL LoadGameSoundBank(XSound* sound, s32 stage)
{
	if (stage < 0) {
		switch (sound->mLoadState) {
		case 0:
			sound->mSampleReady = 0;
			if (sound->mFile.ReadAsync(sSoundSampleSemPath, 0, 0, FALSE) >= 0) {
				++sound->mLoadState;
			} else {
#if defined(VERSION_JP)
				OSPanic(sSoundSourceFile, 0xED, "%s%s", "", sSoundReadOperation);
#else
				OSPanic(sSoundSourceFile, 0xF1, "%s%s", "", sSoundReadOperation);
#endif
			}
			break;
		case 1: {
			s32 result = sound->mFile.GetResult();
			if (result == 0) {
				++sound->mLoadState;
			} else if (result < 0) {
#if defined(VERSION_JP)
				OSPanic(sSoundSourceFile, 0xF5, "%s%s", "", sSoundReadOperation);
#else
				OSPanic(sSoundSourceFile, 0xF9, "%s%s", "", sSoundReadOperation);
#endif
			}
			break;
		}
		case 2:
			LoadSoundSampleMetadata(sound->mFile.mBuffer);
			HSD_SynthSFXLoad(sSoundSampleSsmPath, sound->mSampleBank, NULL, 0);
			++sound->mLoadState;
			break;
		case 3:
			if (!HasPendingSoundBankLoad()) {
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
	OSReport(">>>%s\n", stageSfxTable[stage]);
#endif
	sound->registerName(stageSfxTable[stage], sound->mStageBank);
	return TRUE;
}

/**
 * @note Address: 0x8001FC54
 * @note Size: 0x68
 */
extern "C" void InitGameSound(XSound* sound)
{
	InitAudio(sound, 0x700000, 0x60, 0x20);
	sound->mSampleBank  = HSD_SynthSFXNewBank(0x480000);
	sound->mStageBank   = HSD_SynthSFXNewBank(0x280000);
	sound->mLoadState   = 0;
	sound->mSampleReady = 0;
	HSD_JObjSetJSoundCallback(PlayGameSfxAtFullVolume);
}

/**
 * @note Address: 0x8001FCBC
 * @note Size: 0x38
 */
// particle sounds from joint anims
extern "C" void PlayGameSfxAtFullVolume(s32 sfx)
{
	PlayGameSound(&gGame.mSound, sfx, 255);
}
