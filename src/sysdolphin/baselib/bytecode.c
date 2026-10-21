#include <global.h>
#include <math.h>
#include <sysdolphin/list.h>
#include <sysdolphin/random.h>

#define DEG_TO_RAD 0.017453292519943295
#define RAD_TO_DEG 57.29577951308232

#define HSD_ASSERT(line, cond) ((cond) ? ((void)0) : __assert("bytecode.c", line, #cond))

typedef union {
	void* p;
	int i;
	f32 f;
} ByteCodeVal;

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline f32 bytecode_sqrtf(f32 x)
{
	volatile f32 y;

	if (x > 0.0F) {
		f64 guess = __frsqrte((f64)x);
		guess     = 0.5 * guess * (3.0 - guess * guess * x);
		guess     = 0.5 * guess * (3.0 - guess * guess * x);
		guess     = 0.5 * guess * (3.0 - guess * guess * x);
		y         = (f32)(x * guess);
		return y;
	}
	return x;
}

/**
 * @note Address: N/A
 * @note Size: ?
 */
static inline f32 bytecode_atan2f(f32 y, f32 x)
{
	if (x == 0.0F) {
		return y >= 0.0F ? 1.5707963267948966F : -1.5707963267948966F;
	}
	return atan2(y, x);
}

/**
 * @note Address: 0x801D466C
 * @note Size: 0x15C8
 */
