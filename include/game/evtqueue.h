#ifndef _GAME_EVTQUEUE_H
#define _GAME_EVTQUEUE_H

#include "game/evt.h"

class XObj;

// maps object ids from 10000 up to an index in the game's object table
struct EvtObjIdEntry {
	s16 mId;    // _00
	s16 mIndex; // _02
};

struct EvtObjIdTable {
	s32 mCount;                // _00
	EvtObjIdEntry mEntry[275]; // _04
};

extern "C" {
extern CEvtQueueEx lbl_80369A3C;
extern CEvtTimer lbl_8036A050;
// the event's own object, dtk has it inside lbl_8036A050
extern XObj lbl_8036A0B0;
// script names for objects
extern EvtObjIdTable lbl_802247B0;
}

#endif
