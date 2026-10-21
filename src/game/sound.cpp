#include <types.h>
#include <string.h>
#include <dolphin/os.h>
#include <dolphin/mtx.h>
#include "Version.h"

// the sound object in CGame at +0xE7E24
struct CSound {
	u8 _00[0xFC];           // _00
	s32 _FC;                // _FC, default track, fn_8001E11C also writes 1 or 0 here, weird
	s32 _100;               // _100
	u8 _104[0x108 - 0x104]; // _104
	s32 _108[32];           // _108, ids that fn_8001E904 started, zero means free
	u8 _188[0x2B4 - 0x188]; // _188
	s32 _2B4;               // _2B4
	u8 _2B8[0x2C0 - 0x2B8]; // _2B8
	s32 _2C0;               // _2C0
	s32 _2C4;               // _2C4, fade length, frames * 60 / 200
	s32 _2C8;               // _2C8
	u8 _2CC;                // _2CC, volume used for the all-sfx id
	u8 _2CD[0x2E0 - 0x2CD]; // _2CD
	// 16 slots of names waiting to load, fn_8001F46C feeds them to HSD_SynthSFXLoad
	struct {
		const char* _00; // _00, name
		s32 _04;         // _04
		s32 _08;         // _08, load result
		u8 _0C;          // _0C, loaded flag
	} _2E0[16];          // _2E0
	s32 _3E0;            // _3E0, pending count

