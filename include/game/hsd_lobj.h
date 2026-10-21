#ifndef _GAME_HSD_LOBJ_H
#define _GAME_HSD_LOBJ_H

// c++ lobj.h helpers. Unref only ever shows up inlined, the name is ours

extern "C" {
#include <sysdolphin/lobj.h>

// in this revision's lobj.c, missing from our lobj.h
void HSD_LObjAddAnim(HSD_LObj* lobj, HSD_LightAnim* anim);
void HSD_LObjAnim(HSD_LObj* lobj);
void HSD_LObjReqAnim(HSD_LObj* lobj, f32 frame);
void HSD_LObjSetColor(HSD_LObj* lobj, GXColor color);
void HSD_LObjGetColor(HSD_LObj* lobj, GXColor* color);
void HSD_LObjSetSpot(HSD_LObj* lobj, f32 cutoff, s32 spot_func);
void HSD_LObjSetDistAttn(HSD_LObj* lobj, f32 ref_dist, f32 ref_br, s32 dist_func);
}

// same reference drop as HSD_CObjRefDec
inline BOOL HSD_LObjRefDec(HSD_LObj* lobj)
{
	BOOL ret;

	if ((ret = (HSD_OBJ(lobj)->ref_count == HSD_OBJ_NOREF))) {
		return ret;
	}
	return HSD_OBJ(lobj)->ref_count-- == 0;
}

inline void HSD_LObjUnref(HSD_LObj* lobj)
{
	if (lobj == NULL) {
		return;
	}
	if (HSD_LObjRefDec(lobj) && lobj != NULL) {
		HSD_CLASS_METHOD(lobj)->release((HSD_Class*)lobj);
		HSD_CLASS_METHOD(lobj)->destroy((HSD_Class*)lobj);
	}
}

#endif
