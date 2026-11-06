#if defined(VERSION_GGTJ01_01)
const char sCardTestLoadLabel[8]   = "\x83\x8D\x81\x5B\x83\x68\0"
                                     "0";
const char sCardTestSaveLabel[8]   = "\x83\x5A\x81\x5B\x83\x75\0"
                                     "0";
const char sCardTestDeleteLabel[8] = "\x8D\xED\x8F\x9C\0"
                                     "000";
#elif defined(VERSION_GGTP01)
const char sCardTestLoadLabel[]   = "load";
const char sCardTestSaveLabel[]   = "save";
const char sCardTestDeleteLabel[] = "delete";
#else
const char sCardTestLoadLabel[8]   = "load\0"
                                     "000";
const char sCardTestSaveLabel[8]   = "save\0"
                                     "000";
const char sCardTestDeleteLabel[8] = "delete\0"
                                     "0";
#endif
