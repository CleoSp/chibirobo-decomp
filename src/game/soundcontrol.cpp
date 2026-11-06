#include "game/soundplayback.h"

/**
 * @note Address: 0x8001E468
 * @note Size: 0x6C
 */
extern "C" void SetGameSfxVolume(XSound* sound, s32 sfx, u8 vol, u16 frames)
{
	setSfxVolume(sound, sfx, vol, frames);
}

/**
 * @note Address: 0x8001E4D4
 * @note Size: 0x30
 */
extern "C" void ResumeGameSoundTrack(XSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->mDefaultTrack;
	}
	AXDriverResumeTrack(track);
}

/**
 * @note Address: 0x8001E504
 * @note Size: 0x8C
 */
extern "C" void ResumeGameSfx(XSound* sound, s32 sfx)
{
	if (isAllSfx(sfx)) {
		if (sound->mPausedMusicId == sound->mCurrentMusicId) {
			AXDriverResumeAllStreams();
		} else {
			PlayGameSound(sound, sfx, 0xFF);
		}
		sound->mPausedMusicId = 0;
	} else {
		AXDriverResumeSfx(sfx);
	}
}

/**
 * @note Address: 0x8001E590
 * @note Size: 0x30
 */
extern "C" void PauseGameSoundTrack(XSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->mDefaultTrack;
	}
	AXDriverPauseTrack(track);
}

/**
 * @note Address: 0x8001E5C0
 * @note Size: 0x70
 */
extern "C" void PauseGameMusic(XSound* sound, s32 sfx, s32 resumeDelay)
{
	if (isAllSfx(sfx)) {
		PauseMusic(sound, resumeDelay);
	}
}

/**
 * @note Address: 0x8001E630
 * @note Size: 0x30
 */
extern "C" void KeyOffGameSoundTrack(XSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->mDefaultTrack;
	}
	HSD_AudioSFXKeyOffTrack(track);
}

/**
 * @note Address: 0x8001E660
 * @note Size: 0x68
 */
extern "C" BOOL StopGameSfx(XSound* sound, s32 sfx)
{
	return stopSfx(sound, sfx);
}

/**
 * @note Address: 0x8001E6C8
 * @note Size: 0x23C
 */
extern "C" BOOL IsGameSoundPlaying(XSound* sound, s32 sfx)
{
	// Only even-numbered slots are checked.
	for (u32 i = 0; i < 32 && sound->_108[i] != 0; i += 2) {
		if (sound->_108[i] == sfx) {
			return TRUE;
		}
	}

	const u32 soundId = sfx;
	if (isAllSfx(sfx)) {
		if (sound->mCurrentMusicId == sfx) {
			return TRUE;
		}
		if (sfx == 0xFFFF0000 && sound->mCurrentMusicId != 0) {
			return TRUE;
		}
		return FALSE;
	}

	switch (sfx) {
	case 0x1B:
		if (HSD_AudioSFXCountByID(soundId) || HSD_AudioSFXCountByID(sfx + 1) || HSD_AudioSFXCountByID(sfx + 2)
		    || HSD_AudioSFXCountByID(sfx + 3)) {
			return TRUE;
		}
		return FALSE;
	case 0xFD:
		if (HSD_AudioSFXCountByID(0xFD) || HSD_AudioSFXCountByID(0xFE) || HSD_AudioSFXCountByID(0xFF) || HSD_AudioSFXCountByID(0x100)
		    || HSD_AudioSFXCountByID(0x101) || HSD_AudioSFXCountByID(0x102) || HSD_AudioSFXCountByID(0x103) || HSD_AudioSFXCountByID(0x104)
		    || HSD_AudioSFXCountByID(0x105) || HSD_AudioSFXCountByID(0x106) || HSD_AudioSFXCountByID(0x107) || HSD_AudioSFXCountByID(0x108)
		    || HSD_AudioSFXCountByID(0x109) || HSD_AudioSFXCountByID(0x10A) || HSD_AudioSFXCountByID(0x10B)) {
			return TRUE;
		}
		return FALSE;
	default:
		return HSD_AudioSFXCountByID(soundId);
	}
}
