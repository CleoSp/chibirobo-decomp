#ifndef _GAME_LOGO_H
#define _GAME_LOGO_H

#include "game/cscrn.h"
#include "game/infoscrn.h"

// the boot logos, CGame holds one. the info screen is in infoscrn.h. class names from RTTI,
// layouts are partial

class Logo {
public:
	s32 mState; // _00
	CTpl mTpl;  // _04
	Vec _1F4;   // _1F4, zeroed with the state
	// _200 vtable

	virtual ~Logo() { } // _08 (weak)
};
typedef char Logo_size_check[sizeof(Logo) == 0x204 ? 1 : -1];

#endif
