#include <iostream>
using std::cout;
using std::endl;

/*
**	模拟实现lua源代码
**	@author ljm
*/
#ifndef studylua_h
#define studylua_h

/*
**	引入对象 size_t
**	typedef unsigned int size_t;
**	size_t 用来记录大小的数据类型，eg，数组的长度，对象的个数
**	当网上资料显示 size_t 定义在 #include <stddef.h>
**
*/
/*
**	引入对象 ptrdiff_t
**	typedef int prtdiff_t;
**	与机器字长相关的数据类型，用来保存2个指针减法操作的结果
*/
#include <crtdefs.h>

/*
**	引入对象 NULL
**	#define NULl 0
*/
#include <stddef.h>

/*
**	引入对象 free
**	引入对象 realloc
*/
#include <malloc.h>

/*
**	引入对象memcpy
*/
#include <string.h>

/* my macro define */
#define TEST

#define NS_SL_BEGIN	namespace SL {
#define NS_SL_END	}

#define LUA_INTEGER long long
#define LUA_NUMBER double
#define LUA_KCONTEXT ptrdiff_t
/*
**	lua基本数据类型状态位
*/
#define LUA_TNONE (-1)
#define LUA_TNIL	0
#define LUA_TBOOLEAN	1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER	3
#define LUA_TSTRING 4
#define LUA_TTABLE	5
#define LUA_TFUNCTION	6
#define LUA_TUSERDATA	7
#define LUA_TTHREAD	8
#define LUA_NUMTAGS	9

/* 
**	LUA_TFUNCTION varinants:
**	0 - Lua function
**	1 - light C function
**	2 - regular C function (closure)
*/

/* Varinant tags for functions */
#define LUA_TLCL	(LUA_TFUNCTION | (0 << 4))	/* Lua closure */
#define LUA_TLCF	(LUA_TFUNCTION | (1 << 4))	/* light C function */
#define LUA_TCCL	(LUA_TFUNCTION | (2 << 4))	/* C closure */

/* Bit mark for collectable types */
#define BIT_ISCOLLECTABLE	(1 << 6 )

/* mark a tag as collectable */
#define ctb(t)		((t) | BIT_ISCOLLECTABLE)

/*
**	thread status
*/
#define LUA_OK	0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define	LUA_ERRGCMM	5
#define LUA_ERRERR	6

/*
**	possible states of the Garbage Collector
*/
#define GCSprogagate	0
#define GCSatmic	1
#define GCswpallgc	2
#define GCSswpfinobj	3
#define GCSswptobefnz	4
#define GCSswpend	5
#define GCScallfin	6
#define GCSpause	7
/*
**	kinds of Garbage Collection
*/
#define KGC_NORMAL	0
#define KGC_EMERGENCY	1	/* gc was forced by an allocation failure */

#if !defined(LUAI_GCPAUSE)
#define LUAI_GCPAUSE	200 /* 200% */
#endif

#if !defined(LUAI_GCMUL)
#define LUAI_GCMUL	200 /* GC runs 'twice the speed' of memory allocation */
#endif


/* value */
#define val_(o)		((o)->value_)

/* raw type tag for a TValue */
#define rttype(o)	((o)->tt_)

/*
**	macros that are executed whenever program enters the LUA core
**	('lua_lock') and leaves the core ('lua_unclock')
*/
#if !defined(lua_lock)
#define lua_lock(L)	((void)0)
#define lua_unlock(L)	((void) 0)
#endif

/*
**	LUAI_BITSINT defines the (miniunm) number of bits in an 'int'
*/
/* avoid undefined shifts */
#if ((INT_MAX >> 15) >> 15) >= 1
#define LUAI_BITSINT	32
#else
/* 'int' always must have at least 16 bits */
#define  LUAI_BITSINT	16
#endif

/* LUAI_MAXSTACK limits the size of the Lua stack. */
#if LUAI_BITSINT >= 32
#define LUAI_MAXSTACK	1000000
#else
#define LUAI_MAXSTACK	15000
#endif



#define LUA_API extern
#define LUAI_FUNC extern
#define LUALIB_API LUA_API
#define LUAI_DDEC LUAI_FUNC
#define LUAI_DDEF /* empty */
typedef LUA_INTEGER lua_Integer;
typedef LUA_NUMBER lua_Number;
typedef LUA_KCONTEXT lua_KContext;
#define _JBLEN	16
#define _JBTYPE int
typedef _JBTYPE jmp_buf[_JBLEN];
#define luai_jmpbuf jmp_buf
#define LUA_EXTRASPACE (sizeof(void *))
#define LUA_IDSIZE	60
#define NILCONSTANT {NULL}, LUA_TNIL

