#include "Dolphin/gx.h"
#include "game/cmodule.h"
#include "game/cfile.h"
#include <stdio.h>
#include <string.h>
#include "types.h"

struct StageDataFile : CFileA {
	u8* header;
	u8* data;
	u8* objects;
	u8* scripts;
};
extern "C" StageDataFile lbl_803675CC;

// Stage tables store their offsets as four little-endian bytes.
static inline u8* StageOffset(u8* base, const u8* p)
{
	return (u8*)((u32)p[0] + ((u32)p[1] << 8) + ((u32)p[2] << 16) + ((u32)p[3] << 24) + (u32)base);
}
static inline u32 StageWord(const u8* p)
{
	return p[0] + ((u32)p[1] << 8) + ((u32)p[2] << 16) + ((u32)p[3] << 24);
}
static inline u8* StageRecord(s32 table, s32 stride, s32 index)
{
	u8* base = lbl_803675CC.data;
	return StageOffset(base, base + table) + index * stride;
}
extern "C" {
extern u8 lbl_8025C400[];
#if defined(VERSION_JP)
// japan has one language, so one table each
extern const char* lbl_802163C4[];
extern const char* lbl_8021617C[];
#else
// a string table per language
extern const char** lbl_802246EC[];
extern const char** lbl_802246D0[];
#endif

const char* fn_8006DBC8(s32 index)
{
#if defined(VERSION_JP)
	return lbl_802163C4[index];
#else
	return lbl_802246EC[lbl_8025C400[0x3318]][index];
#endif
}
const char* fn_8006DBF0(s32 index)
{
#if defined(VERSION_JP)
	return lbl_8021617C[index];
#else
	return lbl_802246D0[lbl_8025C400[0x3318]][index];
#endif
}
u8* fn_8006DC18(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(80, 8, index) + 4);
}
u8* fn_8006DC8C(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(80, 8, index) + 0);
}
u8* fn_8006DD44(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(76, 12, index) + 4);
}
u8* fn_8006DDB8(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(76, 12, index) + 0);
}
u8* fn_8006DE70(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(72, 12, index) + 4);
}
u8* fn_8006DEE4(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(72, 12, index) + 0);
}
u8* fn_8006DF54(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(68, 4, index) + 0);
}
u8* fn_8006DFC4(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(56, 8, index) + 0);
}
u8* fn_8006E034(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(52, 4, index) + 0);
}
u8* fn_8006E0EC(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(48, 12, index) + 4);
}
u8* fn_8006E160(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(48, 12, index) + 0);
}
u8* fn_8006E1D0(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(44, 4, index) + 0);
}
u8* fn_8006E49C(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(40, 20, index) + 4);
}
u8* fn_8006E510(s32 index)
{
	return StageOffset(lbl_803675CC.data, StageRecord(40, 20, index) + 0);
}
s32 fn_8006DCFC(s32 index)
{
	s32 at    = index * 12;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 76);
	return *(u32*)((u32)table + 8 + at);
}
s32 fn_8006DE28(s32 index)
{
	s32 at    = index * 12;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 72);
	return *(s16*)((u32)table + 8 + at);
}
s32 fn_8006E0A4(s32 index)
{
	s32 at    = index * 12;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 48);
	return *(s16*)((u32)table + 8 + at);
}
s32 fn_8006E28C(s32 index)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(s16*)((u32)table + 16 + at);
}
s32 fn_8006E2D4(s32 index)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(s16*)((u32)table + 14 + at);
}
s32 fn_8006E31C(s32 index)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(s16*)((u32)table + 12 + at);
}
s32 fn_8006E364(s32 index)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(s16*)((u32)table + 10 + at);
}
u8 fn_8006E240(s32 index, s32 which)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(u8*)((u32)table + 0x12 + at + which);
}
void fn_8006E3AC(s32 index, u16 mask)
{
	s32 at     = index * 20;
	u8* table  = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	u16* flags = (u16*)((u32)table + 8 + at);
	*flags &= ~mask;
}
void fn_8006E3FC(s32 index, u16 mask)
{
	s32 at     = index * 20;
	u8* table  = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	u16* flags = (u16*)((u32)table + 8 + at);
	*flags |= mask;
}
u16 fn_8006E44C(s32 index, u16 mask)
{
	s32 at    = index * 20;
	u8* table = StageOffset(lbl_803675CC.data, lbl_803675CC.data + 40);
	return *(u16*)((u32)table + 8 + at) & mask;
}
}

