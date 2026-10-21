#ifndef _GAME_EVT_H
#define _GAME_EVT_H

// event script bytecode, the stuff in qp.bin's bin/stageNN.bin that CEvt runs

// command opcodes. the spellings are the game's own, evtCmdTable pairs each opcode
// with its name string, so EVT_CMD_MOVIE really is 49 and sits between MOVETO and MSG
enum EvtCmd {
	EVT_CMD_ABORT  = 1,
	EVT_CMD_RETURN = 2,
	EVT_CMD_GOTO   = 3,
	EVT_CMD_SET    = 4,
	EVT_CMD_IF     = 5,
	EVT_CMD_ELIF   = 6,
	EVT_CMD_ENDIF  = 7,
	EVT_CMD_CASE   = 8,
	EVT_CMD_EXPR   = 9,
	EVT_CMD_WHILE  = 10,
	EVT_CMD_BREAK  = 11,
	EVT_CMD_RUN    = 12,
	EVT_CMD_LIB    = 13,
	EVT_CMD_PUSHBP = 14,
	EVT_CMD_POPBP  = 15,
	EVT_CMD_SETSP  = 16,
	EVT_CMD_ANIM   = 17,
	EVT_CMD_ANIM1  = 18,
	EVT_CMD_ANIM2  = 19,
	EVT_CMD_ATTACH = 20,
	EVT_CMD_BORN   = 21,
	EVT_CMD_CALL   = 22,
	EVT_CMD_CAMERA = 23,
	EVT_CMD_CHECK  = 24,
	EVT_CMD_COLOR  = 25,
	EVT_CMD_DETACH = 26,
	EVT_CMD_DIR    = 27,
	EVT_CMD_MDIR   = 28,
	EVT_CMD_DISP   = 29,
	EVT_CMD_KILL   = 30,
	EVT_CMD_LIGHT  = 31,
	EVT_CMD_MENU   = 32,
	EVT_CMD_MOVE   = 33,
	EVT_CMD_MOVETO = 34,
	EVT_CMD_MSG    = 35,
	EVT_CMD_POS    = 36,
	EVT_CMD_PRINTF = 37,
	EVT_CMD_PTCL   = 38,
	EVT_CMD_READ   = 39,
	EVT_CMD_SCALE  = 40,
	EVT_CMD_MSCALE = 41,
	EVT_CMD_SCRN   = 42,
	EVT_CMD_SELECT = 43,
	EVT_CMD_SFX    = 44,
	EVT_CMD_TIMER  = 45,
	EVT_CMD_WAIT   = 46,
	EVT_CMD_WARP   = 47,
	EVT_CMD_WIN    = 48,
	EVT_CMD_MOVIE  = 49,
};

// expression ops that CEvt_Evaluate, EvalSet and EvalCmp switch on. names follow unplug,
// the game's panic text only spells a few of them, flg, var, pad, battery, item, atc,
// map and time
enum EvtExpr {
	EVT_EXPR_EQUAL         = 0,
	EVT_EXPR_NOT_EQUAL     = 1,
	EVT_EXPR_LESS          = 2,
	EVT_EXPR_LESS_EQUAL    = 3,
	EVT_EXPR_GREATER       = 4,
	EVT_EXPR_GREATER_EQUAL = 5,
	EVT_EXPR_NOT           = 6,
	EVT_EXPR_ADD           = 7,
	EVT_EXPR_SUB           = 8,
	EVT_EXPR_MUL           = 9,
	EVT_EXPR_DIV           = 10,
	EVT_EXPR_MOD           = 11,
	EVT_EXPR_AND           = 12,
	EVT_EXPR_OR            = 13,
	EVT_EXPR_XOR           = 14,
	EVT_EXPR_ADD_ASSIGN    = 15,
	EVT_EXPR_SUB_ASSIGN    = 16,
	EVT_EXPR_MUL_ASSIGN    = 17,
	EVT_EXPR_DIV_ASSIGN    = 18,
	EVT_EXPR_MOD_ASSIGN    = 19,
	EVT_EXPR_AND_ASSIGN    = 20,
	EVT_EXPR_OR_ASSIGN     = 21,
	EVT_EXPR_XOR_ASSIGN    = 22,
	EVT_EXPR_IMM16         = 23,
	EVT_EXPR_IMM32         = 24,
	EVT_EXPR_ADDRESS_OF    = 25,
	EVT_EXPR_STACK         = 26,
	EVT_EXPR_PARENT_STACK  = 27,
	EVT_EXPR_FLAG          = 28,
	EVT_EXPR_VAR           = 29,
	EVT_EXPR_RESULT1       = 30,
	EVT_EXPR_RESULT2       = 31,
	EVT_EXPR_PAD           = 32,

