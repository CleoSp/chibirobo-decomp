#include "game/cmdline.h"

extern "C" {
extern const char sBootConfigPath[]              = "/qp.ini";
extern const char sBootConfigDelimiters[]        = " \t,=";
extern const char sBootConfigRestrictionMarker[] = "&";

extern const char sBootKeyStageNo[]             = "STAGE_NO";
extern const char sBootKeyDrawXZGrid[]          = "DRAW_XZGRID";
extern const char sBootKeyDrawWorldAxis[]       = "DRAW_WORLDAXIS";
extern const char sBootKeyDrawLightMark[]       = "DRAW_LIGHTMARK";
extern const char sBootKeyDrawSafetyFrame[]     = "DRAW_SAFETYFRAME";
extern const char sBootKeyDrawFrameMeter[]      = "DRAW_FRAMEMETER";
extern const char sBootKeyDrawHeapMeter[]       = "DRAW_HEAPMETER";
extern const char sBootKeyDrawCrosshair[]       = "DRAW_CROSSHAIR";
extern const char sBootKeyDrawDebugString[]     = "DRAW_DEBUGSTRING";
extern const char sBootKeyDrawCollision[]       = "DRAW_COLLISION";
extern const char sBootKeyDrawCameraCollision[] = "DRAW_CAMCOLLISION";
extern const char sBootKeyDrawObjectInfo[]      = "DRAW_OBJINFO";
extern const char sBootKeyDrawPlayTime[]        = "DRAW_PLAYTIME";
extern const char sBootKeyPrintFileInfo[]       = "PRINT_FILEINFO";
extern const char sBootKeyEnableLogo[]          = "ENABLE_LOGO";
extern const char sBootKeyEnableTitle[]         = "ENABLE_TITLE";
extern const char sBootKeyEnableGameOver[]      = "ENABLE_GAMEOVER";
extern const char sBootKeyEnableTimeUp[]        = "ENABLE_TIMEUP";
extern const char sBootKeyEnableDayNight[]      = "ENABLE_DAYNIGHT";
extern const char sBootKeyEnableDebugopr[]      = "ENABLE_DEBUGOPR";
extern const char sBootKeyEnableAtcCopter[]     = "ENABLE_ATC_COPTER";
extern const char sBootKeyEnableAtcPopper[]     = "ENABLE_ATC_POPPER";
extern const char sBootKeyEnableAtcRadar[]      = "ENABLE_ATC_RADAR";
extern const char sBootKeyEnableRelModule[]     = "ENABLE_RELMODULE";
extern const char sBootKeyEnableAramCache[]     = "ENABLE_ARAMCACHE";
extern const char sBootKeyEnablePrecache[]      = "ENABLE_PRECACHE";
extern const char sBootKeyEnableArchiver[]      = "ENABLE_ARCHIVER";
extern const char sBootKeyEnableScreenshot[]    = "ENABLE_SCREENSHOT";
extern const char sBootKeyAtcMenuType[]         = "ATC_MENU_TYPE";
extern const char sBootKeyLiftPlugType[]        = "LIFTPLUG_TYPE";
extern const char sBootKeyAtcActionType[]       = "ATC_ACTION_TYPE";
extern const char sBootKeyToolActionType[]      = "TOOL_ACTION_TYPE";
extern const char sBootKeyTitleCursor[]         = "TITLE_CURSOR";
extern const char sBootKeyEventLog[]            = "EVENT_LOG";

#pragma section RW ".data"
__declspec(section ".data") extern const BootConfigEntry gBootConfigEntries[] = {
	{ sBootKeyStageNo, BootStage, 0 },
	{ sBootKeyDrawXZGrid, BootFlag, 1ULL << 18 },
	{ sBootKeyDrawWorldAxis, BootFlag, 1ULL << 19 },
	{ sBootKeyDrawLightMark, BootFlag, 1ULL << 20 },
	{ sBootKeyDrawSafetyFrame, BootFlag, 1ULL << 21 },
	{ sBootKeyDrawFrameMeter, BootFlag, 1ULL << 22 },
	{ sBootKeyDrawHeapMeter, BootFlag, 1ULL << 23 },
	{ sBootKeyDrawCrosshair, BootCrosshair, 1ULL << 50 },
	{ sBootKeyDrawDebugString, BootFlag, 1ULL << 24 },
	{ sBootKeyDrawCollision, BootFlag, 1ULL << 25 },
	{ sBootKeyDrawCameraCollision, BootFlag, 1ULL << 53 },
	{ sBootKeyDrawObjectInfo, BootFlag, 1ULL << 26 },
	{ sBootKeyDrawPlayTime, BootFlag, 1ULL << 27 },
	{ sBootKeyPrintFileInfo, BootFlag, 1ULL << 28 },
	{ sBootKeyEnableLogo, BootFlag, 1ULL << 34 },
	{ sBootKeyEnableTitle, BootFlag, 1ULL << 29 },
	{ sBootKeyEnableGameOver, BootFlag, 1ULL << 30 },
	{ sBootKeyEnableTimeUp, BootFlag, 1ULL << 46 },
	{ sBootKeyEnableDayNight, BootFlag, 1ULL << 49 },
	{ sBootKeyEnableDebugopr, BootFlag, 1ULL << 31 },
	{ sBootKeyEnableAtcCopter, BootSetting, 1 },
	{ sBootKeyEnableAtcPopper, BootSetting, 2 },
	{ sBootKeyEnableAtcRadar, BootSetting, 3 },
	{ sBootKeyEnableRelModule, BootFlag, 1ULL << 32 },
	{ sBootKeyEnableAramCache, BootFlag, 1ULL << 41 },
	{ sBootKeyEnablePrecache, BootFlag, 1ULL << 42 },
	{ sBootKeyEnableArchiver, BootUnrestrictedFlag, 1ULL << 33 },
	{ sBootKeyEnableScreenshot, BootFlag, 1ULL << 52 },
	{ sBootKeyAtcMenuType, BootSetting, 0 },
	{ sBootKeyLiftPlugType, BootSetting, 4 },
	{ sBootKeyAtcActionType, BootSetting, 5 },
	{ sBootKeyToolActionType, BootSetting, 6 },
	{ sBootKeyTitleCursor, BootSetting, 7 },
	{ sBootKeyEventLog, BootEventLog, 0 },
	{ NULL, BootFlag, 0 },
};
} // extern "C"