extern "C" {
extern u8 lbl_803672CC[];
s32 fn_8006E7FC(s32 index)
{
	return StageWord(StageRecord(36, 4, index));
}
s32 fn_8006E868(s32 index)
{
	return StageWord(StageRecord(32, 4, index));
}
s32 fn_8006E8D4(s32 index)
{
	return StageWord(StageRecord(28, 4, index));
}
s32 fn_8006E940(s32 index)
{
	return StageWord(StageRecord(24, 4, index));
}
s32 fn_8006E9AC(s32 index)
{
	return StageWord(StageRecord(20, 4, index));
}
s32 fn_8006EA18(s32 index)
{
	return StageWord(StageRecord(16, 4, index));
}
s32 fn_8006EA84(s32 index)
{
	return StageWord(StageRecord(12, 4, index));
}
s32 fn_8006EAF0(s32 index)
{
	return StageWord(StageRecord(8, 4, index));
}
s32 fn_8006EB5C(s32 index)
{
	return StageWord(StageRecord(4, 4, index));
}
s32 fn_8006EBC8(s32 index)
{
	return StageWord(StageRecord(0, 4, index));
}
u8* fn_8006E724()
{
	return lbl_803675CC.scripts;
}
// whether a script is one of the globals file's own
static inline BOOL IsGlobalScript(const u8* p)
{
	bool inside = false;
	if (p >= lbl_803675CC.scripts && p < lbl_803675CC.scripts + StageWord(lbl_803675CC.header + 0x14)) {
		inside = true;
	}
	return inside != false;
}
s32 fn_8006E66C(const u8* p)
{
	return IsGlobalScript(p);
}
static inline u8* NullableStageOffset(u8* base, const u8* p)
{
	u32 offset = StageWord(p);
	if (offset == 0)
		return NULL;
	return base + offset;
}
u8* fn_8006E6D4(s32 index)
{
	return NullableStageOffset(lbl_803675CC.scripts, lbl_803675CC.scripts + index * 4);
}

static inline u8* ReadStageAddress(u8* const& base, const u8* p)
{
	u32 offset = StageWord(p);
	if (offset == 0)
		return NULL;
	return base + offset;
}
u8* fn_8006E580(const u8* p)
{
	if (fn_8006E66C(p))
		return ReadStageAddress(lbl_803675CC.scripts, p);
	return ReadStageAddress(*(u8**)(lbl_803672CC + 0x150), p);
}

bool fn_8006EC34()
{
	return *(s32*)((u8*)&lbl_803675CC + 0xCC) != 0;
}
s32 fn_8006F390(s32 index)
{
	return ((s16*)(*(u8**)(lbl_803672CC + 0x170) + 10))[index];
}
u32 fn_8006F3AC()
{
	return *(u32*)(lbl_803672CC + 424);
}
u32 fn_8006F3BC()
{
	return *(u32*)(lbl_803672CC + 420);
}
u32 fn_8006F3CC()
{
	return *(u32*)(lbl_803672CC + 416);
}
u32 fn_8006F3DC()
{
	return *(u32*)(lbl_803672CC + 412);
}
u32 fn_8006F3EC()
{
	return *(u32*)(lbl_803672CC + 404);
}
u32 fn_8006F3FC()
{
	return *(u32*)(lbl_803672CC + 400);
}
u32 fn_8006F40C()
{
	return *(u32*)(lbl_803672CC + 396);
}
u32 fn_8006F41C()
{
	return *(u32*)(lbl_803672CC + 392);
}
u32 fn_8006F42C()
{
	return *(u32*)(lbl_803672CC + 388);
}
}

extern "C" {
extern s32 lbl_80215050[];
extern u8 lbl_80212D88[], spawnTable[], objTable[], objTableEx[], lbl_80216C30[];
struct StageEntry {
	u8* file;
	s32 text;
	u32 flags;
};
extern StageEntry stageTable[];
s32 fn_8006ED90(s32 index)
{
	if (index < 0)
		return -1;
	return lbl_80215050[index];
}
u8* fn_8006EDB4(s32 index)
{
	return lbl_80212D88 + index * 0x38;
}
u8* GetSpawnData(s32 index)
{
	return spawnTable + index * 0x38;
}
u8* GetObjData(s32** object)
{
	s32 index = **object;
	if (index >= 10000)
		return objTableEx + (index - 10000) * 0x18;
	return objTable + index * 0x18;
}
u8* fn_8006EE18()
{
	return lbl_80216C30;
}
u32 fn_8006F490()
{
	return *(u32*)(lbl_803672CC + 336);
}
u8* fn_8006F4A0(s32 index)
{
	return *(u8**)(lbl_803672CC + 376) + index * 0x38;
}
u8* fn_8006F4B8()
{
	return lbl_803672CC + 0x170;
}
s32 fn_8006F4C8()
{
	return *(u32*)(lbl_803672CC + 336) != 0;
}
s32 GetStageNum()
{
	s32 count = 0;
	while (stageTable[count].file)
		++count;
	return count;
}
}

