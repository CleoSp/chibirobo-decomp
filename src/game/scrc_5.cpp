#include "game/scrc_shared.h"

extern "C" {

void fn_8010904C(XObjActor3* self);
void fn_801095E8(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010FB1C(XObjActor3* self);
void fn_8010B394(XObjActor3* self);
void fn_8010B528(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_80024D84(CPtcl* ptcl, u32 frames);
void fn_8010FB1C(XObjActor3* self);

// plays anim n on the first kind 0x10 actor and sizes it for it
void fn_8010C0F0(s32 n)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {
			obj->SetAnim(n, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
			switch (n) {
			case 0:
				obj->SetScale(0.1f, 0.1f, 0.1f, 0);
				obj->_B80 = 0;
				break;
			case 1:
				obj->SetScale(1.4f, 1.4f, 1.4f, 0);
				obj->_B80 = 1;
				break;
			case 2:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 2;
				break;
			case 3:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 1;
				break;
			case 4:
				obj->SetScale(1.0f, 1.0f, 1.0f, 0);
				obj->_B80 = 4;
				break;
			}
			return;
		}
	}
}

// puts the first kind 0x10 actor back to its rest pose, small
void fn_8010BF80(void)
{
	u8* data;
	s32 size;
	size = GAME_ACTORS->mSize;
	data = GAME_ACTORS->mData;
	for (s32 i = 0; i < 48; i++) {
		XObjActor3* obj = (XObjActor3*)(data + i * size);
		if (GetInfoKind(obj->mInfo) == 0x10 && (obj->mFlags & 1)) {

			obj->_B80 = 0;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags |= 4;
			obj->mStatus.mFlags |= 0x10;
			obj->SetAnim(0, TRUE, NULL, NULL, NULL, 0.0f, 1.0f, 0.0f, 10.0f);
			obj->mStatus.mFlags &= ~4;
			obj->mStatus.mFlags &= ~0x10;
			obj->SetScale(0.1f, 0.1f, 0.1f, 0);
			obj->_B80 = 0;
			return;
		}
	}
}

void XObjActor3::Kind17(u32 frames)
{
}
void fn_8010B528(XObjActor3* self);
void fn_8010B7B4(XObjActor3* self, u32 frames);
void fn_8010B394(XObjActor3* self);
void fn_8010AD88(XObjActor3* self);
void fn_8010AF74(XObjActor3* self);
void fn_8010904C(XObjActor3* self);
}
