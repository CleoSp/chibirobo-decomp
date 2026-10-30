#include "game/scr.h"

// XObjActor, an abstract XObj the stage characters build on. the class name is from its
// vtable, nothing else has a name in the dol, so everything is extern "C" under its address

extern "C" {
u8 fn_800F5678(XObj* obj);
u8 fn_800F5684(XObj* obj);
}

// one of the actor's 8 slots, 0x38 bytes
struct UnkActorSlot {
	s32 _00;      // _00
	s32 _04;      // _04
	u8 _08[0x30]; // _08

	void Clear()
	{
		_04 = 0;
		_00 = 0;
	}
};

class XObjActor : public XObj {
public:
	f32 _980;               // _980
	f32 _984;               // _984
	f32 _988;               // _988
	f32 _98C;               // _98C
	s32 _990;               // _990
	s32 _994;               // _994
	CStatus mStatus;        // _998
	s32 _9A0;               // _9A0
	s32 _9A4;               // _9A4
	f32 _9A8;               // _9A8
	f32 _9AC;               // _9AC
	Vec _9B0;               // _9B0
	Vec _9BC;               // _9BC
	Vec _9C8;               // _9C8
	f32 _9D4;               // _9D4
	f32 _9D8;               // _9D8
	f32 _9DC;               // _9DC
	s32 _9E0;               // _9E0
	s32 _9E4;               // _9E4
	s32 _9E8[16];           // _9E8
	s32 _A28;               // _A28
	s32 _A2C;               // _A2C
	u8 _A30[0x8];           // _A30
	f32 _A38;               // _A38
	f32 _A3C;               // _A3C
	Vec _A40;               // _A40
	s16 _A4C;               // _A4C
	s32 _A50;               // _A50
	u8 _A54[0xA84 - 0xA54]; // _A54
	s32 _A84;               // _A84
	s32 _A88;               // _A88
	UnkActorSe mSe;         // _A8C
	s32 _A9C;               // _A9C
	f32 _AA0;               // _AA0
	f32 _AA4;               // _AA4
	f32 _AA8;               // _AA8
	s32 _AAC;               // _AAC
	f32 _AB0;               // _AB0
	UnkActorSlot mSlots[8]; // _AB4

	void ClearList()
	{
		for (int i = 0; i < 16; i++) {
			_9E8[i] = 0;
		}
	}

	virtual void vf148(u32 frames);     // _148
	virtual void vf14C(u32 frames) = 0; // _14C
	virtual void vf150()           = 0; // _150
	virtual void vf154(u32 frames);     // _154
	virtual void vf158(u32 frames) = 0; // _158
	virtual void vf15C();               // _15C
	virtual u32 vf160();                // _160
	virtual void vf164(f32 frame);      // _164
	virtual HSD_JObj* vf168();          // _168
	virtual CVec vf16C();               // _16C
	virtual f32 vf170();                // _170
	virtual f32 vf174();                // _174
	virtual f32 vf178();                // _178
	virtual f32 vf17C();                // _17C
};

