#include "game/octamenu.h"
#include "game/cgame.h"
#include "game/cparticle.h"
#include <math.h>
#include "game/gmath.h"

extern "C" {
extern const s32 lbl_8066021C;
extern const s32 lbl_80660220[2];
void fn_8009F4F8(CJObj* object, s32 depth, s32* path, f32 x, f32 y, f32 z);
}

/**
 * @note Address: 0x800C2D18
 * @note Size: 0x98
 */
extern "C" void fn_800C2D18(OctaMenuBob* bob)
{
	bob->mObj = NULL;
	bob->_38  = 0;
	fn_80024E0C(&bob->mPtcl);
	bob->_C0 = 0.0f;
	bob->_D8 = 0.0f;
	if (bob->mGen != NULL) {
		((CParticle*)lbl_8025C400.mParticle)->Kill(bob->mGen);
		bob->mGen = NULL;
	}
	if (bob->mGen2 != NULL) {
		((CParticle*)lbl_8025C400.mParticle)->Kill(bob->mGen2);
		bob->mGen2 = NULL;
	}
}

/**
 * @note Address: 0x800C2DB0
 * @note Size: 0x108
 */
extern "C" void fn_800C2DB0(OctaMenuBob* bob, u32 frames, f32 rate)
{
	if (rate > 0.0f) {
		bob->mPhase  = (f32)fmod(rate * frames + bob->mPhase, (double)TWO_PI);
		bob->mHeight = 0.4f * (f32)sin(bob->mPhase) + 0.4f;
		s32 path     = lbl_8066021C;
		fn_8009F4F8(bob->mObj, 1, &path, 0.0f, bob->mHeight, 0.0f);
	} else {
		if (bob->mHeight > 0.0f) {
			f32 height   = bob->mHeight - 0.2f * frames;
			bob->mHeight = height;
			if (height <= 0.0f)
				bob->mHeight = 0.0f;
		}
		s32 path = lbl_80660220[0];
		fn_8009F4F8(bob->mObj, 1, &path, 0.0f, bob->mHeight, 0.0f);
	}
}
