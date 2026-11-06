#include "game/xobj.h"
#include "game/xobjread.h"

// The retail reader vtables and their three global objects belong to the same translation unit.
XObjRead::~XObjRead()
{
}

// __sinit at 0x80135010 builds these and registers their dtors
XObjReadAll gAllObjectsReader;
XObjReadOne gSingleObjectReader;
XObjReadAnm gObjectAnimationReader;

// The animation reader destructor precedes the other readers in retail .text.
#pragma section RX ".text.readanm"
__declspec(section ".text.readanm") XObjReadAnm::~XObjReadAnm()
{
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

#pragma section RX ".text.readwait"
/**
 * @note Address: 0x80131C6C
 * @note Size: 0xC0
 */
// any read still waiting
extern "C" __declspec(section ".text.readwait") BOOL vf1C__8XObjReadFv(XObjRead* __restrict reader)
{
	for (s32 i = 0; i < 8; ++i) {
		if (reader->mReqs[i].mObj != NULL) {
			return TRUE;
		}
	}
	return FALSE;
}

/**
 * @note Address: 0x80131D2C
 * @note Size: 0x280
 */
extern "C" __declspec(section ".text.readwait") BOOL HasPendingObjectReads()
{
	return vf1C__8XObjReadFv(&gAllObjectsReader) || vf1C__8XObjReadFv(&gSingleObjectReader) || vf1C__8XObjReadFv(&gObjectAnimationReader);
}

/**
 * @note Address: 0x80131FAC
 * @note Size: 0x94
 */
// XObjReadAnm::vf18, reads the object's anim archive
extern "C" __declspec(section ".text.readwait") void vf18__11XObjReadAnmFP11XObjReadReq(XObjReadAnm* reader, XObjReadReq* req)
{
	XObj* object     = (XObj*)req->mObj;
	const char* path = (const char*)req->mInfo;
	while (!object->vfA0(path)) {
		OSYieldThread();
	}
	req->mObj = NULL;
	object->mStatus.mFlags &= ~0x0080000000000000ULL;
}
