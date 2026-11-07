#pragma inline_max_size(4096)
#include "game/xobjactor.h"

#include "game/eventjointattachment.h"

/**
 * @note Address: 0x801070A4
 * @note Size: 0x498
 */
#pragma push
#pragma opt_dead_assignments off
s32 XObjActor::vf124(s32 count, s32* args)
{
	switch (args[0]) {
	case 100: {
		EventJointPath path;
		SetEventJointAttachment(*this, _998, 0x1000, args, &path);
		break;
	}
	case 101:
		if (args[1] == 0) {
			_998.mFlags &= ~0x800;
		} else {
			_998.mFlags |= 0x800;
		}
		break;
	}
	return 0;
}
#pragma pop
