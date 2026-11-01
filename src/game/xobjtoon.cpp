#include "game/hsd_jobj.h"
#include "game/hsd_dobj.h"
extern "C" {
#include <sysdolphin/class_alloc.h>
}

// the pe presets sit 0x5C into this unit's rodata, which isn't split out yet
struct XObjPEPresets {
	u8 _00[0x5C];          // _00
	HSD_PEDesc mReset;     // _5C
	HSD_PEDesc mPreset[3]; // _68
};

extern "C" {
extern const XObjPEPresets lbl_802091E8;
}

// joint walk callback for XObj::vfD8, the args are five on/off flags
/**
 * @note Address: 0x80091888
 * @note Size: 0x3BC
 */
extern "C" void fn_80091888(HSD_JObj* joint, f32** arguments, s32 type)
{
	const XObjPEPresets* presets = &lbl_802091E8;
	u32 reset                    = (u32)arguments[0];
	u32 firstPreset              = (u32)arguments[1];
	u32 secondPreset             = (u32)arguments[2];
	u32 thirdPreset              = (u32)arguments[3];
	u32 toon                     = (u32)arguments[4];
	for (HSD_DObj* draw = HSD_JObjGetDObj(joint); draw != NULL; draw = HSD_DObjGetNext(draw)) {
		HSD_MObj* material = HSD_DObjGetMObj(draw);
		if (material != NULL) {
			if (reset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mReset;
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
				if (material->rendermode & RENDER_NO_ZUPDATE)
					material->pe->flags &= ~0x20;
				if (material->rendermode & RENDER_ZMODE_ALWAYS)
					material->pe->flags &= ~0x10;
			}
			if (firstPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[0];
				*material->pe       = settings;
			} else if (secondPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[1];
				*material->pe       = settings;
			} else if (thirdPreset) {
				if (material->pe == NULL)
					material->pe = (HSD_PEDesc*)hsdAllocMemPiece(sizeof(HSD_PEDesc));
				HSD_PEDesc settings = presets->mPreset[2];
				*material->pe       = settings;
				if (material->rendermode & RENDER_XLU)
					material->pe->type = 1;
			}
			if (toon) {
				if (!(fn_801B8E5C(material) & RENDER_TOON)) {
					HSD_MObjSetFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			} else {
				if (fn_801B8E5C(material) & RENDER_TOON) {
					HSD_MObjClearFlags(material, RENDER_TOON);
					HSD_MObjCompileTev(material);
				}
			}
		}
	}
}
