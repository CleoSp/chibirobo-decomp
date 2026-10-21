#include "types.h"
#include "Version.h"
#include "dolphin/mtx.h"
#include "game/ut.h"
#include "game/cgame.h"

extern "C" {
#include <sysdolphin/random.h>
}

extern "C" void OSReport(const char* msg, ...);

// partial layout, the vtable sits after a nonvirtual base at 0x34
struct HoriObjBase {
	u8 _00[0x34]; // _00
};

class XObj : public HoriObjBase {
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

	s32 GetId() { return _244 != NULL ? *_244 : -1; }

#if defined(VERSION_JP)
	u8 _38[0x23C - 0x38]; // _38
#else
	u8 _38[0x244 - 0x38]; // _38
#endif
	s32* _244; // _244, _23C in japan
};

struct HoriPlayer {
#if defined(VERSION_JP)
	u8 _00[0x9C4]; // _00
#else
	u8 _00[0x9D4]; // _00
#endif
	s32 _9D4; // _9D4, _9C4 in japan
};

extern "C" s32 CmdSetSoundPitch(s32 cmd, void* arg, void* arg2);
extern "C" s32 CmdStartRandomAnims(s32 cmd, s32 count, void* arg2);
extern "C" void UpdateStage(void* arg);
extern "C" void Init(void* arg);

typedef void (*RelSlotFunc)(void* arg);
typedef s32 (*RelCmdFunc)(s32 cmd, void* arg, void* arg2);

extern "C" {
RelSlotFunc _prolog_slots[7] = { Init, UpdateStage };
RelCmdFunc _prolog_cmds[]    = { CmdSetSoundPitch, (RelCmdFunc)CmdStartRandomAnims };
}

class RandomAnimCtrl {
public:
	typedef s32 (RandomAnimCtrl::*Func)();

	s32 UpdateRandomAnim();

	void Set(XObj* obj, const Func& func)
	{
		_00 = obj;
		if (_00 == NULL) {
			_08 = 0;
		}
		_08 = func;
		_14 = 0;
	}

	void Start(XObj* obj) { Set(obj, &RandomAnimCtrl::UpdateRandomAnim); }

	XObj* _00; // _00
	s32 _04;   // _04
	Func _08;  // _08
	s32 _14;   // _14
};

static RandomAnimCtrl randomAnims[10];
static s32 randomAnimCount;
static s32 prevPlayerObjNo;
static s32 tuta_objno;
static CVec lbl_2_bss_108;

/**
 * @note Address: hori .text 0x0
 * @note Size: 0x21C
 */
s32 RandomAnimCtrl::UpdateRandomAnim()
{
	f32 rnd = HSD_Randf();

	if (_04 >= 0) {
		_04--;
	}
	switch (_14) {
	case 0:
		_04 = HSD_Randi(180) + 180;
		if (rnd < 0.333f) {
			_14 = 10;
		} else if (rnd < 0.666f) {
			_14 = 20;
		} else {
			_14 = 30;
		}
		break;
	case 10:
		_00->vf80(0, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f, 10.0f);
		_14++;
		break;
	case 11:
		if (_04 <= 0) {
			_14 = 0;
		}
		break;
	case 20:
		_00->vf80(3, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f, 10.0f);
		_14++;
		break;
	case 21:
		if (_04 <= 0) {
			_14 = 0;
		}
		break;
	case 30:
		_00->vf80(4, 0, 0, 0, 0, 0.0f, 1.0f, 0.0f, 10.0f);
		_14++;
		break;
	case 31:
		if (_04 <= 0) {
			_14 = 0;
		}
		break;
	}
	return 0;
}

/**
 * @note Address: hori .text 0x21C
 * @note Size: 0x50
 */
extern "C" s32 CmdSetSoundPitch(s32 cmd, void* arg, void* arg2)
{
	s32* args = (s32*)arg2;
	s16 id    = args[0];
	s32 time  = args[1] * 60 / 100;
	XSound* unk;

	if (time <= 0) {
		time = 1;
	}
	unk       = &lbl_8025C400._E7E24;
	unk->_2BA = id;
	unk->_188 = time;
	return 0;
}

/**
 * @note Address: hori .text 0x26C
 * @note Size: 0xBC
 */
extern "C" s32 CmdStartRandomAnims(s32 cmd, s32 count, void* arg2)
{
	s32* ids = (s32*)arg2;
	s32 i;

	for (i = 0; i < count; i++) {
		randomAnims[randomAnimCount].Start(lbl_8025C400.mObjects[*ids]);
		ids++;
		randomAnimCount++;
	}
	return randomAnimCount;
}

/**
 * @note Address: hori .text 0x328
 * @note Size: 0xB0
 */
extern "C" void UpdateStage(void* arg)
{
	s32 cur = ((HoriPlayer*)lbl_8025C400.mPlayer)->_9D4;

	if (prevPlayerObjNo != cur && cur == tuta_objno) {
		lbl_8025C400.mObjects[tuta_objno]->vf80(1, 1, 0, 0, 0, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	prevPlayerObjNo = cur;
}

/**
 * @note Address: hori .text 0x3D8
 * @note Size: 0xB8
 */
extern "C" void Init(void* arg)
{
	s32 i;

	OSReport("Init\n");
	prevPlayerObjNo = -1;
	for (i = 0; i < lbl_8025C400.mObjectCount; i++) {
		if (lbl_8025C400.mObjects[i]->GetId() == 0x2EC) {
			tuta_objno = i;
			OSReport("tuta_objno = %d\n", i);
		}
	}
}

/**
 * @note Address: hori .text 0x490
 * @note Size: 0x44
 */

/**
 * @note Address: hori .text 0x4D4
 * @note Size: 0x48
 */
