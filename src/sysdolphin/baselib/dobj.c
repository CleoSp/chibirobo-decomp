#include <sysdolphin/dobj.h>
#include <sysdolphin/gobjproc.h>

HSD_DObj* DObjCurrent;
HSD_DObjInfo* DObjDefaultClass;
#define current_dobj DObjCurrent
#define default_class DObjDefaultClass

void DObjInfoInit(void);

HSD_DObjInfo hsdDObj = { DObjInfoInit };

/**
 * @note Address: 0x801B7988
 * @note Size: 0x8
 */
void HSD_DObjSetCurrent(HSD_DObj* dobj)
{
    current_dobj = dobj;
    return;
}

/**
 * @note Address: 0x801B7990
 * @note Size: 0x20
 */
void HSD_DObjModifyFlags(HSD_DObj* dobj, u32 flags, u32 mask)
{
    if (dobj == NULL) {
        return;
    }
    dobj->flags = (dobj->flags & ~mask) | (flags & mask);
}

/**
 * @note Address: N/A
 * @note Size: 0x18
 */
u32 HSD_DObjGetFlags(HSD_DObj* dobj)
{
    if (dobj != NULL) {
        return dobj->flags;
    }
    return 0;
}

/**
 * @note Address: N/A
 * @note Size: 0x18
 */
void HSD_DObjSetFlags(HSD_DObj* dobj, u32 flags)
{
    if (dobj != NULL) {
        dobj->flags |= flags;
    }
}

/**
 * @note Address: N/A
 * @note Size: 0x18
 */
