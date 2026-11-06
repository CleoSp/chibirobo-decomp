#include "game/xobjattachment.h"
#include "game/cvec.h"

#pragma inline_max_size(4096)
static inline void AddStep(CVec& position, Vec step)
{
	position += step;
}

template <int direction>
static inline void AdvanceCordSpark(XObjPlugCordNode* node, XObjPlugCordPoint* points, s32 last)
{
	node->mDir = points[node->mIndex + direction].mPos - points[node->mIndex].mPos;
	node->mDir.Normalize();
	node->mPos    = points[node->mIndex].mPos + node->mDir * node->mDistance;
	f32 remaining = PSVECDistance(&node->mPos, &points[node->mIndex + direction].mPos);
	if (remaining > node->mStep.Get()) {
		AddStep(node->mPos, node->mDir * node->mStep.Get());
		node->mDistance += node->mStep.Get();
	} else {
		if (node->mIndex + direction == last)
			node->mState = 0;
		node->mIndex += direction;
		node->mDir = points[node->mIndex + direction].mPos - points[node->mIndex].mPos;
		node->mDir.Normalize();
		node->mPos      = points[node->mIndex].mPos + node->mDir * (node->mStep.Get() - remaining);
		node->mDistance = node->mStep.Get() - remaining;
		node->mStep.Step(1.0f);
	}
}

extern "C" void UpdatePlugCordSpark(XObjPlugCordNode* node, XObjPlugCordPoint* points, s32 count, f32)
{
	if (!node->mState)
		return;
	switch (node->mMode) {
	case 1: {
		AdvanceCordSpark<-1>(node, points, 0);
#if !defined(VERSION_JP)
		f32 remaining = PSVECDistance(&node->mPos, &points[node->mIndex - 1].mPos);
		if (remaining > PSVECDistance(&points[node->mIndex].mPos, &points[node->mIndex - 1].mPos) && node->mIndex - 1 == 0)
			node->mState = 0;
#endif
		break;
	}
	case -1:
		AdvanceCordSpark<1>(node, points, count - 2);
		break;
	}
}
