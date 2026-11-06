#include "game/xobjread.h"
#include "game/xobjs.h"
#include "game/xobjlight.h"
#include "PowerPC_EABI_Support/Runtime/global_destructor_chain.h"
#include "game/xobjactor.h"
#include "game/actorpoolstorage.h"
#include "game/xobjhbox.h"
#include "game/xobjflags.h"
#include "game/xobjspiderboss.h"
#include "game/xobjspider.h"
#include "game/xobjspidersmall.h"
#include "game/xobjarmy.h"
#include "game/xobjleticker.h"
#include "game/xobjsound.h"
#include "game/xchr.h"
#include "game/xobjmap.h"
#include "PowerPC_EABI_Support/Runtime/MWCPlusLib.h"

// the objects XOBJS hands out, their constructors and the weak destructors their arrays are torn
// down with when the game ends

extern "C" {
XObjFree* __dt__8XObjFreeFv(XObjFree* object, s32 flags);
XObjHBox* __dt__8XObjHBoxFv(XObjHBox* object, s32 flags);
XObjDust* __dt__8XObjDustFv(XObjDust* object, s32 flags);
XObjItem* __dt__8XObjItemFv(XObjItem* object, s32 flags);
XObjCoin* __dt__8XObjCoinFv(XObjCoin* object, s32 flags);
XObjSound* __dt__9XObjSoundFv(XObjSound* object, s32 flags);
XObjSpiderSmall* __dt__15XObjSpiderSmallFv(XObjSpiderSmall* object, s32 flags);
XObjSpider* __dt__10XObjSpiderFv(XObjSpider* object, s32 flags);
XObjArmy* __dt__8XObjArmyFv(XObjArmy* object, s32 flags);
XObjActorDeca* __dt__13XObjActorDecaFv(XObjActorDeca* object, s32 flags);
XObjActorToy* __dt__12XObjActorToyFv(XObjActorToy* object, s32 flags);
XObjLeticker* __dt__12XObjLetickerFv(XObjLeticker* object, s32 flags);
}

