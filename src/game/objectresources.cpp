#include "game/xobj.h"

extern "C" {
extern const char sKitchenMapResourcePath[]               = "kitchen/map";
extern const char sKitchenReizoukoResourcePath[]          = "kitchen/reizouko";
extern const char sKitchenGasDaiResourcePath[]            = "kitchen/gas_dai";
extern const char sKitchenSinkUnder1ResourcePath[]        = "kitchen/sink_under1";
extern const char sKitchenSinkMidleResourcePath[]         = "kitchen/sink_midle";
extern const char sKitchenTaoEResourcePath[]              = "kitchen/tao_e";
extern const char sKitchenPlantBoxResourcePath[]          = "kitchen/plant_box";
extern const char sKitchenCounter1ResourcePath[]          = "kitchen/counter1";
extern const char sKitchenCounter2ResourcePath[]          = "kitchen/counter2";
extern const char sKitchenTableResourcePath[]             = "kitchen/table";
extern const char sKitchenShelfLResourcePath[]            = "kitchen/shelf_l";
extern const char sKitchenDoorResourcePath[]              = "kitchen/door";
extern const char sKitchenWallWalkResourcePath[]          = "kitchen/wall_walk";
extern const char sNpcConsentmanResourcePath[]            = "npc/consentman";
extern const char sNpcTonpyResourcePath[]                 = "npc/tonpy";
extern const char sKitchenWindowSResourcePath[]           = "kitchen/window_s";
extern const char sTestBox4ResourcePath[]                 = "test/box_4";
extern const char sTestBox8ResourcePath[]                 = "test/box_8";
extern const char sTestBox12ResourcePath[]                = "test/box_12";
extern const char sTestBoxLiftResourcePath[]              = "test/box_lift";
extern const char sTestRopeResourcePath[]                 = "test/rope";
extern const char sTestLadderResourcePath[]               = "test/ladder";
extern const char sRoukaMapResourcePath[]                 = "rouka/map";
extern const char sRoukaDoorJResourcePath[]               = "rouka/door_j";
extern const char sRoukaDoorKResourcePath[]               = "rouka/door_k";
extern const char sRoukaDoorPamaResourcePath[]            = "rouka/door_pama";
extern const char sRoukaEntranceResourcePath[]            = "rouka/entrance";
extern const char sRoukaKaidan1ResourcePath[]             = "rouka/kaidan1";
extern const char sRoukaKaidan2ResourcePath[]             = "rouka/kaidan2";
extern const char sRoukaKaidan3ResourcePath[]             = "rouka/kaidan3";
extern const char sRoukaMatResourcePath[]                 = "rouka/mat";
extern const char sRoukaRoCounterResourcePath[]           = "rouka/ro_counter";
extern const char sRoukaRoPlantResourcePath[]             = "rouka/ro_plant";
extern const char sRoukaRoSofaResourcePath[]              = "rouka/ro_sofa";
extern const char sRoukaTableKakuResourcePath[]           = "rouka/table_kaku";
extern const char sRoukaTableMaruResourcePath[]           = "rouka/table_maru";
extern const char sKitchenChair1ResourcePath[]            = "kitchen/chair1";
extern const char sRoukaTesuri2fResourcePath[]            = "rouka/tesuri_2f";
extern const char sRoukaWall12fResourcePath[]             = "rouka/wall_1_2f";
extern const char sRoukaWall1fResourcePath[]              = "rouka/wall_1f";
extern const char sRoukaWall2fResourcePath[]              = "rouka/wall_2f";
extern const char sPushobj3ResourcePath[]                 = "pushobj3";
extern const char sBackyardMapResourcePath[]              = "backyard/map";
extern const char sBackyardBaseResourcePath[]             = "backyard/base";
extern const char sBackyardBgResourcePath[]               = "backyard/bg";
extern const char sBackyardDoorResourcePath[]             = "backyard/door";
extern const char sBackyardFenceResourcePath[]            = "backyard/fence";
extern const char sBackyardWallResourcePath[]             = "backyard/wall";
extern const char sSoukoMapResourcePath[]                 = "souko/map";
extern const char sSoukoWallResourcePath[]                = "souko/wall";
extern const char sPushobj1ResourcePath[]                 = "pushobj1";
extern const char sPushobj2ResourcePath[]                 = "pushobj2";
extern const char sItemPenResourcePath[]                  = "item/pen";
extern const char sKitchenWallResourcePath[]              = "kitchen/wall";
extern const char sItemDekaDenchiResourcePath[]           = "item/deka_denchi";
extern const char sChibihousePcGamenResourcePath[]        = "chibihouse/pc_gamen";
extern const char sChibihouseGachaResourcePath[]          = "chibihouse/gacha";
extern const char sChibihouseAirResourcePath[]            = "chibihouse/air";
extern const char sJennyMapResourcePath[]                 = "jenny/map";
extern const char sJennyBedResourcePath[]                 = "jenny/bed";
extern const char sJennyChair1ResourcePath[]              = "jenny/chair1";
extern const char sJennyChair2ResourcePath[]              = "jenny/chair2";
extern const char sKitchenWindowResourcePath[]            = "kitchen/window";
extern const char sJennyClosetResourcePath[]              = "jenny/closet";
extern const char sJennyDeskResourcePath[]                = "jenny/desk";
extern const char sJennyDoorResourcePath[]                = "jenny/door";
extern const char sJennyDresserResourcePath[]             = "jenny/dresser";
extern const char sJennyLight1ResourcePath[]              = "jenny/light1";
extern const char sJennyMatResourcePath[]                 = "jenny/mat";
extern const char sJennyShelfResourcePath[]               = "jenny/shelf";
extern const char sJennyTableResourcePath[]               = "jenny/table";
extern const char sJennyWallResourcePath[]                = "jenny/wall";
extern const char sJennyWallShelfResourcePath[]           = "jenny/wall_shelf";
extern const char sJennyWindow1ResourcePath[]             = "jenny/window1";
extern const char sJennyWindow2ResourcePath[]             = "jenny/window2";
extern const char sParentMapResourcePath[]                = "parent/map";
extern const char sParentBarwallLResourcePath[]           = "parent/barwall_l";
extern const char sParentBarwallMResourcePath[]           = "parent/barwall_m";
extern const char sParentBarwallRResourcePath[]           = "parent/barwall_r";
extern const char sParentBedResourcePath[]                = "parent/bed";
extern const char sParentChairResourcePath[]              = "parent/chair";
extern const char sParentClosetResourcePath[]             = "parent/closet";
extern const char sParentCornershelfResourcePath[]        = "parent/cornershelf";
extern const char sParentDresserResourcePath[]            = "parent/dresser";
extern const char sParentSofaResourcePath[]               = "parent/sofa";
extern const char sParentTableResourcePath[]              = "parent/table";
extern const char sMapJumpBoxResourcePath[]               = "map_jump_box";
extern const char sParentWallResourcePath[]               = "parent/wall";
extern const char sParentWallshelfResourcePath[]          = "parent/wallshelf";
extern const char sParentWindowResourcePath[]             = "parent/window";
extern const char sNpcHockResourcePath[]                  = "npc/hock";
extern const char sSoukoKaidanResourcePath[]              = "souko/kaidan";
extern const char sSoukoDcDaiResourcePath[]               = "souko/dc_dai";
extern const char sLivingShelfDanBoxResourcePath[]        = "living/shelf_dan_box";
extern const char sLivingShelfDanDaiResourcePath[]        = "living/shelf_dan_dai";
extern const char sLivingDoorResourcePath[]               = "living/door";
extern const char sRoukaDoorLResourcePath[]               = "rouka/door_l";
extern const char sRoukaDoorLDamiResourcePath[]           = "rouka/door_l_dami";
extern const char sChibihouseCusCbPrLv1ResourcePath[]     = "chibihouse/cus_cb_pr_lv1";
extern const char sChibihouseGachaPon1ResourcePath[]      = "chibihouse/gacha_pon1";
extern const char sLivingWallShelf1ResourcePath[]         = "living/wall_shelf1";
extern const char sLivingTelResourcePath[]                = "living/tel";
extern const char sLivingTelCounterResourcePath[]         = "living/tel_counter";
extern const char sLivingTelJResourcePath[]               = "living/tel_j";
extern const char sLivingWallCounterResourcePath[]        = "living/wall_counter";
extern const char sLivingShelfWResourcePath[]             = "living/shelf_w";
extern const char sLivingWallUpResourcePath[]             = "living/wall_up";
extern const char sLivingShelfSlopeResourcePath[]         = "living/shelf_slope";
extern const char sLivingFlowerclockResourcePath[]        = "living/flowerclock";
extern const char sSoukoHasiraResourcePath[]              = "souko/hasira";
extern const char sLivingLivingSkyResourcePath[]          = "living/living_sky";
extern const char sLivingButterflyResourcePath[]          = "living/butterfly";
extern const char sLivingFlowerBedResourcePath[]          = "living/flower_bed";
extern const char sRoukaKaidan2hResourcePath[]            = "rouka/kaidan_2h";
extern const char sRoukaKaidan3hResourcePath[]            = "rouka/kaidan_3h";
extern const char sSoukoWineBottleResourcePath[]          = "souko/wine_bottle";
extern const char sRoukaHashiraTResourcePath[]            = "rouka/hashira_t";
extern const char sRoukaHashiraEResourcePath[]            = "rouka/hashira_e";
extern const char sRoukaWallUnderResourcePath[]           = "rouka/wall_under";
extern const char sRoukaFloor2fResourcePath[]             = "rouka/floor_2f";
extern const char sChibihouseMapResourcePath[]            = "chibihouse/map";
extern const char sChibihouseWallResourcePath[]           = "chibihouse/wall";
extern const char sChibihouseChiPcResourcePath[]          = "chibihouse/chi_pc";
extern const char sChibihouseChiSocketResourcePath[]      = "chibihouse/chi_socket";
extern const char sChibihouseChiEResourcePath[]           = "chibihouse/chi_e";
extern const char sRoukaDoorEResourcePath[]               = "rouka/door_e";
extern const char sRoukaWall2fUnderResourcePath[]         = "rouka/wall_2f_under";
extern const char sChibiHouseResourcePath[]               = "chibi_house";
extern const char sRoukaWindowResourcePath[]              = "rouka/window";
extern const char sItemFrogRingResourcePath[]             = "Item/frog_ring";
extern const char sChibiJumpResourcePath[]                = "chibi_jump";
extern const char sNpcJennyResourcePath[]                 = "npc/jenny";
extern const char sLivingMapResourcePath[]                = "living/map";
extern const char sLivingDanroResourcePath[]              = "living/danro";
extern const char sLivingShelfBResourcePath[]             = "living/shelf_b";
extern const char sLivingShelfDanResourcePath[]           = "living/shelf_dan";
extern const char sLivingSidetableResourcePath[]          = "living/sidetable";
extern const char sLivingSofa1ResourcePath[]              = "living/sofa1";
extern const char sLivingSofa2ResourcePath[]              = "living/sofa2";
extern const char sLivingStandlightResourcePath[]         = "living/standlight";
extern const char sLivingTableResourcePath[]              = "living/table";
extern const char sLivingTvResourcePath[]                 = "living/tv";
extern const char sLivingWallResourcePath[]               = "living/wall";
extern const char sLivingWindowResourcePath[]             = "living/window";
extern const char sHouseRChibiHMainResourcePath[]         = "house_r/chibi_h_main";
extern const char sHouseRChibiHDaiResourcePath[]          = "house_r/chibi_h_dai";
extern const char sNpcMamaResourcePath[]                  = "npc/mama";
extern const char sLivingSofaHashigoResourcePath[]        = "living/sofa_hashigo";
extern const char sLivingBooks1ResourcePath[]             = "living/books1";
extern const char sLivingBooks2ResourcePath[]             = "living/books2";
extern const char sLivingSofaCu1ResourcePath[]            = "living/sofa_cu1";
extern const char sLivingSofaCu2ResourcePath[]            = "living/sofa_cu2";
extern const char sLivingFlowerpot2ResourcePath[]         = "living/flowerpot2";
extern const char sLivingShelfDanBookResourcePath[]       = "living/shelf_dan_book";
extern const char sLivingTakoResourcePath[]               = "living/tako";
extern const char sLivingOekakiResourcePath[]             = "living/oekaki";
extern const char sHouseRChibiHHasigoResourcePath[]       = "house_r/chibi_h_hasigo";
extern const char sNpcArmy1ResourcePath[]                 = "npc/army1";
extern const char sNpcArmy2ResourcePath[]                 = "npc/army2";
extern const char sHouseRChibiHDoor1ResourcePath[]        = "house_r/chibi_h_door1";
extern const char sHouseRChibiHDoor2ResourcePath[]        = "house_r/chibi_h_door2";
extern const char sDoorroomResourcePath[]                 = "doorroom";
extern const char sDoorroomDokanResourcePath[]            = "doorroom_dokan";
extern const char sItemSnackBoneResourcePath[]            = "item/snack_bone";
extern const char sItemJunkUnkoResourcePath[]             = "item/junk_unko";
extern const char sItemJunkAResourcePath[]                = "item/junk_a";
extern const char sTestTestRingResourcePath[]             = "test/test_ring";
extern const char sLivingTableFootResourcePath[]          = "living/table_foot";
extern const char sLivingMatResourcePath[]                = "living/mat";
extern const char sLivingTubResourcePath[]                = "living/tub";
extern const char sPlugStartResourcePath[]                = "plug_start";
extern const char sNpcDecaRoboResourcePath[]              = "npc/deca_robo";
extern const char sNpcKeathResourcePath[]                 = "npc/keath";
extern const char sChibihouseDenti1ResourcePath[]         = "chibihouse/denti1";
extern const char sChibihouseDenti2ResourcePath[]         = "chibihouse/denti2";
extern const char sNpcPapaResourcePath[]                  = "npc/papa";
extern const char sHouseRChibiHHasiResourcePath[]         = "house_r/chibi_h_hasi";
extern const char sHouseRChibiHWarpResourcePath[]         = "house_r/chibi_h_warp";
extern const char sNpcFafaResourcePath[]                  = "npc/fafa";
extern const char sNpcSpider1ResourcePath[]               = "npc/spider1";
extern const char sItemBrushResourcePath[]                = "item/brush";
extern const char sLivingSakanaResourcePath[]             = "living/sakana";
extern const char sLivingKaigaJResourcePath[]             = "living/kaiga_j";
extern const char sItemCapsuleResourcePath[]              = "item/capsule";
extern const char sBackyardAriResourcePath[]              = "backyard/ari";
extern const char sLivingBgResourcePath[]                 = "living/bg";
extern const char sBackyardAriDaidaiResourcePath[]        = "backyard/ari_daidai";
extern const char sBackyardTreeBResourcePath[]            = "backyard/tree_b";
extern const char sBackyardKadanResourcePath[]            = "backyard/kadan";
extern const char sPlugDokodakadoorResourcePath[]         = "plug_dokodakadoor";
extern const char sLivingBbDanResourcePath[]              = "living/bb_dan";
extern const char sLivingTrashboxResourcePath[]           = "living/trashbox";
extern const char sLivingTaoDishResourcePath[]            = "living/tao_dish";
extern const char sSoukoSpiderSuBResourcePath[]           = "souko/spider_su_b";
extern const char sLivingSuisouR2ResourcePath[]           = "living/suisou_r2";
extern const char sLivingSakanaAwaResourcePath[]          = "living/sakana_awa";
extern const char sSoukoKumoHimoResourcePath[]            = "souko/kumo_himo";
extern const char sItemKamiKuzuResourcePath[]             = "item/kami_kuzu";
extern const char sItemCookieKakeraResourcePath[]         = "item/cookie_kakera";
extern const char sLivingTvKeathResourcePath[]            = "living/tv_keath";
extern const char sTestDust0ResourcePath[]                = "test/dust0";
extern const char sTestDust1ResourcePath[]                = "test/dust1";
extern const char sLivingSakanaEsaResourcePath[]          = "living/sakana_esa";
extern const char sLivingTanaAnaResourcePath[]            = "living/tana_ana";
extern const char sTestSpItoResourcePath[]                = "test/sp_ito";
extern const char sLivingWallCornerResourcePath[]         = "living/wall_corner";
extern const char sLivingCoUnderResourcePath[]            = "living/co_under";
extern const char sTestJennyHandResourcePath[]            = "test/jenny_hand";
extern const char sLivingWindowKazariResourcePath[]       = "living/window_kazari";
extern const char sItemJunkBResourcePath[]                = "item/junk_b";
extern const char sItemJunkCResourcePath[]                = "item/junk_c";
extern const char sLivingGabyouResourcePath[]             = "living/gabyou";
extern const char sBackyardSkyResourcePath[]              = "backyard/sky";
extern const char sTestFlyRoot1ResourcePath[]             = "test/fly_root1";
extern const char sNpcArmy3ResourcePath[]                 = "npc/army3";
extern const char sNpcNWing1ResourcePath[]                = "npc/n_wing1";
extern const char sNpcNWing2ResourcePath[]                = "npc/n_wing2";
extern const char sRoukaPlanterResourcePath[]             = "rouka/planter";
extern const char sItemSpoonResourcePath[]                = "item/spoon";
extern const char sTestNMatoResourcePath[]                = "test/n_mato";
extern const char sJennyCubeBoxResourcePath[]             = "jenny/cube_box";
extern const char sJennyCubeLResourcePath[]               = "jenny/cube_l";
extern const char sJennyCubeOResourcePath[]               = "jenny/cube_o";
extern const char sJennyCubeVResourcePath[]               = "jenny/cube_v";
extern const char sJennyCubeEResourcePath[]               = "jenny/cube_e";
extern const char sJennyCuAnimal1ResourcePath[]           = "jenny/cu_animal1";
extern const char sJennyCuAnimal2ResourcePath[]           = "jenny/cu_animal2";
extern const char sItemMagCupResourcePath[]               = "item/mag_cup";
extern const char sNpcSunMidaResourcePath[]               = "npc/sun_mida";
extern const char sNpcSunDaiResourcePath[]                = "npc/sun_dai";
extern const char sLivingCakeResourcePath[]               = "living/cake";
extern const char sItemBaraResourcePath[]                 = "item/bara";
extern const char sLivingOrangeGlassResourcePath[]        = "living/orange_glass";
extern const char sLivingOrangeBinResourcePath[]          = "living/orange_bin";
extern const char sLivingChamGlassResourcePath[]          = "living/cham_glass";
extern const char sLivingChamBinResourcePath[]            = "living/cham_bin";
extern const char sLivingVaseResourcePath[]               = "living/vase";
extern const char sLivingChickenResourcePath[]            = "living/chicken";
extern const char sLivingLivingCamResourcePath[]          = "living/living_cam";
extern const char sNpcSpider2ResourcePath[]               = "npc/spider2";
extern const char sLivingCakeDishResourcePath[]           = "living/cake_dish";
extern const char sLivingKazariTResourcePath[]            = "living/kazari_t";
extern const char sKitchenDoorRResourcePath[]             = "kitchen/door_r";
extern const char sLivingAsiatoResourcePath[]             = "living/asiato";
extern const char sLivingHatResourcePath[]                = "living/hat";
extern const char sLivingKazariWResourcePath[]            = "living/kazari_w";
extern const char sLivingBirthdayCResourcePath[]          = "living/birthday_c";
extern const char sLivingBoneRResourcePath[]              = "living/bone_r";
extern const char sKitchenDoorDamiResourcePath[]          = "kitchen/door_dami";
extern const char sItemCosObakeResourcePath[]             = "item/cos_obake";
extern const char sKitchenTutiHutaResourcePath[]          = "kitchen/tuti_huta";
extern const char sKitchenSinkHiResourcePath[]            = "kitchen/sink_hi";
extern const char sKitchenSinkHiUnderResourcePath[]       = "kitchen/sink_hi_under";
extern const char sKitchenTrashboxResourcePath[]          = "kitchen/trashbox";
extern const char sKitchenHammockResourcePath[]           = "kitchen/hammock";
extern const char sRoukaShoesResourcePath[]               = "rouka/shoes";
extern const char sKitchenSink2hiResourcePath[]           = "kitchen/sink_2hi";
extern const char sRoukaSRopeResourcePath[]               = "rouka/s_rope";
extern const char sLivingHappyBoxResourcePath[]           = "living/happy_box";
extern const char sLivingConsentmanAtariResourcePath[]    = "living/consentman_atari";
extern const char sItemTonpyItemAResourcePath[]           = "item/tonpy_item_a";
extern const char sItemTonpyItemBResourcePath[]           = "item/tonpy_item_b";
extern const char sItemTonpyItemCResourcePath[]           = "item/tonpy_item_c";
extern const char sItemHockTizuResourcePath[]             = "item/hock_tizu";
extern const char sHowToChibiroboCbBookResourcePath[]     = "how_to_chibirobo/cb_book";
extern const char sFxCbHouse01ResourcePath[]              = "fx_cb_house01";
extern const char sLivingCbCoverResourcePath[]            = "living/cb_cover";
extern const char sLivingHappyHeartResourcePath[]         = "living/happy_heart";
extern const char sAllYogoreCookOResourcePath[]           = "all/yogore_cook_o";
extern const char sAllYogoreWaterResourcePath[]           = "all/yogore_water";
extern const char sAllYogoreOilResourcePath[]             = "all/yogore_oil";
extern const char sLivingCamLivingResourcePath[]          = "living/cam_living";
extern const char sHouseRChibiHMakuResourcePath[]         = "house_r/chibi_h_maku";
extern const char sKitchenYakanResourcePath[]             = "kitchen/yakan";
extern const char sKitchenTeacupResourcePath[]            = "kitchen/teacup";
extern const char sKitchenTeapotResourcePath[]            = "kitchen/teapot";
extern const char sKitchenMilkBinResourcePath[]           = "kitchen/milk_bin";
extern const char sKitchenSugarBinResourcePath[]          = "kitchen/sugar_bin";
extern const char sKitchenCookieDishResourcePath[]        = "kitchen/cookie_dish";
extern const char sKitchenCbTeasetResourcePath[]          = "kitchen/cb_teaset";
extern const char sItemBlockSugarResourcePath[]           = "item/block_sugar";
extern const char sItemFlowerCookieResourcePath[]         = "item/flower_cookie";
extern const char sKitchenSinkSShelfResourcePath[]        = "kitchen/sink_s_shelf";
extern const char sAllSpiderKageResourcePath[]            = "all/spider_kage";
extern const char sKitchenSinkDishResourcePath[]          = "kitchen/sink_dish";
extern const char sKitchenHukinResourcePath[]             = "kitchen/hukin";
extern const char sKitchenSinkKagoResourcePath[]          = "kitchen/sink_kago";
extern const char sLivingTvSwitchResourcePath[]           = "living/tv_switch";
extern const char sKitchenTeacupSResourcePath[]           = "kitchen/teacup_s";
extern const char sLivingCrackerResourcePath[]            = "living/cracker";
extern const char sFxDecaSpark01ResourcePath[]            = "fx_deca_spark01";
extern const char sLivingTvNewsResourcePath[]             = "living/tv_news";
extern const char sLivingTvKageResourcePath[]             = "living/tv_kage";
extern const char sLivingCbBook2ResourcePath[]            = "living/cb_book2";
extern const char sFxTompCry01ResourcePath[]              = "fx_tomp_cry01";
extern const char sLivingSw1planterResourcePath[]         = "living/sw_1planter";
extern const char sLivingSw2planterResourcePath[]         = "living/sw_2planter";
extern const char sKitchenBbNikkiResourcePath[]           = "kitchen/bb_nikki";
extern const char sKitchenBbPenResourcePath[]             = "kitchen/bb_pen";
extern const char sLivingWallUnderResourcePath[]          = "living/wall_under";
extern const char sHowToChibiroboMapResourcePath[]        = "how_to_chibirobo/map";
extern const char sFxTompScare01ResourcePath[]            = "fx_tomp_scare01";
extern const char sLivingDoorKResourcePath[]              = "living/door_k";
extern const char sItemCrackerResourcePath[]              = "item/cracker";
extern const char sLivingCrackerGomiResourcePath[]        = "living/cracker_gomi";
extern const char sLivingFClockHbResourcePath[]           = "living/f_clock_hb";
extern const char sLivingFClockHsResourcePath[]           = "living/f_clock_hs";
extern const char sLivingFClockFResourcePath[]            = "living/f_clock_f";
extern const char sLivingPresentBoxResourcePath[]         = "living/present_box";
extern const char sLivingBgTreeResourcePath[]             = "living/bg_tree";
extern const char sFxSpider01ResourcePath[]               = "fx_spider01";
extern const char sLivingChickenBoneResourcePath[]        = "living/chicken_bone";
extern const char sLivingCakeAtoResourcePath[]            = "living/cake_ato";
extern const char sRoukaCupArmyResourcePath[]             = "rouka/cup_army";
extern const char sSoukoWineBottleAResourcePath[]         = "souko/wine_bottle_a";
extern const char sSoukoWineBottleBResourcePath[]         = "souko/wine_bottle_b";
extern const char sKitchenSink3hiResourcePath[]           = "kitchen/sink_3hi";
extern const char sJennyTana1ResourcePath[]               = "jenny/tana1";
extern const char sItemChipResourcePath[]                 = "item/chip";
extern const char sLivingWindowWareResourcePath[]         = "living/window_ware";
extern const char sLivingWindowHirakuResourcePath[]       = "living/window_hiraku";
extern const char sNpcArmy4ResourcePath[]                 = "npc/army4";
extern const char sNpcPrimoResourcePath[]                 = "npc/primo";
extern const char sParentPartitionResourcePath[]          = "parent/partition";
extern const char sParentPartition2ResourcePath[]         = "parent/partition2";
extern const char sParentPartition3ResourcePath[]         = "parent/partition3";
extern const char sParentPartition5ResourcePath[]         = "parent/partition5";
extern const char sParentWallshelf2ResourcePath[]         = "parent/wallshelf2";
extern const char sParentWallshelf3ResourcePath[]         = "parent/wallshelf3";
extern const char sParentWallshelf4ResourcePath[]         = "parent/wallshelf4";
extern const char sParentWallshelf5ResourcePath[]         = "parent/wallshelf5";
extern const char sParentWallshelf6ResourcePath[]         = "parent/wallshelf6";
extern const char sParentSwitchResourcePath[]             = "parent/switch";
extern const char sHouseRDream1ResourcePath[]             = "house_r/dream1";
extern const char sParentSidetableResourcePath[]          = "parent/sidetable";
extern const char sParentLadderResourcePath[]             = "parent/ladder";
extern const char sParentCloset2ResourcePath[]            = "parent/closet2";
extern const char sParentDoorResourcePath[]               = "parent/door";
extern const char sParentTable2ResourcePath[]             = "parent/table2";
extern const char sParentChestResourcePath[]              = "parent/chest";
extern const char sParentChest2ResourcePath[]             = "parent/chest2";
extern const char sParentBlindRResourcePath[]             = "parent/blind_r";
extern const char sParentBlindLResourcePath[]             = "parent/blind_l";
extern const char sParentBlindhandleRResourcePath[]       = "parent/blindhandle_r";
extern const char sParentBlindhandleLResourcePath[]       = "parent/blindhandle_l";
extern const char sParentSewingmachineResourcePath[]      = "parent/sewingmachine";
extern const char sJennyFlowerLampResourcePath[]          = "jenny/flower_lamp";
extern const char sItemReceiptResourcePath[]              = "item/receipt";
extern const char sParentMamaPenResourcePath[]            = "parent/mama_pen";
extern const char sParentMamaCalculatorResourcePath[]     = "parent/mama_calculator";
extern const char sParentBillsResourcePath[]              = "parent/bills";
extern const char sParentReceiptResourcePath[]            = "parent/receipt";
extern const char sParentAccountBookResourcePath[]        = "parent/account_book";
extern const char sUfoWallResourcePath[]                  = "ufo/wall";
extern const char sItemTyuusyakiResourcePath[]            = "item/tyuusyaki";
extern const char sKitchenHamburgResourcePath[]           = "kitchen/hamburg";
extern const char sJennyJennyTvResourcePath[]             = "jenny/jenny_tv";
extern const char sNpcFunkyFResourcePath[]                = "npc/funky_f";
extern const char sNpcTRexResourcePath[]                  = "npc/t_rex";
extern const char sAllUfoWarpResourcePath[]               = "all/ufo_warp";
extern const char sAllCllPapaResourcePath[]               = "all/cll_papa";
extern const char sAllCllMamaResourcePath[]               = "all/cll_mama";
extern const char sKitchenGasPlateResourcePath[]          = "kitchen/gas_plate";
extern const char sParentDoorbellResourcePath[]           = "parent/doorbell";
extern const char sParentDoorwindowResourcePath[]         = "parent/doorwindow";
extern const char sParentDoorwindow2ResourcePath[]        = "parent/doorwindow2";
extern const char sParentPlanterResourcePath[]            = "parent/planter";
extern const char sParentPlanter2ResourcePath[]           = "parent/planter2";
extern const char sParentPoleResourcePath[]               = "parent/pole";
extern const char sParentPole2ResourcePath[]              = "parent/pole2";
extern const char sParentPole3ResourcePath[]              = "parent/pole3";
extern const char sParentPole4ResourcePath[]              = "parent/pole4";
extern const char sParentPole5ResourcePath[]              = "parent/pole5";
extern const char sParentPole6ResourcePath[]              = "parent/pole6";
extern const char sParentPole7ResourcePath[]              = "parent/pole7";
extern const char sParentPole8ResourcePath[]              = "parent/pole8";
extern const char sParentRecplayerResourcePath[]          = "parent/recplayer";
extern const char sParentRecspeakerLResourcePath[]        = "parent/recspeaker_l";
extern const char sParentRecspeakerRResourcePath[]        = "parent/recspeaker_r";
extern const char sParentStpoleResourcePath[]             = "parent/stpole";
extern const char sParentStpole2ResourcePath[]            = "parent/stpole2";
extern const char sItemTonpyItemEResourcePath[]           = "item/tonpy_item_e";
extern const char sItemTonpyItemFResourcePath[]           = "item/tonpy_item_f";
extern const char sItemTonpyItemGResourcePath[]           = "item/tonpy_item_g";
extern const char sItemTonpyItemHResourcePath[]           = "item/tonpy_item_h";
extern const char sItemTonpyItemIResourcePath[]           = "item/tonpy_item_i";
extern const char sItemTonpyItemJResourcePath[]           = "item/tonpy_item_j";
extern const char sItemTonpyItemKResourcePath[]           = "item/tonpy_item_k";
extern const char sItemTonpyItemLResourcePath[]           = "item/tonpy_item_l";
extern const char sItemTonpyItemMResourcePath[]           = "item/tonpy_item_m";
extern const char sItemTonpyItemNResourcePath[]           = "item/tonpy_item_n";
extern const char sItemTonpyItemOResourcePath[]           = "item/tonpy_item_o";
extern const char sItemTonpyItemPResourcePath[]           = "item/tonpy_item_p";
extern const char sItemTonpyItemQResourcePath[]           = "item/tonpy_item_q";
extern const char sItemTonpyItemRResourcePath[]           = "item/tonpy_item_r";
extern const char sItemTonpyItemSResourcePath[]           = "item/tonpy_item_s";
extern const char sItemTonpyItemTResourcePath[]           = "item/tonpy_item_t";
extern const char sItemTonpyItemUResourcePath[]           = "item/tonpy_item_u";
extern const char sBackyardWindow1lResourcePath[]         = "backyard/window_1l";
extern const char sBackyardWindow2lResourcePath[]         = "backyard/window_2l";
extern const char sItemLetterResourcePath[]               = "item/letter";
extern const char sParentDoorswitchResourcePath[]         = "parent/doorswitch";
extern const char sJennyAsiba1ResourcePath[]              = "jenny/asiba1";
extern const char sJennyAsiba2ResourcePath[]              = "jenny/asiba2";
extern const char sJennyAsiba3ResourcePath[]              = "jenny/asiba3";
extern const char sJennyCastleDoorResourcePath[]          = "jenny/castle_door";
extern const char sJennyHeitai1ResourcePath[]             = "jenny/heitai1";
extern const char sJennyKaidanResourcePath[]              = "jenny/kaidan";
extern const char sJennyHikidasiCloseResourcePath[]       = "jenny/hikidasi_close";
extern const char sJennyHikidasiOpenResourcePath[]        = "jenny/hikidasi_open";
extern const char sRoukaDoorPamaHResourcePath[]           = "rouka/door_pama_h";
extern const char sRoukaRopeDoorResourcePath[]            = "rouka/rope_door";
extern const char sRoukaRopeNobuResourcePath[]            = "rouka/rope_nobu";
extern const char sJennyAsibaS1ResourcePath[]             = "jenny/asiba_s1";
extern const char sJennyCastleHasigoResourcePath[]        = "jenny/castle_hasigo";
extern const char sJennyHariLResourcePath[]               = "jenny/hari_l";
extern const char sJennyHariSResourcePath[]               = "jenny/hari_s";
extern const char sJennyHeitai2ResourcePath[]             = "jenny/heitai2";
extern const char sJennyHeitai3ResourcePath[]             = "jenny/heitai3";
extern const char sJennyHeitai4ResourcePath[]             = "jenny/heitai4";
extern const char sItemDenchi1ResourcePath[]              = "item/denchi1";
extern const char sItemDenchi2ResourcePath[]              = "item/denchi2";
extern const char sItemDenchi3ResourcePath[]              = "item/denchi3";
extern const char sItemPeetsKutuResourcePath[]            = "item/peets_kutu";
extern const char sItemHocyoukiResourcePath[]             = "item/hocyouki";
extern const char sJennyAsibaKado1ResourcePath[]          = "jenny/asiba_kado1";
extern const char sJennyAsiba4ResourcePath[]              = "jenny/asiba4";
extern const char sLivingMituFlowerResourcePath[]         = "living/mitu_flower";
extern const char sJennyHariL2ResourcePath[]              = "jenny/hari_l2";
extern const char sNpcPapaSleepResourcePath[]             = "npc/papa_sleep";
extern const char sNpcMamaSleepResourcePath[]             = "npc/mama_sleep";
extern const char sNpcJennySleepResourcePath[]            = "npc/jenny_sleep";
extern const char sAllSaSealResourcePath[]                = "all/sa_seal";
extern const char sParentSkirtingBoardResourcePath[]      = "parent/skirting_board";
extern const char sParentSkirtingBoard2ResourcePath[]     = "parent/skirting_board2";
extern const char sParentSkirtingBoardpResourcePath[]     = "parent/skirting_boardp";
extern const char sParentRecplayerCableResourcePath[]     = "parent/recplayer_cable";
extern const char sParentBed2ResourcePath[]               = "parent/bed2";
extern const char sRoukaTakiObjResourcePath[]             = "rouka/taki_obj";
extern const char sRoukaCeilingResourcePath[]             = "rouka/ceiling";
extern const char sJennyTvGamenResourcePath[]             = "jenny/tv_gamen";
extern const char sParentBedStayResourcePath[]            = "parent/bed_stay";
extern const char sAllYogoreTaiyaResourcePath[]           = "all/yogore_taiya";
extern const char sAllYogoreKureyonResourcePath[]         = "all/yogore_kureyon";
extern const char sAllYogoreToyResourcePath[]             = "all/yogore_toy";
extern const char sNpcFrogResourcePath[]                  = "npc/frog";
extern const char sParentCornershelfLadderResourcePath[]  = "parent/cornershelf_ladder";
extern const char sBackyardWindowWareResourcePath[]       = "backyard/window_ware";
extern const char sBackyardJagutiResourcePath[]           = "backyard/jaguti";
extern const char sJennyHasiResourcePath[]                = "jenny/hasi";
extern const char sKitchenHamPanUpResourcePath[]          = "kitchen/ham_pan_up";
extern const char sKitchenHamPanDownResourcePath[]        = "kitchen/ham_pan_down";
extern const char sJennyHeitai5ResourcePath[]             = "jenny/heitai5";
extern const char sFxLeticwarp01ResourcePath[]            = "fx_leticwarp01";
extern const char sBackyardHoseResourcePath[]             = "backyard/hose";
extern const char sRoukaNebukuroResourcePath[]            = "rouka/nebukuro";
extern const char sParentCamPapamamaResourcePath[]        = "parent/cam_papamama";
extern const char sJennyCamJennyResourcePath[]            = "jenny/cam_jenny";
extern const char sAllEnjoyLightResourcePath[]            = "all/enjoy_light";
extern const char sNpcZobinResourcePath[]                 = "npc/zobin";
extern const char sNpcPeetsResourcePath[]                 = "npc/peets";
extern const char sJennyHeitaiTaikoResourcePath[]         = "jenny/heitai_taiko";
extern const char sJennyHeitaiRappaResourcePath[]         = "jenny/heitai_rappa";
extern const char sJennyJennyHimoResourcePath[]           = "jenny/jenny_himo";
extern const char sJennyJennyBagResourcePath[]            = "jenny/jenny_bag";
extern const char sFxZactGiccoResourcePath[]              = "fx_zact_gicco";
extern const char sFxZactTaoResourcePath[]                = "fx_zact_tao";
extern const char sFxZactFrogResourcePath[]               = "fx_zact_frog";
extern const char sFxZactInjuredResourcePath[]            = "fx_zact_injured";
extern const char sFxZactPajamasResourcePath[]            = "fx_zact_pajamas";
extern const char sParentSaisunBoxResourcePath[]          = "parent/saisun_box";
extern const char sBackyardKumoUfoResourcePath[]          = "backyard/kumo_ufo";
extern const char sLivingHappyHeart2ResourcePath[]        = "living/happy_heart2";
extern const char sJennyBedTopResourcePath[]              = "jenny/bed_top";
extern const char sAll1dayTimerResourcePath[]             = "all/1day_timer";
extern const char sRoukaDoorStopResourcePath[]            = "rouka/door_stop";
extern const char sItemRexToothResourcePath[]             = "item/rex_tooth";
extern const char sJennyZobinBoxResourcePath[]            = "jenny/zobin_box";
extern const char sRoukaKaidanAtamaResourcePath[]         = "rouka/kaidan_atama";
extern const char sAllLeafResourcePath[]                  = "all/leaf";
extern const char sRoukaDoorKazariResourcePath[]          = "rouka/door_kazari";
extern const char sJennyHeitaiGuitarResourcePath[]        = "jenny/heitai_guitar";
extern const char sNpcZobinBm1ResourcePath[]              = "npc/zobin_bm1";
extern const char sItemTaneAResourcePath[]                = "item/tane_a";
extern const char sItemBbPenResourcePath[]                = "item/bb_pen";
extern const char sItemInkResourcePath[]                  = "item/ink";
extern const char sItemKareFlowerResourcePath[]           = "item/kare_flower";
extern const char sJennyPresentHanaResourcePath[]         = "jenny/present_hana";
extern const char sLivingSunmidaskyResourcePath[]         = "living/sunmidasky";
extern const char sItemKayakuResourcePath[]               = "item/kayaku";
extern const char sItemGoggleResourcePath[]               = "item/goggle";
extern const char sItemLoveLetterResourcePath[]           = "item/love_letter";
extern const char sItemTugResourcePath[]                  = "item/tug";
extern const char sItemTicketResourcePath[]               = "item/ticket";
extern const char sItemHoutaiResourcePath[]               = "item/houtai";
extern const char sItemBlockResourcePath[]                = "item/block";
extern const char sItemRecordGiccioResourcePath[]         = "item/record_giccio";
extern const char sItemRecordFfResourcePath[]             = "item/record_ff";
extern const char sLivingWallCHResourcePath[]             = "living/wall_c_h";
extern const char sItemNwingItemResourcePath[]            = "item/nwing_item";
extern const char sItemArmyPhotoResourcePath[]            = "item/army_photo";
extern const char sRoukaCounterHResourcePath[]            = "rouka/counter_h";
extern const char sTestFlyRoot2ResourcePath[]             = "test/fly_root2";
extern const char sNpcCar1ResourcePath[]                  = "npc/car1";
extern const char sNpcCar2ResourcePath[]                  = "npc/car2";
extern const char sNpcFunkyF2ResourcePath[]               = "npc/funky_f2";
extern const char sSoukoShelfResourcePath[]               = "souko/shelf";
extern const char sSoukoChestResourcePath[]               = "souko/chest";
extern const char sSoukoRengaResourcePath[]               = "souko/renga";
extern const char sKitchenKamareBbResourcePath[]          = "kitchen/kamare_bb";
extern const char sSoukoShelfDamiResourcePath[]           = "souko/shelf_dami";
extern const char sKitchenTutiAnaResourcePath[]           = "kitchen/tuti_ana";
extern const char sNpcHockShipResourcePath[]              = "npc/hock_ship";
extern const char sItemLeftFootResourcePath[]             = "item/left_foot";
extern const char sParentSuitCaseResourcePath[]           = "parent/suit_case";
extern const char sItemKairozuResourcePath[]              = "item/kairozu";
extern const char sItemDekaDenchiFullResourcePath[]       = "item/deka_denchi_full";
extern const char sJennyHagurumaResourcePath[]            = "jenny/haguruma";
extern const char sJennyPlanterZobinResourcePath[]        = "jenny/planter_zobin";
extern const char sJennyPlanterJenny1ResourcePath[]       = "jenny/planter_jenny1";
extern const char sJennyPlanterJenny2ResourcePath[]       = "jenny/planter_jenny2";
extern const char sUfoUfoBallResourcePath[]               = "ufo/ufo_ball";
extern const char sUfoTimeMcResourcePath[]                = "ufo/time_mc";
extern const char sItemPajamaKijiResourcePath[]           = "item/pajama_kiji";
extern const char sJennyPresentHanaZobinResourcePath[]    = "jenny/present_hana_zobin";
extern const char sAllFlower1ResourcePath[]               = "all/flower1";
extern const char sAllFlower2ResourcePath[]               = "all/flower2";
extern const char sAllFlower3ResourcePath[]               = "all/flower3";
extern const char sAllJunkTreeResourcePath[]              = "all/junk_tree";
extern const char sAllCoinTreeResourcePath[]              = "all/coin_tree";
extern const char sAllHappyTreeResourcePath[]             = "all/happy_tree";
extern const char sAllMameTreeResourcePath[]              = "all/mame_tree";
extern const char sJennySanpuHouseResourcePath[]          = "jenny/sanpu_house";
extern const char sItemKareFlowerMituResourcePath[]       = "item/kare_flower_mitu";
extern const char sItemSakiFlower1ResourcePath[]          = "item/saki_flower1";
extern const char sKitchenJaguchiHResourcePath[]          = "kitchen/jaguchi_h";
extern const char sKitchenTutiHutaDResourcePath[]         = "kitchen/tuti_huta_d";
extern const char sNpcTTrackResourcePath[]                = "npc/t_track";
extern const char sNpcArmy5ResourcePath[]                 = "npc/army5";
extern const char sUfoWall2ResourcePath[]                 = "ufo/wall2";
extern const char sKitchenWallTonneruResourcePath[]       = "kitchen/wall_tonneru";
extern const char sKitchenWallTWareResourcePath[]         = "kitchen/wall_t_ware";
extern const char sJennyToyDenwaResourcePath[]            = "jenny/toy_denwa";
extern const char sJennyToyPianoResourcePath[]            = "jenny/toy_piano";
extern const char sUfoExitResourcePath[]                  = "ufo/exit";
extern const char sLivingDensetuHanaResourcePath[]        = "living/densetu_hana";
extern const char sHaisuiMapResourcePath[]                = "haisui/map";
extern const char sHaisuiPipeAResourcePath[]              = "haisui/pipe_a";
extern const char sHaisuiHashigoBResourcePath[]           = "haisui/hashigo_b";
extern const char sHaisuiHashigoCResourcePath[]           = "haisui/hashigo_c";
extern const char sHaisuiHashigoDResourcePath[]           = "haisui/hashigo_d";
extern const char sUfoGroundResourcePath[]                = "ufo/ground";
extern const char sUfoGround2ResourcePath[]               = "ufo/ground2";
extern const char sUfoGround3ResourcePath[]               = "ufo/ground3";
extern const char sUfoBallRotResourcePath[]               = "ufo/ball_rot";
extern const char sUfoGame1ResourcePath[]                 = "ufo/game1";
extern const char sUfoGame2ResourcePath[]                 = "ufo/game2";
extern const char sUfoGame3ResourcePath[]                 = "ufo/game3";
extern const char sUfoGame4ResourcePath[]                 = "ufo/game4";
extern const char sUfoGameBotanResourcePath[]             = "ufo/game_botan";
extern const char sUfoGameP1ResourcePath[]                = "ufo/game_p1";
extern const char sUfoGameP2ResourcePath[]                = "ufo/game_p2";
extern const char sUfoGameP3ResourcePath[]                = "ufo/game_p3";
extern const char sUfoGameP4ResourcePath[]                = "ufo/game_p4";
extern const char sUfoWall3ResourcePath[]                 = "ufo/wall3";
extern const char sTestCarRoot1ResourcePath[]             = "test/car_root1";
extern const char sTestCarRoot2ResourcePath[]             = "test/car_root2";
extern const char sUfoGameBotan2ResourcePath[]            = "ufo/game_botan2";
extern const char sTestNMato2ResourcePath[]               = "test/n_mato2";
extern const char sKitchenSinkHashigoResourcePath[]       = "kitchen/sink_hashigo";
extern const char sRoukaTutaAResourcePath[]               = "rouka/tuta_a";
extern const char sRoukaTutaARaResourcePath[]             = "rouka/tuta_aRa";
extern const char sRoukaTutaARbResourcePath[]             = "rouka/tuta_aRb";
extern const char sJennyChibiIsuResourcePath[]            = "jenny/chibi_isu";
extern const char sRoukaTutaBResourcePath[]               = "rouka/tuta_b";
extern const char sRoukaTutaBRResourcePath[]              = "rouka/tuta_bR";
extern const char sRoukaTutaCResourcePath[]               = "rouka/tuta_c";
extern const char sRoukaTutaCRResourcePath[]              = "rouka/tuta_cR";
extern const char sRoukaTutaDResourcePath[]               = "rouka/tuta_d";
extern const char sRoukaTutaDRResourcePath[]              = "rouka/tuta_dR";
extern const char sKitchenConroResourcePath[]             = "kitchen/conro";
extern const char sKitchenKingyoResourcePath[]            = "kitchen/kingyo";
extern const char sKitchenWPlanterResourcePath[]          = "kitchen/w_planter";
extern const char sRoukaTakiHashiAResourcePath[]          = "rouka/taki_hashi_a";
extern const char sRoukaTakiHashiBResourcePath[]          = "rouka/taki_hashi_b";
extern const char sJennyKenban1ResourcePath[]             = "jenny/kenban1";
extern const char sJennyKenban2ResourcePath[]             = "jenny/kenban2";
extern const char sParentTpMike1ResourcePath[]            = "parent/tp_mike1";
extern const char sParentTpMike2ResourcePath[]            = "parent/tp_mike2";
extern const char sJennyHouseHimoResourcePath[]           = "jenny/house_himo";
extern const char sJennyBagHimoResourcePath[]             = "jenny/bag_himo";
extern const char sRoukaTakiKakoiResourcePath[]           = "rouka/taki_kakoi";
extern const char sRoukaArmyIta1ResourcePath[]            = "rouka/army_ita1";
extern const char sRoukaArmyIta2ResourcePath[]            = "rouka/army_ita2";
extern const char sRoukaArmyHeri1ResourcePath[]           = "rouka/army_heri1";
extern const char sUfoUfoBedResourcePath[]                = "ufo/ufo_bed";
extern const char sJennyJennyDressResourcePath[]          = "jenny/jenny_dress";
extern const char sFxKitchenburnerResourcePath[]          = "fx_kitchenburner";
extern const char sFxWarpzoneResourcePath[]               = "fx_warpzone";
extern const char sUfoUfoBed2ResourcePath[]               = "ufo/ufo_bed2";
extern const char sUfoUfoBed3ResourcePath[]               = "ufo/ufo_bed3";
extern const char sUfoUfoBed4ResourcePath[]               = "ufo/ufo_bed4";
extern const char sNpcNasubiResourcePath[]                = "npc/nasubi";
extern const char sAllTonpyItemKageResourcePath[]         = "all/tonpy_item_kage";
extern const char sRoukaRaceItaResourcePath[]             = "rouka/race_ita";
extern const char sHaisuiSotoResourcePath[]               = "haisui/soto";
extern const char sHaisuiNezumiResourcePath[]             = "haisui/nezumi";
extern const char sHaisuiHashigoFResourcePath[]           = "haisui/hashigo_f";
extern const char sHaisuiRopeResourcePath[]               = "haisui/rope";
extern const char sHaisuiPipeBResourcePath[]              = "haisui/pipe_b";
extern const char sJennyJennyDress2ResourcePath[]         = "jenny/jenny_dress2";
extern const char sJennyJennyDress3ResourcePath[]         = "jenny/jenny_dress3";
extern const char sHaisuiKabeResourcePath[]               = "haisui/kabe";
extern const char sSoukoBirdcageResourcePath[]            = "souko/birdcage";
extern const char sSoukoBoardResourcePath[]               = "souko/board";
extern const char sSoukoNailResourcePath[]                = "souko/nail";
extern const char sSoukoVRopeResourcePath[]               = "souko/v_rope";
extern const char sHaisuiWaterResourcePath[]              = "haisui/water";
extern const char sItemPajamaKiji2ResourcePath[]          = "item/pajama_kiji2";
extern const char sItemPajamaKiji3ResourcePath[]          = "item/pajama_kiji3";
extern const char sParentTKazan1ResourcePath[]            = "parent/t_kazan1";
extern const char sParentTKazan2ResourcePath[]            = "parent/t_kazan2";
extern const char sParentTKazan3ResourcePath[]            = "parent/t_kazan3";
extern const char sParentTKazan4ResourcePath[]            = "parent/t_kazan4";
extern const char sParentTKazan5ResourcePath[]            = "parent/t_kazan5";
extern const char sParentTKazan6ResourcePath[]            = "parent/t_kazan6";
extern const char sItemTBlock1ResourcePath[]              = "item/t_block1";
extern const char sItemTBlock2ResourcePath[]              = "item/t_block2";
extern const char sItemTBlock3ResourcePath[]              = "item/t_block3";
extern const char sItemTBlock4ResourcePath[]              = "item/t_block4";
extern const char sItemTBlock5ResourcePath[]              = "item/t_block5";
extern const char sItemTBlock6ResourcePath[]              = "item/t_block6";
extern const char sItemCarItemResourcePath[]              = "item/car_item";
extern const char sBackyardTHashigoResourcePath[]         = "backyard/t_hashigo";
extern const char sBackyardBrankoResourcePath[]           = "backyard/branko";
extern const char sBackyardBRopeResourcePath[]            = "backyard/b_rope";
extern const char sItemPapaYubiwaResourcePath[]           = "item/papa_yubiwa";
extern const char sParentTKageResourcePath[]              = "parent/t_kage";
extern const char sFxZactGhostResourcePath[]              = "fx_zact_ghost";
extern const char sParentFrogHagataResourcePath[]         = "parent/frog_hagata";
extern const char sItemKusaResourcePath[]                 = "item/kusa";
extern const char sRoukaBebeBinsenResourcePath[]          = "rouka/bebe_binsen";
extern const char sItemSekkeizuResourcePath[]             = "item/sekkeizu";
extern const char sItemItemFrogResourcePath[]             = "item/item_frog";
extern const char sFxPcBuy01ResourcePath[]                = "fx_pc_buy01";
extern const char sJennyJennyBookResourcePath[]           = "jenny/jenny_book";
extern const char sJennyPlanterHachiResourcePath[]        = "jenny/planter_hachi";
extern const char sJennyKureyonResourcePath[]             = "jenny/kureyon";
extern const char sFxCbdoor01ResourcePath[]               = "fx_cbdoor01";
extern const char sFxTmBarrierResourcePath[]              = "fx_tm_barrier";
extern const char sJennySuberidai1ResourcePath[]          = "jenny/suberidai1";
extern const char sJennySuberidai2ResourcePath[]          = "jenny/suberidai2";
extern const char sLivingAlbumTResourcePath[]             = "living/album_t";
extern const char sLivingAlbumYResourcePath[]             = "living/album_y";
extern const char sLivingTableStandResourcePath[]         = "living/table_stand";
extern const char sLivingKaizouPoperResourcePath[]        = "living/kaizou_poper";
extern const char sParentWeldingMaskpResourcePath[]       = "parent/welding_maskp";
extern const char sParentWeldingMaskmResourcePath[]       = "parent/welding_maskm";
extern const char sParentWeldingMaskjResourcePath[]       = "parent/welding_maskj";
extern const char sParentWeldingMasktResourcePath[]       = "parent/welding_maskt";
extern const char sParentWeldingTorchResourcePath[]       = "parent/welding_torch";
extern const char sParentWeldingToolboxResourcePath[]     = "parent/welding_toolbox";
extern const char sParentWeldingToolsResourcePath[]       = "parent/welding_tools";
extern const char sParentDrafterResourcePath[]            = "parent/drafter";
extern const char sKitchenGasHashigoResourcePath[]        = "kitchen/gas_hashigo";
extern const char sKitchenGasRopeResourcePath[]           = "kitchen/gas_rope";
extern const char sParentCaseJanksResourcePath[]          = "parent/case_janks";
extern const char sYaneTorinosuResourcePath[]             = "yane/torinosu";
extern const char sBackyardKusaTutiResourcePath[]         = "backyard/kusa_tuti";
extern const char sLivingKeathOmcResourcePath[]           = "living/keath_omc";
extern const char sTitleIconIconBbResourcePath[]          = "title_icon/icon_bb";
extern const char sRoukaTaoBagResourcePath[]              = "rouka/tao_bag";
extern const char sRoukaKokubanResourcePath[]             = "rouka/kokuban";
extern const char sParentDcfootSpiderResourcePath[]       = "parent/dcfoot_spider";
extern const char sAllTakarabakoResourcePath[]            = "all/takarabako";
extern const char sCbCannonLv2ResourcePath[]              = "cb_cannon_lv2";
extern const char sCbRadarResourcePath[]                  = "cb_radar";
extern const char sLivingCbKaiPoperResourcePath[]         = "living/cb_kai_poper";
extern const char sHaisuiRoom1CResourcePath[]             = "haisui/room1_c";
extern const char sFxDecaEnergyResourcePath[]             = "fx_deca_energy";
extern const char sFxDecaBatteryResourcePath[]            = "fx_deca_battery";
extern const char sFx3pu01ResourcePath[]                  = "fx_3pu01";
extern const char sFxPapa01ResourcePath[]                 = "fx_papa01";
extern const char sKitchenKankisenResourcePath[]          = "kitchen/kankisen";
extern const char sItemTamagottiResourcePath[]            = "item/tamagotti";
extern const char sNpcDecaRobo2ResourcePath[]             = "npc/deca_robo2";
extern const char sKitchenToasterResourcePath[]           = "kitchen/toaster";
extern const char sKitchenSenzaiResourcePath[]            = "kitchen/senzai";
extern const char sKitchenSuponjiResourcePath[]           = "kitchen/suponji";
extern const char sKitchenJamResourcePath[]               = "kitchen/jam";
extern const char sKitchenGusUpResourcePath[]             = "kitchen/gus_up";
extern const char sAllYogoreHSResourcePath[]              = "all/yogore_h_s";
extern const char sAllYogoreHKResourcePath[]              = "all/yogore_h_k";
extern const char sNpcSpider3ResourcePath[]               = "npc/spider3";
extern const char sNpcSunKaoResourcePath[]                = "npc/sun_kao";
extern const char sKitchenPuroperaResourcePath[]          = "kitchen/puropera";
extern const char sJennyCastleHasigo2ResourcePath[]       = "jenny/castle_hasigo2";
extern const char sAllYogoreHitoResourcePath[]            = "all/yogore_hito";
extern const char sAllYogoreHunResourcePath[]             = "all/yogore_hun";
extern const char sAllYogoreShimiResourcePath[]           = "all/yogore_shimi";
extern const char sFxArmySgtResourcePath[]                = "fx_army_sgt";
extern const char sJennyJennyKagamiResourcePath[]         = "jenny/jenny_kagami";
extern const char sLivingTamagottiGamenResourcePath[]     = "living/tamagotti_gamen";
extern const char sKitchenKankisenAsibaResourcePath[]     = "kitchen/kankisen_asiba";
extern const char sLivingLightRopeResourcePath[]          = "living/light_rope";
extern const char sLivingAudioResourcePath[]              = "living/audio";
extern const char sLivingSibariMamaResourcePath[]         = "living/sibari_mama";
extern const char sLivingSibariPapaResourcePath[]         = "living/sibari_papa";
extern const char sLivingSibariTaoResourcePath[]          = "living/sibari_tao";
extern const char sLivingSibariJennyResourcePath[]        = "living/sibari_jenny";
extern const char sItemAkikanResourcePath[]               = "item/akikan";
extern const char sItemCandyGomiResourcePath[]            = "item/candy_gomi";
extern const char sItemOkasiGomi1ResourcePath[]           = "item/okasi_gomi1";
extern const char sItemOkasiGomi2ResourcePath[]           = "item/okasi_gomi2";
extern const char sItemItemNasubiResourcePath[]           = "item/item_nasubi";
extern const char sJennyWallHabakiResourcePath[]          = "jenny/wall_habaki";
extern const char sJennyWallTopResourcePath[]             = "jenny/wall_top";
extern const char sJennyWallTileResourcePath[]            = "jenny/wall_tile";
extern const char sParentPapaToyboxResourcePath[]         = "parent/papa_toybox";
extern const char sParentPapaToybox2ResourcePath[]        = "parent/papa_toybox2";
extern const char sParentRecordsResourcePath[]            = "parent/records";
extern const char sFxGiccoTwinb01ResourcePath[]           = "fx_gicco_twinb01";
extern const char sFxGiccoTwinb02ResourcePath[]           = "fx_gicco_twinb02";
extern const char sFxGiccoTwinb03ResourcePath[]           = "fx_gicco_twinb03";
extern const char sItemDensetuTaneResourcePath[]          = "item/densetu_tane";
extern const char sFxBackyardRainResourcePath[]           = "fx_backyard_rain";
extern const char sRoukaCoUnderResourcePath[]             = "rouka/co_under";
extern const char sRoukaTakiSResourcePath[]               = "rouka/taki_s";
extern const char sRoukaTakiSCResourcePath[]              = "rouka/taki_s_c";
extern const char sParentRecplayerNeedleResourcePath[]    = "parent/recplayer_needle";
extern const char sFxAlien01ResourcePath[]                = "fx_alien01";
extern const char sBackyardAriSuResourcePath[]            = "backyard/ari_su";
extern const char sKitchenCoUnderResourcePath[]           = "kitchen/co_under";
extern const char sNpcBirdResourcePath[]                  = "npc/bird";
extern const char sNpcTaoSResourcePath[]                  = "npc/tao_s";
extern const char sParentTBlocksResourcePath[]            = "parent/t_blocks";
extern const char sKitchenMituBinResourcePath[]           = "kitchen/mitu_bin";
extern const char sJennySanpuBinResourcePath[]            = "jenny/sanpu_bin";
extern const char sTestSpiderJanks1ResourcePath[]         = "test/spider_janks1";
extern const char sChibihouseRankingNoResourcePath[]      = "chibihouse/ranking_no";
extern const char sChibihouseRankingBoardResourcePath[]   = "chibihouse/ranking_board";
extern const char sSoukoSunRagResourcePath[]              = "souko/sun_rag";
extern const char sSoukoSunRouResourcePath[]              = "souko/sun_rou";
extern const char sLivingTvNews2ResourcePath[]            = "living/tv_news2";
extern const char sBackyardKumoAmeResourcePath[]          = "backyard/kumo_ame";
extern const char sAllYogore30kResourcePath[]             = "all/yogore_30k";
extern const char sAllYogore50kResourcePath[]             = "all/yogore_50k";
extern const char sHaisuiAnaAResourcePath[]               = "haisui/ana_a";
extern const char sKitchenBgResourcePath[]                = "kitchen/bg";
extern const char sKitchenFruitResourcePath[]             = "kitchen/fruit";
extern const char sKitchenHaeResourcePath[]               = "kitchen/hae";
extern const char sFxMspiderEyeResourcePath[]             = "fx_mspider_eye";
extern const char sTestMtJank1ResourcePath[]              = "test/mt_jank1";
extern const char sTestMtJank2ResourcePath[]              = "test/mt_jank2";
extern const char sTestMtJank3ResourcePath[]              = "test/mt_jank3";
extern const char sTestMtJank4ResourcePath[]              = "test/mt_jank4";
extern const char sTestMtJank5ResourcePath[]              = "test/mt_jank5";
extern const char sTestMtJank6ResourcePath[]              = "test/mt_jank6";
extern const char sTestMtJank7ResourcePath[]              = "test/mt_jank7";
extern const char sTestMtJank8ResourcePath[]              = "test/mt_jank8";
extern const char sTestMtJank9ResourcePath[]              = "test/mt_jank9";
extern const char sItemKure1ResourcePath[]                = "item/kure1";
extern const char sItemKure2ResourcePath[]                = "item/kure2";
extern const char sItemKure3ResourcePath[]                = "item/kure3";
extern const char sItemKure4ResourcePath[]                = "item/kure4";
extern const char sItemKure5ResourcePath[]                = "item/kure5";
extern const char sParentGboxBigResourcePath[]            = "parent/gbox_big";
extern const char sParentGboxSmallResourcePath[]          = "parent/gbox_small";
extern const char sLivingFafaOmcResourcePath[]            = "living/fafa_omc";
extern const char sTestSp3BomResourcePath[]               = "test/sp3_bom";
extern const char sBackyardWindowKResourcePath[]          = "backyard/window_k";
extern const char sLivingTvSunaResourcePath[]             = "living/tv_suna";
extern const char sBackyardJimenRyuukiResourcePath[]      = "backyard/jimen_ryuuki";
extern const char sBackyardFuneTutiDResourcePath[]        = "backyard/fune_tuti_d";
extern const char sChibihouseElect10ResourcePath[]        = "chibihouse/elect10";
extern const char sChibihouseElect2ResourcePath[]         = "chibihouse/elect2";
extern const char sChibihouseElect3ResourcePath[]         = "chibihouse/elect3";
extern const char sChibihouseElect4ResourcePath[]         = "chibihouse/elect4";
extern const char sChibihouseElect5ResourcePath[]         = "chibihouse/elect5";
extern const char sChibihouseElect6ResourcePath[]         = "chibihouse/elect6";
extern const char sChibihouseElect7ResourcePath[]         = "chibihouse/elect7";
extern const char sChibihouseElect8ResourcePath[]         = "chibihouse/elect8";
extern const char sChibihouseElect9ResourcePath[]         = "chibihouse/elect9";
extern const char sLivingHodokeItoResourcePath[]          = "living/hodoke_ito";
extern const char sChibihouseDenti2conResourcePath[]      = "chibihouse/denti2con";
extern const char sAllYogoreKureyonKResourcePath[]        = "all/yogore_kureyon_k";
extern const char sBackyardGrassResourcePath[]            = "backyard/grass";
extern const char sLivingMotherWallResourcePath[]         = "living/mother_wall";
extern const char sLivingMotherMapResourcePath[]          = "living/mother_map";
extern const char sLivingProtectorResourcePath[]          = "living/protector";
extern const char sLivingApronResourcePath[]              = "living/apron";
extern const char sLivingCurtainResourcePath[]            = "living/curtain";
extern const char sParentTBlocks2ResourcePath[]           = "parent/t_blocks2";
extern const char sRoukaNwingDaiResourcePath[]            = "rouka/nwing_dai";
extern const char sKitchenShelfLHaResourcePath[]          = "kitchen/shelf_l_ha";
extern const char sKitchenShelfLHbResourcePath[]          = "kitchen/shelf_l_hb";
extern const char sKitchenClockLResourcePath[]            = "kitchen/clock_l";
extern const char sKitchenClockSResourcePath[]            = "kitchen/clock_s";
extern const char sKitchenShelfLRResourcePath[]           = "kitchen/shelf_l_r";
extern const char sKitchenHamburgKResourcePath[]          = "kitchen/hamburg_k";
extern const char sJennyMiniBoxResourcePath[]             = "jenny/mini_box";
extern const char sJennyMiniHikidasiResourcePath[]        = "jenny/mini_hikidasi";
extern const char sJennyMapKure1ResourcePath[]            = "jenny/map_kure1";
extern const char sJennyMapKure2ResourcePath[]            = "jenny/map_kure2";
extern const char sJennyMapKure3ResourcePath[]            = "jenny/map_kure3";
extern const char sJennyMapKure4ResourcePath[]            = "jenny/map_kure4";
extern const char sJennyMapKure5ResourcePath[]            = "jenny/map_kure5";
extern const char sFxTompCam2dResourcePath[]              = "fx_tomp_cam2d";
extern const char sFxMummiformResourcePath[]              = "fx_mummiform";
extern const char sFxPirateboatResourcePath[]             = "fx_pirateboat";
extern const char sFxMspider01ResourcePath[]              = "fx_mspider_01";
extern const char sFxMspider02ResourcePath[]              = "fx_mspider_02";
extern const char sFxTmUniteResourcePath[]                = "fx_tm_unite";
extern const char sJennyJClockLResourcePath[]             = "jenny/j_clock_l";
extern const char sJennyJClockSResourcePath[]             = "jenny/j_clock_s";
extern const char sSoukoSClockLResourcePath[]             = "souko/s_clock_l";
extern const char sSoukoSClockSResourcePath[]             = "souko/s_clock_s";
extern const char sRoukaRClockLResourcePath[]             = "rouka/r_clock_l";
extern const char sRoukaRClockSResourcePath[]             = "rouka/r_clock_s";
extern const char sJennyCartainResourcePath[]             = "jenny/cartain";
extern const char sJennyTvSampooResourcePath[]            = "jenny/tv_sampoo";
extern const char sJennyBagHimo2ResourcePath[]            = "jenny/bag_himo2";
extern const char sJennyBagHimo3ResourcePath[]            = "jenny/bag_himo3";
extern const char sJennyBagHimo4ResourcePath[]            = "jenny/bag_himo4";
extern const char sJennyHeitaiTaoreResourcePath[]         = "jenny/heitai_taore";
extern const char sJennyShelfBooksResourcePath[]          = "jenny/shelf_books";
extern const char sJennyMoveBook1ResourcePath[]           = "jenny/move_book1";
extern const char sJennyMoveBook2ResourcePath[]           = "jenny/move_book2";
extern const char sJennySanpuBin2ResourcePath[]           = "jenny/sanpu_bin2";
extern const char sSoukoBirdcageShortcutResourcePath[]    = "souko/birdcage_shortcut";
extern const char sSoukoBirdcageShortcut2ResourcePath[]   = "souko/birdcage_shortcut2";
extern const char sItemMiniFunky1ResourcePath[]           = "item/mini_funky1";
extern const char sNpcSpider4ResourcePath[]               = "npc/spider4";
extern const char sNpcZobinBm2ResourcePath[]              = "npc/zobin_bm2";
extern const char sNpcZobinBm3ResourcePath[]              = "npc/zobin_bm3";
extern const char sParentFunkyStageResourcePath[]         = "parent/funky_stage";
extern const char sItemMiniFunkyButiResourcePath[]        = "item/mini_funky_buti";
extern const char sItemTaneFunkyResourcePath[]            = "item/tane_funky";
extern const char sNpcFunkyF3ResourcePath[]               = "npc/funky_f3";
extern const char sKitchenShelfLRbResourcePath[]          = "kitchen/shelf_l_rb";
extern const char sRoukaSpiderBResourcePath[]             = "rouka/spider_b";
extern const char sNpcTyouBbResourcePath[]                = "npc/tyou_bb";
extern const char sRoukaWHashigoResourcePath[]            = "rouka/w_hashigo";
extern const char sRoukaWRopeResourcePath[]               = "rouka/w_rope";
extern const char sRoukaTeRopeAResourcePath[]             = "rouka/te_rope_a";
extern const char sRoukaTeRopeBResourcePath[]             = "rouka/te_rope_b";
extern const char sRoukaBgResourcePath[]                  = "rouka/bg";
extern const char sRoukaLightResourcePath[]               = "rouka/light";
extern const char sSoukoCoUnderResourcePath[]             = "souko/co_under";
extern const char sSoukoSUnderObjResourcePath[]           = "souko/s_under_obj";
extern const char sParentClockLResourcePath[]             = "parent/clock_l";
extern const char sParentClockSResourcePath[]             = "parent/clock_s";
extern const char sParentClockFResourcePath[]             = "parent/clock_f";
extern const char sParentClockCuckooResourcePath[]        = "parent/clock_cuckoo";
extern const char sParentBg01ResourcePath[]               = "parent/bg01";
extern const char sFxSpider02ResourcePath[]               = "fx_spider02";
extern const char sSoukoObjAResourcePath[]                = "souko/obj_a";
extern const char sSoukoObjBResourcePath[]                = "souko/obj_b";
extern const char sSoukoObjCResourcePath[]                = "souko/obj_c";
extern const char sRoukaCObjResourcePath[]                = "rouka/c_obj";
extern const char sRouka2fObjResourcePath[]               = "rouka/2f_obj";
extern const char sRouka1fSRopeResourcePath[]             = "rouka/1f_s_rope";
extern const char sRouka2fSRopeResourcePath[]             = "rouka/2f_s_rope";
extern const char sParentIgaguBookResourcePath[]          = "parent/igagu_book";
extern const char sParentFfHanaResourcePath[]             = "parent/ff_hana";
extern const char sLivingTanaueRopeResourcePath[]         = "living/tanaue_rope";
extern const char sNpcFunkyF2BResourcePath[]              = "npc/funky_f2_b";
extern const char sNpcFunkyF3BResourcePath[]              = "npc/funky_f3_b";
extern const char sParentFunkyStage2ResourcePath[]        = "parent/funky_stage2";
extern const char sBackyardCamBackyardResourcePath[]      = "backyard/cam_backyard";
extern const char sItemSakiFlower2ResourcePath[]          = "item/saki_flower2";
extern const char sItemSakiFlower3ResourcePath[]          = "item/saki_flower3";
extern const char sLivingFlowerKazari1ResourcePath[]      = "living/flower_kazari1";
extern const char sLivingFlowerKazari2ResourcePath[]      = "living/flower_kazari2";
extern const char sLivingFlowerKazari3ResourcePath[]      = "living/flower_kazari3";
extern const char sNpcTaoHunburgResourcePath[]            = "npc/tao_hunburg";
extern const char sNpcItoArmyResourcePath[]               = "npc/ito_army";
extern const char sHouseRItoHMainResourcePath[]           = "house_r/ito_h_main";
extern const char sKitchenTaoCollisionBox01ResourcePath[] = "kitchen/tao_collision_box01";
extern const char sKitchenTaoCollisionBox02ResourcePath[] = "kitchen/tao_collision_box02";
extern const char sKitchenTaoCollisionBox03ResourcePath[] = "kitchen/tao_collision_box03";
extern const char sKitchenBbCollisionBox01ResourcePath[]  = "kitchen/bb_collision_box01";
extern const char sAllYogoreFfResourcePath[]              = "all/yogore_ff";
extern const char sNpcEtTakoResourcePath[]                = "npc/et_tako";
extern const char sNpcDryFrogResourcePath[]               = "npc/dry_frog";
extern const char sLivingSpiderBLResourcePath[]           = "living/spider_b_l";
extern const char sKitchenFrypanResourcePath[]            = "kitchen/frypan";
extern const char sRoukaSpiderKakoiResourcePath[]         = "rouka/spider_kakoi";
extern const char sParentFfHanatabaResourcePath[]         = "parent/ff_hanataba";
extern const char sKitchenShelfLObjResourcePath[]         = "kitchen/shelf_l_obj";
extern const char sFxScream2dResourcePath[]               = "fx_scream2d";
extern const char sFxProse2dResourcePath[]                = "fx_prose2d";
extern const char sFxCaution2dResourcePath[]              = "fx_caution2d";
extern const char sFxTmTakeoffResourcePath[]              = "fx_tm_takeoff";
extern const char sFxMspiderEye2dResourcePath[]           = "fx_mspider_eye2d";
extern const char sNpcFunkyFAllResourcePath[]             = "npc/funky_f_all";
extern const char sNpcFunkyF2AllResourcePath[]            = "npc/funky_f2_all";
extern const char sNpcFunkyF3AllResourcePath[]            = "npc/funky_f3_all";
extern const char sJennyJObj1ResourcePath[]               = "jenny/j_obj1";
extern const char sJennyJObj2ResourcePath[]               = "jenny/j_obj2";
extern const char sItemCDenchiResourcePath[]              = "item/c_denchi";
extern const char sChibihouseJunkIreruResourcePath[]      = "chibihouse/junk_ireru";
extern const char sBackyardFullMoonResourcePath[]         = "backyard/full_moon";
extern const char sBackyardBrankoEndResourcePath[]        = "backyard/branko_end";
extern const char sAllRakugakiDekaResourcePath[]          = "all/rakugaki_deka";
extern const char sKitchenDenkiconroResourcePath[]        = "kitchen/denkiconro";
extern const char sBackyardTaoBag2ResourcePath[]          = "backyard/tao_bag2";
extern const char sNpcTHanamizuResourcePath[]             = "npc/t_hanamizu";
extern const char sNpcSpIto2ResourcePath[]                = "npc/sp_ito2";
extern const char sNpcTaoSlpResourcePath[]                = "npc/tao_slp";
extern const char sLivingTvKeath2ResourcePath[]           = "living/tv_keath2";
extern const char sNpcCusDecaroboResourcePath[]           = "npc/cus_decarobo";
extern const char sNpcDecaroboHandResourcePath[]          = "npc/decarobo_hand";
extern const char sJennyJTrashboxResourcePath[]           = "jenny/j_trashbox";
extern const char sParentPoleRopeResourcePath[]           = "parent/pole_rope";
extern const char sNpcUfo2ResourcePath[]                  = "npc/ufo2";
extern const char sKitchenPotResourcePath[]               = "kitchen/pot";
extern const char sKitchenPotRopeResourcePath[]           = "kitchen/pot_rope";
extern const char sParentDatuTamaResourcePath[]           = "parent/datu_tama";
extern const char sRoukaArmyHeri2ResourcePath[]           = "rouka/army_heri2";
extern const char sRoukaArmyRun1ResourcePath[]            = "rouka/army_run1";
extern const char sRoukaArmyRun2ResourcePath[]            = "rouka/army_run2";
extern const char sJennyPeetsFootResourcePath[]           = "jenny/peets_foot";
extern const char sBackyardHockAnaResourcePath[]          = "backyard/hock_ana";
extern const char sBackyardHockAnaKaidanResourcePath[]    = "backyard/hock_ana_kaidan";
extern const char sFxAlienEndResourcePath[]               = "fx_alien_end";
extern const char sFxDecaEndResourcePath[]                = "fx_deca_end";
extern const char sNpcZobin2ResourcePath[]                = "npc/zobin2";
extern const char sParentTpStageResourcePath[]            = "parent/tp_stage";
extern const char sJennyZobinDaiResourcePath[]            = "jenny/zobin_dai";
extern const char sKitchenTaoCollisionBox04ResourcePath[] = "kitchen/tao_collision_box04";
extern const char sNpcSunUfoResourcePath[]                = "npc/sun_ufo";
extern const char sCbRoboResourcePath[]                   = "cb_robo";
extern const char sCbPlugResourcePath[]                   = "cb_plug";
extern const char sCoinCResourcePath[]                    = "coin_c";
extern const char sCoinGResourcePath[]                    = "coin_g";
extern const char sCoinSResourcePath[]                    = "coin_s";
extern const char sSocketResourcePath[]                   = "socket";
extern const char sNpcTaoResourcePath[]                   = "npc/tao";
extern const char sEnergybResourcePath[]                  = "energyb";
extern const char sNpcUfoResourcePath[]                   = "npc/ufo";
extern const char sNpcEt1ResourcePath[]                   = "npc/et1";
extern const char sNpcBbResourcePath[]                    = "npc/bb";
extern const char sUfoMapResourcePath[]                   = "ufo/map";

XObjSource gObjectSources[] = {
	{ sCbRoboResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenMapResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenReizoukoResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenGasDaiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkUnder1ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkMidleResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTaoEResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenPlantBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenCounter1ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenCounter2ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTableResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenShelfLResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenWallWalkResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcConsentmanResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcTonpyResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenWindowSResourcePath, NULL, 0, { 0, 1 } },
	{ sCbPlugResourcePath, NULL, 0, { 0, 0 } },
	{ sTestBox4ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestBox8ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestBox12ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestBoxLiftResourcePath, NULL, 0, { 0, 0 } },
	{ sTestRopeResourcePath, NULL, 0, { 0, 0 } },
	{ sTestLadderResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaMapResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaDoorJResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorKResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorPamaResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaEntranceResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaKaidan1ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaKaidan2ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaKaidan3ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaMatResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaRoCounterResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRoPlantResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRoSofaResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTableKakuResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTableMaruResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenChair1ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTesuri2fResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWall12fResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWall1fResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaWall2fResourcePath, NULL, 0, { 0, 1 } },
	{ sPushobj3ResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardMapResourcePath, NULL, 0, { 0, 0 } },
	{ sCoinCResourcePath, NULL, 0, { 0, 1 } },
	{ sCoinGResourcePath, NULL, 0, { 0, 1 } },
	{ sCoinSResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardBaseResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardBgResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardFenceResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardWallResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoMapResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoWallResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sPushobj1ResourcePath, NULL, 0, { 0, 1 } },
	{ sPushobj2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemPenResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sSocketResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenWallResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDekaDenchiResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihousePcGamenResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseGachaResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseAirResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapResourcePath, NULL, 0, { 0, 0 } },
	{ sJennyBedResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sJennyChair1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyChair2ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenWindowResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyClosetResourcePath, NULL, 0, { 0, 0 } },
	{ sJennyDeskResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyDresserResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyLight1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMatResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyShelfResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyTableResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWallResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWallShelfResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWindow1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWindow2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentMapResourcePath, NULL, 0, { 0, 0 } },
	{ sParentBarwallLResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBarwallMResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBarwallRResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentBedResourcePath, NULL, 0, { 0, 1 } },
	{ sParentChairResourcePath, NULL, 0, { 0, 1 } },
	{ sParentClosetResourcePath, NULL, 0, { 0, 0 } },
	{ sParentCornershelfResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDresserResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSofaResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTableResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sMapJumpBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcTaoResourcePath, NULL, 0, { 0, 0 } },
	{ sParentWallResourcePath, NULL, 0, { 0, 0 } },
	{ sParentWallshelfResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWindowResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcHockResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoKaidanResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoDcDaiResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingShelfDanBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingShelfDanDaiResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorLResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorLDamiResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseCusCbPrLv1ResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseGachaPon1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallShelf1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTelResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTelCounterResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTelJResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallCounterResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingShelfWResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallUpResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingShelfSlopeResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFlowerclockResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sSoukoHasiraResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingLivingSkyResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingButterflyResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFlowerBedResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sRoukaKaidan2hResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaKaidan3hResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoWineBottleResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sRoukaHashiraTResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaHashiraEResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWallUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaFloor2fResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseMapResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseWallResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseChiPcResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseChiSocketResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseChiEResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorEResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWall2fUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sChibiHouseResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWindowResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemFrogRingResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sChibiJumpResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sEnergybResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcJennyResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingMapResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingDanroResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingShelfBResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingShelfDanResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSidetableResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSofa1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSofa2ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingStandlightResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTableResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTvResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWindowResourcePath, NULL, 0, { 0, 1 } },
	{ sHouseRChibiHMainResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHouseRChibiHDaiResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcMamaResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingSofaHashigoResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBooks1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBooks2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingSofaCu1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSofaCu2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingFlowerpot2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingShelfDanBookResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingTakoResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingOekakiResourcePath, NULL, 0, { 0, 0 } },
	{ sHouseRChibiHHasigoResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcArmy1ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcArmy2ResourcePath, NULL, 0, { 0, 0 } },
	{ sHouseRChibiHDoor1ResourcePath, NULL, 0, { 0, 1 } },
	{ sHouseRChibiHDoor2ResourcePath, NULL, 0, { 0, 1 } },
	{ sDoorroomResourcePath, NULL, 0, { 0, 1 } },
	{ sDoorroomDokanResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemSnackBoneResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemJunkUnkoResourcePath, NULL, 0, { 0, 1 } },
	{ sItemJunkAResourcePath, NULL, 0, { 0, 1 } },
	{ sTestTestRingResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingTableFootResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingMatResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTubResourcePath, NULL, 0, { 0, 1 } },
	{ sPlugStartResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcDecaRoboResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcKeathResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseDenti1ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseDenti2ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcPapaResourcePath, NULL, 0, { 0, 0 } },
	{ sHouseRChibiHHasiResourcePath, NULL, 0, { 0, 1 } },
	{ sHouseRChibiHWarpResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcFafaResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcSpider1ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcUfoResourcePath, NULL, 0, { 0, 0 } },
	{ sItemBrushResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSakanaResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingKaigaJResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCapsuleResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardAriResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBgResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardAriDaidaiResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sBackyardTreeBResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardKadanResourcePath, NULL, 0, { 0, 1 } },
	{ sPlugDokodakadoorResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBbDanResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingTrashboxResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTaoDishResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoSpiderSuBResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingSuisouR2ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSakanaAwaResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoKumoHimoResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcEt1ResourcePath, NULL, 0, { 0, 0 } },
	{ sItemKamiKuzuResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCookieKakeraResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTvKeathResourcePath, NULL, 0, { 0, 0 } },
	{ sTestDust0ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestDust1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSakanaEsaResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTanaAnaResourcePath, NULL, 0, { 0, 1 } },
	{ sTestSpItoResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingWallCornerResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCoUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sTestJennyHandResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingWindowKazariResourcePath, NULL, 0, { 0, 1 } },
	{ sItemJunkBResourcePath, NULL, 0, { 0, 1 } },
	{ sItemJunkCResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingGabyouResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sBackyardSkyResourcePath, NULL, 0, { 0, 1 } },
	{ sTestFlyRoot1ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcArmy3ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcNWing1ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcNWing2ResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaPlanterResourcePath, NULL, 0, { 0, 1 } },
	{ sItemSpoonResourcePath, NULL, 0, { 0, 1 } },
	{ sTestNMatoResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCubeBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCubeLResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCubeOResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCubeVResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCubeEResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCuAnimal1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCuAnimal2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemMagCupResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcSunMidaResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcSunDaiResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingCakeResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcBbResourcePath, NULL, 0, { 0, 0 } },
	{ sItemBaraResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingOrangeGlassResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingOrangeBinResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingChamGlassResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingChamBinResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingVaseResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingChickenResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingLivingCamResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcSpider2ResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingCakeDishResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingKazariTResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenDoorRResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingAsiatoResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingHatResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingKazariWResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBirthdayCResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBoneRResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenDoorDamiResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemCosObakeResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTutiHutaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkHiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkHiUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTrashboxResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHammockResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaShoesResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenSink2hiResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaSRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingHappyBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingConsentmanAtariResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTonpyItemAResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemBResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemCResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemHockTizuResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHowToChibiroboCbBookResourcePath, NULL, 0, { 0, 0 } },
	{ sFxCbHouse01ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCbCoverResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingHappyHeartResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreCookOResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreWaterResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreOilResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCamLivingResourcePath, NULL, 0, { 0, 1 } },
	{ sHouseRChibiHMakuResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenYakanResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTeacupResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTeapotResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenMilkBinResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSugarBinResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenCookieDishResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenCbTeasetResourcePath, NULL, 0, { 0, 1 } },
	{ sItemBlockSugarResourcePath, NULL, 0, { 0, 1 } },
	{ sItemFlowerCookieResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkSShelfResourcePath, NULL, 0, { 0, 1 } },
	{ sAllSpiderKageResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkDishResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHukinResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenSinkKagoResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTvSwitchResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTeacupSResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingCrackerResourcePath, NULL, 0, { 0, 1 } },
	{ sFxDecaSpark01ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTvNewsResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingTvKageResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCbBook2ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxTompCry01ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSw1planterResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSw2planterResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenBbNikkiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenBbPenResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sHowToChibiroboMapResourcePath, NULL, 0, { 0, 0 } },
	{ sFxTompScare01ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingDoorKResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCrackerResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCrackerGomiResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFClockHbResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFClockHsResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFClockFResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingPresentBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingBgTreeResourcePath, NULL, 0, { 0, 1 } },
	{ sFxSpider01ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingChickenBoneResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCakeAtoResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaCupArmyResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoWineBottleAResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoWineBottleBResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSink3hiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyTana1ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemChipResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingWindowWareResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWindowHirakuResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcArmy4ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcPrimoResourcePath, NULL, 0, { 0, 0 } },
	{ sParentPartitionResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPartition2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPartition3ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentPartition5ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWallshelf2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWallshelf3ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWallshelf4ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWallshelf5ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWallshelf6ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSwitchResourcePath, NULL, 0, { 0, 1 } },
	{ sHouseRDream1ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSidetableResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentLadderResourcePath, NULL, 0, { 0, 1 } },
	{ sParentCloset2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTable2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentChestResourcePath, NULL, 0, { 0, 1 } },
	{ sParentChest2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentBlindRResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBlindLResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBlindhandleRResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBlindhandleLResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSewingmachineResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyFlowerLampResourcePath, NULL, 0, { 0, 0 } },
	{ sItemReceiptResourcePath, NULL, 0, { 0, 1 } },
	{ sParentMamaPenResourcePath, NULL, 0, { 0, 1 } },
	{ sParentMamaCalculatorResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBillsResourcePath, NULL, 0, { 0, 1 } },
	{ sParentReceiptResourcePath, NULL, 0, { 0, 1 } },
	{ sParentAccountBookResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoMapResourcePath, NULL, 0, { 0, 0 } },
	{ sUfoWallResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTyuusyakiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHamburgResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyTvResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcFunkyFResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcTRexResourcePath, NULL, 0, { 0, 0 } },
	{ sAllUfoWarpResourcePath, NULL, 0, { 0, 1 } },
	{ sAllCllPapaResourcePath, NULL, 0, { 0, 1 } },
	{ sAllCllMamaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenGasPlateResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDoorbellResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDoorwindowResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDoorwindow2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPlanterResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPlanter2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPoleResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole3ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole4ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole5ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole6ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole7ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPole8ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentRecplayerResourcePath, NULL, 0, { 0, 1 } },
	{ sParentRecspeakerLResourcePath, NULL, 0, { 0, 1 } },
	{ sParentRecspeakerRResourcePath, NULL, 0, { 0, 1 } },
	{ sParentStpoleResourcePath, NULL, 0, { 0, 1 } },
	{ sParentStpole2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemEResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemFResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemGResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTonpyItemHResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemIResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTonpyItemJResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemKResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemLResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemMResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemNResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemOResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemPResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTonpyItemQResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemRResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemSResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTonpyItemTResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTonpyItemUResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardWindow1lResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardWindow2lResourcePath, NULL, 0, { 0, 1 } },
	{ sItemLetterResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDoorswitchResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsiba1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsiba2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsiba3ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCastleDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitai1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyKaidanResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHikidasiCloseResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHikidasiOpenResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorPamaHResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRopeDoorResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRopeNobuResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsibaS1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCastleHasigoResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHariLResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHariSResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitai2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitai3ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitai4ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDenchi1ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDenchi2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDenchi3ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemPeetsKutuResourcePath, NULL, 0, { 0, 1 } },
	{ sItemHocyoukiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsibaKado1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyAsiba4ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingMituFlowerResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHariL2ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcPapaSleepResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcMamaSleepResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcJennySleepResourcePath, NULL, 0, { 0, 0 } },
	{ sAllSaSealResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSkirtingBoardResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSkirtingBoard2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSkirtingBoardpResourcePath, NULL, 0, { 0, 1 } },
	{ sParentRecplayerCableResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBed2ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiObjResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaCeilingResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyTvGamenResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBedStayResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sAllYogoreTaiyaResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreKureyonResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreToyResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcFrogResourcePath, NULL, 0, { 0, 0 } },
	{ sParentCornershelfLadderResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardWindowWareResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardJagutiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHasiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHamPanUpResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHamPanDownResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitai5ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxLeticwarp01ResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardHoseResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaNebukuroResourcePath, NULL, 0, { 0, 1 } },
	{ sParentCamPapamamaResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCamJennyResourcePath, NULL, 0, { 0, 1 } },
	{ sAllEnjoyLightResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcZobinResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcPeetsResourcePath, NULL, 0, { 0, 0 } },
	{ sJennyHeitaiTaikoResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitaiRappaResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyHimoResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyBagResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactGiccoResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactTaoResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactFrogResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactInjuredResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactPajamasResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSaisunBoxResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sBackyardKumoUfoResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingHappyHeart2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyBedTopResourcePath, NULL, 0, { 0, 1 } },
	{ sAll1dayTimerResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorStopResourcePath, NULL, 0, { 0, 1 } },
	{ sItemRexToothResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyZobinBoxResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sRoukaKaidanAtamaResourcePath, NULL, 0, { 0, 1 } },
	{ sAllLeafResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaDoorKazariResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitaiGuitarResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcZobinBm1ResourcePath, NULL, 0, { 0, 0 } },
	{ sItemTaneAResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemBbPenResourcePath, NULL, 0, { 0, 1 } },
	{ sItemInkResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKareFlowerResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPresentHanaResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSunmidaskyResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKayakuResourcePath, NULL, 0, { 0, 1 } },
	{ sItemGoggleResourcePath, NULL, 0, { 0, 1 } },
	{ sItemLoveLetterResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTugResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTicketResourcePath, NULL, 0, { 0, 1 } },
	{ sItemHoutaiResourcePath, NULL, 0, { 0, 1 } },
	{ sItemBlockResourcePath, NULL, 0, { 0, 1 } },
	{ sItemRecordGiccioResourcePath, NULL, 0, { 0, 1 } },
	{ sItemRecordFfResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingWallCHResourcePath, NULL, 0, { 0, 1 } },
	{ sItemNwingItemResourcePath, NULL, 0, { 0, 0 } },
	{ sItemArmyPhotoResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaCounterHResourcePath, NULL, 0, { 0, 1 } },
	{ sTestFlyRoot2ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcCar1ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcCar2ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcFunkyF2ResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoShelfResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoChestResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoRengaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenKamareBbResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoShelfDamiResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenTutiAnaResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcHockShipResourcePath, NULL, 0, { 0, 0 } },
	{ sItemLeftFootResourcePath, NULL, 0, { 0, 1 } },
	{ sParentSuitCaseResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKairozuResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDekaDenchiFullResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHagurumaResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPlanterZobinResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPlanterJenny1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPlanterJenny2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoUfoBallResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoTimeMcResourcePath, NULL, 0, { 0, 0 } },
	{ sItemPajamaKijiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPresentHanaZobinResourcePath, NULL, 0, { 0, 1 } },
	{ sAllFlower1ResourcePath, NULL, 0, { 0, 1 } },
	{ sAllFlower2ResourcePath, NULL, 0, { 0, 1 } },
	{ sAllFlower3ResourcePath, NULL, 0, { 0, 1 } },
	{ sAllJunkTreeResourcePath, NULL, 0, { 0, 1 } },
	{ sAllCoinTreeResourcePath, NULL, 0, { 0, 1 } },
	{ sAllHappyTreeResourcePath, NULL, 0, { 0, 1 } },
	{ sAllMameTreeResourcePath, NULL, 0, { 0, 1 } },
	{ sJennySanpuHouseResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKareFlowerMituResourcePath, NULL, 0, { 0, 1 } },
	{ sItemSakiFlower1ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenJaguchiHResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTutiHutaDResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcTTrackResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcArmy5ResourcePath, NULL, 0, { 0, 0 } },
	{ sUfoWall2ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenWallTonneruResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenWallTWareResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyToyDenwaResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyToyPianoResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoExitResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingDensetuHanaResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiMapResourcePath, NULL, 0, { 0, 0 } },
	{ sHaisuiPipeAResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHaisuiHashigoBResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiHashigoCResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiHashigoDResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sUfoGroundResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGround2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGround3ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoBallRotResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGame1ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGame2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGame3ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGame4ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameBotanResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameP1ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameP2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameP3ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameP4ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoWall3ResourcePath, NULL, 0, { 0, 1 } },
	{ sTestCarRoot1ResourcePath, NULL, 0, { 0, 1 } },
	{ sTestCarRoot2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoGameBotan2ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestNMato2ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSinkHashigoResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaAResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaARaResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaARbResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyChibiIsuResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaBResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaBRResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaCResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaCRResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaDResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTutaDRResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenConroResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenKingyoResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenWPlanterResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiHashiAResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiHashiBResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyKenban1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyKenban2ResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentTpMike1ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTpMike2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHouseHimoResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyBagHimoResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiKakoiResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaArmyIta1ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaArmyIta2ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaArmyHeri1ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoUfoBedResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyDressResourcePath, NULL, 0, { 0, 1 } },
	{ sFxKitchenburnerResourcePath, NULL, 0, { 0, 1 } },
	{ sFxWarpzoneResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoUfoBed2ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoUfoBed3ResourcePath, NULL, 0, { 0, 1 } },
	{ sUfoUfoBed4ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcNasubiResourcePath, NULL, 0, { 0, 0 } },
	{ sAllTonpyItemKageResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRaceItaResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiSotoResourcePath, NULL, 0, { 0, 0 } },
	{ sHaisuiNezumiResourcePath, NULL, 0, { 0, 0 } },
	{ sHaisuiHashigoFResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHaisuiRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiPipeBResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyDress2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyDress3ResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiKabeResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sSoukoBirdcageResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoBoardResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoNailResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoVRopeResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHaisuiWaterResourcePath, NULL, 0, { 0, 1 } },
	{ sItemPajamaKiji2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemPajamaKiji3ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan1ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan3ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan4ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan5ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKazan6ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock1ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock3ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock4ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock5ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTBlock6ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCarItemResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sBackyardTHashigoResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardBrankoResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardBRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sItemPapaYubiwaResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTKageResourcePath, NULL, 0, { 0, 1 } },
	{ sFxZactGhostResourcePath, NULL, 0, { 0, 1 } },
	{ sParentFrogHagataResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKusaResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaBebeBinsenResourcePath, NULL, 0, { 0, 0 } },
	{ sItemSekkeizuResourcePath, NULL, 0, { 0, 1 } },
	{ sItemItemFrogResourcePath, NULL, 0, { 0, 1 } },
	{ sFxPcBuy01ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyBookResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyPlanterHachiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyKureyonResourcePath, NULL, 0, { 0, 1 } },
	{ sFxCbdoor01ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxTmBarrierResourcePath, NULL, 0, { 0, 1 } },
	{ sJennySuberidai1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennySuberidai2ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingAlbumTResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingAlbumYResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTableStandResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingKaizouPoperResourcePath, NULL, 0, { 0, 0 } },
	{ sParentWeldingMaskpResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingMaskmResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingMaskjResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingMasktResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingTorchResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingToolboxResourcePath, NULL, 0, { 0, 1 } },
	{ sParentWeldingToolsResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDrafterResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenGasHashigoResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenGasRopeResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentCaseJanksResourcePath, NULL, 0, { 0, 0 } },
	{ sYaneTorinosuResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardKusaTutiResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingKeathOmcResourcePath, NULL, 0, { 0, 1 } },
	{ sTitleIconIconBbResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTaoBagResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaKokubanResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentDcfootSpiderResourcePath, NULL, 0, { 0, 1 } },
	{ sAllTakarabakoResourcePath, NULL, 0, { 0, 1 } },
	{ sCbCannonLv2ResourcePath, NULL, 0, { 0, 1 } },
	{ sCbRadarResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingCbKaiPoperResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sHaisuiRoom1CResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sFxDecaEnergyResourcePath, NULL, 0, { 0, 1 } },
	{ sFxDecaBatteryResourcePath, NULL, 0, { 0, 1 } },
	{ sFx3pu01ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxPapa01ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenKankisenResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTamagottiResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcDecaRobo2ResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenToasterResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSenzaiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenSuponjiResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenJamResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenGusUpResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreHSResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreHKResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcSpider3ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcSunKaoResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenPuroperaResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyCastleHasigo2ResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreHitoResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreHunResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreShimiResourcePath, NULL, 0, { 0, 1 } },
	{ sFxArmySgtResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJennyKagamiResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTamagottiGamenResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenKankisenAsibaResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sLivingLightRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingAudioResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingSibariMamaResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingSibariPapaResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingSibariTaoResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingSibariJennyResourcePath, NULL, 0, { 0, 0 } },
	{ sItemAkikanResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCandyGomiResourcePath, NULL, 0, { 0, 1 } },
	{ sItemOkasiGomi1ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemOkasiGomi2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemItemNasubiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWallHabakiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWallTopResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyWallTileResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPapaToyboxResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPapaToybox2ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentRecordsResourcePath, NULL, 0, { 0, 1 } },
	{ sFxGiccoTwinb01ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxGiccoTwinb02ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxGiccoTwinb03ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemDensetuTaneResourcePath, NULL, 0, { 0, 1 } },
	{ sFxBackyardRainResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaCoUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiSResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTakiSCResourcePath, NULL, 0, { 0, 1 } },
	{ sParentRecplayerNeedleResourcePath, NULL, 0, { 0, 1 } },
	{ sFxAlien01ResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardAriSuResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenCoUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcBirdResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sNpcTaoSResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentTBlocksResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenMituBinResourcePath, NULL, 0, { 0, 1 } },
	{ sJennySanpuBinResourcePath, NULL, 0, { 0, 1 } },
	{ sTestSpiderJanks1ResourcePath, NULL, 0, { 0, 0 } },
	{ sChibihouseRankingNoResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseRankingBoardResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sSoukoSunRagResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoSunRouResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingTvNews2ResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardKumoAmeResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogore30kResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogore50kResourcePath, NULL, 0, { 0, 1 } },
	{ sHaisuiAnaAResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenBgResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenFruitResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHaeResourcePath, NULL, 0, { 0, 1 } },
	{ sFxMspiderEyeResourcePath, NULL, 0, { 0, 1 } },
	{ sTestMtJank1ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank2ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank3ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank4ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank5ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank6ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank7ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank8ResourcePath, NULL, 0, { 0, 0 } },
	{ sTestMtJank9ResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sItemKure1ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKure2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKure3ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKure4ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemKure5ResourcePath, NULL, 0, { 0, 1 } },
	{ sParentGboxBigResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sParentGboxSmallResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFafaOmcResourcePath, NULL, 0, { 0, 1 } },
	{ sTestSp3BomResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardWindowKResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTvSunaResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardJimenRyuukiResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sBackyardFuneTutiDResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect10ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect2ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect3ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect4ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect5ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect6ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect7ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect8ResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseElect9ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingHodokeItoResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseDenti2conResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreKureyonKResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardGrassResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingMotherWallResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingMotherMapResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingProtectorResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingApronResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingCurtainResourcePath, NULL, 0, { 0, 1 } },
	{ sParentTBlocks2ResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaNwingDaiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenShelfLHaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenShelfLHbResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenClockLResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenClockSResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenShelfLRResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenHamburgKResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMiniBoxResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMiniHikidasiResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapKure1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapKure2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapKure3ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapKure4ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMapKure5ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxTompCam2dResourcePath, NULL, 0, { 0, 1 } },
	{ sFxMummiformResourcePath, NULL, 0, { 0, 1 } },
	{ sFxPirateboatResourcePath, NULL, 0, { 0, 1 } },
	{ sFxMspider01ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxMspider02ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxTmUniteResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJClockLResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJClockSResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoSClockLResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoSClockSResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRClockLResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaRClockSResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sJennyCartainResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyTvSampooResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyBagHimo2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyBagHimo3ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyBagHimo4ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyHeitaiTaoreResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyShelfBooksResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMoveBook1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyMoveBook2ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennySanpuBin2ResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoBirdcageShortcutResourcePath, NULL, 0, { 0, 0 } },
	{ sSoukoBirdcageShortcut2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemMiniFunky1ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcSpider4ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcZobinBm2ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcZobinBm3ResourcePath, NULL, 0, { 0, 0 } },
	{ sParentFunkyStageResourcePath, NULL, 0, { 0, 0 } },
	{ sItemMiniFunkyButiResourcePath, NULL, 0, { 0, 1 } },
	{ sItemTaneFunkyResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcFunkyF3ResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenShelfLRbResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaSpiderBResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcTyouBbResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaWHashigoResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaWRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTeRopeAResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaTeRopeBResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaBgResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaLightResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoCoUnderResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoSUnderObjResourcePath, NULL, 0, { 0, 1 } },
	{ sParentClockLResourcePath, NULL, 0, { 0, 1 } },
	{ sParentClockSResourcePath, NULL, 0, { 0, 1 } },
	{ sParentClockFResourcePath, NULL, 0, { 0, 1 } },
	{ sParentClockCuckooResourcePath, NULL, 0, { 0, 1 } },
	{ sParentBg01ResourcePath, NULL, 0, { 0, 1 } },
	{ sFxSpider02ResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoObjAResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoObjBResourcePath, NULL, 0, { 0, 1 } },
	{ sSoukoObjCResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaCObjResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sRouka2fObjResourcePath, NULL, 0, { 0, 1 } },
	{ sRouka1fSRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sRouka2fSRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sParentIgaguBookResourcePath, NULL, 0, { 0, 1 } },
	{ sParentFfHanaResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingTanaueRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcFunkyF2BResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcFunkyF3BResourcePath, NULL, 0, { 0, 0 } },
	{ sParentFunkyStage2ResourcePath, NULL, 0, { 0, 0 } },
	{ sBackyardCamBackyardResourcePath, NULL, 0, { 0, 1 } },
	{ sItemSakiFlower2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemSakiFlower3ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFlowerKazari1ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFlowerKazari2ResourcePath, NULL, 0, { 0, 1 } },
	{ sLivingFlowerKazari3ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcTaoHunburgResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcItoArmyResourcePath, NULL, 0, { 0, 0 } },
	{ sHouseRItoHMainResourcePath, NULL, 0, { 0, 1 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sKitchenTaoCollisionBox01ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTaoCollisionBox02ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTaoCollisionBox03ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenBbCollisionBox01ResourcePath, NULL, 0, { 0, 1 } },
	{ sAllYogoreFfResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcEtTakoResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcDryFrogResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingSpiderBLResourcePath, NULL, 0, { 0, 0 } },
	{ sKitchenFrypanResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaSpiderKakoiResourcePath, NULL, 0, { 0, 1 } },
	{ sParentFfHanatabaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenShelfLObjResourcePath, NULL, 0, { 0, 1 } },
	{ sFxScream2dResourcePath, NULL, 0, { 0, 0 } },
	{ sFxProse2dResourcePath, NULL, 0, { 0, 1 } },
	{ sFxCaution2dResourcePath, NULL, 0, { 0, 1 } },
	{ sFxTmTakeoffResourcePath, NULL, 0, { 0, 1 } },
	{ sFxMspiderEye2dResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcFunkyFAllResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcFunkyF2AllResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcFunkyF3AllResourcePath, NULL, 0, { 0, 0 } },
	{ sJennyJObj1ResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyJObj2ResourcePath, NULL, 0, { 0, 1 } },
	{ sItemCDenchiResourcePath, NULL, 0, { 0, 1 } },
	{ sChibihouseJunkIreruResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardFullMoonResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardBrankoEndResourcePath, NULL, 0, { 0, 1 } },
	{ sAllRakugakiDekaResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenDenkiconroResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardTaoBag2ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcTHanamizuResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcSpIto2ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcTaoSlpResourcePath, NULL, 0, { 0, 0 } },
	{ sLivingTvKeath2ResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcCusDecaroboResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcDecaroboHandResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0, 0 } },
	{ sJennyJTrashboxResourcePath, NULL, 0, { 0, 1 } },
	{ sParentPoleRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcUfo2ResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenPotResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenPotRopeResourcePath, NULL, 0, { 0, 1 } },
	{ sParentDatuTamaResourcePath, NULL, 0, { 0, 1 } },
	{ sRoukaArmyHeri2ResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaArmyRun1ResourcePath, NULL, 0, { 0, 0 } },
	{ sRoukaArmyRun2ResourcePath, NULL, 0, { 0, 0 } },
	{ sJennyPeetsFootResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardHockAnaResourcePath, NULL, 0, { 0, 1 } },
	{ sBackyardHockAnaKaidanResourcePath, NULL, 0, { 0, 1 } },
	{ sFxAlienEndResourcePath, NULL, 0, { 0, 1 } },
	{ sFxDecaEndResourcePath, NULL, 0, { 0, 1 } },
	{ sNpcZobin2ResourcePath, NULL, 0, { 0, 0 } },
	{ sParentTpStageResourcePath, NULL, 0, { 0, 1 } },
	{ sJennyZobinDaiResourcePath, NULL, 0, { 0, 1 } },
	{ sKitchenTaoCollisionBox04ResourcePath, NULL, 0, { 0, 0 } },
	{ sNpcSunUfoResourcePath, NULL, 0, { 0, 0 } },
	{ NULL, NULL, 0, { 0xffffffff, 0xffffffff } },
};

// Resource indices are shared by objects with the same archive.
#define OBJECT(source, flags, shadow, priority, range, category, item) \
	{ { (u32)(source) }, 0, flags, shadow, priority, range, category, item }
UnkObjData objTable[] = {
	// Object IDs 0-31
	OBJECT(gObjectSources + 0, 0x2c9, 3, 0, 0, 2, 0),
	OBJECT(gObjectSources + 1, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 2, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 3, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 4, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 5, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 6, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 7, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 8, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 9, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 10, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 11, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 12, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 13, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 14, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 15, 0x289, 3, 0, 0, 10, 10),
	OBJECT(gObjectSources + 16, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 17, 0x1, 0, 0, 0, 24, 0),
	OBJECT(gObjectSources + 18, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 19, 0x1, 0, 0, 0, 5, 13),
	OBJECT(gObjectSources + 20, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 21, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 22, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 23, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 24, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 25, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 26, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 27, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 28, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 29, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 30, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 31, 0x3, 0, 0, 0, 22, 0),
	// Object IDs 32-63
	OBJECT(gObjectSources + 32, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 33, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 34, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 35, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 36, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 37, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 38, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 39, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 40, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 41, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 42, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 43, 0x1, 0, 0, 0, 22, 30),
	OBJECT(gObjectSources + 44, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 45, 0x1, 0, 1, 15, 7, 2),
	OBJECT(gObjectSources + 46, 0x1, 0, 1, 15, 7, 0),
	OBJECT(gObjectSources + 47, 0x1, 0, 1, 15, 7, 1),
	OBJECT(gObjectSources + 48, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 49, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 50, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 51, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 52, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 53, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 54, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 55, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 56, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 57, 0x1, 0, 0, 0, 22, 30),
	OBJECT(gObjectSources + 58, 0x1, 0, 0, 0, 22, 30),
	OBJECT(gObjectSources + 59, 0x1, 0, 1, 15, 8, 1),
	OBJECT(gObjectSources + 60, 0x1, 0, 1, 15, 8, 2),
	OBJECT(gObjectSources + 61, 0x1, 0, 1, 15, 8, 3),
	OBJECT(gObjectSources + 62, 0x1, 0, 1, 15, 8, 4),
	OBJECT(gObjectSources + 63, 0x1, 0, 1, 15, 8, 5),
	// Object IDs 64-95
	OBJECT(gObjectSources + 64, 0x1, 0, 1, 15, 8, 6),
	OBJECT(gObjectSources + 65, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 66, 0x1, 0, 1, 15, 8, 7),
	OBJECT(gObjectSources + 67, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 68, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 69, 0x1, 0, 0, 0, 8, 8),
	OBJECT(gObjectSources + 70, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 71, 0x1, 0, 0, 0, 5, 7),
	OBJECT(gObjectSources + 72, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 73, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 74, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 75, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 76, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 77, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 78, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 79, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 80, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 81, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 82, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 83, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 84, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 85, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 86, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 87, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 88, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 89, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 90, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 91, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 92, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 93, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 94, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 95, 0x3, 0, 0, 0, 22, 0),
	// Object IDs 96-127
	OBJECT(gObjectSources + 96, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 97, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 98, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 99, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 100, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 101, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 102, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 103, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 104, 0x3, 0, 0, 0, 22, 0),
	OBJECT(NULL, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 106, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 107, 0x81, 0, 0, 0, 14, 0),
	OBJECT(gObjectSources + 108, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 109, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 110, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 111, 0x81, 0, 0, 0, 10, 20),
	OBJECT(gObjectSources + 112, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 113, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 114, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 115, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 116, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 117, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 118, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 119, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 120, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 121, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 122, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 123, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 124, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 125, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 126, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 127, 0x3, 0, 0, 0, 22, 0),
	// Object IDs 128-159
	OBJECT(gObjectSources + 128, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 129, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 130, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 131, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 132, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 133, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 134, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 135, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 136, 0x1, 0, 0, 0, 22, 0),
	OBJECT(NULL, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 138, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 139, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 140, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 141, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 142, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 143, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 144, 0x1, 0, 1, 15, 8, 50),
	OBJECT(gObjectSources + 145, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 146, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 147, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 148, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 149, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 150, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 151, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 152, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 153, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 154, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 155, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 156, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 157, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 158, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 159, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 160-191
	OBJECT(gObjectSources + 160, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 161, 0x1, 0, 1, 15, 8, 0),
	OBJECT(gObjectSources + 162, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 163, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 164, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 165, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 166, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 167, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 168, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 169, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 170, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 171, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 172, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 173, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 174, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 175, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 176, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 177, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 178, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 179, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 180, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 181, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 182, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 183, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 184, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 185, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 186, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 187, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 188, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 189, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 190, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 191, 0x1, 0, 1, 15, 7, 200),
	// Object IDs 192-223
	OBJECT(gObjectSources + 192, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 193, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 194, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 195, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 196, 0x81, 0, 0, 0, 11, 0),
	OBJECT(gObjectSources + 197, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 198, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 199, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 200, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 201, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 202, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 203, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 204, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 205, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 206, 0x2003, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 207, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 208, 0x4003, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 209, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 210, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 211, 0x4021, 0, 1, 20, 9, 0),
	OBJECT(gObjectSources + 212, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 213, 0x4003, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 214, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 215, 0x81, 0, 0, 0, 12, 0),
	OBJECT(gObjectSources + 216, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 217, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 218, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 219, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 220, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 221, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 222, 0x4003, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 223, 0x4003, 0, 0, 0, 22, 0),
	// Object IDs 224-255
	OBJECT(gObjectSources + 224, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 225, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 226, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 227, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 228, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 229, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 230, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 231, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 232, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 233, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 234, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 235, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 236, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 237, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 238, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 239, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 240, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 241, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 242, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 243, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 244, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 245, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 246, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 247, 0x81, 0, 0, 0, 16, 0),
	OBJECT(gObjectSources + 248, 0x81, 4, 0, 0, 16, 0),
	OBJECT(gObjectSources + 249, 0x4003, 0, 0, 0, 5, 1),
	OBJECT(gObjectSources + 250, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 251, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 252, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 253, 0x1, 0, 1, 15, 8, 9),
	OBJECT(gObjectSources + 254, 0x1, 0, 1, 15, 8, 10),
	OBJECT(gObjectSources + 255, 0x1, 0, 1, 15, 8, 11),
	// Object IDs 256-287
	OBJECT(gObjectSources + 256, 0x1, 0, 1, 15, 8, 12),
	OBJECT(gObjectSources + 257, 0x1, 0, 1, 15, 8, 13),
	OBJECT(gObjectSources + 258, 0x1, 0, 1, 15, 8, 14),
	OBJECT(gObjectSources + 259, 0x1, 0, 1, 15, 8, 15),
	OBJECT(gObjectSources + 260, 0x1, 0, 1, 15, 7, 103),
	OBJECT(gObjectSources + 261, 0x1, 0, 1, 15, 7, 100),
	OBJECT(gObjectSources + 262, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 263, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 264, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 265, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 266, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 267, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 268, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 269, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 270, 0x21, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 271, 0x81, 0, 0, 0, 15, 0),
	OBJECT(gObjectSources + 272, 0x81, 0, 0, 0, 10, 30),
	OBJECT(gObjectSources + 273, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 274, 0x1, 0, 0, 0, 8, 48),
	OBJECT(gObjectSources + 275, 0x81, 0, 0, 0, 13, 0),
	OBJECT(gObjectSources + 276, 0x6003, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 277, 0x23, 0, 1, 20, 9, 1),
	OBJECT(gObjectSources + 278, 0x1, 0, 1, 20, 9, 2),
	OBJECT(gObjectSources + 279, 0x81, 0, 0, 0, 10, 40),
	OBJECT(gObjectSources + 280, 0x181, 0, 0, 0, 17, 0),
	OBJECT(gObjectSources + 281, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 282, 0x81, 0, 1, 15, 8, 16),
	OBJECT(gObjectSources + 283, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 284, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 285, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 17),
	OBJECT(gObjectSources + 287, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 288-319
	OBJECT(gObjectSources + 288, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 289, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 290, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 291, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 292, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 293, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 294, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 295, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 296, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 297, 0x21, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 298, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 299, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 300, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 301, 0x1, 0, 0, 0, 5, 6),
	OBJECT(gObjectSources + 302, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 303, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 304, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 305, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 306, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 307, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 308, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 309, 0x1, 0, 1, 15, 8, 18),
	OBJECT(gObjectSources + 310, 0x1, 0, 1, 15, 8, 19),
	OBJECT(gObjectSources + 311, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 312, 0x1, 0, 0, 0, 20, 1),
	OBJECT(gObjectSources + 313, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 314, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 315, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 316, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 317, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 318, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 319, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 320-351
	OBJECT(gObjectSources + 320, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 321, 0x1, 0, 0, 0, 7, 101),
	OBJECT(gObjectSources + 322, 0x1, 0, 0, 0, 7, 102),
	OBJECT(gObjectSources + 323, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 324, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 325, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 326, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 327, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 328, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 329, 0x1, 0, 0, 0, 5, 0),
	OBJECT(gObjectSources + 330, 0x1, 0, 0, 0, 5, 0),
	OBJECT(gObjectSources + 331, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 332, 0x81, 0, 1, 15, 8, 21),
	OBJECT(gObjectSources + 333, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 334, 0x3, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 335, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 336, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 337, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 338, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 339, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 340, 0x1, 0, 1, 15, 22, 0),
	OBJECT(gObjectSources + 341, 0x81, 0, 1, 15, 8, 22),
	OBJECT(gObjectSources + 342, 0x81, 0, 0, 0, 10, 50),
	OBJECT(gObjectSources + 343, 0x1, 0, 0, 0, 10, 51),
	OBJECT(gObjectSources + 344, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 345, 0x81, 0, 0, 0, 10, 60),
	OBJECT(gObjectSources + 346, 0x1, 0, 1, 15, 8, 23),
	OBJECT(gObjectSources + 347, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 348, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 349, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 350, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 351, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 352-383
	OBJECT(gObjectSources + 352, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 353, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 354, 0x181, 0, 0, 0, 18, 0),
	OBJECT(gObjectSources + 355, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 356, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 357, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 358, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 359, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 360, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 361, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 362, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 363, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 364, 0x1, 0, 0, 0, 10, 70),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 24),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 25),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 26),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 27),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 28),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 29),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 30),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 31),
	OBJECT(gObjectSources + 286, 0x1, 0, 1, 15, 8, 32),
	OBJECT(gObjectSources + 374, 0x1, 0, 0, 0, 8, 33),
	OBJECT(gObjectSources + 375, 0x1, 0, 0, 0, 8, 34),
	OBJECT(gObjectSources + 376, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 377, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 378, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 379, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 380, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 381, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 382, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 383, 0x3, 0, 0, 0, 22, 0),
	// Object IDs 384-415
	OBJECT(gObjectSources + 384, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 385, 0x9, 3, 0, 0, 21, 0),
	OBJECT(gObjectSources + 386, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 387, 0x1, 0, 1, 15, 8, 35),
	OBJECT(gObjectSources + 388, 0x1, 0, 1, 15, 8, 36),
	OBJECT(gObjectSources + 389, 0x1, 0, 1, 15, 8, 37),
	OBJECT(gObjectSources + 390, 0x1, 0, 1, 15, 8, 38),
	OBJECT(gObjectSources + 391, 0x1, 0, 1, 15, 8, 39),
	OBJECT(gObjectSources + 392, 0x1, 0, 1, 15, 8, 40),
	OBJECT(gObjectSources + 393, 0x1, 0, 1, 15, 8, 41),
	OBJECT(gObjectSources + 393, 0x1, 0, 1, 15, 8, 42),
	OBJECT(gObjectSources + 393, 0x1, 0, 1, 15, 8, 43),
	OBJECT(gObjectSources + 396, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 397, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 398, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 399, 0x1, 0, 0, 0, 7, 300),
	OBJECT(gObjectSources + 400, 0x1, 0, 0, 0, 20, 50),
	OBJECT(gObjectSources + 401, 0x1, 0, 0, 0, 20, 50),
	OBJECT(gObjectSources + 402, 0x1, 0, 0, 0, 20, 50),
	OBJECT(gObjectSources + 403, 0x1, 0, 0, 0, 0, 0),
	OBJECT(gObjectSources + 404, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 405, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 406, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 407, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 408, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 409, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 410, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 411, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 412, 0x1, 0, 1, 15, 8, 44),
	OBJECT(gObjectSources + 413, 0x1, 0, 1, 15, 8, 45),
	OBJECT(gObjectSources + 414, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 415, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 416-447
	OBJECT(gObjectSources + 416, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 417, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 418, 0x1, 0, 1, 15, 8, 46),
	OBJECT(gObjectSources + 419, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 420, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 421, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 422, 0x1, 0, 1, 15, 8, 47),
	OBJECT(gObjectSources + 423, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 424, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 425, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 426, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 427, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 428, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 429, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 430, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 431, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 432, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 433, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 434, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 435, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 436, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 437, 0x1, 0, 1, 15, 8, 49),
	OBJECT(gObjectSources + 438, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 439, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 440, 0x1, 0, 0, 0, 5, 3),
	OBJECT(gObjectSources + 441, 0x1, 0, 0, 0, 5, 4),
	OBJECT(gObjectSources + 442, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 443, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 444, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 445, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 446, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 447, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 448-479
	OBJECT(gObjectSources + 448, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 449, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 450, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 451, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 452, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 453, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 454, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 455, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 456, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 457, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 458, 0x1, 0, 1, 15, 8, 51),
	OBJECT(gObjectSources + 459, 0x1, 0, 1, 15, 8, 52),
	OBJECT(gObjectSources + 460, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 461, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 462, 0x1, 0, 1, 15, 8, 53),
	OBJECT(gObjectSources + 462, 0x1, 0, 1, 15, 8, 54),
	OBJECT(gObjectSources + 464, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 465, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 466, 0x81, 0, 0, 0, 6, 0),
	OBJECT(gObjectSources + 467, 0x81, 0, 0, 0, 10, 0),
	OBJECT(gObjectSources + 468, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 469, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 470, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 471, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 472, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 473, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 474, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 475, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 476, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 477, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 478, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 479, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 480-511
	OBJECT(gObjectSources + 480, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 481, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 482, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 483, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 484, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 485, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 486, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 487, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 488, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 489, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 490, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 491, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 492, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 493, 0x2001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 494, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 495, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 496, 0x1, 0, 1, 15, 8, 55),
	OBJECT(gObjectSources + 497, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 498, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 499, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 500, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 501, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 502, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 503, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 504, 0x81, 0, 1, 15, 8, 56),
	OBJECT(gObjectSources + 505, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 506, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 507, 0x81, 0, 0, 0, 10, 100),
	OBJECT(gObjectSources + 508, 0x81, 0, 0, 0, 10, 80),
	OBJECT(gObjectSources + 509, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 510, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 511, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 512-543
	OBJECT(gObjectSources + 512, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 513, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 514, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 515, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 516, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 517, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 518, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 519, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 520, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 521, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 522, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 523, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 524, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 525, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 526, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 527, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 528, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 529, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 530, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 531, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 532, 0x1, 0, 1, 15, 8, 63),
	OBJECT(gObjectSources + 533, 0x1, 0, 1, 15, 8, 64),
	OBJECT(gObjectSources + 534, 0x1, 0, 1, 15, 8, 65),
	OBJECT(gObjectSources + 535, 0x1, 0, 1, 15, 8, 66),
	OBJECT(gObjectSources + 536, 0x1, 0, 1, 15, 8, 67),
	OBJECT(gObjectSources + 537, 0x1, 0, 1, 15, 8, 68),
	OBJECT(gObjectSources + 538, 0x1, 0, 1, 15, 8, 69),
	OBJECT(gObjectSources + 539, 0x1, 0, 1, 15, 8, 70),
	OBJECT(gObjectSources + 540, 0x1, 0, 1, 15, 8, 71),
	OBJECT(gObjectSources + 541, 0x1, 0, 1, 15, 8, 72),
	OBJECT(gObjectSources + 542, 0x9, 2, 1, 15, 8, 73),
	OBJECT(gObjectSources + 543, 0x1, 0, 1, 15, 8, 74),
	// Object IDs 544-575
	OBJECT(gObjectSources + 544, 0x1, 0, 1, 15, 8, 75),
	OBJECT(gObjectSources + 545, 0x1, 0, 1, 15, 8, 76),
	OBJECT(gObjectSources + 546, 0x1, 0, 1, 15, 8, 77),
	OBJECT(gObjectSources + 547, 0x1, 0, 1, 15, 8, 78),
	OBJECT(gObjectSources + 548, 0x1, 0, 1, 15, 8, 79),
	OBJECT(gObjectSources + 549, 0x1, 0, 1, 15, 8, 80),
	OBJECT(gObjectSources + 550, 0x1, 0, 1, 15, 8, 81),
	OBJECT(gObjectSources + 551, 0x1, 0, 1, 15, 8, 82),
	OBJECT(gObjectSources + 552, 0x1, 0, 1, 15, 8, 83),
	OBJECT(gObjectSources + 553, 0x1, 0, 1, 15, 8, 84),
	OBJECT(gObjectSources + 554, 0x1, 0, 1, 15, 8, 85),
	OBJECT(gObjectSources + 555, 0x1, 0, 1, 15, 8, 86),
	OBJECT(gObjectSources + 556, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 557, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 558, 0x1, 0, 1, 15, 8, 57),
	OBJECT(gObjectSources + 559, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 560, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 561, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 562, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 563, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 564, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 565, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 566, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 567, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 568, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 569, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 570, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 571, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 572, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 573, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 574, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 575, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 576-607
	OBJECT(gObjectSources + 576, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 577, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 578, 0x1, 0, 1, 15, 8, 58),
	OBJECT(gObjectSources + 579, 0x1, 0, 1, 15, 8, 59),
	OBJECT(gObjectSources + 580, 0x1, 0, 1, 15, 8, 60),
	OBJECT(gObjectSources + 581, 0x1, 0, 1, 15, 8, 61),
	OBJECT(gObjectSources + 582, 0x1, 0, 1, 15, 8, 62),
	OBJECT(gObjectSources + 583, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 584, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 585, 0x1, 0, 0, 0, 20, 36),
	OBJECT(gObjectSources + 586, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 587, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 588, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 589, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 590, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 591, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 592, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 593, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 594, 0x2401, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 595, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 596, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 597, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 598, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 599, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 600, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 601, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 602, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 603, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 604, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 605, 0x1, 0, 0, 0, 20, 50),
	OBJECT(gObjectSources + 606, 0x81, 0, 0, 0, 10, 120),
	OBJECT(gObjectSources + 607, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 608-639
	OBJECT(gObjectSources + 608, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 609, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 610, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 611, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 612, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 613, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 614, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 615, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 616, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 617, 0x1, 0, 0, 0, 0, 0),
	OBJECT(gObjectSources + 618, 0x1, 0, 0, 0, 0, 0),
	OBJECT(gObjectSources + 619, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 620, 0x81, 0, 0, 0, 10, 90),
	OBJECT(gObjectSources + 621, 0x81, 0, 0, 0, 10, 110),
	OBJECT(gObjectSources + 622, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 623, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 624, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 625, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 626, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 627, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 628, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 629, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 630, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 631, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 632, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 633, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 634, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 635, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 636, 0x1, 0, 1, 15, 8, 87),
	OBJECT(gObjectSources + 637, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 638, 0x1, 0, 1, 15, 8, 88),
	OBJECT(gObjectSources + 639, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 640-671
	OBJECT(gObjectSources + 636, 0x1, 0, 1, 15, 8, 89),
	OBJECT(gObjectSources + 636, 0x1, 0, 1, 15, 8, 90),
	OBJECT(gObjectSources + 642, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 643, 0x1, 0, 1, 15, 8, 91),
	OBJECT(gObjectSources + 644, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 645, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 646, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 647, 0x1, 0, 1, 15, 8, 92),
	OBJECT(gObjectSources + 647, 0x1, 0, 1, 15, 8, 93),
	OBJECT(gObjectSources + 647, 0x1, 0, 1, 15, 8, 94),
	OBJECT(gObjectSources + 650, 0x1, 0, 1, 15, 8, 95),
	OBJECT(gObjectSources + 651, 0x1, 0, 1, 15, 8, 96),
	OBJECT(gObjectSources + 652, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 653, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 654, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 655, 0x1, 0, 1, 15, 8, 98),
	OBJECT(gObjectSources + 656, 0x1, 0, 1, 15, 8, 99),
	OBJECT(gObjectSources + 657, 0x1, 0, 1, 15, 8, 100),
	OBJECT(gObjectSources + 658, 0x1, 0, 1, 15, 8, 101),
	OBJECT(gObjectSources + 659, 0x1, 0, 1, 15, 8, 102),
	OBJECT(gObjectSources + 660, 0x1, 0, 1, 15, 8, 103),
	OBJECT(gObjectSources + 661, 0x1, 0, 1, 15, 8, 104),
	OBJECT(gObjectSources + 662, 0x1, 0, 1, 15, 8, 105),
	OBJECT(gObjectSources + 663, 0x1, 0, 1, 15, 8, 106),
	OBJECT(gObjectSources + 664, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 665, 0x1, 0, 0, 0, 8, 107),
	OBJECT(gObjectSources + 666, 0x1, 0, 1, 15, 8, 108),
	OBJECT(gObjectSources + 667, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 668, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 669, 0x81, 0, 0, 0, 22, 8),
	OBJECT(gObjectSources + 670, 0x81, 0, 0, 0, 5, 8),
	OBJECT(gObjectSources + 671, 0x81, 0, 0, 0, 22, 0),
	// Object IDs 672-703
	OBJECT(gObjectSources + 672, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 673, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 674, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 675, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 676, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 677, 0x1, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 678, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 679, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 680, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 681, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 682, 0x81, 0, 0, 0, 5, 8),
	OBJECT(gObjectSources + 683, 0x1, 0, 1, 15, 8, 109),
	OBJECT(gObjectSources + 684, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 685, 0x1, 0, 1, 15, 8, 110),
	OBJECT(gObjectSources + 686, 0x1, 0, 0, 0, 8, 111),
	OBJECT(gObjectSources + 687, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 688, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 689, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 690, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 691, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 692, 0x201, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 693, 0x1, 0, 1, 15, 8, 112),
	OBJECT(gObjectSources + 694, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 695, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 696, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 697, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 698, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 699, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 700, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 701, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 702, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 703, 0x1, 0, 1, 15, 8, 97),
	// Object IDs 704-735
	OBJECT(gObjectSources + 704, 0x1, 0, 1, 15, 8, 113),
	OBJECT(gObjectSources + 705, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 706, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 707, 0x81, 0, 0, 0, 5, 19),
	OBJECT(gObjectSources + 708, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 709, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 710, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 711, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 712, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 713, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 714, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 715, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 716, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 717, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 718, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 719, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 720, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 721, 0x1001, 0, 0, 0, 20, 36),
	OBJECT(gObjectSources + 722, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 723, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 724, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 725, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 726, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 727, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 728, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 729, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 730, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 731, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 732, 0x1, 0, 0, 0, 10, 0),
	OBJECT(gObjectSources + 733, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 734, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 735, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 736-767
	OBJECT(gObjectSources + 736, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 737, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 738, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 739, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 740, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 741, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 742, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 743, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 744, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 745, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 746, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 747, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 748, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 749, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 750, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 751, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 752, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 753, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 754, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 755, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 756, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 757, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 758, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 759, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 760, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 761, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 762, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 764, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 764, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 768-799
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 764, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 764, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 764, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 763, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 776, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 777, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 778, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 779, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 780, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 781, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 782, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 783, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 784, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 785, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 786, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 787, 0x201, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 788, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 789, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 790, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 791, 0x81, 0, 0, 0, 10, 0),
	OBJECT(gObjectSources + 792, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 793, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 794, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 795, 0x1, 0, 0, 0, 5, 9),
	OBJECT(gObjectSources + 796, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 797, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 798, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 799, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 800-831
	OBJECT(gObjectSources + 800, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 801, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 802, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 803, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 682, 0x1, 0, 1, 15, 8, 114),
	OBJECT(gObjectSources + 805, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 806, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 807, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 808, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 809, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 810, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 811, 0x1, 0, 1, 15, 8, 115),
	OBJECT(gObjectSources + 812, 0x1, 0, 1, 15, 8, 116),
	OBJECT(gObjectSources + 813, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 814, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 815, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 816, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 817, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 818, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 819, 0x1, 0, 1, 15, 8, 117),
	OBJECT(gObjectSources + 820, 0x1, 0, 1, 15, 8, 118),
	OBJECT(gObjectSources + 821, 0x1, 0, 1, 15, 8, 119),
	OBJECT(gObjectSources + 822, 0x1, 0, 1, 15, 8, 120),
	OBJECT(gObjectSources + 823, 0x1, 0, 1, 15, 8, 121),
	OBJECT(gObjectSources + 824, 0x1, 0, 1, 15, 8, 122),
	OBJECT(gObjectSources + 825, 0x1, 0, 1, 15, 8, 123),
	OBJECT(gObjectSources + 826, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 827, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 828, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 829, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 830, 0x1, 0, 1, 15, 8, 124),
	OBJECT(gObjectSources + 831, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 832-863
	OBJECT(gObjectSources + 832, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 833, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 834, 0x1, 0, 1, 15, 8, 125),
	OBJECT(gObjectSources + 835, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 836, 0x1, 0, 1, 15, 8, 126),
	OBJECT(gObjectSources + 837, 0x1, 0, 1, 15, 8, 127),
	OBJECT(gObjectSources + 838, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 839, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 840, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 841, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 842, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 843, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 844, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 845, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 846, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 847, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 848, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 849, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 850, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 851, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 852, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 853, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 854, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 855, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 856, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 857, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 858, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 859, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 860, 0x1, 0, 1, 15, 8, 128),
	OBJECT(gObjectSources + 861, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 862, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 863, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 864-895
	OBJECT(gObjectSources + 864, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 865, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 866, 0x1, 0, 1, 15, 8, 130),
	OBJECT(gObjectSources + 867, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 868, 0x1, 0, 1, 15, 8, 129),
	OBJECT(gObjectSources + 869, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 870, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 871, 0x1, 0, 1, 15, 8, 131),
	OBJECT(gObjectSources + 872, 0x1, 0, 1, 15, 8, 132),
	OBJECT(gObjectSources + 873, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 874, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 875, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 876, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 877, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 878, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 879, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 880, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 881, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 882, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 883, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 884, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 885, 0x1, 0, 1, 15, 8, 133),
	OBJECT(gObjectSources + 886, 0x81, 0, 0, 0, 15, 0),
	OBJECT(gObjectSources + 887, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 888, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 889, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 890, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 891, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 892, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 893, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 894, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 895, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 896-927
	OBJECT(gObjectSources + 896, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 897, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 898, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 899, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 900, 0x181, 0, 0, 0, 19, 0),
	OBJECT(gObjectSources + 901, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 902, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 903, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 904, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 905, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 906, 0x1, 0, 0, 0, 20, 34),
	OBJECT(gObjectSources + 907, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 908, 0x401, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 909, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 467, 0x81, 0, 1, 15, 8, 134),
	OBJECT(gObjectSources + 911, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 912, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 913, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 914, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 915, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 916, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 917, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 918, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 919, 0x1, 0, 1, 15, 8, 135),
	OBJECT(gObjectSources + 920, 0x1, 0, 1, 15, 8, 136),
	OBJECT(gObjectSources + 921, 0x1, 0, 1, 15, 8, 137),
	OBJECT(gObjectSources + 922, 0x1, 0, 1, 15, 8, 138),
	OBJECT(gObjectSources + 923, 0x1, 0, 1, 15, 8, 139),
	OBJECT(gObjectSources + 924, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 925, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 926, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 927, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 928-959
	OBJECT(gObjectSources + 928, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 929, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 930, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 931, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 932, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 933, 0x1, 0, 1, 15, 8, 140),
	OBJECT(gObjectSources + 934, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 935, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 936, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 937, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 938, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 939, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 940, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 941, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 942, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 943, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 944, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 945, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 946, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 947, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 948, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 949, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 950, 0x1, 0, 1, 15, 8, 141),
	OBJECT(gObjectSources + 950, 0x1, 0, 1, 15, 8, 142),
	OBJECT(gObjectSources + 950, 0x1, 0, 1, 15, 8, 143),
	OBJECT(gObjectSources + 950, 0x1, 0, 1, 15, 8, 144),
	OBJECT(gObjectSources + 950, 0x1, 0, 1, 15, 8, 145),
	OBJECT(gObjectSources + 955, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 956, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 957, 0x1, 0, 0, 0, 5, 12),
	OBJECT(gObjectSources + 958, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 959, 0x1, 0, 0, 0, 5, 8),
	// Object IDs 960-991
	OBJECT(gObjectSources + 343, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 961, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 962, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 963, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 964, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 965, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 966, 0x1, 0, 0, 0, 20, 18),
	OBJECT(gObjectSources + 967, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 968, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 969, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 970, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 971, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 972, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 973, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 974, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 975, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 976, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 977, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 978, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 979, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 980, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 981, 0x1, 0, 0, 0, 5, 11),
	OBJECT(gObjectSources + 982, 0x1, 0, 1, 15, 8, 146),
	OBJECT(gObjectSources + 983, 0x1, 0, 1, 15, 8, 147),
	OBJECT(gObjectSources + 984, 0x1, 0, 1, 15, 8, 148),
	OBJECT(gObjectSources + 985, 0x1, 0, 1, 15, 8, 149),
	OBJECT(gObjectSources + 986, 0x1, 0, 1, 15, 8, 150),
	OBJECT(gObjectSources + 987, 0x1, 0, 1, 15, 8, 151),
	OBJECT(gObjectSources + 988, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 989, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 990, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 991, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 992-1023
	OBJECT(gObjectSources + 992, 0x1, 0, 0, 0, 5, 13),
	OBJECT(gObjectSources + 993, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 994, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 995, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 996, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 997, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 998, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 999, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1000, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1001, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1002, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1003, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1004, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1005, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1006, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1007, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1008, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1009, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1010, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1011, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1012, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1013, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1014, 0x3, 0, 0, 0, 3, 0),
	OBJECT(gObjectSources + 1015, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1016, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1017, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1018, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1019, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1020, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1021, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1022, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 1023, 0x1, 0, 0, 0, 5, 3),
	// Object IDs 1024-1055
	OBJECT(gObjectSources + 1024, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1025, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1026, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1027, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1028, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1029, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1030, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1031, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1032, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1033, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1034, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1035, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1036, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1037, 0x1, 0, 0, 0, 5, 16),
	OBJECT(gObjectSources + 1038, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1039, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 1040, 0x1, 0, 0, 0, 5, 3),
	OBJECT(gObjectSources + 1041, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 1042, 0x1, 0, 0, 0, 5, 3),
	OBJECT(gObjectSources + 1043, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 1044, 0x1, 0, 0, 0, 5, 3),
	OBJECT(gObjectSources + 647, 0x1, 0, 1, 15, 8, 152),
	OBJECT(gObjectSources + 1046, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1047, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1048, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1049, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1050, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1051, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1052, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1053, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1054, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1055, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 1056-1087
	OBJECT(gObjectSources + 1056, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1057, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1058, 0x1, 0, 1, 15, 8, 153),
	OBJECT(gObjectSources + 1059, 0x1, 0, 0, 0, 17, 0),
	OBJECT(gObjectSources + 1060, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1061, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1062, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1063, 0x1, 0, 1, 15, 8, 154),
	OBJECT(gObjectSources + 1064, 0x1, 0, 1, 15, 8, 155),
	OBJECT(gObjectSources + 1065, 0x81, 0, 0, 0, 5, 0),
	OBJECT(gObjectSources + 1066, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1067, 0x1, 0, 0, 0, 5, 14),
	OBJECT(gObjectSources + 1068, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1069, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1070, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1071, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1072, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1073, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1074, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1075, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1076, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1077, 0x1, 0, 0, 0, 5, 2),
	OBJECT(gObjectSources + 1078, 0x1, 0, 0, 0, 5, 3),
	OBJECT(gObjectSources + 1079, 0x1, 0, 0, 0, 5, 4),
	OBJECT(gObjectSources + 1080, 0x3, 0, 0, 0, 5, 18),
	OBJECT(gObjectSources + 1081, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1082, 0x1, 0, 0, 0, 5, 15),
	OBJECT(gObjectSources + 1083, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1084, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1085, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1086, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1087, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 1088-1119
	OBJECT(gObjectSources + 1088, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1089, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1090, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1091, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1092, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1093, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1094, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1095, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1096, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1097, 0x1, 0, 0, 0, 0, 0),
	OBJECT(gObjectSources + 1098, 0x1, 0, 1, 15, 8, 156),
	OBJECT(gObjectSources + 1099, 0x1, 0, 1, 15, 8, 157),
	OBJECT(gObjectSources + 1100, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1101, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1102, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1103, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1104, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1105, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 274, 0x1, 0, 0, 0, 5, 0),
	OBJECT(gObjectSources + 1107, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1108, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1109, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1110, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1111, 0x1001, 0, 0, 0, 20, 50),
	OBJECT(gObjectSources + 1112, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1113, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1114, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1115, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1116, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1117, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1118, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1119, 0x801, 0, 0, 0, 22, 0),
	// Object IDs 1120-1151
	OBJECT(gObjectSources + 1120, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1121, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1122, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1123, 0x801, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1124, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1125, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1126, 0x81, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1127, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1128, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1129, 0x1, 0, 0, 0, 8, 158),
	OBJECT(gObjectSources + 1130, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1131, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1132, 0x3, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1133, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1134, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1135, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1136, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1137, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1138, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1139, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1140, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1141, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1142, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1143, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1144, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1145, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1146, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1147, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1148, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1149, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1150, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1151, 0x1, 0, 0, 0, 22, 0),
	// Object IDs 1152-1161
	OBJECT(gObjectSources + 1152, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1153, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1154, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1155, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1156, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1157, 0x81, 0, 0, 0, 10, 90),
	OBJECT(gObjectSources + 1158, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1159, 0x1, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1160, 0x4001, 0, 0, 0, 22, 0),
	OBJECT(gObjectSources + 1161, 0x1, 0, 0, 0, 22, 0),
};
}
