#include "game/xobj.h"

// todo: xobjs.cpp's pool, move it to xobjs.h along with it
struct UnkXObjPool {
	u8* _00;         // _00
	s32 _04;         // _04, objects handed out so far
	s32 _08;         // _08, count
	const char* _0C; // _0C, the pool's name, printed when it runs out
	s32 _10;         // _10, size of one object

	XObj* Get(s32 i) { return (XObj*)(_00 + i * _10); }
};
STATIC_ASSERT(sizeof(UnkXObjPool) == 0x14);

/**
 * @note Address: 0x80132D08
 * @note Size: 0x94
 */
// vf94 on every object of the 23 pools
extern "C" void fn_80132D08(UnkXObjPool* pools)
{
	UnkXObjPool* pool = pools;
	for (s32 index = 0; index < 23; ++index, ++pool) {
		for (s32 i = 0; i < pools[index]._08; ++i) {
			pool->Get(i)->vf94();
		}
	}
}
