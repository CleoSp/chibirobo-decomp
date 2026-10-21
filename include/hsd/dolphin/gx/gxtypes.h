#ifndef _GXTYPES_h_
#define _GXTYPES_h_

#include "dolphin/types.h"

#define GX_FIFO_OBJSIZE 128

typedef enum _GXTexFmt {
	// Intensities (I) and RGB/RGBA.
	GX_TF_I4     = 0x0, // 4-bit I
	GX_TF_I8     = 0x1, // 8-bit I
	GX_TF_IA4    = 0x2, // 8-bit I + alpha (4+4).
	GX_TF_IA8    = 0x3, // 16-bit I + alpha (8+8).
	GX_TF_RGB565 = 0x4, // 16-bit RGB.
	GX_TF_RGB5A3 = 0x5, // MSB=1, RGB555 (opaque). MSB=0, RGBA4443 (transparent).
	GX_TF_RGBA8  = 0x6, // 32-bit RGB.
	GX_TF_CMPR   = 0xE, // Compressed 4-bit texel.

	// Z-texture format.
	GX_TF_Z8    = 0x11, // Unsigned 8-bit Z. For texture copies, specify the upper 8 bits of Z.
	GX_TF_Z16   = 0x13, // Unsigned 16-bit Z. For texture copies, specify the upper 16 bits of Z.
	GX_TF_Z24X8 = 0x16, // Unsigned 24-bit (32-bit texture) Z. For texture copies, copy the 24-bit Z and 0xff.

	// Copy-texture format.
	GX_CTF_R4    = 0x20, // 4-bit red. For copying 4 bits from red.
	GX_CTF_RA4   = 0x22, // 4-bit red + 4-bit alpha. For copying 4 bits from red, 4 bits from alpha.
	GX_CTF_RA8   = 0x23, // 8-bit red + 8-bit alpha. For copying 8 bits from red, 8 bits from alpha.
	GX_CTF_YUVA8 = 0x26, // 8-bit YUV + alpha. For copying 8 bits from YUV, 8 bits from alpha.
	GX_CTF_A8    = 0x27, // 8-bit alpha. For copying 8 bits from alpha.
	GX_CTF_R8    = 0x28, // 8-bit red. For copying 8 bits from red.
	GX_CTF_G8    = 0x29, // 8-bit green. For copying 8 bits from green.
	GX_CTF_B8    = 0x2A, // 8-bit blue. For copying 8 bits from blue.
	GX_CTF_RG8   = 0x2B, // 8-bit red +8-bit green. For copying 8 bits from red, 8 bits from green.
	GX_CTF_GB8   = 0x2C, // 8-bit green +8-bit blue. For copying 8 bits from green, 8 bits from blue.

	// Copy-Z-texture format.
	GX_CTF_Z4   = 0x30, // 4-bit Z. For copying the 4 upper bits from Z.
	GX_CTF_Z8M  = 0x39, // 8-bit Z (median byte). For copying the middle 8 bits of Z.
	GX_CTF_Z8L  = 0x3A, // 8-bit Z (lower byte). For copying the lower 8 bits of Z.
	GX_CTF_Z16L = 0x3C, // 16-bit Z (lower portion). For copying the lower 16 bits of Z.
} GXTexFmt;

typedef enum _GXTexMtx {
	GX_TEXMTX0  = 30,
	GX_TEXMTX1  = 33,
	GX_TEXMTX2  = 36,
	GX_TEXMTX3  = 39,
	GX_TEXMTX4  = 42,
	GX_TEXMTX5  = 45,
	GX_TEXMTX6  = 48,
	GX_TEXMTX7  = 51,
	GX_TEXMTX8  = 54,
	GX_TEXMTX9  = 57,
	GX_IDENTITY = 60
} GXTexMtx;

typedef enum _GXTexMapID {
	GX_TEXMAP0,
	GX_TEXMAP1,
	GX_TEXMAP2,
	GX_TEXMAP3,
	GX_TEXMAP4,
	GX_TEXMAP5,
	GX_TEXMAP6,
	GX_TEXMAP7,
	GX_MAX_TEXMAP,
	GX_TEXMAP_NULL = 0xff,
	GX_TEX_DISABLE = 0x100
} GXTexMapID;

typedef enum _GXPTTexMtx {
	GX_PTTEXMTX0  = 64,
	GX_PTTEXMTX1  = 67,
	GX_PTTEXMTX2  = 70,
	GX_PTTEXMTX3  = 73,
	GX_PTTEXMTX4  = 76,
	GX_PTTEXMTX5  = 79,
	GX_PTTEXMTX6  = 82,
	GX_PTTEXMTX7  = 85,
	GX_PTTEXMTX8  = 88,
	GX_PTTEXMTX9  = 91,
	GX_PTTEXMTX10 = 94,
	GX_PTTEXMTX11 = 97,
	GX_PTTEXMTX12 = 100,
	GX_PTTEXMTX13 = 103,
	GX_PTTEXMTX14 = 106,
	GX_PTTEXMTX15 = 109,
	GX_PTTEXMTX16 = 112,
	GX_PTTEXMTX17 = 115,
	GX_PTTEXMTX18 = 118,
	GX_PTTEXMTX19 = 121,
	GX_PTIDENTITY = 125
} GXPTTexMtx;

