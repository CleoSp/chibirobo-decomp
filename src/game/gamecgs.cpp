#include "game/hsd_jobj.h"
#include "game/cgs.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__3CGs[];
CGsTevStage* fn_80008344(CGsTevStage* stage, s32 flags);
CGsChannel* fn_8000838C(CGsChannel* channel, s32 flags);
}

/**
 * @note Address: 0x80008050
 * @note Size: 0x2F4
 */
// todo: weak copy
extern "C" CGs* fn_80008050(CGs* gs, s32 flags)
{
	if (gs != NULL) {
		*(void***)((u8*)gs + sizeof(CGs) - 4) = __vt__3CGs;
		gs->mTorus.CTorus::~CTorus();
		gs->mCylinder.CCylinder::~CCylinder();
		gs->mSphere.CSphere::~CSphere();
		gs->mIcosahedron.CIcosahedron::~CIcosahedron();
		gs->mDodeca.CDodeca::~CDodeca();
		gs->mOctahedron.COctahedron::~COctahedron();
		gs->mCube.CCube::~CCube();
		gs->mSprite2.CSprite::~CSprite();
		gs->mTile.CTile::~CTile();
		gs->mCircle.CCircle::~CCircle();
		gs->mTriangle.CTriangle::~CTriangle();
		gs->mLine.CLine::~CLine();
		gs->mSprite.CSprite::~CSprite();
		gs->mTex.CTexObj::~CTexObj();
		gs->mFont.CFont::~CFont();
		__destroy_arr(gs->mChannels, (ConstructorDestructor*)fn_8000838C, sizeof(CGsChannel), 4);
		__destroy_arr(gs->mTevStages, (ConstructorDestructor*)fn_80008344, sizeof(CGsTevStage), 2);
		gs->mRenderMode.CGsRenderMode::~CGsRenderMode();
		gs->mScreen.CGsScreen::~CGsScreen();
		if ((s16)flags > 0)
			::operator delete(gs);
	}
	return gs;
}
