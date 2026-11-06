#include "Version.h"
#include "game/xobj.h"

extern "C" {
extern const char sCbCannonLv2ResourcePath[];
extern const char sCbRadarResourcePath[];
extern const char sTitleIconIconBbResourcePath[];

extern const char sTestShortplayTitleResourcePath[]   = "test/shortplay_title";
extern const char sChargeFx01ResourcePath[]           = "charge_fx01";
extern const char sCbPropellerLv2ResourcePath[]       = "cb_propeller_lv2";
extern const char sMaruBatuResourcePath[]             = "maru_batu";
extern const char sExclamationResourcePath[]          = "exclamation";
extern const char sKisekaePartsKeathResourcePath[]    = "kisekae/parts_keath";
extern const char sKisekaePartsKaeruResourcePath[]    = "kisekae/parts_kaeru";
extern const char sKisekaePartsTaoResourcePath[]      = "kisekae/parts_tao";
extern const char sKisekaePartsKegaResourcePath[]     = "kisekae/parts_kega";
extern const char sKisekaePartsObakeResourcePath[]    = "kisekae/parts_obake";
extern const char sKisekaePartsPajamaResourcePath[]   = "kisekae/parts_pajama";
extern const char sKisekaePartsSuperResourcePath[]    = "kisekae/parts_super";
extern const char sKisekaePartsPajama01ResourcePath[] = "kisekae/parts_pajama_01";
extern const char sKisekaePartsPajama02ResourcePath[] = "kisekae/parts_pajama_02";
extern const char sTitleIconIconArmyResourcePath[]    = "title_icon/icon_army";
extern const char sTitleIconIconFafaResourcePath[]    = "title_icon/icon_fafa";
extern const char sTitleIconIconFunkyResourcePath[]   = "title_icon/icon_funky";
extern const char sTitleIconIconHockResourcePath[]    = "title_icon/icon_hock";
extern const char sTitleIconIconKeathResourcePath[]   = "title_icon/icon_keath";
extern const char sTitleIconIconPeetsResourcePath[]   = "title_icon/icon_peets";
extern const char sTitleIconIconSunmidaResourcePath[] = "title_icon/icon_sunmida";
extern const char sTitleIconIconTrexResourcePath[]    = "title_icon/icon_trex";
extern const char sTitleIconIconZobinResourcePath[]   = "title_icon/icon_zobin";
extern const char sTitleIconIconArmy2ResourcePath[]   = "title_icon/icon_army2";
#if !defined(VERSION_JP)
extern const char sUsMaruBatuResourcePath[]  = "us_maru_batu";
extern const char sUsMaruModelResourcePath[] = "us_maru_model";
extern const char sUsBatuModelResourcePath[] = "us_batu_model";
#endif
extern const char sTestWardrobeResourcePath[] = "test/wardrobe";

XObjSource gTestShortplayTitleSource   = { sTestShortplayTitleResourcePath };
XObjSource gChargeFx01Source           = { sChargeFx01ResourcePath };
XObjSource gCbPropellerLv2Source       = { sCbPropellerLv2ResourcePath };
XObjSource gCbCannonLv2Source          = { sCbCannonLv2ResourcePath };
XObjSource gCbRadarSource              = { sCbRadarResourcePath };
XObjSource gMaruBatuSource             = { sMaruBatuResourcePath };
XObjSource gExclamationSource          = { sExclamationResourcePath };
XObjSource gKisekaePartsKeathSource    = { sKisekaePartsKeathResourcePath };
XObjSource gKisekaePartsKaeruSource    = { sKisekaePartsKaeruResourcePath };
XObjSource gKisekaePartsTaoSource10021 = { sKisekaePartsTaoResourcePath };
XObjSource gKisekaePartsKegaSource     = { sKisekaePartsKegaResourcePath };
XObjSource gKisekaePartsTaoSource10023 = { sKisekaePartsTaoResourcePath };
XObjSource gKisekaePartsObakeSource    = { sKisekaePartsObakeResourcePath };
XObjSource gKisekaePartsPajamaSource   = { sKisekaePartsPajamaResourcePath };
XObjSource gKisekaePartsSuperSource    = { sKisekaePartsSuperResourcePath };
XObjSource gKisekaePartsPajama01Source = { sKisekaePartsPajama01ResourcePath };
XObjSource gKisekaePartsPajama02Source = { sKisekaePartsPajama02ResourcePath };
XObjSource gTitleIconIconBbSource      = { sTitleIconIconBbResourcePath };
XObjSource gTitleIconIconArmySource    = { sTitleIconIconArmyResourcePath };
XObjSource gTitleIconIconFafaSource    = { sTitleIconIconFafaResourcePath };
XObjSource gTitleIconIconFunkySource   = { sTitleIconIconFunkyResourcePath };
XObjSource gTitleIconIconHockSource    = { sTitleIconIconHockResourcePath };
XObjSource gTitleIconIconKeathSource   = { sTitleIconIconKeathResourcePath };
XObjSource gTitleIconIconPeetsSource   = { sTitleIconIconPeetsResourcePath };
XObjSource gTitleIconIconSunmidaSource = { sTitleIconIconSunmidaResourcePath };
XObjSource gTitleIconIconTrexSource    = { sTitleIconIconTrexResourcePath };
XObjSource gTitleIconIconZobinSource   = { sTitleIconIconZobinResourcePath };
XObjSource gTitleIconIconArmy2Source   = { sTitleIconIconArmy2ResourcePath };
#if !defined(VERSION_JP)
XObjSource gUsMaruBatuSource  = { sUsMaruBatuResourcePath };
XObjSource gUsMaruModelSource = { sUsMaruModelResourcePath };
XObjSource gUsBatuModelSource = { sUsBatuModelResourcePath };
#endif
XObjSource gTestWardrobeSource10029 = { sTestWardrobeResourcePath };
XObjSource gTestWardrobeSource10030 = { sTestWardrobeResourcePath };

#define EXTENDED_OBJECT(source, flags, category, item) { { (u32) & source }, 0, flags, 0, 0, 0, category, item }
UnkObjData objTableEx[] = {
	EXTENDED_OBJECT(gTitleIconIconBbSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconArmySource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconFafaSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconFunkySource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconHockSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconKeathSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconPeetsSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconSunmidaSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconTrexSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconZobinSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gTitleIconIconArmy2Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gTestShortplayTitleSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gObjectSources[15], 0x1, 22, 0),
	EXTENDED_OBJECT(gChargeFx01Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gCbPropellerLv2Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gCbCannonLv2Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gCbRadarSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gMaruBatuSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gExclamationSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsKeathSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsKaeruSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsTaoSource10021, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsKegaSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsTaoSource10023, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsObakeSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsPajamaSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsSuperSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsPajama01Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gKisekaePartsPajama02Source, 0x1, 22, 0),
	EXTENDED_OBJECT(gTestWardrobeSource10029, 0x401, 22, 0),
	EXTENDED_OBJECT(gTestWardrobeSource10030, 0x401, 22, 0),
	EXTENDED_OBJECT(gObjectSources[312], 0x1, 20, 1),
	EXTENDED_OBJECT(gObjectSources[313], 0x1, 20, 50),
#if !defined(VERSION_JP)
	EXTENDED_OBJECT(gUsMaruBatuSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gUsMaruModelSource, 0x1, 22, 0),
	EXTENDED_OBJECT(gUsBatuModelSource, 0x1, 22, 0),
#endif
};

s32 gGlobalTextIdRemap[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 19, 19, 312, 313,
#if !defined(VERSION_JP)
	-1, -1, -1,
#endif
};
}
