#include "game/hsd_jobj.h"
#include "game/cparticle.h"
#include "game/xobjactorfamily.h"
#include <stdlib.h>

static inline MtxPtr ActorMatrix(const CJObj& obj)
{
	return HSD_JObjGetMtxPtr(obj.mJObj);
}

// picks up the object _A4C names and holds it in his hand
// NONMATCHING: one register
void XObjActorPapa::StatePickUp(u32 frames)
{
	switch (_990) {
	case 0: {
		XObjActorTao* tao = GAME_TAO;
		if ((tao->mFlags & 1) && !tao->IsFree()) {
			return;
		}
		bool busy = false;
		for (s32 i = 0; i < 15; i++) {
			XObj* obj = GAME_POOL_975C->Get(i);
			if (obj->GetDescKind() == 0x159) {
				if (!(obj->mFlags & 1)) {
					break;
				}
				if (!fn_8011566C(obj)) {
					busy = true;
					break;
				}
			}
		}
		if (busy) {
			return;
		}
		if (_A50 != NULL) {
			_A50->Release();
		}
		if (_A4C >= 0) {
			_A50 = ReadObj(_A4C);
		} else {
			_A50 = NULL;
		}
		mStatus.mFlags |= 0x2000;
		_990++;
	}
	case 1:
		if (_A50 != NULL) {
			if (_A50->mFlags & 0x0040000000000000ULL) {
				return;
			}
			GAME_FLAGS |= 0x1000;
		}
		_990++;
	case 2:
		if (_A4C >= 0 && _A50 != NULL) {
			((UnkCarried*)_A50)->SetScale(1.0f, 1.0f, 1.0f, 0);
			CJObj jobj;
			s32 path[6] = { 0, 2, 2, 0, 0, 0 };
			jobj.Attach(FindJoint(mJObj, 6, path));
			Vec pos;
			pos = GetJObjPos(jobj.mJObj);
			Vec scale;
			scale.x = pos.x;
			scale.y = pos.y;
			scale.z = pos.z;
			((UnkCarried*)_A50)->SetMoveTranslate(pos.x, pos.y + 2.0f, pos.z, 0);
			PSMTXCopy(ActorMatrix(jobj), (MtxPtr)_A54);
			HSD_MtxGetScale((MtxPtr)_A54, &scale);
			scale.x = fn_800621E0(_A50).x / scale.x;
			scale.y = fn_800621E0(_A50).y / scale.y;
			scale.z = fn_800621E0(_A50).z / scale.z;
			Mtx scaleMtx;
			Mtx rotMtx;
			Mtx transMtx;
			Mtx m;
			PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
			PSMTXConcat((MtxPtr)_A54, scaleMtx, m);
			PSMTXRotRad(rotMtx, 'z', 1.5707964f);
			PSMTXConcat(m, rotMtx, m);
			PSMTXTrans(transMtx, -5.5f, 3.5f, 0.0f);
			PSMTXConcat(m, transMtx, m);
			HSD_JObj* joint = _A50->mJObj;
			MtxPtr mp       = m;
			HSD_JObjCopyMtx(joint, mp);

			_A50->mFlags &= ~0x0200000000000000ULL;
		}
		mStatus.mFlags &= ~0x2000;
		break;
	}
}

void XObjActorPapa::StateNone(u32 frames)
{
}
