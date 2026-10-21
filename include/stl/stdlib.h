#ifndef _STDLIB_H
#define _STDLIB_H

#include "types.h"
#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

#include "PowerPC_EABI_Support/MSL_C/MSL_Common/alloc.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/arith.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/mbstring.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/rand.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/strtold.h"
#include "PowerPC_EABI_Support/MSL_C/MSL_Common/strtoul.h"

void exit(int status);
void abort(void);
void qsort(void* table_base, size_t num_members, size_t member_size, int (*compare_members)(const void*, const void*));

#ifdef __cplusplus
};
#endif // ifdef __cplusplus

#endif
