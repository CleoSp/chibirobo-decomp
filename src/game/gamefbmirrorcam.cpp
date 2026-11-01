#include "game/fbtexeff.h"
#include "game/cgame.h"
#include "game/hsd_jobj.h"
extern "C" {
#include <dolphin/mtx/vec.h>
#include <sysdolphin/mtx_transform.h>
}

/**
 * @note Address: 0x80014DA4
 * @note Size: 0x2BC
 */
// FbMirror's Update, reflects the game camera's eye in the joint's xy plane
extern "C" void fn_80014DA4(FbMirror* mirror)
{
	if (mirror->mJoint != NULL) {
		Mtx transform, inverse;
		Vec eye, origin, delta, x, y, normal;
		PSMTXCopy(HSD_JObjGetMtxPtr(mirror->mJoint), transform);
		x.x = transform[0][0];
		x.y = transform[1][0];
		x.z = transform[2][0];
		y.x = transform[0][1];
		y.y = transform[1][1];
		y.z = transform[2][1];
		PSVECScale(&x, &x, -1.0f);
		PSVECCrossProduct(&x, &y, &normal);
		PSVECNormalize(&normal, &normal);
		transform[0][0] = x.x;
		transform[1][0] = x.y;
		transform[2][0] = x.z;
		transform[0][2] = normal.x;
		transform[1][2] = normal.y;
		transform[2][2] = normal.z;
		CCamera* camera = (CCamera*)lbl_8025C400.mCamera;
		eye.x           = camera->mEye.x;
		eye.y           = camera->mEye.y;
		eye.z           = camera->mEye.z;
		HSD_MtxGetTranslate(transform, &origin);
		PSVECSubtract(&origin, &eye, &delta);
		f32 distance = PSVECDotProduct(&normal, &delta);
		if (distance <= 0.0f) {
			mirror->mVisible = FALSE;
		} else {
			PSVECScale(&normal, &delta, 2.0f * distance);
			PSVECAdd(&delta, &eye, &delta);
			transform[0][3] = delta.x;
			transform[1][3] = delta.y;
			transform[2][3] = delta.z;
			PSMTXInverse(transform, inverse);
			PSMTXMultVec(inverse, &origin, &origin);
			mirror->mCamera.SetNear(0.1f - origin.z);
			mirror->mCamera.SetFar(32768.0f + (0.1f - origin.z));
			mirror->mCamera.SetFrustum(0.5f + origin.y, -0.5f + origin.y, -0.5f + origin.x, 0.5f + origin.x);
			mirror->mCamera.SetEye(delta);
			mirror->mCamera.SetMtx(inverse);
			PSMTXCopy(inverse, mirror->mView);
			mirror->mVisible = TRUE;
		}
	}
}
