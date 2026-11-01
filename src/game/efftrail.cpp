#include "types.h"
#include <math.h>
#include "game/gmath.h"

// one point of a spiral trail
struct EffTrailPoint {
	f32 mX;      // _00
	f32 mY;      // _04
	f32 mZ;      // _08
	f32 mAmount; // _0C, fades to 0
	u32 _10;     // _10
	f32 mPhase;  // _14
};
STATIC_ASSERT(sizeof(EffTrailPoint) == 0x18);

// two spirals rising off chibi robo, the newest point first
struct EffTrail {
	BOOL mActive;                // _00
	EffTrailPoint mPoint[2][10]; // _04
};
STATIC_ASSERT(sizeof(EffTrail) == 0x1E4);

/**
 * @note Address: 0x800940A4
 * @note Size: 0x298
 */
extern "C" void fn_800940A4(EffTrail* trail, u32 frames)
{
	if (trail->mActive != 0) {
		f32 total     = 0.0f;
		f32 fade      = 0.02f * frames;
		f32 phaseStep = DEG2RAD * 9.0f * frames;
		f32 rise      = 0.3f * frames;
		for (s32 i = 0; i < 2; ++i) {
			EffTrailPoint* point = trail->mPoint[i];
			for (s32 j = 9; j > 0; --j) {
				point[j].mX      = point[j - 1].mX;
				point[j].mY      = point[j - 1].mY;
				point[j].mZ      = point[j - 1].mZ;
				point[j].mAmount = point[j - 1].mAmount;
				point[j].mPhase  = point[j - 1].mPhase;
			}
			point[0].mAmount -= fade;
			if (point[0].mAmount <= 0.0f)
				point[0].mAmount = 0.0f;
			point[0].mPhase += phaseStep;
			f32 sine    = (f32)sin(point[0].mPhase);
			point[0].mX = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * sine));
			f32 cosine  = (f32)cos(point[0].mPhase);
			point[0].mZ = point[0].mAmount * (15.0f * ((1.0f - point[0].mAmount) * cosine));
			point[0].mY += rise;
			for (s32 j = 0; j < 10; ++j)
				total += point[j].mAmount;
		}
		if (total == 0.0f)
			trail->mActive = 0;
	}
}
