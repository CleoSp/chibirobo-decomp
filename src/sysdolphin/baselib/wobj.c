#include <dolphin/mtx/mtx.h>
#include <sysdolphin/wobj.h>
#include <sysdolphin/jobj.h>
#include <sysdolphin/robj.h>

void WObjInfoInit(void);

HSD_WObjInfo hsdWObj = { { { { WObjInfoInit } } } };

static HSD_WObjInfo* WObjCurrentInfo = NULL;

#define assert_line_named(line, cond, condstr) ((cond) ? ((void)0) : __assert("wobj.c", line, condstr))
// asserts from the jobj.h inlines carry that header's name
#define assert_line_jobjh(line, cond) ((cond) ? ((void)0) : __assert("jobj.h", line, "jobj"))

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline void wobj_JObjSetupMatrix(HSD_JObj* jobj)
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
static inline void HSD_WObjRemoveAnim(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        HSD_AObjRemove(wobj->aobj);
        wobj->aobj = NULL;
        HSD_RObjRemoveAnimAll(wobj->robj);
    }
}

/**
 * @note Address: 0x801D2F08
 * @note Size: 0x4C
 */
void HSD_WObjReqAnim(HSD_WObj* wobj, f32 frame)
{
    if (wobj != NULL) {
        HSD_AObjReqAnim(wobj->aobj, frame);
        HSD_RObjReqAnimAll(wobj->robj, frame);
    }
}

/**
 * @note Address: 0x801D2F54
 * @note Size: 0x68
 */
void HSD_WObjAddAnim(HSD_WObj* wobj, HSD_WObjAnim* anim)
{
    if (wobj != NULL && anim != NULL) {
        if (wobj->aobj != NULL) {
            HSD_AObjRemove(wobj->aobj);
        }
        wobj->aobj = HSD_AObjLoadDesc(anim->aobjdesc);
        HSD_RObjAddAnimAll(wobj->robj, anim->robjanim);
    }
}

/**
 * @note Address: 0x801D2FBC
 * @note Size: 0x144
 */
void WObjUpdateFunc(void* obj, u32 type, f32* fval)
{
    HSD_WObj* wobj;
    Vec p;
    HSD_JObj* jp;

    wobj = obj;
    if (wobj != NULL) {
        switch (type) {
        case 4:
            if (*fval < 0.0) {
                *fval = 0.0f;
            }
            if (1.0 < *fval) {
                *fval = 1.0f;
            }

            assert_line_named(152, wobj->aobj, "wobj->aobj");
            jp = (HSD_JObj*)wobj->aobj->hsd_obj;
            assert_line_named(154, jp, "jp");
            assert_line_named(155, jp->u.spline, "jp->u.spline");

            splArcLengthPoint(&p, jp->u.spline, *fval);
            HSD_WObjSetPosition(wobj, &p);
            wobj->flags |= 1;
            break;

        case 5:
            HSD_WObjSetPositionX(wobj, *fval);
            break;

        case 6:
            HSD_WObjSetPositionY(wobj, *fval);
            break;

        case 7:
            HSD_WObjSetPositionZ(wobj, *fval);
            break;
        }
    }
}

void HSD_RObjAnimAll();

/**
 * @note Address: 0x801D3100
 * @note Size: 0x48
 */
void HSD_WObjInterpretAnim(HSD_WObj* wobj)
{
    if (wobj != NULL) {
        HSD_AObjInterpretAnim(wobj->aobj, wobj,
                              (HSD_ObjUpdateFunc) HSD_WOBJ_METHOD(wobj)->update);
        HSD_RObjAnimAll(wobj->robj);
    }
}

/**
 * @note Address: 0x801D3148
 * @note Size: 0x68
 */
int WObjLoad(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    HSD_WObjSetPosition(wobj, &desc->pos);
    if (wobj->robj != NULL) {
        HSD_RObjRemoveAll(wobj->robj);
    }
    wobj->robj = HSD_RObjLoadDesc(desc->robjdesc);
    HSD_RObjResolveRefsAll(wobj->robj, desc->robjdesc);
    return 0;
}

/**
 * @note Address: 0x801D31B0
 * @note Size: 0x74
 */
void HSD_WObjInit(HSD_WObj* wobj, HSD_WObjDesc* desc)
{
    if (wobj == NULL || desc == NULL) {
        return;
    }

    HSD_WObjSetPosition(wobj, &desc->pos);
    if (wobj->robj != NULL) {
        HSD_RObjRemoveAll(wobj->robj);
    }
    wobj->robj = HSD_RObjLoadDesc(desc->robjdesc);
    HSD_RObjResolveRefsAll(wobj->robj, desc->robjdesc);
}

void* hsdNew();

/**
 * @note Address: 0x801D3224
 * @note Size: 0x98
 */
HSD_WObj* HSD_WObjLoadDesc(HSD_WObjDesc* desc)
{
    if (desc != NULL) {
        HSD_WObj* wobj;
        HSD_ClassInfo* info;
        if (desc->class_name == NULL || !(info = hsdSearchClassInfo(desc->class_name))) {
            wobj = HSD_WObjAlloc();
        } else {
            wobj = hsdNew(info);
            assert_line_named(260, wobj, "wobj");
        }
        HSD_WOBJ_METHOD(wobj)->load(wobj, desc);
        return wobj;
    }
    return NULL;
}

/**
 * @note Address: 0x801D32BC
 * @note Size: 0x48
 */
