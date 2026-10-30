#include "game/scr.h"
#include "game/ctpl.h"

// XObjActor3, the actor most of the stage's small moving things are. which one it is comes
// from the stage table, see GetDescKind and GetInfoKind. the class name is from its vtable,
// nothing else has a name in the dol, so everything is extern "C" under its address

#include "game/gmath.h"
#include <stdlib.h>

extern "C" {
BOOL HSD_AudioSFXCheck(s32 id);
void HSD_AudioSFXKeyOff(s32 id);
u16 fn_8006E44C(s16 item, s32 kind);
f32 fn_8006697C(f32 x);
u8 fn_800EBDB0(XObj* obj);
BOOL fn_800A0000(XObj* obj);
void fn_800661E4(void* mgr, XObj* obj, s32 on, s32 b);
void fn_80068BAC(void* mgr);
void fn_80068BC0(void* mgr);
void fn_80068BD4(void* mgr);
void fn_80068BE8(void* mgr);
void fn_80068BFC(void* mgr);
void fn_80068C10(void* mgr);
}

// one slot of the robot's item list
struct UnkPlayerItem {
	s16 mId;  // _00
	s16 mNum; // _02
};

// chibi robo, only the item list
class UnkPlayer : public XObj {
public:
	u8 _0980[0x95D4 - 0x980];  // _0980
	s32 _95D4;                 // _95D4
	u8 _95D8[0x9688 - 0x95D8]; // _95D8
	UnkPlayerItem mItems[256]; // _9688
};

// the robot is out of reach, past the front edge or in one of its climbing anims
static inline bool IsPlayerAway(UnkPlayer* player, Vec pos)
{
	if (pos.z < -4.28f) {
		return true;
	}
	switch (player->mAnimId) {
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6A:
	case 0x6B:
	case 0x6C:
	case 0x6D:
	case 0x6E:
	case 0x6F:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
		return true;
	}
	return false;
}

// the anims the robot reaches further in
static inline bool IsLongReachAnim(s32 anim)
{
	switch (anim) {
	case 0x4:
	case 0xD:
	case 0x35:
	case 0x79:
	case 0x89:
	case 0x10D:
	case 0x819:
	case 0x898:
		return true;
	}
	return false;
}

// the robot's anims it can't be talked to in
static inline bool IsBusyAnim(s32 anim)
{
	switch (anim) {
	case 0x5:
	case 0x6:
	case 0x7:
	case 0x8:
	case 0x9:
	case 0xA:
	case 0xB:
	case 0x27:
	case 0x28:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x43:
	case 0x97:
	case 0xA1:
	case 0xED:
	case 0xEE:
	case 0xF6:
	case 0x814:
	case 0x816:
	case 0x817:
	case 0x81F:
	case 0x820:
	case 0x824:
	case 0x879:
	case 0x87A:
	case 0x8AE:
	case 0x8AF:
	case 0x8B7:
	case 0x8C0:
	case 0x8C1:
	case 0x8C2:
	case 0x8C3:
	case 0x8C4:
	case 0x8C5:
		return true;
	}
	return false;
}
#define GAME_PLAYER    (*(UnkPlayer**)(lbl_8025C400 + 0x96BC))
#define GAME_ITEMCOUNT (*(s32*)(lbl_8025C400 + 0x5D08C))
#define GAME_HOLDER    (*(XObj**)(lbl_8025C400 + 0x9810)) // what the kind 0xB and 0xC actors sit on

// a particle set, name from its vtable
class CPtcl {
public:
	u8 _00[0x10]; // _00
	// _10 vtable

	virtual ~CPtcl(); // _08

	u8 mParticles[0x32][0x50]; // _14
};

extern "C" {
void fn_80024CA0(CPtcl* ptcl);
void fn_80024FAC(CPtcl* ptcl, void* particles, u32 size, u32 count);
}

// one of the actor's 8 slots, 0x68 bytes
struct UnkActor3Slot {
	s32 _00;      // _00
	s32 _04;      // _04
	u8 _08[0x60]; // _08

	void Clear()
	{
		_04 = 0;
		_00 = 0;
	}
};

