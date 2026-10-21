#include <sysdolphin/list.h>

HSD_ObjAllocData slist_alloc_data;
HSD_ObjAllocData dlist_alloc_data;

/**
 * @note Address: 0x801D3B10
 * @note Size: 0x44
 */
void HSD_ListInitAllocData()
{
    HSD_ObjAllocInit(&slist_alloc_data, sizeof(HSD_SList), 4);
    HSD_ObjAllocInit(&dlist_alloc_data, sizeof(HSD_DList), 4);
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
HSD_ObjAllocData* HSD_SListGetAllocData()
{
    return &slist_alloc_data;
}

/**
 * @note Address: N/A
 * @note Size: 0xC
 */
HSD_ObjAllocData* HSD_DListGetAllocData()
{
    return &dlist_alloc_data;
}

/**
 * @note Address: 0x801D3B54
 * @note Size: 0x5C
 */
HSD_SList *HSD_SListAlloc()
{
    HSD_SList *list;

    list = HSD_ObjAlloc(&slist_alloc_data);
    assert_line(76, list);

    memset(list, 0, sizeof(HSD_SList));
    return list;
}

/**
 * @note Address: 0x801D3BB0
 * @note Size: 0x80
 */
HSD_SList* HSD_SListAppend(HSD_SList* list, void* data)
{
    HSD_SList* next = HSD_SListAlloc();
    next->data = data;
    return HSD_SListAppendList(list, next);
}

/**
 * @note Address: 0x801D3C30
 * @note Size: 0x80
 */
HSD_SList* HSD_SListPrepend(HSD_SList* list, void* data)
{
    HSD_SList* next = HSD_SListAlloc();
    next->data = data;
    return HSD_SListPrependList(list, next);
}

/**
 * @note Address: 0x801D3CB0
 * @note Size: 0x70
 */
HSD_SList* HSD_SListAppendList(HSD_SList* list, HSD_SList* next)
{
    assert_line(179, next);
    if (list != NULL) {
        next->next = list->next;
        list->next = next;
        return list;
    }
    next->next = NULL;
    return next;
}

/**
 * @note Address: 0x801D3D20
 * @note Size: 0x50
 */
HSD_SList *HSD_SListPrependList(HSD_SList *list, HSD_SList *prev)
{
    assert_line(202, prev);
    prev->next = list;
    return prev;
}

/**
 * @note Address: 0x801D3D70
 * @note Size: 0x50
 */
HSD_SList *HSD_SListRemove(HSD_SList *list)
{
    HSD_SList *next;

    if (list != NULL) {
        next = list->next;
        HSD_ObjFree(&slist_alloc_data, list);
        return next;
    }

    return NULL;
}
