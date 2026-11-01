#include "game/hsd_jobj.h"
#include "game/cgs.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__11CGsTevStage[];
extern void* __vt__10CGsChannel[];
}

/**
 * @note Address: 0x80008344
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CGsTevStage* __dt__11CGsTevStageFv(CGsTevStage* stage, s32 flags)
{
	if (stage != NULL) {
		*(void***)((u8*)stage + 0x94) = __vt__11CGsTevStage;
		if ((s16)flags > 0)
			::operator delete(stage);
	}
	return stage;
}

/**
 * @note Address: 0x8000838C
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CGsChannel* __dt__10CGsChannelFv(CGsChannel* channel, s32 flags)
{
	if (channel != NULL) {
		*(void***)((u8*)channel + 0x34) = __vt__10CGsChannel;
		if ((s16)flags > 0)
			::operator delete(channel);
	}
	return channel;
}