extern "C" {

s32 fn_80106AC0(void)
{
	s32 count = 0;
	for (s32 i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL->Get(i);
		if ((obj->mFlags & 1) && fn_800F5678(obj)) {
			count++;
		}
	}
	return count;
}

s32 fn_80106B50(void)
{
	s32 count = 0;
	for (s32 i = 0; i < 16; i++) {
		XObj* obj = GAME_POOL->Get(i);
		if ((obj->mFlags & 1) && fn_800F5684(obj)) {
			count++;
		}
	}
	return count;
}

void fn_80106A8C(XObjActor* self)
{
}

HSD_JObj* fn_80106A90(XObjActor* self)
{
	return self->mJObj;
}

CVec fn_80106A98(XObjActor* self)
{
	return self->mMoveTranslate;
}

void fn_80106BE0(XObjActor* self, f32 frame)
{
	if ((self->mFlags & 1) && (self->mFlags & 0x10000)) {
		s32 vol;
		if (self->mStatus.mFlags & 0x800) {
			vol = 0xFF;
		} else {
			vol = fn_8001DF00(GAME_SOUND, &self->mMoveTranslate, 0);
		}
		self->mSe.Play(self->mAnimId, vol, frame);
	}
}

void fn_8010706C(XObjActor* self)
{
	self->_990 = 0;
	self->_994 = 0;
	self->_984 = 0.0f;
}

Vec fn_80107084(XObjActor* self)
{
	return self->mMoveTranslate;
}

void fn_801070A0(XObjActor* self)
{
}

u32 fn_8010753C(XObjActor* self)
{
	return (self->mStatus.mFlags >> 1) & 1;
}

void fn_80107548(XObjActor* self, const Vec* pos, f32 f)
{
	fn_8008A6C0(self, pos, f);
	self->mStatus.mFlags |= 2;
	Vec v      = { 0.0f, 0.0f, 0.0f };
	v          = *pos;
	self->_9B0 = v;
	self->_9D4 = f;
}

s32 fn_801075E8(XObjActor* self, s32 a, s32 b)
{
	self->mStatus.mFlags |= 1;
	return b;
}

void fn_801075FC(XObjActor* self)
{
}

void fn_80107600(XObjActor* self)
{
	fn_8009047C(self);
}

void fn_80107620(XObjActor* self)
{
	if (self->mStatus.mFlags & 0x80) {
		self->Push(self->vf168(), 0x80, self->_AA0);
	}
	self->vfB0(0);
}

f32 fn_80107690(XObjActor* self)
{
	return -0.34906584f;
}

f32 fn_80107698(XObjActor* self)
{
	return 0.34906584f;
}

f32 fn_801076A0(XObjActor* self)
{
	return -1.3962634f;
}

f32 fn_801076A8(XObjActor* self)
{
	return 1.3962634f;
}

void fn_801076FC(XObjActor* self, s32 id, f32 rate)
{
	self->mStatus.mFlags |= 0x80;
	self->_A9C = 1;
	self->_AA8 = rate;
	self->_AAC = id;
}

void fn_801076B0(XObjActor* self, s32 id, f32 rate)
{
	fn_801076FC(self, id, rate);
	self->mStatus.mFlags |= 0x400;
}

void fn_801076DC(XObjActor* self, f32 a, f32 b)
{
	self->mStatus.mFlags |= 0x80;
	self->_A9C = 3;
	self->_AA4 = a;
	self->_AA8 = b;
}

void fn_8010771C(XObjActor* self, f32 rate)
{
	self->mStatus.mFlags |= 0x80;
	self->_A9C = 0;
	self->_AA8 = rate;
	self->_AA4 = 0.0f;
	self->mStatus.mFlags &= ~0x400;
	self->_AB0 = 0.0f;
}

void fn_80107DE0(XObjActor* self, u32 frames)
{
	self->vf158(frames);
	self->vf14C(frames);
	if (self->mStatus.mFlags & 4) {
		self->vf154(frames);
	}
	fn_80090ED4(self, frames);
	self->vf148(frames);
	HSD_AObj* aobj = GetAObj(self->mJObj);
	self->vf164(aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f);
}

BOOL fn_80107F18(XObjActor* self, void* info)
{
	if (fn_80091C80(self, info)) {
		self->_980 = 0.0f;
		self->_984 = 0.0f;
		self->_988 = 0.0f;
		self->_9E0 = 0;
		self->_9E4 = 0;
		self->ClearList();
		self->_A28 = 0;
		self->_A2C = 0;
		self->_A4C = -1;
		self->_A50 = 0;
		self->_A84 = 0;
		self->_A88 = 0;
		self->vf150();
		self->vf15C();
		self->_A9C = 0;
		self->_AA0 = 0.0f;
		self->_AA4 = 0.0f;
		self->_AA8 = 0.0f;
		self->_AAC = 0;
		return TRUE;
	}
	return FALSE;
}

// NONMATCHING: the zero Vec templates sit at other .rodata offsets
void fn_80108014(XObjActor* self)
{
	self->_980           = 0.0f;
	self->_984           = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 3.1415927f;
	self->_990           = 0;
	self->_994           = 0;
	self->mStatus.mFlags = 0;
	self->_9A0           = 0;
	self->_9A4           = 0;
	self->_9A8           = 1.0f;
	self->_9AC           = 1.0f;
	Vec zero0            = { 0.0f, 0.0f, 0.0f };
	self->_9B0           = zero0;
	Vec zero1            = { 0.0f, 0.0f, 0.0f };
	self->_9BC           = zero1;
	Vec zero2            = { 0.0f, 0.0f, 0.0f };
	self->_9C8           = zero2;
	Vec zero3            = { 0.0f, 0.0f, 0.0f };
	self->_A40           = zero3;
	self->_9D4           = 0.0f;
	self->_9D8           = 0.0f;
	self->_9DC           = 0.0f;
	self->_9E0           = 0;
	self->_9E4           = 0;
	self->_A28           = 0;
	self->ClearList();
	self->_A38 = 0.0f;
	self->_A3C = 0.0f;
	for (s32 i = 0; i < 8; i++) {
		self->mSlots[i].Clear();
	}
	self->mSe.mTable = NULL;
	self->_A4C       = -1;
	self->_A50       = 0;
	self->_A84       = 0;
	self->_A88       = 0;
	self->_A9C       = 0;
	self->_AA0       = 0.0f;
	self->_AA4       = 0.0f;
	self->_AA8       = 0.0f;
	self->_AAC       = 0;
}

// NONMATCHING: the zero Vec templates sit at other .rodata offsets
void fn_801081F0(XObjActor* self)
{
	fn_800928E8(self);
	self->_980           = 0.0f;
	self->_984           = 0.0f;
	self->_988           = 0.0f;
	self->_98C           = 3.1415927f;
	self->_990           = 0;
	self->_994           = 0;
	self->mStatus.mFlags = 0;
	self->_9A0           = 0;
	self->_9A4           = 0;
	self->_9A8           = 1.0f;
	self->_9AC           = 1.0f;
	Vec zero0            = { 0.0f, 0.0f, 0.0f };
	self->_9B0           = zero0;
	Vec zero1            = { 0.0f, 0.0f, 0.0f };
	self->_9BC           = zero1;
	Vec zero2            = { 0.0f, 0.0f, 0.0f };
	self->_9C8           = zero2;
	Vec zero3            = { 0.0f, 0.0f, 0.0f };
	self->_A40           = zero3;
	self->_9D4           = 0.0f;
	self->_9D8           = 0.0f;
	self->_9DC           = 0.0f;
	self->_9E0           = 0;
	self->_9E4           = 0;
	self->_A28           = 0;
	self->ClearList();
	self->_A38 = 0.0f;
	self->_A3C = 0.0f;
	for (s32 i = 0; i < 8; i++) {
		self->mSlots[i].Clear();
	}
	self->_A4C = -1;
	self->_A50 = 0;
	self->_A84 = 0;
	self->_A88 = 0;
	self->_A9C = 0;
	self->_AA0 = 0.0f;
	self->_AA4 = 0.0f;
	self->_AA8 = 0.0f;
	self->_AAC = 0;
}

void fn_801083F0(XObjActor* self, u32 id)
{
	fn_800929C4(self, id);
	self->mSe.mTable = NULL;
}

} // extern "C"
