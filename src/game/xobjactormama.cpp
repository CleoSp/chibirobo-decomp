#include "game/particle_srt.h"
#include "game/xobjactorfamily.h"

HSD_JObj* XObjActorMama::vf168()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return FindJoint(mJObj, 4, path);
}

CVec XObjActorMama::vf16C()
{
	s32 path[4] = { 0, 4, 0, 0 };
	return fn_8007E7C8(this, 4, path);
}

f32 XObjActorMama::vf17C()
{
	return -0.17453292f;
}

f32 XObjActorMama::vf178()
{
	return 0.17453292f;
}

f32 XObjActorMama::vf174()
{
	return -0.6981317f;
}

f32 XObjActorMama::vf170()
{
	return 0.6981317f;
}
