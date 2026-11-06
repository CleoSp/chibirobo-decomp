#define DEFER_CSCRNWIPECIRCLE_DESTRUCTOR
#include "game/hsd_jobj.h"
#include "game/cscrn.h"

#pragma section RX ".text.circledtor"
__declspec(section ".text.circledtor") CScrnWipeCircle::~CScrnWipeCircle()
{
}
