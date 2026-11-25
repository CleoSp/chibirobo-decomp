#include "game/hsd_jobj.h"
#include "game/xobj.h"

extern "C" const Vec* GetCurrentEdgeWorldOffsetPosition(const XObj* object)
{
	const Vec edge = object->mEdges[object->mSelectedFloorIndex].mContactOffset;
	// Retail returns the address of the temporary sum, which dies with the frame; the caller copies it right away.
	return &(object->mMoveTranslate + edge);
}
