#include "game/scrollroomdata.h"

Scroll2Limits sScrollLimitsStage01 = { 0.0f, 0.0f, 0.0f, 0.0f, -252.42f, 235.19f, -112.42f, 138.56f, 60.0f, 15.0f, 33.0f };
Scroll2Limits sScrollLimitsStage02 = { 0.0f, 0.0f, 0.0f, 0.0f, -174.25f, 174.25f, -316.0f, 260.0f, 60.0f, 15.0f, 60.0f };
Scroll2Limits sScrollLimitsStage07 = { 0.0f, 0.0f, 0.0f, 0.0f, -214.98f, 214.98f, -293.72f, 293.72f, 60.0f, 15.0f, 60.0f };
Scroll2Limits sScrollLimitsStage03 = { 0.0f, 0.0f, 0.0f, 0.0f, -125.0f, 125.0f, -110.0f, 110.0f, 60.0f, 15.0f, 60.0f };
Scroll2Limits sScrollLimitsStage04 = { 0.0f, 0.0f, 0.0f, 0.0f, -200.0f, 200.0f, -200.0f, 200.0f, 60.0f, 15.0f, 60.0f };
Scroll2Limits sScrollLimitsStage06 = { 0.0f, 0.0f, 0.0f, 0.0f, -200.0f, 200.0f, -200.0f, 200.0f, 60.0f, 15.0f, 33.0f };
Scroll2Limits sScrollLimitsStage09 = { 0.0f, 0.0f, 0.0f, 0.0f, -130.43f, 149.38f, -268.49f, 439.07f, 60.0f, 15.0f, 60.0f };
Scroll2Limits sScrollLimitsStage11 = { 0.0f, 0.0f, 0.0f, 0.0f, -160.0f, 160.0f, -80.0f, 80.0f, 40.0f, 15.0f, 27.07f };

#if defined(VERSION_PAL)
ScrollRoomData gScrollRoomData = {
	{ NULL,
	  &sScrollLimitsStage01,
	  &sScrollLimitsStage02,
	  &sScrollLimitsStage03,
	  &sScrollLimitsStage04,
	  NULL,
	  &sScrollLimitsStage06,
	  &sScrollLimitsStage07,
	  NULL,
	  &sScrollLimitsStage09,
	  NULL,
	  &sScrollLimitsStage11,
	  NULL,
	  NULL,
	  &sScrollLimitsStage07,
	  NULL,
	  NULL,
	  NULL,
	  NULL,
	  NULL },
	{ -1, 1, 1, -1 },
	{ -1, -1, 1, 1 },
	{ -1, 1, 1, -1 },
	{ -1, -1, 1, 1 },
};
#else
Scroll2Limits* gScrollCameraLimitTables[20] = { NULL,
	                                            &sScrollLimitsStage01,
	                                            &sScrollLimitsStage02,
	                                            &sScrollLimitsStage03,
	                                            &sScrollLimitsStage04,
	                                            NULL,
	                                            &sScrollLimitsStage06,
	                                            &sScrollLimitsStage07,
	                                            NULL,
	                                            &sScrollLimitsStage09,
	                                            NULL,
	                                            &sScrollLimitsStage11,
	                                            NULL,
	                                            NULL,
	                                            &sScrollLimitsStage07,
	                                            NULL,
	                                            NULL,
	                                            NULL,
	                                            NULL,
	                                            NULL };
f32 gScrollCaptionOutlineX[4]               = { -1, 1, 1, -1 };
f32 gScrollCaptionOutlineY[4]               = { -1, -1, 1, 1 };
#endif