// Each pool is constructed on first use and registered for destruction exactly once.
// The original keeps the aligned object storage immediately before its destructor-chain record.
extern "C" {
#define DECLARE_OBJECT_POOL(name, type, count)                   \
	u8 g##name##Pool[sizeof(type) * (count)] ATTRIBUTE_ALIGN(8); \
	static DestructorChain s##name##Destructor;                  \
	static s8 s##name##Initialized

DECLARE_OBJECT_POOL(Camera, XObjCamera, 1);
DECLARE_OBJECT_POOL(Light, XObjLight, 1);
DECLARE_OBJECT_POOL(Map, XObjMap, 1);
DECLARE_OBJECT_POOL(Player, XObjChr, 1);
DECLARE_OBJECT_POOL(Actors, XObj, 2);
DECLARE_OBJECT_POOL(Actor3, XObjActor3, 48);
DECLARE_OBJECT_POOL(Sound, XObjSound, 8);
DECLARE_OBJECT_POOL(Coin, XObjCoin, 96);
DECLARE_OBJECT_POOL(Item, XObjItem, 64);
DECLARE_OBJECT_POOL(Leticker, XObjLeticker, 12);
DECLARE_OBJECT_POOL(ActorToy, XObjActorToy, 15);
DECLARE_OBJECT_POOL(ActorJenny, XObjActorJenny, 1);
DECLARE_OBJECT_POOL(ActorMama, XObjActorMama, 1);
DECLARE_OBJECT_POOL(ActorPapa, XObjActorPapa, 1);
DECLARE_OBJECT_POOL(ActorTao, XObjActorTao, 1);
DECLARE_OBJECT_POOL(ActorDeca, XObjActorDeca, 2);
DECLARE_OBJECT_POOL(Army, XObjArmy, 21);
DECLARE_OBJECT_POOL(Spider, XObjSpider, 16);
DECLARE_OBJECT_POOL(SpiderSmall, XObjSpiderSmall, 16);
DECLARE_OBJECT_POOL(SpiderBoss, XObjSpiderBoss, 1);
DECLARE_OBJECT_POOL(Dust, XObjDust, 64);
static s8 sHBoxInitialized;
static s8 sFreeInitialized;

#undef DECLARE_OBJECT_POOL

XObj* __ct__4XObjFv(XObj*);
XObjActor3* __ct__10XObjActor3Fv(XObjActor3*);
XObjActor3* __dt__10XObjActor3Fv(XObjActor3*, s32);
XObjActorDeca* __ct__13XObjActorDecaFv(XObjActorDeca*);
XObjActorJenny* __ct__14XObjActorJennyFv(XObjActorJenny*);
XObjActorJenny* __dt__14XObjActorJennyFv(XObjActorJenny*, s32);
XObjActorMama* __ct__13XObjActorMamaFv(XObjActorMama*);
XObjActorMama* __dt__13XObjActorMamaFv(XObjActorMama*, s32);
XObjActorPapa* __ct__13XObjActorPapaFv(XObjActorPapa*);
XObjActorPapa* __dt__13XObjActorPapaFv(XObjActorPapa*, s32);
XObjActorTao* __ct__12XObjActorTaoFv(XObjActorTao*);
XObjActorTao* __dt__12XObjActorTaoFv(XObjActorTao*, s32);
XObjActorToy* __ct__12XObjActorToyFv(XObjActorToy*);
XObjArmy* __ct__8XObjArmyFv(XObjArmy*);
XObjCamera* __ct__10XObjCameraFv(XObjCamera*);
XObjCamera* __dt__10XObjCameraFv(XObjCamera*, s32);
XObjChr* __ct__7XObjChrFv(XObjChr*);
XObjChr* __dt__7XObjChrFv(XObjChr*, s32);
XObjCoin* __ct__8XObjCoinFv(XObjCoin*);
XObjDust* __ct__8XObjDustFv(XObjDust*);
XObjFree* __ct__8XObjFreeFv(XObjFree*);
XObjHBox* __ct__8XObjHBoxFv(XObjHBox*);
XObjItem* __ct__8XObjItemFv(XObjItem*);
XObjLeticker* __ct__12XObjLetickerFv(XObjLeticker*);
XObjLight* __ct__9XObjLightFv(XObjLight*);
XObjLight* __dt__9XObjLightFv(XObjLight*, s32);
XObjMap* __ct__7XObjMapFv(XObjMap*);
XObjMap* __dt__7XObjMapFv(XObjMap*, s32);
XObjSound* __ct__9XObjSoundFv(XObjSound*);
XObjSpider* __ct__10XObjSpiderFv(XObjSpider*);
XObjSpiderBoss* __ct__14XObjSpiderBossFv(XObjSpiderBoss*);
XObjSpiderBoss* __dt__14XObjSpiderBossFv(XObjSpiderBoss*, s32);
XObjSpiderSmall* __ct__15XObjSpiderSmallFv(XObjSpiderSmall*);
void DestroyXObjActor3Array();
void DestroyXObjActorDecaArray();
void DestroyXObjActorToyArray();
void DestroyXObjArmyArray();
void DestroyXObjArray();
void DestroyXObjCoinArray();
void DestroyXObjDustArray();
void DestroyXObjFreeArray();
void DestroyXObjHBoxArray();
void DestroyXObjItemArray();
void DestroyXObjLetickerArray();
void DestroyXObjSoundArray();
void DestroyXObjSpiderArray();
void DestroyXObjSpiderSmallArray();
void vf90__10XObjActor3FUl(XObjActor3*, u32);
void vf90__10XObjCameraFUl(XObjCamera*, u32);
void vf90__10XObjSpiderFUl(XObjSpider*, u32);
void vf90__12XObjActorToyFUl(XObjActorToy*, u32);
void vf90__14XObjSpiderBossFUl(XObjSpiderBoss*, u32);
void vf90__15XObjSpiderSmallFUl(XObjSpiderSmall*, u32);
void vf90__7XObjChrFUl(XObjChr*, u32);
void vf90__7XObjMapFUl(XObjMap*, u32);
void vf90__8XObjHBoxFUl(XObjHBox*, u32);
void vf90__9XObjLightFUl(XObjLight*, u32);
void vf90__9XObjSoundFUl(XObjSound*, u32);
}

