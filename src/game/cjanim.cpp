#include "game/cobj.h"
#include "game/cfile.h"
#include "game/cgame.h"

extern "C" CObjArchive* fn_80132620(CObjArchive* archives);

/**
 * @note Address: 0x8008A360
 * @note Size: 0x2C
 */
// CJAnim::Release
extern "C" void fn_8008A360(CJAnim* anim)
{
	if (anim->mArchive != NULL)
		anim->CJArchive::Release();
}

/**
 * @note Address: 0x8008A38C
 * @note Size: 0x170
 */
// starts the read, then sets it up once it's in, TRUE when done
extern "C" BOOL fn_8008A38C(CJArchive* archive, const char* path)
{
	CJAnim* anim = (CJAnim*)archive;
	if (anim->mLoading == 0) {
		anim->Release();
		if (path == NULL || *path == '\0')
			return TRUE;
		CFile file;
		if (file.ConvertPathToEntrynum(path) < 0)
			return TRUE;
		CObjArchive* storage = fn_80132620((CObjArchive*)lbl_8025C400.mObjArchives);
		if (storage != NULL) {
			anim->ReadAsync(storage, path, 0, 0, 0);
			anim->mLoading++;
		} else {
			return TRUE;
		}
	} else {
		if (anim->ReadCheck()) {
			anim->SetupScene(0, 0);
			anim->mLoading = 0;
		}
	}
	return anim->mLoading == 0;
}
