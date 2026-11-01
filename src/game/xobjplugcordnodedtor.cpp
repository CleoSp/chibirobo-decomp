#include "game/xobjattachment.h"

void operator delete(void* object);

/**
 * @note Address: 0x800BCCF4
 * @note Size: 0x90
 */
// todo: weak copy
extern "C" XObjPlugCordNode* fn_800BCCF4(XObjPlugCordNode* node, s32 flags)
{
	if (node != NULL) {
		node->mDir.CVec::~CVec();
		node->mStep.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		node->mPos.CVec::~CVec();
		if ((s16)flags > 0)
			::operator delete(node);
	}
	return node;
}