template <class T, int Bytes>
static inline T* SetPool(XObjPool& pool, u8 (&storage)[Bytes], const char* name)
{
	pool.mObjects = storage;
	pool.mUsed    = 0;
	pool.mCount   = Bytes / sizeof(T);
	pool.mName    = name;
	pool.mStride  = sizeof(T);
	return (T*)storage;
}

template <class T, int Bytes, class Base>
static inline void InitSingle(XObjPool& pool, u8 (&storage)[Bytes], DestructorChain& record, s8& initialized, const char* name, u32& id,
                              T* (*ctor)(T*), T* (*dtor)(T*, s32), void (*init)(Base*, u32))
{
	STATIC_ASSERT(Bytes == sizeof(T));
	if (!initialized) {
		__register_global_object(ctor((T*)storage), (void*)dtor, &record);
		initialized = 1;
	}
	T* object = SetPool<T>(pool, storage, name);
	init(object, id);
	++id;
}

template <class T, int Bytes, class Base>
static inline void InitArray(XObjPool& pool, u8 (&storage)[Bytes], DestructorChain& record, s8& initialized, const char* name, u32& id,
                             T* (*ctor)(T*), T* (*dtor)(T*, s32), void (*destroyArray)(), void (*init)(Base*, u32))
{
	enum { Count = Bytes / sizeof(T) };
	STATIC_ASSERT(Bytes == Count * sizeof(T));
	if (!initialized) {
		__construct_array(storage, (ConstructorDestructor)ctor, (ConstructorDestructor)dtor, sizeof(T), Count);
		__register_global_object(NULL, (void*)destroyArray, &record);
		initialized = 1;
	}
	T* objects = SetPool<T>(pool, storage, name);
	for (int i = 0; i < Count; ++i, ++id)
		init(&objects[i], id);
}

