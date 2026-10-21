#include <sysdolphin/shadow.h>

#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
#include <sysdolphin/cobj.h>
#include <sysdolphin/constants.h>
#include <sysdolphin/identity_mtx.h>
#include <sysdolphin/list.h>
#include <sysdolphin/mobj.h>
#include <sysdolphin/tobj.h>
#include <sysdolphin/vec_orthogonal.h>

/**
 * @note Address: N/A
 */
extern inline f64 fabs(f64 x)
{
	return __fabs(x);
}

void* HSD_Alloc(s32);
s32 GXGetTexBufferSize(u16, u16, s32, s32, s32);

static void makeMatrix(HSD_Shadow* shadow);

/* msl's float.h data, flt_min and flt_epsilon */
extern f32 __float_min[];
extern f32 __float_epsilon[];

#define ASSERT_NAMED(line, cond) ((cond) ? ((void)0) : __assert("shadow.c", line, #cond))

void* memset(void* dst, s32 val, u32 n);

HSD_ObjAllocData shadow_alloc_data;

static inline HSD_ObjAllocData* HSD_ShadowGetAllocData(void)
{
	return &shadow_alloc_data;
}

static inline bool ref_DEC(void* o)
{
	bool ret;
	if ((ret = (HSD_OBJ(o)->ref_count == HSD_OBJ_NOREF))) {
		return ret;
	}
	return HSD_OBJ(o)->ref_count-- == 0;
}

static inline HSD_TObj* makeShadowTObj(void)
{
	HSD_TObj* tobj;

	tobj            = HSD_TObjAlloc();
	tobj->src       = 0;
	tobj->wrap_s    = 0;
	tobj->wrap_t    = 0;
	tobj->flags     = 0x540103;
	tobj->imagedesc = HSD_ImageDescAlloc();
	return tobj;
}

/**
 * @note Address: 0x801ABD58
 * @note Size: 0x30
 */
void HSD_ShadowInitAllocData(void)
{
	HSD_ObjAllocInit(&shadow_alloc_data, sizeof(HSD_Shadow), 4);
}

/**
 * @note Address: 0x801ABC4C
 * @note Size: 0x10C
 */
HSD_Shadow* HSD_ShadowAlloc(void)
{
	HSD_Shadow* shadow;

	shadow = HSD_ObjAlloc(HSD_ShadowGetAllocData());
	memset(shadow, 0, sizeof(HSD_Shadow));
	shadow->camera  = HSD_CObjAlloc();
	shadow->texture = makeShadowTObj();

	shadow->scaleS    = 0.5F;
	shadow->scaleT    = -0.5F;
	shadow->transS    = 0.5F;
	shadow->transT    = 0.5F;
	shadow->intensity = 0;

	// yes format really gets stored twice
	shadow->texture->imagedesc->format = 1;
	shadow->texture->imagedesc->format = 1;
	shadow->texture->imagedesc->width  = 256;
	shadow->texture->imagedesc->height = 256;

	HSD_CObjSetViewportfx4(shadow->camera, 0.0F, 256.0F, 0.0F, 256.0F);
	HSD_CObjSetScissorx4(shadow->camera, 0, 256, 0, 256);

	return shadow;
}

/**
 * @note Address: 0x801ABB24
 * @note Size: 0x128
 */
void HSD_ShadowRemove(HSD_Shadow* shadow)
{
	HSD_CObj* cobj;
	HSD_TObj* tobj;

	if (shadow == NULL) {
		return;
	}
	if ((cobj = shadow->camera) != NULL) {
		if (ref_DEC(cobj) && cobj != NULL) {
			HSD_CLASS_METHOD(cobj)->release((HSD_Class*)cobj);
			HSD_CLASS_METHOD(cobj)->destroy((HSD_Class*)cobj);
		}
	}
	HSD_ShadowDeleteObject(shadow, NULL);
	if (shadow->active) {
		HSD_MObjDeleteShadowTexture(shadow->texture);
	}
	if (shadow->texture->imagedesc->img_ptr != NULL) {
		HSD_Free(shadow->texture->imagedesc->img_ptr);
	}
	tobj = shadow->texture;
	HSD_ImageDescRemove(tobj->imagedesc);
	HSD_TObjRemove(tobj);
	HSD_ObjFree(HSD_ShadowGetAllocData(), shadow);
}

