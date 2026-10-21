#include <sysdolphin/robj.h>
#include <dolphin/mtx/mtx.h>
#include <dolphin/mtx/vec.h>
#include <kar/math.h>
#include <sysdolphin/gobjproc.h>
#include <sysdolphin/id.h>
#include <sysdolphin/memory.h>
#include <sysdolphin/util.h>
#include <sysdolphin/mtx.h>

static HSD_ObjAllocData hsdRObj_alloc_data;
static HSD_ObjAllocData hsdRvalueObj_alloc_data;

#define assert_line_named(line, cond, condstr) ((cond) ? ((void)0) : __assert("robj.c", line, condstr))
#define assert_line_jobjh(line, cond)          ((cond) ? ((void)0) : __assert("jobj.h", line, "jobj"))

extern void memset(void*, int, int);

extern void PSVECScale(f32 scale, Vec* src, Vec* dst);

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void robj_JObjSetupMatrix(HSD_JObj* jobj)
{
	BOOL dirty;

	if (jobj == NULL) {
		return;
	}
	assert_line_jobjh(0x25d, jobj);
	dirty = FALSE;
	if (!(jobj->flags & 0x800000) && (jobj->flags & 0x40)) {
		dirty = TRUE;
	}
	if (dirty) {
		HSD_JObjSetupMatrixSub(jobj);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline MtxPtr robj_JObjGetMtxPtr(HSD_JObj* jobj)
{
	assert_line_jobjh(0x47c, jobj);
	robj_JObjSetupMatrix(jobj);
	return jobj->mtx;
}

void expEvaluate(HSD_Exp* exp, u32 type, void* obj, HSD_ObjUpdateFunc update_func);
void HSD_RvalueRemoveAll(HSD_Rvalue* rvalue);
void HSD_RvalueResolveRefsAll(HSD_Rvalue* rvalue, HSD_RvalueList* list);
void HSD_RObjFree(HSD_RObj* robj);

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline f32 robj_sqrtf(f32 x)
{
	volatile f32 y;

	if (x > 0.0F) {
		f64 guess = __frsqrte((f64)x);
		guess     = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
		guess     = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
		guess     = 0.5 * guess * __fnmsub(x, guess * guess, 3.0);
		y         = (f32)(x * guess);
		return y;
	}
	return x;
}

/**
 * @note Address: 0x801D0C80
 * @note Size: 0x44
 */
void HSD_RObjInitAllocData(void)
{
	HSD_ObjAllocInit(&hsdRObj_alloc_data, sizeof(HSD_RObj), 4);
	HSD_ObjAllocInit(&hsdRvalueObj_alloc_data, sizeof(HSD_Rvalue), 4);
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
HSD_ObjAllocData* HSD_RObjGetAllocData(void)
{
	return &hsdRObj_alloc_data;
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
HSD_ObjAllocData* HSD_RvalueObjGetAllocData(void)
{
	return &hsdRvalueObj_alloc_data;
}

/**
 * @note Address: N/A
 * @note Size: 0x18
 */
void HSD_RObjSetFlags(HSD_RObj* robj, u32 flags)
{
	if (robj != NULL) {
		robj->flags |= flags;
	}
}

/**
 * @note Address: 0x801D0CC4
 * @note Size: 0x58
 */
HSD_RObj* HSD_RObjGetByType(HSD_RObj* robj, u32 type, u32 subtype)
{
	BOOL has_type;
	HSD_RObj* curr;

	if (robj == NULL) {
		return NULL;
	}

	for (curr = robj; curr != NULL; curr = curr->next) {
		if (curr->flags & 0x80000000) {
			has_type = TRUE;
		} else {
			has_type = FALSE;
		}

		if (has_type) {
			if ((curr->flags & TYPE_MASK) == type && (!subtype || subtype == (curr->flags & 0xFFFFFFF))) {
				return curr;
			}
		}
	}

	return NULL;
}

/**
 * @note Address: 0x801D0D1C
 * @note Size: 0x44
 */
void RObjAnimUpdateFlags(void* obj, s32 type, FObjData* val)
{
	HSD_RObj* robj;

	if (obj == NULL) {
		return;
	}

	if (type != TYPE_ROBJ) {
		return;
	}

	robj = (HSD_RObj*)obj;
	if (val->fv >= 0.5) {
		robj->flags = robj->flags | 0x80000000;
		return;
	}
	robj->flags = robj->flags & 0x7fffffff;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RObjAnim(HSD_RObj* robj)
{
	if (robj == NULL) {
		return;
	}

	HSD_AObjInterpretAnim(robj->aobj, robj, RObjAnimUpdateFlags);
}

/**
 * @note Address: 0x801D0D60
 * @note Size: 0x68
 */
void HSD_RObjAnimAll(HSD_RObj* robj)
{
	HSD_RObj* curr;

	if (robj == NULL) {
		return;
	}

	for (curr = robj; curr != NULL; curr = curr->next) {
		HSD_RObjAnim(curr);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RObjRemoveAnimByFlags(HSD_RObj* robj, u32 flags)
{
	if (robj == NULL) {
		return;
	}

	if (robj->aobj != NULL && (flags & 0x80) != 0) {
		HSD_AObjRemove(robj->aobj);
		robj->aobj = NULL;
	}
}

/**
 * @note Address: 0x801D0DC8
 * @note Size: 0x84
 */
void HSD_RObjRemoveAnimAllByFlags(HSD_RObj* robj, u32 flags)
{
	if (robj == NULL) {
		return;
	}

	for (; robj != NULL; robj = robj->next) {
		HSD_RObjRemoveAnimByFlags(robj, flags);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RObjReqAnimByFlags(HSD_RObj* robj, f32 startframe, u32 flags)
{
	if (robj == NULL) {
		return;
	}

	if (robj->aobj != NULL && (flags & 0x80) != 0) {
		HSD_AObjReqAnim(robj->aobj, startframe);
	}
}

/**
 * @note Address: 0x801D0E4C
 * @note Size: 0x84
 */
void HSD_RObjReqAnimAllByFlags(HSD_RObj* robj, f32 startframe, u32 flags)
{
	if (robj == NULL) {
		return;
	}

	for (; robj != NULL; robj = robj->next) {
		HSD_RObjReqAnimByFlags(robj, startframe, flags);
	}
}

/**
 * @note Address: 0x801D0ED0
 * @note Size: 0x6C
 */
void HSD_RObjReqAnimAll(HSD_RObj* robj, f32 startframe)
{
	HSD_RObjReqAnimAllByFlags(robj, startframe, 0x7FF);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RObjAddAnim(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
	if (robj == NULL || anim == NULL) {
		return;
	}

	if (robj->aobj != NULL) {
		HSD_AObjRemove(robj->aobj);
	}
	robj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
}

/**
 * @note Address: 0x801D0F3C
 * @note Size: 0x90
 */
void HSD_RObjAddAnimAll(HSD_RObj* robj, HSD_RObjAnimJoint* anim)
{
	HSD_RObj* i;
	HSD_RObjAnimJoint* j;

	if (robj == NULL || anim == NULL) {
		return;
	}

	for (i = robj, j = anim; i != NULL && j != NULL; i = i->next, j = j->next) {
		HSD_RObjAddAnim(i, j);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline BOOL RObjIsRefType(HSD_RObj* robj, u32 type)
{
	if ((robj->flags & TYPE_MASK) == type) {
		return TRUE;
	}
	return FALSE;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static u32 HSD_RObjGetConstraintType(HSD_RObj* robj)
{
	if (robj == NULL) {
		return 0;
	}
	return robj->flags & 0xFFFFFFF;
}

/**
 * @note Address: 0x801D0FCC
 * @note Size: 0x1B4
 */
int HSD_RObjGetGlobalPosition(HSD_RObj* robj, int type, Vec* p)
{
	Vec v = { 0.0F, 0.0F, 0.0F };
	HSD_RObj* rp;
	int n = 0;

	if (robj == NULL) {
		return 0;
	}
	for (rp = robj; rp != NULL; rp = rp->next) {
		if (RObjIsRefType(rp, REFTYPE_JOBJ)) {
			if ((rp->flags & 0x80000000) != 0 && (u32)type == HSD_RObjGetConstraintType(rp)) {
				assert_line_named(0x1F2, rp->u.jobj, "rp->u.jobj");
				robj_JObjSetupMatrix(rp->u.jobj);
				n += 1;
				v.x += rp->u.jobj->mtx[0][3];
				v.y += rp->u.jobj->mtx[1][3];
				v.z += rp->u.jobj->mtx[2][3];
			}
		}
	}
	if (n != 0) {
		f32 f = 1.0F / (f32)n;
		p->x  = f * v.x;
		p->y  = f * v.y;
		p->z  = f * v.z;
	}
	return n;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static void set_dirup_matrix(Vec* dir_ptr, Vec* uv_ptr, Vec* scale_ptr, void* obj, HSD_ObjUpdateFunc update_func)
{
	Vec z_vec;
	Vec v;
	f32 kz;
	f32 kdir;

	PSVECCrossProduct(dir_ptr, uv_ptr, &z_vec);
	kdir = robj_sqrtf(1.0F / (1e-10F + PSVECDotProduct(dir_ptr, dir_ptr)));
	PSVECScale(kdir, dir_ptr, dir_ptr);
	kz = robj_sqrtf(1.0F / (1e-10F + PSVECDotProduct(&z_vec, &z_vec)));
	PSVECScale(kz, &z_vec, &z_vec);
	PSVECCrossProduct(&z_vec, dir_ptr, uv_ptr);
	v.x = dir_ptr->x * scale_ptr->x;
	v.y = dir_ptr->y * scale_ptr->x;
	v.z = dir_ptr->z * scale_ptr->x;
	update_func(obj, 0x32, (HSD_ObjData*)&v);
	v.x = uv_ptr->x * scale_ptr->y;
	v.y = uv_ptr->y * scale_ptr->y;
	v.z = uv_ptr->z * scale_ptr->y;
	update_func(obj, 0x33, (HSD_ObjData*)&v);
	v.x = z_vec.x * scale_ptr->z;
	v.y = z_vec.y * scale_ptr->z;
	v.z = z_vec.z * scale_ptr->z;
	update_func(obj, 0x34, (HSD_ObjData*)&v);
	update_func(obj, 0x37, NULL);
}

/**
 * @note Address: 0x801D1180
 * @note Size: 0x5E8
 */
void resolveCnsDirUp(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
	Vec this_scale = { 1.0F, 1.0F, 1.0F };
	Vec up         = { 0.0F, 1.0F, 0.0F };
	Vec this_pos;
	Vec dir;
	f32 k;

	if (HSD_RObjGetGlobalPosition(robj, 2, &this_pos) != 0) {
		dir.x = ((HSD_JObj*)obj)->mtx[0][3];
		dir.y = ((HSD_JObj*)obj)->mtx[1][3];
		dir.z = ((HSD_JObj*)obj)->mtx[2][3];
		PSVECSubtract(&this_pos, &dir, &this_pos);
		if (HSD_RObjGetGlobalPosition(robj, 3, &up) != 0) {
			PSVECSubtract(&up, &dir, &up);
		} else {
			k = 1.0F - PSVECDotProduct(&this_pos, &up);
			if (__fabs(k) < 1e-10F) {
				up.x = 0.0F;
				up.y = 0.0F;
				up.z = 1.0F;
			}
		}

		if (((HSD_JObj*)obj)->scl != NULL) {
			this_scale = *((HSD_JObj*)obj)->scl;
		}
		set_dirup_matrix(&this_pos, &up, &this_scale, obj, update_func);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline HSD_RObj* inlined_HSD_RObjGetByType(HSD_RObj* robj, u32 type, u32 subtype)
{
	BOOL has_type;
	HSD_RObj* curr;

	if (robj == NULL) {
		return NULL;
	}

	for (curr = robj; curr != NULL; curr = curr->next) {
		if (curr->flags & 0x80000000) {
			has_type = TRUE;
		} else {
			has_type = FALSE;
		}

		if (has_type) {
			if ((curr->flags & TYPE_MASK) == type && (!subtype || subtype == (curr->flags & 0xFFFFFFF))) {
				return curr;
			}
		}
	}

	return NULL;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline f32 HSD_MtxColMagFloat(MtxPtr mtx, int col)
{
	return robj_sqrtf((mtx[0][col] * mtx[0][col]) + (mtx[1][col] * mtx[1][col]) + (mtx[2][col] * mtx[2][col]));
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_MtxColVec(MtxPtr mtx, int col, Vec* vec)
{
	vec->x = mtx[0][col];
	vec->y = mtx[1][col];
	vec->z = mtx[2][col];
}

static s32 RObjOrientationUpdateKinds[3] = { 0x32, 0x33, 0x34 };

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void resolveCnsOrientation(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
	Mtx mtx0;
	f32 sval;
	Vec v;
	HSD_JObj* jobj;
	HSD_RObj* found;
	int i;

	assert_line_named(0x276, obj, "obj");

	found = inlined_HSD_RObjGetByType(robj, REFTYPE_JOBJ, 4);
	if (found == NULL) {
		return;
	}

	PSMTXCopy(robj_JObjGetMtxPtr(found->u.jobj), mtx0);
	jobj = obj;

	for (i = 0; i < 3; i++) {
		HSD_MtxColVec(mtx0, i, &v);
		sval = PSVECMag(&v);
		if (sval > 1e-10F) {
			sval = 1.0F / sval;
		}
		sval *= HSD_MtxColMagFloat(jobj->mtx, i);
		v.x *= sval;
		v.y *= sval;
		v.z *= sval;
		update_func(obj, RObjOrientationUpdateKinds[i], (HSD_ObjData*)&v);
	}
	update_func(obj, 0x37, NULL);
}

/**
 * @note Address: 0x801D1768
 * @note Size: 0x208
 */
void resolveLimits(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
	HSD_JObj* jobj = (HSD_JObj*)obj;
	HSD_RObj* rp;
	BOOL update_mtx = FALSE;

	assert_line_named(0x29e, jobj, "jobj");

	rp = robj;
	while (rp != NULL) {
		if (RObjIsRefType(rp, REFTYPE_LIMIT)) {
			break;
		}
		rp = rp->next;
	}

	if (rp != NULL) {
		for (rp = robj; rp != NULL; rp = rp->next) {
			if (RObjIsRefType(rp, REFTYPE_LIMIT)) {
				switch (rp->flags & 0xFFFFFFF) {
				default:
					continue;
				case 1:
					if (jobj->rotate.x < rp->u.limit) {
						jobj->rotate.x = rp->u.limit;
					}
					break;

				case 2:
					if (jobj->rotate.x > rp->u.limit) {
						jobj->rotate.x = rp->u.limit;
					}
					break;

				case 3:
					if (jobj->rotate.y < rp->u.limit) {
						jobj->rotate.y = rp->u.limit;
					}
					break;

				case 4:
					if (jobj->rotate.y > rp->u.limit) {
						jobj->rotate.y = rp->u.limit;
					}
					break;

				case 5:
					if (jobj->rotate.z < rp->u.limit) {
						jobj->rotate.z = rp->u.limit;
					}
					break;

				case 6:
					if (jobj->rotate.z > rp->u.limit) {
						jobj->rotate.z = rp->u.limit;
					}
					break;

				case 7:
					if (jobj->translate.x < rp->u.limit) {
						jobj->translate.x = rp->u.limit;
					}
					break;

				case 8:
					if (jobj->translate.x > rp->u.limit) {
						jobj->translate.x = rp->u.limit;
					}
					break;

				case 9:
					if (jobj->translate.y < rp->u.limit) {
						jobj->translate.y = rp->u.limit;
					}
					break;

				case 10:
					if (jobj->translate.y > rp->u.limit) {
						jobj->translate.y = rp->u.limit;
					}
					break;

				case 11:
					if (jobj->translate.z < rp->u.limit) {
						jobj->translate.z = rp->u.limit;
					}
					break;

				case 12:
					if (jobj->translate.z > rp->u.limit) {
						jobj->translate.z = rp->u.limit;
					}
					break;
				}
				update_mtx = TRUE;
			}
		}
		if (update_mtx) {
			HSD_JObjMakeMatrix(jobj);
		}
	}
}

/**
 * @note Address: 0x801D1970
 * @note Size: 0x484
 */
/**
 * @note Address: N/A
 * @note Size: ?
 */
// the position constraint as its own inline like resolveCnsDirUp and
// resolveCnsOrientation, the name is ours. written straight into
// HSD_RObjUpdateAll its vec and the inlined HSD_RObjGetGlobalPosition's v
// take the wrong stack slots
static inline void resolveCnsPosition(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
	Vec vec;

	if (HSD_RObjGetGlobalPosition(robj, 1, &vec) != 0) {
		update_func(obj, 0x35, (HSD_ObjData*)&vec);
		update_func(obj, 0x38, NULL);
	}
}

void HSD_RObjUpdateAll(HSD_RObj* robj, void* obj, HSD_ObjUpdateFunc update_func)
{
	HSD_RObj* rp;

	if (robj != NULL) {
		resolveCnsPosition(robj, obj, update_func);
		resolveCnsDirUp(robj, obj, update_func);
		resolveCnsOrientation(robj, obj, update_func);
		resolveLimits(robj, obj, update_func);

		for (rp = robj; rp != NULL; rp = rp->next) {
			if (RObjIsRefType(rp, REFTYPE_EXP) && (rp->flags & 0x80000000) != 0) {
				expEvaluate(&rp->u.exp, rp->flags & 0xFFFFFFF, obj, update_func);
			}
		}
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RObjResolveRefs(HSD_RObj* robj, HSD_RObjDesc* desc)
{
	if (robj != NULL && desc != NULL) {
		switch (robj->flags & TYPE_MASK) {
		case REFTYPE_JOBJ:
			HSD_JObjUnrefThis(robj->u.jobj);
			robj->u.jobj = HSD_IDGetData((u32)desc->u.joint, NULL);
			assert_line_named(0x330, robj->u.jobj, "robj->u.jobj");
			if (robj->u.jobj != NULL) {
				iref_INC(robj->u.jobj);
			}
			break;
		case REFTYPE_EXP:
			HSD_RvalueResolveRefsAll(robj->u.exp.rvalue, desc->u.exp->rvalue);
			break;
		}
	}
}

/**
 * @note Address: 0x801D1DF4
 * @note Size: 0x114
 */
void HSD_RObjResolveRefsAll(HSD_RObj* robj, HSD_RObjDesc* desc)
{
	for (; robj != NULL && desc != NULL; robj = robj->next, desc = desc->next) {
		HSD_RObjResolveRefs(robj, desc);
	}
}

void expLoadDesc(HSD_Exp* exp, HSD_ExpDesc* desc);
void bcexpLoadDesc(HSD_Exp* exp, HSD_ByteCodeExpDesc* desc);

/**
 * @note Address: 0x801D1F08
 * @note Size: 0x474
 */
HSD_RObj* HSD_RObjLoadDesc(HSD_RObjDesc* robjdesc)
{
	HSD_RObj* robj;

	if (robjdesc != NULL) {
		robj        = HSD_RObjAlloc();
		robj->next  = HSD_RObjLoadDesc(robjdesc->next);
		robj->flags = robjdesc->flags;
		switch (robj->flags & TYPE_MASK) {
		case REFTYPE_JOBJ:
			break;
		case REFTYPE_LIMIT:
			switch (robj->flags & 0xFFFFFFF) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				robj->u.limit = 0.017453292F * robjdesc->u.limit;
				break;
			default:
				robj->u.limit = robjdesc->u.limit;
				break;
			}
			break;
		case REFTYPE_EXP:
			expLoadDesc(&robj->u.exp, robjdesc->u.exp);
			break;
		case REFTYPE_BYTECODE:
			bcexpLoadDesc(&robj->u.exp, robjdesc->u.bcexp);
			robj->flags &= ~TYPE_MASK;
			break;
		case REFTYPE_IKHINT:
			robj->u.ik_hint.bone_length = robjdesc->u.ik_hint->bone_length;
			robj->u.ik_hint.rotate_x    = robjdesc->u.ik_hint->rotate_x;
			break;
		default:
			HSD_Panic("robj.c", 0x37d, "unexpected type of robj.\n");
			break;
		}
		return robj;
	}
	return NULL;
}

#pragma push
#pragma force_active on
static char HSD_RObj_80406F14[] = "(ptr && nitems) || !ptr";
#pragma pop

/**
 * @note Address: N/A
 * @note Size: 0x74
 */
void HSD_RObjRemove(HSD_RObj* robj)
{
	if (robj != NULL) {
		switch (robj->flags & TYPE_MASK) {
		case REFTYPE_JOBJ:
			HSD_JObjUnrefThis(robj->u.jobj);
			break;
		case REFTYPE_EXP:
			HSD_RvalueRemoveAll(robj->u.exp.rvalue);
			break;
		}
		HSD_AObjRemove(robj->aobj);
		HSD_RObjFree(robj);
	}
}

/**
 * @note Address: 0x801D237C
 * @note Size: 0x9C
 */
void HSD_RObjRemoveAll(HSD_RObj* robj)
{
	HSD_RObj* next;

	for (; robj != NULL; robj = next) {
		next = robj->next;
		HSD_RObjRemove(robj);
	}
}

/**
 * @note Address: 0x801D2418
 * @note Size: 0x5C
 */
HSD_RObj* HSD_RObjAlloc(void)
{
	HSD_RObj* new = HSD_ObjAlloc(&hsdRObj_alloc_data);
	assert_line_named(0x3c5, new, "new");
	memset(new, 0, sizeof(HSD_RObj));
	return new;
}

/**
 * @note Address: 0x801D2474
 * @note Size: 0x2C
 */
void HSD_RObjFree(HSD_RObj* robj)
{
	HSD_ObjFree(&hsdRObj_alloc_data, robj);
}

static u32 arg_buf_size;
static f32* arg_buf;

extern f32 HSD_ByteCodeEval(u8* bytecode, f32* argv, s32 nb_args);

/**
 * @note Address: 0x801D24A0
 * @note Size: 0x49C
 */
void expEvaluate(HSD_Exp* exp, u32 type, void* obj, HSD_ObjUpdateFunc update_func)
{
	HSD_Rvalue* rvalue;
	Vec scale;
	Quaternion rotate; /* only xyz get written, the slot is quaternion sized like jobj's rotate */
	f32* cur_arg;
	int cur_bit;
	HSD_ObjData sp1C;

	if (exp->nb_args == -1) {
		u32 nb_args = 0;
		HSD_Rvalue* rvalue2;
		for (rvalue2 = exp->rvalue; rvalue2 != NULL; rvalue2 = rvalue2->next) {
			nb_args += HSD_GetNbBits(rvalue2->flags);
		}
		exp->nb_args = nb_args;
	}
	if (arg_buf == NULL) {
		if (arg_buf_size == 0) {
			arg_buf_size = 100;
		}
		arg_buf = HSD_Alloc(arg_buf_size * sizeof(f32));
	}
	if (arg_buf_size < exp->nb_args) {
		OSReport("Number of argment of expression exceeds the argument buffer\n"
		         "size. (requested num of arg %d, allocated %d)\n",
		         exp->nb_args, arg_buf_size);
		HSD_Panic("robj.c", 0x41b, "");
	}
	cur_arg = arg_buf;
	for (rvalue = exp->rvalue; rvalue != NULL; rvalue = rvalue->next) {
		HSD_JObj* jobj = rvalue->jobj;
		assert_line_named(0x424, jobj, "jobj");
		robj_JObjSetupMatrix(rvalue->jobj);
		for (cur_bit = 1; cur_bit && cur_bit <= rvalue->flags; cur_bit <<= 1) {
			switch (rvalue->flags & cur_bit) {
			case 0x1:
				*cur_arg++ = 57.29578F * jobj->rotate.x;
				break;
			case 0x2:
				*cur_arg++ = 57.29578F * jobj->rotate.y;
				break;
			case 0x4:
				*cur_arg++ = 57.29578F * jobj->rotate.z;
				break;
			case 0x8:
				break;
			case 0x10:
				*cur_arg++ = jobj->translate.x;
				break;
			case 0x20:
				*cur_arg++ = jobj->translate.y;
				break;
			case 0x40:
				*cur_arg++ = jobj->translate.z;
				break;
			case 0x80:
				*cur_arg++ = jobj->scale.x;
				break;
			case 0x100:
				*cur_arg++ = jobj->scale.y;
				break;
			case 0x200:
				*cur_arg++ = jobj->scale.z;
				break;
			case 0x400:
			case 0x800:
				break;
			case 0x10000:
				HSD_MtxGetRotation(jobj->mtx, (Vec*)&rotate);
				*cur_arg++ = 57.29578F * rotate.x;
				break;
			case 0x20000:
				HSD_MtxGetRotation(jobj->mtx, (Vec*)&rotate);
				*cur_arg++ = 57.29578F * rotate.y;
				break;
			case 0x40000:
				HSD_MtxGetRotation(jobj->mtx, (Vec*)&rotate);
				*cur_arg++ = 57.29578F * rotate.z;
				break;
			case 0x100000:
				*cur_arg++ = jobj->mtx[0][3];
				break;
			case 0x200000:
				*cur_arg++ = jobj->mtx[1][3];
				break;
			case 0x400000:
				*cur_arg++ = jobj->mtx[2][3];
				break;
			case 0x800000:
				HSD_MtxGetScale(jobj->mtx, &scale);
				*cur_arg++ = scale.x;
				break;
			case 0x1000000:
				HSD_MtxGetScale(jobj->mtx, &scale);
				*cur_arg++ = scale.y;
				break;
			case 0x2000000:
				HSD_MtxGetScale(jobj->mtx, &scale);
				*cur_arg++ = scale.z;
				break;
			}
		}
	}
	if (exp->is_bytecode) {
		sp1C.fv = HSD_ByteCodeEval(exp->expr.bytecode, arg_buf, exp->nb_args);
	} else {
		sp1C.fv = exp->expr.func(arg_buf);
	}
	if (type == 1 || type == 2 || type == 3) {
		sp1C.fv = sp1C.fv * 0.017453292F;
	}
	update_func(obj, type, &sp1C);
}

/**
 * @note Address: 0x801D293C
 * @note Size: 0x8
 */
f32 RObjEvalZero(void* unused)
{
	return 0.0F;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline HSD_Rvalue* HSD_RvalueAlloc(void)
{
	HSD_Rvalue* rvalue = HSD_ObjAlloc(&hsdRvalueObj_alloc_data);
	assert_line_named(0x485, rvalue, "rvalue");
	memset(rvalue, 0, sizeof(HSD_Rvalue));
	return rvalue;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RvalueRemove(HSD_Rvalue* rvalue)
{
	if (rvalue != NULL) {
		HSD_JObjUnrefThis(rvalue->jobj);
		HSD_ObjFree(&hsdRvalueObj_alloc_data, rvalue);
	}
}

/**
 * @note Address: 0x801D2944
 * @note Size: 0x70
 */
void HSD_RvalueRemoveAll(HSD_Rvalue* rvalue)
{
	HSD_Rvalue* next;

	for (; rvalue != NULL; rvalue = next) {
		next = rvalue->next;
		HSD_RvalueRemove(rvalue);
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline HSD_Rvalue* loadRvalue(HSD_RvalueList* list)
{
	HSD_Rvalue* rp;
	HSD_Rvalue rv;

	rv.next = NULL;
	rp      = &rv;
	if (list == NULL) {
		return NULL;
	} else {
		for (; list->joint != NULL; list++) {
			rp->next        = HSD_RvalueAlloc();
			rp->next->flags = list->flags;
			rp              = rp->next;
		}
	}
	return rv.next;
}

/**
 * @note Address: 0x801D29B4
 * @note Size: 0xF4
 */
void expLoadDesc(HSD_Exp* exp, HSD_ExpDesc* desc)
{
	memset(exp, 0, sizeof(HSD_Exp));
	if (desc != NULL) {
		if (desc->func != NULL) {
			exp->expr.func = desc->func;
		} else {
			exp->expr.func = RObjEvalZero;
		}
		exp->rvalue  = loadRvalue(desc->rvalue);
		exp->nb_args = -1;
	}
}

/**
 * @note Address: 0x801D2AA8
 * @note Size: 0xF8
 */
void bcexpLoadDesc(HSD_Exp* exp, HSD_ByteCodeExpDesc* desc)
{
	memset(exp, 0, sizeof(HSD_Exp));
	if (desc != NULL) {
		if (desc->bytecode != NULL) {
			exp->expr.bytecode = desc->bytecode;
		} else {
			exp->expr.bytecode = NULL;
		}
		exp->rvalue      = loadRvalue(desc->rvalue);
		exp->nb_args     = -1;
		exp->is_bytecode = 1;
	}
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_RvalueResolveRefs(HSD_Rvalue* rvalue, HSD_RvalueList* list)
{
	if (rvalue != NULL && list != NULL) {
		HSD_JObjUnrefThis(rvalue->jobj);
		rvalue->jobj = HSD_IDGetData((u32)list->joint, NULL);
		assert_line_named(0x4F2, rvalue->jobj, "rvalue->jobj");
		if (rvalue->jobj != NULL) {
			iref_INC(rvalue->jobj);
		}
	}
}

/**
 * @note Address: 0x801D2BA0
 * @note Size: 0xE8
 */
void HSD_RvalueResolveRefsAll(HSD_Rvalue* rvalue, HSD_RvalueList* list)
{
	if (list == NULL) {
		return;
	}
	for (; rvalue != NULL && list->joint != NULL; rvalue = rvalue->next, list++) {
		HSD_RvalueResolveRefs(rvalue, list);
	}
}

/**
 * @note Address: N/A
 * @note Size: 0xCC
 */
void HSD_RObjSetConstraintObj(HSD_RObj* robj, void* o)
{
	if (robj != NULL) {
		if (robj->u.jobj != NULL) {
			HSD_JObjUnrefThis(robj->u.jobj);
			robj->u.jobj = NULL;
		}

		if (hsdObjIsDescendantOf((HSD_Class*)&((HSD_JObj*)o)->object, (HSD_ClassInfo*)&hsdJObj)) {
			robj->u.jobj = o;
			if (o != NULL) {
				iref_INC(o);
			}
		} else {
			// stripped from the game, its line number is a guess. the
			// assert form would leave a "0" string behind and retail has none
			OSReport("constraint only support jobj target.\n");
			HSD_Panic("robj.c", 0x51D, "");
		}
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x38
 */
void _HSD_RObjForgetMemory(void)
{
	if (_HSD_MemCheckOwn(arg_buf)) {
		arg_buf      = 0;
		arg_buf_size = 0;
	}
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
HSD_ObjAllocData* HSD_IDGetAllocData(void)
{
	return &hsd_iddata;
}
