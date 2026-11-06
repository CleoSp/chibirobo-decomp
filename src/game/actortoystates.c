#include "PowerPC_EABI_Support/Runtime/ptmf.h"

struct XObjActorToy;
extern void StateCarry__12XObjActorToyFUl(struct XObjActorToy* self, unsigned long frames);
extern void StateHitRecovery__12XObjActorToyFUl(struct XObjActorToy* self, unsigned long frames);
extern void StatePickUp__12XObjActorToyFUl(struct XObjActorToy* self, unsigned long frames);

PTMF gToyInitialState     = { 0, -1, { (void*)StateCarry__12XObjActorToyFUl } };
PTMF gToyHitRecoveryState = { 0, -1, { (void*)StateHitRecovery__12XObjActorToyFUl } };
PTMF gToy0FPickUpState    = { 0, -1, { (void*)StatePickUp__12XObjActorToyFUl } };
PTMF gToy156PickUpState   = { 0, -1, { (void*)StatePickUp__12XObjActorToyFUl } };
PTMF gToy159PickUpState   = { 0, -1, { (void*)StatePickUp__12XObjActorToyFUl } };
PTMF gToy25EPickUpState   = { 0, -1, { (void*)StatePickUp__12XObjActorToyFUl } };
PTMF gToyPostHitState     = { 0, -1, { (void*)StateCarry__12XObjActorToyFUl } };
PTMF gToyPostPickUpState  = { 0, -1, { (void*)StateCarry__12XObjActorToyFUl } };
