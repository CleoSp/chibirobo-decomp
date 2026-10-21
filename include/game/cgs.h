#ifndef _GAME_CGS_H
#define _GAME_CGS_H

#include "game/hsd_cobj.h"
#include "game/cprimitive.h"
#include "game/cfont.h"
extern "C" {
#include <sysdolphin/tev.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/texp.h>

// hsd's gx headers don't have these two, the tev stage setup needs them
void GXSetNumTexGens(u8 nTexGens);
void GXSetTexCoordGen2(u32 dst_coord, u32 func, u32 src_param, u32 mtx, u8 normalize, u32 pt_texmtx);
}

// the game's graphics state. class names are RTTI, layout from the constructor at
// 0x8001B9B8. CBase derives from it, so every CGs call is on the game object lbl_8025C400.
// the wii version splits the screen and channel parts out into cgs_scrn.cpp and cgs_chan.cpp

class CCamera;
class CLight;

// base classes are from RTTI

class CGsScreen : public GXRenderModeObj {
public:
	s32 mMode; // _3C, index into the mode table, -1 before the first set
	// _40 vtable

	virtual ~CGsScreen() { } // _08
};
typedef char CGsScreen_size_check[sizeof(CGsScreen) == 0x44 ? 1 : -1];

// CGsRenderMode::Init args
struct GsBlendMode {
	s32 mType;      // _00
	s32 mSrcFactor; // _04
	s32 mDstFactor; // _08
	s32 mLogicOp;   // _0C
};
struct GsAlphaCompare {
	s32 mComp0; // _00
	u8 mRef0;   // _04
	s32 mOp;    // _08
	s32 mComp1; // _0C
	u8 mRef1;   // _10
};
struct GsDstAlpha {
	u8 mEnable; // _00
	u8 mValue;  // _01
};
struct GsZMode {
	u8 mEnable; // _00
	s32 mFunc;  // _04
	u8 mUpdate; // _08
};
struct GsLineWidth {
	u8 mWidth;    // _00
	s32 mTexOffs; // _04
};

// pixel engine state, each part goes out to HSD when its dirty flag is set
class CGsRenderMode : public HSD_PEDesc {
public:
	BOOL mPEDirty;     // _0C
	BOOL mCullDirty;   // _10
	BOOL mLineDirty;   // _14
	BOOL mPointDirty;  // _18
	s32 mCullMode;     // _1C
	u8 mLineWidth;     // _20
	s32 mLineTexOffs;  // _24
	u8 mPointSize;     // _28
	s32 mPointTexOffs; // _2C
	// _30 vtable

	virtual ~CGsRenderMode() { } // _08

	void Invalidate()
	{
		if (!mPEDirty) {
			mPEDirty = TRUE;
		}
		if (!mCullDirty) {
			mCullDirty = TRUE;
		}
		if (!mLineDirty) {
			mLineDirty = TRUE;
		}
		if (!mPointDirty) {
			mPointDirty = TRUE;
		}
	}

	void Setup(BOOL force)
	{
		if (force || mPEDirty) {
			mPEDirty = FALSE;
			HSD_StateInvalidate(HSD_STATE_RENDER_MODE);
			HSD_SetupPEMode(0, this);
		}
		if (force || mCullDirty) {
			mCullDirty = FALSE;
			HSD_StateSetCullMode(mCullMode);
		}
		if (force || mLineDirty) {
			mLineDirty = FALSE;
			HSD_StateSetLineWidth(mLineWidth, mLineTexOffs);
		}
		if (force || mPointDirty) {
			mPointDirty = FALSE;
			HSD_StateSetPointSize(mPointSize, mPointTexOffs);
		}
	}
};
typedef char CGsRenderMode_size_check[sizeof(CGsRenderMode) == 0x34 ? 1 : -1];

// CGsTevStage::Init args, in GXSetTevColorIn/GXSetTevColorOp order
struct GsTevOrder {
	u32 mCoord; // _00
	u32 mMap;   // _04
	u32 mColor; // _08
};
struct GsTevIn {
	u32 mA; // _00
	u32 mB; // _04
	u32 mC; // _08
	u32 mD; // _0C
};
struct GsTevOp {
	u32 mOp;    // _00
	u32 mBias;  // _04
	u32 mScale; // _08
	u8 mClamp;  // _0C
	u32 mReg;   // _10
};

// one tev stage with its texgen
class CGsTevStage : public HSD_TevDesc {
public:
	BOOL mStageDirty;   // _74
	BOOL mNumGensDirty; // _78
	BOOL mTexGenDirty;  // _7C
	u8 mNumTexGens;     // _80
	u32 mTexCoord;      // _84
	u32 mTexGenFunc;    // _88
	u32 mTexGenSrc;     // _8C
	u32 mTexMtx;        // _90
	// _94 vtable