void HSD_DObjClearFlags(HSD_DObj* dobj, u32 flags)
{
    if (dobj != NULL) {
        dobj->flags &= ~flags;
    }
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_DObjRemoveAnimByFlags(HSD_DObj* dobj, u32 flags)
{
    if (dobj == NULL)
        return;

    if ((flags & 2) != 0) {
        HSD_AObjRemove(dobj->aobj);
        dobj->aobj = NULL;
    }
    HSD_PObjRemoveAnimAllByFlags(dobj->pobj, flags);
    HSD_MObjRemoveAnimByFlags(dobj->mobj, flags);
}

/**
 * @note Address: 0x801B79B0
 * @note Size: 0x98
 */
void HSD_DObjRemoveAnimAllByFlags(HSD_DObj* dobj, u32 flags)
{
    HSD_DObj* dp;

    if (dobj == NULL)
        return;

    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjRemoveAnimByFlags(dp, flags);
    }
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_DObjAddAnim(HSD_DObj* dobj, HSD_MatAnim* mat_anim, HSD_ShapeAnimDObj* sh_anim)
{
    if (dobj == NULL)
        return;

    HSD_PObjAddAnimAll(dobj->pobj, sh_anim != NULL ? sh_anim->shapeanim : NULL);
    HSD_MObjAddAnim(dobj->mobj, mat_anim);
}

/**
 * @note Address: 0x801B7A48
 * @note Size: 0xB0
 */
void HSD_DObjAddAnimAll(HSD_DObj* dobj, HSD_MatAnim* matanim, HSD_ShapeAnimDObj* shapeanimdobj)
{
    HSD_DObj* dp;
    HSD_MatAnim* ma;
    HSD_ShapeAnimDObj* sd;

    if (dobj == NULL)
        return;

    for (dp = dobj, ma = matanim, sd = shapeanimdobj; dp != NULL; dp = dp->next, ma = next_p(ma), sd = next_p(sd)) {
        HSD_DObjAddAnim(dp, ma, sd);
    }
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_DObjReqAnimByFlags(HSD_DObj* dobj, f32 startframe, u32 flags)
{
    if (dobj == NULL)
        return;

    HSD_PObjReqAnimAllByFlags(dobj->pobj, startframe, flags);
    HSD_MObjReqAnimByFlags(dobj->mobj, startframe, flags);
}

/**
 * @note Address: 0x801B7AF8
 * @note Size: 0x80
 */
void HSD_DObjReqAnimAllByFlags(HSD_DObj* dobj, f32 startframe, u32 flags)
{
    HSD_DObj* dp;

    if (dobj == NULL)
        return;

    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjReqAnimByFlags(dp, startframe, flags);
    }
}

static inline void HSD_DObjAnim(HSD_DObj* dobj)
{
    if (dobj == NULL) {
        return;
    }
    HSD_AObjInterpretAnim(dobj->aobj, dobj, HSD_DOBJ_METHOD(dobj)->update);
    HSD_PObjAnimAll(dobj->pobj);
    HSD_MObjAnim(dobj->mobj);
}

/**
 * @note Address: 0x801B7B78
 * @note Size: 0x6C
 */
void HSD_DObjAnimAll(HSD_DObj* dobj)
{
    HSD_DObj* dp;

    if (dobj == NULL) {
        return;
    }
    for (dp = dobj; dp != NULL; dp = dp->next) {
        HSD_DObjAnim(dp);
    }
}

HSD_PObj* HSD_PObjLoadDesc(HSD_PObjDesc* desc);
HSD_MObj* HSD_MObjLoadDesc(HSD_MObjDesc* desc);

/**
 * @note Address: 0x801B7BE4
 * @note Size: 0x110
 */
int DObjLoad(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    dobj->next = HSD_DObjLoadDesc(desc->next);
    dobj->mobj = HSD_MObjLoadDesc(desc->mobjdesc);
    dobj->pobj = HSD_PObjLoadDesc(desc->pobjdesc);

    if (dobj->mobj != NULL) {
        switch (dobj->mobj->rendermode & 0x60000000) {
            case 0:
                HSD_DObjModifyFlags(dobj, 2, 0xE);
                break;
            case 0x40000000:
                HSD_DObjModifyFlags(dobj, 8, 0xE);
                break;
            case 0x60000000:
                HSD_DObjModifyFlags(dobj, 4, 0xE);
                break;
            default:
                OSReport("mobj has unexpected blending flags (0x%x).", dobj->mobj->rendermode);
                HSD_Panic(__FILE__, 319, "");
        }
    }
    return 0;
}

/**
 * @note Address: N/A
 * @note Size: 0x58
 */
void HSD_DObjSetDefaultClass(HSD_DObjInfo* info)
{
    if (info != NULL)
        assert_line(0x155, hsdIsDescendantOf(info, &hsdDObj));
    default_class = info;
}

void* hsdNew();

/**
 * @note Address: 0x801B7CF4
 * @note Size: 0x98
 */
HSD_DObj* HSD_DObjLoadDesc(HSD_DObjDesc* desc)
{
    HSD_DObj* dobj;

    if (desc == NULL)
        return NULL;

    if (desc->class_name == NULL || hsdSearchClassInfo(desc->class_name) == NULL) {
        dobj = HSD_DObjAlloc();
    } else {
        dobj = HSD_DOBJ(hsdNew());
        assert_line(0x181, dobj);
    }
    HSD_DOBJ_METHOD(dobj)->load(dobj, desc);

    return dobj;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void hsdDelete(void* object)
{
    if (object == NULL)
        return;

    HSD_CLASS_METHOD(object)->release((HSD_Class*) object);
    HSD_CLASS_METHOD(object)->destroy((HSD_Class*) object);
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_DObjRemove(HSD_DObj* dobj)
{
    hsdDelete(dobj);
}

/**
 * @note Address: 0x801B7D8C
 * @note Size: 0x74
 */
void HSD_DObjRemoveAll(HSD_DObj* dobj)
{
    HSD_DObj* next;

    for (; dobj != NULL; dobj = next) {
        next = dobj->next;
        HSD_DObjRemove(dobj);
    }
}

/**
 * @note Address: 0x801B7E00
 * @note Size: 0x5C
 */
HSD_DObj* HSD_DObjAlloc(void)
{
    HSD_DObj* dobj = hsdNew((HSD_ClassInfo*) (default_class ? default_class : &hsdDObj));
    assert_line(0x214, dobj);
    return dobj;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void HSD_DObjResolveRefs(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    if (dobj == NULL || desc == NULL)
       return;
    HSD_PObjResolveRefsAll(dobj->pobj, desc->pobjdesc);
}

/**
 * @note Address: 0x801B7E5C
 * @note Size: 0x6C
 */
void HSD_DObjResolveRefsAll(HSD_DObj* dobj, HSD_DObjDesc* desc)
{
    for (; dobj != NULL && desc != NULL; dobj = dobj->next, desc = desc->next) {
        HSD_DObjResolveRefs(dobj, desc);
    }
}

void HSD_PObjCountVertices(HSD_PObj* pobj, u32* a, u32* b);

/**
 * @note Address: 0x801B7EC8
 * @note Size: 0x88
 */
void HSD_DObjCountVertices(HSD_DObj* dobj, u32* a, u32* b)
{
    // adds up two counts over the pobjs of every dobj in the list
    u32 total_a = 0;
    u32 total_b = 0;

    for (; dobj != NULL; dobj = dobj->next) {
        u32 pa, pb;
        HSD_PObjCountVertices(dobj->pobj, &pa, &pb);
        total_a += pa;
        total_b += pb;
    }
    if (a != NULL) {
        *a = total_a;
    }
    if (b != NULL) {
        *b = total_b;
    }
}

/**
 * @note Address: 0x801B7F50
 * @note Size: 0xC0
 */
void HSD_DObjDisp(HSD_DObj* dobj, Mtx vmtx, Mtx pmtx, u32 rendermode)
{
    HSD_PObj* p;

    HSD_MObjSetCurrent(dobj->mobj);
    if ((rendermode & 0x4000000) == 0) {
        HSD_MOBJ_METHOD(dobj->mobj)->setup(dobj->mobj, rendermode);
    }
    for (p = dobj->pobj; p != NULL; p = p->next) {
        HSD_POBJ_METHOD(p)->disp(p, vmtx, pmtx, rendermode);
    }
    if ((rendermode & 0x4000000) == 0) {
        HSD_MOBJ_METHOD(dobj->mobj)->unset(dobj->mobj, rendermode);
    }
    HSD_MObjSetCurrent(NULL);
}

/**
 * @note Address: 0x801B8010
 * @note Size: 0x5C
 */
void DObjRelease(HSD_Class* o)
{
    HSD_DObj* dobj = HSD_DOBJ(o);

    HSD_MObjRemove(dobj->mobj);
    HSD_PObjRemoveAll(dobj->pobj);
    HSD_AObjRemove(dobj->aobj);

    HSD_PARENT_INFO(&hsdDObj)->release(o);
}

/**
 * @note Address: 0x801B806C
 * @note Size: 0x48
 */
void DObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(default_class)) {
        default_class = NULL;
    }
    HSD_PARENT_INFO(&hsdDObj)->amnesia(info);
}

#pragma push
#pragma force_active on
static char unused1[] = "can not find specified pobj in link.\n";
static char unused2[] = "can not find specified pobj in link.";
static char unused3[] = "dobj->mobj == mobj";
#pragma pop

/**
 * @note Address: 0x801B80B4
 * @note Size: 0x80
 */
void DObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdDObj), &hsdClass,
        "sysdolphin_base_library", "hsd_dobj",
        sizeof(HSD_DObjInfo), sizeof(HSD_DObj));

    HSD_CLASS_INFO(&hsdDObj)->release = DObjRelease;
    HSD_CLASS_INFO(&hsdDObj)->amnesia = DObjAmnesia;
    hsdDObj.disp = HSD_DObjDisp;
    hsdDObj.load = DObjLoad;
}
