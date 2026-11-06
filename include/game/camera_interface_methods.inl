CAMERA_METHOD void CInterfaceCameraAction::PreviousUpdate() {}

CAMERA_METHOD void CInterfaceCameraAction::SetCamera(CCamera* camera) {}

CAMERA_METHOD void CInterfaceCameraAction::Init() {}

CAMERA_METHOD void CInterfaceCameraAction::vf20() {}

CAMERA_METHOD void CInterfaceCameraAction::vf24() {}

CAMERA_METHOD void CInterfaceCameraAction::vf28() {}

CAMERA_METHOD void CInterfaceCameraAction::SetEye(const Vec& eye)
{
	SetEye(eye.x, eye.y, eye.z);
}

CAMERA_METHOD void CInterfaceCameraAction::SetEye(f32 x, f32 y, f32 z)
{
	mEye.x = x;
	mEye.y = y;
	mEye.z = z;
}

CAMERA_METHOD void CInterfaceCameraAction::SetInterest(const Vec& interest)
{
	SetInterest(interest.x, interest.y, interest.z);
}

CAMERA_METHOD void CInterfaceCameraAction::SetInterest(f32 x, f32 y, f32 z)
{
	mInterest.x = x;
	mInterest.y = y;
	mInterest.z = z;
}

CAMERA_METHOD void CInterfaceCameraAction::SetAngle(const Vec& angle)
{
	SetAngle(angle.x, angle.y, angle.z);
}

CAMERA_METHOD void CInterfaceCameraAction::SetAngle(f32 x, f32 y, f32 z)
{
	mAngle.x = x;
	mAngle.y = y;
	mAngle.z = z;
}

CAMERA_METHOD void CInterfaceCameraAction::SetDistance(f32 distance)
{
	mDistance = distance;
}

CAMERA_METHOD void CInterfaceCameraAction::SetFov(f32 fov)
{
	mFov = fov;
}

CAMERA_METHOD void CInterfaceCameraAction::SetAngleZ(f32 z)
{
	mAngle.z = z;
}

CAMERA_METHOD u32 CInterfaceCameraAction::vf50()
{
	return 0;
}

CAMERA_METHOD u32 CInterfaceCameraAction::GetCameraFlags()
{
	return _38;
}

#ifdef DEFINE_CAMERA_INTERFACE_METHODS
CAMERA_METHOD f32 CInterfaceCameraAction::GetFollowDivisor()
{
	return 12.0f;
}
#endif

CAMERA_METHOD u32 CInterfaceCameraAction::vf5C()
{
	return 0;
}

CAMERA_METHOD XObj* CInterfaceCameraAction::GetTarget()
{
	return NULL;
}