/*
**	macros to set valuse
*/
#define settt_(o, t) ((o)->tt_ = (t))

#define setnilvalue(obj) settt_(obj, LUA_TNIL)

/*
**	位操作
*/
#define bitmask(b)	(1<<(b))
#define bit2mask(b1, b2)	(bitmask(b1) | bitmask(b2))

/*
**	GC使用的状态位
*/
#define WHITE0BIT	0	/* object is white (type 0) */
#define WHITE1BIT	1	/* object is white (type 1) */

#define WHITEBITS bit2mask(WHITE0BIT, WHITE1BIT)

#define luaC_white(g) cast(lu_byte, (g)->currentwhite & WHITEBITS) /* char */

#define resethookcount(L)	(L->hookcount = L->basehookcount)

/*
**	Lua will use at most ~(2^LUAI_HASHLIMIT) bytes from a string to compute its has
*/
#if !defined(LUAI_HASHLIMIT)
#define LUAI_HASHLIMIT	5
#endif

/*
**	{===========================================
**	"Abstraction Layer" for basic report of messages and errors
*/
/* print an error message */
#if !defined(lua_wirtestringerror)
#define lua_writestringerror(s, p)	\
		(fprintf(stderr, (s), (p), fflush(stderr))
#endif

/*
**	}===========================================
*/


/*
**	cast
*/
#define cast(t, exp) ((t)(exp))
#define cast_byte(i) cast(lu_byte, (i))

typedef struct GCObject GCObject;
typedef struct UpVal Upval;
typedef struct lua_Debug lua_Debug;
typedef unsigned char lu_byte;
typedef struct lua_State lua_State;
typedef struct global_state global_state;
//typedef struct LClosure LClosure;
typedef unsigned int Instruction;
typedef int (*lua_CFunction) (lua_State *L);
typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);
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

/* type to ensure maximum alignment */
typedef union {
	lua_Number n;
	double u;
	void *s;
	lua_Integer i;
	long l;
} L_Umaxalign;


#define CommonHeader GCObject *next;lu_byte tt;lu_byte marked

/*
**	lua的基本数据类型
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

/* internal assertions for in-house debugging */
#define lua_assert(c)		((void)0)
#define check_exp(c, e)		(e)


/* assertion for checking API calls */
#define luai_apicheck(l, e) lua_assert(e)
#define api_check(l, e, msg) luai_apicheck(l, (e) && msg)


/* (address of) a fixed nil value */
#define luaO_nilobject (&luaO_nilobject_)
LUAI_DDEC const TValue luaO_nilobject_;

/*
**	Pseudo-indices
**	(-LUAI_MAXSTACK is the minium valid index; we keep some free empty
**	space after that to help overflowdetection
*/
#define LUA_REGISTRYINDEX	(-LUAI_MAXSTACK - 1000)

/* value at a non-valid index */
#define NONVALIDVALUE	cast(TValue *, luaO_nilobject)

/* test for pseudo index */
#define ispseudo(i) ((i) <= LUA_REGISTRYINDEX)


/* string cache */
#define STRCACHE_N 53
#define STRCACHE_M 2

/*
** Header for string value; string bytes follow the end of this structure
** (aligned according to 'UTString'; see next).
*/
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

/*
** Ensures that address after this type is always fully aligned
*/
typedef union UTString{
	L_Umaxalign dummy; /* ensures maximum alignment for strings */
	TString tsv;
} UTString;


typedef struct stringtable {
	TString **hash;
	int nuse;
	int size;
} stringtable;

/* 
**	Headers for userdata; memory area follows the end of structure
**	(aligned according to 'UUdata'; see next).
*/
typedef struct Udata {
	CommonHeader;
	lu_byte ttuv_;	/* user value's tag */
	struct Table *metatable;
	size_t len;	/* number of bytes */
	union Value user_;
} Udata;

/* Ensures that address after this type is always fully alignment. */
typedef union UUdata {
	L_Umaxalign dummy;	/* ensures maximum alignment for 'local' udata */
	Udata uv;
} UUdata;

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
} Node;
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
**	lua的垃圾回收机制
*/
struct GCObject {
	CommonHeader;
};

/*
**	调用信息
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
**	长跳转缓冲区链表
**	volatile 易变的
**	适用于多线程访问
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
**	Description of a local variable for function prototypes
**	(used for debug information)
*/
typedef struct LocVar {
	TString *varname;
	int startpc; /* first point where variable is active */
	int endpc; /* first point where variable is dead */
} LocVar;

