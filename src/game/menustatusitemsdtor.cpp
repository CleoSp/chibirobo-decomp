#include "game/menu.h"

inline void* operator new(size_t, void* p)
{
	return p;
}

/**
 * @note Address: 0x8006331C
 * @note Size: 0x94
 */
// todo: weak copy
extern "C" MenuStatusItems* fn_8006331C(MenuStatusItems* items, s16 flags)
{
	if (items != NULL) {
		items->~MenuStatusItems();
		if (flags > 0)
			::operator delete(items);
	}
	return items;
}

/**
 * @note Address: 0x800633B0
 * @note Size: 0x54
 */
// todo: weak copy
extern "C" MenuStatusItem* fn_800633B0(MenuStatusItem* item, s16 flags)
{
	if (item != NULL) {
		item->~MenuStatusItem();
		if (flags > 0)
			::operator delete(item);
	}
	return item;
}

/**
 * @note Address: 0x80063404
 * @note Size: 0x18
 */
// todo: weak copy
extern "C" MenuStatusItem* fn_80063404(MenuStatusItem* item)
{
	return new (item) MenuStatusItem;
}
