#define DEFER_EVENT_DESTRUCTORS
#define DEFER_CAMERA_MEMENTO_CONSTRUCTOR
#include "game/hsd_jobj.h"
#include "game/cmotioncontrolcamera.h"
#include "game/evt.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" CCameraMemento* __dt__14CCameraMementoFv(CCameraMemento* memento, s32 flags);

#pragma section RX ".text.eventdtors"
__declspec(section ".text.eventdtors") CEvtFlags::~CEvtFlags()
{
}
__declspec(section ".text.eventdtors") CEvtVars::~CEvtVars()
{
}
__declspec(section ".text.eventdtors") CEvtQueue::~CEvtQueue()
{
}
__declspec(section ".text.eventdtors") CEvtQueueEx::~CEvtQueueEx()
{
}
__declspec(section ".text.eventdtors") CEvtTimer::~CEvtTimer()
{
}
__declspec(section ".text.eventdtors") CCameraMemento::CCameraMemento()
{
}

/**
 * @note Address: 0x8007FDDC
 * @note Size: 0x38
 */
extern "C" __declspec(section ".text.eventdtors") void DestroyCCameraMementoArray()
{
	__destroy_arr(gEventCameraMementos, (ConstructorDestructor*)__dt__14CCameraMementoFv, sizeof(CCameraMemento), 4);
}
