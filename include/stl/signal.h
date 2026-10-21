#ifndef _STL_SIGNAL_H
#define _STL_SIGNAL_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SIGABRT 1

int raise(int sig);

#ifdef __cplusplus
}
#endif

#endif
