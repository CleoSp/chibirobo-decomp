extern const void* sPlayerTypeInfoVec[];
extern const void* sPlayerTypeInfoOctaMenu[];
extern const void* gTypeInfoAtcMenu[];

extern void ConfirmSelection__7AtcMenuFv(void);
extern void ConfirmSelection__8OctaMenuFv(void);
extern void GetItemColor__7AtcMenuFl(void);
extern void GetItemColor__8OctaMenuFl(void);
extern void GetItemIconTextureIndex__7AtcMenuFl(void);
extern void GetItemIconTextureIndex__8OctaMenuFl(void);
extern void GetItemLabel__7AtcMenuFl(void);
extern void GetItemLabel__8OctaMenuFl(void);
extern void Open__7AtcMenuFl(void);
extern void Open__8OctaMenuFl(void);
extern void Select__8OctaMenuFl(void);
extern void __dt__7AtcMenuFv(void);
extern void __dt__8OctaMenuFv(void);
extern void vf14__8OctaMenuFv(void);
extern void vf18__8OctaMenuFv(void);
extern void vf2C__7AtcMenuFl(void);
extern void vf2C__8OctaMenuFl(void);

const void* sPlayerBaseTypesCVec[] = { sPlayerTypeInfoVec, 0, 0, 0, 0, 0 };
const void* sBaseTypesAtcMenu[]    = { sPlayerTypeInfoOctaMenu, 0, 0 };
const void* __vt__7AtcMenu[]       = { gTypeInfoAtcMenu,
	                                   0,
	                                   __dt__7AtcMenuFv,
	                                   Open__7AtcMenuFl,
	                                   Select__8OctaMenuFl,
	                                   vf14__8OctaMenuFv,
	                                   vf18__8OctaMenuFv,
	                                   ConfirmSelection__7AtcMenuFv,
	                                   GetItemIconTextureIndex__7AtcMenuFl,
	                                   GetItemLabel__7AtcMenuFl,
	                                   GetItemColor__7AtcMenuFl,
	                                   vf2C__7AtcMenuFl };
const void* __vt__8OctaMenu[]      = { sPlayerTypeInfoOctaMenu,
	                                   0,
	                                   __dt__8OctaMenuFv,
	                                   Open__8OctaMenuFl,
	                                   Select__8OctaMenuFl,
	                                   vf14__8OctaMenuFv,
	                                   vf18__8OctaMenuFv,
	                                   ConfirmSelection__8OctaMenuFv,
	                                   GetItemIconTextureIndex__8OctaMenuFl,
	                                   GetItemLabel__8OctaMenuFl,
	                                   GetItemColor__8OctaMenuFl,
	                                   vf2C__8OctaMenuFl };

const char sTypeNameXAction[] = "XAction";

extern const char sTypeNameOctaMenu[];
extern const char sTypeNameDrsMenu[];
extern const char sPlayerTypeNameCJObj[];
extern const char sPlayerTypeNameGXTexObj[];
extern const char sPlayerTypeNameVec[];
extern const char sPlayerTypeNameCVec[];
extern const char sTypeNameAtcMenu[];
extern const char sPlayerTypeNameCXObj[];
extern const char sPlayerTypeNameCXObjIntp[];
extern const char sPlayerTypeNameXObj[];
extern const char sTypeNameXObjChr[];
extern const char sTypeNameXActLift[];
extern const char sTypeNameXActHang[];
extern const char sTypeNameXActJump[];
extern const char sTypeNameXActLadder[];
extern const char sTypeNameXActRope[];
extern const char sTypeNameXActPush[];
extern const char sTypeNameXActTumble[];
extern const char sTypeNameXActCopter[];
extern const char sTypeNameXActPopper[];
extern const char sTypeNameXActRadar[];
extern const char sTypeNameCJointRot[];
extern const char sPlayerParameterName[];
extern const char sTypeNameParams[];
extern const char sPlayerTypeNameCPadOne[];
extern const char sTypeNameXPad[];
extern const char sTypeNameOXMenu[];
extern const char sPlayerTypeNameCPtcl[];
extern const char sAttachmentTypeNameCJObj[];
extern const char sAttachmentTypeNameCXObj[];
extern const char sAttachmentIntpTypeName[];
extern const char sAttachmentTypeNameXObj[];
extern const char sTypeNameXObjAttachment[];
extern const char sTypeNameXObjOX[];
extern const char sTypeNameXObjRadar[];
extern const char sTypeNameXObjPopper[];
extern const char sTypeNameXObjCopter[];
extern const char sPlugTypeNameCJObj[];
extern const char sPlugTypeNameCXObj[];
extern const char sPlugTypeNameCXObjIntp[];
extern const char sPlugTypeNameXObj[];
extern const char sTypeNameXObjWireEff[];
extern const char sTypeNameXObjPlug[];

