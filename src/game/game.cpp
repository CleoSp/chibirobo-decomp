#include "game/hsd_jobj.h"
#include "game/cgame.h"
#include "game/ctpl.h"
#include "game/fbtexeff.h"
#include "game/cmotioncontrolcamera.h"
#include "game/title.h"
#include "game/evt.h"
#include "game/cparticle.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

void operator delete(void* object);

extern "C" {
extern void* __vt__5CGame[];
extern void* __vt__6CMovie[];
extern void* __vt__5CCard[];
extern void* __vt__7Memcard[];
extern void* __vt__6CSound[];
extern void* __vt__6XSound[];
extern void* __vt__4CEvt[];
extern void* __vt__5XOBJS[];
extern void* __vt__10CScrnQuake[];
extern void* __vt__9CParticle[];
extern void* __vt__8InfoScrn[];
extern void* __vt__4Logo[];
extern void* __vt__10CScrnColor[];
extern void* __vt__9CScrnWipe[];
extern void* __vt__9CScrnFade[];
extern void* __vt__7CFbBlur[];
extern void* __vt__10CFbOutline[];
extern void* __vt__13CScrnRoboeyes[];
extern void* __vt__13CScrnWipeTile[];
extern void* __vt__15CScrnWipeCustom[];
extern void* __vt__12CScrnWipeTex[];
extern void* __vt__15CScrnWipeCircle[];
CCameraActions* fn_800091CC(CCameraActions* actions, s32 flags);
CMotionControlCamera* fn_8000902C(CMotionControlCamera* camera, s32 flags);
void* __dt__4CMsgFv(void* msg, s32 flags);
CObjArchive* fn_80008E10(CObjArchive* archive, s32 flags);
CVec* dtor_80008DC8(CVec* vec, s32 flags);
CGenerator* fn_80008D80(CGenerator* generator, s32 flags);
CFile* __dt__5CFileFv(CFile* file, s32 flags);
void* fn_80008C74(void* menu, s32 flags);
Title* fn_80008650(Title* title, s32 flags);
CBase* __dt__5CBaseFv(CBase* base, s32 flags);
}

// todo: cleanup, stand-ins for member dtors retail inlines,
// their classes are still untyped or out of line in our headers

struct CMovieDtor {
	u8 _00[0xB4];   // _00
	void** mVtable; // _B4

	~CMovieDtor() { mVtable = __vt__6CMovie; }
};

// the CMovie, the Memcard and the screen colors each sit in a struct of their own
struct GameMovie {
	CMovieDtor mMovie; // _00
};

struct CCardDtor {
	u8 _00[0xA020]; // _00
	void** mVtable; // _A020

	~CCardDtor() { mVtable = __vt__5CCard; }
};

struct MemcardDtor {
	u8 _00[0x20];               // _00
	CCardDtor mCard;            // _20
	u8 _A044[0x16A20 - 0xA044]; // _A044
	void** mVtable;             // _16A20

	~MemcardDtor() { mVtable = __vt__7Memcard; }
};

struct GameMemcard {
	MemcardDtor mMemcard; // _00
};

struct CSoundDtor {
	void** mVtable; // _00
	u32 _04;        // _04

	~CSoundDtor() { mVtable = __vt__6CSound; }
};

struct XSoundDtor : CSoundDtor {
	CFile mFile; // _08

	~XSoundDtor() { mVtable = __vt__6XSound; }
};

struct CEvtDtor {
	u32 _00;             // _00
	CEvtData mData;      // _04
	u8 _10[0x24 - 0x10]; // _10
	CStack mStack0;      // _24
	CStack mStack1;      // _34
	CStack mStack2;      // _44
	u8 _54[0x64 - 0x54]; // _54
	void** mVtable;      // _64

	~CEvtDtor() { mVtable = __vt__4CEvt; }
};

struct XOBJSDtor {
	u8 _00[0x11D8]; // _00
	void** mVtable; // _11D8

	~XOBJSDtor() { mVtable = __vt__5XOBJS; }
};

struct CScrnQuakeDtor {
	u8 mVecs[3][0x10]; // _00, CVecs
	u32 _30;           // _30
	void** mVtable;    // _34

	~CScrnQuakeDtor()
	{
		mVtable = __vt__10CScrnQuake;
		__destroy_arr(mVecs, (ConstructorDestructor*)dtor_80008DC8, sizeof(CVec), 3);
	}
};

struct CParticleDtor {
	u32 mReadStep;              // _00
	u8 mFile[4][sizeof(CFile)]; // _04
	u8 _3B4[0x3C4 - 0x3B4];     // _3B4
	u8 mGenerators[32][0xC];    // _3C4, CGenerators
	void** mVtable;             // _544

	~CParticleDtor()
	{
		mVtable = __vt__9CParticle;
		__destroy_arr(mGenerators, (ConstructorDestructor*)fn_80008D80, 0xC, 32);
		__destroy_arr(mFile, (ConstructorDestructor*)__dt__5CFileFv, sizeof(CFile), 4);
	}
};

struct CScrnColorDtor {
	u8 _00[0x30];   // _00
	void** mVtable; // _30

	~CScrnColorDtor() { mVtable = __vt__10CScrnColor; }
};

struct CScrnWipeDtor : CScrnColorDtor {
	~CScrnWipeDtor() { mVtable = __vt__9CScrnWipe; }
};

