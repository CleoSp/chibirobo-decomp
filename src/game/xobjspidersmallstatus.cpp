#include "game/xobjspidersmall.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

// what the compiler made for lbl_8037E97C, CStatus[39][16]

extern "C" {
extern void* __vt__7CStatus[];
}

/**
 * @note Address: 0x8010011C
 * @note Size: 0x38
 */
// todo: weak copy
extern "C" void fn_8010011C()
{
	__destroy_arr(lbl_8037E97C, (ConstructorDestructor*)fn_80100154, sizeof(lbl_8037E97C[0]), 39);
}

/**
 * @note Address: 0x80100154
 * @note Size: 0x60
 */
// todo: weak copy
extern "C" CStatus* fn_80100154(CStatus* row, s32 flags)
{
	if (row != NULL) {
		__destroy_arr(row, (ConstructorDestructor*)fn_801001B4, sizeof(CStatus), 16);
		if ((s16)flags > 0) {
			::operator delete(row);
		}
	}
	return row;
}

/**
 * @note Address: 0x801001B4
 * @note Size: 0x50
 */
// todo: weak copy
extern "C" CStatus* fn_801001B4(CStatus* status, s32 flags)
{
	if (status != NULL) {
		status->CStatus::~CStatus();
		if ((s16)flags > 0) {
			::operator delete(status);
		}
	}
	return status;
}

/**
 * @note Address: 0x80100204
 * @note Size: 0x48
 */
// todo: weak copy
extern "C" CStatus* fn_80100204(CStatus* row)
{
	__construct_array(row, (ConstructorDestructor)fn_8010024C, (ConstructorDestructor)fn_801001B4, sizeof(CStatus), 16);
	return row;
}

/**
 * @note Address: 0x8010024C
 * @note Size: 0x18
 */
// todo: weak copy
extern "C" CStatus* fn_8010024C(CStatus* status)
{
	*(void***)((u8*)status + 0x4) = __vt__7CStatus;
	status->mFlags                = 0;
	return status;
}
