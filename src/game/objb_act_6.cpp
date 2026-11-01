#include "game/objb_act_shared.h"

extern "C" {

static inline void PutDown(XActLift* act)
{
	XObjFlags* target;
	XObjChr* chr = act->mChr;
	if (chr->mAnimId == 0x8B1 || chr->mAnimId == 0x8B3) {
		chr->SetAnim(0x8B2, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	} else if (chr->mAnimId == 0xB) {
		chr->SetAnim(8, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
	}
	act->mState      = 0;
	act->mChr->_95D0 = 0;
	act->mTarget->mFlags &= ~0x0200000000000000ULL;
	act->mTarget->SetAnim(7, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
	act->_24 = 0x14;
	target   = (XObjFlags*)act->mTarget;
	fn_800DC9B0(&act->mChr->mStats.mTable, target->GetKind(), 1, TRUE);
	target->SetBit(target->GetFlagNo());
	fn_8001E904(GAME_SOUND, 0x39, 0xFF);
}

// the ladder update, defined early so that it calls the joint translate instead of inlining it
/**
 * @note Address: 0x800C7BC4
 * @note Size: 0x1FB0
 */
// NONMATCHING: for the ladder's move the dol reads the target again after copying its last position
void fn_800C7BC4(XActLadder* act, s32 anim, HSD_AObj* aobj, u32 frames, f32 frame, f32 end)
{
	switch (act->mState) {
	case 1:
		if (anim != 0x66) {
			act->mChr->SetAnim(0x66, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mChr->SetRotationY(WrapAngle(PI + act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, 4.0f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mChr->SetAnim(0x67, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				} else {
					act->mChr->SetTranslateX(act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0 && frame >= 25.0f) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		}
		break;
	case 2:
		if (anim != 0x6D) {
			act->mChr->SetAnim(0x6D, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			if (act->mTarget != NULL) {
				act->mChr->SetRotationY(WrapAngle(act->mTarget->mMoveRotate.y), 0);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, -1.5f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			act->mChr->SetRotationY(PI + act->mChr->mMoveRotate.y, 0);
			s32 path3[] = { 0 };
			fn_800BB3D0(act->mChr, 1, path3, 0.0f);
			act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
		} else {
			if (act->mTarget != NULL) {
				Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
				act->mPos.x += move.x;
				act->mPos.z += move.z;
				if (act->mDist > 0.0f && frame > 20.0f) {
					if ((act->mDist -= 0.2f * frames) < 0.0f) {
						act->mDist = 0.0f;
					}
					act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
					act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
				}
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0 && frame >= 50.0f) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		}
		break;
	case 3:
		if (anim != 0x87) {
			if (act->mTarget != NULL) {
				if (act->mTarget->mPoints[act->mPoint].mKind == 0) {
					CVec chrPos(act->mChr->mMoveTranslate);
					chrPos.y = 0.0f;
					CVec pt(fn_800D5190(act));
					pt.y = 0.0f;
					act->mChr->SetRotationY(PI + (f32)atan2(pt.x - chrPos.x, pt.z - chrPos.z), 0);
				} else {
					act->mChr->SetRotationY(act->mTarget->mMoveRotate.y, 0);
				}
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, fn_800A2CE4(act->mChr), 1.0f, 1.0f, 10.0f);
				act->mChr->UpdateAnim(0);
				s32 path[] = { 0 };
				CVec d     = fn_8007E7C8(act->mChr, 1, path) - act->mChr->mMoveTranslate;
				act->mChr->SetTranslateX(act->mChr->mMoveTranslate.x - d.x, 0);
				act->mChr->SetTranslateZ(act->mChr->mMoveTranslate.z - d.z, 0);
				act->mChr->SetTranslateY(act->mTarget->_77C, 0);
				act->mChr->SetAnim(0x87, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				CVec chrPos(act->mChr->mMoveTranslate);
				chrPos.y = 0.0f;
				CVec ladPos(act->mTarget->mMoveTranslate);
				ladPos.y = 0.0f;
				CVec off(0.0f, 0.0f, -1.0f);
				Mtx m;
				PSMTXRotRad(m, 'y', act->mTarget->mMoveRotate.y);
				PSMTXMultVec(m, &off, &off);
				AddVec(ladPos, off);
				act->mPos.x = ladPos.x;
				act->mPos.y = ladPos.y;
				act->mPos.z = ladPos.z;
				act->mDist  = PSVECDistance(&ladPos, &chrPos);
				act->mAngle = atan2(chrPos.x - ladPos.x, chrPos.z - ladPos.z);
			}
			if (CheckStageFlag(lbl_8025C400.mStage, 1)) {
				XObjChrPart* part = &act->mChr->mPart;
				if (part->mOn != 0) {
					fn_800B2BD0(part, TRUE);
					fn_8001E904(GAME_SOUND, 0x37, 0xFF);
				}
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[]      = { 0 };
			HSD_JObj* joint = fn_800B2764(act->mChr, 1, path);
			f32 turn        = HSD_JObjGetRotationY(joint);
			f32 facing      = HSD_JObjGetRotationY(act->mChr->mJObj);
			act->mChr->SetRotationY(facing + turn, 0);
			fn_800C9B74(joint, 0.0f);
			s32 path2[] = { 0 };
			CVec d      = fn_8007E7C8(act->mChr, 1, path2) - act->mChr->mMoveTranslate;
			f32 angle   = HSD_JObjGetRotationY(act->mChr->mJObj);
			d.x         = -d.x;
			d.z         = -d.z;
			act->mChr->SetPos((act->mChr->mMoveTranslate + d), 0);
			fn_800C9C34(act->mChr);
			s32 path3[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path3, 0.0f, 0.0f, 0.0f);
			act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
			act->mState = 5;
			act->_0C    = 0;
		} else if (act->mTarget != NULL) {
			Vec move = act->mTarget->mMoveTranslate - act->mTarget->mLastPos;
			act->mPos.x += move.x;
			act->mPos.z += move.z;
			if (act->mDist > 0.0f) {
				if ((act->mDist -= 0.4f * frames) < 0.0f) {
					act->mDist = 0.0f;
				}
				act->mChr->SetTranslateX(act->mDist * (f32)sin(act->mAngle) + act->mPos.x, 0);
				act->mChr->SetTranslateZ(act->mDist * (f32)cos(act->mAngle) + act->mPos.z, 0);
			} else {
				act->mChr->SetTranslateX(act->mPos.x, 0);
				act->mChr->SetTranslateZ(act->mPos.z, 0);
			}
			act->mChr->SetTranslateY(act->mTarget->_77C, 0);
		}
		break;
	case 4:
		act->mChr->SetAnim(0x68, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
		act->mState = 5;
		break;
	case 5:
		if (anim == 0x69 || anim == 0x6A || anim == 0x6B || anim == 0x6C) {
			if (aobj->flags & 0x44000000) {
				s32 path[] = { 0 };
				CVec hand  = fn_800BAD80(act->mChr, 1, path);
				if (hand.y < 0.0f) {
					hand.y = (s32)(hand.y - 0.5f);
				}
				s32 path2[] = { 0 };
				fn_800C7810(act->mChr, 1, path2, hand);
				s32 path3[] = { 0 };
				act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path3), 0);
				fn_800C9C34(act->mChr);
				s32 path4[] = { 0 };
				fn_8009F4F8(act->mChr, 1, path4, 0.0f, 0.0f, 0.0f);
				if (act->_0C == 1) {
					if (act->mTarget != NULL) {
						s32 path5[] = { 0 };
						if (14.0f + fn_8007E7C8(act->mChr, 1, path5).y >= act->mTarget->mPointDist[0]) {
							act->mState = 6;
							goto top;
						}
					}
					if (anim == 0x69 || anim == 0x6B) {
						act->mChr->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mChr->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (act->_0C == -1) {
					if (act->mChr->GetHeight() <= 2.0f) {
						act->mState = 7;
					} else if (anim == 0x69 || anim == 0x6B) {
						act->mChr->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					} else if (anim == 0x6A || anim == 0x6C) {
						act->mChr->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
					}
				} else if (anim == 0x69 || anim == 0x6B) {
					act->mChr->SetAnim(0x68, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x6A || anim == 0x6C) {
					act->mChr->SetAnim(0x67, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			top:
				if (act->_2C && act->mState == 5) {
					act->mState = 9;
				}
			}
		} else {
			if (act->_0C == 1) {
				if (act->mTarget != NULL) {
					s32 path[] = { 0 };
					if (14.0f + fn_8007E7C8(act->mChr, 1, path).y >= act->mTarget->mPointDist[0]) {
						act->mState = 6;
						goto top2;
					}
				}
				if (anim == 0x67) {
					act->mChr->SetAnim(0x69, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mChr->SetAnim(0x6A, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			} else if (act->_0C == -1) {
				if (act->mChr->GetHeight() <= 2.0f) {
					act->mState = 7;
				} else if (anim == 0x67) {
					act->mChr->SetAnim(0x6B, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				} else if (anim == 0x68) {
					act->mChr->SetAnim(0x6C, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				}
			}
		top2:
			if (act->_2C && act->mState == 5) {
				act->mState = 9;
			}
		}
		break;
	case 6:
		if (!(anim == 0x6E || anim == 0x6F)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x6E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x6F, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 7:
		if (!(anim == 0x70 || anim == 0x71)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x70, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x71, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C    = 3;
			act->mState  = 0;
			act->mTarget = NULL;
		}
		break;
	case 8:
		if (anim == 0x69 || anim == 0x6B) {
			act->mChr->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x6A || anim == 0x6C) {
			act->mChr->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x67) {
			act->mChr->SetAnim(0x72, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		} else if (anim == 0x68) {
			act->mChr->SetAnim(0x73, TRUE, NULL, NULL, NULL, 10.0f, 1.0f, 1.0f, 10.0f);
		}
		act->mState = 9;
	case 9:
		if (!(anim == 0x72 || anim == 0x73)) {
			if (anim == 0x69 || anim == 0x6B) {
				act->mChr->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x6A || anim == 0x6C) {
				act->mChr->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x67) {
				act->mChr->SetAnim(0x72, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			} else if (anim == 0x68) {
				act->mChr->SetAnim(0x73, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			}
		} else if (aobj->flags & 0x44000000) {
			s32 path[] = { 0 };
			act->mChr->SetPos(fn_8007E7C8(act->mChr, 1, path), 0);
			fn_800C9C34(act->mChr);
			s32 path2[] = { 0 };
			fn_8009F4F8(act->mChr, 1, path2, 0.0f, 0.0f, 0.0f);
			if (act->mChr->GetMode() == 1) {
				act->mState                  = 0;
				act->mChr->mActCopter.mState = 4;
			} else {
				act->mChr->SetAnim(0x74, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 10;
			}
			act->mTarget = NULL;
		}
		break;
	case 10:
		if (act->mChr->GetHeight() <= 0.0f) {
			if (act->mChr->_9A90 > 30.0f) {
				act->mState = 0;
				act->mChr->SetAnim(0x8E, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mChr->mActJump.mState = 5;
				fn_800C9D54((XActJump*)&act->mChr->mActJump, act->mChr->_9A90);
				act->mChr->SetColor(1, 0xFF, 0, 0, 0.0f);
				act->mChr->SetColor(1, 0, 0, 0, 0.3f);
				fn_80023EE0(lbl_8025C400._4588, 0xFF, 0, 0, 0, 100, 100, 200, 0, 0.3f);
			} else {
				act->mChr->SetAnim(0x75, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 1.0f, 10.0f);
				act->mState = 11;
			}
		}
		break;
	case 11:
		if (aobj->flags & 0x44000000) {
			XObjChr* chr = act->mChr;
			chr->SetAnim(0x11, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			chr->_A2C   = 3;
			act->mState = 0;
		}
		break;
	}
}
void fn_800C7810(CJObj* obj, s32 depth, s32* path, const Vec& pos);

/**
 * @note Address: 0x800C9C34
 * @note Size: 0x3C
 */
void fn_800C9C34(XObj* obj)
{
	if (obj->mMoveTranslate.y != obj->mLastPos.y) {
		obj->mFlags |= 0x0010000000000000ULL;
	}
	obj->mLastPos.x = obj->mMoveTranslate.x;
	obj->mLastPos.y = obj->mMoveTranslate.y;
	obj->mLastPos.z = obj->mMoveTranslate.z;
}

/**
 * @note Address: 0x800C9C70
 * @note Size: 0xE4
 */
void fn_800C9C70(XActC* act, CPadOne* pad, f32* out)
{
	act->_2C = 0;
	if (act->mState == 5) {
		if (CanGameAct() && lbl_8025C400._5D080 == NULL && (pad->mTrigger & 0x200)) {
			act->_2C = 1;
		}
	} else if (act->mState != 0) {
		*out = 0.0f;
	}
}

/**
 * @note Address: 0x800C9D54
 * @note Size: 0x360
 */
void fn_800C9D54(XActJump* act, f32 fall)
{
	if (lbl_8025C400._5D080 != NULL) {
		act->mFall = 0.0f;
		act->_48   = 0.0f;
	} else {
		act->mFall = fall;
		act->_48   = fall;
		if (act->mFall >= 50.0f) {
			XObjChr* chr = act->mChr;
			if (chr->mActLift.mState != 0 && IsCarrying(&chr->mActLift, 0x16)) {
				PutDown((XActLift*)&chr->mActLift);
				if (act->mFall >= 60.0f) {
					act->mState                  = 0;
					act->mChr->mActTumble.mState = 1;
					act->mChr->SetAnim(0x35, FALSE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
				}
			}
		}
	}
}

/**
 * @note Address: 0x800C9B74
 * @note Size: 0xC0
 */
void fn_800C9B74(HSD_JObj* jobj, f32 y)
{
	HSD_JObjSetRotationY(jobj, y);
}

CVec fn_800A0978(XObjChr* chr);
}
