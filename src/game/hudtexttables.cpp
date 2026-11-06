#include <dolphin/types.h>
#include <string.h>

extern "C" {
extern const char sHudAsciiCharacters[96]
    = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
extern const char sHudFullWidthCharacters[192] = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
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

const char* gHudAsciiTable     = sHudAsciiCharacters;
const char* gHudFullWidthTable = sHudFullWidthCharacters;
}

/**
 * @note Address: 0x80066630
 * @note Size: 0xE4
 */
extern "C" s32 ConvertHudTextToFullWidth(char* text)
{
	char buf[0x200];
	char* out = buf;
	char* dst;
	char* p;
	s32 count = 0;
	for (p = text, dst = out; *p != '\0'; p++) {
		BOOL found = FALSE;
		for (s32 i = 0; gHudAsciiTable[i] != '\0'; i++) {
			if (*p == gHudAsciiTable[i]) {
				strncpy(dst, &gHudFullWidthTable[i * 2], 2)[2] = '\0';
				found                                          = TRUE;
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
