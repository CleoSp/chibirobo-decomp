#include "game/actorupdateconstants.h"
#include "game/gmath.h"

__declspec(section ".rodata") XObjActorLiterals gActorUpdateConstants = { { { { 1, 1, 1 } }, { { 1, 1, 1 } } } };

__declspec(section ".rodata") Vec sActorHitPositionZero = { 0, 0, 0 };
const float sActorMinHeadPitch                          = -20.0f * DEG2RAD;
const float sActorMaxHeadPitch                          = 20.0f * DEG2RAD;
const float sActorMinHeadYaw                            = -80.0f * DEG2RAD;
const float sActorMaxHeadYaw                            = 80.0f * DEG2RAD;
