#include "game/infoscrn.h"
#include "game/cgame.h"
#include "game/cgs.h"

// todo: infoscrn's rodata, the zero vectors its resets copy, reached off the pool base
struct InfoScrnRodata {
	u8 _00[0x48];  // _00
	Vec mReset[4]; // _48
	Vec mInit[4];  // _78
};

extern "C" {
extern InfoScrnRodata lbl_802015B0;
extern f32 lbl_8065A368;
void fn_8006AB00(InfoScrn* scrn);
void fn_8006AF34(InfoScrn* scrn, u32 frames);
}

static inline void HidePoints(InfoPointPool* pool)
{
	InfoPoint* point1 = &pool->mPoint1;
	InfoPoint* point0 = &pool->mPoint0;
	InfoPoint* point2 = &pool->mPoint2;
	point1->mVisible  = 0;
	point0->mVisible  = 0;
	point2->mVisible  = 0;
}

static inline void CopyVec(Vec& dst, const Vec& src)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

/**
 * @note Address: 0x8006D498
 * @note Size: 0xB0
 */
extern "C" void fn_8006D498(InfoScrn* scrn)
{
	fn_80199AB0((CGs*)lbl_8025C400.mGs, 0, 0);
	fn_8019B56C(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 1, 4, 5, 15);
	fn_8019B464(((CGs*)lbl_8025C400.mGs)->mCurRenderMode, 0, 3, 0);
	fn_8002390C(&scrn->mFade, (CGs*)lbl_8025C400.mGs);
	scrn->mWipe.Draw((CGs*)lbl_8025C400.mGs, 0);
	fn_8006AB00(scrn);
}

/**
 * @note Address: 0x8006D548
 * @note Size: 0x5C
 */
extern "C" void fn_8006D548(InfoScrn* scrn, u32 frames)
{
	fn_80023B38(&scrn->mFade, frames);
	scrn->mWipe.Update(frames);
	fn_8006AF34(scrn, frames);
}

/**
 * @note Address: 0x8006D5A4
 * @note Size: 0x280
 */
extern "C" void fn_8006D5A4(InfoScrn* scrn)
{
	InfoScrnRodata* rodata = &lbl_802015B0;
	InfoPointPool* pool    = &lbl_80366E58;
	scrn->_A8              = 0.0f;
	scrn->_AC              = 0.0f;
	scrn->_B0              = 0.0f;
	scrn->_140             = 0.0f;
	scrn->_144             = 0.0f;
	scrn->_98              = 0;
	scrn->_9C              = 0;
	scrn->_A0              = 0;
	scrn->_A4              = 0;
	scrn->_B4              = 0;
	scrn->_1D4             = 0.0f;
	scrn->_B8              = 0;
	scrn->_17C             = 0;
	scrn->_180             = 0;
	fn_800650AC(&pool->mPoint1, INFOPOINT_ID + 2);
	fn_800650AC(&pool->mPoint0, INFOPOINT_ID);
	fn_800650AC(&pool->mPoint2, INFOPOINT_ID + 1);
	HidePoints(pool);
	scrn->_BC = 0;
	scrn->_C0 = 0.0f;
	scrn->_C4 = 0.0f;
	scrn->_C8 = 0.0f;
	Vec first = rodata->mInit[0];
	CopyVec(scrn->_128, first);
	scrn->_148 = 0;
	scrn->_14C = 0.0f;
	scrn->_150 = 0;
	scrn->_154 = 0;
	scrn->_158 = 0;
	Vec second = rodata->mInit[1];
	CopyVec(scrn->_170, second);
	scrn->_16C = 1.0f;
	scrn->_188 = 0;
	scrn->_18C = 0.0f;
	Vec third  = rodata->mInit[2];
	CopyVec(scrn->_190, third);
	scrn->_19C = 0;
	scrn->_1A0 = 0;
	scrn->_1A4 = 0.0f;
	Vec fourth = rodata->mInit[3];
	CopyVec(scrn->_1A8, fourth);
	scrn->_1B4           = 0;
	scrn->_1B8           = 0.0f;
	scrn->mTimer         = 0.0f;
	scrn->mTimerShown    = 0;
	scrn->_1C4           = 0;
	scrn->mScanlineAlpha = 0.0f;
	scrn->_1CC           = 0.0f;
	scrn->_184           = 0;
	scrn->_1D0           = 0;
	scrn->_CC            = 0.0f;
	scrn->_D0            = 0.0f;
	scrn->_D4            = 0;
	scrn->_D8            = 0;
	scrn->_DC            = 1.4f;
	f32 diff             = scrn->_CC - scrn->_D0;
	for (u32 i = 0; i < 4; ++i) {
		scrn->_E0[i]  = 0.0f;
		scrn->_F0[i]  = 0;
		scrn->_100[i] = 0;
		scrn->_104[i] = diff;
		scrn->_114[i] = 0.0f;
	}
	scrn->_124     = 0;
	scrn->_134     = 0;
	scrn->_138     = 0.0f;
	scrn->_13C     = 0.0f;
	lbl_8065A368   = 1.0f;
	scrn->mItemMax = 5;
}

