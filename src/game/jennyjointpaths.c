struct JennyAttachmentConstants {
	float transforms[2][3][3];
	int releasePaths[3][6];
	int objectVisibilityPaths[2][5];
	int animationVisibilityPaths[2][7];
	int propHandPaths[5][6];
	int playerHandPath[6];
	int playerFacingPath[4];
	int alternatePlayerHandPath[6];
	int alternatePlayerFacingPath[4];
};

const struct JennyAttachmentConstants sJennyAttachmentConstants = {
	{ { { 1, 1, 1 } }, { { 1, 1, 1 } } },
	{ { 0, 3, 1 }, { 0, 3, 2 }, { 0, 3, 2 } },
	{ { 0, 3, 0, 0, 4 }, { 0, 3, 0, 0, 4 } },
	{ { 0, 3, 2, 0, 0, 1 }, { 0, 3, 2, 0, 0, 1 } },
	{ { 0, 3, 1 }, { 0, 3, 1 }, { 0, 3, 1 }, { 0, 3, 1 }, { 0, 3, 2 } },
	{ 0, 3, 2 },
	{ 0, 3 },
	{ 0, 3, 2 },
	{ 0, 3 },
};