f32 HSD_ByteCodeEval(u8* bytecode, f32* args, s32 nb_args)
{
	HSD_SList* stack;
	int i;
	unsigned char last_command;
	int operand_count;
	unsigned int operand;
	HSD_SList* list;
	float fv, f0, f1;
	int d0, d1;

	stack         = 0;
	operand_count = 0;

	if (bytecode == 0) {
		return 0.0F;
	}

	for (;;) {
		if (operand_count > 0) {
			operand_count--;
			operand = ((operand << 8) & ~0xffU) | (*bytecode & 0xffU);
			bytecode++;

			if (operand_count != 0) {
				continue;
			}

			switch (last_command) {
			case 2:
				HSD_ASSERT(281, operand < nb_args);
				stack = HSD_SListPrepend(stack, (void*)((ByteCodeVal*)&args[operand])->i);
				break;
			case 5:
				for (i = 0; i < operand; i++) {
					stack = HSD_SListRemove(stack);
				}
				break;
			case 0x3C: {
				list = stack;
				i    = 0;
				while (list != 0 && i < operand) {
					list = list->next;
					i++;
				}
				if (list == 0) {
					OSReport("specified stack doesn't exist (%d).\n", operand);
					HSD_Panic("bytecode.c", 299, "");
				} else {
					stack = HSD_SListPrepend(stack, list->data);
				}
				break;
			}
			case 3:
				HSD_ASSERT(307, stack);
				if ((int)stack->data != 0) {
					bytecode += operand;
				}
				stack = HSD_SListRemove(stack);
				break;
			case 4:
				bytecode += operand;
				break;
			case 6:
				stack = HSD_SListPrepend(stack, (void*)operand);
				break;
			case 0xFF:
				HSD_Panic("bytecode.c", 323, "not yet implemented.\n");
				/* fallthrough */
			default:
				HSD_Panic("bytecode.c", 326, "unexpected byte code.\n");
				break;
			}
			continue;
		}

		last_command = *bytecode++;
		switch (last_command) {
		case 0:
			break;
		case 1:
			HSD_ASSERT(339, stack);
			f0 = ((ByteCodeVal*)&stack->data)->f;
			while (stack != 0) {
				stack = HSD_SListRemove(stack);
			}
			return f0;
		case 5:
		case 0x3C:
		case 0xFF:
			operand_count = 1;
			operand       = 0;
			break;
		case 2:
		case 3:
		case 4:
			operand_count = 2;
			operand       = 0;
			break;
		case 6:
			operand_count = 4;
			operand       = 0;
			break;
		case 7:
			HSD_ASSERT(376, stack);
			{
				((ByteCodeVal*)&stack->data)->i = (int)((ByteCodeVal*)&stack->data)->f;
			}
			break;
		case 8:
			HSD_ASSERT(381, stack);
			{
				fv          = (float)((ByteCodeVal*)&stack->data)->i;
				stack->data = *(void**)&fv;
			}
			break;
		case 9:
			HSD_ASSERT(387, stack);
			fv          = -(((ByteCodeVal*)&stack->data)->f);
			stack->data = *(void**)&fv;
			break;
		case 0x0A:
			HSD_ASSERT(393, stack);
			((ByteCodeVal*)&stack->data)->i = -((ByteCodeVal*)&stack->data)->i;
			break;
		case 0x0B:
			HSD_ASSERT(399, stack);
			((ByteCodeVal*)&stack->data)->i = HSD_Randi(2);
			break;
		case 0x0C:
			HSD_ASSERT(405, stack);
			fv          = HSD_Randf();
			stack->data = *(void**)&fv;
			break;
		case 0x0D:
			HSD_ASSERT(411, stack);
			fv          = (float)sin((float)(DEG_TO_RAD * (double)((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x0E:
			HSD_ASSERT(417, stack);
			fv          = (float)cos((float)(DEG_TO_RAD * (double)((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x0F:
			HSD_ASSERT(423, stack);
			fv          = (float)tan((float)(DEG_TO_RAD * (double)((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x10:
			HSD_ASSERT(429, stack);
			fv          = (float)(RAD_TO_DEG * (float)asin(((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x11:
			HSD_ASSERT(435, stack);
			fv          = (float)(RAD_TO_DEG * (float)acos(((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x12:
			HSD_ASSERT(441, stack);
			fv          = (float)(RAD_TO_DEG * (float)atan(((ByteCodeVal*)&stack->data)->f));
			stack->data = *(void**)&fv;
			break;
		case 0x13:
			HSD_ASSERT(447, stack);
			fv          = (float)log(((ByteCodeVal*)&stack->data)->f);
			stack->data = *(void**)&fv;
			break;
		case 0x14:
			HSD_ASSERT(453, stack);
			fv          = (float)exp(((ByteCodeVal*)&stack->data)->f);
			stack->data = *(void**)&fv;
			break;
		case 0x15:
			HSD_ASSERT(459, stack);
			if (((ByteCodeVal*)&stack->data)->f < 0.0F) {
				fv          = -(((ByteCodeVal*)&stack->data)->f);
				stack->data = *(void**)&fv;
			}
			break;
		case 0x28:
			HSD_ASSERT(467, stack);
			{
				d0 = ((ByteCodeVal*)&stack->data)->i;
				if (d0 < 0) {
					((ByteCodeVal*)&stack->data)->i = -d0;
				}
			}
			break;
		case 0x16:
			HSD_ASSERT(474, stack);
			fv          = bytecode_sqrtf(((ByteCodeVal*)&stack->data)->f);
			stack->data = *(void**)&fv;
			break;
		case 0x31:
			HSD_ASSERT(480, stack);
			stack->data = (void*)!(int)stack->data;
			break;
		case 0x17:
			HSD_ASSERT(501, stack);
			HSD_ASSERT(501, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			fv          = ((ByteCodeVal*)&stack->data)->f + f0;
			stack->data = *(void**)&fv;
			break;
		case 0x18:
			HSD_ASSERT(507, stack);
			HSD_ASSERT(507, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			fv          = ((ByteCodeVal*)&stack->data)->f - f0;
			stack->data = *(void**)&fv;
			break;
		case 0x19:
			HSD_ASSERT(513, stack);
			HSD_ASSERT(513, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			fv          = ((ByteCodeVal*)&stack->data)->f * f0;
			stack->data = *(void**)&fv;
			break;
		case 0x1A:
			HSD_ASSERT(519, stack);
			HSD_ASSERT(519, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			fv          = ((ByteCodeVal*)&stack->data)->f / f0;
			stack->data = *(void**)&fv;
			break;
		case 0x1B:
			HSD_ASSERT(525, stack);
			HSD_ASSERT(525, stack->next);
			f0    = ((ByteCodeVal*)&stack->data)->f;
			stack = HSD_SListRemove(stack);
			f1 =
#ifdef MUST_MATCH
			    f1 =
#endif
			        ((ByteCodeVal*)&stack->data)->f;
			fv          = (float)fmod(f1, f0);
			stack->data = *(void**)&fv;
			break;
		case 0x1C:
			HSD_ASSERT(531, stack);
			HSD_ASSERT(531, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i + d1);
			break;
		case 0x1D:
			HSD_ASSERT(536, stack);
			HSD_ASSERT(536, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i - d1);
			break;
		case 0x1E:
			HSD_ASSERT(541, stack);
			HSD_ASSERT(541, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i * d1);
			break;
		case 0x1F:
			HSD_ASSERT(546, stack);
			HSD_ASSERT(546, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i / d1);
			break;
		case 0x20:
			HSD_ASSERT(551, stack);
			HSD_ASSERT(551, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			d0                              = ((ByteCodeVal*)&stack->data)->i;
			((ByteCodeVal*)&stack->data)->i = (d0 % d1);
			break;
		case 0x21:
			HSD_ASSERT(556, stack);
			HSD_ASSERT(556, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			fv          = (float)pow(((ByteCodeVal*)&stack->data)->f, f0);
			stack->data = *(void**)&fv;
			break;
		case 0x22:
			HSD_ASSERT(562, stack);
			HSD_ASSERT(562, stack->next);
			f0    = ((ByteCodeVal*)&stack->data)->f;
			stack = HSD_SListRemove(stack);
			if (((ByteCodeVal*)&stack->data)->f > f0) {
				stack->data = *(void**)&f0;
			}
			break;
		case 0x23:
			HSD_ASSERT(569, stack);
			HSD_ASSERT(569, stack->next);
			f0    = ((ByteCodeVal*)&stack->data)->f;
			stack = HSD_SListRemove(stack);
			if (((ByteCodeVal*)&stack->data)->f < f0) {
				stack->data = *(void**)&f0;
			}
			break;
		case 0x24:
			HSD_ASSERT(576, stack);
			HSD_ASSERT(576, stack->next);
			d1    = ((ByteCodeVal*)&stack->data)->i;
			stack = HSD_SListRemove(stack);
			if (((ByteCodeVal*)&stack->data)->i > d1) {
				((ByteCodeVal*)&stack->data)->i = d1;
			}
			break;
		case 0x25:
			HSD_ASSERT(583, stack);
			HSD_ASSERT(583, stack->next);
			d1    = ((ByteCodeVal*)&stack->data)->i;
			stack = HSD_SListRemove(stack);
			if (((ByteCodeVal*)&stack->data)->i < d1) {
				((ByteCodeVal*)&stack->data)->i = d1;
			}
			break;
		case 0x26: {
			HSD_ASSERT(590, stack);
			HSD_ASSERT(590, stack->next);
			f0          = ((ByteCodeVal*)&stack->data)->f;
			stack       = HSD_SListRemove(stack);
			f1          = ((ByteCodeVal*)&stack->data)->f;
			fv          = (float)(RAD_TO_DEG * bytecode_atan2f(f1, f0));
			stack->data = *(void**)&fv;
			break;
		}
		case 0x33:
			HSD_ASSERT(596, stack);
			HSD_ASSERT(596, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			f1                              = ((ByteCodeVal*)&stack->data)->f;
			((ByteCodeVal*)&stack->data)->i = (f1 < f0);
			break;
		case 0x34:
			HSD_ASSERT(601, stack);
			HSD_ASSERT(601, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			f1                              = ((ByteCodeVal*)&stack->data)->f;
			((ByteCodeVal*)&stack->data)->i = (f1 > f0);
			break;
		case 0x35:
			HSD_ASSERT(606, stack);
			HSD_ASSERT(606, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->f <= f0);
			break;
		case 0x36:
			HSD_ASSERT(611, stack);
			HSD_ASSERT(611, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			f1                              = ((ByteCodeVal*)&stack->data)->f;
			((ByteCodeVal*)&stack->data)->i = (f1 >= f0);
			break;
		case 0x37:
			HSD_ASSERT(616, stack);
			HSD_ASSERT(616, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			f1                              = ((ByteCodeVal*)&stack->data)->f;
			((ByteCodeVal*)&stack->data)->i = (f1 == f0);
			break;
		case 0x38:
			HSD_ASSERT(621, stack);
			HSD_ASSERT(621, stack->next);
			f0                              = ((ByteCodeVal*)&stack->data)->f;
			stack                           = HSD_SListRemove(stack);
			f1                              = ((ByteCodeVal*)&stack->data)->f;
			((ByteCodeVal*)&stack->data)->i = (f1 != f0);
			break;
		case 0x29:
			HSD_ASSERT(626, stack);
			HSD_ASSERT(626, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i < d1);
			break;
		case 0x2A:
			HSD_ASSERT(631, stack);
			HSD_ASSERT(631, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i > d1);
			break;
		case 0x2B:
			HSD_ASSERT(636, stack);
			HSD_ASSERT(636, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i <= d1);
			break;
		case 0x2C:
			HSD_ASSERT(641, stack);
			HSD_ASSERT(641, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i >= d1);
			break;
		case 0x2D:
			HSD_ASSERT(646, stack);
			HSD_ASSERT(646, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i == d1);
			break;
		case 0x2E:
			HSD_ASSERT(651, stack);
			HSD_ASSERT(651, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			((ByteCodeVal*)&stack->data)->i = (((ByteCodeVal*)&stack->data)->i != d1);
			break;
		case 0x2F:
			HSD_ASSERT(656, stack);
			HSD_ASSERT(656, stack->next);
			d1    = ((ByteCodeVal*)&stack->data)->i;
			stack = HSD_SListRemove(stack);
			d0    = ((ByteCodeVal*)&stack->data)->i;
			{
				int val = 0;
				if (d0 != 0 && d1 != 0) {
					val = 1;
				}
				((ByteCodeVal*)&stack->data)->i = val;
			}
			break;
		case 0x30:
			HSD_ASSERT(661, stack);
			HSD_ASSERT(661, stack->next);
			d1    = ((ByteCodeVal*)&stack->data)->i;
			stack = HSD_SListRemove(stack);
			d0    = ((ByteCodeVal*)&stack->data)->i;
			{
				int val = 0;
				if (d0 != 0 || d1 != 0) {
					val = 1;
				}
				((ByteCodeVal*)&stack->data)->i = val;
			}
			break;
		case 0x32:
			HSD_ASSERT(666, stack);
			HSD_ASSERT(666, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			d0                              = ((ByteCodeVal*)&stack->data)->i;
			((ByteCodeVal*)&stack->data)->i = (d0 == 0 && d1 != 0) || (d0 != 0 && d1 == 0);
			break;
		case 0x39:
			HSD_ASSERT(671, stack);
			HSD_ASSERT(671, stack->next);
			d1          = ((ByteCodeVal*)&stack->data)->i;
			stack       = HSD_SListRemove(stack);
			stack->data = (void*)(((ByteCodeVal*)&stack->data)->i & d1);
			break;
		case 0x3A:
			HSD_ASSERT(676, stack);
			HSD_ASSERT(676, stack->next);
			d1          = ((ByteCodeVal*)&stack->data)->i;
			stack       = HSD_SListRemove(stack);
			stack->data = (void*)(((ByteCodeVal*)&stack->data)->i | d1);
			break;
		case 0x3B:
			HSD_ASSERT(681, stack);
			HSD_ASSERT(681, stack->next);
			d1          = ((ByteCodeVal*)&stack->data)->i;
			stack       = HSD_SListRemove(stack);
			stack->data = (void*)(((ByteCodeVal*)&stack->data)->i ^ d1);
			break;
		case 0x27:
			HSD_ASSERT(687, stack);
			HSD_ASSERT(687, stack->next);
			d1                              = ((ByteCodeVal*)&stack->data)->i;
			stack                           = HSD_SListRemove(stack);
			d0                              = ((ByteCodeVal*)&stack->data)->i;
			((ByteCodeVal*)&stack->data)->i = (d0 + HSD_Randi((d1 - d0) + 1));
			break;
		default:
			OSReport("unexpected opcode 0x%x.\n", last_command);
			HSD_Panic("bytecode.c", 693, "");
			break;
		}
	}
}