struct CScrnFadeDtor : CScrnColorDtor {
	~CScrnFadeDtor() { mVtable = __vt__9CScrnFade; }
};

struct InfoScrnDtor {
	CScrnFadeDtor mFade;  // _00
	CScrnWipeDtor mWipe;  // _34
	u8 _68[0x1D8 - 0x68]; // _68
	void** mVtable;       // _1D8

	~InfoScrnDtor() { mVtable = __vt__8InfoScrn; }
};

struct LogoDtor {
	s32 mState;             // _00
	CTpl mTpl;              // _04
	u8 _1F4[0x200 - 0x1F4]; // _1F4
	void** mVtable;         // _200

	~LogoDtor() { mVtable = __vt__4Logo; }
};

struct GameColor {
	CScrnColorDtor mColor; // _00
};

// the color and texture between FbSubwin and the screen colors
struct GameColorTex {
	u32 _00;               // _00
	CScrnColorDtor mColor; // _04
	CTexObj mTex;          // _38
};

struct CFbBlurDtor {
	u8 _00[0x40];   // _00
	void** mVtable; // _40

	~CFbBlurDtor() { mVtable = __vt__7CFbBlur; }
};

struct CFbOutlineDtor {
	u8 _00[0x60];   // _00
	void** mVtable; // _60

	~CFbOutlineDtor() { mVtable = __vt__10CFbOutline; }
};

struct CScrnRoboeyesDtor : CScrnWipeDtor {
	~CScrnRoboeyesDtor() { mVtable = __vt__13CScrnRoboeyes; }
};

struct CScrnWipeTileDtor : CScrnWipeDtor {
	~CScrnWipeTileDtor() { mVtable = __vt__13CScrnWipeTile; }
};

struct CScrnWipeCustomDtor : CScrnWipeDtor {
	~CScrnWipeCustomDtor() { mVtable = __vt__15CScrnWipeCustom; }
};

struct CScrnWipeTexDtor : CScrnWipeDtor {
	u8 _34[0x64 - 0x34]; // _34
	CTexObj mTex;        // _64

	~CScrnWipeTexDtor() { mVtable = __vt__12CScrnWipeTex; }
};

struct CScrnWipeCircleDtor : CScrnWipeDtor {
	~CScrnWipeCircleDtor() { mVtable = __vt__15CScrnWipeCircle; }
};
STATIC_ASSERT(sizeof(CParticleDtor) == 0x548);

/**
 * @note Address: 0x80007880
 * @note Size: 0x6BC
 */
// todo: weak copy
extern "C" CGame* __dt__5CGameFv(CGame* game, s32 flags)
{
	if (game != NULL) {
		*(void***)(&game->mReset + 1) = __vt__5CGame;
		((GameMovie*)game->mMovie)->~GameMovie();
		((GameMemcard*)&game->mMemcard)->~GameMemcard();
		((XSoundDtor*)&game->mSound)->~XSoundDtor();
		fn_800091CC((CCameraActions*)game->mCameraActions, -1);
		fn_8000902C((CMotionControlCamera*)game->mCamera, -1);
		__destroy_arr(game->mEvtData, (ConstructorDestructor*)__dt__4CMsgFv, sizeof(game->mEvtData[0]), 2);
		((CEvtDtor*)&game->_5D07C)->~CEvtDtor();
		if (game->mObjArchives != NULL) {
			__destroy_arr(game->mObjArchives, (ConstructorDestructor*)fn_80008E10, sizeof(CObjArchive), 0x400);
		}
		((XOBJSDtor*)game->_9694)->~XOBJSDtor();
		((CScrnQuakeDtor*)game->mQuake)->~CScrnQuakeDtor();
		((CParticleDtor*)game->mParticle)->~CParticleDtor();
		((InfoScrnDtor*)game->mInfoScrn)->~InfoScrnDtor();
		fn_80008C74(&game->_8580, -1);
		fn_80008650((Title*)&game->_4930, -1);
		((LogoDtor*)&game->_472C)->~LogoDtor();
		((GameColor*)game->_46F0)->~GameColor();
		((GameColor*)game->mColor)->~GameColor();
		((GameColorTex*)(game->mColor - sizeof(GameColorTex)))->~GameColorTex();
		((FbSubwin*)game->mSubwin)->FbSubwin::~FbSubwin();
		((CFbBlurDtor*)game->mBlur)->~CFbBlurDtor();
		((CFbOutlineDtor*)game->mOutline)->~CFbOutlineDtor();
		((CScrnRoboeyesDtor*)game->mRoboeyes)->~CScrnRoboeyesDtor();
		((CScrnWipeTileDtor*)game->mWipeTile2)->~CScrnWipeTileDtor();
		((CScrnWipeTileDtor*)game->mWipeTile)->~CScrnWipeTileDtor();
		((CScrnWipeCustomDtor*)game->mWipeCustom)->~CScrnWipeCustomDtor();
		((CScrnWipeTexDtor*)game->mWipeTex)->~CScrnWipeTexDtor();
		((CScrnWipeCircleDtor*)game->mWipeCircle)->~CScrnWipeCircleDtor();
		((CScrnFadeDtor*)game->mFade)->~CScrnFadeDtor();
		((CTpl*)game->mTpl)->CTpl::~CTpl();
		((CFileArchive*)game->mArchive)->CFileArchive::~CFileArchive();
		__dt__5CBaseFv(game, 0);
		if ((s16)flags > 0)
			::operator delete(game);
	}
	return game;
}