extern "C" void InitializeObjectPools(XOBJS* list)
{
	u32 id = 0;
	InitSingle(list->mPools[0], gCameraPool, sCameraDestructor, sCameraInitialized, "CAMERA", id, __ct__10XObjCameraFv,
	           __dt__10XObjCameraFv, vf90__10XObjCameraFUl);
	InitSingle(list->mPools[1], gLightPool, sLightDestructor, sLightInitialized, "LIGHT", id, __ct__9XObjLightFv, __dt__9XObjLightFv,
	           vf90__9XObjLightFUl);
	InitSingle(list->mPools[3], gMapPool, sMapDestructor, sMapInitialized, "MAP", id, __ct__7XObjMapFv, __dt__7XObjMapFv,
	           vf90__7XObjMapFUl);
	InitSingle(list->mPools[2], gPlayerPool, sPlayerDestructor, sPlayerInitialized, "CHR", id, __ct__7XObjChrFv, __dt__7XObjChrFv,
	           vf90__7XObjChrFUl);
	InitArray(list->mPools[4], gActorsPool, sActorsDestructor, sActorsInitialized, "ACTOR2", id, __ct__4XObjFv, __dt__4XObjFv,
	          DestroyXObjArray, vf90__4XObjFUl);
	InitArray(list->mPools[5], gActor3Pool, sActor3Destructor, sActor3Initialized, "ACTOR3", id, __ct__10XObjActor3Fv, __dt__10XObjActor3Fv,
	          DestroyXObjActor3Array, vf90__10XObjActor3FUl);
	InitArray(list->mPools[6], gSoundPool, sSoundDestructor, sSoundInitialized, "SOUND", id, __ct__9XObjSoundFv, __dt__9XObjSoundFv,
	          DestroyXObjSoundArray, vf90__9XObjSoundFUl);
	InitArray(list->mPools[7], gCoinPool, sCoinDestructor, sCoinInitialized, "COIN", id, __ct__8XObjCoinFv, __dt__8XObjCoinFv,
	          DestroyXObjCoinArray, vf90__4XObjFUl);
	InitArray(list->mPools[8], gItemPool, sItemDestructor, sItemInitialized, "ITEM", id, __ct__8XObjItemFv, __dt__8XObjItemFv,
	          DestroyXObjItemArray, vf90__4XObjFUl);
	InitArray(list->mPools[9], gLetickerPool, sLetickerDestructor, sLetickerInitialized, "LETICKER", id, __ct__12XObjLetickerFv,
	          __dt__12XObjLetickerFv, DestroyXObjLetickerArray, vf90__4XObjFUl);
	InitArray(list->mPools[10], gActorToyPool, sActorToyDestructor, sActorToyInitialized, "ACTOR_TOY", id, __ct__12XObjActorToyFv,
	          __dt__12XObjActorToyFv, DestroyXObjActorToyArray, vf90__12XObjActorToyFUl);
	InitSingle(list->mPools[11], gActorJennyPool, sActorJennyDestructor, sActorJennyInitialized, "ACTOR_JENNY", id,
	           __ct__14XObjActorJennyFv, __dt__14XObjActorJennyFv, vf90__9XObjActorFUl);
	InitSingle(list->mPools[12], gActorMamaPool, sActorMamaDestructor, sActorMamaInitialized, "ACTOR_MAMA", id, __ct__13XObjActorMamaFv,
	           __dt__13XObjActorMamaFv, vf90__9XObjActorFUl);
	InitSingle(list->mPools[13], gActorPapaPool, sActorPapaDestructor, sActorPapaInitialized, "ACTOR_PAPA", id, __ct__13XObjActorPapaFv,
	           __dt__13XObjActorPapaFv, vf90__9XObjActorFUl);
	InitSingle(list->mPools[14], gActorTaoPool, sActorTaoDestructor, sActorTaoInitialized, "ACTOR_TAO", id, __ct__12XObjActorTaoFv,
	           __dt__12XObjActorTaoFv, vf90__9XObjActorFUl);
	InitArray(list->mPools[15], gActorDecaPool, sActorDecaDestructor, sActorDecaInitialized, "ACTOR_DECA", id, __ct__13XObjActorDecaFv,
	          __dt__13XObjActorDecaFv, DestroyXObjActorDecaArray, vf90__9XObjActorFUl);
	InitArray(list->mPools[16], gArmyPool, sArmyDestructor, sArmyInitialized, "ARMY", id, __ct__8XObjArmyFv, __dt__8XObjArmyFv,
	          DestroyXObjArmyArray, vf90__4XObjFUl);
	InitArray(list->mPools[17], gSpiderPool, sSpiderDestructor, sSpiderInitialized, "SPIDER", id, __ct__10XObjSpiderFv,
	          __dt__10XObjSpiderFv, DestroyXObjSpiderArray, vf90__10XObjSpiderFUl);
	InitArray(list->mPools[18], gSpiderSmallPool, sSpiderSmallDestructor, sSpiderSmallInitialized, "SPIDER_SMALL", id,
	          __ct__15XObjSpiderSmallFv, __dt__15XObjSpiderSmallFv, DestroyXObjSpiderSmallArray, vf90__15XObjSpiderSmallFUl);
	InitSingle(list->mPools[19], gSpiderBossPool, sSpiderBossDestructor, sSpiderBossInitialized, "SPIDER_BOSS", id,
	           __ct__14XObjSpiderBossFv, __dt__14XObjSpiderBossFv, vf90__14XObjSpiderBossFUl);
	InitArray(list->mPools[20], gDustPool, sDustDestructor, sDustInitialized, "DUST", id, __ct__8XObjDustFv, __dt__8XObjDustFv,
	          DestroyXObjDustArray, vf90__4XObjFUl);
	InitArray(list->mPools[21], gHBoxPool, sHBoxDestructor, sHBoxInitialized, "HBOX", id, __ct__8XObjHBoxFv, __dt__8XObjHBoxFv,
	          DestroyXObjHBoxArray, vf90__8XObjHBoxFUl);
	InitArray(list->mPools[22], gFreePool, sFreeDestructor, sFreeInitialized, "FREE", id, __ct__8XObjFreeFv, __dt__8XObjFreeFv,
	          DestroyXObjFreeArray, vf90__4XObjFUl);
	list->_11D4 = 0;
	gAllObjectsReader.StartReader(ReadObjectListThread, NULL, 8192, 17, OS_THREAD_ATTR_DETACH);
	gSingleObjectReader.StartReader(ReadSingleObjectThread, NULL, 8192, 16, OS_THREAD_ATTR_DETACH);
	gObjectAnimationReader.StartReader(ReadObjectAnimationsThread, NULL, 8192, 16, OS_THREAD_ATTR_DETACH);
}

