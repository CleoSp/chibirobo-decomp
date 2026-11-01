#include "game/xobjread.h"

/**
 * @note Address: 0x80134E4C
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CMsgQueue* fn_80134E4C(CMsgQueue* queue, s32 flags)
{
	if (queue != NULL) {
		queue->CMsgQueue::~CMsgQueue();
		if ((s16)flags > 0) {
			operator delete(queue);
		}
	}
	return queue;
}

/**
 * @note Address: 0x80134E94
 * @note Size: 0x84
 */
XObjReadOne::~XObjReadOne()
{
}

/**
 * @note Address: 0x80134F18
 * @note Size: 0x84
 */
XObjReadAll::~XObjReadAll()
{
}

/**
 * @note Address: 0x80134F9C
 * @note Size: 0x74
 */
// todo: weak copy
extern "C" XObjRead* fn_80134F9C(XObjRead* reader, s32 flags)
{
	if (reader != NULL) {
		reader->XObjRead::~XObjRead();
		if ((s16)flags > 0) {
			operator delete(reader);
		}
	}
	return reader;
}
