#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactorfamily.h"

// lets go of what she carries
void XObjActorMama::Drop()
{
	s32 kind = -1;
	if (TestFlag(_C8C, 4)) {
		_C8C.mFlags &= ~4;
		kind = 0x7D;
	} else if (TestFlag(_C8C, 1)) {
		_C8C.mFlags &= ~1;
		kind = 0x40;
	} else if (TestFlag(_C8C, 2)) {
		_C8C.mFlags &= ~2;
		kind = 0x5B;
	} else if (TestFlag(_C8C, 8)) {
		_C8C.mFlags &= ~8;
		kind = 0x196;
	} else if (TestFlag(_C8C, 0x10)) {
		_C8C.mFlags &= ~0x10;
		kind = 0x1A1;
	} else if (TestFlag(_C8C, 0x20)) {
		_C8C.mFlags &= ~0x20;
		kind = 0x45B;
	}
	if (kind < 0) {
		return;
	}
	switch (kind) {
	case 0x40:
		for (s32 i = 0; i < 64; i++) {
			XObj* obj = GAME_POOL_9734->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x5B:
	case 0x7D:
	case 0x196:
	case 0x45B:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	case 0x1A1:
		for (s32 i = 0; i < 288; i++) {
			XObj* obj = GAME_POOL_984C->Get(i);
			if (obj->GetDescKind() == 0x1A0) {
				obj->mFlags &= ~0x10000ULL;
			} else if (kind == obj->GetDescKind()) {
				obj->mFlags &= ~0x10000ULL;
			}
		}
		break;
	}
}