/**
 * @note Address: 0x80132D08
 * @note Size: 0x94
 */
// vf94 on every object of the 23 pools
extern "C" void fn_80132D08(XObjPool* pools)
{
	XObjPool* pool = pools;
	for (s32 index = 0; index < 23; ++index, ++pool) {
		for (s32 i = 0; i < pool->mCount; ++i) {
			pools[index].Get(i)->vf94();
		}
	}
}

/**
 * @note Address: 0x80133B98
 * @note Size: 0x38
 */
extern "C" void DestroyXObjFreeArray()
{
	__destroy_arr(gFreePool, (ConstructorDestructor*)__dt__8XObjFreeFv, sizeof(XObjFree), sizeof(gFreePool) / sizeof(XObjFree));
}

/**
 * @note Address: 0x80133BD0
 * @note Size: 0x3C
 */
XObjFree::XObjFree()
{
}

/**
 * @note Address: 0x80133C0C
 * @note Size: 0x38
 */
extern "C" void DestroyXObjHBoxArray()
{
	__destroy_arr(gHBoxPool, (ConstructorDestructor*)__dt__8XObjHBoxFv, sizeof(XObjHBox), sizeof(gHBoxPool) / sizeof(XObjHBox));
}

/**
 * @note Address: 0x80133C44
 * @note Size: 0x44
 */
XObjHBox::XObjHBox()
{
}

/**
 * @note Address: 0x80133C88
 * @note Size: 0x30
 */
XObjHBoxPart::XObjHBoxPart()
{
}

/**
 * @note Address: 0x80133CB8
 * @note Size: 0x38
 */
extern "C" void DestroyXObjDustArray()
{
	__destroy_arr(gDustPool, (ConstructorDestructor*)__dt__8XObjDustFv, sizeof(XObjDust), sizeof(gDustPool) / sizeof(XObjDust));
}

/**
 * @note Address: 0x80133CF0
 * @note Size: 0x48
 */
XObjDust::XObjDust()
{
	mFlags = &gDustEventFlags;
}

/**
 * @note Address: 0x80133D7C
 * @note Size: 0x5C
 */
XObjSpiderBoss::XObjSpiderBoss()
{
}

/**
 * @note Address: 0x80133DD8
 * @note Size: 0x38
 */
extern "C" void DestroyXObjSpiderSmallArray()
{
	__destroy_arr(gSpiderSmallPool, (ConstructorDestructor*)__dt__15XObjSpiderSmallFv, sizeof(XObjSpiderSmall),
	              sizeof(gSpiderSmallPool) / sizeof(XObjSpiderSmall));
}

/**
 * @note Address: 0x80133E10
 * @note Size: 0x74
 */
XObjSpiderSmall::XObjSpiderSmall()
{
}

/**
 * @note Address: 0x80133E84
 * @note Size: 0x38
 */
extern "C" void DestroyXObjSpiderArray()
{
	__destroy_arr(gSpiderPool, (ConstructorDestructor*)__dt__10XObjSpiderFv, sizeof(XObjSpider), sizeof(gSpiderPool) / sizeof(XObjSpider));
}

/**
 * @note Address: 0x80133EBC
 * @note Size: 0x84
 */
XObjSpider::XObjSpider()
{
}

/**
 * @note Address: 0x80133F40
 * @note Size: 0x38
 */
extern "C" void DestroyXObjArmyArray()
{
	__destroy_arr(gArmyPool, (ConstructorDestructor*)__dt__8XObjArmyFv, sizeof(XObjArmy), sizeof(gArmyPool) / sizeof(XObjArmy));
}