class XObjActor3 : public XObj {
public:
	CTpl mTpl;               // _980
	f32 _B70;                // _B70
	f32 _B74;                // _B74
	f32 _B78;                // _B78
	f32 _B7C;                // _B7C
	s32 _B80;                // _B80
	u8 _B84[0x4];            // _B84
	s32 _B88;                // _B88
	CStatus mStatus;         // _B8C
	s32 _B94[11];            // _B94
	f32 _BC0;                // _BC0
	f32 _BC4;                // _BC4
	f32 _BC8;                // _BC8
	Vec _BCC;                // _BCC
	f32 _BD8;                // _BD8
	CVec _BDC;               // _BDC
	s32 _BEC;                // _BEC
	f32 _BF0;                // _BF0
	f32 _BF4;                // _BF4
	u8 _BF8;                 // _BF8
	s32 _BFC;                // _BFC
	f32 _C00;                // _C00
	s32 _C04;                // _C04
	u8 _C08;                 // _C08
	CPtcl mPtcl;             // _C0C
	CTexObj mTex[2];         // _1BC0
	s32 _1DC0[10];           // _1DC0
	s32 mSfx;                // _1DE8
	UnkActorSe mSe;          // _1DEC
	UnkActor3Slot mSlots[8]; // _1DFC

	virtual void vf148(); // _148
};

// the script's variables
extern "C" s32 lbl_80367A2C[];

// the sound table of kind 0x31B
extern "C" const UnkActorSeEntry lbl_8022C178[];

extern "C" {

s32 lbl_8065B2F0;
s32 lbl_8065B2F4;

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);

// NONMATCHING: register allocation
void fn_80108540(XObjActor3* self)
{
	if ((self->mFlags & 1) && (self->mFlags & 0x10000) && self->_258 == 3) {
		s32 id    = self->mAnimId;
		f32 frame = GetAnimFrame(self->mJObj);
		self->mSe.Play(id, fn_8001DF00(GAME_SOUND, &self->mMoveTranslate, 0), frame);
	}
}

void fn_80108694(XObjActor3* self, s32 id, BOOL force, void* jointAnim, void* matAnim, void* shapeAnim, f32 frame, f32 rate, f32 blend,
                 f32 blendRate)
{
	fn_800902A4(self, id, force, jointAnim, matAnim, shapeAnim, frame, rate, blend, blendRate);
	if (self->GetDescKind() == 0x31B) {
		if (self->mAnimId == 7) {
			self->mSfx = fn_8001E904(GAME_SOUND, 0xB0000, 0xFF);
		} else if (HSD_AudioSFXCheck(self->mSfx)) {
			HSD_AudioSFXKeyOff(self->mSfx);
		}
	}
}

void fn_80108728(XObjActor3* self, const Vec* pos, f32 f)
{
	if (self->GetInfoKind() == 1) {
		fn_8008A6C0(self, pos, f);
	} else if (!self->mStatus.Check(1)) {
		self->mStatus.mFlags |= 1;
	}
}

void fn_80109004(XObjActor3* self)
{
}

void fn_80109008(XObjActor3* self)
{
	if (self->GetDescKind() == 0x31B) {
		fn_80024CA0(&self->mPtcl);
	}
}

void fn_80109DD8(XObjActor3* self)
{
	switch (self->GetInfoKind()) {
	case 0xE:
		fn_801095E8(self);
		fn_8010904C(self);
		break;
	}
}

void fn_80109E44(XObjActor3* self)
{
	if (!(self->mStatus.mFlags & 0x10)) {
		self->vfB0(0);
	}
}

void fn_8010A500(XObjActor3* self)
{
	switch (self->GetDescKind()) {
	case 0x149:
	case 0x42B:
		self->mTpl.Close();
		break;
	}
	fn_800911C0(self);
}

