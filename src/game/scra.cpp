#include "game/scr.h"

// XObjSpiderBoss, the spider boss. the class name is from its vtable, nothing else has a
// name in the dol, so everything is extern "C" under its address. offsets are usa

// the boss's anim table, 0x18 byte entries
extern "C" const UnkActorSeEntry lbl_8022B9D8[];

// the placement entry the object was put down from
struct UnkSpiderPlace {
	s16 mKind;   // _00
	s16 _02;     // _02
	s32 mPos[3]; // _04, hundredths
	s32 _10;     // _10
	s32 mRotY;   // _14, degrees
	s32 _18;     // _18
	s32 _1C;     // _1C, hundredths
};

class XObjSpiderBoss : public XObj {
public:
	f32 mRotY;              // _980
	u8 _984[0x998 - 0x984]; // _984
	f32 _998;               // _998
	u8 _99C[0x9A8 - 0x99C]; // _99C
	s32 mState;             // _9A8
	s32 mStep;              // _9AC
	s32 mTimer;             // _9B0
	CStatus mStatus;        // _9B4
	s32 mPrevState;         // _9BC
	u8 _9C0[0x9D8 - 0x9C0]; // _9C0
	Vec mHome;              // _9D8
	f32 _9E4;               // _9E4
	s32 _9E8;               // _9E8
	u8 _9EC[0x9FC - 0x9EC]; // _9EC
	f32 _9FC;               // _9FC
	f32 _A00;               // _A00
	f32 _A04;               // _A04
	f32 _A08;               // _A08
	f32 _A0C;               // _A0C
	u8 _A10[0xA24 - 0xA10]; // _A10
	s32 mAnim2;             // _A24, the anim the sounds follow while flag 0x40000000 is set
	u8 _A28[0xA6C - 0xA28]; // _A28
	s32 mBlink;             // _A6C, frames of the red blink
	u8 _A70[0xA98 - 0xA70]; // _A70
	UnkActorSe mSe;         // _A98

	void SetState(s32 state)
	{
		mStep      = 0;
		mTimer     = 0;
		mPrevState = mState;
		mState     = state;
	}
};

extern "C" {

void fn_801002DC(XObjSpiderBoss* self)
{
	if ((self->mFlags & 1) && (self->mFlags & 0x10000) && !fn_800665E0(GAME_EVTMGR)) {
		s32 id;
		if (self->mStatus.mFlags & 0x40000000) {
			id = self->mAnim2;
		} else {
			id = self->mAnimId;
		}
		self->mSe.Play(id, 0xFF, GetAnimFrame(self->mJObj));
	}
}

u32 fn_80100424(XObjSpiderBoss* self)
{
	return (self->mStatus.mFlags >> 9) & 1;
}

void fn_80103B28(XObjSpiderBoss* self)
{
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x80000;
		self->mStatus.mFlags |= 0x10;
		self->mStatus.mFlags &= ~0x200;
		self->mStatus.mFlags &= ~0x100000;
		self->mStep++;
		break;
	case 1:
		break;
	}
}

// hit: blinks red through anim 0xB, then back to state 0
void fn_80103B80(XObjSpiderBoss* self, u32 frames)
{
	switch (self->mStep) {
	case 0:
		self->mStatus.mFlags |= 0x10;
		self->SetAnim(0xB, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		self->mStep++;
		self->mBlink = 0;
		self->mStatus.mFlags &= ~0x200;
		break;
	case 1:
		if (self->mBlink % 30 < 15) {
			self->SetColor(1, 0x80, 0, 0, 0.0f);
		} else {
			self->SetColor(1, 0, 0, 0, 0.0f);
		}
		self->mBlink++;
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->SetAnim(7, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			self->mBlink = 0;
			self->SetColor(1, 0, 0, 0, 0.0f);
			self->SetState(0);
			self->mStatus.mFlags |= 0x8000;
			self->mStatus.mFlags |= 0x2000000;
			self->mStatus.mFlags |= 0x40000000;
			self->mStatus.mFlags &= ~0x10;
			self->mStatus.mFlags |= 0x80000;
			self->_9E8 = 0x10;
		}
		break;
	}
}

void fn_80103EC0(XObjSpiderBoss* self)
{
	self->mStatus.mFlags &= ~0x80000;
	if ((self->mStatus.mFlags >> 25) & 1) {
		return;
	}
	if (self->mStatus.mFlags & 0x40000000) {
		self->SetState(4);
		self->_9FC = -200.0f;
		self->_A00 = 0.0f;
		self->_A04 = -200.0f;
		self->_A08 = 400.0f;
		self->_A0C = 400.0f;
	} else {
		self->SetState(3);
		self->mStep = 100;
	}
	self->mStatus.mFlags &= ~0x8000;
}

s32 fn_801051A0(XObjSpiderBoss* self, s32 a, s32 b)
{
	self->mStatus.mFlags |= 1;
	return b;
}

void fn_80105CDC(XObjSpiderBoss* self)
{
}

void fn_80105CE0(XObjSpiderBoss* self)
{
}

void fn_80105CE4(XObjSpiderBoss* self)
{
}

void fn_80105CE8(XObjSpiderBoss* self)
{
	fn_8009056C(self);
}

void fn_80105D08(XObjSpiderBoss* self)
{
	fn_80090864(self);
}

void fn_80105D28(XObjSpiderBoss* self)
{
	self->vfB0(0);
}

void fn_80106000(XObjSpiderBoss* self)
{
	fn_800928E8(self);
	self->mSe.mTable = lbl_8022B9D8;
}

void fn_8010660C(XObjSpiderBoss* self);
void fn_80106088(XObjSpiderBoss* self);

BOOL fn_80106038(XObjSpiderBoss* self, void* info)
{
	if (fn_80091C80(self, info)) {
		fn_8010660C(self);
		fn_80106088(self);
		return TRUE;
	}
	return FALSE;
}

void fn_801068D4(XObjSpiderBoss* self)
{
	fn_8010660C(self);
	self->SetState(0);
	self->mStatus.mFlags |= 0x4000000;
}

void fn_80106924(XObjSpiderBoss* self, u32 id)
{
	fn_800929C4(self, id);
	fn_8010660C(self);
	self->SetState(0);
	self->mStatus.mFlags |= 0x4000000;
}

} // extern "C"
