#include <global.h>

#include <string.h>

#include <dolphin/os.h>
#include <sysdolphin/jobj.h>
#include <sysdolphin/object.h>
#include <sysdolphin/particle.h>
#include <sysdolphin/particle_internal.h>
#include <sysdolphin/psinterpret.h>
#include <sysdolphin/pslist.h>

void psClearPointJObj(void);

const u32* ptclref[PS_BANK_COUNT];
s32 psBankTextureCounts[PS_BANK_COUNT];
HSD_PSTexGroup** psTexGroupArray[PS_BANK_COUNT];
HSD_PSFormGroup** psFormGroupArray[PS_BANK_COUNT];
s32 psNumCmdList[PS_BANK_COUNT];
HSD_PSCmdList** psCmdListArray[PS_BANK_COUNT];
HSD_JObj* psPointJObj[8];

u16 psParticleCount;
u16 psGeneratorCount;
u16 psAppSRTCount;
u16 psPeakParticleCount;
u16 psPeakGeneratorCount;
u16 psPeakAppSRTCount;
PSParticleCallback* psParticleCallbackTable;

/**
 * @note Address: 0x801DD33C
 * @note Size: 0x140
 */
void psInitDataBankLoad(s32 bank, s32* cmdBank, s32* texBank, void* ref, s32* formBank)
{
	if (formBank != NULL && *formBank != *texBank) {
		OSPanic("particle.c", 95, "illigal form data (strange number of group)\n");
	}

	ptclref[bank]             = ref;
	psBankTextureCounts[bank] = *texBank;
	psTexGroupArray[bank]     = (HSD_PSTexGroup**)(texBank + 1);
	if (formBank != NULL) {
		psFormGroupArray[bank] = (HSD_PSFormGroup**)(formBank + 1);
	} else {
		psFormGroupArray[bank] = NULL;
	}

	switch (*(u16*)cmdBank) {
	case 0:
		psNumCmdList[bank]   = cmdBank[1];
		psCmdListArray[bank] = (HSD_PSCmdList**)(cmdBank + 2);
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43: {
		s32 first            = cmdBank[1];
		psNumCmdList[bank]   = cmdBank[2] + first;
		psCmdListArray[bank] = (HSD_PSCmdList**)(cmdBank + 3 - first);
		break;
	}
	default:
		OSPanic("particle.c", 125, "psInitDataBanks: unknown version\n");
	}
}

/**
 * @note Address: 0x801DD47C
 * @note Size: 0x374
 */
void psInitDataBankLocate(s32* cmdBank, s32* texBank, s32* formBank)
{
	s32 i;
	s32 j;
	s32 first;
	s32 last;
	s32 numGroups;
	HSD_PSCmdList** cmdList;
	HSD_PSTexGroup** texGroups;

	switch (*(u16*)cmdBank) {
	case 0:
		last    = cmdBank[1];
		cmdList = (HSD_PSCmdList**)(cmdBank + 2);
		first   = 0;
		for (i = 0; i < last; i++) {
			cmdBank[i + 2] += (s32)cmdBank;
		}
		break;
	case 0x40:
	case 0x41:
	case 0x42:
	case 0x43:
		first   = cmdBank[1];
		last    = cmdBank[2] + first;
		cmdList = (HSD_PSCmdList**)(cmdBank + 3 - first);
		for (i = 0; i < cmdBank[2]; i++) {
			if (cmdBank[i + 3] != 0) {
				cmdBank[i + 3] += (s32)cmdBank;
			}
		}
		break;
	}

	for (i = first; i < last; i++) {
		if (cmdList[i] != NULL) {
			cmdList[i]->kind &= ~PS_PARTICLE_SORT_ORDER_MASK;
			cmdList[i]->kind |= (PS_PARTICLE_SORT_ORDER_COUNT / 2) << PS_PARTICLE_SORT_ORDER_SHIFT;
		}
	}

	numGroups = texBank[0];
	texGroups = (HSD_PSTexGroup**)(texBank + 1);
	for (i = 1; i <= numGroups; i++) {
		if (texBank[i] != 0) {
			texBank[i] += (s32)texBank;
		}
	}

	for (i = 0; i < numGroups; i++) {
		if (texGroups[i] == NULL) {
			continue;
		}
		for (j = 0; j < texGroups[i]->num; j++) {
			if (texGroups[i]->texTable[j] != NULL) {
				texGroups[i]->texTable[j] += (u32)texBank;
			}
		}
		if (texGroups[i]->fmt != GX_TF_C4 && texGroups[i]->fmt != GX_TF_C8 && texGroups[i]->fmt != GX_TF_C14X2) {
			continue;
		}
		if (texGroups[i]->palflag & 1) {
			j = texGroups[i]->num;
			if (texGroups[i]->texTable[j] != NULL) {
				texGroups[i]->texTable[j] += (u32)texBank;
			}
		} else if (texGroups[i]->palnum != 0) {
			for (j = texGroups[i]->num; j < texGroups[i]->num + texGroups[i]->palnum; j++) {
				if (texGroups[i]->texTable[j] != NULL) {
					texGroups[i]->texTable[j] += (u32)texBank;
				}
			}
		} else {
			for (j = texGroups[i]->num; j < texGroups[i]->num * 2; j++) {
				if (texGroups[i]->texTable[j] != NULL) {
					texGroups[i]->texTable[j] += (u32)texBank;
				}
			}
		}
	}

	if (formBank == NULL) {
		return;
	}
	for (i = 1; i <= numGroups; i++) {
		if (formBank[i] != 0) {
			HSD_PSFormGroup* fg;

			formBank[i] += (s32)formBank;
			fg = (HSD_PSFormGroup*)formBank[i];
			for (j = 0; j < fg->num; j++) {
				if (fg->formTable[j] != NULL) {
					fg->formTable[j] = (u32*)((u8*)fg->formTable[j] + (u32)formBank);
				}
			}
		}
	}
}