	// skips names that are already in, otherwise takes the first free slot
	inline void registerName(const char* name)
	{
		s32 val = _100;
		if (name == NULL) {
			return;
		}
		for (u32 i = 0; i < 16; i++) {
			if (_2E0[i]._00 != NULL && strcmp(_2E0[i]._00, name) == 0) {
				return;
			}
		}
		for (u32 i = 0; i < 16; i++) {
			if (_2E0[i]._00 == NULL) {
				_3E0++;
				_2E0[i]._00 = name;
				_2E0[i]._04 = val;
				OSReport("Register:%s\n", name);
				return;
			}
		}
		OSReport("Ssm::Load xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
	}

	// a negative track means the default one
	inline s32 resolveTrack(s32 track)
	{
		if (track < 0) {
			track = _FC;
		}
		return track;
	}
};

// the game object, CSound is at +0xE7E24 in it
extern "C" u8 lbl_8025C400[];
#if defined(VERSION_JP)
#define SOUND() ((CSound*)(lbl_8025C400 + 0xE713C))
#else
#define SOUND() ((CSound*)(lbl_8025C400 + 0xE7E24))
#endif

extern "C" {
BOOL fn_801E93A8(s32 track);
BOOL HSD_AudioSFXKeyOffTrack(s32 track);
BOOL fn_801E9724(s32 track);
void HSD_SynthSetSoundMode(s32 mode);
BOOL fn_801E62A8(s32 sfx);
void fn_801E9FA4(void);
BOOL fn_801EA028(u8 vol, u16 frames);
BOOL fn_801EA0C4(void);
BOOL fn_801EA134(void);
BOOL HSD_AudioSFXSetVolumeByID(s32 sfx, u8 vol, u16 frames);
BOOL fn_801E9604(s32 sfx);
s32 HSD_AudioSFXCountByID(s32 sfx);
}

extern "C" BOOL fn_8001E6C8(CSound* sound, s32 sfx);
extern "C" BOOL fn_8001E904(CSound* sound, s32 sfx, u8 vol);

// one entry per music track, the path to the .hps stream and its volume
struct MusicEntry {
	const char* _00; // _00, stream path
	u8 _04;          // _04, volume
	u8 _05[3];       // _05
};
extern "C" MusicEntry musicTable[];

// listener position in CGame, stage07 living reads the same spot
#if defined(VERSION_JP)
#define SOUND_LISTENER_POS() ((Vec*)(lbl_8025C400 + 0xE50F4))
#else
#define SOUND_LISTENER_POS() ((Vec*)(lbl_8025C400 + 0xE5DDC))
#endif

// distance falloff steps, far to near, one row per falloff kind
extern "C" f32 lbl_80210C10[][4];

// guess: ids with high half 0xFFFF (but not -1) mean "every sfx", stops all slots
static inline bool isAllSfx(s32 sfx)
{
	if (sfx == -1) {
		return false;
	}
	return (sfx & 0xFFFF0000) == 0xFFFF0000;
}

// a music id's volume from musicTable, 0 for anything that isn't music
static inline u8 getMusicVolume(s32 sfx)
{
	if (!isAllSfx(sfx)) {
		return 0;
	}
	u8 idx = sfx;
	if (idx > 0x6D) {
		idx = 0;
	}
	return musicTable[idx]._04;
}

// the every-sfx id goes to the master volume and remembers it, anything else to that one sfx
static inline void setSfxVolume(CSound* sound, s32 sfx, u8 vol, u16 frames)
{
	if (isAllSfx(sfx)) {
		sound->_2CC = vol;
		fn_801EA028(vol, frames);
	} else {
		HSD_AudioSFXSetVolumeByID(sfx, vol, frames);
	}
}

/**
 * @note Address: 0x8001DC20
 * @note Size: 0x2E0
 */
// fn_8001DC20
/*
    stwu     r1, -0x40(r1)
    mflr     r0
    stw      r0, 0x44(r1)
    stfd     f31, 0x30(r1)
    psq_st   f31, 0x38(r1), 0, qr0
    stmw     r26, 0x18(r1)
    fmr      f31, f1
    lfs      f0, lbl_8065C08C@sda21(r0)
    lis      r5, lbl_802107A0@ha
    mr       r31, r3
    mr       r26, r4
    fmuls    f1, f0, f31
    addi     r30, r5, lbl_802107A0@l
    bl       __cvt_fp2unsigned
    lfs      f0, lbl_8065C090@sda21(r0)
    mr       r29, r3
    fcmpo    cr0, f31, f0
    bge      .L_8001DC94
    lis      r0, 0x4330
    stw      r29, 0xc(r1)
    lfd      f2, lbl_8065C0A8@sda21(r0)
    stw      r0, 0x8(r1)
    lfs      f0, lbl_8065C094@sda21(r0)
    lfd      f1, 0x8(r1)
    fsubs    f1, f1, f2
    fmuls    f1, f1, f0
    bl       __cvt_fp2unsigned
    mr       r29, r3
    b        .L_8001DCC8
.L_8001DC94:
    lfs      f0, lbl_8065C098@sda21(r0)
    fcmpo    cr0, f31, f0
    bge      .L_8001DCC8
    lis      r0, 0x4330
    stw      r29, 0xc(r1)
    lfd      f2, lbl_8065C0A8@sda21(r0)
    stw      r0, 0x8(r1)
    lfd      f0, lbl_8065C0A0@sda21(r0)
    lfd      f1, 0x8(r1)
    fsub     f1, f1, f2
    fmul     f1, f1, f0
    bl       __cvt_fp2unsigned
    mr       r29, r3
.L_8001DCC8:
    cmplwi   r26, 0x21
    bgt      .L_8001DEE4
    lis      r3, jumptable_802112C4@ha
    slwi     r0, r26, 2
    addi     r3, r3, jumptable_802112C4@l
    lwzx     r0, r3, r0
    mtctr    r0
    bctr
.L_8001DCE8:
    addi     r28, r30, 0x6c4
    li       r27, 0x7
    b        .L_8001DE6C
.L_8001DCF4:
    addi     r28, r30, 0x9ac
    li       r27, 0x5
    b        .L_8001DE6C
.L_8001DD00:
    addi     r28, r30, 0x6e0
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD0C:
    addi     r28, r30, 0x730
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD18:
    addi     r28, r30, 0x708
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD24:
    addi     r28, r30, 0x794
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD30:
    addi     r28, r30, 0x780
    li       r27, 0x5
    b        .L_8001DE6C
.L_8001DD3C:
    addi     r28, r30, 0x758
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD48:
    addi     r28, r30, 0x7bc
    li       r27, 0x5
    b        .L_8001DE6C
.L_8001DD54:
    addi     r28, r30, 0x7e8
    li       r27, 0x5
    b        .L_8001DE6C
.L_8001DD60:
    addi     r28, r30, 0x7fc
    li       r27, 0x6
    b        .L_8001DE6C
.L_8001DD6C:
    addi     r28, r30, 0x814
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD78:
    li       r28, lbl_8065A1C0@sda21
    li       r27, 0x2
    b        .L_8001DE6C
.L_8001DD84:
    addi     r28, r30, 0x83c
    li       r27, 0xc
    b        .L_8001DE6C
.L_8001DD90:
    addi     r28, r30, 0x86c
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DD9C:
    addi     r28, r30, 0x894
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDA8:
    addi     r28, r30, 0x984
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDB4:
    addi     r28, r30, 0x8bc
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDC0:
    addi     r28, r30, 0x8e4
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDCC:
    addi     r28, r30, 0x90c
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDD8:
    addi     r28, r30, 0x934
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDE4:
    addi     r28, r30, 0x95c
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DDF0:
    addi     r28, r30, 0x9c0
    li       r27, 0x6
    b        .L_8001DE6C
.L_8001DDFC:
    addi     r28, r30, 0xa00
    li       r27, 0xb
    b        .L_8001DE6C
.L_8001DE08:
    addi     r28, r30, 0xa2c
    li       r27, 0xc
    b        .L_8001DE6C
.L_8001DE14:
    addi     r28, r30, 0x9d8
    li       r27, 0xa
    b        .L_8001DE6C
.L_8001DE20:
    addi     r28, r30, 0xa5c
    li       r27, 0x6
    b        .L_8001DE6C
.L_8001DE2C:
    addi     r28, r30, 0x7d0
    li       r27, 0x6
    b        .L_8001DE6C
.L_8001DE38:
    addi     r28, r30, 0xa74
    li       r27, 0x8
    b        .L_8001DE6C
.L_8001DE44:
    addi     r28, r30, 0xa94
    li       r27, 0x4
    b        .L_8001DE6C
.L_8001DE50:
    addi     r28, r30, 0xaa4
    li       r27, 0x4
    b        .L_8001DE6C
.L_8001DE5C:
    addi     r28, r30, 0xab4
    li       r27, 0x6
    b        .L_8001DE6C
.L_8001DE68:
    b        .L_8001DEE4
.L_8001DE6C:
    cmpwi    r27, 0x0
    ble      .L_8001DEE4
    mr       r30, r28
    li       r26, 0x0
    b        .L_8001DE9C
.L_8001DE80:
    lwz      r4, 0x0(r30)
    mr       r3, r31
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001DEE4
    addi     r30, r30, 0x4
    addi     r26, r26, 0x1
.L_8001DE9C:
    cmpw     r26, r27
    blt      .L_8001DE80
    mr       r3, r27
    bl       HSD_Randi
    cmplwi   r29, 0x200
    blt      .L_8001DEB8
    li       r29, 0x1ff
.L_8001DEB8:
    slwi     r30, r3, 2
    clrlwi   r5, r29, 24
    lwzx     r4, r28, r30
    mr       r3, r31
    bl       fn_8001E904
    cmplwi   r29, 0xff
    ble      .L_8001DEE4
    lwzx     r4, r28, r30
    mr       r3, r31
    li       r5, 0xff
    bl       fn_8001E904
.L_8001DEE4:
    psq_l    f31, 0x38(r1), 0, qr0
    lfd      f31, 0x30(r1)
    lmw      r26, 0x18(r1)
    lwz      r0, 0x44(r1)
    mtlr     r0
    addi     r1, r1, 0x40
    blr
*/

/**
 * @note Address: 0x8001DF00
 * @note Size: 0x10C
 */
extern "C" s32 fn_8001DF00(CSound* sound, Vec* pos, s32 kind)
{
	f32 dist   = PSVECDistance(SOUND_LISTENER_POS(), pos);
	f32* range = lbl_80210C10[kind];

	if (range[0] < dist) {
		return 0;
	}
	if (range[1] < dist) {
		return 127.0f - (64.0f * (dist - range[1])) / (range[0] - range[1]);
	}
	if (range[2] < dist) {
		return 171.0f - (64.0f * (dist - range[2])) / (range[1] - range[2]);
	}
	if (range[3] < dist) {
		return 255.0f - (64.0f * (dist - range[3])) / (range[2] - range[3]);
	}
	return 255;
}

/**
 * @note Address: 0x8001E00C
 * @note Size: 0x38
 */
extern "C" void fn_8001E00C(CSound* sound, s32 stereo)
{
	if (stereo == 1) {
		HSD_SynthSetSoundMode(1);
	} else {
		HSD_SynthSetSoundMode(0);
	}
}

/**
 * @note Address: 0x8001E044
 * @note Size: 0xD8
 */
extern "C" BOOL fn_8001E044(CSound* sound, const char* name)
{
	sound->registerName(name);
	return FALSE;
}
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stmw     r27, 0xc(r1)
    mr.      r28, r4
    mr       r27, r3
    lwz      r30, 0x100(r3)
    beq      .L_8001E104
    li       r31, 0x0
    mr       r29, r27
.L_8001E06C:
    lwz      r3, 0x2e0(r29)
    cmplwi   r3, 0x0
    beq      .L_8001E088
    mr       r4, r28
    bl       strcmp
    cmpwi    r3, 0x0
    beq      .L_8001E104
.L_8001E088:
    addi     r31, r31, 0x1
    addi     r29, r29, 0x10
    cmplwi   r31, 0x10
    blt      .L_8001E06C
    li       r0, 0x10
    mr       r4, r27
    li       r3, 0x0
    mtctr    r0
.L_8001E0A8:
    lwz      r0, 0x2e0(r4)
    cmplwi   r0, 0x0
    bne      .L_8001E0E8
    lwz      r5, 0x3e0(r27)
    slwi     r0, r3, 4
    lis      r3, lbl_801FED84@ha
    mr       r4, r28
    addi     r6, r5, 0x1
    add      r5, r27, r0
    stw      r6, 0x3e0(r27)
    addi     r3, r3, lbl_801FED84@l
    stw      r28, 0x2e0(r5)
    stw      r30, 0x2e4(r5)
    crclr    cr1eq
    bl       OSReport
    b        .L_8001E104
.L_8001E0E8:
    addi     r4, r4, 0x10
    addi     r3, r3, 0x1
    bdnz     .L_8001E0A8
    lis      r3, lbl_801FED94@ha
    addi     r3, r3, lbl_801FED94@l
    crclr    cr1eq
    bl       OSReport
.L_8001E104:
    lmw      r27, 0xc(r1)
    li       r3, 0x0
    lwz      r0, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x8001E11C
 * @note Size: 0x88
 */
extern "C" void fn_8001E11C(CSound* sound, BOOL on)
{
	if (on) {
		CSound* game = SOUND();
		fn_801E93A8(game->resolveTrack(0));
		fn_801EA0C4();
		game->_FC = 1;
	} else {
		CSound* game = SOUND();
		fn_801E9724(game->resolveTrack(0));
		fn_801EA134();
		game->_FC = 0;
	}
}

/**
 * @note Address: 0x8001E1A4
 * @note Size: 0x128
 */
extern "C" void fn_8001E1A4(CSound* sound, s32 sfx, u16 frames)
{
	if (isAllSfx(sfx)) {
		sound->_2C4 = (60.0f * frames) / 200.0f;
		setSfxVolume(sound, sfx, 0, frames);
	} else {
		setSfxVolume(sound, sfx, 0, frames);
	}
}

/**
 * @note Address: 0x8001E2CC
 * @note Size: 0x19C
 */
extern "C" void fn_8001E2CC(CSound* sound, s32 sfx, u16 frames)
{
	if (isAllSfx(sfx)) {
		fn_8001E904(sound, sfx, 0);
		setSfxVolume(sound, sfx, getMusicVolume(sfx), frames);
	} else {
		if (!fn_8001E6C8(sound, sfx)) {
			fn_8001E904(sound, sfx, 0xFF);
		}
		setSfxVolume(sound, sfx, 0xFF, frames);
	}
}
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stw      r31, 0x1c(r1)
    mr       r31, r4
    cmpwi    r31, -0x1
    stw      r30, 0x18(r1)
    mr       r30, r5
    stw      r29, 0x14(r1)
    mr       r29, r3
    bne      .L_8001E300
    li       r0, 0x0
    b        .L_8001E314
.L_8001E300:
    clrrwi   r3, r31, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E314:
    clrlwi.  r0, r0, 24
    beq      .L_8001E3D4
    mr       r3, r29
    mr       r4, r31
    li       r5, 0x0
    bl       fn_8001E904
    cmpwi    r31, -0x1
    bne      .L_8001E33C
    li       r0, 0x0
    b        .L_8001E350
.L_8001E33C:
    clrrwi   r3, r31, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E350:
    clrlwi.  r0, r0, 24
    bne      .L_8001E360
    li       r4, 0x0
    b        .L_8001E384
.L_8001E360:
    clrlwi   r0, r31, 24
    cmplwi   r0, 0x6d
    ble      .L_8001E370
    li       r0, 0x0
.L_8001E370:
    lis      r3, musicTable@ha
    clrlslwi r0, r0, 24, 3
    addi     r3, r3, musicTable@l
    add      r3, r3, r0
    lbz      r4, 0x4(r3)
.L_8001E384:
    cmpwi    r31, -0x1
    bne      .L_8001E394
    li       r0, 0x0
    b        .L_8001E3A8
.L_8001E394:
    clrrwi   r3, r31, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E3A8:
    clrlwi.  r0, r0, 24
    beq      .L_8001E3C4
    stb      r4, 0x2cc(r29)
    mr       r3, r4
    mr       r4, r30
    bl       fn_801EA028
    b        .L_8001E44C
.L_8001E3C4:
    mr       r3, r31
    mr       r5, r30
    bl       HSD_AudioSFXSetVolumeByID
    b        .L_8001E44C
.L_8001E3D4:
    mr       r3, r29
    mr       r4, r31
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001E3F8
    mr       r3, r29
    mr       r4, r31
    li       r5, 0xff
    bl       fn_8001E904
.L_8001E3F8:
    cmpwi    r31, -0x1
    bne      .L_8001E408
    li       r0, 0x0
    b        .L_8001E41C
.L_8001E408:
    clrrwi   r3, r31, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E41C:
    clrlwi.  r0, r0, 24
    beq      .L_8001E43C
    li       r0, 0xff
    mr       r4, r30
    stb      r0, 0x2cc(r29)
    li       r3, 0xff
    bl       fn_801EA028
    b        .L_8001E44C
.L_8001E43C:
    mr       r3, r31
    mr       r5, r30
    li       r4, 0xff
    bl       HSD_AudioSFXSetVolumeByID
.L_8001E44C:
    lwz      r0, 0x24(r1)
    lwz      r31, 0x1c(r1)
    lwz      r30, 0x18(r1)
    lwz      r29, 0x14(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x8001E468
 * @note Size: 0x6C
 */
extern "C" void fn_8001E468(CSound* sound, s32 sfx, u8 vol, u16 frames)
{
	setSfxVolume(sound, sfx, vol, frames);
}

/**
 * @note Address: 0x8001E4D4
 * @note Size: 0x30
 */
extern "C" void fn_8001E4D4(CSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->_FC;
	}
	fn_801E9724(track);
}

/**
 * @note Address: 0x8001E504
 * @note Size: 0x8C
 */
extern "C" void fn_8001E504(CSound* sound, s32 sfx)
{
	if (isAllSfx(sfx)) {
		if (sound->_2C8 == sound->_2B4) {
			fn_801EA134();
		} else {
			fn_8001E904(sound, sfx, 0xFF);
		}
		sound->_2C8 = 0;
	} else {
		fn_801E9604(sfx);
	}
}

/**
 * @note Address: 0x8001E590
 * @note Size: 0x30
 */
extern "C" void fn_8001E590(CSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->_FC;
	}
	fn_801E93A8(track);
}

