#include "game/xobjactor.h"

/**
 * @note Address: 0x8010A500
 * @note Size: 0x74
 */
extern "C" void Release__10XObjActor3Fv(XObjActor3* actor)
{
	switch (actor->GetKind()) {
	case 0x149:
	case 0x42B:
		actor->_980.Close();
		break;
	}
	Release__4XObjFv(actor);
}
