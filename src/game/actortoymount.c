#include "types.h"

const f32 sActorToyMountQuarterTurn         = 1.5707964f;
const f32 sActorToyHeldPropOffsetX          = -1.2f;
const f32 sActorToy0FMountOffsetY           = -5.5f;
const f32 sActorToyHeldPropOffsetZ          = -0.5f;
const f32 sActorToyCarryPositionOffsetZ     = 2.5f;
const f32 sActorToy159CarryHeight           = 12.0f;
const f32 sActorToyMountNegativeHalfTurn    = -3.1415927f;
const f32 sActorToyMountNegativeQuarterTurn = -1.5707964f;
const f32 sActorToy159MountOffsetX          = 5.0f;
const f32 sActorToyPlacementScale           = 100.0f;

#if defined(VERSION_GGTP01)
const char sActorToyMountPadding[4] = { 0 };
#else
const char sActorToyMountPadding[4] = { '0', '0', '0', '0' };
#endif
