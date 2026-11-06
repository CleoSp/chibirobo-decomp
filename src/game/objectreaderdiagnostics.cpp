#include "Dolphin/vec.h"
#include "game/readertypeinfo.h"

extern "C" {
extern const Vec sObjectPoolTransformDefaults[2][3] = {
	{ { 1, 1, 1 }, {}, {} },
	{ { 1, 1, 1 }, {}, {} },
};

extern const char sTypeNameXObjReadAnm[]                   = "XObjReadAnm";
extern const char sTypeNameXObjRead[]                      = "XObjRead";
extern const char sPathXobjsCpp[]                          = "xobjs.cpp";
extern const char sObjectReaderFullMessage[]               = "no more space for XObjRead thread!\n";
extern const char sObjectPoolCapacityFormat[]              = "%s:: cnt=%d, max=%d\n";
extern const char sLetickerPoolName[]                      = "LETICKER";
extern const char sActorToyPoolName[]                      = "ACTOR_TOY";
extern const char sActorJennyPoolName[]                    = "ACTOR_JENNY";
extern const char sActorMamaPoolName[]                     = "ACTOR_MAMA";
extern const char sActorPapaPoolName[]                     = "ACTOR_PAPA";
extern const char sActorTaoPoolName[]                      = "ACTOR_TAO";
extern const char sActorDecaPoolName[]                     = "ACTOR_DECA";
extern const char sSpiderSmallPoolName[]                   = "SPIDER_SMALL";
extern const char sSpiderBossPoolName[]                    = "SPIDER_BOSS";
extern const char sReaderTypeNameGXColor[]                 = "_GXColor";
extern const char sReaderTypeNameCInterfaceInterpolation[] = "CInterfaceInterpolation";
extern const char sReaderTypeNameCScrnColor[]              = "CScrnColor";
extern const char sReaderTypeNameXActLift[]                = "XActLift";
extern const char sReaderTypeNameXActHang[]                = "XActHang";
extern const char sReaderTypeNameXActJump[]                = "XActJump";
extern const char sReaderTypeNameXActLadder[]              = "XActLadder";
extern const char sReaderTypeNameXActRope[]                = "XActRope";
extern const char sReaderTypeNameXActPush[]                = "XActPush";
extern const char sReaderTypeNameXActTumble[]              = "XActTumble";
extern const char sReaderTypeNameXActCopter[]              = "XActCopter";
extern const char sReaderTypeNameXActPopper[]              = "XActPopper";
extern const char sReaderTypeNameXActRadar[]               = "XActRadar";
extern const char sReaderTypeNameCJointRot[]               = "CJointRot";
extern const char sReaderTypeNamePARAM_CHR[]               = "PARAM_CHR";
extern const char sReaderJObjRotationAssertion[]           = "!(jobj->flags & JOBJ_USE_QUATERNION)";
extern const char sReaderTranslateName[]                   = "translate";
extern const char sReaderTypeNameCStatus64[]               = "CStatus64";
extern const char sReaderTypeNameCEvtData[]                = "CEvtData";
extern const char sReaderTypeNameCAnmQueue[]               = "CAnmQueue";
extern const char sReaderTypeNameCPtclOne[]                = "CPtclOne";
extern const char sReaderTypeNameGXTexObj[]                = "_GXTexObj";
extern const char sReaderTypeNameXObjFlags[]               = "XObjFlags";
extern const char sReaderTypeNameCXObjIntp[]               = "CXObjIntp";
extern const char sReaderTypeNameAttrInfo[]                = "AttrInfo";
extern const char sReaderTypeNameCoordInfo[]               = "CoordInfo";
extern const char sTypeNameCMsgQueue[]                     = "CMsgQueue";
extern const char sTypeNameXObjReadOne[]                   = "XObjReadOne";
extern const char sReaderTypeNameXObjReadAll[]             = "XObjReadAll";
char sObjectReadStartName[12]                              = "ReadStart";
char sObjectReadAllName[]                                  = "ReadAll";
extern const char sReaderTypeNameCThread[]                 = "CThread";
extern const char sObjectReaderErrorFormat[]               = "%s%s";
extern const char sCameraPoolName[]                        = "CAMERA";
extern const char sLightPoolName[]                         = "LIGHT";
extern const char sMapPoolName[]                           = "MAP";
extern const char sChrPoolName[]                           = "CHR";
extern const char sActor2PoolName[]                        = "ACTOR2";
extern const char sActor3PoolName[]                        = "ACTOR3";
extern const char sSoundPoolName[]                         = "SOUND";
extern const char sCoinPoolName[]                          = "COIN";
extern const char sItemPoolName[]                          = "ITEM";
extern const char sArmyPoolName[]                          = "ARMY";
extern const char sSpiderPoolName[]                        = "SPIDER";
extern const char sDustPoolName[]                          = "DUST";
extern const char sHBoxPoolName[]                          = "HBOX";
extern const char sFreePoolName[]                          = "FREE";
extern const float gScreenColorZero                        = 0.0f;
extern const char sReaderTypeNameXAction[]                 = "XAction";
extern const char sReaderTypeNameCPadOne[]                 = "CPadOne";
extern const char sReaderTypeNameVec[]                     = "Vec";
extern const char sReaderTypeNameCVec[]                    = "CVec";
extern const char sReaderTypeNameCJObj[]                   = "CJObj";
extern const char sReaderTypeNameCXObj[]                   = "CXObj";
extern const char sReaderTypeNameXObj[]                    = "XObj";
}

