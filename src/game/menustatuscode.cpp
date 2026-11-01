#include "game/cgs.h"
#include "game/cgame.h"
#include <stdio.h>
#include <string.h>

extern "C" {
void fn_80057318(u32 color, f32 x, f32 y, f32 z, f32 width, f32 height, f32 opacity);
s32 fn_80066630(char* text);
}

// draws a code one character to a box, digits and the narrow letters nudged to the middle
/**
 * @note Address: 0x80057084
 * @note Size: 0x294
 */
extern "C" void fn_80057084(const char* text, f32 x, f32 y, f32 z, f32 width, f32 height, f32 opacity)
{
	u32 count          = strlen(text) / 2;
	const char* cursor = text;
	s32 index          = 0;
	u32 background     = (u32)(128.0f * opacity) | 0xFFFFFF00;
	u32 foreground     = (u32)(255.0f * opacity) | 0x20202000;
	for (; (u32)index < count; ++index, cursor += 2) {
		char character[3];
		char candidate[4];
		character[0]   = cursor[0];
		character[1]   = cursor[1];
		character[2]   = 0;
		f32 characterX = index * width + x;
		fn_80057318(background, characterX, y, z, width, height, opacity);
		f32 offset = 0.0f;
		for (s32 digit = 0; digit < 10; ++digit) {
			sprintf(candidate, "%d", digit);
			fn_80066630(candidate);
			if (strcmp(character, candidate) == 0) {
				offset = digit == 1 ? 6.0f : 3.0f;
				break;
			}
		}
		for (s32 letter = 0; letter < 26; ++letter) {
			candidate[0] = 'A' + letter;
			candidate[1] = 0;
			fn_80066630(candidate);
			if (strcmp(character, candidate) == 0) {
				switch (letter) {
				case 1:
				case 4:
				case 5:
				case 9:
				case 10:
				case 11:
				case 15:
				case 17:
				case 18:
				case 19:
					offset = 6.0f;
					break;
				case 8:
					offset = 12.0f;
					break;
				case 2:
				case 7:
				case 20:
				case 23:
					offset = 4.5f;
					break;
				default:
					offset = 3.0f;
					break;
				}
				break;
			}
		}
		fn_80196610(&lbl_8025C400, foreground, character, offset + characterX - width * 0.5f, y - height * 0.5f, 0.0f, width, height);
	}
}
