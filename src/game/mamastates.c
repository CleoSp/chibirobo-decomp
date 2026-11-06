#include "PowerPC_EABI_Support/Runtime/ptmf.h"

struct XObjActorMama;
extern void StateNone__13XObjActorMamaFUl(struct XObjActorMama* self, unsigned long frames);

PTMF sMamaIdleState = { 0, -1, { (void*)StateNone__13XObjActorMamaFUl } };

struct MamaAttachmentConstants {
	float transforms[2][3][3];
	int releasePlayerPath[6];
	int particleOriginPath[5];
	float particleScale[3];
	float particleRotation[3];
	int propHandPaths[2][6];
	int heldPlayerPath[6];
	int playerFacingPath[5];
};

const struct MamaAttachmentConstants sMamaAttachmentConstants = {
	{ { { 1, 1, 1 } }, { { 1, 1, 1 } } }, { 0, 4, 2 }, { 0, 4 }, { 1, 1, 1 }, { 0, 0, 0 },
	{ { 0, 4, 2 }, { 0, 4, 1 } },         { 0, 4, 2 }, { 0, 4 },
};