extern "C" {

extern ReaderBaseList<1> sReaderBaseTypesCVec;
extern ReaderBaseList<1> sReaderBaseTypesCXObj;
extern ReaderBaseList<2> sReaderBaseTypesCXObjIntp;
extern ReaderBaseList<3> sReaderBaseTypesXObj;
extern const unsigned char sBaseTypesXObjReadOne[];
extern const unsigned char sBaseTypesXObjReadAll[];

ReaderTypeInfo __RTTI__7CThread         = { sReaderTypeNameCThread, 0 };
ReaderBaseList<1> sBaseTypesXObjRead    = { { { &__RTTI__7CThread, 0 } }, 0 };
ReaderTypeInfo __RTTI__8XObjRead        = { sTypeNameXObjRead, &sBaseTypesXObjRead };
ReaderBaseList<2> sBaseTypesXObjReadAnm = { { { &__RTTI__7CThread, 0 }, { &__RTTI__8XObjRead, 0 } }, 0 };
ReaderTypeInfo __RTTI__11XObjReadAnm    = { sTypeNameXObjReadAnm, &sBaseTypesXObjReadAnm };
ReaderTypeInfo gReaderTypeInfoGXColor   = { sReaderTypeNameGXColor, 0 };
ReaderTypeInfo gReaderTypeInfoXAction   = { sReaderTypeNameXAction, 0 };
ReaderTypeInfo gReaderTypeInfoPARAM_CHR = { sReaderTypeNamePARAM_CHR, 0 };
ReaderTypeInfo gReaderTypeInfoCPadOne   = { sReaderTypeNameCPadOne, 0 };
ReaderTypeInfo gReaderTypeInfoVec       = { sReaderTypeNameVec, 0 };
ReaderTypeInfo gReaderTypeInfoCVec      = { sReaderTypeNameCVec, &sReaderBaseTypesCVec };
ReaderTypeInfo gReaderTypeInfoCJObj     = { sReaderTypeNameCJObj, 0 };
ReaderTypeInfo gReaderTypeInfoGXTexObj  = { sReaderTypeNameGXTexObj, 0 };
ReaderTypeInfo gReaderTypeInfoCXObj     = { sReaderTypeNameCXObj, &sReaderBaseTypesCXObj };
ReaderTypeInfo gReaderTypeInfoCXObjIntp = { sReaderTypeNameCXObjIntp, &sReaderBaseTypesCXObjIntp };
ReaderTypeInfo gReaderTypeInfoXObj      = { sReaderTypeNameXObj, &sReaderBaseTypesXObj };
ReaderTypeInfo __RTTI__9CMsgQueue       = { sTypeNameCMsgQueue, 0 };
ReaderTypeInfo __RTTI__11XObjReadOne    = { sTypeNameXObjReadOne, sBaseTypesXObjReadOne };
ReaderTypeInfo __RTTI__11XObjReadAll    = { sReaderTypeNameXObjReadAll, sBaseTypesXObjReadAll };
}