	EVT_EXPR_BATTERY      = 100,
	EVT_EXPR_MONEY        = 101,
	EVT_EXPR_ITEM         = 102,
	EVT_EXPR_ATC          = 103,
	EVT_EXPR_RANK         = 104,
	EVT_EXPR_EXP          = 105,
	EVT_EXPR_LEVEL        = 106,
	EVT_EXPR_HOLD         = 107,
	EVT_EXPR_MAP          = 108,
	EVT_EXPR_ACTOR_NAME   = 109,
	EVT_EXPR_ITEM_NAME    = 110,
	EVT_EXPR_TIME         = 111,
	EVT_EXPR_CURRENT_SUIT = 112,
	EVT_EXPR_SCRAP        = 113,
	EVT_EXPR_CURRENT_ATC  = 114,
	EVT_EXPR_USE          = 115,
	EVT_EXPR_HIT          = 116,
	EVT_EXPR_STICKER_NAME = 117,

	EVT_EXPR_OBJ           = 200,
	EVT_EXPR_RANDOM        = 201,
	EVT_EXPR_SIN           = 202,
	EVT_EXPR_COS           = 203,
	EVT_EXPR_ARRAY_ELEMENT = 204,
};

// atoms, the @WORD sub keywords commands take, like wait(@ANIM, ...). the ones the
// game's debug text spells out use that spelling, the rest follow unplug. values
// nobody has figured out yet are left out on purpose
enum EvtAtom {
	EVT_ATOM_TIME      = 200,
	EVT_ATOM_FADE      = 201,
	EVT_ATOM_WIPE      = 202,
	EVT_ATOM_ANIM      = 204,
	EVT_ATOM_DIR       = 205,
	EVT_ATOM_MOVE      = 206,
	EVT_ATOM_POS       = 207,
	EVT_ATOM_OBJ       = 208,
	EVT_ATOM_RESET     = 209,
	EVT_ATOM_POSX      = 212,
	EVT_ATOM_POSY      = 213,
	EVT_ATOM_POSZ      = 214,
	EVT_ATOM_BONEX     = 215,
	EVT_ATOM_BONEY     = 216,
	EVT_ATOM_BONEZ     = 217,
	EVT_ATOM_DIRTO     = 218,
	EVT_ATOM_COLOR     = 219,
	EVT_ATOM_LEAD      = 220,
	EVT_ATOM_SFX       = 221,
	EVT_ATOM_MODULATE  = 222,
	EVT_ATOM_BLEND     = 223,
	EVT_ATOM_REAL      = 224,
	EVT_ATOM_CAM       = 225,
	EVT_ATOM_HUD       = 226,
	EVT_ATOM_DISTANCE  = 228,
	EVT_ATOM_READ      = 233,
	EVT_ATOM_ZBLUR     = 234,
	EVT_ATOM_LETTERBOX = 239,
	EVT_ATOM_SHAKE     = 241,
	EVT_ATOM_MONO      = 242,
	EVT_ATOM_SCALE     = 244,
	EVT_ATOM_CUE       = 245,

	// unplug has these as UnkNNN and the game never spells them, so the names are
	// ours, picked from what each case calls
	EVT_ATOM_KILL     = 210, // ptcl, CParticle::Kill
	EVT_ATOM_AREA     = 211, // camera, switches to the area camera
	EVT_ATOM_INTEREST = 227, // camera and light, SetInterest
	EVT_ATOM_FOV      = 229, // camera, SetFov_Interp
	EVT_ATOM_SAVE     = 236, // camera, copies the camera state into a slot
	EVT_ATOM_LOAD     = 237, // camera, puts a saved state back
	EVT_ATOM_MOVIE    = 246, // wait and check, until the movie gets to a frame
};

// sfx(sound, mode) modes, sound -2 means every sound. EVT_ATOM_CUE as the mode starts a
// music stream with the beat callback. the msg sfx code runs the same switch
enum EvtSfxMode {
	EVT_SFX_STOP     = 0,
	EVT_SFX_PLAY     = 1,
	EVT_SFX_FADE_OUT = 2, // frames, doubled here but not in msg text
	EVT_SFX_FADE_IN  = 3,
	EVT_SFX_FADE     = 4, // frames, volume
	EVT_SFX_PAUSE    = 5, // ends up in AXDriverPause
	EVT_SFX_RESUME   = 6, // ends up in AXDriverResume
};

// scrn(@HUD, part, ...) parts, 3 reads four args and throws them away
enum EvtHud {
	EVT_HUD_BATTERY = 0,
	EVT_HUD_CLOCK   = 1,
	EVT_HUD_SCORE   = 2,
	EVT_HUD_TIMER   = 4,
};

// call() targets below zero are syscalls instead of objects. these four are the game's
// own spellings from the debug text, each string only shows up inside its own case.
// the rest of -10 to -45 and -200 are syscalls too, just unnamed
enum EvtSyscall {
	SYSCALL_SET_OBJSTAT = -26,  // obj, bit
	SYSCALL_CLR_OBJSTAT = -27,  // obj, bit
	SYSCALL_CHK_OBJSTAT = -28,  // obj, bit
	SYSCALL_STGFUNC     = -100, // first arg plus 7 picks the stage rel function
};

#endif