void HSD_WObjSetPosition(HSD_WObj* wobj, Vec* pos)
{
    if (wobj == NULL || pos == NULL) {
        return;
    }

    wobj->pos = *pos;
    wobj->flags |= 0x2;
    wobj->flags &= 0xFFFFFFFE;
}

/**
 * @note Address: 0x801D3304
 * @note Size: 0xD4
 */
void HSD_WObjSetPositionX(HSD_WObj* wobj, f32 val)
{
    HSD_JObj* jp;

    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            if (wobj->aobj != NULL && wobj->aobj->hsd_obj != NULL) {
                jp = (HSD_JObj*) wobj->aobj->hsd_obj;
                wobj_JObjSetupMatrix((HSD_JObj*) wobj->aobj->hsd_obj);
                PSMTXMultVec(jp->mtx, &wobj->pos, &wobj->pos);
            }
            wobj->flags &= 0xFFFFFFFE;
        }
        wobj->pos.x = val;
        wobj->flags |= 0x2;
    }
}

/**
 * @note Address: 0x801D33D8
 * @note Size: 0xD4
 */
void HSD_WObjSetPositionY(HSD_WObj* wobj, f32 val)
{
    HSD_JObj* jp;

    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            if (wobj->aobj != NULL && wobj->aobj->hsd_obj != NULL) {
                jp = (HSD_JObj*) wobj->aobj->hsd_obj;
                wobj_JObjSetupMatrix((HSD_JObj*) wobj->aobj->hsd_obj);
                PSMTXMultVec(jp->mtx, &wobj->pos, &wobj->pos);
            }
            wobj->flags &= 0xFFFFFFFE;
        }
        wobj->pos.y = val;
        wobj->flags |= 0x2;
    }
}

/**
 * @note Address: 0x801D34AC
 * @note Size: 0xD4
 */
void HSD_WObjSetPositionZ(HSD_WObj* wobj, f32 val)
{
    HSD_JObj* jp;

    if (wobj != NULL) {
        if ((wobj->flags & 1) != 0) {
            if (wobj->aobj != NULL && wobj->aobj->hsd_obj != NULL) {
                jp = (HSD_JObj*) wobj->aobj->hsd_obj;
                wobj_JObjSetupMatrix((HSD_JObj*) wobj->aobj->hsd_obj);
                PSMTXMultVec(jp->mtx, &wobj->pos, &wobj->pos);
            }
            wobj->flags &= 0xFFFFFFFE;
        }
        wobj->pos.z = val;
        wobj->flags |= 0x2;
    }
}

/**
 * @note Address: 0x801D3580
 * @note Size: 0xE8
 */
void HSD_WObjGetPosition(HSD_WObj* wobj, Vec* vec)
{
    HSD_JObj* jp;

    if (wobj == NULL || vec == NULL) {
        return;
    }
    if ((wobj->flags & 1) != 0) {
        if (wobj->aobj != NULL && wobj->aobj->hsd_obj != NULL) {
            jp = (HSD_JObj*)wobj->aobj->hsd_obj;
            wobj_JObjSetupMatrix((HSD_JObj*)wobj->aobj->hsd_obj);
            PSMTXMultVec(jp->mtx, &wobj->pos, &wobj->pos);
        }
        wobj->flags &= 0xFFFFFFFE;
    }
    *vec = wobj->pos;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
void HSD_WObjSetDefaultClass(HSD_WObjInfo* info)
{
    // stripped from the game, its assert string is still in .data.
    // the line is a guess, nothing left says what it was
    if (info) {
        assert_line_named(221, hsdIsDescendantOf(HSD_CLASS_INFO(info), HSD_CLASS_INFO(&hsdWObj)), "hsdIsDescendantOf(info, &hsdWObj)");
    }
    WObjCurrentInfo = info;
}

/**
 * @note Address: 0x801D3668
 * @note Size: 0x5C
 */
HSD_WObj* HSD_WObjAlloc(void)
{
    HSD_WObj* wobj =
        hsdNew((HSD_ClassInfo*) (WObjCurrentInfo ? WObjCurrentInfo : &hsdWObj));
    assert_line_named(599, wobj, "wobj");
    return wobj;
}

/**
 * @note Address: 0x801D36C4
 * @note Size: 0x54
 */
void WObjRelease(HSD_Class* o)
{
    HSD_WObj* wobj = (HSD_WObj*) o;
    HSD_RObjRemoveAll(wobj->robj);
    HSD_AObjRemove(wobj->aobj);
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->release(o);
}

/**
 * @note Address: 0x801D3718
 * @note Size: 0x48
 */
void WObjAmnesia(HSD_ClassInfo* info)
{
    if (info == HSD_CLASS_INFO(WObjCurrentInfo)) {
        WObjCurrentInfo = NULL;
    }
    HSD_OBJECT_PARENT_INFO(&hsdWObj)->amnesia(info);
}

/**
 * @note Address: 0x801D3760
 * @note Size: 0x80
 */
void WObjInfoInit(void)
{
    hsdInitClassInfo(HSD_CLASS_INFO(&hsdWObj), HSD_CLASS_INFO(&hsdObj), "sysdolphin_base_library", "had_wobj", sizeof(HSD_WObjInfo), sizeof(HSD_WObj));
    HSD_CLASS_INFO(&hsdWObj)->release = WObjRelease;
    HSD_CLASS_INFO(&hsdWObj)->amnesia = WObjAmnesia;
    HSD_WOBJ_INFO(&hsdWObj)->load     = WObjLoad;
    HSD_WOBJ_INFO(&hsdWObj)->update   = WObjUpdateFunc;
}
