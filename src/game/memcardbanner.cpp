#include "game/ccard.h"
#include "game/ctpl.h"
#include "Version.h"
#include <stdio.h>
#include <string.h>
#include "game/globals.h"
#include "game/cgame.h"

#if defined(VERSION_JP)
enum { BANNER_TEXTURE = 87, ICON_TEXTURE = 88 };
#elif defined(VERSION_PAL)
enum { BANNER_TEXTURE = 89, ICON_TEXTURE = 90 };
#else
enum { BANNER_TEXTURE = 88, ICON_TEXTURE = 90 };
#endif

extern "C" {
#if defined(VERSION_JP)
extern const char lbl_80209EC8[];
extern const char lbl_80209ED4[];
#elif defined(VERSION_PAL)
extern const char lbl_8020FB48[];
#else
extern const char lbl_802091C0[];
#endif
}

static inline void* SaveTextureData(const CTexObj& texture)
{
	return texture.mImage != NULL ? texture.mImage->data : NULL;
}

/**
 * @note Address: 0x800884F0
 * @note Size: 0x358
 */
extern "C" void fn_800884F0(Memcard* card, s32 slot)
{
	SaveBanner* data = &card->mFile.mBanner;
#if defined(VERSION_JP)
	sprintf(data->mTitle, lbl_80209EC8);
	s32 time = lbl_8025C400.mPlayTime;
	sprintf(data->mComment, lbl_80209ED4, slot + 1, time / 216000, time / 3600 % 60, time / 60 % 60);
#elif defined(VERSION_PAL)
	sprintf(data->mTitle, fn_8006DBF0(0x102));
	f32 time    = lbl_8025C400.mPlayTime;
	s32 seconds = (s32)(time / 60.0f) % 60;
	sprintf(data->mComment, lbl_8020FB48, slot + 1, fn_8006DBF0(0x103), (s32)(time / 216000.0f), (s32)(time / 3600.0f) % 60, seconds);
#else
	sprintf(data->mTitle, fn_8006DBF0(0x102));
	s32 time    = lbl_8025C400.mPlayTime;
	s32 seconds = time / 60 % 60;
	sprintf(data->mComment, lbl_802091C0, slot + 1, fn_8006DBF0(0x103), time / 216000, time / 3600 % 60, seconds);
#endif
	CTpl* resource   = (CTpl*)lbl_8025C400.mTpl;
	CTexObj* texture = &resource->mTex;
	CTexObj banner   = texture->Init(((TPLPalette*)resource->mBuffer)->descriptorArray + BANNER_TEXTURE, 0);
	memcpy(data->mBanner, SaveTextureData(banner), sizeof(data->mBanner));
	CTexObj icon = texture->Init(((TPLPalette*)resource->mBuffer)->descriptorArray + ICON_TEXTURE, 0);
	memcpy(data->mIcon, SaveTextureData(icon), sizeof(data->mIcon));

	CARDSetCommentAddress(&card->mStat, 0);
	CARDSetBannerFormat(&card->mStat, CARD_STAT_BANNER_RGB5A3);
	CARDSetIconAddress(&card->mStat, 0x40);
	CARDSetIconAnim(&card->mStat, CARD_STAT_ANIM_LOOP);
	CARDSetIconFormat(&card->mStat, 0, CARD_STAT_ICON_RGB5A3);
	CARDSetIconSpeed(&card->mStat, 0, CARD_STAT_SPEED_SLOW);
	CARDSetIconSpeed(&card->mStat, 1, CARD_STAT_SPEED_END);
}