/**
 * @note Address: 0x801ABAB4
 * @note Size: 0x70
 */
void HSD_ShadowInit(HSD_Shadow* shadow)
{
	HSD_ImageDesc* imagedesc;

	ASSERT_NAMED(0x104, shadow);
	imagedesc = shadow->texture->imagedesc;
	GXSetTexCopySrc(0, 0, imagedesc->width, imagedesc->height);
	GXSetTexCopyDst(imagedesc->width, imagedesc->height, 0x28, 0); /* GX_CTF_G8 */
}

/**
 * @note Address: 0x801AB95C
 * @note Size: 0x158
 */
void HSD_ShadowSetSize(HSD_Shadow* shadow, u16 width, u16 height)
{
	u32 size;
	HSD_ImageDesc* imgdesc;

	ASSERT_NAMED(0x12F, shadow);
	ASSERT_NAMED(0x130, width > 0);
	ASSERT_NAMED(0x131, height > 0);

	imgdesc = shadow->texture->imagedesc;
	if (imgdesc->img_ptr == NULL || imgdesc->width != width || imgdesc->height != height) {
		if (imgdesc->img_ptr != NULL) {
			HSD_Free(imgdesc->img_ptr);
		}
		size = GXGetTexBufferSize(width, height, 1, 0, 0); /* GX_TF_I8 */
		ASSERT_NAMED(0x140, size > 0);

		imgdesc->img_ptr = HSD_Alloc(size);
		imgdesc->width   = width;
		imgdesc->height  = height;
		HSD_CObjSetViewportfx4(shadow->camera, 0.0F, width, 0.0F, height);
		HSD_CObjSetScissorx4(shadow->camera, 0, width, 0, height);
	}
}

typedef struct _HSD_Chan {
	struct _HSD_Chan* next;
	u32 chan;
	u32 flags;
	GXColor amb_color;
	GXColor mat_color;
	u8 enable;
	u32 amb_src;
	u32 mat_src;
	u32 light_mask;
	u32 diff_fn;
	u32 attn_fn;
	struct _HSD_AObj* aobj;
} HSD_Chan;

HSD_Chan channel = {
	NULL,             // next
	4,                // chan
	0,                // flags
	{ 0 },            // amb_color
	{ 0, 0, 0, 255 }, // mat_color
	FALSE,            // enable
	0,                // amb_src
	0,                // mat_src
	0,                // light_mask
	2,                // diff_fn
	2,                // attn_fn
};

extern struct {
	u8 x0_pad[0xC];
	s32 unkC;
} HSD_PerfCurrentStat;

struct {
	u32 unused[29];
} tevstage = {
	0, 1, 0, 0xFF, 0xFF, 4, 0, 15, 15, 15, 10, 0, 0, 0x1000000, 0, 0, 7, 7, 7, 5, 0, 0, 0x1000000,
};

HSD_PEDesc pedesc = {
	9, 0, 0, 0, 0, 4, 5, 15, 7, 7, 0, 7,
};

f32 HSD_CObjGetEyeDistance();
void HSD_JObjDispAll(HSD_JObj* jobj, Mtx vmtx, u32 flags, u32 rendermode);
void C_MTXOrtho(void*, f32, f32, f32, f32, f32, f32);
void GXSetViewport(f32, f32, f32, f32, f32, f32);

/**
 * @note Address: 0x801AB6BC
 * @note Size: 0x2A0
 */
