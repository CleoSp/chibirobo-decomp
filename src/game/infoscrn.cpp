#include "game/particle_srt.h"
#include "game/infoscrn_shared.h"

// the loading bar, the robot walks along it
static const u8 sWalkAnim[] = { 3, 2, 1, 0, 3, 1, 2, 0xFF };
static struct {
	f32 mX;          // _00
	s32 mFrame;      // _04
	const u8* mAnim; // _08
} sLoading               = { 0.0f, 0, sWalkAnim };
static const char* sWide = "\x81\x40\x81\x49\x81\x68\x81\x94\x81\x90\x81\x93\x81\x95\x81\x66"
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
 * @note Address: 0x80065E44
 * @note Size: 0x29C
 */
extern "C" void fn_80065E44(u32 color, f32 x, f32 y, f32 w, f32 h, f32 rx, f32 ry)
{
	Vec pos[65];
	f32 right;
	f32 bottom;
	f32 sx;
	f32 sy;
	sx     = 9.0f * rx;
	sy     = 9.0f * ry;
	right  = x + w;
	bottom = y + h;
	for (int i = 0; i < 65; i++) {
		f32 cx;
		f32 cy;
		f32 s;
		f32 angle = 0.09817477f * i - 1.5707964f;
		s         = sin(angle);
		f32 c     = -(f32)cos(angle);
		if ((i >= 0 && i < 16) || i == 64) {
			cx = x;
			cy = y;
		} else if ((i >= 16 && i < 32) || i == 64) {
			cx = right;
			cy = y;
		} else if ((i >= 32 && i < 48) || i == 64) {
			cx = right;
			cy = bottom;
		} else {
			cx = x;
			cy = bottom;
		}
		pos[i].x = s * sx + cx - w / 2;
		pos[i].y = c * sy + cy - h / 2;
		pos[i].z = 0.0f;
	}
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, color, 65);
	fn_8019B464(GS_RENDER, 1, 7, 0);
}
extern "C" {
void fn_80198614(void* gs, u32 c0, u32 c1, u32 c2, u32 c3, f32 x0, f32 y0, f32 z0, f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32 x3,
                 f32 y3, f32 z3);
void fn_80198258(void* gs, s32 mode, u32 c0, u32 c1, f32 x, f32 y, f32 z, f32 w, f32 h, f32 a, f32 b, f32 c);
}

/**
 * @note Address: 0x800652C0
 * @note Size: 0xB84
 */
