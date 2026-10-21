#ifndef _GAME_CVEC_H
#define _GAME_CVEC_H

#include "game/ut.h"

// CVec's methods that call the sdk. this header doesn't pick a vec.h, include
// dolphin/mtx/vec.h or dolphin/vec.h first. the casts are for the hsd one, it
// takes non const pointers

inline void CVec::operator+=(const Vec& v)
{
	PSVECAdd(this, (Vec*)&v, this);
}

inline f32 CVec::Length() const
{
	return PSVECMag((Vec*)this);
}

inline CVec& CVec::Normalize()
{
	PSVECNormalize(this, this);
	return *this;
}

#endif
