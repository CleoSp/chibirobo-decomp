#include "game/stg.h"

// the destructors of the menus and of the status menu's value table

// a value with flags of its own, the status menu keeps tables of them
struct MenuStatusEntry {
	s32 mValue;      // _00
	CStatus mStatus; // _04

	MenuStatusEntry();
	~MenuStatusEntry();
};
STATIC_ASSERT(sizeof(MenuStatusEntry) == 0xC);

struct MenuStatusTable {
	s32 mCount;                    // _000
	MenuStatusEntry mEntries[256]; // _004
	MenuStatusEntry mLeft[4];      // _C04
	MenuStatusEntry mRight[4];     // _C34

	~MenuStatusTable();
};
STATIC_ASSERT(sizeof(MenuStatusTable) == 0xC64);

/**
 * @note Address: 0x8006331C
 * @note Size: 0x94
 */
MenuStatusTable::~MenuStatusTable()
{
}

/**
 * @note Address: 0x800633B0
 * @note Size: 0x54
 */
MenuStatusEntry::~MenuStatusEntry()
{
}

/**
 * @note Address: 0x80063404
 * @note Size: 0x18
 */
MenuStatusEntry::MenuStatusEntry()
{
}

/**
 * @note Address: 0x8006341C
 * @note Size: 0x90
 */
MenuMCTestSub::~MenuMCTestSub()
{
}

/**
 * @note Address: 0x800634AC
 * @note Size: 0x90
 */
MenuMCTest::~MenuMCTest()
{
}

/**
 * @note Address: 0x8006353C
 * @note Size: 0x90
 */
MenuStageSelect::~MenuStageSelect()
{
}

/**
 * @note Address: 0x800635CC
 * @note Size: 0x90
 */
MenuSave::~MenuSave()
{
}

/**
 * @note Address: 0x8006365C
 * @note Size: 0x90
 */
MenuPC::~MenuPC()
{
}

/**
 * @note Address: 0x800636EC
 * @note Size: 0x90
 */
MenuItem::~MenuItem()
{
}

/**
 * @note Address: 0x8006377C
 * @note Size: 0x90
 */
MenuStatus::~MenuStatus()
{
}

/**
 * @note Address: 0x8006380C
 * @note Size: 0x90
 */
MenuMain::~MenuMain()
{
}