void fn_8010AD88(XObjActor3* self)
{
	self->_B70           = 0.0f;
	self->_B74           = 0.0f;
	self->_B78           = 0.0f;
	self->_B7C           = 0.0f;
	self->_B80           = 0;
	self->_B88           = 0;
	self->mStatus.mFlags = 0;
	self->mStatus.mFlags |= 4;
	self->_B94[0]  = 0;
	self->_B94[1]  = 0;
	self->_B94[2]  = 0;
	self->_B94[3]  = 0;
	self->_B94[4]  = 0;
	self->_B94[5]  = 0;
	self->_B94[6]  = 0;
	self->_B94[7]  = 0;
	self->_B94[8]  = 0;
	self->_B94[9]  = 0;
	self->_B94[10] = 0;
	self->_BC0     = 0.0f;
	self->_BC4     = 0.0f;
	self->_BC8     = 0.0f;
	Vec zero       = { 0.0f, 0.0f, 0.0f };
	self->_BCC     = zero;
	for (int i = 0; i < 8; i++) {
		self->mSlots[i].Clear();
	}
	if (self->_258 == 0) {
		lbl_8065B2F0 = 0;
		lbl_8065B2F4 = 0;
	}
	self->SetColor(1, 0, 0, 0, 0.0f);
	self->_BDC.x = 0.0f;
	self->_BDC.y = 0.0f;
	self->_BDC.z = 0.0f;
	self->_BEC   = 0;
	self->_BF0   = 0.0f;
	self->_BF4   = 0.0f;
	self->_BF8   = 0;
	self->_BFC   = 0;
	self->_C00   = 0.0f;
	self->_C04   = 0;
	self->_C08   = 0;
	if (self->GetDescKind() == 0x31B) {
		fn_80024FAC(&self->mPtcl, self->mPtcl.mParticles, 0x50, 0x32);
		self->mSe.mTable = lbl_8022C178;
	}
	self->_1DC0[0] = 0;
	self->_1DC0[1] = 0;
	self->_1DC0[2] = 0;
	self->_1DC0[3] = 0;
	self->_1DC0[4] = 0;
	self->_1DC0[5] = 0;
	self->_1DC0[6] = 0;
	self->_1DC0[7] = 0;
	self->_1DC0[8] = 0;
	self->_1DC0[9] = 0;
	self->mSfx     = -1;
}

// the script's commands to the actor, _B94 is the command and _B98 its argument
void fn_8010B394(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 0:
		break;
	case 1:
		if (self->_B94[1] != 0) {
			self->_BEC = 3;
		} else {
			self->_BEC = 4;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 2:
		if (self->_B94[1] != 0) {
			self->_BF8 = 1;
		} else {
			self->_BF8 = 0;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 3:
		self->_BF4    = 60.0f * (self->_B94[1] / 100.0f);
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 4:
		lbl_80367A2C[self->_B94[1]] = fn_8006697C(self->_BF4);
		self->_B94[0]               = 0;
		self->_B94[1]               = 0;
		break;
	case 5:
		if (self->_B94[1] != 0) {
			self->_BFC = 3;
		} else {
			self->_BFC = 4;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 6:
		self->_C04    = self->_B94[1];
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 7:
		lbl_80367A2C[self->_B94[1]] = self->_C04;
		self->_B94[0]               = 0;
		self->_B94[1]               = 0;
		break;
	}
}

void fn_8010B528(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 1:
		if (self->_B94[1] != 0) {
			self->mStatus.mFlags &= ~4;
			self->_BC0 = self->_B94[2] / 100.0f;
			self->_BC4 = self->_B94[3] / 100.0f;
		} else {
			self->mStatus.mFlags |= 4;
			self->_BC0 = 0.0f;
			self->_BC4 = 0.0f;
			self->_B80 = 0;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 2:
		if ((self->mStatus.mFlags & 0x80) && !(self->mStatus.mFlags & 4)) {
			lbl_80367A2C[self->_B94[1]] = 1;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 3:
		self->mStatus.mFlags &= ~0x80;
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	case 4:
		UnkPlayer* player = GAME_PLAYER;
		if (!IsBusyAnim(player->mAnimId) && player->_95D4 < 0) {
			lbl_80367A2C[self->_B94[1]] = 1;
		}
		self->_B94[0] = 0;
		self->_B94[1] = 0;
		break;
	}
}

void fn_8010CE80(XObjActor3* self, u32 frames)
{
	switch (self->_B80) {
	case 0:
		if (self->mStatus.mFlags & 4) {
			break;
		}
		self->_B80++;
		self->SetAnim(1, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		if (++lbl_8065B2F4 >= 10) {
			lbl_8065B2F4 = 0;
		}
		if (!fn_8001E6C8(GAME_SOUND, 0x127)) {
			fn_8001E904(GAME_SOUND, 0x127, 0xFF);
		}
		break;
	case 1:
		f32 end = GetAnimEndFrame(self->mJObj);
		if (end - frames <= GetAnimFrame(self->mJObj)) {
			self->_B80++;
			self->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
		}
		break;
	case 2:
		self->mStatus.mFlags |= 4;
		self->_B80 = 0;
		break;
	}
}

void fn_8010D114(XObjActor3* self, u32 frames)
{
	switch (self->_BEC) {
	case 0:
		self->_BEC = 4;
		self->_BF0 = 0.0f;
		break;
	case 1:
		self->_BF0 = 1.0f;
		break;
	case 2:
		self->_BF0 = 0.0f;
		break;
	case 3:
		self->_BF0 += 0.08f * frames;
		if (self->_BF0 >= 1.0f) {
			self->_BF0 = 1.0f;
		}
		break;
	case 4:
		self->_BF0 -= 0.08f * frames;
		if (self->_BF0 < 0.0f) {
			self->_BF0 = 0.0f;
		}
		break;
	}
	if (self->_BF8) {
		self->_BF4 -= frames;
	}
	switch (self->_BFC) {
	case 0:
		self->_BFC = 4;
		self->_C00 = 0.0f;
		break;
	case 1:
		self->_C00 = 1.0f;
		break;
	case 2:
		self->_C00 = 0.0f;
		break;
	case 3:
		self->_C00 += 0.08f * frames;
		if (self->_C00 >= 1.0f) {
			self->_C00 = 1.0f;
		}
		break;
	case 4:
		self->_C00 -= 0.08f * frames;
		if (self->_C00 < 0.0f) {
			self->_C00 = 0.0f;
		}
		break;
	}
	s32 count = 0;
	for (s32 i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL2->Get(i);
		if ((obj->mFlags & 0x10000) && !fn_800EBDB0(obj)) {
			count++;
		}
	}
	self->_C04 = count;
}

// NONMATCHING: register allocation
void fn_8010D390(s32 mode)
{
	if (mode == 0) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)GAME_ACTORS->Get(i);
			if (obj->GetDescKind() == 0x3E0 && (obj->mFlags & 1)) {
				obj->mStatus.mFlags &= ~4;
				obj->_B80    = 0;
				lbl_8065B2F0 = 0;
			}
		}
	}
	if (mode == 2) {
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)GAME_ACTORS->Get(i);
			if (obj->GetDescKind() == 0x3E0 && (obj->mFlags & 1) && obj->mStatus.Check(4)) {
				obj->mStatus.mFlags &= ~4;
				obj->_B80    = 20;
				lbl_8065B2F0 = 0;
			}
		}
	} else {
		u8* data;
		s32 size;
		size = GAME_ACTORS->mSize;
		data = GAME_ACTORS->mData;
		for (s32 i = 0; i < 48; i++) {
			XObjActor3* obj = (XObjActor3*)(data + i * size);
			if (obj->GetDescKind() == 0x3E0) {
				if (!(obj->mFlags & 1)) {
					return;
				}
				if (obj->mStatus.Check(4)) {
					obj->mStatus.mFlags &= ~4;
					obj->_B80 = 10;
					return;
				}
			}
		}
	}
}

