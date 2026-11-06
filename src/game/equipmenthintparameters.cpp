#include "Version.h"

extern "C" {
#if defined(VERSION_JP)
extern const int sAttachmentRingTextureIds[9] = { 124, 89, 90, 91, 92, 94, 93, 95, 124 };
#else
extern const int sAttachmentRingTextureIds[9] = { 126, 91, 92, 93, 94, 96, 95, 97, 126 };
#endif
extern const short sCostumeMenuRowItemIds[8]  = { 0x18, 0x20, 0x22, 0x19, 0x1A, 0x1E, 0x1F, 0x21 };
extern const short sCostumeMenuRingItemIds[8] = { 0x19, 0x21, 0x20, 0x18, 0x22, 0x1F, 0x1E, 0x1A };

extern const float sEquipmentCursorRadius        = 40.0f;
extern const float sEquipmentHintSlideAngle      = 1.5707964f;
extern const float sEquipmentHintRightEdge       = 608.0f;
extern const float sPlayerFloorUnitsPerWorldUnit = 100.0f;
extern const float sEquipmentHintFontSize        = 20.0f;
#if defined(VERSION_JP)
extern const float sEquipmentHintIconWidth = 1.0f / 14.0f;
#else
extern const float sEquipmentHintIconWidth = 1.0f / 17.0f;
#endif
#if defined(VERSION_PAL)
extern const float sEquipmentHintIconHalfSize = 12.0f;
#endif
extern const float sEquipmentRingRestHeadAngle = -1.52f;
#if defined(VERSION_PAL)
extern const float sEquipmentRingFadeInSeconds  = 0.18f;
extern const float sEquipmentRingFadeOutSeconds = 0.25f;
#else
extern const float sEquipmentRingFadeInPerFrame  = 1.0f / (0.18f * 60.0f);
extern const float sEquipmentRingFadeOutPerFrame = 1.0f / (0.25f * 60.0f);
#endif
}
