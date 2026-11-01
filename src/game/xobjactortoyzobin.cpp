#include "game/xobjactor.h"
#include "game/xobjs.h"

/**
 * @note Address: 0x8011A050
 * @note Size: 0x340
 */
extern "C" void fn_8011A050(XObjActorToy* object, u32 frames)
{
	CJObj joint;
	switch (object->_990) {
	case 0:
		if (object->_9A4 > 0)
			++object->_990;
		break;
	case 1:
		s32 path[4] = { 0, 2, 4, 0 };
		joint.Attach(FindJointByPath(object->mJObj, path, 4));
		joint.SetTranslateY(-object->_9A8, 0);
		if (1.0f == object->_9A8)
			object->_990 = 0;
		break;
	}
}