/**
 * @note Address: 0x8001E5C0
 * @note Size: 0x70
 */
extern "C" void fn_8001E5C0(CSound* sound, s32 sfx, s32 arg2)
{
	if (isAllSfx(sfx)) {
		fn_801EA0C4();
		sound->_2C0 = arg2;
		sound->_2C8 = sound->_2B4;
	}
}

/**
 * @note Address: 0x8001E630
 * @note Size: 0x30
 */
extern "C" void fn_8001E630(CSound* sound, s32 track)
{
	if (track < 0) {
		track = sound->_FC;
	}
	HSD_AudioSFXKeyOffTrack(track);
}

/**
 * @note Address: 0x8001E660
 * @note Size: 0x68
 */
extern "C" BOOL fn_8001E660(CSound* sound, s32 sfx)
{
	if (isAllSfx(sfx)) {
		sound->_2B4 = 0;
		sound->_2C8 = 0;
		fn_801E9FA4();
		return TRUE;
	}
	return fn_801E62A8(sfx);
}

/**
 * @note Address: 0x8001E6C8
 * @note Size: 0x23C
 */
extern "C" BOOL fn_8001E6C8(CSound* sound, s32 sfx)
{
	// steps by two so it only ever looks at every other slot, nice one
	for (u32 i = 0; i < 32 && sound->_108[i] != 0; i += 2) {
		if (sound->_108[i] == sfx) {
			return TRUE;
		}
	}

	if (isAllSfx(sfx)) {
		if (sound->_2B4 == sfx) {
			return TRUE;
		}
		if (sfx == 0xFFFF0000 && sound->_2B4 != 0) {
			return TRUE;
		}
		return FALSE;
	}

	switch (sfx) {
	case 0x1B:
		if (HSD_AudioSFXCountByID(sfx) || HSD_AudioSFXCountByID(sfx + 1) || HSD_AudioSFXCountByID(sfx + 2)
		    || HSD_AudioSFXCountByID(sfx + 3)) {
			return TRUE;
		}
		return FALSE;
	case 0xFD:
		if (HSD_AudioSFXCountByID(0xFD) || HSD_AudioSFXCountByID(0xFE) || HSD_AudioSFXCountByID(0xFF) || HSD_AudioSFXCountByID(0x100)
		    || HSD_AudioSFXCountByID(0x101) || HSD_AudioSFXCountByID(0x102) || HSD_AudioSFXCountByID(0x103) || HSD_AudioSFXCountByID(0x104)
		    || HSD_AudioSFXCountByID(0x105) || HSD_AudioSFXCountByID(0x106) || HSD_AudioSFXCountByID(0x107) || HSD_AudioSFXCountByID(0x108)
		    || HSD_AudioSFXCountByID(0x109) || HSD_AudioSFXCountByID(0x10A) || HSD_AudioSFXCountByID(0x10B)) {
			return TRUE;
		}
		return FALSE;
	default:
		return HSD_AudioSFXCountByID(sfx);
	}
}
/*
    stwu     r1, -0x10(r1)
    mflr     r0
    li       r5, 0x0
    stw      r0, 0x14(r1)
    stw      r31, 0xc(r1)
    mr       r31, r4
    mr       r4, r3
    b        .L_8001E704
.L_8001E6E8:
    lwz      r0, 0x108(r4)
    cmpw     r0, r31
    bne      .L_8001E6FC
    li       r3, 0x1
    b        .L_8001E8F0
.L_8001E6FC:
    addi     r4, r4, 0x8
    addi     r5, r5, 0x2
.L_8001E704:
    cmplwi   r5, 0x20
    bge      .L_8001E718
    lwz      r0, 0x108(r4)
    cmpwi    r0, 0x0
    bne      .L_8001E6E8
.L_8001E718:
    cmpwi    r31, -0x1
    mr       r5, r31
    bne      .L_8001E72C
    li       r0, 0x0
    b        .L_8001E740
.L_8001E72C:
    clrrwi   r4, r31, 16
    lis      r0, 0xffff
    subf     r0, r4, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E740:
    clrlwi.  r0, r0, 24
    beq      .L_8001E780
    lwz      r3, 0x2b4(r3)
    cmpw     r3, r31
    bne      .L_8001E75C
    li       r3, 0x1
    b        .L_8001E8F0
.L_8001E75C:
    addis    r0, r31, 0x1
    cmplwi   r0, 0x0
    bne      .L_8001E778
    cmpwi    r3, 0x0
    beq      .L_8001E778
    li       r3, 0x1
    b        .L_8001E8F0
.L_8001E778:
    li       r3, 0x0
    b        .L_8001E8F0
.L_8001E780:
    cmpwi    r31, 0xfd
    beq      .L_8001E7E8
    bge      .L_8001E8E8
    cmpwi    r31, 0x1b
    beq      .L_8001E798
    b        .L_8001E8E8
.L_8001E798:
    mr       r3, r5
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E7D8
    addi     r3, r31, 0x1
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E7D8
    addi     r3, r31, 0x2
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E7D8
    addi     r3, r31, 0x3
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    beq      .L_8001E7E0
.L_8001E7D8:
    li       r3, 0x1
    b        .L_8001E8F0
.L_8001E7E0:
    li       r3, 0x0
    b        .L_8001E8F0
.L_8001E7E8:
    li       r3, 0xfd
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0xfe
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0xff
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x100
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x101
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x102
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x103
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x104
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x105
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x106
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x107
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x108
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x109
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x10a
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    bne      .L_8001E8D8
    li       r3, 0x10b
    bl       HSD_AudioSFXCountByID
    cmpwi    r3, 0x0
    beq      .L_8001E8E0
.L_8001E8D8:
    li       r3, 0x1
    b        .L_8001E8F0
.L_8001E8E0:
    li       r3, 0x0
    b        .L_8001E8F0
.L_8001E8E8:
    mr       r3, r5
    bl       HSD_AudioSFXCountByID
.L_8001E8F0:
    lwz      r0, 0x14(r1)
    lwz      r31, 0xc(r1)
    mtlr     r0
    addi     r1, r1, 0x10
    blr
*/

