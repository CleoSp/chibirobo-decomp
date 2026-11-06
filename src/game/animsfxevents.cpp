#include "Version.h"
#include "game/xsound.h"

BOOL AnimSfx::CheckStart(AnimSfxEvent* event, f32 min, f32 max)
{
	f32 a = event->mStart;
	f32 b = event->mEnd;

	switch (event->mKind) {
	case 1:
		if (0.0f <= a && min <= a && a < max) {
			return TRUE;
		}
		if (0.0f <= b && min <= b && b < max) {
			return TRUE;
		}
		break;
	}
	return FALSE;
}

#if defined(VERSION_JP)
extern "C" void SetSoundMenuSelection(XSound* sound, s32 index, s32 selection)
{
	if (index < 0 || index >= ARRAY_SIZE(gSoundMenuSelections))
		return;
	if (selection < 0 || selection >= ARRAY_SIZE(gSoundMenuEffects))
		return;
	PlayGameSound(sound, gSoundMenuEffects[selection], 0xFF);
	gSoundMenuSelections[index] = selection;
}

extern "C" s32 GetSoundMenuSelection(XSound*, s32 index)
{
	if (index < 0 || index >= ARRAY_SIZE(gSoundMenuSelections))
		return -1;
	return gSoundMenuSelections[index];
}
#endif

/**
 * @note Address: 0x8001DC10
 * @note Size: 0x10
 */
// empties the sound's queue
extern "C" void ClearGameSoundQueue(XSound* sound)
{
	sound->mQueueWrite = 0;
	sound->mQueueRead  = 0;
}