/**
 * @note Address: 0x801DD7F0
 * @note Size: 0x15C
 */
void psInitDataBank(s32 bank, s32* cmdBank, s32* texBank, void* ref, s32* formBank)
{
	if (bank < PS_BANK_COUNT) {
		psInitDataBankLocate(cmdBank, texBank, formBank);
		psInitDataBankLoad(bank, cmdBank, texBank, ref, formBank);
	}
}

HSD_PSCmdList* psGetBankCommand(s32 bank, s32 id)
{
	if (bank < 0 || bank >= PS_BANK_COUNT || id >= psNumCmdList[bank]) {
		return NULL;
	}
	return psCmdListArray[bank][id];
}

/**
 * @note Address: 0x801DD94C
 * @note Size: 0x164
 */
void psInitParticle(s32 count)
{
	int i;

	_psLinkInit(count);
	psParticleCount     = 0;
	psPeakParticleCount = 0;
	for (i = 0; i < PS_BANK_COUNT; i++) {
		psNumCmdList[i]        = 0;
		psBankTextureCounts[i] = 0;
		psCmdListArray[i]      = NULL;
		psTexGroupArray[i]     = NULL;
		psFormGroupArray[i]    = NULL;
		ptclref[i]             = NULL;
	}
	psParticleCallbackTable = NULL;
	for (i = 0; i < 8; i++) {
		psPointJObj[i] = NULL;
	}
}

/**
 * @note Address: 0x801DDAB0
 * @note Size: 0x13C
 */
void psRemoveParticle(void)
{
	int i;

	psKillAllParticle();
	psKillAllGenerator();
	_psListClear();
	for (i = 0; i < PS_BANK_COUNT; i++) {
		psNumCmdList[i]        = 0;
		psBankTextureCounts[i] = 0;
		psCmdListArray[i]      = NULL;
		psTexGroupArray[i]     = NULL;
		psFormGroupArray[i]    = NULL;
		ptclref[i]             = NULL;
	}
	psClearPointJObj();
}

/**
 * @note Address: N/A
 * @note Size: 0x2FC
 */