/**
 * @note Address: 0x8001E904
 * @note Size: 0xB68
 */
// fn_8001E904
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stmw     r26, 0x8(r1)
    mr       r28, r4
    lis      r4, lbl_802107A0@ha
    mr       r27, r3
    cmpwi    r28, -0x1
    mr       r29, r5
    addi     r31, r4, lbl_802107A0@l
    bne      .L_8001E938
    li       r0, 0x0
    b        .L_8001E94C
.L_8001E938:
    clrrwi   r3, r28, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001E94C:
    clrlwi.  r0, r0, 24
    beq      .L_8001EB9C
    addis    r0, r28, 0x1
    cmplwi   r0, 0x0
    bne      .L_8001E968
    li       r3, 0x0
    b        .L_8001F458
.L_8001E968:
    cmplwi   r0, 0x5
    bne      .L_8001E990
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x5d28(r3)
    cmpwi    r0, 0x1
    bne      .L_8001E990
    lis      r3, 0xffff
    addi     r28, r3, 0x10
.L_8001E990:
    addis    r0, r28, 0x1
    cmplwi   r0, 0x10
    bne      .L_8001EA28
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x5d28(r3)
    cmpwi    r0, 0x1
    bne      .L_8001E9CC
    lfs      f1, 0x5d2c(r3)
    lfs      f0, lbl_8065C090@sda21(r0)
    fcmpo    cr0, f1, f0
    ble      .L_8001E9CC
    li       r0, 0x0
    b        .L_8001E9D0
.L_8001E9CC:
    li       r0, 0x1
.L_8001E9D0:
    clrlwi.  r0, r0, 24
    bne      .L_8001EA28
    cmpwi    r28, -0x1
    mr       r3, r28
    bne      .L_8001E9EC
    li       r0, 0x0
    b        .L_8001EA00
.L_8001E9EC:
    clrrwi   r4, r28, 16
    lis      r0, 0xffff
    subf     r0, r4, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001EA00:
    clrlwi.  r0, r0, 24
    beq      .L_8001EA1C
    li       r0, 0x0
    stw      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
    bl       fn_801E9FA4
    b        .L_8001EA20
.L_8001EA1C:
    bl       fn_801E62A8
.L_8001EA20:
    li       r3, 0x1
    b        .L_8001F458
.L_8001EA28:
    clrlwi   r0, r28, 24
    cmplwi   r0, 0x6d
    mr       r3, r0
    blt      .L_8001EA3C
    li       r3, 0x0
.L_8001EA3C:
    lwz      r0, 0x2b4(r27)
    clrlslwi r3, r3, 24, 3
    addi     r26, r31, 0x108
    cmpw     r0, r28
    add      r26, r26, r3
    bne      .L_8001EA5C
    li       r3, 0x1
    b        .L_8001F458
.L_8001EA5C:
    addis    r0, r28, 0x1
    stw      r28, 0x2b4(r27)
    cmplwi   r0, 0x5
    bne      .L_8001EAB8
    lis      r3, lbl_8025C400@ha
    li       r30, 0x0
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xa48(r3)
    cmpwi    r0, 0x4
    bne      .L_8001EAA0
    addi     r3, r3, 0xa44
    bl       fn_800D4FE8
    cmpwi    r3, 0x0
    beq      .L_8001EAA0
    li       r30, 0x1
.L_8001EAA0:
    clrlwi.  r0, r30, 24
    beq      .L_8001EAB8
    li       r0, 0xc8
    sth      r0, 0x2ba(r27)
    sth      r0, 0x2b8(r27)
    b        .L_8001EAC4
.L_8001EAB8:
    li       r0, 0x0
    sth      r0, 0x2ba(r27)
    sth      r0, 0x2b8(r27)
