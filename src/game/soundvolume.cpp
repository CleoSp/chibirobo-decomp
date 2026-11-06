#include "game/cgame.h"
#include "game/xsound.h"

extern "C" void HSD_SynthSetSoundMode(s32 mode);

// the group lists sit in the sound data block, the two entry one is in small data
extern "C" u8 lbl_802107A0[];
extern "C" s32 sMessageGroup14SoundIds[2];
extern "C" s32 HSD_Randi(s32 max);

/**
 * @note Address: 0x8001DC20
 * @note Size: 0x2E0
 */
extern "C" void PlayMessageGlyphSound(XSound* sound, u32 group, f32 strength, u8 first, u8 second)
{
	u8* data   = lbl_802107A0;
	u32 volume = 255.0f * strength;
	if (strength < 0.5f) {
		volume *= 0.2f;
	} else if (strength < 1.0f) {
		volume *= 0.6;
	}

	s32* ids;
	s32 count;
	switch (group) {
	case 0:
		ids   = (s32*)(data + 0x6C4);
		count = 7;
		break;
	case 1:
		ids   = (s32*)(data + 0x9AC);
		count = 5;
		break;
	case 2:
		ids   = (s32*)(data + 0x6E0);
		count = 10;
		break;
	case 3:
		ids   = (s32*)(data + 0x730);
		count = 10;
		break;
	case 4:
		ids   = (s32*)(data + 0x708);
		count = 10;
		break;
	case 7:
		ids   = (s32*)(data + 0x794);
		count = 10;
		break;
	case 8:
		ids   = (s32*)(data + 0x780);
		count = 5;
		break;
	case 9:
		ids   = (s32*)(data + 0x758);
		count = 10;
		break;
	case 10:
		ids   = (s32*)(data + 0x7BC);
		count = 5;
		break;
	case 11:
		ids   = (s32*)(data + 0x7E8);
		count = 5;
		break;
	case 12:
		ids   = (s32*)(data + 0x7FC);
		count = 6;
		break;
	case 13:
		ids   = (s32*)(data + 0x814);
		count = 10;
		break;
	case 14:
		ids   = sMessageGroup14SoundIds;
		count = 2;
		break;
	case 15:
		ids   = (s32*)(data + 0x83C);
		count = 12;
		break;
	case 16:
		ids   = (s32*)(data + 0x86C);
		count = 10;
		break;
	case 17:
		ids   = (s32*)(data + 0x894);
		count = 10;
		break;
	case 18:
		ids   = (s32*)(data + 0x984);
		count = 10;
		break;
	case 19:
		ids   = (s32*)(data + 0x8BC);
		count = 10;
		break;
	case 20:
		ids   = (s32*)(data + 0x8E4);
		count = 10;
		break;
	case 21:
		ids   = (s32*)(data + 0x90C);
		count = 10;
		break;
	case 22:
		ids   = (s32*)(data + 0x934);
		count = 10;
		break;
	case 23:
		ids   = (s32*)(data + 0x95C);
		count = 10;
		break;
	case 24:
		ids   = (s32*)(data + 0x9C0);
		count = 6;
		break;
	case 25:
		ids   = (s32*)(data + 0xA00);
		count = 11;
		break;
	case 26:
		ids   = (s32*)(data + 0xA2C);
		count = 12;
		break;
	case 27:
		ids   = (s32*)(data + 0x9D8);
		count = 10;
		break;
	case 28:
		ids   = (s32*)(data + 0xA5C);
		count = 6;
		break;
	case 29:
		ids   = (s32*)(data + 0x7D0);
		count = 6;
		break;
	case 30:
		ids   = (s32*)(data + 0xA74);
		count = 8;
		break;
	case 31:
		ids   = (s32*)(data + 0xA94);
		count = 4;
		break;
	case 32:
		ids   = (s32*)(data + 0xAA4);
		count = 4;
		break;
	case 33:
		ids   = (s32*)(data + 0xAB4);
		count = 6;
		break;
	default:
		return;
	}
	if (count > 0) {
		for (s32 i = 0; i < count; i++) {
			if (IsGameSoundPlaying(sound, ids[i])) {
				return;
			}
		}
		s32 index = HSD_Randi(count);
		if (volume >= 512) {
			volume = 511;
		}
		PlayGameSound(sound, ids[index], volume & 0xFF);
		if (volume > 255) {
			PlayGameSound(sound, ids[index], 255);
		}
	}
}

/**
 * @note Address: 0x8001DF00
 * @note Size: 0x10C
 */
extern "C" u8 GetSoundVolumeAtPosition(XSound* sound, Vec* pos, s32 kind)
{
	f32 dist   = PSVECDistance(&gGame.mMotionControlCamera.mCamera.mInterest, pos);
	f32* range = gSoundFalloffRanges[kind];

	if (range[0] < dist) {
		return 0;
	}
	if (range[1] < dist) {
		return 127.0f - (64.0f * (dist - range[1])) / (range[0] - range[1]);
	}
	if (range[2] < dist) {
		return 171.0f - (64.0f * (dist - range[2])) / (range[1] - range[2]);
	}
	if (range[3] < dist) {
		return 255.0f - (64.0f * (dist - range[3])) / (range[2] - range[3]);
	}
	return 255;
}

/**
 * @note Address: 0x8001E00C
 * @note Size: 0x38
 */
extern "C" void SetGameSoundStereo(XSound* sound, s32 stereo)
{
	if (stereo == 1) {
		HSD_SynthSetSoundMode(1);
	} else {
		HSD_SynthSetSoundMode(0);
	}
}