/*
**	Function Prototypes
*/
typedef struct Proto {
	CommonHeader;
	lu_byte numparams; /* number of fixed parameters */
	lu_byte is_vararg; /* 2: declared vararg; 1: uses vararg */
	lu_byte maxstacksize; /* number of registers needed by this function 
*/
	int sizeupvalues; /* size of 'upvalues' */
	int sizek; /* size of 'k' */
	int sizecode;
	int sizelineinfo;
	int sizep; /* size of 'p' */
	int sizelocvars;
	int linedefined; /* debug information */
	int lastlinedefine; /* debug information */
	TValue *k; /* constants used by the function */
	Instruction *code; /* opcodes */
	struct Proto **p; /* functions defined inside the function */
	int *lineinfo; /* map from opcodes to source lines (debug information) */
	LocVar *locvars; /* information about local variables (debug information) */
	struct LClosure *cache; /* last-created closure with this prototype */
	TString *source; /* used for debug information */
	GCObject *gclist;
} Proto;

/*
**	Closures
*/
#define ClosureHeader	\
	CommonHeader; lu_byte nupvalues; GCObject *gclist

typedef struct CClosure {
	ClosureHeader;
	lua_CFunction f;
	UpVal *upvals[1];	/* list of upvalues */
} CClosure;

typedef struct LClosure {
	ClosureHeader;
	struct Proto *p;
	UpVal *upvals[1]; /* list of upvalues */
} LClosure;

typedef union Closure {
	CClosure c;
	LClosure l;
} Closure;


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
	GCObject *allgc;
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
	TString *strcache[STRCACHE_N][STRCACHE_M];
} global_State;

#define	G(L)	(L->l_G)

/*
**	Union of all collectable objects (only for conversions)
*/
union GCUnion {
	GCObject gc; /* common header */
	struct TString ts;
	struct Udata u;
	union Closure cl;
	struct Table h;
	struct Proto p;
	struct lua_State th; /* thread */
};

#define cast_u(o)	cast(union GCUnion *, (o))

/* macros to convert a GCObject into a specific value */
#define gco2ccl(o)	check_exp((o)->tt == LUA_TCCL, &((cast_u(o))->cl.c))

struct lua_State {
	CommonHeader;
	unsigned short nci;
	lu_byte status;
	StkId top;
	global_State *l_G;
	CallInfo *ci;
	const Instruction *oldpc;
	StkId stack_last;
	StkId stack;
	UpVal *openupval;
	GCObject *gclist;
	struct lua_State *twups;
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


/* Macros to test type */
#define checktag(o, t)	(rttype(o) == (t))
#define ttisclosure		((rttype(o) & 0x1F) == LUA_TFUNCTION)
#define ttisCclosure(o)	checktag((o), ctb(LUA_TCCL))
#define ttislcf(o)	checktag((o), LUA_TLCF)

/* Macros to access value */
#define clCvalue(o)	check_exp(ttisCclosure(o), gco2ccl(val_(o).gc))


//~~ random number begin
/* random seed */
#if !defined(luai_makeseed)
#include <time.h>
#define luai_makeseed() cast(unsigned int, time(NULL))
#endif
#define addbuf(b, p, e) \
{ size_t t = cast(size_t, e); \
	memcpy(b + p, &t, sizeof(t)); \
	printf("t address value is %x\n", &t);	\
	printf("t value is %d\n", t);	\
	p += sizeof(t); }

unsigned int makeseed(lua_State *L);
//~~ random number end

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
**	从栈中获取数据
*/
static TValue *index2addr(lua_State *L, int idx);

/*
**	根据nsize申请内存，存储在ptr中
*/
static void *l_alloc(void *ud, void *ptr, size_t oszie, size_t nsize);

/*
**	使用常量，但不申请内存（避免错误）的预初始化一个线程
*/
static void preinit_thread(lua_State *L, global_State *g);

/*
**	lua panic
*/
static int panic(lua_State *L);

/*
**	lua hash
*/
LUAI_FUNC unsigned int luaS_hash(const char *str, size_t l, unsigned int seed);

//~~ LUALIB_API begin
LUALIB_API lua_State *luaL_newstate(void);


//~~ LUALIB_API end

//~~ LUA_API begin
LUA_API lua_State *(lua_newstate)(lua_Alloc f, void *ud);
LUA_API lua_CFunction(lua_atpanic) (lua_State *L, lua_CFunction panicf);
//~~ LUA_API end

#endif