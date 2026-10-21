#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

static s32 __AXBufferAuxA[3][480] ATTRIBUTE_ALIGN(32);
static s32 __AXBufferAuxB[3][480] ATTRIBUTE_ALIGN(32);

static void (* __AXCallbackAuxA)(void*, void*);
static void (* __AXCallbackAuxB)(void*, void*);
static void* __AXContextAuxA;
static void* __AXContextAuxB;
static s32* __AXAuxADspWrite;
static s32* __AXAuxADspRead;
static s32* __AXAuxBDspWrite;
static s32* __AXAuxBDspRead;
static u32 __AXAuxDspWritePosition;
static u32 __AXAuxDspReadPosition;
static u32 __AXAuxDspWritePositionDpl2;
static u32 __AXAuxDspReadPositionDpl2;
static u32 __AXAuxCpuReadWritePosition;

/**
 * @note Address: 0x80177564
 * @note Size: 0xE4
 */
void __AXAuxInit(void) {
    int i;
    s32* pA;
    s32* pB;

#if DEBUG
    OSReport("Initializing AXAux code module\n");
#endif
    __AXCallbackAuxA = NULL;
    __AXCallbackAuxB = NULL;
    __AXContextAuxA = 0;
    __AXContextAuxB = 0;
    __AXAuxDspWritePosition = 0;
    __AXAuxDspReadPosition = 1;
    __AXAuxDspWritePositionDpl2 = 0;
    __AXAuxDspReadPositionDpl2 = 1;
    __AXAuxCpuReadWritePosition = 2;

    pA = (s32*)&__AXBufferAuxA;
    pB = (s32*)&__AXBufferAuxB;

    for (i = 0; i < 480; i++) {
        *(pA) = 0; pA++;
        *(pB) = 0; pB++;
    }
}

/**
 * @note Address: 0x80177648
 * @note Size: 0x10
 */
void __AXAuxQuit(void) {
#if DEBUG
    OSReport("Shutting down AXAux code module\n");
#endif
    __AXCallbackAuxA = NULL;
    __AXCallbackAuxB = NULL;
}

/**
 * @note Address: 0x80177658
 * @note Size: 0x34
 */
void __AXGetAuxAInput(u32* p) {
    if (__AXCallbackAuxA) {
        *p = (u32)&__AXBufferAuxA[__AXAuxDspWritePosition][0];
    } else {
        *p = 0;
    }
}

/**
 * @note Address: 0x8017768C
 * @note Size: 0x20
 */
void __AXGetAuxAInputDpl2(u32* p) {
    *p = (u32)&__AXBufferAuxB[__AXAuxDspWritePosition][320];
}

/**
 * @note Address: 0x801776AC
 * @note Size: 0x1C
 */
void __AXGetAuxAOutput(u32* p) {
    *p = (u32)&__AXBufferAuxA[__AXAuxDspReadPosition][0];
}

/**
 * @note Address: 0x801776C8
 * @note Size: 0x20
 */
void __AXGetAuxAOutputDpl2R(u32* p) {
    *p = (u32)&__AXBufferAuxA[__AXAuxDspReadPosition][160];
}

/**
 * @note Address: 0x801776E8
 * @note Size: 0x20
 */
void __AXGetAuxAOutputDpl2Ls(u32* p) {
    *p = (u32)&__AXBufferAuxA[__AXAuxDspReadPosition][320];
}

/**
 * @note Address: 0x80177708
 * @note Size: 0x20
 */
void __AXGetAuxAOutputDpl2Rs(u32* p) {
    *p = (u32)&__AXBufferAuxB[__AXAuxDspReadPosition][320];
}

/**
 * @note Address: 0x80177728
 * @note Size: 0x34
 */
void __AXGetAuxBInput(u32* p) {
    if (__AXCallbackAuxB) {
        *p = (u32)&__AXBufferAuxB[__AXAuxDspWritePosition][0];
    } else {
        *p = 0;
    }
}

/**
 * @note Address: 0x8017775C
 * @note Size: 0x1C
 */