.L_8001EAC4:
    li       r3, 0x0
    clrlwi   r0, r29, 24
    stw      r3, 0x2bc(r27)
    cmplwi   r0, 0xff
    stw      r3, 0x2c4(r27)
    bne      .L_8001EAE0
    lbz      r29, 0x4(r26)
.L_8001EAE0:
    stb      r29, 0x2cc(r27)
    mr       r4, r29
    li       r5, 0x1f
    lwz      r3, 0x0(r26)
    bl       HSD_AudioPStreamStartChParam
    mr       r29, r3
    lha      r3, 0x2b8(r27)
    bl       fn_801EA1A4
    addis    r0, r28, 0x1
    cmplwi   r0, 0x5
    beq      .L_8001EB14
    cmplwi   r0, 0x10
    bne      .L_8001EB94
.L_8001EB14:
    mr       r3, r27
    li       r4, 0x18c
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001EB50
    mr       r3, r27
    li       r4, 0x18d
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001EB50
    mr       r3, r27
    li       r4, 0x18e
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001EB94
.L_8001EB50:
    lwz      r0, 0x2b4(r27)
    cmpwi    r0, -0x1
    bne      .L_8001EB64
    li       r0, 0x0
    b        .L_8001EB78
.L_8001EB64:
    clrrwi   r3, r0, 16
    lis      r0, 0xffff
    subf     r0, r3, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001EB78:
    clrlwi.  r0, r0, 24
    beq      .L_8001EB94
    bl       fn_801EA0C4
    li       r0, 0x78
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001EB94:
    mr       r3, r29
    b        .L_8001F458
.L_8001EB9C:
    cmpwi    r28, 0x110
    lwz      r30, 0xfc(r27)
    beq      .L_8001F090
    bge      .L_8001EC38
    cmpwi    r28, 0x3c
    beq      .L_8001EFF4
    bge      .L_8001EC00
    cmpwi    r28, 0x15
    beq      .L_8001EEFC
    bge      .L_8001EBDC
    cmpwi    r28, 0x11
    beq      .L_8001EE04
    bge      .L_8001F3C4
    cmpwi    r28, 0xa
    beq      .L_8001EDF0
    b        .L_8001F3C4
.L_8001EBDC:
    cmpwi    r28, 0x1a
    beq      .L_8001F3C4
    bge      .L_8001EBF4
    cmpwi    r28, 0x19
    bge      .L_8001ED00
    b        .L_8001F3C4
.L_8001EBF4:
    cmpwi    r28, 0x1c
    bge      .L_8001F3C4
    b        .L_8001ECF0
.L_8001EC00:
    cmpwi    r28, 0x84
    bge      .L_8001EC20
    cmpwi    r28, 0x44
    beq      .L_8001F288
    bge      .L_8001F3C4
    cmpwi    r28, 0x3e
    beq      .L_8001EDF8
    b        .L_8001F3C4
.L_8001EC20:
    cmpwi    r28, 0xfd
    beq      .L_8001F010
    bge      .L_8001F3C4
    cmpwi    r28, 0x89
    bge      .L_8001F3C4
    b        .L_8001F1EC
.L_8001EC38:
    cmpwi    r28, 0x18c
    bge      .L_8001EC90
    cmpwi    r28, 0x179
    bge      .L_8001EC6C
    cmpwi    r28, 0x137
    beq      .L_8001F1B4
    bge      .L_8001EC60
    cmpwi    r28, 0x11d
    beq      .L_8001F268
    b        .L_8001F3C4
.L_8001EC60:
    cmpwi    r28, 0x175
    bge      .L_8001F120
    b        .L_8001F3C4
.L_8001EC6C:
    cmpwi    r28, 0x185
    beq      .L_8001F3C4
    bge      .L_8001EC84
    cmpwi    r28, 0x184
    bge      .L_8001F370
    b        .L_8001F3C4
.L_8001EC84:
    cmpwi    r28, 0x187
    bge      .L_8001F3C4
    b        .L_8001F354
.L_8001EC90:
    lis      r3, 0x4
    addi     r0, r3, 0x1e
    cmpw     r28, r0
    bge      .L_8001ECC8
    cmpwi    r28, 0x192
    bge      .L_8001ECB8
    cmpwi    r28, 0x18f
    beq      .L_8001F3C4
    bge      .L_8001F3A8
    b        .L_8001F288
.L_8001ECB8:
    addi     r0, r3, 0x1c
    cmpw     r28, r0
    bge      .L_8001F38C
    b        .L_8001F3C4
.L_8001ECC8:
    lis      r3, 0x11
    addi     r0, r3, 0x1c
    cmpw     r28, r0
    beq      .L_8001F338
    bge      .L_8001F3C4
    lis      r3, 0x10
    addi     r0, r3, 0x1f
    cmpw     r28, r0
    beq      .L_8001F3A8
    b        .L_8001F3C4
.L_8001ECF0:
    li       r3, 0x4
    bl       HSD_Randi
    add      r28, r28, r3
    b        .L_8001F3C4
.L_8001ED00:
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    addis    r3, r3, 0x1
    lwz      r28, 0x58c0(r3)
    cmpwi    r28, 0xd
    blt      .L_8001ED24
    li       r28, 0x0
.L_8001ED24:
    cmpwi    r28, 0xc
    bne      .L_8001ED34
    li       r0, -0x1
    b        .L_8001EDC4
.L_8001ED34:
    cmpwi    r28, 0x9
    beq      .L_8001ED74
    bge      .L_8001ED4C
    cmpwi    r28, 0x8
    bge      .L_8001ED58
    b        .L_8001EDA8
.L_8001ED4C:
    cmpwi    r28, 0xb
    bge      .L_8001EDA8
    b        .L_8001ED90
.L_8001ED58:
    li       r3, 0x19d
    li       r4, 0x46
    li       r5, 0x7f
    li       r6, 0x0
    li       r7, 0x0
    bl       HSD_AudioSFXStartParam
    b        .L_8001EDA8
.L_8001ED74:
    li       r3, 0x19c
    li       r4, 0xff
    li       r5, 0x7f
    li       r6, 0x0
    li       r7, 0x0
    bl       HSD_AudioSFXStartParam
    b        .L_8001EDA8
.L_8001ED90:
    li       r3, 0x19e
    li       r4, 0x32
    li       r5, 0x7f
    li       r6, 0x0
    li       r7, 0x0
    bl       HSD_AudioSFXStartParam
.L_8001EDA8:
    li       r3, 0x5
    bl       HSD_Randi
    mulli    r0, r28, 0x14
    slwi     r4, r3, 2
    addi     r3, r31, 0x4c0
    add      r0, r0, r4
    lwzx     r0, r3, r0
.L_8001EDC4:
    cmpwi    r0, 0x0
    mr       r28, r0
    bge      .L_8001EDD8
    li       r3, -0x1
    b        .L_8001F458
.L_8001EDD8:
    lwz      r0, 0x2c0(r27)
    cmpwi    r0, 0xa
    ble      .L_8001F3C4
    li       r0, 0xa
    stw      r0, 0x2c0(r27)
    b        .L_8001F3C4
.L_8001EDF0:
    li       r29, 0x0
    b        .L_8001F3C4
.L_8001EDF8:
    li       r0, 0x0
    stw      r0, 0x18c(r27)
    b        .L_8001F3C4
.L_8001EE04:
    li       r28, 0x0
    addi     r26, r31, 0x5d4