typedef struct _GXFifoObj {
	u8 pad[GX_FIFO_OBJSIZE];
} GXFifoObj;

typedef struct _GXRenderModeObj {
	u32 viTVMode;
	u16 fbWidth;
	u16 efbHeight;
	u16 xfbHeight;
	u16 viXOrigin;
	u16 viYOrigin;
	u16 viWidth;
	u16 viHeight;
	u32 xfbMode;
	u8 field_rendering;
	u8 aa;
	u8 sample_pattern[12][2];
	u8 vfilter[7];
} GXRenderModeObj;

typedef struct _GXLightObj {
	u32 dummy[16];
} GXLightObj;

typedef struct _GXColor {
	u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
	s16 r, g, b, a;
} GXColorS10;

// GXBool
#define GX_FALSE   0
#define GX_TRUE    1
#define GX_DISABLE 0
#define GX_ENABLE  1

#define GX_SRC_REG   0
#define GX_SRC_VTX   1
#define GX_TEV_SWAP0 0

typedef enum _GXAttr {
	GX_VA_PNMTXIDX   = 0,
	GX_VA_TEX0MTXIDX = 1,
	GX_VA_TEX1MTXIDX = 2,
	GX_VA_TEX2MTXIDX = 3,
	GX_VA_TEX3MTXIDX = 4,
	GX_VA_TEX4MTXIDX = 5,
	GX_VA_TEX5MTXIDX = 6,
	GX_VA_TEX6MTXIDX = 7,
	GX_VA_TEX7MTXIDX = 8,
	GX_VA_POS        = 9,
	GX_VA_NRM        = 10,
	GX_VA_CLR0       = 11,
	GX_VA_CLR1       = 12,
	GX_VA_TEX0       = 13,
	GX_VA_TEX1       = 14,
	GX_VA_TEX2       = 15,
	GX_VA_TEX3       = 16,
	GX_VA_TEX4       = 17,
	GX_VA_TEX5       = 18,
	GX_VA_TEX6       = 19,
	GX_VA_TEX7       = 20,
	GX_POS_MTX_ARRAY = 21,
	GX_NRM_MTX_ARRAY = 22,
	GX_TEX_MTX_ARRAY = 23,
	GX_LIGHT_ARRAY   = 24,
	GX_VA_NBT        = 25,
	GX_VA_MAXATTR    = 26,
	GX_VA_NULL       = 0xff,
} GXAttr;

// GXPixelFormat
#define GX_PF_RGB8_Z24   0
#define GX_PF_RGBA6_Z24  1
#define GX_PF_RGB565_Z16 2
#define GX_PF_Z24        3
#define GX_PF_Y8         4
#define GX_PF_U8         5
#define GX_PF_V8         6
#define GX_PF_YUV420     7

// GXCompressedZFormat
#define GX_ZC_LINEAR 0
#define GX_ZC_NEAR   1
#define GX_ZC_MID    2
#define GX_ZC_FAR    3

// GXGamma
#define GX_GM_1_0 0
#define GX_GM_1_7 1
#define GX_GM_2_2 2

#define GX_MAX_Z24 0x00ffffff

#define MAX_GXLIGHT 10

// the few texture enums the HSD code needs, values from the SDK
typedef enum _GXTevOp { GX_TEV_ADD, GX_TEV_SUB } GXTevOp;
typedef enum _GXTevBias { GX_TB_ZERO, GX_TB_ADDHALF, GX_TB_SUBHALF, GX_MAX_TEVBIAS } GXTevBias;
typedef enum _GXTevScale { GX_CS_SCALE_1, GX_CS_SCALE_2, GX_CS_SCALE_4, GX_CS_DIVIDE_2, GX_MAX_TEVSCALE } GXTevScale;
typedef enum _GXChannelID {
	GX_COLOR0,
	GX_COLOR1,
	GX_ALPHA0,
	GX_ALPHA1,
	GX_COLOR0A0,
	GX_COLOR1A1,
	GX_COLOR_ZERO,
	GX_ALPHA_BUMP,
	GX_ALPHA_BUMPN,
	GX_COLOR_NULL = 0xFF
} GXChannelID;
typedef enum _GXTexWrapMode { GX_CLAMP, GX_REPEAT, GX_MIRROR, GX_MAX_TEXWRAPMODE } GXTexWrapMode;
typedef enum _GXTexFilter { GX_NEAR, GX_LINEAR, GX_NEAR_MIP_NEAR, GX_LIN_MIP_NEAR, GX_NEAR_MIP_LIN, GX_LIN_MIP_LIN } GXTexFilter;
#define GX_TG_COLOR0 19

#endif
