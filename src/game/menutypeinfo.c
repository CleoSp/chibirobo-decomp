// US and JP fill the last bytes of the short type names with ASCII zeroes.
#if defined(VERSION_GGTP01)
const char sStageTypeNameCVec[] = "CVec";
#else
const char sStageTypeNameCVec[8] = "CVec\0"
                                   "000";
#endif
const char sStageTypeNameVec[] = "Vec";
#if defined(VERSION_GGTP01)
const char sTypeNameMenuPC[] = "MenuPC";
#else
const char sTypeNameMenuPC[8] = "MenuPC\0"
                                "0";
#endif

extern const char sTypeNameCMenu[];
extern const char sTypeNameMenu[];
extern const void* sBaseTypesMenu[];
extern const char sTypeNameMenuMCTestSub[];
extern const void* sBaseTypesMenuMCTestSub[];
extern const void* sStageBaseTypesCVec[];
extern const char sStageTypeNameGXTexObj[];
extern const char sTypeNameMenuMCTest[];
extern const void* sBaseTypesMenuMCTest[];
extern const char sTypeNameMenuStageSelect[];
extern const char sTypeNameMenuSave[];
extern const void* sBaseTypesMenuSave[];
extern const void* sBaseTypesMenuPC[];
extern const char sTypeNameMenuStatus[];
extern const void* sBaseTypesMenuStatus[];
extern const char sTypeNameMenuItem[];
extern const void* sBaseTypesMenuItem[];
extern const char sTypeNameMenuMain[];
extern const void* sBaseTypesMenuMain[];
extern const char sTypeNameMenuRequestClear[];
extern const void* sBaseTypesMenuRequestClear[];
extern const char sTypeNameMenuSound[];
extern const void* sBaseTypesMenuSound[];
extern const char sTypeNameMenuPadControl[];
extern const void* sBaseTypesMenuPadControl[];
extern const char sTypeNameMenuRequest[];
extern const void* sBaseTypesMenuRequest[];
extern const char sTypeNameMenuSettings[];
extern const void* sBaseTypesMenuSettings[];

const void* __RTTI__5CMenu[]            = { sTypeNameCMenu, 0 };
const void* __RTTI__4Menu[]             = { sTypeNameMenu, sBaseTypesMenu };
const void* sBaseTypesMenuStageSelect[] = { __RTTI__5CMenu, 0, __RTTI__4Menu, 0, 0 };
#if defined(VERSION_GGTJ01_01)
const void* __RTTI__8MenuSave[] = { sTypeNameMenuSave, sBaseTypesMenuSave };
#else
const void* __RTTI__13MenuMCTestSub[] = { sTypeNameMenuMCTestSub, sBaseTypesMenuMCTestSub };
#endif
const void* __RTTI__3Vec[]           = { sStageTypeNameVec, 0 };
const void* gStageTypeInfoCVec[]     = { sStageTypeNameCVec, sStageBaseTypesCVec };
const void* gStageTypeInfoGXTexObj[] = { sStageTypeNameGXTexObj, 0 };
#if defined(VERSION_GGTJ01_01)
const void* __RTTI__16MenuRequestClear[] = { sTypeNameMenuRequestClear, sBaseTypesMenuRequestClear };
const void* __RTTI__6MenuPC[]            = { sTypeNameMenuPC, sBaseTypesMenuPC };
const void* __RTTI__9MenuSound[]         = { sTypeNameMenuSound, sBaseTypesMenuSound };
const void* __RTTI__14MenuPadControl[]   = { sTypeNameMenuPadControl, sBaseTypesMenuPadControl };
const void* __RTTI__10MenuStatus[]       = { sTypeNameMenuStatus, sBaseTypesMenuStatus };
const void* __RTTI__11MenuRequest[]      = { sTypeNameMenuRequest, sBaseTypesMenuRequest };
const void* __RTTI__15MenuStageSelect[]  = { sTypeNameMenuStageSelect, sBaseTypesMenuStageSelect };
const void* __RTTI__13MenuMCTestSub[]    = { sTypeNameMenuMCTestSub, sBaseTypesMenuMCTestSub };
#endif
const void* __RTTI__10MenuMCTest[] = { sTypeNameMenuMCTest, sBaseTypesMenuMCTest };
#if defined(VERSION_GGTJ01_01)
const void* __RTTI__12MenuSettings[] = { sTypeNameMenuSettings, sBaseTypesMenuSettings };
#else
const void* __RTTI__15MenuStageSelect[] = { sTypeNameMenuStageSelect, sBaseTypesMenuStageSelect };
const void* __RTTI__8MenuSave[]         = { sTypeNameMenuSave, sBaseTypesMenuSave };
const void* __RTTI__6MenuPC[]           = { sTypeNameMenuPC, sBaseTypesMenuPC };
const void* __RTTI__10MenuStatus[]      = { sTypeNameMenuStatus, sBaseTypesMenuStatus };
#endif
const void* __RTTI__8MenuItem[] = { sTypeNameMenuItem, sBaseTypesMenuItem };
const void* __RTTI__8MenuMain[] = { sTypeNameMenuMain, sBaseTypesMenuMain };
