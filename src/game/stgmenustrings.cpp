#include "Version.h"

extern "C" {
#if defined(VERSION_JP)
extern const char sTypeNameMenuSave[12]                = "MenuSave";
extern const char sMenuSoundActivateMessage[24]        = "MenuSound::Activate\n";
extern const char sStageOrderedJapaneseLabelFormat[16] = "(%2d) %-.20s";
extern const char sStageListLabelFormat[16]            = "(%2d) %-.18s";
extern const char sStageFileLabelFormat[12]            = "%-8s(%2d)";
#else
extern const char sTypeNameMenuMCTestSub[16] = "MenuMCTestSub";
#endif
extern const char sCardTestSavingFormat[16]    = "(%d) saving...";
extern const char sCardTestLoadingFormat[]     = "(%d) loading...";
extern const char sCardTestDeletingFormat[20]  = "(%d) deleting...";
extern const char sCardTestBrokenFileFormat[]  = "(%d) broken [%d,%d]";
extern const char sCardTestFileSummaryFormat[] = "(%d) %02d/%02d/%02d %2d:%02d [%2d:%02d] (%3d)%s";
extern const char sCardTestEmptyFileFormat[12] = "(%d) none";
extern const char sCardTestFreeSpaceFormat[48] = "SLOT: %c   FREE BLOCKS: %d   FREE ENTRIES: %d";
extern const char sCardTestSlotFormat[12]      = "SLOT: %c";
extern const char sCardTestInitialFileLabel[56]
    = "(0) 00/00/00 00:00 [ 0:00] (000)\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40";

#if defined(VERSION_JP)
extern const char sSettingsSelectedPrefixFormat[16] = "\\C0000FF%02X";
extern const char sSettingsSelectedSuffixFormat[16] = "\\C202020%02X";
extern const char sSettingsNormalPrefixFormat[16]   = "\\C404080%02X";
extern const char sSettingsNormalSuffixFormat[16]   = "\\C646464%02X";
#else
extern const char sStageOrderedJapaneseLabelFormat[16] = "(%2d) %-.20s";
extern const char sStageOrderedLabelFormat[16]         = "(%2d) %-.30s";
extern const char sStageListLabelFormat[16]            = "(%2d) %-.18s";
extern const char sStageFileLabelFormat[12]            = "%-8s(%2d)";
#endif
extern const char sStageJointTranslationField[12]     = "translate";
extern const char sStageJointTranslationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";

extern const char sStageTypeNameGXTexObj[12]  = "_GXTexObj";
extern const char sStageTypeNameCoordInfo[12] = "CoordInfo";
#if defined(VERSION_JP)
extern const char sTypeNameMenuRequestClear[20] = "MenuRequestClear";
extern const char sTypeNameMenuSound[12]        = "MenuSound";
extern const char sTypeNameMenuPadControl[16]   = "MenuPadControl";
extern const char sTypeNameMenuStatus[12]       = "MenuStatus";
extern const char sTypeNameMenuRequest[12]      = "MenuRequest";
extern const char sTypeNameMenuStageSelect[16]  = "MenuStageSelect";
extern const char sTypeNameMenuMCTestSub[16]    = "MenuMCTestSub";
extern const char sTypeNameMenuMCTest[12]       = "MenuMCTest";
extern const char sTypeNameMenuSettings[16]     = "MenuSettings";
#else
extern const char sTypeNameMenuMCTest[12]      = "MenuMCTest";
extern const char sTypeNameMenuStageSelect[16] = "MenuStageSelect";
extern const char sTypeNameMenuSave[12]        = "MenuSave";
extern const char sTypeNameMenuStatus[12]      = "MenuStatus";
#endif
extern const char sTypeNameMenuItem[12]       = "MenuItem";
extern const char sTypeNameMenuMain[12]       = "MenuMain";
extern const char sTypeNameMenuCursor[12]     = "MenuCursor";
extern const char sStageTypeNameCXObjIntp[12] = "CXObjIntp";
extern const char sStageTypeNameCStatus64[12] = "CStatus64";
extern const char sStageTypeNameAttrInfo[12]  = "AttrInfo";
extern const char sStageTypeNameCEvtData[12]  = "CEvtData";
extern const char sStageTypeNameCAnmQueue[]   = "CAnmQueue";
}
