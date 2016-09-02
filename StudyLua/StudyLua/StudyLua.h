/*
**	ģ��ʵ��luaԴ����
**	@author ljm
*/
#ifndef studylua_h
#define studylua_h

/*
**	������� size_t
**	typedef unsigned int size_t;
**	size_t ������¼��С���������ͣ�eg������ĳ��ȣ�����ĸ���
**	������������ʾ size_t ������ #include <stddef.h>
**
*/
/*
**	������� ptrdiff_t
**	typedef int prtdiff_t;
**	������ֳ���ص��������ͣ���������2��ָ����������Ľ��
*/
#include <crtdefs.h>

/*
**	������� NULL
**	#define NULl 0
*/
#include <stddef.h>

/*
**	������� free
**	������� realloc
*/
#include <malloc.h>

#define LUA_INTEGER long long
#define LUA_NUMBER double
#define LUA_KCONTEXT ptrdiff_t
/*
**	lua������������״̬λ
*/
#define LUA_TNONE (-1)
#define LUA_TNIL	0
#define LUA_TBOOLEAN	1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER	3
#define LUA_TSTRING 4
#define LUA_TTABLE	5
#define LUA_TUSERDATA	6
#define LUA_TUSERDATA 7
#define LUA_TTHREAD	8
#define LUA_NUMTAGS	9

#define LUA_API extern
typedef LUA_INTEGER lua_Integer;
typedef LUA_NUMBER lua_Number;
typedef LUA_KCONTEXT lua_KContext;
#define _JBLEN	16
#define _JBTYPE int
typedef _JBTYPE jmp_buf[_JBLEN];
#define luai_jmpbuf jmp_buf
#define LUA_EXTRASPACE (sizeof(void *))
#define LUA_IDSIZE	60

/*
**	λ����
*/
#define bitmask(b)	(1<<(b))
#define bit2mask(b1, b2)	(bitmask(b1) | bitmask(b2))

/*
**	GCʹ�õ�״̬λ
*/
#define WHITE0BIT	0	/* object is white (type 0) */
#define WHITE1BIT	1	/* object is white (type 1) */

#define WHITEBITS bit2mask(WHITE0BIT, WHITE1BIT)

#define luaC_white(g) cast(lu_byte, (g)->currentwhite & WHITEBITS) /* char */

/*
**	cast
*/
#define cast(t, exp) ((t)(exp))

typedef struct GCObject GCObject;
typedef struct UpVal Upval;
typedef struct lua_Debug lua_Debug;
typedef unsigned char lu_byte;
typedef struct lua_State lua_State;
typedef unsigned int Instruction;
typedef int(*lua_CFunction) (lua_State *L);
typedef (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);
typedef void * (*lua_Alloc) (void *ud, void *ptr,size_t osize, size_t nsize);
typedef void(*lua_Hook) (lua_State *L, lua_Debug *ar);
typedef int sig_atomic_t;
#define l_signalT sig_atomic_t
typedef ptrdiff_t l_mem;
typedef size_t lu_mem;
typedef enum {
	TM_INDEX, TM_NEWINDEX, TM_GC, TM_MODE, TM_LEN, TM_EQ, TM_ADD, TM_SUB,
	TM_MUL, TM_MOD, TM_POW, TM_DIV, TM_IDIV, TM_BAND, TM_BOR, TM_BXOR,
	TM_SHL, TM_SHR, TM_UNM, TM_BNOT, TM_LT, TM_LE, TM_CONCAT, TM_CALL, TM_N
} TMS;


#define CommonHeader GCObject *next;lu_byte tt;lu_byte marked

/*
**	lua�Ļ���������
*/
typedef union Value{
	GCObject *gc;
	void *p;
	int b;
	lua_CFunction f;
	lua_Integer i;
	lua_Number n;
} Value;
#define TValuefields Value value_; int tt_
typedef struct lua_TValue {
	TValuefields;
} TValue;
typedef TValue *StkId;

/*
** TString
*/
//string cache
#define STRCACHE_N 53
#define STRCACHE_M 2
typedef struct TString {
	CommonHeader;
	lu_byte extra;
	lu_byte shrlen;
	unsigned int hash;
	union {
		size_t lnglen;
		struct TString *hnext;
	} u;
} TString;
typedef struct stringtable {
	TString **hash;
	int nuse;
	int size;
} stringtable;