// NONMATCHING: vertex temporaries and floating-point register allocation
extern "C" void fn_800652C0(u32 color, u32 textureColor, f32 x, f32 y, f32 width, f32 height, f32 alpha, f32 scaleX, f32 scaleY)
{
	f32 angle  = 4.712389f * (1.0f - alpha);
	f32 sx     = 4.0f * (f32)sin(angle);
	f32 sy     = 4.0f * (f32)sin(angle);
	f32 left   = x - width / 2.0f - 40.0f - sx;
	f32 right  = sx + (40.0f + (x + width / 2.0f));
	f32 top    = y - 8.0f - sy;
	f32 bottom = sy + (8.0f + (y + height));
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198614(lbl_8025C400, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, left - 8.0f, top - 8.0f, -1.0f, 9.0f + (8.0f + right),
	            top - 8.0f, -1.0f, 9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f);
	Vec pos[10] = {
		(Vec) { x, y, 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (1.0f + top), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (1.0f + top), 0.0f },
		(Vec) { (right - 1.0f), (8.0f + (1.0f + top)), 0.0f },
		(Vec) { (right - 1.0f), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { ((right - 1.0f) - 8.0f), (bottom - 1.0f), 0.0f },
		(Vec) { (8.0f + (1.0f + left)), (bottom - 1.0f), 0.0f },
		(Vec) { (1.0f + left), ((bottom - 1.0f) - 8.0f), 0.0f },
		(Vec) { (1.0f + left), (8.0f + (1.0f + top)), 0.0f },
	};
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, color, 10);
	fn_8019B464(GS_RENDER, 1, 1, 1);
	fn_80198258(lbl_8025C400, 16, color, color, (8.0f + (1.0f + left)), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, ((right - 1.0f) - 8.0f), (8.0f + (1.0f + top)), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, (8.0f + (1.0f + left)), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, color, color, ((right - 1.0f) - 8.0f), ((bottom - 1.0f) - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f, 0.0f, 0.0f);
	u32 shadow = (u32)(64.0f * alpha);
	for (int i = 0; i < 10; i++) {
		pos[i].x += 9.0f;
		pos[i].y += 9.0f;
	}
	fn_80198D08(lbl_8025C400, 0xA0, GAME_44 + 0xE8, pos, shadow, 10);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), (8.0f + (1.0f + (9.0f + top))), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, (8.0f + (1.0f + (9.0f + left))), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f,
	            0.0f, 0.0f, 0.0f);
	fn_80198258(lbl_8025C400, 16, shadow, shadow, ((9.0f + right) - 1.0f - 8.0f), ((9.0f + bottom) - 1.0f - 8.0f), 0.0f, 16.0f, 16.0f, 0.0f,
	            0.0f, 0.0f);
	CTexObj tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[114], 0);
	f32 tw      = GXGetTexObjWidth(&tex);
	f32 th      = GXGetTexObjHeight(&tex);
	f32 w       = right - left - 128.0f;
	if (w < 0.0f)
		w = 0.0f;
	f32 h = bottom - top - 64.0f;
	if (h < 0.0f)
		h = 0.0f;
	f32 tiles[8][8] = {
		{ 0.0f, 0.0f, .5f, .5f, (32.0f + left - 3.0f), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ .5f, 0.0f, 1.0f / tw, .5f, x, (8.0f + ((16.0f + top) - 10.0f)), (6.0f + w), 32.0f },
		{ .5f, 0.0f, .5f, .5f, (3.0f + (right - 32.0f)), (8.0f + ((16.0f + top) - 10.0f)), 64.0f, 32.0f },
		{ 0.0f, .5f, .5f, 1.0f / th, (32.0f + left - 3.0f), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ .5f, .5f, .5f, 1.0f / th, (3.0f + (right - 32.0f)), (4.0f + (height * 0.5f + y)), 64.0f, ((20.0f + h) - 8.0f) },
		{ 0.0f, .5f, .5f, .5f, (32.0f + left - 3.0f), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
		{ .5f, .5f, 1.0f / tw, .5f, x, (10.0f + (bottom - 16.0f)), (6.0f + w), 32.0f },
		{ .5f, .5f, .5f, .5f, (3.0f + (right - 32.0f)), (10.0f + (bottom - 16.0f)), 64.0f, 32.0f },
	};
	fn_8019AFA0(GS_TEV, 0);
	fn_8019B464(GS_RENDER, 1, 1, 1);
	for (int i = 0; i < 8; i++) {
		PSMTXTrans(tex.mMtx[0], tiles[i][0], tiles[i][1], 0.0f);
		tex.mMtxDirty = TRUE;
		PSMTXScale(tex.mMtx[1], tiles[i][2], tiles[i][3], 1.0f);
		tex.mMtxDirty = TRUE;
		fn_80197800(lbl_8025C400, &tex, textureColor, textureColor, textureColor, textureColor, tiles[i][4], tiles[i][5], 1.0f, tiles[i][6],
		            tiles[i][7], 0.0f, 0.0f, 0.0f);
	}
	fn_8019AFA0(GS_TEV, 4);
	fn_8019B464(GS_RENDER, 1, 7, 1);
	fn_80198614(lbl_8025C400, 0xFF000000, 0xFF000000, 0xFF000000, 0xFF000000, left - 8.0f, top - 8.0f, -1.0f, 9.0f + (8.0f + right),
	            top - 8.0f, -1.0f, 9.0f + (8.0f + right), 9.0f + (8.0f + bottom), -1.0f, left - 8.0f, 9.0f + (8.0f + bottom), -1.0f);
	fn_8019B464(GS_RENDER, 1, 3, 0);
}

/**
 * @note Address: 0x800650AC
 * @note Size: 0x214
 */
