#include "game/xchr.h"

extern "C" {
extern const char* gAttachmentAnimationPaths[];
extern const f32 lbl_8065D35C, lbl_8065D380, lbl_8065D43C;
}

/**
 * @note Address: 0x800B9758
 * @note Size: 0xA4
 */
// reads the attachment's anims, true once they're in
#pragma push
#pragma opt_common_subs off
extern "C" BOOL LoadPlayerAttachmentAnimation(XObjChr* chr)
{
	const char* path = gAttachmentAnimationPaths[chr->mAttachment];
	if (path != NULL) {
		if (LoadJointAnimationAsync(&chr->mAttachmentAnim, path)) {
			chr->SetAnim(17, TRUE, NULL, NULL, NULL, lbl_8065D35C, lbl_8065D380, lbl_8065D380, lbl_8065D43C);
			return TRUE;
		}
		return FALSE;
	}
	return TRUE;
}

#pragma pop