/*
**	Table
*/
typedef union TKey {
	struct {
		TValuefields;
		int next;
	} nk;
	TValue tvk;
} Tkey;
typedef struct Node {
	TValue i_val;
	Tkey i_key;
};
typedef struct Table {
	CommonHeader;
	lu_byte flags;
	lu_byte lsizenode;
	unsigned int sizearray;
	TValue *array;
	Node *node;
	Node *lastfree;
	struct Table *metatable;
	GCObject *gclist;
} Table;

/*
**	lua���������ջ���
*/
struct GCObject {
	CommonHeader;
};

/*
**	������Ϣ
*/
typedef struct CallInfo {
	StkId func;
	StkId top;
	struct CallInfo *previous, *next;
	union {
		struct {
			StkId base;
			const Instruction *savedpc;
		} l;
		struct {
			lua_KFunction k;
			ptrdiff_t old_errfunc;
			lua_KContext ctx;
		} c;
	} u;
	ptrdiff_t extra;
	short nresults;
	lu_byte callstatus;
} CallInfo;

/*
**	Upvalues for lua closures
*/
struct UpVal {
	TValue *v;
	lu_mem refcount;
	union {
		struct {
			UpVal *next;
			int touched;
		} open;
		TValue value;
	} u;
};

/*
**	����ת����������
**	volatile �ױ��
**	�����ڶ��̷߳���
*/
struct lua_longjmp {
	struct lua_longjmp *previous;
	luai_jmpbuf b;
	volatile int status;
};

/*
**	lua debug
*/
struct lua_Debug {
	int event;
	const char *name;
	const char *namewhat;
	const char *what;
	const char *source;
	int currentline;
	int linedefined;
	int lastlinedefined;
	unsigned char nups;
	unsigned char nparams;
	char isvararg;
	char istailcall;
	char short_src[LUA_IDSIZE];
	struct CallInfo *i_ci;
};

/*
**	thread state + extra space
*/
typedef struct LX {
	lu_byte extra_[LUA_EXTRASPACE];
	lua_State l;
} LX;
typedef struct LG {
	LX l;
	global_State g;
} LG;

/*
**	global_State
*/
typedef struct global_State {
	lua_Alloc frealloc;
	void *ud;
	l_mem totalbytes;
	l_mem GCdebt;
	lu_mem GCmemtray;
	lu_mem GCestimate;
	stringtable strt;
	TValue l_registry;
	unsigned seed;
	lu_byte currentwhite;
	lu_byte gcstate;
	lu_byte gckind;
	lu_byte gcrunning;
	GCObject *allc;
	GCObject **sweepgc;
	GCObject *finobj;
	GCObject *gray;
	GCObject *grayagain;
	GCObject *weak;
	GCObject *ephemeron;
	GCObject *allweak;
	GCObject * tobefnz;
	GCObject *fixdegc;
	struct lua_State *twups;
	unsigned int gcfinnum;
	int gcpause;
	int gcstepmul;
	lua_CFunction panic;
	struct lua_State *mainthread;
	const lua_Number *version;
	TString *memerrmsg;
	TString *tmname[TM_N];
	struct Table *mt[LUA_NUMTAGS];
	TString *[STRCACHE_N][STRCACHE_M];
} global_State;

struct lua_State {
	CommonHeader;
	unsigned short nci;
	lu_byte status;
	StkId top;
	global_State *l_G;
	CallInfo *ci;
	const Instruction *oldpc;
	StkId stack_last;
	UpVal *openupval;
	GCObject *gclist;
	struct lua_State *wtups;
	struct lua_longjump *errorJmp;
	CallInfo base_ci;
	volatile lua_Hook hook;
	ptrdiff_t errfunc;
	int stacksize;
	int basehookcount;
	int hookcount;
	unsigned short nny;
	unsigned short nCcalls;
	l_signalT hookmask;
	lu_byte allowhook;
};

/*
**	����nsize�����ڴ棬�洢��ptr��
*/
static void *l_alloc(void *ud, void *ptr, size_t oszie, size_t nsize);


//~~ LUALIB_API begin



//~~ LUALIB_API end

//~~ LUA_API begin
LUA_API lua_State *(lua_newstate)(lua_Alloc f, void *ud);

//~~ LUA_API end

#endif