void HSD_ShadowStartRender(HSD_Shadow* shadow)
{
	Mtx44 sp8;
	HSD_Chan* chan;
	HSD_CObj* camera;
	u8 temp_r0;
	HSD_ImageDesc* imgdesc;
	HSD_SList* var_r26;

	ASSERT_NAMED(0x189, shadow);
	ASSERT_NAMED(0x18A, shadow->camera);
	ASSERT_NAMED(0x18B, shadow->texture);
	ASSERT_NAMED(0x18C, shadow->texture->imagedesc);

	camera  = shadow->camera;
	imgdesc = shadow->texture->imagedesc;
	if (shadow->active) {
		HSD_StateInitTev();
		HSD_SetupTevStageAll(&tevstage);
		HSD_StateSetNumTexGens();
		HSD_SetupPEMode(0, &pedesc);
		channel.mat_color.r = 0xFF;
		channel.mat_color.g = 0xFF;
		channel.mat_color.b = 0xFF;
		HSD_SetupChannelAll(&channel);
		C_MTXOrtho(&sp8, 1.0F, 0.0F, 0.0F, 1.0F, -1.0F, 1.0F);
		GXSetProjection(&sp8, 1);
		GXSetViewport(0.0F, 0.0F, imgdesc->width, imgdesc->height, 0.0F, 1.0F);
		GXSetScissor(0, 0, imgdesc->width, imgdesc->height);
		GXLoadPosMtxImm(&HSD_identityMtx, 0);
		GXSetCurrentMtx(0, HSD_PerfCurrentStat.unkC++, &HSD_PerfCurrentStat);
		HSD_ClearVtxDesc();
		GXSetVtxDesc(9, 1);
		GXSetVtxAttrFmt(0, 9, 0, 0, 0);
		HSD_StateSetCullMode(2);
		GXBegin(0x80, 0, 4);
		*(s8*)0xCC008000    = 0;
		*(s8*)0xCC008000    = 1;
		*(s8*)0xCC008000    = 1;
		*(s8*)0xCC008000    = 1;
		*(s8*)0xCC008000    = 1;
		*(s8*)0xCC008000    = 0;
		*(s8*)0xCC008000    = 0;
		*(s8*)0xCC008000    = 0;
		temp_r0             = shadow->intensity;
		channel.mat_color.r = temp_r0;
		channel.mat_color.g = temp_r0;
		channel.mat_color.b = temp_r0;
		HSD_SetupChannelAll(&channel);
		if (HSD_CObjGetEyeDistance(camera) >= __float_epsilon[0] && HSD_CObjSetCurrent(camera)) {
			GXSetScissor(2, 2, imgdesc->width - 4, imgdesc->height - 4);
			for (var_r26 = shadow->objects; var_r26 != NULL; var_r26 = var_r26->next) {
				HSD_JObjDispAll(var_r26->data, 0, 5, 0x04000000);
			}
			HSD_CObjEndCurrent();
		}
	}
}

/**
 * @note Address: 0x801AB420
 * @note Size: 0x29C
 */
void HSD_ShadowEndRender(HSD_Shadow* shadow)
{
	HSD_ImageDesc* imgdesc;
	ASSERT_NAMED(0x244, shadow);
	imgdesc = shadow->texture->imagedesc;
	if (imgdesc->img_ptr == NULL) {
		HSD_ShadowSetSize(shadow, imgdesc->width, imgdesc->height);
	}
	GXCopyTex(imgdesc->img_ptr, 1);
	GXPixModeSync();
	GXInvalidateTexAll();
	makeMatrix(shadow);
}

/**
 * @note Address: 0x801AB260
 * @note Size: 0x1C0
 */
void HSD_ShadowSetActive(HSD_Shadow* shadow, s32 active)
{
	ASSERT_NAMED(0x297, shadow);
	// skip if already in requested state
	if (shadow->active && active || !shadow->active && !active) {
		return;
	}

	shadow->active = active;
	if (active) {
		HSD_ImageDesc* imgdesc = shadow->texture->imagedesc;
		if (imgdesc->img_ptr == NULL) {
			HSD_ShadowSetSize(shadow, imgdesc->width, imgdesc->height);
		}
		HSD_MObjAddShadowTexture(shadow->texture);
	} else {
		HSD_MObjDeleteShadowTexture(shadow->texture);
	}
}

HSD_SList* HSD_SListPrepend();

/**
 * @note Address: 0x801AB1B4
 * @note Size: 0xAC
 */
void HSD_ShadowAddObject(HSD_Shadow* shadow, HSD_Obj* obj)
{
	HSD_SList* cur;

	if (shadow == NULL || obj == NULL) {
		return;
	}
	for (cur = shadow->objects; cur != NULL; cur = cur->next) {
		if (cur->data == obj) {
			return;
		}
	}
	shadow->objects = HSD_SListPrepend(shadow->objects, obj);
	if (obj != NULL) {
		obj->ref_count += 1;
		if (obj->ref_count == (u16)-1) {
			__assert("object.h", 0x5D, "HSD_OBJ(o)->ref_count != HSD_OBJ_NOREF");
		}
	}
}

/**
 * @note Address: 0x801AB120
 * @note Size: 0x94
 */