void fn_8010AD00(XObjActor3* self)
{
	fn_8010AD88(self);
}

void fn_8010AD20(XObjActor3* self)
{
	fn_800928E8(self);
	fn_8010AD88(self);
}

void fn_8010AD54(XObjActor3* self, u32 id)
{
	fn_800929C4(self, id);
	fn_8010AD88(self);
}

void fn_8010BC08(XObjActor3* self)
{
}

void fn_8010BF7C(XObjActor3* self)
{
}

// puts the first kind 0x10 actor back to its rest pose, small
void fn_8010BF80(void)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {

			obj->_B80 = 0;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags |= 4;
			obj->mStatus.mFlags |= 0x10;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
			obj->SetScale(0.1f, 0.1f, 0.1f, 0);
			obj->_B80 = 0;
			return;
		}
	}
}

// plays anim n on the first kind 0x10 actor and sizes it for it
void fn_8010C0F0(s32 n)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {
			obj->SetAnim(n, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
			switch (n) {
			case 0:
				obj->SetScale(0.1f, 0.1f, 0.1f, 0);
				obj->_B80 = 0;
				break;
			case 1:
				obj->SetScale(1.4f, 1.4f, 1.4f, 0);
				obj->_B80 = 1;
				break;
			case 2:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 2;
				break;
			case 3:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 1;
				break;
			case 4:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 4;
				break;
			}
			return;
		}
	}
}

// puts every waiting 0x43A actor at the point
// NONMATCHING: register allocation
void fn_8010CDDC(f32 x, f32 y, f32 z)
{
	XObjActor3* obj;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)GAME_ACTORS->Get(i);
		switch (obj->GetDescKind()) {
		case 0x43A:
			if ((obj->mFlags & 1) && obj->mStatus.Check(4)) {
				obj->mStatus.mFlags &= ~4;
				obj->mMoveTranslate.x = x;
				obj->mMoveTranslate.y = y;
				obj->mMoveTranslate.z = z;
			}
			break;
		}
	}
}

