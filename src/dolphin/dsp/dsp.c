#include <cstddef>
#include <dolphin/dolphin.h>
#include <dolphin/hw_regs.h>

#include "__dsp.h"

#define BUILD_DATE "Apr  5 2004"
#if DEBUG
#define BUILD_TIME "03:56:49"
#else
#define BUILD_TIME "04:15:32"
#endif

#if DEBUG
const char* __DSPVersion = "<< Dolphin SDK - DSP\tdebug build: Apr  5 2004 03:56:49 (0x2301) >>";
#else
const char* __DSPVersion = "<< Dolphin SDK - DSP\trelease build: Apr  5 2004 04:15:32 (0x2301) >>";
#endif

extern DSPTaskInfo* __DSP_rude_task;
extern int __DSP_rude_task_pending;

static BOOL __DSP_init_flag;

/**
 * @note Address: 0x8017CD80
 * @note Size: 0x10
 */
u32 DSPCheckMailToDSP(void) {
    return (__DSPRegs[0] & (1 << 15)) >> 15;
}

/**
 * @note Address: 0x8017CD90
 * @note Size: 0x10
 */
u32 DSPCheckMailFromDSP(void) {
    return (__DSPRegs[2] & (1 << 15)) >> 15;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
u32 DSPReadCPUToDSPMbox(void) {
    return (__DSPRegs[0] << 16) | __DSPRegs[1];
}

/**
 * @note Address: 0x8017CDA0
 * @note Size: 0x18
 */
u32 DSPReadMailFromDSP(void) {
    return (__DSPRegs[2] << 16) | __DSPRegs[3];
}

/**
 * @note Address: 0x8017CDB8
 * @note Size: 0x14
 */
void DSPSendMailToDSP(u32 mail) {
    __DSPRegs[0] = mail >> 16;
    __DSPRegs[1] = mail & 0xFFFF;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void DSPAssertInt(void) {
    BOOL old;
    u16 tmp;

    old = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA8) | 2;
    __DSPRegs[5] = tmp;
    OSRestoreInterrupts(old);
}

/**
 * @note Address: 0x8017CDCC
 * @note Size: 0xC4
 */
void DSPInit(void) {
    BOOL old;
    u16 tmp;

    __DSP_debug_printf("DSPInit(): Build Date: %s %s\n", BUILD_DATE, BUILD_TIME);

    if (__DSP_init_flag == 1)
        return;

    OSRegisterVersion(__DSPVersion);

    old = OSDisableInterrupts();
    __OSSetInterruptHandler(7, __DSPHandler);
    __OSUnmaskInterrupts(OS_INTERRUPTMASK_DSP_DSP);

    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA8) | 0x800;
    __DSPRegs[5] = tmp;

    tmp = __DSPRegs[5];
    __DSPRegs[5] = tmp = tmp & ~0xAC;

    __DSP_first_task = __DSP_last_task = __DSP_curr_task = __DSP_tmp_task = NULL;
    __DSP_init_flag = 1;

    OSRestoreInterrupts(old);
}

/**
 * @note Address: 0x8017CE90
 * @note Size: 0x8
 */
BOOL DSPCheckInit(void) {
    return __DSP_init_flag;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void DSPReset(void) {
    BOOL old;
    u16 tmp;

    old = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA8) | 0x800 | 1;
    __DSPRegs[5] = tmp;
    __DSP_init_flag = 0;
    OSRestoreInterrupts(old);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void DSPHalt(void) {
    BOOL old;
    u16 tmp;

    old = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xA8) | 4;
    __DSPRegs[5] = tmp;
    OSRestoreInterrupts(old);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void DSPUnhalt(void) {
    BOOL old;
    u16 tmp;

    old = OSDisableInterrupts();
    tmp = __DSPRegs[5];
    tmp = (tmp & ~0xAC);
    __DSPRegs[5] = tmp;
    OSRestoreInterrupts(old);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
u32 DSPGetDMAStatus(void) {
    return (__DSPRegs[5] & (1 << 9));
}

/**
 * @note Address: 0x8017CE98
 * @note Size: 0x70
 */
DSPTaskInfo* DSPAddTask(DSPTaskInfo* task) {
    BOOL old;

    ASSERTMSGLINE(556, __DSP_init_flag == 1, "DSPAddTask(): DSP driver not initialized!\n");

    old = OSDisableInterrupts();

    __DSP_insert_task(task);
    task->state = 0;
    task->flags = 1;

    OSRestoreInterrupts(old);
    if (task == __DSP_first_task)
        __DSP_boot_task(task);
    return task;
}

/**
 * @note Address: 0x8017CF08
 * @note Size: 0x40
 */
DSPTaskInfo* DSPCancelTask(DSPTaskInfo* task) {
    BOOL old;

    ASSERTMSGLINE(592, __DSP_init_flag == 1, "DSPCancelTask(): DSP driver not initialized!\n");

    old = OSDisableInterrupts();

    task->flags |= 2;

    OSRestoreInterrupts(old);
    return task;
}

/**
 * @note Address: 0x8017CF48
 * @note Size: 0xC8
 */
DSPTaskInfo* DSPAssertTask(DSPTaskInfo* task) {
    s32 old;

    ASSERTMSGLINE(623, __DSP_init_flag == 1, "DSPAssertTask(): DSP driver not initialized!\n");
    ASSERTMSGLINE(624, task->flags & 1, "DSPAssertTask(): Specified task not in active task list!\n");

    old = OSDisableInterrupts();

    if (__DSP_curr_task == task) {
        __DSP_rude_task = task;
        __DSP_rude_task_pending = 1;
        OSRestoreInterrupts(old);
        return task;
    }

    if (task->priority < __DSP_curr_task->priority) {
        __DSP_rude_task = task;
        __DSP_rude_task_pending = 1;
        if (__DSP_curr_task->state == 1) {
            DSPAssertInt();
        }
        OSRestoreInterrupts(old);
        return task;
    }

    OSRestoreInterrupts(old);
    return NULL;
}
