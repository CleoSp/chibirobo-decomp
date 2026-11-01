#include "game/ccameraaction.h"
#include "game/cgame.h"
#include "game/evtqueue.h"

// what fn_8007F570 gets as its context
struct EvtContext {
	u32 _00;       // _00
	XObj* mObject; // _04, the one running the script
};

static inline XObj* NamedObject(s16 id)
{
	for (s32 i = 0; i < lbl_802247B0.mCount; ++i) {
		if (lbl_802247B0.mEntry[i].mId == id) {
			XObj* object = lbl_8025C400.GetObject(lbl_802247B0.mEntry[i].mIndex);
			if (object != NULL && (object->mStatus.mFlags & 1ULL))
				return object;
			return NULL;
		}
	}
	return NULL;
}

/**
 * @note Address: 0x8007F570
 * @note Size: 0xF4
 */
extern "C" XObj* fn_8007F570(void* context, s16 id)
{
	if (id >= 10000) {
		XObj* object = NamedObject(id);
		if (object != NULL)
			return object;
		return NULL;
	}
	if (id >= 0)
		return lbl_8025C400.GetObject(id);
	if (id == -1)
		return ((EvtContext*)context)->mObject;
	return (XObj*)(s32)id;
}
