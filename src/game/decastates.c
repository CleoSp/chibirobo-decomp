#include "PowerPC_EABI_Support/Runtime/ptmf.h"
#include "game/decacamerapath.h"

struct XObjActorDeca;
extern void StateHold__13XObjActorDecaFUl(struct XObjActorDeca* self, unsigned long frames);
extern void StatePickUp__13XObjActorDecaFUl(struct XObjActorDeca* self, unsigned long frames);

PTMF sDecaHoldState   = { 0, -1, { (void*)StateHold__13XObjActorDecaFUl } };
PTMF sDecaPickupState = { 0, -1, { (void*)StatePickUp__13XObjActorDecaFUl } };

const long sDecaPlayerMountJointPath[2]   = { 0, 3 };
const DecaJointPath2 sDecaHoldJointPath   = { { 0, 5 } };
const long sDecaPickupJointPath[2]        = { 0, 5 };
const DecaJointPath2 sDecaCameraJointPath = { { 0, 3 } };

struct DecaJointConstants {
	float vectors[2][3][3];
	long firstHidePath[3];
	long secondHidePath[4];
	long firstShowPath[3];
	long secondShowPath[4];
	long headRotationPath[5];
};

const struct DecaJointConstants sDecaJointConstants = {
	{ { { 1, 1, 1 } }, { { 1, 1, 1 } } }, { 0 }, { 0 }, { 0 }, { 0 }, { 0, 1, 0, 0, 0 },
};

const long sDecaPlayerReleaseJointPath[5] = { 0, 6, 0, 0, 1 };

const char sTypeNameXObjActorDeca[16]      = "XObjActorDeca";
const char sDecaIntpTypeName[12]           = "CXObjIntp";
const char sDecaActorTypeName[12]          = "XObjActor";
const char sDecaEulerRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sDecaTranslationCondition[12]   = "translate";
const char sDecaStatusTypeName[12]         = "CStatus64";
const char sDecaCoordInfoTypeName[12]      = "CoordInfo";
const char sDecaAttrInfoTypeName[12]       = "AttrInfo";
const char sDecaEventDataTypeName[12]      = "CEvtData";
const char sDecaAnimQueueTypeName[10]      = "CAnmQueue";
