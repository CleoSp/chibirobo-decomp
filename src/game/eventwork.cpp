#include "game/evtqueue.h"
#include "game/xobj.h"
#include "game/cmotioncontrolcamera.h"
#include "PowerPC_EABI_Support/Runtime/global_destructor_chain.h"

// __sinit_evt_cpp installs the vtable after each array and registers its destructor.
extern "C" {
// The retail initializer addresses its globals relative to this BSS prefix.
#if defined(VERSION_JP)
u8 lbl_80345C78[0x134];
#else
u8 lbl_803676D0[0x140];
#endif
DestructorChain sEventFlagsDestructorChain;
u32 gEventFlagWords[128];
void* sEventFlagsVTable;
DestructorChain sEventVarsDestructorChain;
s32 gEventVariables[2048];
void* sEventVarsVTable;
CEvtQueueEx gEventScriptQueue;
CEvtTimer gEventTimers;
XObj gEventScriptObject;
CCameraMemento gEventCameraMementos[4];
}
