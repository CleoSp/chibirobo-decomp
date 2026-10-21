#include <dolphin/types.h>

#include <sysdolphin/cobj.h>
#include <sysdolphin/memory.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/objalloc.h>
#include <sysdolphin/shadow.h>
#include <sysdolphin/tobj.h>

typedef struct _HSD_MemCallbackData {
    HSD_MemCallbacks callbacks;
    u32 unused;
} HSD_MemCallbackData;

HSD_MemCallbackData HSD_MemCallbackState;

/**
 * @note Address: 0x801D41F8
 * @note Size: 0x74
 */
void _HSD_MemSetCallbacks(HSD_MemCallbacks* callbacks, u32 size)
{
    if (size != sizeof(HSD_MemCallbacks)) {
        __assert("memory.c", 0x14, "size == sizeof(__mem_cb)");
    }
    HSD_MemCallbackState.callbacks = *callbacks;
}

/**
 * @note Address: 0x801D426C
 * @note Size: 0x2C
 */
void* _HSD_MemAlloc(u32 size, u32 align, u32 flags)
{
    return HSD_MemCallbackState.callbacks.alloc(size, align, flags);
}

/**
 * @note Address: 0x801D4298
 * @note Size: 0x30
 */
u32 _HSD_MemGetRemain(void)
{
    return HSD_MemCallbackState.callbacks.get_remain();
}

/**
 * @note Address: N/A
 * @note Size: 0x30
 */
void _HSD_MemClear(void)
{
    HSD_MemCallbackState.callbacks.clear();
}

/**
 * @note Address: 0x801D42C8
 * @note Size: 0x30
 */
s32 _HSD_MemCheckOwn(void* ptr)
{
    return HSD_MemCallbackState.callbacks.check_own(ptr);
}

#pragma push
#pragma dont_inline on
/**
 * @note Address: 0x801D42F8
 * @note Size: 0x30
 */
void HSD_Free(void* ptr)
{
    HSD_MemCallbackState.callbacks.free(ptr);
}

/**
 * @note Address: 0x801D4328
 * @note Size: 0x38
 */
void* HSD_Alloc(u32 size)
{
    return HSD_MemCallbackState.callbacks.alloc(size, 0x20, 0);
}
#pragma pop
