extern "C" const char sAttachmentEulerCondition[], sAttachmentTranslationCondition[];
#define JOBJ_EULER_CONDITION       sAttachmentEulerCondition
#define JOBJ_TRANSLATION_CONDITION sAttachmentTranslationCondition
#include "game/xobjattachment.h"
#include "game/cjobjinlineattach.h"

extern "C" {

extern const Vec sAttachmentTransformDefaults[2][3] = {
	{ { 1, 1, 1 }, {}, {} },
	{ { 1, 1, 1 }, {}, {} },
};

struct RadarJointPath {
	s32 nodes[4];
};
extern const RadarJointPath sRadarPlacementPath     = {};
extern const RadarJointPath sRadarActivationPath    = {};
extern const RadarJointPath sRadarWorldPositionPath = {};
extern const RadarJointPath sRadarPitchPath         = {};
extern const char sAttachmentIntpTypeName[]         = "CXObjIntp";
extern const char sTypeNameXObjAttachment[]         = "XObjAttachment";
extern const char sAttachmentEulerCondition[]       = "!(jobj->flags & JOBJ_USE_QUATERNION)";
extern const char sAttachmentTranslationCondition[] = "translate";
extern const char sAttachmentCStatus64TypeName[]    = "CStatus64";
extern const char sAttachmentCoordInfoTypeName[]    = "CoordInfo";
extern const char sAttachmentAttrInfoTypeName[]     = "AttrInfo";
extern const char sAttachmentCEvtDataTypeName[]     = "CEvtData";
extern const char sAttachmentCAnmQueueTypeName[]    = "CAnmQueue";

#pragma section RX ".text.radarpitch"
/**
 * @note Address: 0x800D58E4
 * @note Size: 0x374
 */
__declspec(section ".text.radarpitch") void SetRadarDishPitch(XObjRadar* object, f32 angle)
{
	RadarJointPath path = sRadarPitchPath;
	CJObj joint(FindJointByPath(object->mJObj, path.nodes, 4));
	joint.SetRotationX(object->mPitch + angle, 0);
}

} // extern "C"
