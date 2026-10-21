#ifndef _BUILDSETTINGS_H
#define _BUILDSETTINGS_H

#include "Version.h"

// configure.py passes DTK_CONFIG_NONMATCHING=0 for normal builds and 1 for
// --non-matching. only this file reads it, everything else checks BUILD_MATCHING
#if defined(DTK_CONFIG_NONMATCHING) && !DTK_CONFIG_NONMATCHING
#define BUILD_MATCHING
#endif

// fixes for known crashes and mistakes, on by default in non matching builds
#if !defined(BUILD_MATCHING) && !defined(BUGFIX)
#define BUGFIX
#endif

#endif
