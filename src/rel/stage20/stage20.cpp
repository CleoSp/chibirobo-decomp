#include "types.h"
#include "Version.h"
#include "game/cgame.h"

// partial, vtable's after a nonvirtual base at 0x34
struct Stage20ObjBase {
	u8 _00[0x34]; // _00
};

class XObj : public Stage20ObjBase {
public:
	virtual void vf08();                                            // _08
	virtual void vf0C();                                            // _0C
	virtual void vf10();                                            // _10
	virtual void vf14();                                            // _14
	virtual void vf18();                                            // _18
	virtual void vf1C();                                            // _1C
	virtual void vf20();                                            // _20
	virtual void vf24();                                            // _24
	virtual void vf28();                                            // _28
	virtual void vf2C();                                            // _2C
	virtual void vf30();                                            // _30
	virtual void vf34();                                            // _34
	virtual void vf38();                                            // _38
	virtual void vf3C();                                            // _3C
	virtual void vf40();                                            // _40
	virtual void vf44();                                            // _44
	virtual void vf48();                                            // _48
	virtual void vf4C();                                            // _4C
	virtual void vf50();                                            // _50
	virtual void vf54();                                            // _54
	virtual void vf58();                                            // _58
	virtual void vf5C();                                            // _5C
	virtual void vf60();                                            // _60
	virtual void vf64();                                            // _64
	virtual void vf68();                                            // _68
	virtual void vf6C();                                            // _6C
	virtual void vf70();                                            // _70
	virtual void vf74();                                            // _74
	virtual void vf78();                                            // _78
	virtual void vf7C();                                            // _7C
	virtual void vf80(s32, s32, s32, s32, s32, f32, f32, f32, f32); // _80

#if defined(VERSION_JP)
	u8 _38[0x220 - 0x38]; // _38
#else
	u8 _38[0x228 - 0x38]; // _38
#endif
	u64 _228;     // _228, _220 in japan
	u8 _230[0x8]; // _230, _228 in japan
	s32* _238;    // _238, _230 in japan
};

extern "C" s32 fn_16_0(s32 cmd, void* arg, void* arg2);
extern "C" void fn_16_8(void* arg);
extern "C" void UpdateStage(void* arg);
extern "C" void InitStage(void* arg);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { InitStage, UpdateStage, 0, 0, 0, fn_16_8 };
RelCmdFunc _prolog_cmds[]    = { fn_16_0 };
}

static XObj* timeAnimObj;

/**
 * @note Address: stage20 .text 0x0
 * @note Size: 0x8
 */
extern "C" s32 fn_16_0(s32 cmd, void* arg, void* arg2)
{
	return 0;
}

/**
 * @note Address: stage20 .text 0x8
 * @note Size: 0x4
 */
extern "C" void fn_16_8(void* arg)
{
}

/**
 * @note Address: stage20 .text 0xC
 * @note Size: 0xA0
 */
extern "C" void UpdateStage(void* arg)
{
	if (timeAnimObj != NULL) {
		f32 x = 150.0f * lbl_8025C400._E5D2C;
		if (lbl_8025C400._E5D28 == 1) {
			x += 150.0f;
		}
		timeAnimObj->vf80(0, 1, 0, 0, 0, x, 1.0f, 0.0f, 10.0f);
	}
}

/**
 * @note Address: stage20 .text 0xAC
 * @note Size: 0x78
 */
extern "C" void InitStage(void* arg)
{
	XObj* obj;
	s32 i;

	timeAnimObj = 0;
	for (i = 0; i < lbl_8025C400.mObjectCount; i++) {
		obj = lbl_8025C400.GetObject(i);
		if ((obj->_228 & 1) != 0 && *obj->_238 == 0x2CD) {
			timeAnimObj = obj;
		}
	}
}
