#pragma section RX ".text.eventscreen"
#include "game/cgame.h"
#include "game/infoscrn.h"
#include "game/eventhudcommands.h"
#include "game/video.h"
#include "game/gmath.h"

extern "C" {
#include "game/eventconstantpool.inc"
extern char sEvtCommandSCRNName[];
s32 CEvt_Evaluate(CEvt* evt);
void OSPanic(const char* file, int line, const char* message, ...);
}

#if defined(VERSION_PAL)
#define FRAMES_PER_SEC gVideoFramesPerSecond
#else
#define FRAMES_PER_SEC 60
#endif

extern "C" {
void InfoScrn_ApplyBatteryHudCommand(void* hud, u8 enabled, s32 mode);
void InfoScrn_ApplyTimerHudCommand(void* hud, u8 enabled, s32 mode);
void InfoScrn_ShowTimer(void* hud);
void InfoScrn_HideTimer(void* hud);
void InfoScrn_SetTimerHundredths(void* hud, s32 value);
s32 InfoScrn_GetTimerHundredths(void* hud);
}

/**
 * @note Address: 0x80072224
 * @note Size: 0xEF8
 */
extern "C" __declspec(section ".text.eventscreen") s32 CEvt__cmd_SCRN(CEvt* evt)
{
	u16 mode = CEvt_Evaluate(evt);
	switch (mode) {
	case EVT_ATOM_FADE: {
		u8 r0    = CEvt_Evaluate(evt);
		u8 g0    = CEvt_Evaluate(evt);
		u8 b0    = CEvt_Evaluate(evt);
		u8 a0    = CEvt_Evaluate(evt);
		u8 r1    = CEvt_Evaluate(evt);
		u8 g1    = CEvt_Evaluate(evt);
		u8 b1    = CEvt_Evaluate(evt);
		u8 a1    = CEvt_Evaluate(evt);
		f32 time = CEvt_Evaluate(evt) / 100.0f;
		StartScreenFade(&gGame.mFade, r0, g0, b0, a0, r1, g1, b1, a1, time);
		break;
	}
	case EVT_ATOM_MONO: {
		u8 r0    = CEvt_Evaluate(evt);
		u8 g0    = CEvt_Evaluate(evt);
		u8 b0    = CEvt_Evaluate(evt);
		u8 a0    = CEvt_Evaluate(evt);
		u8 r1    = CEvt_Evaluate(evt);
		u8 g1    = CEvt_Evaluate(evt);
		u8 b1    = CEvt_Evaluate(evt);
		u8 a1    = CEvt_Evaluate(evt);
		f32 time = CEvt_Evaluate(evt) / 100.0f;
		StartScreenFade(&gGame.mColor0.mColor, r0, g0, b0, a0, r1, g1, b1, a1, time);
		break;
	}
	case EVT_ATOM_WIPE: {
		s32 type = CEvt_Evaluate(evt);
		s32 open = CEvt_Evaluate(evt);
		s32 x    = CEvt_Evaluate(evt);
		s32 y    = CEvt_Evaluate(evt);
		s32 w0   = CEvt_Evaluate(evt);
		s32 h0   = CEvt_Evaluate(evt);
		u8 r0    = CEvt_Evaluate(evt);
		u8 g0    = CEvt_Evaluate(evt);
		u8 b0    = CEvt_Evaluate(evt);
		u8 a0    = CEvt_Evaluate(evt);
		s32 w1   = CEvt_Evaluate(evt);
		s32 h1   = CEvt_Evaluate(evt);
		u8 r1    = CEvt_Evaluate(evt);
		u8 g1    = CEvt_Evaluate(evt);
		u8 b1    = CEvt_Evaluate(evt);
		u8 a1    = CEvt_Evaluate(evt);
		f32 time = CEvt_Evaluate(evt) / 100.0f;
		switch (type) {
		case 0:
			StartScreenWipeColorTransition(&gGame.mWipeCircle, r0, g0, b0, a0, r1, g1, b1, x, y, w0, h0, w1, h1, time, a1, !open);
			break;
		case 1:
			StartScreenWipeColorTransition(&gGame.mWipeTile0, r0, g0, b0, a0, r1, g1, b1, x, y, w0, h0, w1, h1, time, a1, !open);
			break;
		default:
			if (type >= 2000) {
				if (open) {
					StartScreenWipeColorTransition(&gGame.mWipeCustom, r0, g0, b0, a0, r1, g1, b1, x, y, 57.0f, 57.0f, 0.0f, 0.0f, time, a1,
					                               !open);
				} else {
					StartScreenWipeColorTransition(&gGame.mWipeCustom, r0, g0, b0, a0, r1, g1, b1, x, y, 0.0f, 0.0f, 57.0f, 57.0f, time, a1,
					                               !open);
				}
			} else if (type >= 1000) {
				TPLPalette* palette = (TPLPalette*)(&gGame.mTpl)->mBuffer;
				CTexObj* texture    = &(&gGame.mTpl)->mTex;
				gGame.mWipeTex.mTex = texture->Init(&palette->descriptorArray[type - 1000], 0);
				StartScreenWipeColorTransition(&gGame.mWipeTex, r0, g0, b0, a0, r1, g1, b1, x, y, w0, h0, w1, h1, time, a1, !open);
			} else {
#if defined(VERSION_PAL)
				const s32 line = 3542;
#else
				const s32 line = 3531;
#endif
				OSPanic("evt.cpp", line, "%s%s", FormatToStaticBuffer(sEvtScrnWipeUnresolved, type), sEvtCommandSCRNName);
			}
			break;
		}
		break;
	}
	case EVT_ATOM_LETTERBOX: {
		s32 open    = CEvt_Evaluate(evt);
		s32 height0 = open ? 448 : 314;
		u8 r0       = CEvt_Evaluate(evt);
		u8 g0       = CEvt_Evaluate(evt);
		u8 b0       = CEvt_Evaluate(evt);
		u8 a0       = CEvt_Evaluate(evt);
		s32 height1 = open ? 314 : 448;
		u8 r1       = CEvt_Evaluate(evt);
		u8 g1       = CEvt_Evaluate(evt);
		u8 b1       = CEvt_Evaluate(evt);
		u8 a1       = CEvt_Evaluate(evt);
		f32 time    = CEvt_Evaluate(evt) / 100.0f;
		StartScreenWipeColorTransition(&gGame.mWipeTile1, r0, g0, b0, a0, r1, g1, b1, 320.0f, 200.0f, 640.0f, height0, 640.0f, height1,
		                               time, a1, !open);
		break;
	}
	case EVT_ATOM_HUD:
		switch (CEvt_Evaluate(evt)) {
		case EVT_HUD_BATTERY: {
			s32 index               = CEvt_Evaluate(evt);
			EvtHudTable<8> commands = sEvtBatteryHudCommands;
			InfoScrn_ApplyBatteryHudCommand(&gGame.mInfoScrn, commands.entries[index].enabled, commands.entries[index].mode);
			break;
		}
		case EVT_HUD_CLOCK: {
			s32 index               = CEvt_Evaluate(evt);
			EvtHudTable<8> commands = sEvtClockHudCommands;
			InfoScrn_ApplyClockHudCommand(&gGame.mInfoScrn, commands.entries[index].enabled, commands.entries[index].mode, 0);
			break;
		}
		case EVT_HUD_SCORE: {
			s32 index               = CEvt_Evaluate(evt);
			EvtHudTable<4> commands = sEvtScoreHudCommands;
			InfoScrn_ApplyScoreHudCommand(&gGame.mInfoScrn, commands.entries[index].enabled, commands.entries[index].mode);
			break;
		}
		case 3:
			CEvt_Evaluate(evt);
			CEvt_Evaluate(evt);
			CEvt_Evaluate(evt);
			CEvt_Evaluate(evt);
			break;
		case EVT_HUD_TIMER: {
			s32 index = CEvt_Evaluate(evt);
			if (index >= 0) {
				EvtHudTable<4> commands = sEvtTimerHudCommands;
				InfoScrn_ApplyTimerHudCommand(&gGame.mInfoScrn, commands.entries[index].enabled, commands.entries[index].mode);
			} else {
				switch (index) {
				case -1:
					InfoScrn_ShowTimer(&gGame.mInfoScrn);
					break;
				case -2:
					InfoScrn_HideTimer(&gGame.mInfoScrn);
					break;
				case -3: {
					s32 hundredths = CEvt_Evaluate(evt);
					InfoScrn_SetTimerHundredths(&gGame.mInfoScrn, hundredths);
					break;
				}
				case -4:
					evt->_10 = InfoScrn_GetTimerHundredths(&gGame.mInfoScrn);
					break;
				}
			}
			break;
		}
		}
		break;
	case EVT_ATOM_ZBLUR: {
		u8 a0     = CEvt_Evaluate(evt);
		u8 a1     = CEvt_Evaluate(evt);
		s32 near  = CEvt_Evaluate(evt);
		s32 far   = CEvt_Evaluate(evt);
		f32 time  = CEvt_Evaluate(evt) / 100.0f;
		f32 start = a0 / 255.0f;
		f32 end   = a1 / 255.0f;
		f32 step;
		if (time == 0.0f) {
			start = end;
			step  = 0.0f;
		} else {
			step = (end - start) / (FRAMES_PER_SEC * time);
		}
		if (start >= 0.0f)
			gGame.mBlur.mAlpha = start;
		if (end >= 0.0f)
			gGame.mBlur.mTarget = end;
		gGame.mBlur.mSpeed   = Abs(step);
		gGame.mBlur.mOffsetX = near;
		gGame.mBlur.mOffsetY = far;
		break;
	}
	case EVT_ATOM_SHAKE: {
		f32 x0   = CEvt_Evaluate(evt) / 100.0f;
		f32 y0   = CEvt_Evaluate(evt) / 100.0f;
		f32 z0   = CEvt_Evaluate(evt) / 100.0f;
		f32 x1   = CEvt_Evaluate(evt) / 100.0f;
		f32 y1   = CEvt_Evaluate(evt) / 100.0f;
		f32 z1   = CEvt_Evaluate(evt) / 100.0f;
		f32 time = CEvt_Evaluate(evt) / 100.0f;
		StartScreenShake(&gGame.mQuake, x0, y0, z0, x1, y1, z1, time);
		break;
	}
	default:
#if defined(VERSION_PAL)
		const s32 line = 3737;
#else
		const s32 line = 3726;
#endif
		OSPanic("evt.cpp", line, "%s%s", FormatToStaticBuffer(sEvtScrnUnresolved, mode), sEvtCommandSCRNName);
		break;
	}
	return 0;
}
