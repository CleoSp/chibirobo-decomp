#include "game/ccameraaction.h"
#include "game/cobj.h"

void operator delete(void* object);

extern "C" void* __vt__16CCameraActionMap[];

/**
 * @note Address: 0x80009F20
 * @note Size: 0x340
 */
// todo: weak copy
extern "C" CCameraActionMap* __dt__16CCameraActionMapFv(CCameraActionMap* map, s32 flags)
{
	if (map != NULL) {
		*(void***)((u8*)map + 0x3C) = __vt__16CCameraActionMap;
		map->_11E0.CStatus::~CStatus();
		map->_11D8.CStatus::~CStatus();
		map->_FB8.CVec::~CVec();
		map->_FA8.CVec::~CVec();
		map->_F98.CVec::~CVec();
		map->_F88.CVec::~CVec();
		map->_F78.CVec::~CVec();
		map->_F68.CVec::~CVec();
		map->_F2C.CVectorEaseOutDivideInterpolation::~CVectorEaseOutDivideInterpolation();
		map->_E7C.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		map->_E68.CInterpolationEaseOutDivide::~CInterpolationEaseOutDivide();
		map->_E34.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		map->_E14.CInterpolationEaseInOut::~CInterpolationEaseInOut();
		map->_DB8.CVectorEaseInOutInterpolation::~CVectorEaseInOutInterpolation();
		map->_D8.CVec::~CVec();
		map->_C8.CInterpolationEaseOutDivide::~CInterpolationEaseOutDivide();
		// _8C to _BC is one CObjVec, the header splits it into three CVecs
		((CObjVec*)&map->_8C)->CObjVec::~CObjVec();
		map->_70.CVec::~CVec();
		map->_60.CVec::~CVec();
		map->_48.CVec::~CVec();
		map->CCameraActionAreaExM::~CCameraActionAreaExM();
		if ((s16)flags > 0)
			::operator delete(map);
	}
	return map;
}
