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
**	thread status
*/
#define LUA_OK	0
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define	LUA_ERRGCMM	5
#define LUA_ERRERR	6

#define LUA_API extern
#define LUAI_FUNC extern
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

#define CommonHeader GCObject *next;lu_byte tt;lu_byte marked

/*
**	lua的基本书类型
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
#define lua_assert(c) ((void)0)

/* (address of) a fixed nil value */
#define luaO_nilobject (&luaO_nilobject_)
LUAI_DDEC const TValue luaO_nilobject_;

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
	TString *strcache[STRCACHE_N][STRCACHE_M];
} global_State;

#define	G(L)	(L->l_G)


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
**	根据nsize申请内存，存储在ptr中
*/
static void *l_alloc(void *ud, void *ptr, size_t oszie, size_t nsize);

/*
**	使用常量，但不申请内存（避免错误）的预初始化一个线程
*/
static void *preinit_thread(lua_State *L, global_State *g);

/*
**	lua hash
*/
LUAI_FUNC unsigned int luaS_hash(const char *str, size_t l, unsigned int seed);

//~~ LUALIB_API begin



//~~ LUALIB_API end

//~~ LUA_API begin
LUA_API lua_State *(lua_newstate)(lua_Alloc f, void *ud);

//~~ LUA_API end

#endif