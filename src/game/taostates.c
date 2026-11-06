#include "PowerPC_EABI_Support/Runtime/ptmf.h"

struct XObjActorTao;
extern void StateNone__12XObjActorTaoFUl(struct XObjActorTao* self, unsigned long frames);
extern void StatePickUp__12XObjActorTaoFUl(struct XObjActorTao* self, unsigned long frames);

PTMF sTaoIdleState   = { 0, -1, { (void*)StateNone__12XObjActorTaoFUl } };
PTMF sTaoPickupState = { 0, -1, { (void*)StatePickUp__12XObjActorTaoFUl } };

struct TaoAttachmentConstants {
	float vectors[2][3][3];
	long mouthPath[6];
	long kind2A3Path[5];
	long kind46FPath[3];
	long extraObjectPath[3];
};

const struct TaoAttachmentConstants sTaoAttachmentConstants = {
	{ { { 1, 1, 1 } }, { { 1, 1, 1 } } }, { 0 }, { 0 }, { 0 }, { 0 },
};

const long sTaoPickupJointPath[6] = { 0 };

const char sTypeNameXObjActorTao[16]      = "XObjActorTao";
const char sTaoIntpTypeName[12]           = "CXObjIntp";
const char sTaoActorTypeName[12]          = "XObjActor";
const char sTaoEulerRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sTaoTranslationCondition[12]   = "translate";
const char sTaoStatusTypeName[12]         = "CStatus64";
const char sTaoCoordInfoTypeName[12]      = "CoordInfo";
const char sTaoAttrInfoTypeName[12]       = "AttrInfo";
const char sTaoEventDataTypeName[12]      = "CEvtData";
const char sTaoAnimQueueTypeName[10]      = "CAnmQueue";
