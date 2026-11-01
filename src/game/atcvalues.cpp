#include "game/xchr.h"

// todo: cleanup
static inline s32 FindAttachment(const AttachmentValues* values, s32 id)
{
	for (s32 i = 0; i < 9; ++i) {
		if (values->mEntries[i].mId == id)
			return i;
	}
	return -1;
}

// the count for an attachment id, -1 when it isn't in the list
/**
 * @note Address: 0x8009C1BC
 * @note Size: 0x10C
 */
extern "C" s32 fn_8009C1BC(AttachmentValues* values, s32 id)
{
	s32 index = FindAttachment(values, id);
	if (index >= 0) {
		fn_8009C398(values);
		return values->mEntries[index].mValue;
	}
	return -1;
}

/**
 * @note Address: 0x8009C2C8
 * @note Size: 0xD0
 */
extern "C" void fn_8009C2C8(AttachmentValues* values, s32 id, s32 value)
{
	s32 index = FindAttachment(values, id);
	if (index >= 0)
		values->mEntries[index].mValue = value;
}