.L_8001EE0C:
    lwz      r4, 0x0(r26)
    mr       r3, r27
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001EE70
    lwz      r3, 0x0(r26)
    cmpwi    r3, -0x1
    bne      .L_8001EE34
    li       r0, 0x0
    b        .L_8001EE48
.L_8001EE34:
    clrrwi   r4, r3, 16
    lis      r0, 0xffff
    subf     r0, r4, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001EE48:
    clrlwi.  r0, r0, 24
    beq      .L_8001EE64
    li       r0, 0x0
    stw      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
    bl       fn_801E9FA4
    b        .L_8001EE68
.L_8001EE64:
    bl       fn_801E62A8
.L_8001EE68:
    li       r0, 0x0
    stw      r0, 0x18c(r27)
.L_8001EE70:
    addi     r28, r28, 0x1
    addi     r26, r26, 0x4
    cmplwi   r28, 0x4
    blt      .L_8001EE0C
    li       r28, 0x0
    addi     r26, r31, 0x5c4
.L_8001EE88:
    lwz      r4, 0x0(r26)
    mr       r3, r27
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001EEA4
    li       r0, -0x1
    b        .L_8001EEE8
.L_8001EEA4:
    addi     r28, r28, 0x1
    addi     r26, r26, 0x4
    cmplwi   r28, 0x4
    blt      .L_8001EE88
    lwz      r0, 0x18c(r27)
    cmpwi    r0, 0x0
    blt      .L_8001EEC8
    cmplwi   r0, 0x4
    blt      .L_8001EED0
.L_8001EEC8:
    li       r0, 0x0
    stw      r0, 0x18c(r27)
.L_8001EED0:
    lwz      r5, 0x18c(r27)
    addi     r3, r31, 0x5c4
    addi     r4, r5, 0x1
    slwi     r0, r5, 2
    stw      r4, 0x18c(r27)
    lwzx     r0, r3, r0
.L_8001EEE8:
    cmpwi    r0, 0x0
    mr       r28, r0
    bge      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001EEFC:
    li       r28, 0x0
    addi     r26, r31, 0x5c4
.L_8001EF04:
    lwz      r4, 0x0(r26)
    mr       r3, r27
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001EF68
    lwz      r3, 0x0(r26)
    cmpwi    r3, -0x1
    bne      .L_8001EF2C
    li       r0, 0x0
    b        .L_8001EF40
.L_8001EF2C:
    clrrwi   r4, r3, 16
    lis      r0, 0xffff
    subf     r0, r4, r0
    cntlzw   r0, r0
    srwi     r0, r0, 5
.L_8001EF40:
    clrlwi.  r0, r0, 24
    beq      .L_8001EF5C
    li       r0, 0x0
    stw      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
    bl       fn_801E9FA4
    b        .L_8001EF60
.L_8001EF5C:
    bl       fn_801E62A8
.L_8001EF60:
    li       r0, 0x0
    stw      r0, 0x18c(r27)
.L_8001EF68:
    addi     r28, r28, 0x1
    addi     r26, r26, 0x4
    cmplwi   r28, 0x4
    blt      .L_8001EF04
    li       r28, 0x0
    addi     r26, r31, 0x5d4
.L_8001EF80:
    lwz      r4, 0x0(r26)
    mr       r3, r27
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001EF9C
    li       r0, -0x1
    b        .L_8001EFE0
.L_8001EF9C:
    addi     r28, r28, 0x1
    addi     r26, r26, 0x4
    cmplwi   r28, 0x4
    blt      .L_8001EF80
    lwz      r0, 0x18c(r27)
    cmpwi    r0, 0x0
    blt      .L_8001EFC0
    cmplwi   r0, 0x4
    blt      .L_8001EFC8
.L_8001EFC0:
    li       r0, 0x0
    stw      r0, 0x18c(r27)
.L_8001EFC8:
    lwz      r5, 0x18c(r27)
    addi     r3, r31, 0x5d4
    addi     r4, r5, 0x1
    slwi     r0, r5, 2
    stw      r4, 0x18c(r27)
    lwzx     r0, r3, r0
.L_8001EFE0:
    cmpwi    r0, 0x0
    mr       r28, r0
    bge      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001EFF4:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F010:
    mr       r3, r27
    lis      r4, 0xffff
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F038
    bl       fn_801EA0C4
    li       r0, 0x78
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F038:
    mr       r3, r27
    li       r4, 0xfd
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F054
    li       r0, -0x1
    b        .L_8001F088
.L_8001F054:
    lwz      r0, 0x190(r27)
    cmpwi    r0, 0x0
    blt      .L_8001F068
    cmplwi   r0, 0x19
    blt      .L_8001F070
.L_8001F068:
    li       r0, 0x0
    stw      r0, 0x190(r27)
.L_8001F070:
    lwz      r5, 0x190(r27)
    addi     r3, r31, 0x5e4
    addi     r4, r5, 0x1
    slwi     r0, r5, 2
    stw      r4, 0x190(r27)
    lwzx     r0, r3, r0
.L_8001F088:
    mr       r28, r0
    b        .L_8001F3C4
.L_8001F090:
    lis      r4, 0xffff
    mr       r3, r27
    addi     r4, r4, 0x37
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F0B0
    li       r0, -0x1
    b        .L_8001F10C
.L_8001F0B0:
    mr       r3, r27
    lis      r4, 0xffff
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F0D8
    bl       fn_801EA0C4
    li       r0, 0x78
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F0D8:
    lwz      r0, 0x190(r27)
    cmpwi    r0, 0x0
    blt      .L_8001F0EC
    cmplwi   r0, 0x8
    blt      .L_8001F0F4
.L_8001F0EC:
    li       r0, 0x0
    stw      r0, 0x190(r27)
.L_8001F0F4:
    lwz      r5, 0x190(r27)
    addi     r3, r31, 0x648
    addi     r4, r5, 0x1
    slwi     r0, r5, 2
    stw      r4, 0x190(r27)
    lwzx     r0, r3, r0
.L_8001F10C:
    cmpwi    r0, 0x0
    mr       r28, r0
    bge      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F120:
    lis      r4, 0xffff
    mr       r3, r27
    addi     r4, r4, 0x5
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F14C
    bl       fn_801EA0C4
    li       r0, 0xb4
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F14C:
    lis      r4, 0xffff
    mr       r3, r27
    addi     r4, r4, 0x10
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F178
    bl       fn_801EA0C4
    li       r0, 0xb4
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F178:
    cmpwi    r28, 0x175
    bne      .L_8001F198
    li       r3, 0x6
    bl       HSD_Randi
    slwi     r0, r3, 2
    addi     r3, r31, 0x6ac
    lwzx     r0, r3, r0
    b        .L_8001F1AC
.L_8001F198:
    li       r3, 0x6
    bl       HSD_Randi
    slwi     r0, r3, 2
    addi     r3, r31, 0x694
    lwzx     r0, r3, r0
.L_8001F1AC:
    mr       r28, r0
    b        .L_8001F3C4
.L_8001F1B4:
    lwz      r0, 0x194(r27)
    cmpwi    r0, 0x0
    blt      .L_8001F1C8
    cmplwi   r0, 0xb
    blt      .L_8001F1D0
.L_8001F1C8:
    li       r0, 0x0
    stw      r0, 0x194(r27)
.L_8001F1D0:
    lwz      r5, 0x194(r27)
    addi     r3, r31, 0x668
    addi     r4, r5, 0x1
    slwi     r0, r5, 2
    stw      r4, 0x194(r27)
    lwzx     r28, r3, r0
    b        .L_8001F3C4
