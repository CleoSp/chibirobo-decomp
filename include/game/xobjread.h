#ifndef _GAME_XOBJREAD_H
#define _GAME_XOBJREAD_H

#include "game/cthread.h"
#include "Dolphin/OS/OSMessage.h"

// the threads that read objects in the background, class names from RTTI. xobjs.cpp keeps
// one of each kind, the methods are there too

// an OSMessageQueue with its message buffer
class CMsgQueue {
public:
	OSMessageQueue mQueue; // _00
	OSMessage* mMsgs;      // _20
	// _24 vtable

	virtual ~CMsgQueue() { }      // _08 (weak)
	virtual void vf0C(s32 count); // _0C, allocates mMsgs and inits mQueue
};
typedef char CMsgQueue_size_check[sizeof(CMsgQueue) == 0x28 ? 1 : -1];

// one read waiting in a reader
struct XObjReadReq {
	void* mObj;  // _00, null when the slot is free
	void* mInfo; // _04
};

class XObjRead : public CThread {
public:
	// _00-_328 = CThread
	CMsgQueue mQueue;     // _328
	XObjReadReq mReqs[8]; // _350

	virtual ~XObjRead() { } // _08 (weak)
	virtual s32 vf0C(OSThreadStartFunction func, void* param, u32 stackSize, OSPriority priority,
	                 u16 attr);          // _0C, makes the thread and its queue
	virtual void vf10();                 // _10
	virtual void vf14();                 // _14
	virtual void vf18(XObjReadReq* req); // _18, runs one read
};
typedef char XObjRead_size_check[sizeof(XObjRead) == 0x390 ? 1 : -1];

// reads a whole object list
class XObjReadAll : public XObjRead {
public:
	virtual ~XObjReadAll();              // _08
	virtual void vf18(XObjReadReq* req); // _18
};

// reads one object
class XObjReadOne : public XObjRead {
public:
	virtual ~XObjReadOne();              // _08
	virtual void vf18(XObjReadReq* req); // _18
};

// reads the anim archives
class XObjReadAnm : public XObjRead {
public:
	// _00-_390 = XObjRead
	u8 _390[0x59A8 - 0x390]; // _390

	virtual ~XObjReadAnm();              // _08
	virtual void vf18(XObjReadReq* req); // _18
};

extern "C" {
extern XObjReadAll sXObjReadAll;
extern XObjReadOne sXObjReadOne;
extern XObjReadAnm sXObjReadAnm;
}

#endif
