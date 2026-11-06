#ifndef _GAME_GLOBALS_H
#define _GAME_GLOBALS_H

#include "game/cfile.h"

// the loaded globals file and its sections, class name from RTTI
class Globals {
public:
	CFile mFile;  // _00
	u8* mHeader;  // _EC
	u8* mData;    // _F0
	u8* mText;    // _F4
	u8* mScripts; // _F8
	// _FC vtable

	virtual ~Globals() { } // _08
};
STATIC_ASSERT(sizeof(Globals) == 0x100);
extern "C" Globals gGlobalScriptResources;

// the files' offsets and tables are little endian, a few record fields are native and
// don't go through this
static inline u32 ReadResourceOffset(const u8* p)
{
	return p[0] + ((u32)p[1] << 8) + ((u32)p[2] << 16) + ((u32)p[3] << 24);
}

extern "C" {
// lookups into the globals file's tables by index
const char* fn_8006DBC8(s32 index);
const char* GetLocalizedInterfaceText(s32 index);
u32 GetStatusIconEventFlagId(s32 index);
u8* GetStatusIconName(s32 index);
u8* GetCostumeName(s32 index);    // a costume's name
u8* GetAttachmentName(s32 index); // an attachment's name
u8* fn_8006E1D0(s32 index);
s16 fn_8006E28C(s32 index);
s16 fn_8006E2D4(s32 index);
void ClearItemFlags(s32 index, u16 mask);
void SetItemFlags(s32 index, u16 mask);
u16 GetItemFlagsMasked(s32 index, u16 mask);
u8* GetItemName(s32 index);
// a 4 byte little endian offset to a script pointer, null for 0
u8* ResolveEventScriptOffset(u8* operand);
BOOL IsGlobalEventScript(u8* script);
// the same for an entry of an offset table
u8* GetGlobalScriptByIndex(s32 index);
u8* GetGlobalScriptTable(s32);
u32 fn_8006E9AC(s32 index);
u32 fn_8006EAF0(s32 index);
u32 fn_8006EBC8(s32 index);

// the language folders, "bin%s", usa and europe
extern const char* lbl_802244C8[];
extern const char* lbl_8022AE28[];
}

#endif
