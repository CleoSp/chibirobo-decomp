#include "game/mic.h"

extern "C" {
char sMovieUpdateName[]           = "UpdateMovie";
MicChannel gMicrophoneChannels[2] = { { -3 }, { -3 } };
s32 sMicSamplePhaseSteps[]        = { 0x581D, 0x581F, 0x5821 };
s32 sMicBufferThresholds[]        = { 80, 160, 240 };

BOOL gMicRecording;
s64 gMicButtonReleaseTime;
s32 gMicObjectPeak;
s32 gMicMeterPeak;
s16* gMicDMABuffers[2];
u8 gMicDMABufferIndex;
u32 gMicSamplePhase[2];
void (*gPreviousAudioDMACallback)(void);
}
