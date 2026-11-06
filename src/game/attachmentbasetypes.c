extern const void* sAttachmentTypeInfoCJObj[];
extern const void* sAttachmentTypeInfoCXObj[];
extern const void* sAttachmentTypeInfoCXObjIntp[];
extern const void* sAttachmentTypeInfoXObj[];
extern const void* gTypeInfoXObjAttachment[];

const void* sAttachmentBaseTypesCXObj[]     = { sAttachmentTypeInfoCJObj, 0, 0 };
const void* sAttachmentBaseTypesCXObjIntp[] = { sAttachmentTypeInfoCJObj, 0, sAttachmentTypeInfoCXObj, 0, 0 };
const void* sAttachmentBaseTypesXObj[] = { sAttachmentTypeInfoCJObj, 0, sAttachmentTypeInfoCXObj, 0, sAttachmentTypeInfoCXObjIntp, 0, 0 };
const void* sBaseTypesXObjAttachment[]
    = { sAttachmentTypeInfoCJObj, 0, sAttachmentTypeInfoCXObj, 0, sAttachmentTypeInfoCXObjIntp, 0, sAttachmentTypeInfoXObj, 0, 0 };
const void* sBaseTypesXObjOX[] = { sAttachmentTypeInfoCJObj,
	                               0,
	                               sAttachmentTypeInfoCXObj,
	                               0,
	                               sAttachmentTypeInfoCXObjIntp,
	                               0,
	                               sAttachmentTypeInfoXObj,
	                               0,
	                               gTypeInfoXObjAttachment,
	                               0,
	                               0 };
