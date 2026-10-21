#ifndef _VERSION_H
#define _VERSION_H

// configure.py defines exactly one VERSION_<game id> flag. for version specific
// code test the flags with defined, never their values
#if defined(VERNUM) || defined(BUILDTARGET)
#error Use a VERSION_<game ID> flag instead of VERNUM or BUILDTARGET
#endif

#if (defined(VERSION_GGTE01) + defined(VERSION_GGTJ01_00) + defined(VERSION_GGTJ01_01) + defined(VERSION_GGTP01)) != 1
#error Define exactly one supported version: VERSION_GGTE01, VERSION_GGTJ01_00, VERSION_GGTJ01_01 or VERSION_GGTP01
#endif

// readable aliases. VERSION_JP covers both japanese revisions, test the rev
// flag itself when they differ
#if defined(VERSION_GGTE01)
#define VERSION_US
#elif defined(VERSION_GGTJ01_00) || defined(VERSION_GGTJ01_01)
#define VERSION_JP
#elif defined(VERSION_GGTP01)
#define VERSION_PAL
#endif

#endif
