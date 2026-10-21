#include <global.h>
#include <sysdolphin/memory.h>
#include <sysdolphin/random.h>

static u32 seed      = 1;
u32* HSD_RandSeedPtr = &seed;

/**
 * @note Address: 0x801D45A8
 * @note Size: 0x30
 */
s32 HSD_Rand(void)
{
    *HSD_RandSeedPtr = *HSD_RandSeedPtr * 214013 + 2531011;
    return *HSD_RandSeedPtr >> 0x10;
}

/**
 * @note Address: 0x801D45D8
 * @note Size: 0x58
 */
f32 HSD_Randf(void)
{
    *HSD_RandSeedPtr = *HSD_RandSeedPtr * 214013 + 2531011;
    return (f32) (*HSD_RandSeedPtr >> 0x10) / (1 << 16);
}

/**
 * @note Address: 0x801D4630
 * @note Size: 0x3C
 */
s32 HSD_Randi(s32 max_val)
{
    return max_val * HSD_Rand() / (1 << 16);
}

/**
 * @note Address: N/A
 * @note Size: 0x34
 */
void _HSD_RandForgetMemory(void)
{
    if (_HSD_MemCheckOwn(HSD_RandSeedPtr)) {
        HSD_RandSeedPtr = &seed;
    }
}