/**
 * @note Address: 0x80133F78
 * @note Size: 0x88
 */
XObjArmy::XObjArmy()
{
}

/**
 * @note Address: 0x80134014
 * @note Size: 0x38
 */
// 0x80134000, XObjArmyShot's ctor, the compiler emits it here

extern "C" void DestroyXObjActorDecaArray()
{
	__destroy_arr(gActorDecaPool, (ConstructorDestructor*)__dt__13XObjActorDecaFv, sizeof(XObjActorDeca),
	              sizeof(gActorDecaPool) / sizeof(XObjActorDeca));
}

/**
 * @note Address: 0x8013404C
 * @note Size: 0x58
 */
XObjActorDeca::XObjActorDeca()
{
}

/**
 * @note Address: 0x80134100
 * @note Size: 0x50
 */
XObjActorTao::XObjActorTao()
{
}

/**
 * @note Address: 0x80134150
 * @note Size: 0x50
 */
XObjActorPapa::XObjActorPapa()
{
}

/**
 * @note Address: 0x801341A0
 * @note Size: 0x50
 */
XObjActorMama::XObjActorMama()
{
}

/**
 * @note Address: 0x801341F0
 * @note Size: 0x58
 */
XObjActorJenny::XObjActorJenny()
{
}

/**
 * @note Address: 0x80134248
 * @note Size: 0x38
 */
extern "C" void DestroyXObjActorToyArray()
{
	__destroy_arr(gActorToyPool, (ConstructorDestructor*)__dt__12XObjActorToyFv, sizeof(XObjActorToy),
	              sizeof(gActorToyPool) / sizeof(XObjActorToy));
}

/**
 * @note Address: 0x80134280
 * @note Size: 0x80
 */
XObjActorToy::XObjActorToy()
{
}

/**
 * @note Address: 0x801340A4
 * @note Size: 0x5C
 */
XObjActor::XObjActor()
{
}

/**
 * @note Address: 0x80134300
 * @note Size: 0x38
 */
extern "C" void DestroyXObjLetickerArray()
{
	__destroy_arr(gLetickerPool, (ConstructorDestructor*)__dt__12XObjLetickerFv, sizeof(XObjLeticker),
	              sizeof(gLetickerPool) / sizeof(XObjLeticker));
}

/**
 * @note Address: 0x80134338
 * @note Size: 0x48
 */
XObjLeticker::XObjLeticker()
{
}

/**
 * @note Address: 0x80134380
 * @note Size: 0x38
 */
extern "C" void DestroyXObjItemArray()
{
	__destroy_arr(gItemPool, (ConstructorDestructor*)__dt__8XObjItemFv, sizeof(XObjItem), sizeof(gItemPool) / sizeof(XObjItem));
}

/**
 * @note Address: 0x801343B8
 * @note Size: 0x48
 */
XObjItem::XObjItem()
{
	mFlags = &gItemEventFlags;
}

/**
 * @note Address: 0x80134400
 * @note Size: 0x38
 */
extern "C" void DestroyXObjCoinArray()
{
	__destroy_arr(gCoinPool, (ConstructorDestructor*)__dt__8XObjCoinFv, sizeof(XObjCoin), sizeof(gCoinPool) / sizeof(XObjCoin));
}

/**
 * @note Address: 0x80134438
 * @note Size: 0x48
 */
XObjCoin::XObjCoin()
{
	mFlags = &gCoinEventFlags;
}

/**
 * @note Address: 0x80133D38
 * @note Size: 0x44
 */
XObjFlags::XObjFlags()
{
	mFlags = NULL;
}

/**
 * @note Address: 0x80134480
 * @note Size: 0x38
 */
extern "C" void DestroyXObjSoundArray()
{
	__destroy_arr(gSoundPool, (ConstructorDestructor*)__dt__9XObjSoundFv, sizeof(XObjSound), sizeof(gSoundPool) / sizeof(XObjSound));
}

/**
 * @note Address: 0x801344B8
 * @note Size: 0x48
 */
XObjSound::XObjSound()
{
}
