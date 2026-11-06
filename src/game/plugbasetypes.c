extern const void* sPlugTypeInfoCJObj[];
extern const void* sPlugTypeInfoCXObj[];
extern const void* sPlugTypeInfoCXObjIntp[];
extern const void* sPlugTypeInfoXObj[];

const void* sPlugBaseTypesCXObj[]     = { sPlugTypeInfoCJObj, 0, 0 };
const void* sPlugBaseTypesCXObjIntp[] = { sPlugTypeInfoCJObj, 0, sPlugTypeInfoCXObj, 0, 0 };
const void* sPlugBaseTypesXObj[]      = { sPlugTypeInfoCJObj, 0, sPlugTypeInfoCXObj, 0, sPlugTypeInfoCXObjIntp, 0, 0 };
const void* sBaseTypesXObjWireEff[] = { sPlugTypeInfoCJObj, 0, sPlugTypeInfoCXObj, 0, sPlugTypeInfoCXObjIntp, 0, sPlugTypeInfoXObj, 0, 0 };
