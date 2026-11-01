#include "game/xchr.h"

extern "C" const char* lbl_802262AC[];

/**
 * @note Address: 0x800B9758
 * @note Size: 0xA4
 */
// reads the attachment's anims, true once they're in
extern "C" BOOL fn_800B9758(XObjChr* chr)
{
	const char* path = lbl_802262AC[chr->mAttachment];
	if (path != NULL) {
		if (fn_8008A38C(&chr->mAttachmentAnim, path)) {
			chr->SetAnim(17, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}