	virtual ~CGsTevStage() { } // _08

	void Invalidate()
	{
		if (!mStageDirty) {
			mStageDirty = TRUE;
		}
		if (!mNumGensDirty) {
			mNumGensDirty = TRUE;
		}
		if (!mTexGenDirty) {
			mTexGenDirty = TRUE;
		}
	}

	void Setup(BOOL force)
	{
		if (force || mStageDirty) {
			mStageDirty = FALSE;
			HSD_StateInvalidate(HSD_STATE_TEV_STAGE);
			HSD_StateInvalidate(HSD_STATE_TEV_REGISTER);
			HSD_StateInvalidate(HSD_STATE_TEX_COORD_GEN);
			HSD_SetupTevStageAll(this);
		}
		if (force || mNumGensDirty) {
			HSD_StateInvalidate(HSD_STATE_TEX_COORD_GEN);
			GXSetNumTexGens(mNumTexGens);
		}
		if (force || mTexGenDirty) {
			HSD_StateInvalidate(HSD_STATE_TEV_REGISTER);
			HSD_StateInvalidate(HSD_STATE_TEX_COORD_GEN);
			GXSetTexCoordGen2(mTexCoord, mTexGenFunc, mTexGenSrc, mTexMtx, GX_FALSE, GX_PTIDENTITY);
		}
	}
	void SetOrder(s32 coord, s32 map, s32 color)
	{
		if (this->coord != coord) {
			this->coord = coord;
			Invalidate();
		}
		if (this->map != map) {
			this->map = map;
			Invalidate();
		}
		if (this->color != color) {
			this->color = color;
			Invalidate();
		}
	}
	void SetNumTexGens(u8 num)
	{
		mNumTexGens   = num;
		mNumGensDirty = TRUE;
	}
	void SetTexGen(u32 coord, u32 func, u32 src, u32 mtx)
	{
		mTexCoord    = coord;
		mTexGenFunc  = func;
		mTexGenSrc   = src;
		mTexMtx      = mtx;
		mTexGenDirty = TRUE;
	}
};
typedef char CGsTevStage_size_check[sizeof(CGsTevStage) == 0x98 ? 1 : -1];

// channel setup args, in HSD_Chan's order
struct GsChanCtrl {
	u8 mEnable;     // _00
	s32 mAmbSrc;    // _04
	s32 mMatSrc;    // _08
	s32 mLightMask; // _0C
	s32 mDiffFn;    // _10
	s32 mAttnFn;    // _14
};

class CGsChannel : public HSD_Chan {
public:
	BOOL mDirty; // _30
	// _34 vtable

	virtual ~CGsChannel() { } // _08

	void Setup(BOOL force)
	{
		if (force || mDirty) {
			mDirty = FALSE;
			HSD_SetupChannelAll(this);
		}
	}
};
typedef char CGsChannel_size_check[sizeof(CGsChannel) == 0x38 ? 1 : -1];

class CGs {
public:
	CGsScreen mScreen;             // _00
	CCamera* mCamera;              // _44, made current by fn_8019997C
	CLight* mLight;                // _48
	CGsRenderMode mRenderMode;     // _4C
	CGsTevStage mTevStages[2];     // _80
	CGsChannel mChannels[4];       // _1B0
	CGsRenderMode* mCurRenderMode; // _290, mRenderMode unless someone set their own
	CGsTevStage* mCurTevStage;     // _294
	CGsChannel* mCurChannel;       // _298
	BOOL _29C;                     // _29C, compared signed
	CFont mFont;                   // _2A0
	CTexObj mTex;                  // _3E0
	CSprite mSprite;               // _4E0
	CLine mLine;                   // _668
	CTriangle mTriangle;           // _7D4
	CCircle mCircle;               // _940
	CTile mTile;                   // _ABC
	CSprite mSprite2;              // _C40
	CCube mCube;                   // _DC8
	COctahedron mOctahedron;       // _F30
	CDodeca mDodeca;               // _1098
	CIcosahedron mIcosahedron;     // _1200
	CSphere mSphere;               // _1368
	CCylinder mCylinder;           // _14D0
	CTorus mTorus;                 // _1638
	s32 _17A0;                     // _17A0, 5 after Init
	s32 _17A4;                     // _17A4, Init's argument
	f32 _17A8;                     // _17A8
	f32 _17AC;                     // _17AC
	// _17B0 vtable

	virtual ~CGs(); // _08
};
typedef char CGs_size_check[sizeof(CGs) == 0x17B4 ? 1 : -1];

#endif