/**
 * @note Address: 0x8006D824
 * @note Size: 0x2C
 */
extern "C" BOOL fn_8006D824(InfoScrn*)
{
	HidePoints(&lbl_80366E58);
	return TRUE;
}

/**
 * @note Address: 0x8006D850
 * @note Size: 0x2A4
 */
extern "C" void fn_8006D850(InfoScrn* scrn)
{
	InfoScrnRodata* rodata = &lbl_802015B0;
	InfoPointPool* pool    = &lbl_80366E58;
	scrn->mFade.Reset();
	scrn->mWipe.Reset();
	scrn->_A8  = 0.0f;
	scrn->_AC  = 0.0f;
	scrn->_B0  = 0.0f;
	scrn->_140 = 0.0f;
	scrn->_144 = 0.0f;
	scrn->_98  = 0;
	scrn->_9C  = 0;
	scrn->_A0  = 0;
	scrn->_A4  = 0;
	scrn->_B4  = 0;
	scrn->_1D4 = 0.0f;
	scrn->_B8  = 0;
	scrn->_17C = 0;
	scrn->_180 = 0;
	fn_800650AC(&pool->mPoint1, INFOPOINT_ID + 2);
	fn_800650AC(&pool->mPoint0, INFOPOINT_ID);
	fn_800650AC(&pool->mPoint2, INFOPOINT_ID + 1);
	HidePoints(pool);
	scrn->_BC = 0;
	scrn->_C0 = 0.0f;
	scrn->_C4 = 0.0f;
	scrn->_C8 = 0.0f;
	Vec first = rodata->mReset[0];
	CopyVec(scrn->_128, first);
	scrn->_148 = 0;
	scrn->_14C = 0.0f;
	scrn->_150 = 0;
	scrn->_154 = 0;
	scrn->_158 = 0;
	Vec second = rodata->mReset[1];
	CopyVec(scrn->_170, second);
	scrn->_16C = 1.0f;
	scrn->_188 = 0;
	scrn->_18C = 0.0f;
	Vec third  = rodata->mReset[2];
	CopyVec(scrn->_190, third);
	scrn->_19C = 0;
	scrn->_1A0 = 0;
	scrn->_1A4 = 0.0f;
	Vec fourth = rodata->mReset[3];
	CopyVec(scrn->_1A8, fourth);
	scrn->_1B4           = 0;
	scrn->_1B8           = 0.0f;
	scrn->mTimer         = 0.0f;
	scrn->mTimerShown    = 0;
	scrn->_1C4           = 0;
	scrn->mScanlineAlpha = 0.0f;
	scrn->_1CC           = 0.0f;
	scrn->_184           = 0;
	scrn->_1D0           = 0;
	scrn->_CC            = 0.0f;
	scrn->_D0            = 0.0f;
	scrn->_D4            = 0;
	scrn->_D8            = 0;
	scrn->_DC            = 1.4f;
	f32 diff             = scrn->_CC - scrn->_D0;
	for (u32 i = 0; i < 4; ++i) {
		scrn->_E0[i]  = 0.0f;
		scrn->_F0[i]  = 0;
		scrn->_100[i] = 0;
		scrn->_104[i] = diff;
		scrn->_114[i] = 0.0f;
	}
	scrn->_124     = 0;
	scrn->_134     = 0;
	scrn->_138     = 0.0f;
	scrn->_13C     = 0.0f;
	lbl_8065A368   = 1.0f;
	scrn->mItemMax = 5;
}
