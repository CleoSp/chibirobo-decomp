#include "game/cgame.h"
#include "game/cjobjset.h"

#pragma section RX ".text.gamebasedtors"

/**
 * @note Address: 0x80007880
 * @note Size: 0x6BC
 */
__declspec(section ".text.gamebasedtors") CGame::~CGame()
{
}

/**
 * @note Address: 0x80007F3C
 * @note Size: 0x114
 */
__declspec(section ".text.gamebasedtors") CBase::~CBase()
{
}

__declspec(section ".text.gamebasedtors") CGs::~CGs()
{
}
