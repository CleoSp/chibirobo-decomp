#include "game/cgame.h"
#include "game/xobjmap.h"
#include "game/xobjs.h"

// todo: cleanup
static inline CLight* MapLights(XObjMap* map)
{
	return map->mLightNum > 0 ? map->mLights : NULL;
}

// todo: cleanup
static inline CLight* LightObjectLights(XObjLight* object)
{
	return object->mLightNum > 0 ? object->mLights : NULL;
}

/**
 * @note Address: 0x8001146C
 * @note Size: 0x1A0
 */
// picks the light list the frame uses, the map's, the light object's or CBase's own
extern "C" void fn_8001146C(CGame* game)
{
	game->_E7E20 = NULL;
	XObjMap* map = (XObjMap*)game->mPools[0].mObjects;
	if ((map->mStatus.mFlags & 1ULL) && MapLights(map) != NULL) {
		game->_E7E20 = MapLights(map);
	} else {
		XObjLight* lights = (XObjLight*)game->_9694[1].mObjects;
		if (lights->mStatus.mFlags & 1ULL)
			game->_E7E20 = LightObjectLights(lights);
	}
	if (game->_E7E20 != NULL) {
		for (CLight* light = game->_E7E20; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive()) {
				HSD_CObjSetProjectionType(light->mShadow.mShadow->camera, PROJ_ORTHO);
			}
		}
		for (CLight* light = (CLight*)game->mLights[0]; light != NULL; light = (CLight*)light->next) {
			if (light->mShadow.IsActive())
				light->mShadow.SetActive(FALSE);
		}
	} else {
		game->_E7E20 = (CLight*)game->mLights[0];
	}
}
