#include <sysdolphin/object.h>

void ObjInfoInit(void);

HSD_ClassInfo hsdObj = { { ObjInfoInit } };

/**
 * @note Address: 0x801D3DC0
 * @note Size: 0x44
 */
void ObjInfoInit(void)
{
    hsdInitClassInfo(&hsdObj, &hsdClass, "sysdolphin_base_library", "hsd_obj", sizeof(HSD_ObjInfo), sizeof(HSD_Obj));
}