void HSD_ShadowDeleteObject(HSD_Shadow* shadow, struct _HSD_JObj* jobj)
{
	if (shadow == NULL) {
		return;
	}
	if (jobj != NULL) {
		HSD_SList* list = (HSD_SList*)shadow;
		while (list->next != NULL) {
			if (list->next->data == jobj) {
				HSD_JObjUnref(jobj);
				list->next = HSD_SListRemove(list->next);
				break;
			}
			list = list->next;
		}
	} else {
		while (shadow->objects != NULL) {
			HSD_JObjUnref(shadow->objects->data);
			shadow->objects = HSD_SListRemove(shadow->objects);
		}
	}
}

void C_MTXLightPerspective(Mtx, f32, f32, f32, f32, f32, f32);
void C_MTXLightFrustum(Mtx, f32, f32, f32, f32, f32, f32, f32, f32, f32);
void C_MTXLightOrtho(Mtx, f32, f32, f32, f32, f32, f32, f32, f32);

Mtx ShadowProjectionFallbackMtx = {
	{ 0.0F, 0.0F, 0.0F, 0.0F },
	{ 0.0F, 0.0F, 0.0F, 0.0F },
	{ 0.0F, 0.0F, 0.0F, 1.0F },
};

/**
 * @note Address: N/A
 * @note Size: 0x140
 */
static void makeMatrix(HSD_Shadow* shadow)
{
	Mtx Mprj;

	if (HSD_CObjGetEyeDistance(shadow->camera) < __float_epsilon[0]) {
		PSMTXCopy(ShadowProjectionFallbackMtx, shadow->texture->mtx);
		return;
	}

	switch (HSD_CObjGetProjectionType(shadow->camera)) {
	case PROJ_PERSPECTIVE:
		C_MTXLightPerspective(Mprj, shadow->camera->projection_param.perspective.fov, shadow->camera->projection_param.perspective.aspect,
		                      shadow->scaleS, shadow->scaleT, shadow->transS, shadow->transT);
		break;

	case PROJ_FRUSTRUM:
		C_MTXLightFrustum(Mprj, shadow->camera->projection_param.frustrum.top, shadow->camera->projection_param.frustrum.bottom,
		                  shadow->camera->projection_param.frustrum.left, shadow->camera->projection_param.frustrum.right,
		                  shadow->camera->near, shadow->scaleS, shadow->scaleT, shadow->transS, shadow->transT);
		break;

	case PROJ_ORTHO:
		C_MTXLightOrtho(Mprj, shadow->camera->projection_param.ortho.top, shadow->camera->projection_param.ortho.bottom,
		                shadow->camera->projection_param.ortho.left, shadow->camera->projection_param.ortho.right, shadow->scaleS,
		                shadow->scaleT, shadow->transS, shadow->transT);
		break;

	default:
		ASSERT_NAMED(0x357, 0);
	}

	PSMTXConcat(Mprj, shadow->camera->view_mtx, shadow->texture->mtx);
}

f64 atan2(f64, f64);

/**
 * @note Address: 0x801AAF54
 * @note Size: 0x1CC
 */
void HSD_ShadowSetViewingRect(HSD_Shadow* shadow, f32 top, f32 bottom, f32 left, f32 right)
{
	HSD_CObj* camera;
	f32 distance;

	ASSERT_NAMED(0x371, shadow);

	camera   = shadow->camera;
	distance = HSD_CObjGetEyeDistance(camera);
	if (distance < __float_epsilon[0]) {
		return;
	}

	switch (HSD_CObjGetProjectionType(camera)) {
	case PROJ_PERSPECTIVE: {
		f32 width, height;

		if (fabs(top) > fabs(bottom)) {
			width = fabs(top);
		} else {
			width = fabs(bottom);
		}
		if (fabs(left) > fabs(right)) {
			height = fabs(left);
		} else {
			height = fabs(right);
		}
		HSD_CObjSetAspect(camera, height / (width + __float_epsilon[0]));
		HSD_CObjSetFov(camera, (f32)atan2(width, distance) * 57.29578F);
	} break;

	case PROJ_ORTHO:
		HSD_CObjSetOrtho(camera, top, bottom, left, right);
		break;

	case PROJ_FRUSTRUM: {
		f32 scale = HSD_CObjGetNear(camera) / distance;
		if (scale < __float_epsilon[0]) {
			scale = __float_epsilon[0];
		}
		HSD_CObjSetFrustum(camera, scale * top, scale * bottom, scale * left, scale * right);
	} break;

	default:
		ASSERT_NAMED(0x39E, 0);
	}
}

