#include "game/xchr.h"

extern "C" {
void fn_8009C8E0(AttachmentValues* values);
}

/**
 * @note Address: 0x8009CEA0
 * @note Size: 0x34
 */
extern "C" void fn_8009CEA0(AttachmentValues* values)
{
	fn_8009C8E0(values);
	fn_8009C398(values);
}

// the eight attachments in ring order, then a refresh
/**
 * @note Address: 0x8009CED4
 * @note Size: 0x60
 */
extern "C" void fn_8009CED4(AttachmentValues* values)
{
	values->mEntries[0].mId = 1;
	values->mEntries[1].mId = 2;
	values->mEntries[2].mId = 3;
	values->mEntries[3].mId = 4;
	values->mEntries[4].mId = 5;
	values->mEntries[5].mId = 6;
	values->mEntries[6].mId = 7;
	values->mEntries[7].mId = 8;
	fn_8009CEA0(values);
}
