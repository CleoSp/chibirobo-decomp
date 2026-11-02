#include "game/xobj.h"
#include <math.h>

/**
 * @note Address: 0x8008C7CC
 * @note Size: 0xD8
 */
extern "C" void fn_8008C7CC(XObj* object)
{
	object->_780 = NAN;
	for (s32 i = 0; object->mFloors[i].mId != -1; ++i) {
		if (object->_250[0] & (1U << i)) {
			if (__fpclassifyf(object->_780) == FP_NAN || object->_6FC[i] > object->_780) {
				object->_784 = i;
				object->_780 = object->_6FC[i];
			}
		}
	}
}
