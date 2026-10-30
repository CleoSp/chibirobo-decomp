#include "game/objb.h"

// the wire paths the plug pulls, and the robot's status numbers

extern "C" {
void fn_800DB810(XObjPlugWire* wire, f32 x, f32 y, f32 z);
void fn_800B5B7C(XObjChr* chr, s32 kind, s32 delta); // tells the robot a number changed
void fn_800660E0(void* obj, s32 level);
void fn_800660E4(void* obj, s32 delta);
}

extern "C" XObjPathPoint lbl_8037C448[0x80];

// ---- functions ----

extern "C" {

XObjPathPoint* fn_800DBC04(XObjPathPoint* point, s16 del);

/**
 * @note Address: 0x800DB7A8
 * @note Size: 0x30
 */
void fn_800DB7A8(XObjPlugWire* wire, s32 on, u8 mode)
{
	if (on) {
		wire->mFlags |= 6;
		wire->mMode = mode;
	} else {
		wire->mFlags &= ~6;
		wire->mMode = mode;
	}
}

/**
 * @note Address: 0x800DB7D8
 * @note Size: 0x38
 */
void fn_800DB7D8(XObjPlugWire* wire, s32 i, Vec* target)
{
	wire->mTargets[i] = target;
	if (target != NULL) {
		wire->mEnds[i].x = target->x;
		wire->mEnds[i].y = target->y;
		wire->mEnds[i].z = target->z;
	}
}

/**
 * @note Address: 0x800DBA7C
 * @note Size: 0x150
 */
void fn_800DBA7C(XObjPlugWire* wire, s32 count, f32 x, f32 y, f32 z, f32 length)
{
	wire->mCount = count;
	static XObjPathPoint sPoints[0x80];
	wire->mPoints = sPoints;
	fn_800DB810(wire, x, y, z);
	wire->_08         = length;
	wire->mTargets[0] = NULL;
	wire->mTargets[1] = NULL;
	wire->mTargets[2] = NULL;
	wire->_3C         = 0.0f;
	wire->_40         = 0.0f;
	wire->_48         = 0;
	wire->_4C         = 0;
	wire->_54         = 6;
	wire->_58         = 0.2f;
	wire->mColors[1]  = 0xFF;
	wire->mColors[0]  = 0xFF;
	wire->mMode       = 0;
	wire->mFlags      = 0;
	wire->mFlags |= 1;
	wire->mFlags &= ~8;
}

/**
 * @note Address: 0x800DBBCC
 * @note Size: 0x38
 */
void fn_800DBBCC(void)
{
	__destroy_arr(lbl_8037C448, (Dtor)fn_800DBC04, sizeof(XObjPathPoint), 0x80);
}

/**
 * @note Address: 0x800DBC04
 * @note Size: 0x7C
 */
XObjPathPoint* fn_800DBC04(XObjPathPoint* point, s16 del)
{
	if (point) {
		point->_20.CVec::~CVec();
		point->mSpeed.CVec::~CVec();
		point->mPos.CVec::~CVec();
		if (del > 0) {
			operator delete(point);
		}
	}
	return point;
}

/**
 * @note Address: 0x800DC62C
 * @note Size: 0x128
 */
void fn_800DC62C(UnkSlotTable* table)
{
	for (int i = 0; i < 255; i++) {
		if (table->mSlots[i].mValue <= 0) {
			table->Remove(i);
		}
	}
}

/**
 * @note Address: 0x800DC754
 * @note Size: 0x25C
 */
// NONMATCHING: the dol keeps the slot offset in r8 for the shift, this recomputes it
BOOL fn_800DC754(UnkSlotTable* table, s16 id, s16 n)
{
	s32 i = table->Find(id);
	if (i >= 0 && table->mSlots[i].mValue > 0) {
		s16 value = table->mSlots[i].mValue - n;
		if (value < 0) {
			value = 0;
		}
		table->mSlots[i].mValue = value;
		if (value == 0) {
			table->Remove(i);
		}
		return FALSE;
	}
	return FALSE;
}

/**
 * @note Address: 0x800DCD30
 * @note Size: 0x140
 */
// NONMATCHING: the dol walks the table with lhau
s32 fn_800DCD30(UnkSlotTable* table, s16 id)
{
	for (int i = 0; i < 256; i++) {
		if (id == table->mSlots[i].mId && table->mSlots[i].mValue > 0) {
			return i;
		}
	}
	return -1;
}

/**
 * @note Address: 0x800DCE70
 * @note Size: 0x15C
 */
// NONMATCHING: the dol steps the slot pointer every unrolled check
s16 fn_800DCE70(UnkStatus* status, s16 id)
{
	s32 i = status->mTable.Find(id);
	if (i >= 0) {
		return status->mTable.mSlots[i].mValue;
	}
	return 0;
}

/**
 * @note Address: 0x800DCFCC
 * @note Size: 0x78
 */
void fn_800DCFCC(UnkStatus* status, s32 delta)
{
	status->_018 += delta;
	if (status->_018 < 0) {
		status->_018 = 0;
	}
	if (status->_018 > 99999) {
		status->_018 = 99999;
	}
	if (delta != 0) {
		fn_800B5B7C(lbl_8025C400.mChr, 3, delta);
	}
}

/**
 * @note Address: 0x800DD044
 * @note Size: 0x78
 */
void fn_800DD044(UnkStatus* status, s32 delta)
{
	status->_014 += delta;
	if (status->_014 < 0) {
		status->_014 = 0;
	}
	if (status->_014 > 99999) {
		status->_014 = 99999;
	}
	if (delta != 0) {
		fn_800B5B7C(lbl_8025C400.mChr, 2, delta);
	}
}

/**
 * @note Address: 0x800DD0BC
 * @note Size: 0x3C
 */
void fn_800DD0BC(UnkStatus* status, s32 delta)
{
	status->_008 += delta;
	if (status->_008 < 1) {
		status->_008 = 1;
	}
	if (status->_008 > 1000000) {
		status->_008 = 1000000;
	}
}

/**
 * @note Address: 0x800DD0F8
 * @note Size: 0xA0
 */
void fn_800DD0F8(UnkStatus* status, s32 delta)
{
	status->mLevel += delta;
	if (status->mLevel < 0) {
		status->mLevel = 0;
	}
	if (status->mLevel > 30) {
		status->mLevel = 30;
	}
	if (delta != 0) {
		fn_800660E0(lbl_8025C400._08F38, status->mLevel + 1);
		fn_800B5B7C(lbl_8025C400.mChr, 1, status->mLevel + 1);
	}
}

/**
 * @note Address: 0x800DD198
 * @note Size: 0x9C
 */
void fn_800DD198(UnkStatus* status, s32 delta)
{
	status->mHappy += delta;
	if (status->mHappy < 0) {
		status->mHappy = 0;
	}
	if (status->mHappy > 99999) {
		status->mHappy = 99999;
	}
	if (delta != 0) {
		fn_800660E4(lbl_8025C400._08F38, delta);
		fn_800B5B7C(lbl_8025C400.mChr, 0, delta);
	}
}

/**
 * @note Address: 0x800DD234
 * @note Size: 0xF8
 */
void fn_800DD234(UnkStatus* status)
{
	status->mHappy  = 0;
	status->mLevel  = 0;
	status->_008    = 1000000;
	status->_010    = 80.0f;
	status->_00C    = status->_010;
	status->_014    = 0;
	status->_018    = 0;
	status->_01C[0] = 0;
	status->_01C[1] = 0;
	status->_01C[2] = 0;
	status->_01C[3] = 0;
	status->_01C[4] = 0;
	status->_01C[5] = 0;
	status->_01C[6] = 0;
	status->_01C[7] = 0;
	for (int i = 0; i < 256; i++) {
		status->mTable.mSlots[i].mId    = -1;
		status->mTable.mSlots[i].mValue = 0;
	}
	status->mTable.mCompact = TRUE;
}

} // extern "C"
