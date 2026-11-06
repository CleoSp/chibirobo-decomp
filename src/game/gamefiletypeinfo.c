const char sTypeNameCFileText[]       = "CFileText";
const char sGameTypeNameDVDFileInfo[] = "DVDFileInfo";
#if defined(VERSION_GGTP01)
const char sGameTypeNameCFile[] = "CFile";
const char sTypeNameCPad[]      = "CPad";
#else
const char sGameTypeNameCFile[8] = "CFile\0"
                                   "00";
const char sTypeNameCPad[8]      = "CPad\0"
                                   "000";
#endif

const void* gGameTypeInfoDVDFileInfo[] = { sGameTypeNameDVDFileInfo, 0 };
const void* sGameBaseTypesCFile[]      = { gGameTypeInfoDVDFileInfo, 0, 0 };
const void* gGameTypeInfoCFile[]       = { sGameTypeNameCFile, sGameBaseTypesCFile };
const void* sBaseTypesCFileText[]      = { gGameTypeInfoDVDFileInfo, 0, gGameTypeInfoCFile, 0, 0 };
const void* gTypeInfoCFileText[]       = { sTypeNameCFileText, sBaseTypesCFileText };
const void* gTypeInfoCPad[]            = { sTypeNameCPad, 0 };