extern void PSVECScale(f32 scale, Vec* src, Vec* dst);

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline BOOL TryNormalizeChecked(Vec* src, Vec* dst)
{
	if (src == NULL || dst == NULL) {
		return ~0;
	}
	if (__fabs(src->x) <= __float_min[0] && __fabs(src->y) <= __float_min[0] && __fabs(src->z) <= __float_min[0]) {
		return ~0;
	}
	PSVECNormalize(src, dst);
	return 0;
}

/**
 * @note Address: 0x801AAD48
 * @note Size: 0x20C
 */
void HSD_ViewingRectInit(HSD_ViewingRect* rect, Vec* position, Vec* interest, Vec* upvector, s32 perspective)
{
	ASSERT_NAMED(0x3BD, rect);

	rect->invalid = 0;
	rect->origin  = *position;
	PSVECSubtract(interest, position, &rect->eye_v);
	if (TryNormalizeChecked(&rect->eye_v, &rect->eye_vn) < 0) {
		rect->invalid = 1;
		return;
	}

	PSVECCrossProduct(&rect->eye_vn, upvector, &rect->right_v);
	if (TryNormalizeChecked(&rect->right_v, &rect->right_v) < 0) {
		HSD_VecGetOrthogonal(&rect->eye_vn, &rect->right_v);
		PSVECNormalize(&rect->right_v, &rect->right_v);
	}
	PSVECCrossProduct(&rect->right_v, &rect->eye_vn, &rect->up_v);
	rect->distance = PSVECMag(&rect->eye_v);

	rect->top = rect->right = -3.4028235E38F;
	rect->bottom = rect->left = 3.4028235E38F;
	rect->perspective         = perspective != 0;
}

/**
 * @note Address: 0x801AACCC
 * @note Size: 0x7C
 */
s32 HSD_ViewingRectCheck(HSD_ViewingRect* rect)
{
	ASSERT_NAMED(0x3DB, rect);
	return !rect->invalid && rect->top > rect->bottom && rect->right > rect->left;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
// stripped from the game, only its assert string is left in .rodata, between
// HSD_ViewingRectAddRect's strings and the rest. the name is the 2005 game's
// decomp's guess and the body is unknown past the assert
void HSD_ViewingRectScale(HSD_ViewingRect* rect, f32 a)
{
	ASSERT_NAMED(0x3E8, a > 0.0F);
}

/**
 * @note Address: 0x801AAABC
 * @note Size: 0x210
 */
void HSD_ViewingRectAddRect(HSD_ViewingRect* rect, Vec* position, f32 top, f32 bottom, f32 left, f32 right)
{
	f32 x, y, dot;
	Vec o2p, e2p;

	ASSERT_NAMED(0x401, rect);
	ASSERT_NAMED(0x402, position);

	if (rect->invalid) {
		return;
	}

	PSVECSubtract(position, &rect->origin, &o2p);
	dot = PSVECDotProduct(&o2p, &rect->eye_vn);
	if (rect->perspective) {
		f32 scale;

		if (dot <= 0.0F) {
			return;
		}
		scale = rect->distance / dot;
		PSVECScale(scale, &o2p, &o2p);
		PSVECSubtract(&o2p, &rect->eye_v, &e2p);
		x = PSVECDotProduct(&rect->right_v, &e2p);
		y = PSVECDotProduct(&rect->up_v, &e2p);

		top *= scale;
		bottom *= scale;
		left *= scale;
		right *= scale;
	} else {
		Vec tmp;
		PSVECScale(dot, &rect->eye_vn, &tmp);
		PSVECSubtract(&o2p, &tmp, &e2p);
		x = PSVECDotProduct(&rect->right_v, &e2p);
		y = PSVECDotProduct(&rect->up_v, &e2p);
	}

	if (x + right > rect->right) {
		rect->right = x + right;
	}
	if (x + left < rect->left) {
		rect->left = x + left;
	}
	if (y + top > rect->top) {
		rect->top = y + top;
	}
	if (y + bottom < rect->bottom) {
		rect->bottom = y + bottom;
	}
}