HSD_Particle* psGenerateParticle0(HSD_Particle* parent, int linkNo, int bank, u64 kind, int texGroup,
                                  u8* list, f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 size, f32 grav, int life, f32 fric,
                                  int palflag, HSD_Generator* gp, int flgInterpret)
{
	HSD_Particle* pp;

	pp = _psListNew(parent, linkNo);
	if (pp == NULL) {
		return NULL;
	}

	if (gp != NULL) {
		pp->idnum = gp->idnum;
	} else {
		pp->idnum = psGetNewIDNum();
	}

	pp->appsrt = NULL;
	if (gp != NULL && gp->appsrt != NULL) {
		psAttachParticleAppSRT(pp, gp->appsrt);
	}

	pp->bank       = (u8)bank;
	pp->linkNo     = (u8)linkNo;
	pp->kind       = kind;
	pp->texGroup   = (u8)texGroup;
	pp->pos.x      = x;
	pp->pos.y      = y;
	pp->pos.z      = z;
	pp->vel.x      = vx;
	pp->vel.y      = vy;
	pp->vel.z      = vz;
	pp->size       = size;
	pp->grav       = grav;
	pp->fric       = fric;
	pp->life       = (u16)(life + 1);
	pp->cmdList    = list;
	pp->cmdMarkPtr = 0;
	pp->cmdPtr     = 0;

	if (palflag != 0) {
		pp->kind |= PS_PARTICLE_SHARED_PALETTE;
	}

	pp->cmdWait       = (u16)(list != NULL);
	pp->poseNum       = 0;
	pp->palNum        = PS_PARTICLE_PALETTE_DEFAULT;
	pp->primCol.a     = 0xFF;
	pp->primCol.b     = 0xFF;
	pp->primCol.g     = 0xFF;
	pp->primCol.r     = 0xFF;
	pp->envCol.a      = 0;
	pp->envCol.b      = 0;
	pp->envCol.g      = 0;
	pp->envCol.r      = 0;
	pp->envColCount   = 0;
	pp->primColCount  = 0;
	pp->sizeCount     = 0;
	pp->envColRemain  = 0;
	pp->primColRemain = 0;
	pp->aCmpMode      = 0x33;
	if (((pp->kind >> PS_PARTICLE_BLEND_SHIFT) & PS_PARTICLE_BLEND_MODE_MASK) >= PS_PARTICLE_BLEND_SUBTRACT) {
		pp->aCmpParam1 = 0;
	} else {
		pp->aCmpParam1 = 1;
	}
	pp->aCmpParam2   = 0xFF;
	pp->aCmpRemain   = 0;
	pp->aCmpCount    = 0;
	pp->rotateCount  = 0;
	pp->spreadRate   = 0.0F;
	pp->rotateTarget = 0.0F;
	pp->rotate       = 0.0F;
	pp->gen          = gp;
	if (gp != NULL) {
		gp->numChild++;
	}
	pp->pJObjOfs     = 0;
	pp->matColRemain = 0;
	pp->matColCount  = 0;
	pp->matRGB       = 0xFF;
	pp->matA         = 0xFF;
	pp->ambColRemain = 0;
	pp->ambColCount  = 0;
	pp->ambRGB       = 0xFF;
	pp->ambA         = 0xFF;
	pp->trail        = 1.0F;
	pp->userdata     = NULL;

	if (gp != NULL && gp->userfunc != NULL && gp->userfunc->hookCreate != NULL) {
		gp->userfunc->hookCreate(pp);
	}

	pp->callback = NULL;
	if (flgInterpret != 0) {
		psInterpretParticle0(pp, NULL);
	}
	return pp;
}

/**
 * @note Address: 0x801DDEE4
 * @note Size: 0xF8
 */
HSD_Particle* psGenerateParticleID0(HSD_Particle* head, int linkNo, int bank, int id, int flgInterpret)
{
	HSD_PSCmdList* cl;
	HSD_PSTexGroup* tg;
	s32 palflag;

	if (linkNo >= 8) {
		return NULL;
	}
	if (bank >= PS_BANK_COUNT) {
		return NULL;
	}
	if (id >= psNumCmdList[bank]) {
		return NULL;
	}

	cl = psCmdListArray[bank][id];
	if (cl == NULL) {
		return NULL;
	}

	tg = psTexGroupArray[bank][cl->texGroup];
	if (tg != NULL) {
		palflag = tg->palflag;
	} else {
		palflag = 0;
	}

	return psGenerateParticle0(head, linkNo, bank, cl->kind, cl->texGroup, cl->cmdList, 0.0F, 0.0F, 0.0F, cl->vx, cl->vy,
	                           cl->vz, cl->size, cl->grav, cl->life, cl->fric, palflag, NULL, flgInterpret);
}

/**
 * @note Address: N/A
 * @note Size: 0x64
 */
HSD_Particle* psGenerateParticle(int linkNo, int bank, u64 kind, int texGroup, u8* list, int life, f32 x, f32 y, f32 z,
                                 f32 vx, f32 vy, f32 vz, f32 size, f32 grav, f32 fric, int palflag, HSD_Generator* gp)
{
	return psGenerateParticle0(NULL, linkNo, bank, kind, texGroup, list, x, y, z, vx, vy, vz, size, grav, life, fric,
	                           palflag, gp, 1);
}

/**
 * @note Address: 0x801DE040
 * @note Size: 0xFC
 */
