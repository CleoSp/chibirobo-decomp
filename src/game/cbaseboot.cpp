#include "Version.h"
extern "C" {
#include <sysdolphin/initialize.h>
void* OSGetArenaLo();
void* OSGetArenaHi();
void OSSetArenaLo(void* arena);
void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps);
extern volatile OSHeapHandle __OSCurrHeap;
u32 OSGetResetCode();
void OSSetSaveRegion(void* start, void* end);
#if defined(VERSION_PAL)
extern GXRenderModeObj GXPal528IntDf;
#endif
}
#include "Dolphin/OS/OSFastCast.h"
#include "game/mm.h"
#include <stdlib.h>

// kept across resets in the os save region
struct ResetSaveRegion {
	u32 _00; // _00
	u32 _04; // _04
};
extern "C" ResetSaveRegion* lbl_8065B178;

/**
 * @note Address: 0x8001D8A0
 * @note Size: 0x20
 */
extern "C" void fn_8001D8A0(s32 framebuffer)
{
	HSD_VIDrawDoneXFB(framebuffer);
}

/**
 * @note Address: 0x8001D8C0
 * @note Size: 0xE8
 */
extern "C" void fn_8001D8C0()
{
	OSInitFastCast();
	srand(OSGetTick());
	void* arenaLo = OSGetArenaLo();
	void* arenaHi = OSGetArenaHi();
	OSSetArenaLo(OSInitAlloc(arenaLo, arenaHi, 1));
	HSD_SetInitParameter(HSD_INIT_FIFO_SIZE, 0x40000);
	HSD_SetInitParameter(HSD_INIT_XFB_MAX_NUM, 3);
#if defined(VERSION_PAL)
	HSD_SetInitParameter(HSD_INIT_RENDER_MODE_OBJ, &GXPal528IntDf);
#endif
	HSD_Init();
	mm::Init();
	lbl_8065B178 = (ResetSaveRegion*)OSAllocFromHeap(__OSCurrHeap, sizeof(ResetSaveRegion));
	if (!(OSGetResetCode() & 0x80000000)) {
		lbl_8065B178->_00 = 0;
		lbl_8065B178->_04 = 1;
	}
	OSSetSaveRegion(lbl_8065B178, lbl_8065B178 + 1);
}
