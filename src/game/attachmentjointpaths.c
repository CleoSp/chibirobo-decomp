#include "types.h"

const s32 sAttachmentResetJointPath[] = { 0, 1, 0, 1, 2, 0, 0, 5 };
#pragma section R ".sdata2" \
                  ".sbss2"
__declspec(section ".sdata2") const s32 sAttachmentYawRoot;
__declspec(section ".sdata2") const s32 sAttachmentTranslationRoot;