void psKillAllParticle(void)
{
	HSD_Particle* node;
	HSD_Particle* prev;
	HSD_Particle* next;
	HSD_Particle* cur;
	int i;

	for (i = 0; i < 16; i++) {
		cur = _psListGetFirst(i);
		while (cur != NULL) {
			next = cur->next;
			prev = NULL;
			node = _psListGetFirst(cur->linkNo);
			while (node != NULL) {
				if (node == cur) {
					if (cur->gen != NULL) {
						cur->gen->numChild--;
					}
					if (cur->appsrt != NULL) {
						psRemoveParticleAppSRT(cur);
					}
					psDeletePntJObjwithParticle(node);
					_psListDelete(node, prev);
					if (cur->gen != NULL && cur->gen->userfunc != NULL && cur->gen->userfunc->hookDelete != NULL) {
						cur->gen->userfunc->hookDelete(cur);
					}
					break;
				}
				prev = node;
				node = node->next;
			}
			cur = next;
		}
	}
}

/**
 * @note Address: 0x801DE13C
 * @note Size: 0xF8
 */
void psSetPointJObj(s32 no, HSD_JObj* jobj)
{
	HSD_JObj* old;

	if (no < 0 || no > 8) {
		return;
	}

	if (no != 0) {
		old = (HSD_JObj*)psPointJObj[no - 1];
		if (old == jobj) {
			return;
		}
		if (old != NULL) {
			HSD_JObjUnref(old);
		}
		psPointJObj[no - 1] = jobj;
		ref_INC(jobj);
	} else {
		int i;
		for (i = 0; i < 8; i++) {
			if ((HSD_JObj*)psPointJObj[i] == jobj) {
				HSD_JObjUnref((HSD_JObj*)psPointJObj[i]);
				psPointJObj[i] = NULL;
			}
		}
	}
}

/**
 * @note Address: 0x801DE234
 * @note Size: 0xDC
 */
void psSetPointJObjNodup(HSD_JObj* jobj, s32 no)
{
	int i;
	HSD_JObj* old;

	if (no < 0 || no > 8) {
		return;
	}

	for (i = 0; i < 8; i++) {
		if ((HSD_JObj*)psPointJObj[i] == jobj) {
			HSD_JObjUnref((HSD_JObj*)psPointJObj[i]);
			psPointJObj[i] = NULL;
		}
	}

	if (no != 0) {
		old = (HSD_JObj*)psPointJObj[no - 1];
		if (old != NULL) {
			HSD_JObjUnref(old);
		}
		psPointJObj[no - 1] = jobj;
		ref_INC(jobj);
	}
}

/**
 * @note Address: N/A
 * @note Size: 0x68
 */
void psClearPointJObj(void)
{
	int i;
	for (i = 0; i < 8; i++) {
		if (psPointJObj[i] != NULL) {
			HSD_JObjUnref((HSD_JObj*)psPointJObj[i]);
			psPointJObj[i] = NULL;
		}
	}
}

/**
 * @note Address: 0x801DE378
 * @note Size: 0x5C
 */
void psDeletePntJObjwithParticle(HSD_Particle* pp)
{
	HSD_JObj* jobj;
	u32 idx;

	if (pp->kind & 0x8000) {
		idx  = (pp->kind >> 10) & 0x1c;
		jobj = *(HSD_JObj**)((u8*)psPointJObj + idx);
		if (jobj != NULL) {
			HSD_JObjUnref(jobj);
			*(HSD_JObj**)((u8*)psPointJObj + idx) = NULL;
		}
	}
}

static inline HSD_Particle* _psListGetNext(HSD_Particle* pp)
{
	return pp->next;
}

/**
 * @note Address: 0x801DE3D4
 * @note Size: 0x114
 */
void psKillGeneratorChild(HSD_Generator* gp)
{
	HSD_Particle* prev = NULL;
	HSD_Particle* pp;
	u16 idnum = gp->idnum;

	pp = _psListGetFirst(gp->linkNo);
	while (pp != NULL) {
		HSD_Particle* next = _psListGetNext(pp);
		if (pp->idnum == idnum && pp->gen != NULL && pp->gen == gp) {
			if (pp->gen != NULL) {
				pp->gen->numChild--;
			}
			if (pp->appsrt != NULL) {
				psRemoveParticleAppSRT(pp);
			}
			psDeletePntJObjwithParticle(pp);
			_psListDelete(pp, prev);
			if (pp->gen != NULL && pp->gen->userfunc != NULL && pp->gen->userfunc->hookDelete != NULL) {
				pp->gen->userfunc->hookDelete(pp);
			}
		} else {
			prev = pp;
		}
		pp = next;
	}
}

/**
 * @note Address: 0x801DE4E8
 * @note Size: 0x8
 */
void psSetUserFunc(HSD_Generator* gp, HSD_PSUserFunc* userfunc)
{
	gp->userfunc = userfunc;
}