extern "C" void fn_800650AC(InfoPoint* self, s32 id)
{
	self->mId = id;
	self->_18 = 0;
	self->_08 = 0;
	self->_0C = 0;
	self->_10 = 0;
	self->_14 = 0.0f;
	for (int i = 0; i < 6; i++) {
		self->mPos[i].x = -220.0f;
	}
	for (int i = 0; i < 4; i++) {
		self->_1C[i] = 0.0f;
	}
	self->_74 = 0;
	self->_75 = 0;
	switch (self->mId) {
	case 0x9A:
		self->mKind     = self->mId;
		self->mPos[0].y = GetRight() - 64.0f - 40.0f;
		self->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case 0x98:
		self->mKind     = self->mId;
		self->mPos[0].y = GetRight() - 64.0f;
		self->mPos[1].y = GetRight() - 64.0f;
		break;
	case 0x99:
		self->mKind     = self->mId;
		self->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		self->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}
	self->_78 = 0;
	self->_7C = 0.0f;
	self->_80 = 0;
}

/**
 * @note Address: 0x800643A4
 * @note Size: 0xD08
 */
extern "C" void fn_800643A4(InfoPoint* self, u32 frames)
{
	switch (self->mId) {
	case 0x9A:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x964C);
		break;
	case 0x98:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x9660);
		break;
	case 0x99:
		self->_1C[1] = *(s32*)(GAME_PLAYER + 0x9664);
		break;
	}

	bool changed = false;
	if (self->_1C[1] - self->_1C[0]) {
		changed = true;
	}
	if (IsMenu3Open()) {
		self->_1C[0]    = self->_1C[1];
		self->_1C[2]    = self->_1C[3];
		self->mPos[1].x = -220.0f;
		self->mPos[0].x = -220.0f;
		self->mPos[3].x = -220.0f;
		self->mPos[2].x = -220.0f;
		self->mPos[5].x = -220.0f;
		self->mPos[4].x = -220.0f;
		self->_08       = 0;
		changed         = false;
	} else {
		f32 step = (self->_1C[1] - self->_1C[0]) / 4;
		if (step < 0.0f) {
			step = -step;
		}
		f32 speed = step;
		if (step <= 1.0f) {
			speed = 1.0f;
		}
		Approach(&self->_1C[0], &self->_1C[1], speed);
		Approach(&self->_1C[2], &self->_1C[3], 1.0f);
	}
	Approach(&self->mPos[0].x, &self->mPos[1].x, ApproachStep(self->mPos[0].x, self->mPos[1].x));
	Approach(&self->mPos[2].x, &self->mPos[3].x, ApproachStep(self->mPos[2].x, self->mPos[3].x));
	Approach(&self->mPos[4].x, &self->mPos[5].x, ApproachStep(self->mPos[4].x, self->mPos[5].x));

	bool empty = false;
	if (0.0f == *(f32*)(GAME_PLAYER + 0xA30)) {
		empty = true;
	}
	if (self->_75) {
		empty = false;
	}
	switch (self->mId) {
	case 0x9A:
		self->mPos[0].y = GetRight() - 64.0f - 40.0f;
		self->mPos[1].y = GetRight() - 64.0f - 40.0f;
		break;
	case 0x98:
		self->mPos[0].y = GetRight() - 64.0f;
		self->mPos[1].y = GetRight() - 64.0f;
		break;
	case 0x99:
		self->mPos[0].y = 40.0f + (GetRight() - 64.0f);
		self->mPos[1].y = 40.0f + (GetRight() - 64.0f);
		break;
	}

	switch (self->_08) {
	case 0:
		if (IsMenu3Open()) {
			break;
		}
		if (empty) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		} else if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		}
		break;
	case 1:
		self->_10++;
		if (self->_10 > 6) {
			self->mPos[3].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 2:
		self->_10++;
		if (self->_10 > 6) {
			self->mPos[5].x = 60.0f;
			self->_08++;
		}
		break;
	case 3:
		if (!empty) {
			self->mPos[1].x = -220.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 4:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[5].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 5:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[3].x = -220.0f;
				self->_08       = 0;
			}
		}
		break;
	case 10:
		self->_10++;
		if (self->_10 > 3) {
			self->mPos[3].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 11:
		self->_10++;
		if (self->_10 > 3) {
			self->mPos[5].x = 60.0f;
			self->_10       = 0;
			self->_08++;
		}
		break;
	case 12:
		if (empty) {
			self->_08 = 3;
		} else {
			self->_10++;
			if (changed) {
				self->_10 = 0;
			}
			if (self->_10 > 90) {
				self->mPos[1].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 13:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[5].x = -220.0f;
				self->_10       = 0;
				self->_08++;
			}
		}
		break;
	case 14:
		if (changed) {
			self->mPos[1].x = 60.0f;
			self->_10       = 0;
			self->_08       = 10;
		} else {
			self->_10++;
			if (self->_10 > 6) {
				self->mPos[3].x = -220.0f;
				self->_08       = 0;
			}
		}
		break;
	}

	if (self->_74) {
		self->mPos[1].x = 60.0f;
		self->mPos[3].x = 60.0f;
		self->mPos[5].x = 60.0f;
		self->_10       = 0;
		self->_08       = 3;
	}

	switch (self->_0C) {
	case 0:
		self->_14 = 1.0f;
		break;
	case 1:
		self->_14 = 1.0f;
		break;
	case 2:
		self->_14 = 0.0f;
		break;
	case 3:
		self->_14 += 0.08f * frames;
		if (self->_14 >= 1.0f) {
			self->_14 = 1.0f;
		}
		if (GAME_EVT) {
			self->_0C = 4;
		}
		break;
	case 4:
		self->_14 -= 0.08f * frames;
		if (self->_14 < 0.0f) {
			self->_14 = 0.0f;
		}
		if (!GAME_EVT) {
			self->_0C = 3;
		}
		break;
	}

	self->_7C = 1.0f;
	switch (self->mId) {
	case 0x99:
		if (fn_80051C60()) {
			self->_78 = 0;
			self->_18 = 0;
			self->_7C = 1.0f;
			break;
		}
		switch (self->_78) {
		case 0:
			if (self->_1C[1] >= 300.0f) {
				self->_78++;
				self->_18 = 0;
			}
			break;
		case 1:
			self->_18 += frames;
			self->_7C = sin(0.017453292f * ((self->_18 * 6) % 360));
			if (self->_7C > 0.0f) {
				fn_8001E904(GAME_SOUND, 0x183, 0xFF);
				self->_78++;
			}
			break;
		case 2:
			self->_18 += frames;
			self->_7C = sin(0.017453292f * ((self->_18 * 6) % 360));
			if (self->_1C[1] < 300.0f) {
				self->_78 = 0;
			}
			break;
		case 3:
			if (self->_1C[1] < 300.0f) {
				self->_78 = 0;
			}
			break;
		}
		break;
	}
}

