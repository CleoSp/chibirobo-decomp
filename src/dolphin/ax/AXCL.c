#include <dolphin/dolphin.h>
#include <dolphin/ax.h>

#include "__ax.h"

static u16 __AXCommandList[2][384];

static u32 __AXCommandListPosition;
static u16* __AXClWrite;
static u32 __AXCommandListCycles;
static u32 __AXCompressor;
u32 __AXClMode;

/**
 * @note Address: 0x80177A04
 * @note Size: 0x8
 */
u32 __AXGetCommandListCycles(void) {
    return __AXCommandListCycles;
}

/**
 * @note Address: 0x80177A0C
 * @note Size: 0x3C
 */
u32 __AXGetCommandListAddress(void) {
    u32 address;

    address = (u32)&__AXCommandList[__AXCommandListPosition][0];
    __AXCommandListPosition += 1;
    __AXCommandListPosition &= 1;
    __AXClWrite = (void*)&__AXCommandList[__AXCommandListPosition][0];
    return address;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void __AXWriteToCommandList(u16 data) {
    *__AXClWrite = data;
    __AXClWrite++;
}

/**
 * @note Address: 0x80177A48
 * @note Size: 0x6B0
 */
void __AXNextFrame(void* sbuffer, void* buffer) {
    u32 data;
    u16* pCommandList;

    __AXCommandListCycles = 0x1A9;
    pCommandList = __AXClWrite;
    data = __AXGetStudio();
    __AXWriteToCommandList(0);
    __AXWriteToCommandList((u16)(data >> 0x10));
    __AXWriteToCommandList((u16)(data));
    __AXCommandListCycles += 0x2E44;

    switch (__AXClMode) {
    case 0:
        __AXWriteToCommandList(7);
        __AXWriteToCommandList((u16)((u32)sbuffer >> 0x10));
        __AXWriteToCommandList((u32)sbuffer);
        __AXCommandListCycles += 0x546;
        break;
    case 1:
        __AXWriteToCommandList(0x11);
        __AXWriteToCommandList((u16)((u32)sbuffer >> 0x10));
        __AXWriteToCommandList((u32)sbuffer);
        __AXCommandListCycles += 0x5E6;
        break;
    case 2:
        break;
    default:
        ASSERTMSGLINE(193, 0, "Unknown AX mode!");
    }

    data = (u32)__AXGetPBs();
    __AXWriteToCommandList(2);
    __AXWriteToCommandList((u16)(data >> 0x10));
    __AXWriteToCommandList((u16)data);
    __AXWriteToCommandList(3);

    if (__AXClMode == 2) {
        __AXGetAuxAInput(&data);
        if (data != 0) {
            __AXWriteToCommandList(0x13);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAInputDpl2(&data);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAOutput(&data);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAOutputDpl2R(&data);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAOutputDpl2Ls(&data);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAOutputDpl2Rs(&data);
            __AXWriteToCommandList(data >> 0x10);
            __AXWriteToCommandList((u16)data);
            __AXCommandListCycles += 0xDED;
        }
        __AXWriteToCommandList(0x10);
        __AXGetAuxBForDPL2(&data);
        __AXWriteToCommandList(data >> 0x10);
        __AXWriteToCommandList((u16)data);
        __AXGetAuxBOutputDPL2(&data);
        __AXWriteToCommandList(data >> 0x10);
        __AXWriteToCommandList((u16)data);
        __AXCommandListCycles += 0xDED;
    } else {
        __AXGetAuxAInput(&data);

        if (data != 0) {
            __AXWriteToCommandList(4);
            __AXWriteToCommandList((u16)(data >> 0x10));
            __AXWriteToCommandList((u16)data);
            __AXGetAuxAOutput(&data);
            __AXWriteToCommandList((u16)(data >> 0x10));
            __AXWriteToCommandList((u16)data);
            __AXCommandListCycles += 0xDED;
        }

        __AXGetAuxBInput(&data);
        if (data != 0) {
            __AXWriteToCommandList(5);
            __AXCommandListCycles += 0xDED;
            __AXWriteToCommandList((u16)(data >> 0x10));
            __AXWriteToCommandList((u16)data);
            __AXGetAuxBOutput(&data);
            __AXWriteToCommandList((u16)(data >> 0x10));
            __AXWriteToCommandList((u16)data);
        }
    }

    if (__AXCompressor) {
        __AXWriteToCommandList(0x12);
        __AXWriteToCommandList(0x8000);
        __AXWriteToCommandList(0xA);
        __AXWriteToCommandList((u32)__AXCompressorTable >> 0x10);
        __AXWriteToCommandList((u32)__AXCompressorTable);
        __AXCommandListCycles += 0xBB8;
    }
    
    __AXWriteToCommandList(0xE);
    __AXWriteToCommandList((u16)((u32)sbuffer >> 0x10));
    __AXWriteToCommandList((u32)sbuffer);
    __AXWriteToCommandList((u16)((u32)buffer >> 0x10));
    __AXWriteToCommandList((u32)buffer);
    __AXCommandListCycles += 0x2710;
    __AXWriteToCommandList(0xF);
    __AXCommandListCycles += 2;
    DCFlushRange(pCommandList, 0x300);
}

/**
 * @note Address: 0x801780F8
 * @note Size: 0x24
 */
void __AXClInit(void) {
#if DEBUG
    OSReport("Initializing AXCL code module\n");
#endif
    ASSERTLINE(338, ((u32)&__AXCommandList[0][0] & 0x1F) == 0);
    ASSERTLINE(339, ((u32)&__AXCommandList[1][0] & 0x1F) == 0);
    __AXClMode = 0;
    __AXCommandListPosition = 0;
    __AXClWrite = (void*)&__AXCommandList;
    __AXCompressor = 1;
}

/**
 * @note Address: 0x8017811C
 * @note Size: 0x4
 */
void __AXClQuit(void) {
#if DEBUG
    OSReport("Shutting down AXCL code module\n");
#endif
}

/**
 * @note Address: 0x80178120
 * @note Size: 0x14
 */
void AXSetMode(u32 mode) {
    if (__AXClMode != mode) {
        __AXClMode = mode;
    }
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
u32 AXGetMode(void) {
    return __AXClMode;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void AXSetCompressor(u32 i) {
    __AXCompressor = i;
}
