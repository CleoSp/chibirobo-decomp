#include "game/hsd_jobj.h"
#include "game/cgs.h"

extern "C" {
extern void* __vt__10CGsChannel[];
extern void* __vt__11CGsTevStage[];
}

/**
 * @note Address: 0x8001BC14
 * @note Size: 0x10
 */
// todo: weak copy
extern "C" CGsChannel* __ct__10CGsChannelFv(CGsChannel* channel)
{
	*(void***)((u8*)channel + 0x34) = __vt__10CGsChannel;
	return channel;
}

/**
 * @note Address: 0x8001BC24
 * @note Size: 0x10
 */
// todo: weak copy
extern "C" CGsTevStage* __ct__11CGsTevStageFv(CGsTevStage* stage)
{
	*(void***)((u8*)stage + 0x94) = __vt__11CGsTevStage;
	return stage;
}
