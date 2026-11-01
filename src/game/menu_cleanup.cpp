#include "game/cobj.h"
#include "game/cjobjset.h"
#include "game/ctpl.h"
#include "game/ut.h"

// The texture holder adds an ownership layer without changing CTexObj's layout.
struct MenuTexture {
	CTexObj mTexture;
};
class MenuCursor;
struct MenuWindow {
	BOOL mVisible;
	f32 mX, mY, mWidth, mHeight;
	MenuTexture mTex;
	f32 mTransX, mTransY, mScaleX, mScaleY, mAlpha;
	MenuCursor* mCursor;
};
class CMenu {
public:
	s32 mState;
	f32 mAlpha;
	s32 mSelect;
	MenuWindow mWin;
	BOOL mFading;
	s32 mFadeDir;
	f32 mFade, mFadeAlpha;
	virtual void Init();
	virtual BOOL Load();
	virtual void Reset();
	virtual BOOL Update(u32 frames);
	virtual BOOL Draw();
	virtual void Open();
	virtual void Close();
	virtual BOOL IsOpen();
	virtual void StartFade(s32 dir);
	virtual void StopFade();
};
class Menu : public CMenu {
public:
	f32 mLineHeight, mMaxWidth, mLines;
};
// Only the base subobjects require destruction for these menu variants.
class MenuMCTestSub : public Menu {
public:
	~MenuMCTestSub();
};
class MenuMCTest : public Menu {
public:
	~MenuMCTest();
};
class MenuStageSelect : public Menu {
public:
	~MenuStageSelect();
};
class MenuSave : public Menu {
public:
	~MenuSave();
};
class MenuPC : public Menu {
public:
	~MenuPC();
};
class MenuItem : public Menu {
public:
	~MenuItem();
};
class MenuStatus : public Menu {
public:
	~MenuStatus();
};
class MenuMain : public Menu {
public:
	~MenuMain();
};

// Each entry carries one value and its independent CStatus flags.
struct MenuStatusEntry {
	s32 mValue;
	CStatus mStatus;
	MenuStatusEntry();
	~MenuStatusEntry();
};
struct MenuStatusTable {
	s32 mCount;
	MenuStatusEntry mEntries[256];
	MenuStatusEntry mLeft[4];
	MenuStatusEntry mRight[4];
	~MenuStatusTable();
};

typedef char MenuTexture_size_check[sizeof(MenuTexture) == 0x100 ? 1 : -1];
typedef char MenuWindow_size_check[sizeof(MenuWindow) == 0x12C ? 1 : -1];
typedef char CMenu_size_check[sizeof(CMenu) == 0x14C ? 1 : -1];
typedef char MenuStatusEntry_size_check[sizeof(MenuStatusEntry) == 0xC ? 1 : -1];
typedef char MenuStatusTable_size_check[sizeof(MenuStatusTable) == 0xC64 ? 1 : -1];

MenuStatusTable::~MenuStatusTable()
{
}
MenuStatusEntry::~MenuStatusEntry()
{
}
MenuStatusEntry::MenuStatusEntry()
{
}
MenuMCTestSub::~MenuMCTestSub()
{
}
MenuMCTest::~MenuMCTest()
{
}
MenuStageSelect::~MenuStageSelect()
{
}
MenuSave::~MenuSave()
{
}
MenuPC::~MenuPC()
{
}
MenuItem::~MenuItem()
{
}
MenuStatus::~MenuStatus()
{
}
MenuMain::~MenuMain()
{
}
