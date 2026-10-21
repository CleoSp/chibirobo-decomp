#include <global.h>
#include <dolphin/ostime.h>
#include <sysdolphin/perf.h>

void memset(void*, int, int);

static s64 start_time;

HSD_PerfStat HSD_PerfLastStat    = { 0 };
HSD_PerfStat HSD_PerfCurrentStat = { 0 };

/**
 * @note Address: N/A
 * @note Size: 0x64
 */
void HSD_PerfInitStat(void)
{
    HSD_PerfLastStat = HSD_PerfCurrentStat;
    memset(&HSD_PerfCurrentStat, 0, sizeof(HSD_PerfStat));
}

/**
 * @note Address: N/A
 * @note Size: 0x28
 */
void HSD_PerfSetStartTime(void)
{
    start_time = OSGetTime();
}

/**
 * @note Address: N/A
 * @note Size: 0x74
 */
void HSD_PerfSetCPUTime(void)
{
    s64 elapsed = OSGetTime() - start_time;
    HSD_PerfCurrentStat.cpu_time = elapsed / (f32) (*(u32*) 0x800000F8 / 4 / 60);
}

/**
 * @note Address: N/A
 * @note Size: 0x78
 */
void HSD_PerfSetDrawTime(void)
{
    s64 elapsed = OSGetTime() - start_time;
    HSD_PerfCurrentStat.draw_time = elapsed / (f32) (*(u32*) 0x800000F8 / 4 / 60);
}

/**
 * @note Address: N/A
 * @note Size: 0x78
 */
void HSD_PerfSetTotalTime(void)
{
    s64 elapsed = OSGetTime() - start_time;
    HSD_PerfCurrentStat.total_time = elapsed / (f32) (*(u32*) 0x800000F8 / 4 / 60);
}

/**
 * @note Address: 0x801D3AB4
 * @note Size: 0x5C
 */
void HSD_PerfCountEnvelopeBlending(s32 n)
{
    assert_line(164, n < 32);
    HSD_PerfCurrentStat.env_blend[n]++;
}
