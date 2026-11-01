#include "game/cobj.h"
#include "game/cjobjset.h"

#include "game/ut.h"
#include "game/ccamera.h"
#include "game/ctpl.h"

// Constructor views: unnamed storage represents fields not used during construction.
// The Japanese CXObjIntp base is eight bytes smaller; derived storage is unchanged.

class XObj : public CXObjIntp {
public:
	u8 mActorStorage[0x758];
	XObj();
	virtual ~XObj();
	virtual void vf90();
	virtual void vf94();
};

class AnimSfx {
public:
	u8 mState[12];
	virtual void Play();
};

class XObjFree : public XObj {
public:
	u8 mUnknown980[0x10];
	XObjFree();
};
typedef char XObjFree_size_check[sizeof(XObjFree) == 0x990 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjFlags : public XObj {
public:
	void* mFlags;
	u8 mUnknown984[0x4];
	XObjFlags();
};
typedef char XObjFlags_size_check[sizeof(XObjFlags) == 0x988 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjSpiderBoss : public XObj {
public:
	u8 mUnknown980[0x34];
	CStatus mStatus;
	u8 mUnknown9BC[0xDC];
	AnimSfx mSound;
	u8 mUnknownAA8[0x1C0];
	XObjSpiderBoss();
};
typedef char XObjSpiderBoss_size_check[sizeof(XObjSpiderBoss) == 0xC68 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjSpiderSmall : public XObj {
public:
	u8 mUnknown980[0x34];
	CStatus mStatus;
	u8 mUnknown9BC[0x90];
	CVec mPosition;
	CVec mTarget;
	u8 mUnknownA6C[0x10];
	CStatus mAction;
	u8 mUnknownA84[0x44];
	AnimSfx mSound;
	XObjSpiderSmall();
};
typedef char XObjSpiderSmall_size_check[sizeof(XObjSpiderSmall) == 0xAD8 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjSpider : public XObj {
public:
	u8 mUnknown980[0x30];
	CStatus mStatus;
	u8 mUnknown9B8[0xD4];
	CVec mPosition;
	CVec mTarget;
	u8 mUnknownAAC[0x10];
	CStatus mAction;
	u8 mUnknownAC4[0x4];
	CVec mPosition2;
	CVec mTarget2;
	u8 mUnknownAE8[0x10];
	CStatus mAction2;
	u8 mUnknownB00[0xA8];
	AnimSfx mSound;
	u8 mUnknownBB8[0x1C0];
	XObjSpider();
};
typedef char XObjSpider_size_check[sizeof(XObjSpider) == 0xD78 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActor : public XObj {
public:
	u8 mUnknown980[0x18];
	CStatus mStatus;
	u8 mUnknown9A0[0xEC];
	AnimSfx mSound;
	u8 mUnknownA9C[0x1DC];
	XObjActor();
};
typedef char XObjActor_size_check[sizeof(XObjActor) == 0xC78 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorDeca : public XObjActor {
public:
	u8 mUnknownC78[0x18];
	CStatus mStatus0;
	CStatus mStatus1;
	u8 mUnknownCA0[0x8];
	XObjActorDeca();
};
typedef char XObjActorDeca_size_check[sizeof(XObjActorDeca) == 0xCA8 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorTao : public XObjActor {
public:
	u8 mUnknownC78[0x14];
	CStatus mStatus0;
	u8 mUnknownC94[0x6C];
	XObjActorTao();
};
typedef char XObjActorTao_size_check[sizeof(XObjActorTao) == 0xD00 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorPapa : public XObjActor {
public:
	u8 mUnknownC78[0x14];
	CStatus mStatus0;
	u8 mUnknownC94[0x4];
	XObjActorPapa();
};
typedef char XObjActorPapa_size_check[sizeof(XObjActorPapa) == 0xC98 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorMama : public XObjActor {
public:
	u8 mUnknownC78[0x14];
	CStatus mStatus0;
	u8 mUnknownC94[0x4];
	XObjActorMama();
};
typedef char XObjActorMama_size_check[sizeof(XObjActorMama) == 0xC98 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorJenny : public XObjActor {
public:
	u8 mUnknownC78[0x18];
	CStatus mStatus0;
	CStatus mStatus1;
	u8 mUnknownCA0[0x8];
	XObjActorJenny();
};
typedef char XObjActorJenny_size_check[sizeof(XObjActorJenny) == 0xCA8 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjActorToy : public XObjActor {
public:
	u8 mUnknownC78[0xC];
	CStatus mState;
	u8 mUnknownC8C[0x4];
	XObj mPart0;
	XObj mPart1;
	u8 mUnknown1F90[0x4];
	CVec mPosition;
	u8 mUnknown1FA4[0xF4];
	CStatus mExtraState;
	XObjActorToy();
};
typedef char XObjActorToy_size_check[sizeof(XObjActorToy) == 0x20A0 + 3 * (sizeof(CXObjIntp) - 0x228) ? 1 : -1];

class XObjLeticker : public XObj {
public:
	u8 mUnknown980[0xBC];
	AnimSfx mSound;
	u8 mUnknownA4C[0x4];
	XObjLeticker();
};
typedef char XObjLeticker_size_check[sizeof(XObjLeticker) == 0xA50 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjSound : public XObj {
public:
	AnimSfx mSound;
	u8 mUnknown990[0x10];
	XObjSound();
};
typedef char XObjSound_size_check[sizeof(XObjSound) == 0x9A0 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjAttachment : public XObj {
public:
	XObjAttachment();
};
typedef char XObjAttachment_size_check[sizeof(XObjAttachment) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjOX : public XObjAttachment {
public:
	XObjOX();
};
typedef char XObjOX_size_check[sizeof(XObjOX) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjRadar : public XObjAttachment {
public:
	XObjRadar();
};
typedef char XObjRadar_size_check[sizeof(XObjRadar) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjPopper : public XObjAttachment {
public:
	XObjPopper();
};
typedef char XObjPopper_size_check[sizeof(XObjPopper) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjCopter : public XObjAttachment {
public:
	XObjCopter();
};
typedef char XObjCopter_size_check[sizeof(XObjCopter) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

class XObjWireEff : public XObj {
public:
	XObjWireEff();
};
typedef char XObjWireEff_size_check[sizeof(XObjWireEff) == 0x980 - 0x228 + sizeof(CXObjIntp) ? 1 : -1];

struct ArmyShot {
	u8 mUnknown0[8];
	CVec mPosition;
	CVec mVelocity;
	u8 mUnknown28[8];
	ArmyShot();
	~ArmyShot();
};
class XObjArmy : public XObj {
public:
	u8 mUnknown980[0x20];
	CStatus mStatus;
	CVec mPosition;
	u32 mUnknown9B8;
	ArmyShot mShots[5];
	AnimSfx mSound;
	u32 mUnknownABC;
	XObjArmy();
};
struct HBoxContents {
	XObj mObject;
	HBoxContents();
};
class XObjHBox : public XObj {
public:
	u8 mUnknown980[8];
	HBoxContents mContents;
	u8 mUnknown1308[0x10];
	XObjHBox();
};
extern u8 lbl_8037E1DC[], lbl_8037DDBC[], lbl_8037DFCC[];
class XObjDust : public XObjFlags {
public:
	u8 mUnknown988[0x58];
	XObjDust();
};
class XObjItem : public XObjFlags {
public:
	XObjItem();
};
class XObjCoin : public XObjFlags {
public:
	u8 mUnknown988[0x68];
	XObjCoin();
};

class CLight {
public:
	u8 mStorage[0x354];
	CLight();
	virtual ~CLight();
};
class CTrigon {
public:
	u32 mFlags;
	u8 mVertices[0x14];
	CTrigon();
	virtual ~CTrigon();
};
class XObjMap : public XObj {
public:
	u8 mUnknown980[0x44];
	CTrigon mTrigons[16];
	u32 mUnknownB84;
	CLight mLights[8];
	u8 mUnknown2648[0x20];
	XObjMap();
};
class XObjLight : public XObj {
public:
	u32 mUnknown980;
	CLight mLights[8];
	u8 mUnknown2444[0x24];
	XObjLight();
};
class XObjCamera : public XObj {
public:
	CCamera mCamera;
	u32 mUnknownB14;
	XObjCamera();
};
struct UnkSpin {
	u8 mUnknown0[16];
	XObj mObject;
	UnkSpin();
};
class CInterfaceInterpolation {
public:
	virtual ~CInterfaceInterpolation() { }
	virtual bool Start(f32, f32, f32) = 0;
	virtual bool Step(f32)            = 0;
	virtual bool Update(u32, f32)     = 0;
	virtual f32 Get()                 = 0;
};
class CInterpolationEaseInOut : public CInterfaceInterpolation {
public:
	u8 mData[0x18];
	virtual bool Start(f32, f32, f32);
	virtual bool Step(f32);
	virtual bool Update(u32, f32);
	virtual f32 Get();
};
struct XObjPlugNode {
	u32 mUnknown0;
	CVec mPosition;
	CInterpolationEaseInOut mInterpolation;
	u32 mUnknown30;
	CVec mVelocity;
	u8 mUnknown44[8];
	XObjPlugNode();
	~XObjPlugNode();
};
class XObjPlug : public XObj {
public:
	u8 mUnknown980[0x6C];
	CVec mPosition;
	u8 mUnknown9FC[16];
	XObjPlugNode mNodes[10];
	u8 mUnknownD04[0x44];
	XObjWireEff mEffect;
	XObjPlug();
};

class CPtclOne {
public:
	u32 mFlags;
	CVec mPosition, mVelocity, mAcceleration;
	u8 mUnknown34[0x18];
	virtual ~CPtclOne();
};
class CPtclDust2 : public CPtclOne {
public:
	CPtclDust2();
	virtual ~CPtclDust2();
};
class CPtclDust3 : public CPtclOne {
public:
	CPtclDust3();
	virtual ~CPtclDust3();
};
class CPtcl {
public:
	u8 mUnknown0[0x10];
	virtual ~CPtcl();
};
struct XActPopperShot {
	u32 mUnknown0;
	CVec mPosition;
	u8 mUnknown14[0xD0];
	CPtcl mParticles;
	CPtclDust2 mDust[40];
	u8 mUnknownD78[0xC];
	XActPopperShot();
	~XActPopperShot();
};
class XObjActor3 : public XObj {
public:
	CTpl mTexture;
	u8 mUnknownB70[0x1C];
	CStatus mStatus;
	u8 mUnknownB94[0x48];
	CVec mPosition;
	u8 mUnknownBEC[0x20];
	CPtcl mParticles;
	CPtclDust3 mDust[50];
	CTexObj mTextures[2];
	u8 mUnknown1DC0[0x2C];
	AnimSfx mSound;
	u8 mUnknown1DFC[0x344];
	XObjActor3();
};

class CThread {
public:
	u8 mThread[0x318];
	void* mStack;
	u32 mStackSize;
	virtual ~CThread() { }
	virtual s32 Create(void*, void*, u32, s32, u16);
};
class CMsgQueue {
public:
	u8 mQueue[0x24];
	virtual ~CMsgQueue();
	virtual void vf0C(s32);
};
class XObjRead : public CThread {
public:
	u32 mUnknown324;
	CMsgQueue mQueue;
	u8 mRequests[0x40];
	virtual ~XObjRead();
	virtual void vf10();
	virtual void vf14();
	virtual void vf18(void*);
	virtual BOOL vf1C();
};
class XObjReadOne : public XObjRead {
public:
	virtual ~XObjReadOne();
};
class XObjReadAll : public XObjRead {
public:
	virtual ~XObjReadAll();
};
class CScrnColor {
public:
	f32 mColor[4], mTarget[4], mSpeed[4];
	CScrnColor();
	virtual ~CScrnColor();
};
XObjFree::XObjFree()
{
}

XObjSpiderBoss::XObjSpiderBoss()
{
}

XObjSpiderSmall::XObjSpiderSmall()
{
}

XObjSpider::XObjSpider()
{
}

XObjActorDeca::XObjActorDeca()
{
}

XObjActorTao::XObjActorTao()
{
}

XObjActorPapa::XObjActorPapa()
{
}

XObjActorMama::XObjActorMama()
{
}

XObjActorJenny::XObjActorJenny()
{
}

XObjActorToy::XObjActorToy()
{
}

XObjLeticker::XObjLeticker()
{
}

XObjSound::XObjSound()
{
}

XObjOX::XObjOX()
{
}

XObjRadar::XObjRadar()
{
}

XObjPopper::XObjPopper()
{
}

XObjCopter::XObjCopter()
{
}

XObjActor::XObjActor()
{
}
XObjAttachment::XObjAttachment()
{
}

XObjArmy::XObjArmy()
{
}
ArmyShot::ArmyShot()
{
}
XObjHBox::XObjHBox()
{
}
HBoxContents::HBoxContents()
{
}
XObjDust::XObjDust()
{
	mFlags = lbl_8037E1DC;
}
XObjItem::XObjItem()
{
	mFlags = lbl_8037DDBC;
}
XObjCoin::XObjCoin()
{
	mFlags = lbl_8037DFCC;
}
XObjFlags::XObjFlags()
{
	mFlags = 0;
}

XObjMap::XObjMap()
{
}
CTrigon::CTrigon()
{
	mFlags = 0;
}
XObjLight::XObjLight()
{
}
XObjCamera::XObjCamera()
{
}
UnkSpin::UnkSpin()
{
}
XObjPlug::XObjPlug()
{
}
XObjPlugNode::XObjPlugNode()
{
}
XObjWireEff::XObjWireEff()
{
}

XObjActor3::XObjActor3()
{
}
CPtclDust3::CPtclDust3()
{
}
XActPopperShot::XActPopperShot()
{
}
CPtclDust2::CPtclDust2()
{
}

CMsgQueue::~CMsgQueue()
{
}
XObjRead::~XObjRead()
{
}
XObjReadOne::~XObjReadOne()
{
}
XObjReadAll::~XObjReadAll()
{
}
CScrnColor::CScrnColor()
{
	mColor[0]  = 0;
	mColor[1]  = 0;
	mColor[2]  = 0;
	mColor[3]  = 0;
	mTarget[0] = 0;
	mTarget[1] = 0;
	mTarget[2] = 0;
	mTarget[3] = 0;
	mSpeed[0]  = 0;
	mSpeed[1]  = 0;
	mSpeed[2]  = 0;
	mSpeed[3]  = 0;
}

struct ActorPool {
	u8* mData;
	s32 mUnknown;
	s32 mCount;
	const char* mName;
	s32 mStride;
	XObj* Get(s32 index) const { return (XObj*)(mData + index * mStride); }
};
extern "C" void fn_80132D08(ActorPool* pools)
{
	ActorPool* pool = pools;
	for (s32 i = 0; i < 23; ++i, ++pool) {
		for (s32 j = 0; j < pool->mCount; ++j)
			pools[i].Get(j)->vf94();
	}
}

// Cleanup callbacks for the statically allocated actor arrays.
extern "C" {
typedef void* ActorArrayDestructor(void*, s16);
void __destroy_arr(void*, ActorArrayDestructor*, u32, u32);
extern u8 lbl_804F41F0[];
void* fn_800DD5A0(void*, s16);
extern u8 lbl_804E1060[];
void* fn_800E239C(void*, s16);
extern u8 lbl_804B9850[];
void* fn_800E2240(void*, s16);
extern u8 lbl_804ADE48[];
void* __dt__15XObjSpiderSmallFv(void*, s16);
extern u8 lbl_804A06B8[];
void* __dt__10XObjSpiderFv(void*, s16);
extern u8 lbl_804924E8[];
void* __dt__8XObjArmyFv(void*, s16);
extern u8 lbl_80490B88[];
void* __dt__13XObjActorDecaFv(void*, s16);
extern u8 lbl_8046EF00[];
void* __dt__12XObjActorToyFv(void*, s16);
extern u8 lbl_80467330[];
void* __dt__12XObjLetickerFv(void*, s16);
extern u8 lbl_80441120[];
void* fn_800E24C0(void*, s16);
extern u8 lbl_80405710[];
void* fn_800E2460(void*, s16);
extern u8 lbl_80400A00[];
void* fn_800DD32C(void*, s16);
extern u8 lbl_8039CDF0[];
void* __dt__10XObjActor3Fv(void*, s16);
extern u8 lbl_8039BAE0[];
void* __dt__4XObjFv(void*, s16);

void fn_80133B98()
{
	__destroy_arr(lbl_804F41F0, fn_800DD5A0, 0x990, 0x120);
}
void fn_80133C0C()
{
	__destroy_arr(lbl_804E1060, fn_800E239C, 0x1318, 0x10);
}
void fn_80133CB8()
{
	__destroy_arr(lbl_804B9850, fn_800E2240, 0x9e0, 0x40);
}
void fn_80133DD8()
{
	__destroy_arr(lbl_804ADE48, __dt__15XObjSpiderSmallFv, 0xad8, 0x10);
}
void fn_80133E84()
{
	__destroy_arr(lbl_804A06B8, __dt__10XObjSpiderFv, 0xd78, 0x10);
}
void fn_80133F40()
{
	__destroy_arr(lbl_804924E8, __dt__8XObjArmyFv, 0xac0, 0x15);
}
void fn_80134014()
{
	__destroy_arr(lbl_80490B88, __dt__13XObjActorDecaFv, 0xca8, 2);
}
void fn_80134248()
{
	__destroy_arr(lbl_8046EF00, __dt__12XObjActorToyFv, 0x20a0, 0xf);
}
void fn_80134300()
{
	__destroy_arr(lbl_80467330, __dt__12XObjLetickerFv, 0xa50, 0xc);
}
void fn_80134380()
{
	__destroy_arr(lbl_80441120, fn_800E24C0, 0x988, 0x40);
}
void fn_80134400()
{
	__destroy_arr(lbl_80405710, fn_800E2460, 0x9f0, 0x60);
}
void fn_80134480()
{
	__destroy_arr(lbl_80400A00, fn_800DD32C, 0x9a0, 8);
}
void fn_80134500()
{
	__destroy_arr(lbl_8039CDF0, __dt__10XObjActor3Fv, 0x2140, 0x30);
}
void fn_8013465C()
{
	__destroy_arr(lbl_8039BAE0, __dt__4XObjFv, 0x980, 2);
}
}
