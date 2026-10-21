#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

#if DEBUG
const char* __AXVersion = "<< Dolphin SDK - AX\tdebug build: Apr  5 2004 03:56:21 (0x2301) >>";
#else
const char* __AXVersion = "<< Dolphin SDK - AX\trelease build: Apr  5 2004 04:15:05 (0x2301) >>";
#endif

/**
 * @note Address: 0x80176F50
 * @note Size: 0x24
 */
void AXInit(void) {
    AXInitEx(0);
}

/**
 * @note Address: 0x80176F74
 * @note Size: 0x4C
 */
void AXInitEx(u32 outputBufferMode) {
#if DEBUG
    OSReport("Initializing AX\n");
#endif
    OSRegisterVersion(__AXVersion);

    __AXAllocInit();
    __AXVPBInit();
    __AXSPBInit();
    __AXAuxInit();
    __AXClInit();
    __AXOutInit(outputBufferMode);
}

/**
 * @note Address: 0x80176FC0
 * @note Size: 0x34
 */
void AXQuit(void) {
#if DEBUG
    OSReport("Shutting down AX\n");
#endif
    __AXAllocQuit();
    __AXVPBQuit();
    __AXSPBQuit();
    __AXAuxQuit();
    __AXClQuit();
    __AXOutQuit();
}
