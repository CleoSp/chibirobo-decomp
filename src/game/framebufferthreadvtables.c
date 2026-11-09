extern void BeforeUpdate__5CBaseFv(void);
extern void Copy__8FbMirrorFv(void);
extern void Copy__8FbSubwinFv(void);
extern void Create__7CThreadFPFPv_PvPvUllUs(void);
extern void Draw__5CBaseFv(void);
extern void Draw__8FbMirrorFv(void);
extern void Draw__8FbSubwinFv(void);
extern void FrameInterval__5CBaseFv(void);
extern void GetCamera__5CBaseFv(void);
extern void GetLight__5CBaseFv(void);
extern void Init__8FbMirrorFv(void);
extern void Init__8FbSubwinFv(void);
extern void Initialize__5CBaseFiPPc(void);
extern void Reset__5CBaseFUl(void);
extern void Reset__8FbMirrorFv(void);
extern void Reset__8FbSubwinFv(void);
extern void Start__5CBaseFv(void);
extern void Update__5CBaseFUl(void);
extern void Update__8FbMirrorFUl(void);
extern void Update__8FbSubwinFUl(void);
extern void __dt__5CBaseFv(void);
extern void __dt__7CThreadFv(void);
extern const void* gGameTypeInfoCBase[];
extern const void* gTypeInfoCFbTexEff[];
extern const void* gTypeInfoCThread[];
extern const void* gTypeInfoFbMirror[];
extern const void* gTypeInfoFbSubwin[];
extern void vf20__5CBaseFv(void);
extern void vf24__5CBaseFv(void);
extern void vf2C__5CBaseFv(void);
extern void vf30__5CBaseFv(void);
extern void vf34__5CBaseFv(void);
extern void vf3C__5CBaseFv(void);
extern void vf40__5CBaseFv(void);

const void* sBaseTypesFbSubwin[] = { gTypeInfoCFbTexEff, 0, 0 };
const void* __vt__8FbSubwin[]
    = { gTypeInfoFbSubwin, 0, Init__8FbSubwinFv, Reset__8FbSubwinFv, Copy__8FbSubwinFv, Update__8FbSubwinFUl, Draw__8FbSubwinFv };
const void* sBaseTypesFbMirror[] = { gTypeInfoCFbTexEff, 0, 0 };
const void* __vt__8FbMirror[]
    = { gTypeInfoFbMirror, 0, Init__8FbMirrorFv, Reset__8FbMirrorFv, Copy__8FbMirrorFv, Update__8FbMirrorFUl, Draw__8FbMirrorFv };
const void* __vt__5CBase[]   = { gGameTypeInfoCBase,      0,
	                             __dt__5CBaseFv,          Initialize__5CBaseFiPPc,
	                             Start__5CBaseFv,         BeforeUpdate__5CBaseFv,
	                             Update__5CBaseFUl,       Draw__5CBaseFv,
	                             vf20__5CBaseFv,          vf24__5CBaseFv,
	                             FrameInterval__5CBaseFv, vf2C__5CBaseFv,
	                             vf30__5CBaseFv,          vf34__5CBaseFv,
	                             GetCamera__5CBaseFv,     vf3C__5CBaseFv,
	                             vf40__5CBaseFv,          GetLight__5CBaseFv,
	                             Reset__5CBaseFUl };
const void* __vt__7CThread[] = { gTypeInfoCThread, 0, __dt__7CThreadFv, Create__7CThreadFPFPv_PvPvUllUs };
