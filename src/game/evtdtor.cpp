#include "game/hsd_jobj.h"
#include "game/cmotioncontrolcamera.h"
#include "game/evt.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" CCameraMemento* fn_80016F9C(CCameraMemento* memento, s32 flags);

// no <new> here
inline void* operator new(size_t, void* storage)
{
	return storage;
}

// todo: weak copy
/**
 * @note Address: 0x8007FDDC
 * @note Size: 0x38
 */
extern "C" void fn_8007FDDC()
{
	__destroy_arr(lbl_8036AA3C, (ConstructorDestructor*)fn_80016F9C, sizeof(CCameraMemento), 4);
}

// todo: weak copy
/**
 * @note Address: 0x8007FE14
 * @note Size: 0x24
 */
extern "C" CCameraMemento* fn_8007FE14(CCameraMemento* memento)
{
	return new (memento) CCameraMemento;
}

// todo: weak copy
/**
 * @note Address: 0x8007FE38
 * @note Size: 0x48
 */
extern "C" CEvtTimer* fn_8007FE38(CEvtTimer* timer, s32 flags)
{
	if (timer != NULL) {
		timer->CEvtTimer::~CEvtTimer();
		if ((s16)flags > 0)
			::operator delete(timer);
	}
	return timer;
}

// todo: weak copy
/**
 * @note Address: 0x8007FE80
 * @note Size: 0x5C
 */
extern "C" CEvtQueueEx* fn_8007FE80(CEvtQueueEx* queue, s32 flags)
{
	if (queue != NULL) {
		queue->CEvtQueueEx::~CEvtQueueEx();
		if ((s16)flags > 0)
			::operator delete(queue);
	}
	return queue;
}

// todo: weak copy
/**
 * @note Address: 0x8007FEDC
 * @note Size: 0x48
 */
extern "C" CEvtQueue* fn_8007FEDC(CEvtQueue* queue, s32 flags)
{
	if (queue != NULL) {
		queue->CEvtQueue::~CEvtQueue();
		if ((s16)flags > 0)
			::operator delete(queue);
	}
	return queue;
}

// todo: weak copy
/**
 * @note Address: 0x8007FF24
 * @note Size: 0x48
 */
extern "C" CEvtVars* fn_8007FF24(CEvtVars* vars, s32 flags)
{
	if (vars != NULL) {
		vars->CEvtVars::~CEvtVars();
		if ((s16)flags > 0)
			::operator delete(vars);
	}
	return vars;
}

// todo: weak copy
/**
 * @note Address: 0x8007FF6C
 * @note Size: 0x48
 */
extern "C" CEvtFlags* fn_8007FF6C(CEvtFlags* flags, s32 mode)
{
	if (flags != NULL) {
		flags->CEvtFlags::~CEvtFlags();
		if ((s16)mode > 0)
			::operator delete(flags);
	}
	return flags;
}
