#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

static AXSPB __AXStudio ATTRIBUTE_ALIGN(32);

static s32 __AXSpbAL;
static s32 __AXSpbAR;
static s32 __AXSpbAS;
static s32 __AXSpbAAL;
static s32 __AXSpbAAR;
static s32 __AXSpbAAS;
static s32 __AXSpbABL;
static s32 __AXSpbABR;
static s32 __AXSpbABS;

/**
 * @note Address: 0x80178940
 * @note Size: 0xC
 */
u32 __AXGetStudio(void) {
    return (u32)&__AXStudio;
}

#define AX_MAX_DEPOP_DELTA 20

// The DSP keeps the initial volume as a hi/lo halfword pair, written here as one word.
static void AXDepopFade(s32* hostSum, s32* dspVolume, s16* dspDelta) {
    s32 delta = *hostSum / AX_SAMPLES_PER_FRAME;

    if (delta != 0) {
        if (delta > AX_MAX_DEPOP_DELTA) {
            delta = AX_MAX_DEPOP_DELTA;
        }
        if (delta < -AX_MAX_DEPOP_DELTA) {
            delta = -AX_MAX_DEPOP_DELTA;
        }

        *dspVolume = *hostSum;
        *hostSum -= delta * AX_SAMPLES_PER_FRAME;
        *dspDelta = -delta;
        return;
    }

    *hostSum = 0;
    *dspVolume = 0;
    *dspDelta = 0;
}

/**
 * @note Address: 0x8017894C
 * @note Size: 0x3F8
 */
void __AXPrintStudio(void) {
    AXDepopFade(&__AXSpbAL, (s32*)&__AXStudio.dpopLHi, &__AXStudio.dpopLDelta);
    AXDepopFade(&__AXSpbAR, (s32*)&__AXStudio.dpopRHi, &__AXStudio.dpopRDelta);
    AXDepopFade(&__AXSpbAS, (s32*)&__AXStudio.dpopSHi, &__AXStudio.dpopSDelta);
    AXDepopFade(&__AXSpbAAL, (s32*)&__AXStudio.dpopALHi, &__AXStudio.dpopALDelta);
    AXDepopFade(&__AXSpbAAR, (s32*)&__AXStudio.dpopARHi, &__AXStudio.dpopARDelta);
    AXDepopFade(&__AXSpbAAS, (s32*)&__AXStudio.dpopASHi, &__AXStudio.dpopASDelta);
    AXDepopFade(&__AXSpbABL, (s32*)&__AXStudio.dpopBLHi, &__AXStudio.dpopBLDelta);
    AXDepopFade(&__AXSpbABR, (s32*)&__AXStudio.dpopBRHi, &__AXStudio.dpopBRDelta);
    AXDepopFade(&__AXSpbABS, (s32*)&__AXStudio.dpopBSHi, &__AXStudio.dpopBSDelta);
    DCFlushRange(&__AXStudio, sizeof(__AXStudio));
}

/**
 * @note Address: 0x80178D44
 * @note Size: 0x2C
 */
void __AXSPBInit(void) {
#if DEBUG
    OSReport("Initializing AXSPB code module\n");
#endif
    __AXSpbAL = 
    __AXSpbAR = 
    __AXSpbAS = 
    __AXSpbAAL = 
    __AXSpbAAR = 
    __AXSpbAAS = 
    __AXSpbABL = 
    __AXSpbABR = 
    __AXSpbABS = 0;
}

/**
 * @note Address: 0x80178D70
 * @note Size: 0x4
 */
void __AXSPBQuit(void) {
#if DEBUG
    OSReport("Shutting down AXSPB code module\n");
#endif
}

/**
 * @note Address: 0x80178D74
 * @note Size: 0x94
 */
void __AXDepopVoice(AXPB* p) {
    __AXSpbAL += p->dpop.aL;
    __AXSpbAAL += p->dpop.aAuxAL;
    __AXSpbABL += p->dpop.aAuxBL;
    __AXSpbAR += p->dpop.aR;
    __AXSpbAAR += p->dpop.aAuxAR;
    __AXSpbABR += p->dpop.aAuxBR;
    __AXSpbAS += p->dpop.aS;
    __AXSpbAAS += p->dpop.aAuxAS;
    __AXSpbABS += p->dpop.aAuxBS;
}
