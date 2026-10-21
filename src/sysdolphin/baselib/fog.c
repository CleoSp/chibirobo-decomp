#include <sysdolphin/fog.h>

#include <dolphin/gx/gx.h>
#include <sysdolphin/cobj.h>
#include <sysdolphin/gobjproc.h>

typedef struct {
	u16 r[10];
} GXFogAdjTable;

/**
 * @note Address: 0x801D37E0
 * @note Size: 0x2BC
 */
void HSD_FogSet(HSD_Fog* fog)
{
	HSD_FogAdj* fog_adj;
	HSD_CObj* cobj;
	u32 flags;
	s32 range;
	s32 width;
	Mtx44Ptr mtx;
	GXFogAdjTable tbl;
	f32 v[6];

	if (fog == NULL) {
		GXColor color = { 0 };
		GXSetFog(0, 0.0F, 0.0F, 0.0F, 0.0F, color);
		return;
	}

	cobj = HSD_CObjGetCurrent();
	if (cobj == NULL) {
		HSD_Panic(__FILE__, 0x58, "You must specify CObj first.\n");
	}

	GXSetFog(fog->type, fog->start, fog->end, HSD_CObjGetNear(cobj), HSD_CObjGetFar(cobj), fog->color);

	fog_adj = (fog != NULL) ? fog->fog_adj : NULL;
	flags   = HSD_FogAdjGetFlags(fog_adj);
	if (fog_adj != NULL && (flags & 7)) {
		GXGetViewportv(v);

		if (flags & 1) {
			s32 center = (fog_adj != NULL) ? fog_adj->center : -1;
			range      = v[0] + v[2] * (center + 320) / 640.0F;
		} else {
			range = v[0] + v[2] / 2;
		}

		if (flags & 2) {
			width = (fog_adj != NULL) ? fog_adj->width : -1;
		} else {
			width = v[2];
		}

		if (flags & 4) {
			mtx = (fog_adj != NULL) ? fog_adj->mtx : NULL;
		} else {
			Mtx44 local_mtx;
			struct {
				f32 x0;
				f32 v[6];
			} proj;

			mtx = local_mtx;
			memset(mtx, 0, sizeof(Mtx44));
			GXGetProjectionv((f32*)&proj);
			switch ((s32)proj.x0) {
			case 0:
				mtx[0][0] = proj.v[0];
				mtx[0][2] = proj.v[1];
				mtx[1][1] = proj.v[2];
				mtx[1][2] = proj.v[3];
				mtx[2][2] = proj.v[4];
				mtx[2][3] = proj.v[5];
				mtx[3][2] = -1.0F;
				break;
			default:
				mtx[0][0] = proj.v[0];
				mtx[0][3] = proj.v[1];
				mtx[1][1] = proj.v[2];
				mtx[1][3] = proj.v[3];
				mtx[2][2] = proj.v[4];
				mtx[2][3] = proj.v[5];
				mtx[3][3] = 1.0F;
				break;
			}
		}

		GXInitFogAdjTable(&tbl, width, mtx);
		GXSetFogRangeAdj(1, range, &tbl);
	} else {
		GXSetFogRangeAdj(0, 0, NULL);
	}
}

/**
 * @note Address: 0x801D3A9C
 * @note Size: 0x18
 */
u32 HSD_FogAdjGetFlags(HSD_FogAdj* fog_adj)
{
	return (fog_adj != NULL) ? fog_adj->flags : 0;
}
