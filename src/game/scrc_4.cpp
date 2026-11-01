#include "game/scrc_shared.h"

extern "C" {

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B394(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_80024D84(CPtcl* ptcl, u32 frames);
void fn_8010FB1C(XObjActor3* self);

void XObjActor3::Kind19(u32 frames)
{
}
void fn_8010B528(XObjActor3* self);

// carries the robot while it rides the actor, at the actor's first joint
// NONMATCHING: the robot and the joint's vectors take each other's registers
void fn_8010B7B4(XObjActor3* self, u32 frames)
{
	CJObj joint;
	UnkPlayer* player = GAME_PLAYER;
	if ((self->mStatus.mFlags & 0x40) && (player->_0980 & 0x40000000)) {
		Mtx scaleMtx;
		Mtx rotMtx;
		Mtx transMtx;
		Mtx mtx;
		Mtx m;
		s32 path[2] = { 0, 0 };
		joint.Attach(GetJointFromRoot(self->mJObj, 2, path));
		PSMTXCopy(HSD_JObjGetMtxPtr(joint.mJObj), m);
		Vec scale;
		HSD_MtxGetScale(m, &scale);
		scale.x = fn_800621E0(player).x / scale.x;
		scale.y = fn_800621E0(player).y / scale.y;
		scale.z = fn_800621E0(player).z / scale.z;
		PSMTXScale(scaleMtx, scale.x, scale.y, scale.z);
		PSMTXConcat(m, scaleMtx, mtx);
		PSMTXRotRad(rotMtx, 'y', 0.0f);
		PSMTXConcat(mtx, rotMtx, mtx);
		PSMTXTrans(transMtx, 0.0f, 3.7f, 30.0f);
		PSMTXConcat(mtx, transMtx, mtx);
		HSD_JObj* jobj = player->mJObj;
		MtxPtr mp      = mtx;
		HSD_JObjCopyMtx(jobj, mp);
	}
}
void fn_8010B394(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010904C(XObjActor3* self);
}
