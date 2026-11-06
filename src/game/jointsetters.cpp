#define DEFER_OBJECT_DESTRUCTORS
#define CJOINT_SETTER_SECTION
#pragma section RX ".text.jointdtors"
#include "game/cjobjset.h"

CJObj::~CJObj()
{
}