.L_8001F1EC:
    cmpwi    r28, 0x86
    beq      .L_8001F218
    bge      .L_8001F208
    cmpwi    r28, 0x84
    beq      .L_8001F238
    bge      .L_8001F230
    b        .L_8001F240
.L_8001F208:
    cmpwi    r28, 0x88
    beq      .L_8001F228
    bge      .L_8001F240
    b        .L_8001F220
.L_8001F218:
    li       r0, 0x0
    b        .L_8001F248
.L_8001F220:
    li       r0, 0x1
    b        .L_8001F248
.L_8001F228:
    li       r0, 0x2
    b        .L_8001F248
.L_8001F230:
    li       r0, 0x3
    b        .L_8001F248
.L_8001F238:
    li       r0, 0x4
    b        .L_8001F248
.L_8001F240:
    mr       r0, r28
    b        .L_8001F260
.L_8001F248:
    slwi     r0, r0, 2
    addi     r3, r31, 0xaf4
    lwzx     r0, r3, r0
    addi     r3, r31, 0xacc
    slwi     r0, r0, 2
    lwzx     r0, r3, r0
.L_8001F260:
    mr       r28, r0
    b        .L_8001F3C4
.L_8001F268:
    lwz      r0, 0x198(r27)
    cmpwi    r0, 0x0
    ble      .L_8001F27C
    li       r3, -0x1
    b        .L_8001F458
.L_8001F27C:
    li       r0, 0x3c
    stw      r0, 0x198(r27)
    b        .L_8001F3C4
.L_8001F288:
    lis      r4, 0xffff
    mr       r3, r27
    addi     r4, r4, 0x5
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F2B4
    bl       fn_801EA0C4
    li       r0, 0x78
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F2B4:
    lis      r4, 0xffff
    mr       r3, r27
    addi     r4, r4, 0x10
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F2E0
    bl       fn_801EA0C4
    li       r0, 0x78
    stw      r0, 0x2c0(r27)
    lwz      r0, 0x2b4(r27)
    stw      r0, 0x2c8(r27)
.L_8001F2E0:
    mr       r3, r27
    li       r4, 0x18c
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F2FC
    li       r3, 0x18c
    bl       fn_801E62A8
.L_8001F2FC:
    mr       r3, r27
    li       r4, 0x18d
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F318
    li       r3, 0x18d
    bl       fn_801E62A8
.L_8001F318:
    mr       r3, r27
    li       r4, 0x18e
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, 0x18e
    bl       fn_801E62A8
    b        .L_8001F3C4
.L_8001F338:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F354:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F370:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F38C:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F3A8:
    mr       r3, r27
    mr       r4, r28
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F3C4
    li       r3, -0x1
    b        .L_8001F458
.L_8001F3C4:
    clrlwi.  r0, r29, 24
    beq      .L_8001F3EC
    mr       r3, r28
    mr       r4, r29
    mr       r7, r30
    li       r5, 0x7f
    li       r6, 0x0
    bl       HSD_AudioSFXStartParam
    mr       r26, r3
    b        .L_8001F3F0
.L_8001F3EC:
    li       r26, 0x1
.L_8001F3F0:
    mr       r3, r27
    li       r4, 0x0
    b        .L_8001F404
.L_8001F3FC:
    addi     r3, r3, 0x4
    addi     r4, r4, 0x1
.L_8001F404:
    cmplwi   r4, 0x20
    bge      .L_8001F418
    lwz      r0, 0x108(r3)
    cmpwi    r0, 0x0
    bne      .L_8001F3FC
.L_8001F418:
    cmplwi   r4, 0x20
    bge      .L_8001F42C
    slwi     r0, r4, 2
    add      r3, r27, r0
    stw      r28, 0x108(r3)
.L_8001F42C:
    cmpwi    r28, 0x9
    beq      .L_8001F438
    b        .L_8001F454
.L_8001F438:
    li       r3, 0x14
    bl       HSD_Randi
    mulli    r0, r3, 0x6
    mr       r3, r28
    subfic   r0, r0, 0x3c
    extsh    r4, r0
    bl       HSD_AudioSFXSetPitchByID
.L_8001F454:
    mr       r3, r26
