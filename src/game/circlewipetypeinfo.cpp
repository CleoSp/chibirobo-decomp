class CScrnWipeCircle;
class CScrnWipe;
class CGs;

extern "C" {
extern const char sTypeNameCScrnWipeCircle[]       = "CScrnWipeCircle";
extern const char sTypeNameCScrnColor[]            = "CScrnColor";
extern const char sCircleWipeTypeNameCScrnWipe[12] = "CScrnWipe";

const void* gTypeInfoCScrnColor[]           = { sTypeNameCScrnColor, 0 };
const void* sCircleWipeBaseTypesCScrnWipe[] = { gTypeInfoCScrnColor, 0, 0 };
const void* sCircleWipeTypeInfoCScrnWipe[]  = { sCircleWipeTypeNameCScrnWipe, sCircleWipeBaseTypesCScrnWipe };
const void* sBaseTypesCScrnWipeCircle[]     = { gTypeInfoCScrnColor, 0, sCircleWipeTypeInfoCScrnWipe, 0, 0 };
const void* gTypeInfoCScrnWipeCircle[]      = { sTypeNameCScrnWipeCircle, sBaseTypesCScrnWipeCircle };

void* __dt__15CScrnWipeCircleFv(CScrnWipeCircle*, short);
void Reset__9CScrnWipeFv(CScrnWipe*);
void Update__9CScrnWipeFUl(CScrnWipe*, unsigned long);
void Draw__9CScrnWipeFP3CGsl(CScrnWipe*, CGs*, long);

struct CircleWipeVTable {
	const void* typeInfo;
	long offset;
	void* (*destroy)(CScrnWipeCircle*, short);
	void (*reset)(CScrnWipe*);
	void (*update)(CScrnWipe*, unsigned long);
	void (*draw)(CScrnWipe*, CGs*, long);
};

CircleWipeVTable __vt__15CScrnWipeCircle
    = { gTypeInfoCScrnWipeCircle, 0, __dt__15CScrnWipeCircleFv, Reset__9CScrnWipeFv, Update__9CScrnWipeFUl, Draw__9CScrnWipeFP3CGsl };
}
