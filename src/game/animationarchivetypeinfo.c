const char sAnimationArchiveTypeName[12]        = "CJArchive";
const char sAnimationJointRotationCondition[40] = "!(jobj->flags & JOBJ_USE_QUATERNION)";
const char sAnimationTranslationField[12]       = "translate";

#if defined(VERSION_GGTP01)
const char sTypeNameCJAnim[8] = "CJAnim";
#else
const char sTypeNameCJAnim[8] = "CJAnim\0"
                                "0";
#endif
extern const void* sBaseTypesCJAnim[];
const void* gTypeInfoCJArchive[] = { sAnimationArchiveTypeName, 0 };
const void* gTypeInfoCJAnim[]    = { sTypeNameCJAnim, sBaseTypesCJAnim };

extern void __dt__6CJAnimFv(void);
extern void Clear__6CJAnimFv(void);
extern void ReadAsync__9CJArchiveFP11CObjArchivePCcUlUlUl(void);
extern void ReadCheck__9CJArchiveFv(void);
extern void Release__6CJAnimFv(void);

const void* sBaseTypesCJAnim[] = { gTypeInfoCJArchive, 0, 0 };
const void* __vt__6CJAnim[]
    = { gTypeInfoCJAnim,   0, __dt__6CJAnimFv, Clear__6CJAnimFv, ReadAsync__9CJArchiveFP11CObjArchivePCcUlUlUl, ReadCheck__9CJArchiveFv,
	    Release__6CJAnimFv };
