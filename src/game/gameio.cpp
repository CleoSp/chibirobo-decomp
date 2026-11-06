#include "Dolphin/ar.h"

extern "C" {
int DVDSetAutoFatalMessaging(BOOL enable);
BOOL DVDChangeDir(const char* dirName);

u32 gAramBlockLengths[8];
extern const char sGameDVDDirectory[] = "/qp/";

void InitGameDVD()
{
	DVDSetAutoFatalMessaging(TRUE);
	DVDChangeDir(sGameDVDDirectory);
}

void InitGameARAM()
{
	ARInit(gAramBlockLengths, ARRAY_SIZE(gAramBlockLengths));
	ARQInit();
}
}
