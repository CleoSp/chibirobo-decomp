#include "types.h"
#include "Version.h"
#include "game/cgame.h"

extern "C" f64 atan2(f64 y, f64 x);

// not the full layout. vtable comes after the nonvirtual base at 0x34
struct Stage16ObjBase {
	u8 _00[0x34]; // _00
};

class XObj : public Stage16ObjBase {
public:
	virtual void vf08();         // _08
	virtual void vf0C();         // _0C
	virtual void vf10();         // _10
	virtual void vf14();         // _14
	virtual void vf18();         // _18
	virtual void vf1C();         // _1C
	virtual void vf20();         // _20
	virtual void vf24();         // _24
	virtual void vf28();         // _28
	virtual void vf2C();         // _2C
	virtual void vf30();         // _30
	virtual void vf34();         // _34
	virtual void vf38();         // _38
	virtual void vf3C();         // _3C
	virtual void vf40();         // _40
	virtual void vf44();         // _44
	virtual void vf48();         // _48
	virtual void vf4C();         // _4C
	virtual void vf50();         // _50
	virtual void vf54();         // _54
	virtual void vf58();         // _58
	virtual void vf5C();         // _5C
	virtual void vf60();         // _60
	virtual void vf64();         // _64
	virtual void vf68();         // _68
	virtual void vf6C();         // _6C
	virtual void vf70();         // _70
	virtual void vf74();         // _74
	virtual void vf78();         // _78
	virtual void vf7C(f32, f32); // _7C

	s32 GetId() { return _244 != NULL ? *_244 : -1; }

#if defined(VERSION_JP)
	u8 _38[0xC0 - 0x38]; // _38
#else
	u8 _38[0xC8 - 0x38]; // _38
#endif
	f32 _C8;                // _C8, _C0 in japan
	u8 _CC[0x4];            // _CC, _C4 in japan
	f32 _D0;                // _D0, _C8 in japan
	u8 _D4[0x228 - 0xD4];   // _D4, _CC in japan
	u64 _228;               // _228, _220 in japan
	u8 _230[0x244 - 0x230]; // _230, _228 in japan
	s32* _244;              // _244, _23C in japan
};

extern "C" s32 fn_13_0(s32 cmd, void* arg, void* arg2);
extern "C" void UpdateStage(void* arg);
extern "C" void InitStage(void* arg);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { InitStage, UpdateStage };
RelCmdFunc _prolog_cmds[]    = { fn_13_0 };
}

static XObj* gameBotan2;

/**
 * @note Address: stage16 .text 0x0
 * @note Size: 0x8
 */
extern "C" s32 fn_13_0(s32 cmd, void* arg, void* arg2)
{
	return 0;
}

/**
 * @note Address: stage16 .text 0x8
 * @note Size: 0x114
 */
extern "C" void UpdateStage(void* arg)
{
	if (gameBotan2 != NULL) {
		XObj* other = lbl_8025C400.mPlayer;
		f32 angle   = atan2(other->_C8 - gameBotan2->_C8, other->_D0 - gameBotan2->_D0);
		gameBotan2->vf7C(angle, 0.0f);
	} else {
		XObj* obj;
		s32 i;
		for (i = 0; i < lbl_8025C400.mObjectCount && gameBotan2 == NULL; i++) {
			obj = lbl_8025C400.GetObject(i);
			if ((obj->_228 & 1) != 0 && obj->GetId() == 0x2E9) {
				gameBotan2 = obj;
			}
		}
	}
}

/**
 * @note Address: stage16 .text 0x11C
 * @note Size: 0x10
 */
extern "C" void InitStage(void* arg)
{
	gameBotan2 = 0;
}
