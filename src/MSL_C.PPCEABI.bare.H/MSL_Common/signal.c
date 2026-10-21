#include "PowerPC_EABI_Support/MSL_C/MSL_Common/critical_regions.h"
#include "signal.h"
#include "stdlib.h"

typedef void (*__signal_func_ptr)(int);

#define SIG_DFL ((__signal_func_ptr)0)
#define SIG_IGN ((__signal_func_ptr)1)

static __signal_func_ptr signal_funcs[6];

/**
 * @note Address: 0x80158AC4
 * @note Size: 0xC0
 */
int raise(int sig)
{
	__signal_func_ptr func;

	if (sig < 1 || sig > 6) {
		return -1;
	}

	__begin_critical_region(signal_funcs_access);
	func = signal_funcs[sig - 1];
	if (func != SIG_IGN) {
		signal_funcs[sig - 1] = SIG_DFL;
	}
	__end_critical_region(signal_funcs_access);

	if (func == SIG_IGN || (func == SIG_DFL && sig == SIGABRT)) {
		return 0;
	}

	if (func == SIG_DFL) {
		exit(0);
	}

	func(sig);
	return 0;
}
