#include "game/xobjactor.h"
#include "game/cgame.h"
#include "game/xchr.h"
#include "game/gmath.h"

static inline bool PlayerRestricted(XObj* player)
{
	Vec position = player->mMoveTranslate;
	if (position.z < -4.28f)
		return true;
	switch (player->mAnimId) {
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	case 0x6A:
	case 0x6B:
	case 0x6C:
	case 0x6D:
	case 0x6E:
	case 0x6F:
	case 0x70:
	case 0x71:
	case 0x72:
	case 0x73:
		return true;
	}
	return false;
}

static inline bool WidePlayerWindow(XObj* player)
{
	switch (player->mAnimId) {
	case 4:
	case 13:
	case 53:
	case 121:
	case 137:
	case 269:
	case 2073:
	case 2200:
		return true;
	}
	return false;
}

/**
 * @note Address: 0x8011132C
 * @note Size: 0x2D0
 */
extern "C" BOOL fn_8011132C(XObjActor3* object)
{
	XObj* player = lbl_8025C400.mPlayer;
	Vec position = player->mMoveTranslate;
	if (!(object->_B8C.mFlags & 4)) {
		if (PlayerRestricted(player))
			return FALSE;
		if (position.y >= object->mMoveTranslate.y - 16.0f && position.y <= 16.0f + object->mMoveTranslate.y) {
			if (HALF_PI == object->_B7C) {
				if (object->mMoveTranslate.x < position.x && position.x < object->_BC4)
					return TRUE;
				if (object->mMoveTranslate.x > position.x && position.x > object->_BC0) {
					if (WidePlayerWindow(player)) {
						if (object->mMoveTranslate.x > position.x && position.x > object->mMoveTranslate.x - 32.0f)
							return TRUE;
					} else {
						if (object->mMoveTranslate.x > position.x && position.x > object->mMoveTranslate.x - 12.0f)
							return TRUE;
					}
				}
			} else {
				if (object->mMoveTranslate.x > position.x && position.x > object->_BC0)
					return TRUE;
				if (object->mMoveTranslate.x < position.x && position.x < object->_BC4) {
					if (WidePlayerWindow(player)) {
						if (object->mMoveTranslate.x < position.x && position.x < 32.0f + object->mMoveTranslate.x)
							return TRUE;
					} else {
						if (object->mMoveTranslate.x < position.x && position.x < 12.0f + object->mMoveTranslate.x)
							return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}
