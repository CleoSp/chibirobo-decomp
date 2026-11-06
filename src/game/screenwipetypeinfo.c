const char sTypeNameCScrnRoboeyes[]   = "CScrnRoboeyes";
const char sWipeTypeNameCScrnColor[]  = "CScrnColor";
const char sWipeTypeNameCScrnWipe[]   = "CScrnWipe";
const char sWipeTypeNameGXTexObj[]    = "_GXTexObj";
const char sTypeNameCScrnWipeCustom[] = "CScrnWipeCustom";
const char sTypeNameCScrnWipeTex[]    = "CScrnWipeTex";
#if defined(VERSION_GGTP01)
const char sWipeTypeNameCScrnWipeTile[20] = "CScrnWipeTile";
#else
const char sWipeTypeNameCScrnWipeTile[20] = "CScrnWipeTile\0"
                                            "000000";
#endif

extern const void* sBaseTypesCScrnWipeCustom[];
extern const void* sBaseTypesCScrnWipeTex[];
extern const void* sBaseTypesCScrnWipeTile[];

extern const void* sBaseTypesCScrnWipe[];
extern const void* sBaseTypesCScrnRoboeyes[];

const void* gWipeTypeInfoCScrnColor[]  = { sWipeTypeNameCScrnColor, 0 };
const void* gTypeInfoCScrnWipe[]       = { sWipeTypeNameCScrnWipe, sBaseTypesCScrnWipe };
const void* gTypeInfoCScrnRoboeyes[]   = { sTypeNameCScrnRoboeyes, sBaseTypesCScrnRoboeyes };
const void* gTypeInfoCScrnWipeCustom[] = { sTypeNameCScrnWipeCustom, sBaseTypesCScrnWipeCustom };
const void* gTypeInfoCScrnWipeTex[]    = { sTypeNameCScrnWipeTex, sBaseTypesCScrnWipeTex };
const void* gTypeInfoCScrnWipeTile[]   = { sWipeTypeNameCScrnWipeTile, sBaseTypesCScrnWipeTile };

const void* sBaseTypesCScrnWipe[]     = { gWipeTypeInfoCScrnColor, 0, 0 };
const void* sBaseTypesCScrnRoboeyes[] = { gWipeTypeInfoCScrnColor, 0, gTypeInfoCScrnWipe, 0, 0 };
