#include "game/hsd_jobj.h"
#include "game/fbmirror.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
// the five mirrors, a global in CGame's file
extern FbMirrorEx lbl_8035FA24[5];
FbMirrorEx* fn_80019600(FbMirrorEx* mirror, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x800195C8
 * @note Size: 0x38
 */
extern "C" void fn_800195C8(void)
{
	__destroy_arr(lbl_8035FA24, (ConstructorDestructor*)fn_80019600, sizeof(FbMirrorEx), 5);
}