.L_8001F458:
    lmw      r26, 0x8(r1)
    lwz      r0, 0x24(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/

/**
 * @note Address: 0x8001F46C
 * @note Size: 0x46C
 */
// fn_8001F46C
/*
    stwu     r1, -0x20(r1)
    mflr     r0
    stw      r0, 0x24(r1)
    stw      r31, 0x1c(r1)
    mr       r31, r4
    stw      r30, 0x18(r1)
    mr       r30, r3
    stw      r29, 0x14(r1)
    lbz      r0, 0xf8(r3)
    cmplwi   r0, 0x0
    beq      .L_8001F8BC
    lwz      r0, 0x3e0(r30)
    cmpwi    r0, 0x0
    ble      .L_8001F538
    bl       HSD_SynthSFXGetPendingLoadCount
    neg      r0, r3
    or       r0, r0, r3
    srwi.    r0, r0, 31
    bne      .L_8001F538
    li       r0, 0x10
    mr       r4, r30
    li       r3, 0x0
    mtctr    r0
.L_8001F4C8:
    lwz      r0, 0x2e0(r4)
    cmplwi   r0, 0x0
    beq      .L_8001F52C
    lbz      r0, 0x2ec(r4)
    cmplwi   r0, 0x0
    bne      .L_8001F52C
    slwi     r0, r3, 4
    lis      r3, lbl_801FEDD0@ha
    add      r29, r30, r0
    lwz      r4, 0x2e0(r29)
    addi     r3, r3, lbl_801FEDD0@l
    crclr    cr1eq
    bl       OSReport
    lwz      r3, 0x2e0(r29)
    li       r5, 0x0
    lwz      r4, 0x2e4(r29)
    li       r6, 0x0
    bl       HSD_SynthSFXLoad
    stw      r3, 0x2e8(r29)
    li       r0, 0x1
    stb      r0, 0x2ec(r29)
    lwz      r3, 0x3e0(r30)
    subi     r0, r3, 0x1
    stw      r0, 0x3e0(r30)
    b        .L_8001F538
.L_8001F52C:
    addi     r4, r4, 0x10
    addi     r3, r3, 0x1
    bdnz     .L_8001F4C8
.L_8001F538:
    li       r0, 0x0
    stw      r0, 0x108(r30)
    stw      r0, 0x10c(r30)
    stw      r0, 0x110(r30)
    stw      r0, 0x114(r30)
    stw      r0, 0x118(r30)
    stw      r0, 0x11c(r30)
    stw      r0, 0x120(r30)
    stw      r0, 0x124(r30)
    stw      r0, 0x128(r30)
    stw      r0, 0x12c(r30)
    stw      r0, 0x130(r30)
    stw      r0, 0x134(r30)
    stw      r0, 0x138(r30)
    stw      r0, 0x13c(r30)
    stw      r0, 0x140(r30)
    stw      r0, 0x144(r30)
    stw      r0, 0x148(r30)
    stw      r0, 0x14c(r30)
    stw      r0, 0x150(r30)
    stw      r0, 0x154(r30)
    stw      r0, 0x158(r30)
    stw      r0, 0x15c(r30)
    stw      r0, 0x160(r30)
    stw      r0, 0x164(r30)
    stw      r0, 0x168(r30)
    stw      r0, 0x16c(r30)
    stw      r0, 0x170(r30)
    stw      r0, 0x174(r30)
    stw      r0, 0x178(r30)
    stw      r0, 0x17c(r30)
    stw      r0, 0x180(r30)
    stw      r0, 0x184(r30)
    lwz      r0, 0x2b4(r30)
    cmpwi    r0, 0x0
    beq      .L_8001F5E8
    lwz      r0, 0x2c8(r30)
    cmpwi    r0, 0x0
    bne      .L_8001F5E8
    bl       fn_801EA2E8
    cmpwi    r3, 0x0
    bne      .L_8001F5E8
    li       r0, 0x0
    stw      r0, 0x2b4(r30)
.L_8001F5E8:
    lwz      r4, 0x188(r30)
    cmpwi    r4, 0x0
    ble      .L_8001F63C
    lha      r3, 0x2b8(r30)
    lha      r0, 0x2ba(r30)
    cmpw     r3, r0
    beq      .L_8001F63C
    subf     r0, r3, r0
    divw     r0, r0, r4
    add      r0, r3, r0
    sth      r0, 0x2b8(r30)
    lwz      r0, 0x188(r30)
    subf     r0, r31, r0
    stw      r0, 0x188(r30)
    lwz      r0, 0x188(r30)
    cmpwi    r0, 0x0
    bgt      .L_8001F634
    lha      r0, 0x2ba(r30)
    sth      r0, 0x2b8(r30)
.L_8001F634:
    lha      r3, 0x2b8(r30)
    bl       fn_801EA1A4
.L_8001F63C:
    lis      r4, 0xffff
    mr       r3, r30
    addi     r4, r4, 0x5
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    beq      .L_8001F6CC
    lis      r3, lbl_8025C400@ha
    li       r29, 0x0
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0x1
    lwz      r3, -0x6944(r3)
    lwz      r0, 0xa48(r3)
    cmpwi    r0, 0x4
    bne      .L_8001F688
    addi     r3, r3, 0xa44
    bl       fn_800D4FE8
    cmpwi    r3, 0x0
    beq      .L_8001F688
    li       r29, 0x1
.L_8001F688:
    clrlwi.  r0, r29, 24
    beq      .L_8001F6B0
    lha      r0, 0x2ba(r30)
    cmpwi    r0, 0xc8
    beq      .L_8001F6CC
    li       r3, 0xc8
    li       r0, 0xf
    sth      r3, 0x2ba(r30)
    stw      r0, 0x188(r30)
    b        .L_8001F6CC
.L_8001F6B0:
    lha      r0, 0x2ba(r30)
    cmpwi    r0, 0x0
    beq      .L_8001F6CC
    li       r3, 0x0
    li       r0, 0xf
    sth      r3, 0x2ba(r30)
    stw      r0, 0x188(r30)
.L_8001F6CC:
    lwz      r0, 0xfc(r30)
    cmpwi    r0, 0x0
    bne      .L_8001F768
    lwz      r0, 0x2c0(r30)
    cmpwi    r0, 0x0
    ble      .L_8001F768
    mr       r3, r30
    li       r4, 0x18c
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001F72C
    mr       r3, r30
    li       r4, 0x18d
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001F72C
    mr       r3, r30
    li       r4, 0x18e
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001F72C
    lwz      r0, 0x2c0(r30)
    subf     r0, r31, r0
    stw      r0, 0x2c0(r30)
.L_8001F72C:
    lwz      r0, 0x2c0(r30)
    cmpwi    r0, 0x0
    bgt      .L_8001F768
    lwz      r3, 0x2c8(r30)
    lwz      r0, 0x2b4(r30)
    cmpw     r3, r0
    bne      .L_8001F750
    bl       fn_801EA134
    b        .L_8001F760
.L_8001F750:
    mr       r3, r30
    lis      r4, 0xffff
    li       r5, 0xff
    bl       fn_8001E904
.L_8001F760:
    li       r0, 0x0
    stw      r0, 0x2c8(r30)
.L_8001F768:
    lwz      r3, 0x2b4(r30)
    addis    r0, r3, 0x1
    cmplwi   r0, 0x10
    bne      .L_8001F7DC
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    addis    r3, r3, 0xe
    lwz      r0, 0x5d28(r3)
    cmpwi    r0, 0x1
    bne      .L_8001F7A8
    lfs      f1, 0x5d2c(r3)
    lfs      f0, lbl_8065C090@sda21(r0)
    fcmpo    cr0, f1, f0
    ble      .L_8001F7A8
    li       r0, 0x0
    b        .L_8001F7AC
.L_8001F7A8:
    li       r0, 0x1
.L_8001F7AC:
    clrlwi.  r0, r0, 24
    bne      .L_8001F7DC
    lwz      r0, 0x2c4(r30)
    cmpwi    r0, 0x0
    bgt      .L_8001F7DC
    li       r3, 0xb4
    li       r0, 0x0
    stw      r3, 0x2c4(r30)
    li       r3, 0x0
    li       r4, 0x258
    stb      r0, 0x2cc(r30)
    bl       fn_801EA028
.L_8001F7DC:
    lwz      r0, 0x2c4(r30)
    cmpwi    r0, 0x0
    ble      .L_8001F80C
    subf     r0, r31, r0
    stw      r0, 0x2c4(r30)
    lwz      r0, 0x2c4(r30)
    cmpwi    r0, 0x0
    bgt      .L_8001F80C
    li       r0, 0x0
    stw      r0, 0x2b4(r30)
    stw      r0, 0x2c8(r30)
    bl       fn_801E9FA4
.L_8001F80C:
    lwz      r0, 0x198(r30)
    cmpwi    r0, 0x0
    ble      .L_8001F820
    subf     r0, r31, r0
    stw      r0, 0x198(r30)
.L_8001F820:
    lbz      r3, 0x1a0(r30)
    lbz      r0, 0x1a1(r30)
    cmplw    r3, r0
    beq      .L_8001F8BC
    lwz      r0, 0x188(r30)
    cmpwi    r0, 0x0
    ble      .L_8001F844
    subf     r0, r31, r0
    stw      r0, 0x188(r30)
.L_8001F844:
    lwz      r4, 0x2ac(r30)
    cmpwi    r4, 0x0
    beq      .L_8001F860
    mr       r3, r30
    bl       fn_8001E6C8
    cmpwi    r3, 0x0
    bne      .L_8001F8BC
.L_8001F860:
    lwz      r0, 0x188(r30)
    cmpwi    r0, 0x0
    bgt      .L_8001F8BC
    lbz      r0, 0x1a0(r30)
    lis      r3, lbl_8025C400@ha
    addi     r3, r3, lbl_8025C400@l
    li       r5, 0xff
    slwi     r0, r0, 2
    add      r4, r30, r0
    addis    r3, r3, 0xe
    lwz      r0, 0x1ac(r4)
    addi     r3, r3, 0x7e24
    stw      r0, 0x2ac(r30)
    lwz      r4, 0x2ac(r30)
    bl       fn_8001E904
    lbz      r3, 0x1a0(r30)
    addi     r3, r3, 0x1
    clrlwi   r0, r3, 24
    stb      r3, 0x1a0(r30)
    cmplwi   r0, 0x40
    blt      .L_8001F8BC
    li       r0, 0x0
    stb      r0, 0x1a0(r30)
.L_8001F8BC:
    lwz      r0, 0x24(r1)
    lwz      r31, 0x1c(r1)
    lwz      r30, 0x18(r1)
    lwz      r29, 0x14(r1)
    mtlr     r0
    addi     r1, r1, 0x20
    blr
*/
