#include "game/cgame.h"
#include "game/soundtracks.h"
#include "game/soundplayback.h"

// a negative track means the default one
inline s32 XSound::resolveTrack(s32 track)
{
	if (track < 0) {
		track = mDefaultTrack;
	}
	return track;
}

// a music id's volume from musicTable, 0 for anything that isn't music
static inline u8 getMusicVolume(s32 sfx)
{
	if (!isAllSfx(sfx)) {
		return 0;
	}
	u8 idx = sfx;
	if (idx > 0x6D) {
		idx = 0;
	}
	return musicTable[idx].volume;
}

/**
 * @note Address: 0x8001E11C
 * @note Size: 0x88
 */
extern "C" void SetMusicPlaybackPaused(XSound* sound, BOOL on)
{
	if (on) {
		XSound* game = (XSound*)&gGame.mSound;
		AXDriverPauseTrack(game->resolveTrack(0));
		AXDriverPauseAllStreams();
		game->mDefaultTrack = 1;
	} else {
		XSound* game = (XSound*)&gGame.mSound;
		AXDriverResumeTrack(game->resolveTrack(0));
		AXDriverResumeAllStreams();
		game->mDefaultTrack = 0;
	}
}

/**
 * @note Address: 0x8001E1A4
 * @note Size: 0x128
 */
extern "C" void FadeOutGameSfx(XSound* sound, s32 sfx, u16 frames)
{
	if (isAllSfx(sfx)) {
		sound->mMusicFadeOutFrames = (60.0f * frames) / 200.0f;
		setSfxVolume(sound, sfx, 0, frames);
	} else {
		setSfxVolume(sound, sfx, 0, frames);
	}
}

/**
 * @note Address: 0x8001E2CC
 * @note Size: 0x19C
 */
extern "C" void FadeInGameSfx(XSound* sound, s32 sfx, u16 frames)
{
	const u32 soundId = sfx;
	if (isAllSfx(soundId)) {
		PlayGameSound(sound, soundId, 0);
		setSfxVolume(sound, soundId, getMusicVolume(soundId), frames);
	} else {
		if (!IsGameSoundPlaying(sound, soundId)) {
			PlayGameSound(sound, soundId, 0xFF);
		}
		setSfxVolume(sound, soundId, 0xFF, frames);
	}
}
