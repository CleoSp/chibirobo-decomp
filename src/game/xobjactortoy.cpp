#include "game/particle_srt.h"
#include "game/xobjactortoy.h"

bool XObjActorToy::fn_8011566C() const
{
	if (!(mFlags & 1)) {
		return true;
	}
	return !mStatus.Check(0x2000);
}

// the texture animation of the jobj's dobj after skip others
static inline HSD_AObj* GetTexAObj(HSD_JObj* jobj, s32 skip)
{
	if (jobj == NULL) {
		return NULL;
	}
	HSD_DObj* dobj = HSD_JObjGetDObj(jobj);
	for (s32 i = 0; i < skip; i++) {
		if (dobj == NULL) {
			return NULL;
		}
		dobj = HSD_DObjGetNext(dobj);
	}
	HSD_MObj* mobj = HSD_DObjGetMObj(dobj);
	if (mobj == NULL) {
		return NULL;
	}
	HSD_TObj* tobj = HSD_MObjGetTObj(mobj);
	if (tobj == NULL) {
		return NULL;
	}
	return tobj->aobj;
}

void XObjActorToy::vf164(f32 frame)
{
	switch (GetDescKind()) {
	case 0x1FC:
		if (mAnimId == 0x3F6) {
			s32 path[3]    = { 0, 2, 0 };
			HSD_AObj* aobj = GetTexAObj(GetJointByPath(mJObj, path, 3), 4);
			frame          = aobj != NULL ? HSD_AObjGetCurrFrame(aobj) : 0.0f;
		}
		if (fn_8001E6C8(GAME_SOUND, 0x10001F)) {
			if (mAnimId != 0x10) {
				fn_8001E660(GAME_SOUND, 0x10001F);
			} else {
				fn_8001E468(GAME_SOUND, 0x10001F, fn_8001DF00(GAME_SOUND, &mMoveTranslate, 0), 0);
			}
		}
		break;
	case 0x159:
		if (fn_8001E6C8(GAME_SOUND, 0xE0008) && mAnimId != 7) {
			fn_8001E660(GAME_SOUND, 0xE0008);
		}
		break;
	}
	fn_80106BE0(this, frame);
}

// one of the toys, see GAME_POOL_975C
static inline XObjActorToy* GetToy(s32 i)
{
	return (XObjActorToy*)GAME_POOL_975C->Get(i);
}

static inline MtxPtr ToyMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

// the 0x157 toy carries the 0x156 ones along, with what they hold in their hand
void XObjActorToy::vf130()
{
	switch (GetDescKind()) {
	case 0xF:
		break;
	case 0x6F:
		break;
	case 0x110:
		break;
	case 0x117:
		break;
	case 0x156:
		break;
	case 0x157:
		mToyStatus.mFlags |= 2;
		for (s32 i = 0; i < 15; i++) {
			XObjActorToy* toy = GetToy(i);
			if (toy->GetDescKind() == 0x156 && (toy->mToyStatus.mFlags & 1)) {
				toy->SetTranslateX(mMoveTranslate.x, 0);
				toy->SetTranslateZ(mMoveTranslate.z, 0);
				if (toy->_A4C >= 0 && toy->_A50 != NULL) {
					CJObj joint;
					s32 path[4] = { 0, 2, 4, 0 };
					Mtx hand;
					Mtx trans;
					Mtx rot;
					Mtx mtx;
					joint.Attach(GetJointFromRoot(toy->mJObj, 4, path));
					toy->_A50->SetAnim(1, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
					PSMTXCopy(ToyMatrix(joint), hand);
					PSMTXRotRad(rot, 'z', 0.0f);
					PSMTXConcat(hand, rot, mtx);
					PSMTXTrans(trans, 0.0f, 2.0f, 2.0f);
					PSMTXConcat(mtx, trans, mtx);
					HSD_JObj* jobj = toy->_A50->mJObj;
					MtxPtr m       = mtx;
					HSD_JObjCopyMtx(jobj, m);
					if (mFlags & 0x10000) {
						toy->mFlags |= 0x10000ULL;
					} else {
						toy->mFlags &= ~0x10000ULL;
					}
				}
			}
		}
		break;
	case 0x159:
		break;
	case 0x1D3:
		break;
	case 0x1FB:
		break;
	case 0x1FC:
		break;
	case 0x25E:
		break;
	case 0x26C:
		break;
	case 0x26D:
		break;
	case 0x485:
		break;
	}
}

extern "C" const u8 lbl_8022C7E0[];

extern "C" {
void fn_80119774(XObjActorToy* self, u32 frames);
void fn_80119C24(XObjActorToy* self, u32 frames);
void fn_8011A050(XObjActorToy* self, u32 frames);
void fn_8011A390(XObjActorToy* self, u32 frames);
void fn_8011B7EC(XObjActorToy* self, u32 frames);
void fn_8011BE44(XObjActorToy* self, u32 frames);
void fn_8011C44C(XObjActorToy* self, u32 frames);
void fn_8011C834(XObjActorToy* self, u32 frames);
void fn_8011CF94(XObjActorToy* self, u32 frames);
}

static inline s16 ToyItemId(const UnkPlayerItem& item)
{
	return item.mId;
}

static inline s16 ToyItemNum(const UnkPlayerItem& item)
{
	return item.mNum;
}
extern "C" void fn_8011A050(XObjActorToy* self, u32 frames);
extern "C" void fn_80119774(XObjActorToy* self, u32 frames);

static inline f32 ToyTranslationX(const CJObj& obj)
{
	return HSD_JObjGetTranslationX(obj.mJObj);
}

static inline f32 ToyTranslationY(const CJObj& obj)
{
	return HSD_JObjGetTranslationY(obj.mJObj);
}

static inline f32 ToyTranslationZ(const CJObj& obj)
{
	return HSD_JObjGetTranslationZ(obj.mJObj);
}

static inline f32 ToyScaleX(const CJObj& obj)
{
	return HSD_JObjGetScaleX(obj.mJObj);
}

static inline f32 ToyScaleY(const CJObj& obj)
{
	return HSD_JObjGetScaleY(obj.mJObj);
}

static inline f32 ToyScaleZ(const CJObj& obj)
{
	return HSD_JObjGetScaleZ(obj.mJObj);
}

static inline f32 ToyRotationY(const CJObj& obj)
{
	return HSD_JObjGetRotationY(obj.mJObj);
}
extern "C" void fn_80119C24(XObjActorToy* self, u32 frames);
extern "C" void fn_8011A390(XObjActorToy* self, u32 frames);