extern const void* sPlayerBaseTypesDrsMenu[];
extern const void* sPlayerBaseTypesCVec[];
extern const void* sBaseTypesAtcMenu[];
extern const void* sPlayerBaseTypesCXObj[];
extern const void* sPlayerBaseTypesCXObjIntp[];
extern const void* sPlayerBaseTypesXObj[];
extern const void* sBaseTypesXObjChr[];
extern const void* sBaseTypesXActLift[];
extern const void* sBaseTypesXActHang[];
extern const void* sBaseTypesXActJump[];
extern const void* sBaseTypesXActLadder[];
extern const void* sBaseTypesXActRope[];
extern const void* sBaseTypesXActPush[];
extern const void* sBaseTypesXActTumble[];
extern const void* sBaseTypesXActCopter[];
extern const void* sBaseTypesXActPopper[];
extern const void* sBaseTypesXActRadar[];
extern const void* sBaseTypesParams[];
extern const void* sBaseTypesXPad[];
extern const void* sAttachmentBaseTypesCXObj[];
extern const void* sAttachmentBaseTypesCXObjIntp[];
extern const void* sAttachmentBaseTypesXObj[];
extern const void* sBaseTypesXObjAttachment[];
extern const void* sBaseTypesXObjOX[];
extern const void* sBaseTypesXObjRadar[];
extern const void* sBaseTypesXObjPopper[];
extern const void* sBaseTypesXObjCopter[];
extern const void* sPlugBaseTypesCXObj[];
extern const void* sPlugBaseTypesCXObjIntp[];
extern const void* sPlugBaseTypesXObj[];
extern const void* sBaseTypesXObjWireEff[];
extern const void* sBaseTypesXObjPlug[];

