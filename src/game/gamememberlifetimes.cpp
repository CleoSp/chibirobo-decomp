#define DEFINE_GAME_MEMBER_LIFETIMES
#define DEFER_CTEXOBJ_LIFETIME
#include "game/hsd_jobj.h"
#include "game/cgame.h"

#pragma section RX ".text.gamemembers"

// Deferred inlining emits these definitions in reverse source order.
__declspec(section ".text.gamemembers") MsgFontGlyph::MsgFontGlyph()
{
}
__declspec(section ".text.gamemembers") CTexObj::CTexObj()
{
}
__declspec(section ".text.gamemembers") CMsg::CMsg()
{
}
__declspec(section ".text.gamemembers") CCameraActionDefault::~CCameraActionDefault()
{
}
__declspec(section ".text.gamemembers") CInterpolationEaseInOut::CInterpolationEaseInOut()
{
}
__declspec(section ".text.gamemembers") CInterpolationLinear::CInterpolationLinear()
{
}
__declspec(section ".text.gamemembers") CInterpolationEaseOut::CInterpolationEaseOut()
{
}
__declspec(section ".text.gamemembers") CInterpolationEaseOutDivide::CInterpolationEaseOutDivide()
{
}
__declspec(section ".text.gamemembers") CCameraActionChase::~CCameraActionChase()
{
}
__declspec(section ".text.gamemembers") CCameraActionEvt::CCameraActionEvt()
{
}
__declspec(section ".text.gamemembers") CCameraActionScroll2::~CCameraActionScroll2()
{
}
__declspec(section ".text.gamemembers") CCameraActionEye::~CCameraActionEye()
{
}
__declspec(section ".text.gamemembers") CCameraActionAnim::~CCameraActionAnim()
{
}
__declspec(section ".text.gamemembers") CCameraActionMap::CCameraActionMap()
{
}
__declspec(section ".text.gamemembers") CCameraActionLadr::~CCameraActionLadr()
{
}
__declspec(section ".text.gamemembers") CInterpolationNone::CInterpolationNone()
{
}
__declspec(section ".text.gamemembers") CCameraActionScroll::~CCameraActionScroll()
{
}