void __AXGetAuxBOutput(u32* p) {
    *p = (u32)&__AXBufferAuxB[__AXAuxDspReadPosition][0];
}

/**
 * @note Address: 0x80177778
 * @note Size: 0x1C
 */
void __AXGetAuxBForDPL2(u32* p) {
    *p = (u32)&__AXBufferAuxB[__AXAuxDspWritePositionDpl2][0];
}

/**
 * @note Address: 0x80177794
 * @note Size: 0x1C
 */
void __AXGetAuxBOutputDPL2(u32* p) {
    *p = (u32)&__AXBufferAuxB[__AXAuxDspReadPositionDpl2][0];
}

/**
 * @note Address: 0x801777B0
 * @note Size: 0x23C
 */
void __AXProcessAux(void) {
    __AXAuxADspWrite = &__AXBufferAuxA[__AXAuxDspWritePosition][0];
    __AXAuxADspRead = &__AXBufferAuxA[__AXAuxDspReadPosition][0];
    __AXAuxBDspWrite = &__AXBufferAuxB[__AXAuxDspWritePosition][0];
    __AXAuxBDspRead = &__AXBufferAuxB[__AXAuxDspReadPosition][0];

    if (__AXCallbackAuxA) {
        if (__AXClMode == 2) {
            AX_AUX_DATA_DPL2 auxData;
            auxData.l = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][0];
            auxData.r = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][160];
            auxData.ls = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][320];
            auxData.rs = &__AXBufferAuxB[__AXAuxCpuReadWritePosition][320];
            DCInvalidateRange(auxData.l, 0x780);
            DCInvalidateRange(auxData.rs, 0x280);
            __AXCallbackAuxA(&auxData.l, __AXContextAuxA);
            DCFlushRangeNoSync(auxData.l, 0x780);
            DCFlushRangeNoSync(auxData.rs, 0x280);
        } else {
            AX_AUX_DATA auxData;
            auxData.l = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][0];
            auxData.r = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][160];
            auxData.s = &__AXBufferAuxA[__AXAuxCpuReadWritePosition][320];
            DCInvalidateRange(auxData.l, 0x780);
            __AXCallbackAuxA(&auxData.l, __AXContextAuxA);
            DCFlushRangeNoSync(auxData.l, 0x780);
        }        
    }

    if (__AXCallbackAuxB && __AXClMode != 2) {
        AX_AUX_DATA auxData;
        auxData.l = &__AXBufferAuxB[__AXAuxCpuReadWritePosition][0];
        auxData.r = &__AXBufferAuxB[__AXAuxCpuReadWritePosition][160];
        auxData.s = &__AXBufferAuxB[__AXAuxCpuReadWritePosition][320];
        DCInvalidateRange(auxData.l, 0x780);
        __AXCallbackAuxB(&auxData.l, __AXContextAuxB);
        DCFlushRangeNoSync(auxData.l, 0x780);
    }

    __AXAuxDspWritePosition += 1;
    __AXAuxDspWritePosition %= 3;
    __AXAuxDspReadPosition += 1;
    __AXAuxDspReadPosition %= 3;

    __AXAuxDspWritePositionDpl2 += 1;
    __AXAuxDspWritePositionDpl2 &= 1;
    __AXAuxDspReadPositionDpl2 += 1;
    __AXAuxDspReadPositionDpl2 &= 1;

    __AXAuxCpuReadWritePosition += 1;
    __AXAuxCpuReadWritePosition %= 3;
}

/**
 * @note Address: 0x801779EC
 * @note Size: 0xC
 */
void AXRegisterAuxACallback(void (*callback)(void*, void*), void* context) {
    __AXCallbackAuxA = callback;
    __AXContextAuxA = context;    
}

/**
 * @note Address: 0x801779F8
 * @note Size: 0xC
 */
void AXRegisterAuxBCallback(void (*callback)(void*, void*), void* context) {
    __AXCallbackAuxB = callback;
    __AXContextAuxB = context;    
}
