#include "game/hsd_jobj.h"
#include "game/fbtexeff.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

extern "C" {
// the five mirrors, a global in CGame's file
extern FbMirrorEx lbl_8035FA24[5];
FbMirrorEx* __dt__10FbMirrorExFv(FbMirrorEx* mirror, s32 flags);
}

// todo: weak copy
/**
 * @note Address: 0x800195C8
 * @note Size: 0x38
 */
extern "C" void fn_800195C8(void)
{
	__destroy_arr(lbl_8035FA24, (ConstructorDestructor*)__dt__10FbMirrorExFv, sizeof(FbMirrorEx), 5);
}