extern "C" {
extern s32 lbl_802239F0[];
u8* fn_8006E734(s32 index)
{
	if (index >= 10000) {
		index = lbl_802239F0[index - 10000];
		if (index < 0)
			return NULL;
	}
	u8* p = lbl_803675CC.objects + index * 4;
	if (*(u32*)p)
		return StageOffset(lbl_803675CC.objects, p);
	return NULL;
}

u8* fn_8006F43C(s32 index)
{
	return ReadStageAddress(*(u8**)(lbl_803672CC + 336), *(u8**)(lbl_803672CC + 384) + index * 4);
}

static inline s32 StageIndex(s32 index)
{
	if (index < 100)
		return index;
	s32 n = GetStageNum();
	n += index;
	return n - 99;
}
u32 CheckStageFlag(s32 index, u32 mask)
{
	index = StageIndex(index);
	return ((StageEntry*)((u32)stageTable + index * sizeof(StageEntry)))->flags & mask;
}
u8* GetStageFileName(s32 index)
{
	index = StageIndex(index);
	return stageTable[index].file;
}
u8* GetStageName(s32 index)
{
	index    = StageIndex(index);
	s32 text = ((StageEntry*)((u32)stageTable + index * sizeof(StageEntry)))->text;
	if (text >= 0)
		return StageOffset(lbl_803675CC.data, StageRecord(0x38, 8, text));
	return NULL;
}
u8* GetStageDesc(s32 index)
{
	index    = StageIndex(index);
	s32 text = ((StageEntry*)((u32)stageTable + index * sizeof(StageEntry)))->text;
	if (text >= 0)
		return StageOffset(lbl_803675CC.data, StageRecord(0x38, 8, text) + 4);
	return NULL;
}
}

extern "C" s32 fn_8006F654(const char* name)
{
	s32 index = 0;
	for (; stageTable[index].file; index++)
		if (strcmp(name, (char*)stageTable[index].file) == 0)
			return index;
	index++;
	while (stageTable[index].file) {
		if (strcmp(name, (char*)stageTable[index].file) == 0)
			return index - GetStageNum() + 99;
		index++;
	}
	return -1;
}

extern "C" {
extern const char* lbl_802244C8[];
extern const char* lbl_8065A390;
s32 fn_8006E7BC()
{
	if (*(u64*)(lbl_8025C400 + 0x1B10) & 0x0008000000000000ULL)
		return 0;
	return 14;
}
static inline BOOL ReadStageFile(CFile* file, const char* path)
{
	return file->ReadEx(path, 0, 0);
}
bool fn_8006EC50()
{
	char path[72];
	sprintf(path, "bin%s/%s", lbl_802244C8[lbl_8025C400[0x3318]], lbl_8065A390);
	s32 result = ReadStageFile(&lbl_803675CC, path);
	if (result) {
		lbl_803675CC.CloseHandle();
		lbl_803675CC.header  = (u8*)lbl_803675CC.mBuffer;
		u8* buffer           = lbl_803675CC.header;
		lbl_803675CC.data    = StageOffset(buffer, buffer) + 0x18;
		lbl_803675CC.objects = StageOffset(buffer, buffer + 8) + 0x18;
		lbl_803675CC.scripts = StageOffset(buffer, buffer + 16) + 0x18;
		return true;
	}
	return false;
}
}

