#ifndef _GAME_PARTICLE_SRT_H
#define _GAME_PARTICLE_SRT_H

// the HSD headers have to come first, see game/hsd_jobj.h
#include "game/hsd_jobj.h"
#include "game/cparticle.h"

// the game's particle helpers: a generator from its bank and id, with the default scale,
// rotation and position for what it isn't given. MWCC makes the constants of an inline
// function's local initializers when it reads the body, called or not, so these six Vecs open
// the .rodata of every game file that includes this, ahead of the file's own constants.
// objb_act's fn_800D3A5C and fn_800D3BA4 read the last three from there

inline HSD_Generator* CreateParticle(CParticle& ptcl, s32 bank, s32 id, s32 status, BOOL keep)
{
	Vec scale = { 1.0f, 1.0f, 1.0f };
	Vec rot   = { 0.0f, 0.0f, 0.0f };
	Vec pos   = { 0.0f, 0.0f, 0.0f };
	return ptcl.CreateSRT(id | (bank << 16), status, pos, scale, rot, keep);
}

inline Vec ParticleUnitScale()
{
	Vec scale = { 1.0f, 1.0f, 1.0f };
	return scale;
}

inline HSD_Generator* CreateParticle(CParticle& ptcl, s32 bank, s32 id, s32 status, const Vec& pos, BOOL keep)
{
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return ptcl.CreateSRT(id | (bank << 16), status, pos, ParticleUnitScale(), rot, keep);
}

inline HSD_Generator* CreateParticle(CParticle& ptcl, s32 bank, s32 id, s32 status, const Vec& pos, const Vec& scale, BOOL keep)
{
	Vec rot = { 0.0f, 0.0f, 0.0f };
	return ptcl.CreateSRT(id | (bank << 16), status, pos, scale, rot, keep);
}

#endif
