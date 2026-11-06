#include "PowerPC_EABI_Support/Runtime/ptmf.h"

struct XObjActorPapa;
extern void StateNone__13XObjActorPapaFUl(struct XObjActorPapa* self, unsigned long frames);
extern void StatePickUp__13XObjActorPapaFUl(struct XObjActorPapa* self, unsigned long frames);

PTMF sPapaIdleState   = { 0, -1, { (void*)StateNone__13XObjActorPapaFUl } };
PTMF sPapaPickupState = { 0, -1, { (void*)StatePickUp__13XObjActorPapaFUl } };

const char sTypeNameXObjActorPapa[16]      = "XObjActorPapa";
const char sPapaIntpTypeName[12]           = "CXObjIntp";
const char sPapaActorTypeName[12]          = "XObjActor";
const char sPapaEulerRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sPapaTranslationCondition[12]   = "translate";
const char sPapaStatusTypeName[12]         = "CStatus64";
const char sPapaCoordInfoTypeName[12]      = "CoordInfo";
const char sPapaAttrInfoTypeName[12]       = "AttrInfo";
const char sPapaEventDataTypeName[12]      = "CEvtData";
const char sPapaAnimQueueTypeName[10]      = "CAnmQueue";
