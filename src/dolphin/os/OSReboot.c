#include <dolphin/dolphin.h>
#include <dolphin/os.h>

#include "__os.h"

static void* SaveStart;
static void* SaveEnd;

/**
 * @note Address: 0x80168600
 * @note Size: 0x70
 */
void __OSReboot(u32 resetCode, u32 bootDol) {
    OSContext exceptionContext;
    char* argvToPass;

	OSDisableInterrupts();
    OSSetArenaLo((void*)0x81280000);
    OSSetArenaHi((void*)0x812f0000);
    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    argvToPass = NULL;
    __OSBootDol(bootDol, resetCode | 0x80000000, &argvToPass);
}

/**
 * @note Address: 0x80168670
 * @note Size: 0xC
 */
void OSSetSaveRegion(void* start, void* end) {
    ASSERTMSGLINE(134, (u32)start >= 0x80700000 || start == NULL, "OSSetSaveRegion(): start address should be NULL or higher than 0x80700000\n");
    ASSERTMSGLINE(135, 0x81200000 >= (u32)end || end == NULL, "OSSetSaveRegion(): end address should be NULL or lower than 0x81200000\n");
    ASSERTMSGLINE(136, ((start == NULL) ^ (end == NULL)) == 0, "OSSetSaveRegion(): if either start or end is NULL, both should be NULL\n");

    SaveStart = start;
    SaveEnd = end;
}

/**
 * @note Address: 0x8016867C
 * @note Size: 0x14
 */
void OSGetSaveRegion(void** start, void** end) {
    *start = SaveStart;
    *end = SaveEnd;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void OSGetSavedRegion(void** start, void** end) {
    *start = __OSRebootParams.regionStart;
    *end = __OSRebootParams.regionEnd;
}
