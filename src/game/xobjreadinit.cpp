#include "game/xobj.h"
#include "game/xobjread.h"

// The retail reader vtables and their three global objects belong to the same translation unit.
#pragma section RX ".text.readerbasedtor"
__declspec(section ".text.readerbasedtor") XObjRead::~XObjRead()
{
}

// The reader methods address their globals relative to the start of this BSS block.
extern "C" {
#if defined(VERSION_GGTJ01_01)
u8 gObjectReaderBss[0x134];
#else
u8 gObjectReaderBss[0x140];
#endif
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

#pragma section RX ".text.readerdtors"
/**
 * @note Address: 0x80134E94
 * @note Size: 0x84
 */
__declspec(section ".text.readerdtors") XObjReadOne::~XObjReadOne()
{
}

/**
 * @note Address: 0x80134F18
 * @note Size: 0x84
 */
__declspec(section ".text.readerdtors") XObjReadAll::~XObjReadAll()
{
}

#pragma section RX ".text.readcheck"
#pragma section RX ".text.readwait"
#pragma section RX ".text.readanimation"
/**
 * @note Address: 0x80131C6C
 * @note Size: 0xC0
 */
extern "C" __declspec(section ".text.readcheck") BOOL HasPendingReads__8XObjReadFv(XObjRead* __restrict reader)
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
	return HasPendingReads__8XObjReadFv(&gAllObjectsReader) || HasPendingReads__8XObjReadFv(&gSingleObjectReader)
	    || HasPendingReads__8XObjReadFv(&gObjectAnimationReader);
}

/**
 * @note Address: 0x80131FAC
 * @note Size: 0x94
 */
__declspec(section ".text.readanimation") void XObjReadAnm::ProcessRequest(XObjReadReq* req)
{
	XObj* object     = (XObj*)req->mObj;
	const char* path = (const char*)req->mInfo;
	while (!object->LoadAnimationArchiveAsync(path)) {
		OSYieldThread();
	}
	req->mObj = NULL;
	object->mStatus.mFlags &= ~0x0080000000000000ULL;
}