// NONMATCHING: register allocation
void fn_8010F208(void)
{
	XObjActor3* obj;
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xC && (obj->mFlags & 1)) {
			if (obj->_B80 == 0) {
				obj->mStatus.mFlags &= ~4;
			}
			return;
		}
	}
}

// jumps the first kind 0xC actor onto the holder's middle
// NONMATCHING: register allocation
void fn_8010EF70(void)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xC && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
			XObj* holder = GAME_HOLDER;
			if ((holder->mFlags & 1) && (holder->mFlags & 0x10000) && obj->_B80 == 1) {
				obj->_B80      = 2;
				obj->_B78      = 6.0f;
				HSD_JObj* jobj = obj->mJObj;
				JObjClearFlags(jobj, 0x3800000);
				HSD_JObjSetMtxDirty(jobj);
				f32 ofs               = 0.0f; // the holder's middle, where fn_8010FB1C goes round it
				obj->mMoveTranslate.x = ofs + HSD_JObjGetTranslationX(holder->mJObj);
				obj->mMoveTranslate.y = 18.0f;
				obj->mMoveTranslate.z = ofs + HSD_JObjGetTranslationZ(holder->mJObj);
				obj->SetMoveTranslate(&obj->mMoveTranslate, 0);
				obj->_B7C = (f32)atan2(holder->mMoveTranslate.x - obj->mMoveTranslate.x, holder->mMoveTranslate.z - obj->mMoveTranslate.z)
				          + 3.1415927f;
				obj->SetAnim(2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				fn_8001E904(GAME_SOUND, 0x160021, 0xFF);
			}
			return;
		}
	}
}

// jumps the actor onto its holder, at its angle and distance around it
void fn_8010FB1C(XObjActor3* self)
{
	XObj* holder = GAME_HOLDER;
	if ((holder->mFlags & 1) && (holder->mFlags & 0x10000) && self->_B80 == 1) {
		f32 s          = sin(self->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 x          = self->_BD8 * s;
		f32 y          = self->_BCC.y;
		f32 c          = cos(self->_BC8 + HSD_JObjGetRotationY(holder->mJObj));
		f32 z          = self->_BD8 * c;
		self->_B80     = 2;
		self->_B78     = 6.0f;
		HSD_JObj* jobj = self->mJObj;
		JObjClearFlags(jobj, 0x3800000);
		HSD_JObjSetMtxDirty(jobj);
		self->mMoveTranslate.x = x + HSD_JObjGetTranslationX(holder->mJObj);
		self->mMoveTranslate.y = y + 8.0f + 10.0f;
		self->mMoveTranslate.z = z + HSD_JObjGetTranslationZ(holder->mJObj);
		self->SetMoveTranslate(&self->mMoveTranslate, 0);
		self->_B7C
		    = (f32)atan2(holder->mMoveTranslate.x - self->mMoveTranslate.x, holder->mMoveTranslate.z - self->mMoveTranslate.z) + 3.1415927f;
		fn_8001E904(GAME_SOUND, 0x160021, 0xFF);
	}
}

// starts the n-th kind 0xB actor
void fn_8010FDA0(s32 n)
{
	s32 count = 0;
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xB) {
			if (count == n && (obj->mFlags & 1) && (obj->mFlags & 0x10000)) {
				fn_8010FB1C(obj);
				return;
			}
			count++;
		}
	}
}

// NONMATCHING: register allocation
void fn_8010FE68(s32 n)
{
	s32 count = 0;
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0xB) {
			if (count == n && (obj->mFlags & 1)) {
				if (obj->_B80 == 0) {
					obj->mStatus.mFlags &= ~4;
				}
				return;
			}
			count++;
		}
	}
}

