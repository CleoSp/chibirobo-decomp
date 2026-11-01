#include "game/particle_srt.h"
#include "game/xobjspider_shared.h"

extern "C" void fn_800EA99C(void* mgr)
{
	lbl_8065B2C4 = 0;
	for (int i = 0; i < 11; i++) {
		lbl_8037E640[i].mObj   = NULL;
		lbl_8037E640[i].mState = 0;
		lbl_8037E640[i].mTimer = 0;
	}
	for (int i = 0; i < 96; i++) {
		XObj* obj = GAME_ITEMS->Get(i);
		if (obj->GetDescKind() == 0x104) {
			((u8*)obj)[0x9E8] = 0;
		}
	}
}

// NONMATCHING: the drop pointer is set up through r0, the height sum is commuted
// moves the drops: shown, bobbing, then falling to where they land
extern "C" void fn_800EA780(void* mgr, u32 frames)
{
	Vec pos;
	f32 fall         = 0.8f * (f32)frames;
	UnkSpiderDrop* d = lbl_8037E640;
	for (int i = 0; i < 11; i++, d++) {
		XObj* obj = d->mObj;
		if (obj != NULL) {
			s32 state = d->mState;
			s32 timer = d->mTimer;
			f32 toY   = d->mTo.y;
			pos       = d->mPos;
			switch (state) {
			case 3:
				break;
			case 0:
				obj->mFlags |= 0x10000;
				state++;
				break;
			case 1:
				timer++;
				f32 bob = 10.0f * (f32)sin(DEG2RAD * (f32)((timer * 16) % 360)) + 0.2f;
				if (bob < 0.2f) {
					obj->SetTranslateY(pos.y + 0.2f, 0);
					state++;
				} else {
					obj->SetTranslateY(pos.y + bob, 0);
				}
				break;
			case 2:
				pos.y -= fall;
				if (pos.y <= toY) {
					obj->mFlags &= ~0x100000ULL;
					pos.y = toY;
					state++;
				}
				obj->SetTranslateY(pos.y + 0.2f, 0);
				break;
			}
			d->mState = state;
			d->mTimer = timer;
			d->mPos   = pos;
		}
	}
}

// NONMATCHING: the object loop multiplies instead of stepping the offset
// puts one of the free drops at pos and plays its anim
extern "C" void fn_800EA554(Vec* pos, Vec* to, s32 anim)
{
	if (lbl_8065B2C4 >= 11) {
		return;
	}
	UnkObjPool* pool = GAME_ITEMS;
	for (u32 i = 0; i < 96; i++) {
		XObj* obj = pool->Get(i);
		if ((obj->mFlags & 1) && obj->GetDescKind() == 0x104 && (obj->mFlags & 1) && ((u8*)obj)[0x9E8] == 0) {
			((u8*)obj)[0x9E8]                 = 1;
			lbl_8037E640[lbl_8065B2C4].mState = 0;
			lbl_8037E640[lbl_8065B2C4].mTimer += 2;
			lbl_8037E640[lbl_8065B2C4].mPos.x = pos->x;
			lbl_8037E640[lbl_8065B2C4].mPos.y = pos->y + 0.2f;
			lbl_8037E640[lbl_8065B2C4].mPos.z = pos->z;
			lbl_8037E640[lbl_8065B2C4].mTo.x  = to->x;
			lbl_8037E640[lbl_8065B2C4].mTo.y  = to->y;
			lbl_8037E640[lbl_8065B2C4].mTo.z  = to->z;
			lbl_8037E640[lbl_8065B2C4].mObj   = obj;
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateX(pos->x, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateY(pos->y + 0.2f, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->SetTranslateZ(pos->z, 0);
			lbl_8037E640[lbl_8065B2C4].mObj->mFlags |= 0x100000;
			lbl_8037E640[lbl_8065B2C4].mObj->SetAnim(anim, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			lbl_8065B2C4++;
			return;
		}
	}
}

// NONMATCHING: the jobj and the anim id swap registers
void XObjSpider::vf148()
{
	if ((mFlags & 1) && (mFlags & 0x10000)) {
		s32 anim  = mAnimId;
		f32 frame = GetAnimFrame(mJObj);
		mSe.Play(anim, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), frame);
	}
}
