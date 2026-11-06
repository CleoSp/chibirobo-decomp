const char sTypeNameCCameraActionMap[20]          = "CCameraActionMap";
const char sMapTypeNameCInterfaceCameraAction[24] = "CInterfaceCameraAction";
const char sMapTypeNameCCameraActionAreaExM[24]   = "CCameraActionAreaExM";
const char sPathCcameraactionmap2Cpp[24]          = "ccameraactionmap2.cpp";

const char sMapCameraStateFormat[244]
    = "EYE(%f, %f, %f)ANG(%f, %f, %f)DIST(%f)CAMSTAT(%8X)CAMSTATPRE(%8X)UNDER OP(%d)HANG(%d(%d))TGTEYE(%f, %f, %f)TGTEYE_PRE(%f, %f, "
      "%f)INTERP CAM(0x%x)INTERP TURN(0x%x)POS_CAMMODE(%f, %f, %f)ANG_CAMMODE(%f, %f, %f)ANG_CAMMODE_PRE(%f, %f, %f)FIX(%d)\n";
#if !defined(VERSION_GGTP01)
const char sMapCameraDrawDebug3dFormat[44] = "DEBUG:CCameraActionMap::DrawDebug3d():%d\n";
#endif
const char sMapCameraAreaFormat[32]            = "\\B000000A0AREA(%3d,---)(%3d)\n";
const char sMapCameraOverlayTitle[20]          = "\\B000000A0ACMAP2\n";
const char sMapCameraFlagsFormat[44]           = "\\B00000080CAMSTAT\t   :%8X\nCAMSTATPRE :%8X\n";
const char sMapCameraPlayerCollisionFormat[40] = "\\B00000080CHR_InsideCll(%3d,%3d,%3d)\n";
const char sMapCameraEyeCollisionFormat[40]    = "\\B00000080CAM_InsideCll(%3d,%3d,%3d)\n";
const char sMapCameraHitCollisionFormat[36]    = "\\B00000080CamHitCll(%3d,%3d,%3d)\n";
const char sMapCameraIntersectionFormat[28]    = "\\B00000080INTRSCT(%3d-%3d)\n";
const char sMapCameraPlaneIntersectionFormat[108]
    = "CCameraActionMap ... GetIntersectionToPlane\np1(%f, %f, %f) vn(%f, %f, %f)\nvs(%f, %f, %f) ve(%f, %f, %f)\n";
const char sMapCameraCrossPointFormat[100]
    = "CCameraActionMap ... GetCrossPoint\np1(%f, %f, %f) p2(%f, %f, %f)\nvs(%f, %f, %f) ve(%f, %f, %f)\n(1)\n";
const char sMapCameraResetMessage[20]                          = "actionMap::reset\n";
const char sMapCameraDisplacementFormat[40]                    = "CCameraActionMap ... disp(%f, %f, %f)\n";
const char sMapCameraSpringStateFormat[52]                     = "CCameraActionMap ... (%f, %f, %f), %f, %f, %f, %s\n";
const char sMapCameraTypeNameCamAreaInfo[12]                   = "CamAreaInfo";
const char sMapCameraTypeNameCVectorEaseInOutInterpolation[32] = "CVectorEaseInOutInterpolation";
const char sMapCameraTypeNameCInterfaceVectorInterpolation[32] = "CInterfaceVectorInterpolation";
const char sMapCameraTypeNameCInterpolationEaseOutDivide[28]   = "CInterpolationEaseOutDivide";
const char sMapCameraTypeNameCInterfaceInterpolation[24]       = "CInterfaceInterpolation";
#if defined(VERSION_GGTP01)
const char sMapCameraTypeNameCVectorEaseOutDivideInterpolation[36] = "CVectorEaseOutDivideInterpolation";
#else
const char sMapCameraTypeNameCVectorEaseOutDivideInterpolation[36] = "CVectorEaseOutDivideInterpolation"
                                                                     "\0"
                                                                     "00";
#endif
