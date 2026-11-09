extern void BeforeUpdate__5CGameFv(void);
extern void Draw__5CGameFv(void);
extern void FrameInterval__5CGameFv(void);
extern void GetCamera__5CGameFv(void);
extern void GetLight__5CGameFv(void);
extern void Initialize__5CGameFiPPc(void);
extern void Reset__5CGameFUl(void);
extern void Start__5CGameFv(void);
extern void Update__5CGameFUl(void);
extern void __dt__5CGameFv(void);
extern const void* gGameTypeInfoCBase[];
extern const void* gGameTypeInfoCGame[];
extern void vf20__5CGameFv(void);
extern void vf24__5CGameFv(void);
extern void vf2C__5CBaseFv(void);
extern void vf30__5CBaseFv(void);
extern void vf34__5CBaseFv(void);
extern void vf3C__5CBaseFv(void);
extern void vf40__5CBaseFv(void);

const void* sBaseTypesCGame[] = { gGameTypeInfoCBase, 0, 0 };
const void* __vt__5CGame[]    = { gGameTypeInfoCGame,      0,
	                              __dt__5CGameFv,          Initialize__5CGameFiPPc,
	                              Start__5CGameFv,         BeforeUpdate__5CGameFv,
	                              Update__5CGameFUl,       Draw__5CGameFv,
	                              vf20__5CGameFv,          vf24__5CGameFv,
	                              FrameInterval__5CGameFv, vf2C__5CBaseFv,
	                              vf30__5CBaseFv,          vf34__5CBaseFv,
	                              GetCamera__5CGameFv,     vf3C__5CBaseFv,
	                              vf40__5CBaseFv,          GetLight__5CGameFv,
	                              Reset__5CGameFUl };
