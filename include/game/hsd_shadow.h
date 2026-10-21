#ifndef _GAME_HSD_SHADOW_H
#define _GAME_HSD_SHADOW_H

// c++ shadow.h inlines, assert lines from the binary, names are ours

extern "C" {
#include <sysdolphin/shadow.h>
}

#define SHADOW_H_ASSERT(line, cond) ((cond) ? ((void)0) : __assert("shadow.h", line, #cond))

inline void HSD_ShadowSetIntensity(HSD_Shadow* shadow, u8 intensity)
{
	SHADOW_H_ASSERT(0x67, shadow);
	shadow->intensity = intensity;
}

inline void HSD_ShadowSetUserData(HSD_Shadow* shadow, void* user_data)
{
	SHADOW_H_ASSERT(0x76, shadow);
	shadow->user_data = user_data;
}

inline void* HSD_ShadowGetUserData(HSD_Shadow* shadow)
{
	SHADOW_H_ASSERT(0x81, shadow);
	return shadow->user_data;
}

#endif
