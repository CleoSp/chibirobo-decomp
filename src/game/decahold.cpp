#include "game/xobjactor.h"
#include "game/decacamerapath.h"

extern "C" void PSMTXTrans(Mtx mtx, f32 x, f32 y, f32 z);

#pragma section RX ".text.decahold"

/**
 * @note Address: 0x801308E4
 * @note Size: 0x434
 */
// keeps what it carries in its hand
__declspec(section ".text.decahold") void XObjActorDeca::StateHold(u32 frames)
{
	if (_A4C >= 0 && _A50 != NULL) {
		_A50->SetScale(1.0f, 1.0f, 1.0f, 0);
		CJObj jobj;
		DecaJointPath2 path = sDecaHoldJointPath;
		jobj.Attach(FindJointByPath(mJObj, path.indices, 2));
		Mtx m;
		Mtx transMtx;
		Mtx hand;
		PSMTXCopy(jobj.GetMtx(), m);
		PSMTXTrans(transMtx, 0.0f, -3.0f, -4.0f);
		PSMTXConcat(m, transMtx, hand);
		HSD_JObj* joint = _A50->mJObj;
		MtxPtr mp       = hand;
		HSD_JObjCopyMtx(joint, mp);
		if (mStatus.mFlags & 0x10000) {
			_A50->mStatus.mFlags |= 0x10000ULL;
		} else {
			_A50->mStatus.mFlags &= ~0x10000ULL;
		}
		_A50->mStatus.mFlags &= ~0x0200000000000000ULL;
	}
}
