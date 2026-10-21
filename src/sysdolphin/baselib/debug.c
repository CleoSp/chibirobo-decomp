#include <functions.h>
#include <dolphin/os.h>

typedef void (*HSD_PanicCallback)(void* context, ...);

void OSReport(const char* msg, ...);
void HSD_Panic(const char* file, s32 line, const char* msg);

// an OSContext, HSD_SaveContext fills it before the panic callback runs
u8 hsd_saved_context[0x2C8];

static HSD_PanicCallback panicCallback;

/**
 * @note Address: 0x801DD1B0
 * @note Size: 0x98
 */
// saves every register by hand, so this one really is asm
asm void HSD_SaveContext(void)
{
    nofralloc
    mtsprg 0, r3
    lis r3, hsd_saved_context@ha
    addi r3, r3, hsd_saved_context@l
    stmw r0, 0(r3)
    mfsprg r4, 0
    stw r4, 0xc(r3)
    mfspr r4, GQR0
    stw r4, 0x1a4(r3)
    mfspr r4, GQR1
    stw r4, 0x1a8(r3)
    mfspr r4, GQR2
    stw r4, 0x1ac(r3)
    mfspr r4, GQR3
    stw r4, 0x1b0(r3)
    mfspr r4, GQR4
    stw r4, 0x1b4(r3)
    mfspr r4, GQR5
    stw r4, 0x1b8(r3)
    mfspr r4, GQR6
    stw r4, 0x1bc(r3)
    mfspr r4, GQR7
    stw r4, 0x1c0(r3)
    mfcr r4
    stw r4, 0x80(r3)
    mflr r4
    stw r4, 0x84(r3)
    mfctr r4
    stw r4, 0x88(r3)
    mfxer r4
    stw r4, 0x8c(r3)
    mfsrr0 r4
    stw r4, 0x198(r3)
    mfsrr1 r4
    stw r4, 0x19c(r3)
    lhz r4, 0x1a2(r3)
    ori r4, r4, 1
    sth r4, 0x1a2(r3)
    b OSFillFPUContext
}

/**
 * @note Address: 0x801DD248
 * @note Size: 0x5C
 */
void __assert(const char* file, unsigned long line, const char* cond)
{
    OSReport("assertion \"%s\" failed", cond);
    HSD_Panic(file, line, "");
}

/**
 * @note Address: 0x801DD2A4
 * @note Size: 0x98
 */
void HSD_Panic(const char* file, s32 line, const char* msg)
{
    if (panicCallback != NULL) {
        HSD_SaveContext();
        OSReport("%s in %s on line %d.\n", msg, file, line);
        panicCallback(&hsd_saved_context);
    }
    OSPanic(file, line, msg);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void HSD_SetPanicCallback(HSD_PanicCallback cb) { panicCallback = cb; }
