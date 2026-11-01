#include <types.h>
#include <string.h>
#include <dolphin/os.h>
#include "Dolphin/mic.h"
#include "Dolphin/m2s.h"
#include "game/mic.h"

extern "C" {
// resampling steps and the buffer fill levels that pick them
extern u32 lbl_8020FC5C[3];
extern s32 lbl_8020FC68[3];
// the loudest samples seen
extern s32 lbl_8065B158;
extern s32 lbl_8065B15C;
// the two output buffers and which one is next
extern void* lbl_8065B160[2];
extern u8 lbl_8065B168;
// the resampling phase per m2s channel
extern u32 lbl_8065B16C[2];
extern void (*lbl_8065B174)();
void fn_8000A858(s32 channel, s32 result);
}

/**
 * @note Address: 0x8000A560
 * @note Size: 0x118
 */
// fills one 0x280 byte output buffer from the m2s stream
extern "C" void fn_8000A560(void* buffer)
{
	memset(buffer, 0, 0x280);
	u32 channelOffset = M2SGetActiveChannel() * sizeof(u32);
	// todo: cleanup, indexing lbl_8065B16C kills the match
	u8* phases    = (u8*)lbl_8065B16C;
	s32 remaining = M2SGetSamplesLeft();
	if (remaining < 0) {
		*(u32*)(phases + channelOffset) = 0;
	} else if (remaining >= lbl_8020FC68[0]) {
		u32 step;
		if (remaining < lbl_8020FC68[1])
			step = lbl_8020FC5C[0];
		else if (remaining < lbl_8020FC68[2])
			step = lbl_8020FC5C[1];
		else
			step = lbl_8020FC5C[2];
		u32 phase = *(u32*)(phases + channelOffset);
		phase += step * 159;
		s32 count = phase >> 16;
		s16* samples;
		M2SGetSamples(&samples, count + 4);
		s16* current = samples;
		for (s32 i = 0; i < count + 4; ++i) {
			s32 sample = *current;
			if (sample > lbl_8065B15C)
				lbl_8065B15C = sample;
			if (sample > 1000 && sample > lbl_8065B158)
				lbl_8065B158 = sample;
			++current;
		}
		phase += step;
		*(u32*)(phases + channelOffset) = phase & 0xFFFF;
		M2SAdvanceBuffer(phase >> 16);
	}
}

/**
 * @note Address: 0x8000A678
 * @note Size: 0x60
 */
extern "C" void fn_8000A678()
{
	lbl_8065B174();
	fn_8000A560(lbl_8065B160[lbl_8065B168]);
	DCFlushRange(lbl_8065B160[lbl_8065B168], 0x280);
	lbl_8065B168 ^= 1;
}

// todo: cleanup
static inline void FreeMicrophone(MicChannel* channels)
{
	OSFree(channels[1].mBuffer);
	channels[1].mBuffer = NULL;
	channels[1].mState  = 0;
}

/**
 * @note Address: 0x8000A6D8
 * @note Size: 0x180
 */
// brings up the mic in slot b, a step per call
extern "C" void fn_8000A6D8()
{
	s32 result;
	MicChannel* channels = lbl_8020FC3C;
	switch (channels[1].mState) {
	case 0:
		if (MICProbeEx(1) == MIC_RESULT_READY)
			channels[1].mState = 1;
		break;
	case 1:
		channels[1].mBuffer = (s16*)OSAlloc(0x3000);
		result              = MICMountAsync(1, channels[1].mBuffer, 0x3000, fn_8000A858, NULL);
		switch (result) {
		case MIC_RESULT_READY:
			channels[1].mState = 2;
			break;
		case MIC_RESULT_BUSY:
			break;
		default:
			goto error;
		}
		break;
	case 2:
		result = MICGetResultCode(1);
		switch (result) {
		case MIC_RESULT_READY:
			MICGetDeviceID(1, &channels[1].mDeviceID);
			channels[1].mResult = 0;
			channels[1].mState  = 3;
			break;
		case MIC_RESULT_BUSY:
			break;
		default:
			goto error;
		}
		break;
	case 3:
		if (channels[1].mResult != 0) {
			MICUnmount(1);
			FreeMicrophone(channels);
		}
		break;
	}
	return;
error:
	switch (result) {
	case MIC_RESULT_WRONGDEVICE:
		FreeMicrophone(channels);
		break;
	case MIC_RESULT_NOCARD:
		FreeMicrophone(channels);
		break;
	}
}

/**
 * @note Address: 0x8000A858
 * @note Size: 0x18
 */
// the mount callback, any result means the mic went away
extern "C" void fn_8000A858(s32 channel, s32 result)
{
	lbl_8020FC3C[channel].mResult = MIC_RESULT_NOCARD;
}