const void* sPlayerTypeInfoOctaMenu[]      = { sTypeNameOctaMenu, 0 };
const void* sPlayerTypeInfoDrsMenu[]       = { sTypeNameDrsMenu, sPlayerBaseTypesDrsMenu };
const void* sPlayerTypeInfoCJObj[]         = { sPlayerTypeNameCJObj, 0 };
const void* sPlayerTypeInfoGXTexObj[]      = { sPlayerTypeNameGXTexObj, 0 };
const void* sPlayerTypeInfoVec[]           = { sPlayerTypeNameVec, 0 };
const void* sPlayerTypeInfoCVec[]          = { sPlayerTypeNameCVec, sPlayerBaseTypesCVec };
const void* gTypeInfoAtcMenu[]             = { sTypeNameAtcMenu, sBaseTypesAtcMenu };
const void* sPlayerTypeInfoCXObj[]         = { sPlayerTypeNameCXObj, sPlayerBaseTypesCXObj };
const void* sPlayerTypeInfoCXObjIntp[]     = { sPlayerTypeNameCXObjIntp, sPlayerBaseTypesCXObjIntp };
const void* sPlayerTypeInfoXObj[]          = { sPlayerTypeNameXObj, sPlayerBaseTypesXObj };
const void* __RTTI__7XObjChr[]             = { sTypeNameXObjChr, sBaseTypesXObjChr };
const void* __RTTI__7XAction[]             = { sTypeNameXAction, 0 };
const void* __RTTI__8XActLift[]            = { sTypeNameXActLift, sBaseTypesXActLift };
const void* __RTTI__8XActHang[]            = { sTypeNameXActHang, sBaseTypesXActHang };
const void* __RTTI__8XActJump[]            = { sTypeNameXActJump, sBaseTypesXActJump };
const void* __RTTI__10XActLadder[]         = { sTypeNameXActLadder, sBaseTypesXActLadder };
const void* __RTTI__8XActRope[]            = { sTypeNameXActRope, sBaseTypesXActRope };
const void* __RTTI__8XActPush[]            = { sTypeNameXActPush, sBaseTypesXActPush };
const void* __RTTI__10XActTumble[]         = { sTypeNameXActTumble, sBaseTypesXActTumble };
const void* __RTTI__10XActCopter[]         = { sTypeNameXActCopter, sBaseTypesXActCopter };
const void* __RTTI__10XActPopper[]         = { sTypeNameXActPopper, sBaseTypesXActPopper };
const void* __RTTI__9XActRadar[]           = { sTypeNameXActRadar, sBaseTypesXActRadar };
const void* __RTTI__9CJointRot[]           = { sTypeNameCJointRot, 0 };
const void* sPlayerParameterTypeInfo[]     = { sPlayerParameterName, 0 };
const void* gTypeInfoParams[]              = { sTypeNameParams, sBaseTypesParams };
const void* sPlayerTypeInfoCPadOne[]       = { sPlayerTypeNameCPadOne, 0 };
const void* gTypeInfoXPad[]                = { sTypeNameXPad, sBaseTypesXPad };
const void* gTypeInfoOXMenu[]              = { sTypeNameOXMenu, 0 };
const void* gTypeInfoCPtcl[]               = { sPlayerTypeNameCPtcl, 0 };
const void* sAttachmentTypeInfoCJObj[]     = { sAttachmentTypeNameCJObj, 0 };
const void* sAttachmentTypeInfoCXObj[]     = { sAttachmentTypeNameCXObj, sAttachmentBaseTypesCXObj };
const void* sAttachmentTypeInfoCXObjIntp[] = { sAttachmentIntpTypeName, sAttachmentBaseTypesCXObjIntp };
const void* sAttachmentTypeInfoXObj[]      = { sAttachmentTypeNameXObj, sAttachmentBaseTypesXObj };
const void* gTypeInfoXObjAttachment[]      = { sTypeNameXObjAttachment, sBaseTypesXObjAttachment };
const void* gTypeInfoXObjOX[]              = { sTypeNameXObjOX, sBaseTypesXObjOX };
const void* gTypeInfoXObjRadar[]           = { sTypeNameXObjRadar, sBaseTypesXObjRadar };
const void* gTypeInfoXObjPopper[]          = { sTypeNameXObjPopper, sBaseTypesXObjPopper };
const void* gTypeInfoXObjCopter[]          = { sTypeNameXObjCopter, sBaseTypesXObjCopter };
const void* sPlugTypeInfoCJObj[]           = { sPlugTypeNameCJObj, 0 };
const void* sPlugTypeInfoCXObj[]           = { sPlugTypeNameCXObj, sPlugBaseTypesCXObj };
const void* sPlugTypeInfoCXObjIntp[]       = { sPlugTypeNameCXObjIntp, sPlugBaseTypesCXObjIntp };
const void* sPlugTypeInfoXObj[]            = { sPlugTypeNameXObj, sPlugBaseTypesXObj };
const void* gTypeInfoXObjWireEff[]         = { sTypeNameXObjWireEff, sBaseTypesXObjWireEff };
const void* gTypeInfoXObjPlug[]            = { sTypeNameXObjPlug, sBaseTypesXObjPlug };

const void* sPlayerBaseTypesCXObj[]     = { sPlayerTypeInfoCJObj, 0, 0 };
const void* sPlayerBaseTypesCXObjIntp[] = { sPlayerTypeInfoCJObj, 0, sPlayerTypeInfoCXObj, 0, 0 };
const void* sPlayerBaseTypesXObj[]      = { sPlayerTypeInfoCJObj, 0, sPlayerTypeInfoCXObj, 0, sPlayerTypeInfoCXObjIntp, 0, 0 };
const void* sBaseTypesXObjChr[]
    = { sPlayerTypeInfoCJObj, 0, sPlayerTypeInfoCXObj, 0, sPlayerTypeInfoCXObjIntp, 0, sPlayerTypeInfoXObj, 0, 0 };