/**
 * @note Address: 0x8006389C
 * @note Size: 0xB08
 */
// NONMATCHING: float registers
extern "C" void fn_8006389C(InfoPoint* self)
{
	f32 alpha = self->_14 * sScale;
	if (alpha > 0.0f) {
		CTexObj tex;
		s32 len = 1;
		f32 x0  = self->mPos[0].x;
		f32 y0  = self->mPos[0].y;
		f32 x2  = self->mPos[2].x;
		f32 x4  = self->mPos[4].x;
		while (true) {
			if ((s32)(self->_1C[0] / Pow10(len)) == 0) {
				break;
			}
			len++;
		}

		Vec pos[65];
		f32 cy = y0 - 7.2f;
		for (int i = 0; i < 65; i++) {
			f32 s;
			f32 angle = 0.09817477f * i - 1.5707964f;
			s         = sin(angle);
			f32 c     = -(f32)cos(angle);
			pos[i].x  = x0 + s * 14.4f;
			pos[i].y  = cy + c * 14.4f;
			pos[i].z  = 0.0f;
		}
		f32 a255 = 255.0f * alpha;
		u32 a    = a255;
		fn_8019B34C(GS_RENDER, 0x12, 5);
		fn_801990EC(lbl_8025C400, pos, a, 65);
		fn_8019B34C(GS_RENDER, 12, 5);

		tex          = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
		u16 tw       = GXGetTexObjWidth(&tex);
		f32 lineY    = cy + 14.4f;
		self->_1C[3] = len * (1.2f * tw - 3.6f) + 34.0f;
		pos[0].x     = x0;
		pos[0].y     = lineY;
		pos[1].x     = x0 + self->_1C[2];
		pos[1].y     = lineY;
		fn_8019B34C(GS_RENDER, 0x12, 5);
		fn_801990EC(lbl_8025C400, pos, a, 2);
		fn_8019B34C(GS_RENDER, 12, 5);

		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[155], 0);
		fn_8019AFA0(GS_TEV, 0);
		u16 ew = GXGetTexObjWidth(&tex);
		u16 eh = GXGetTexObjHeight(&tex);
		u32 c  = a | 0xC8C8C800;
		fn_80197800(lbl_8025C400, &tex, c, c, c, c, pos[1].x, pos[1].y, 0.0f, 1.2f * ew, 1.2f * eh, 0.0f, 0.0f, 0.0f);
		fn_8019AFA0(GS_TEV, 4);

		tex = GAME_TEX->Init(&GAME_TPL->descriptorArray[self->mKind], 0);
		fn_8019AFA0(GS_TEV, 0);
		f32 iconW = 0.8f * GXGetTexObjWidth(&tex);
		f32 iconY = y0 - 9.0f;
		f32 iconH = 0.8f * GXGetTexObjHeight(&tex);
		if (self->mId == 0x9A) {
			u32 ic = (u32)(a255 * self->_7C) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, ic, ic, ic, ic, x4, 4.0f + iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		} else {
			u32 ic = (u32)(a255 * self->_7C) | 0xC8C8C800;
			fn_80197800(lbl_8025C400, &tex, ic, ic, ic, ic, x4, iconY, 0.0f, iconW, iconH, 0.0f, 0.0f, 0.0f);
		}

		tex    = GAME_TEX->Init(&GAME_TPL->descriptorArray[125], 0);
		f32 dx = x2 - 12.0f;
		f32 dy = y0 - 4.0f;
		f32 w  = 1.2f * GXGetTexObjWidth(&tex);
		f32 h  = 1.2f * (GXGetTexObjHeight(&tex) / 10);
		PSMTXScale(tex.mMtx[1], 1.0f, 0.1f, 1.0f);
		u8 da         = (s32)a255;
		f32 step      = 7.0f * w;
		f32 base      = len * (w - 3.6f) - 117.6f;
		u32 blueT     = da | 0xCECEFF00;
		u32 blueB     = da | 0xE037EE00;
		u32 goldT     = da | 0xEFEBC500;
		u32 goldB     = da | 0xEDA83600;
		u32 pinkB     = da | 0x5959FF00;
		tex.mMtxDirty = TRUE;
		for (int i = 0; i < 7; i++) {
			s32 v = (s32)(self->_1C[0] / Pow10(i));
			if (i != 0 && v == 0) {
				continue;
			}
			PSMTXTrans(tex.mMtx[0], 0.0f, 0.1f * (v % 10), 0.0f);
			tex.mMtxDirty = TRUE;
			fn_8019AFA0(GS_TEV, 0);
			u32 top;
			u32 bottom;
			switch (self->mId) {
			case 0x9A:
				top    = blueT;
				bottom = blueB;
				break;
			case 0x98:
				top    = goldT;
				bottom = goldB;
				break;
			case 0x99:
				top    = blueT;
				bottom = pinkB;
				break;
			}
			fn_80197800(lbl_8025C400, &tex, top, top, bottom, bottom, base + (step + (dx - i * (w - 3.6f))), dy - 2.0f, 0.0f, w, h, 0.0f,
			            0.0f, 0.0f);
			fn_8019AFA0(GS_TEV, 4);
		}
		fn_8019AFA0(GS_TEV, 4);
	}
}