struct StageRuntime {
	char path[64];
	char modulePath[64];
	s32 state;
	CFileA file;
	u8* header;
	u8* _174;
	u8* spawns;
	u8* _17C;
	u8* scripts;
	u8* tables[10];
	CModule module;
};
typedef char StageRuntime_size_check[sizeof(StageRuntime) == 0x2F0 ? 1 : -1];
extern "C" {
extern char* fn_801911B4(const char*, ...);
extern s32 fn_8001F97C(void*, s32);
extern u8 lbl_80344224[];
extern const char lbl_8065CA58[];
s32 fn_8006EE24(StageRuntime* stage, s32 index)
{
	switch (stage->state) {
	case 0:
		strcpy(stage->path, fn_801911B4("bin%s/%s.bin", lbl_802244C8[lbl_8025C400[0x3318]], stageTable[index].file));
		strcpy(stage->modulePath, fn_801911B4("rel/%s%s.rel", stageTable[index].file, ""));
		if (!(*(u64*)(lbl_8025C400 + 0x1B10) & 0x100000000ULL))
			stage->modulePath[0] = 0;
		stage->state++;
	case 1:
		if (!ReadStageFile(&stage->file, stage->path))
			break;
		stage->file.CloseHandle();
		{
			u8* data         = (u8*)stage->file.mBuffer;
			stage->header    = StageOffset(data, data);
			stage->spawns    = StageOffset(data, data + 4);
			stage->scripts   = StageOffset(data, data + 8);
			stage->tables[0] = StageOffset(data, data + 12);
			if (stage->tables[0] == data)
				stage->tables[0] = NULL;
			stage->tables[1] = StageOffset(data, data + 16);
			if (stage->tables[1] == data)
				stage->tables[1] = NULL;
			stage->tables[2] = StageOffset(data, data + 20);
			if (stage->tables[2] == data)
				stage->tables[2] = NULL;
			stage->tables[3] = StageOffset(data, data + 24);
			if (stage->tables[3] == data)
				stage->tables[3] = NULL;
			stage->tables[4] = StageOffset(data, data + 28);
			if (stage->tables[4] == data)
				stage->tables[4] = NULL;
			stage->tables[5] = StageOffset(data, data + 32);
			if (stage->tables[5] == data)
				stage->tables[5] = NULL;
			stage->tables[6] = StageOffset(data, data + 36);
			stage->tables[7] = StageOffset(data, data + 40);
			if (stage->tables[7] == data)
				stage->tables[7] = NULL;
			stage->tables[8] = StageOffset(data, data + 44);
			if (stage->tables[8] == data)
				stage->tables[8] = NULL;
			stage->tables[9] = StageOffset(data, data + 48);
			if (stage->tables[9] == data)
				stage->tables[9] = NULL;
		}
		stage->state++;
	case 2:
		if (stage->modulePath[0] != 0) {
			if (!stage->module.Load(stage->modulePath))
				break;
			stage->state++;
		} else
			stage->state++;
	case 3:
		stage->state++;
	case 4:
		if (fn_8001F97C(lbl_8025C400 + 0xE7E24, index))
			stage->state++;
	}
	return stage->state > 4;
}
s32 fn_8006F5D4(s32 index)
{
	return fn_8006EE24((StageRuntime*)lbl_803672CC, index);
}
static inline bool StageModuleLoaded(CModule* module)
{
	return module->IsLoaded();
}
static inline s32 StageModuleCall(CModule* module, s32 cmd, void* arg, void* arg2)
{
	if (StageModuleLoaded(module))
		return ((CModuleProlog)module->mModule->prolog)(cmd, arg, arg2);
	return -1;
}
static inline void CloseStageFile(CFile* file)
{
	file->Close();
}
s32 fn_8006F338(s32 cmd, void* arg, void* arg2)
{
	CModule* module = &((StageRuntime*)lbl_803672CC)->module;
	if (StageModuleLoaded(module))
		return StageModuleCall(module, cmd, arg, arg2);
	return -1;
}
void fn_8006F544()
{
	if (StageModuleLoaded(&((StageRuntime*)lbl_803672CC)->module))
		StageModuleCall(&((StageRuntime*)lbl_803672CC)->module, 6, NULL, NULL);
	((StageRuntime*)lbl_803672CC)->module.Unload();
	CloseStageFile(&((StageRuntime*)lbl_803672CC)->file);
	((StageRuntime*)lbl_803672CC)->state = 0;
}
}

extern "C" u8 HSD_VIData[];
static inline void StageClearColor(GXColor color)
{
	HSD_VIData[0x48] = color.r;
	HSD_VIData[0x49] = color.g;
	HSD_VIData[0x4A] = color.b;
	HSD_VIData[0x4B] = color.a;
}
extern "C" void fn_8006F4E4()
{
	StageClearColor(*(GXColor*)((StageRuntime*)lbl_803672CC)->header);
	((StageRuntime*)lbl_803672CC)->module.Print();
}