// whether the robot stands where the actor can reach it
BOOL fn_8011132C(XObjActor3* self)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	if (!(self->mStatus.mFlags & 4)) {
		if (IsPlayerAway(player, pos)) {
			return FALSE;
		}
		if (pos.y >= self->mMoveTranslate.y - 16.0f && pos.y <= self->mMoveTranslate.y + 16.0f) {
			if (self->_B7C == 1.5707964f) {
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					return TRUE;
				}
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x > pos.x && pos.x > self->mMoveTranslate.x - 12.0f) {
							return TRUE;
						}
					}
				}
			} else {
				if (self->mMoveTranslate.x > pos.x && pos.x > self->_BC0) {
					return TRUE;
				}
				if (self->mMoveTranslate.x < pos.x && pos.x < self->_BC4) {
					if (IsLongReachAnim(player->mAnimId)) {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 32.0f) {
							return TRUE;
						}
					} else {
						if (self->mMoveTranslate.x < pos.x && pos.x < self->mMoveTranslate.x + 12.0f) {
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

// falls from above the robot, knocks it down on a hit, blinks out on the floor
void fn_801115FC(XObjActor3* self, u32 frames)
{
	UnkPlayer* player = GAME_PLAYER;
	Vec pos           = player->mMoveTranslate;
	switch (self->_B80) {
	case 0:
		if (self->mStatus.mFlags & 4) {
			break;
		}
		self->mMoveTranslate.x = pos.x - (rand() % 3600) / 100.0f + 18.0f;
		self->mMoveTranslate.y = 64.0f;
		self->mMoveTranslate.z = pos.z - (rand() % 3600) / 100.0f + 18.0f;
		self->mFlags |= 0x10000;
		self->mFlags |= 4;
		self->_B70 = 4.0f;
		self->_B88 = rand() % 30 + 30;
		self->_B80++;
		break;
	case 1:
		if (--self->_B88 < 0) {
			self->_B80++;
		}
		break;
	case 2:
		f32 dx   = pos.x - self->mMoveTranslate.x;
		f32 dy   = pos.y - self->mMoveTranslate.y;
		f32 dz   = pos.z - self->mMoveTranslate.z;
		f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
		if (dist < 8.0f && !fn_800A0000(player) && !fn_800665E0(GAME_EVTMGR)) {
			player->vf138(&pos, 10.0f);
			self->_B88 = 30;
			self->_B80 = 3;
			break;
		}
		if (self->_B70 >= -2.3f) {
			self->_B70 -= 0.36f * frames;
		} else {
			self->_B70 = -2.3f;
		}
		self->mMoveTranslate.y += self->_B70;
		if (self->mMoveTranslate.y < 0.0f) {
			self->mMoveTranslate.y = 0.0f;
			self->_B88             = 30;
			self->_B80             = 3;
		}
		break;
	case 3:
		if ((self->_B88 & 3) < 2) {
			self->mFlags |= 0x10000;
		} else {
			self->mFlags &= ~0x10000ULL;
		}
		if (--self->_B88 < 0) {
			self->mFlags &= ~0x10000ULL;
			self->mFlags &= ~4ULL;
			self->mStatus.mFlags |= 4;
			self->_B80 = 0;
		}
		break;
	}
}

void fn_80114540(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 0:
		break;
	case 1:
		if (self->_B94[1] != 0) {
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BAC(GAME_EVTMGR);
		}
		self->_B94[0] = 0;
		break;
	case 2:
		if (self->_B94[1] != 0) {
			fn_80068C10(GAME_EVTMGR);
			fn_80068BE8(GAME_EVTMGR);
			fn_80068BC0(GAME_EVTMGR);
		} else {
			fn_80068BFC(GAME_EVTMGR);
			fn_80068BD4(GAME_EVTMGR);
			fn_80068BAC(GAME_EVTMGR);
		}
		self->_B94[0] = 0;
		break;
	}
}

// NONMATCHING: register allocation
void fn_8011463C(XObjActor3* self)
{
	s16 id;
	s16 num;
	s32 i;
	s32 count = 0;
	switch (self->_B94[0]) {
	case 0:
		break;
	case 1:
		for (i = 0; i < 0x100; i++) {
			UnkPlayerItem* items = GAME_PLAYER->mItems;
			id                   = items[i].mId;
			num                  = items[i].mNum;
			if (fn_8006E44C(id, 4) && id >= 0 && num > 0) {
				count++;
			}
		}
		GAME_ITEMCOUNT = count;
		self->_B94[0]  = 0;
		break;
	}
}

void fn_801146F4(XObjActor3* self)
{
}

void fn_801146F8(XObjActor3* self)
{
	self->_B88++;
	f32 s     = sin(0.017453292f * ((self->_B88 * 6) % 360));
	f32 swing = 0.25f;
	if (self->GetDescKind() == 0x437) {
		self->mMoveRotate.x = s * swing;
	} else {
		self->mMoveRotate.z = s * swing;
	}
}

void fn_801151E0(XObjActor3* self)
{
	switch (self->_B94[0]) {
	case 0:
		break;
	case 1:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(self->_258), 1, 1);
		self->_B94[0] = 0;
		break;
	case 2:
		fn_800661E4(GAME_EVTMGR, GAME_ACTORS->Get(self->_258), 0, 1);
		self->_B94[0] = 0;
		break;
	}
}

void fn_8011529C(XObjActor3* self)
{
}

} // extern "C"
