#include "game/xobjactor.h"

/**
 * @note Address: 0x8010A500
 * @note Size: 0x74
 */
extern "C" void fn_8010A500(XObjActor3* actor)
{
	switch (actor->GetKind()) {
	case 0x149:
	case 0x42B:
		actor->_980.Close();
		break;
	}
	fn_800911C0(actor);
}
