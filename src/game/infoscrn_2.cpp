#include "game/infoscrn_shared.h"

// the loading bar, the robot walks along it
static const u8 sWalkAnim[]  = { 3, 2, 1, 0, 3, 1, 2, 0xFF };
static const u8 sStandAnim[] = { 0, 1, 2, 3, 0xFF };
static struct {
	f32 mX;          // _00
	s32 mFrame;      // _04
	const u8* mAnim; // _08
} sLoading = { 0.0f, 0, sWalkAnim };

/**
 * @note Address: 0x80066714
 * @note Size: 0x268
 */
extern "C" void fn_80066714(InfoScrn* self)
{
	f32 alpha = self->_1C8;
	if (alpha > 0.0f) {
		fn_80199AB0(lbl_8025C400, 0, 0);
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_8019B464(GS_RENDER, 1, 7, 1);
		fn_8019B464(GS_RENDER, 1, 7, 0);
		u32 a = 255.0f * alpha;

		static f32 sScroll = 0.0f;
		if (!MENU_ACTIVE) {
			sScroll += 1.0f;
			if (sScroll >= 4.0f) {
				sScroll = 0.0f;
			}
		}

		fn_8019B56C(GS_RENDER, 3, 4, 5, 15);
		fn_8019B34C(GS_RENDER, 12, 5);
		u32 color = a | 0x30303000;
		for (s32 y = 0; y < 448; y += 4.0f) {
			fn_801992BC(lbl_8025C400, 0, color, color, 0.0f, y + sScroll, 0.0f, 640.0f, y + sScroll, 0.0f);
		}
		fn_8019B56C(GS_RENDER, 1, 4, 5, 15);
		fn_8019B34C(GS_RENDER, 12, 5);
		for (int i = 9; i > 0; i--) {
			sHistory[i] = sHistory[i - 1];
		}
		sHistory[0] = 0.0f;
		fn_8019B34C(GS_RENDER, 12, 5);
	}
}

static const char* sAscii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
static const char* sWide  = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
                            "\x81\x69\x81\x6A\x81\x96\x81\x7B\x81\x43\x81\x5B\x81\x44\x81\x5E"
                            "\x82\x4F\x82\x50\x82\x51\x82\x52\x82\x53\x82\x54\x82\x55\x82\x56"
                            "\x82\x57\x82\x58\x81\x46\x81\x47\x81\x83\x81\x81\x81\x84\x81\x48"
                            "\x81\x97\x82\x60\x82\x61\x82\x62\x82\x63\x82\x64\x82\x65\x82\x66"
                            "\x82\x67\x82\x68\x82\x69\x82\x6A\x82\x6B\x82\x6C\x82\x6D\x82\x6E"
                            "\x82\x6F\x82\x70\x82\x71\x82\x72\x82\x73\x82\x74\x82\x75\x82\x76"
                            "\x82\x77\x82\x78\x82\x79\x81\x6D\x81\x8F\x81\x6E\x81\x4F\x81\x51"
                            "\x81\x65\x82\x81\x82\x82\x82\x83\x82\x84\x82\x85\x82\x86\x82\x87"
                            "\x82\x88\x82\x89\x82\x8A\x82\x8B\x82\x8C\x82\x8D\x82\x8E\x82\x8F"
                            "\x82\x90\x82\x91\x82\x92\x82\x93\x82\x94\x82\x95\x82\x96\x82\x97"
                            "\x82\x98\x82\x99\x82\x9A\x81\x6F\x81\x62\x81\x70";

/**
 * @note Address: 0x80066630
 * @note Size: 0xE4
 */
extern "C" s32 fn_80066630(char* text)
{
	char buf[0x200];
	char* out = buf;
	char* dst;
	char* p;
	s32 count = 0;
	for (p = text, dst = out; *p != '\0'; p++) {
		BOOL found = FALSE;
		for (s32 i = 0; sAscii[i] != '\0'; i++) {
			if (*p == sAscii[i]) {
				strncpy(dst, &sWide[i * 2], 2)[2] = '\0';
				found                             = TRUE;
				count++;
				dst += 2;
				break;
			}
		}
		if (!found) {
			strncpy(dst, p, 1)[1] = '\0';
			dst++;
		}
	}
	strcpy(text, out);
	return count;
}

/**
 * @note Address: 0x800665FC
 * @note Size: 0x34
 */
extern "C" BOOL fn_800665FC(InfoScrn* self)
{
	return self->_1D4 == 10.0f * *(f32*)(GAME_PLAYER + 0x965C);
}

/**
 * @note Address: 0x800665E0
 * @note Size: 0x1C
 */
extern "C" BOOL fn_800665E0(InfoScrn* self)
{
	return self->_1D4 <= 0.0f;
}

/**
 * @note Address: 0x8006656C
 * @note Size: 0x74
 */
extern "C" void fn_8006656C(InfoScrn* self, f32 x)
{
	for (int i = 0; i < 5; i++) {
		self->_E0[i]  = 1.0f;
		self->_100[i] = 0;
		self->_104[i] = 0.0f;
		self->_114[i] = 0.0f;
	}
	self->_D0 = 10.0f * x;
	self->_CC = self->_D0;
}

/**
 * @note Address: 0x800664C4
 * @note Size: 0xA8
 */
extern "C" void fn_800664C4(InfoScrn* self, u8 on, s32 mode)
{
	switch (mode) {
	case 0:
		if (on) {
			self->_BC = 1;
		} else {
			self->_BC = 2;
		}
		break;
	case 1:
		if (on) {
			self->_BC = 3;
		} else {
			self->_BC = 4;
		}
		break;
	case 2:
		if (on) {
			self->_C8 = -52.0f;
		} else {
			self->_C8 = 0.0f;
		}
		break;
	case 3:
		if (on) {
			self->_BC = 5;
		} else {
			self->_BC = 6;
		}
		break;
	}
}
extern "C" {
void fn_80198614(void* gs, u32 c0, u32 c1, u32 c2, u32 c3, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3,
                 f32 y3, f32 z3);
void fn_80198258(void* gs, s32 mode, u32 c0, u32 c1, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
}
