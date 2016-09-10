/*
** $Id: lstate.h,v 2.130 2015/12/16 16:39:38 roberto Exp $
** Global State
** See Copyright Notice in lua.h
*/

#ifndef lstate_h
#define lstate_h

#include "lua.h"

#include "lobject.h"
#include "ltm.h"
#include "lzio.h"


/*

** Some notes about garbage-collected objects: All objects in Lua must
** be kept somehow accessible until being freed, so all objects always
** belong to one (and only one) of these lists, using field 'next' of
** the 'CommonHeader' for the link:
**
** 'allgc': all objects not marked for finalization;
** 'finobj': all objects marked for finalization;
** 'tobefnz': all objects ready to be finalized;
** 'fixedgc': all objects that are not to be collected (currently
** only small strings, such as reserved words).

*/


struct lua_longjmp;  /* defined in ldo.c */


/*
** Atomic type (relative to signals) to better ensure that 'lua_sethook'
** is thread safe
*/
#if !defined(l_signalT)
#include <signal.h>
#define l_signalT	sig_atomic_t
#endif


/* extra stack space to handle TM calls and some other extras */
#define EXTRA_STACK   5


#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)


/* kinds of Garbage Collection */
#define KGC_NORMAL	0
#define KGC_EMERGENCY	1	/* gc was forced by an allocation failure */


typedef struct stringtable {
	TString **hash;
	int nuse;  /* number of elements */
	int size;
} stringtable;


/*
** Information about a call.
** When a thread yields, 'func' is adjusted to pretend that the
** top function has only the yielded values in its stack; in that
** case, the actual 'func' value is saved in field 'extra'.
** When a function calls another with a continuation, 'extra' keeps
** the function index so that, in case of errors, the continuation
** function can be called with the correct top.
*/
typedef struct CallInfo {
	StkId func;  /* function index in the stack */
	StkId	top;  /* top for this function */
	struct CallInfo *previous, *next;  /* dynamic call link */
	union {
		struct {  /* only for Lua functions */
			StkId base;  /* base for this function */
			const Instruction *savedpc;
		} l;
		struct {  /* only for C functions */
			lua_KFunction k;  /* continuation in case of yields */
			ptrdiff_t old_errfunc;
			lua_KContext ctx;  /* context info. in case of yields */
		} c;
	} u;
	ptrdiff_t extra;
	short nresults;  /* expected number of results from this function */
	lu_byte callstatus;
} CallInfo;

/*
** Bits in CallInfo status
*/
#define CIST_OAH	(1<<0)	/* original value of 'allowhook' */
#define CIST_LUA	(1<<1)	/* call is running a Lua function */
#define CIST_HOOKED	(1<<2)	/* call is running a debug hook */
#define CIST_FRESH	(1<<3)	/* call is running on a fresh invocation
of luaV_execute */
#define CIST_YPCALL	(1<<4)	/* call is a yieldable protected call */
#define CIST_TAIL	(1<<5)	/* call was tail called */
#define CIST_HOOKYIELD	(1<<6)	/* last hook called yielded */
#define CIST_LEQ	(1<<7)  /* using __lt for __le */

#define isLua(ci)	((ci)->callstatus & CIST_LUA)

/* assume that CIST_OAH has offset 0 and that 'v' is strictly 0/1 */
#define setoah(st,v)	((st) = ((st) & ~CIST_OAH) | (v))
#define getoah(st)	((st) & CIST_OAH)


/*
**	'global state', shared by all threads of this state
**	全局脚本状态数据
**	Lua这门语言考虑了多线程的情况，在脚本空间中能够开多个
**	线程相关脚本上下文，而大家会共用一个全局脚本状态数据
**	reference: http://www.tuicool.com/articles/YRnyayM
*/
typedef struct global_State {
	lua_Alloc frealloc;  /* function to reallocate memory */
	void *ud;         /* auxiliary（辅助的） data to 'frealloc' */
	l_mem totalbytes;  /* number of bytes currently allocated - GCdebt */
	l_mem GCdebt;  /* bytes allocated not yet compensated(补偿，抵消) by the collector */
	lu_mem GCmemtrav;  /* memory traversed by the GC */
	lu_mem GCestimate;  /* an estimate of the non-garbage memory in use */
	stringtable strt;  /* hash table for strings | TString 全局字符串哈希表*/
	TValue l_registry;	/* 对应 LUA REGISTRYINDEX 的全局table*/
	unsigned int seed;  /* randomized seed for hashes */
	lu_byte currentwhite;
	lu_byte gcstate;  /* state of garbage collector */
	lu_byte gckind;  /* kind of GC running */
	lu_byte gcrunning;  /* true if GC is running */
	GCObject *allgc;  /* list of all collectable objects */
	GCObject **sweepgc;  /* current position of sweep in list */
	GCObject *finobj;  /* list of collectable objects with finalizers(终结) */
	GCObject *gray;  /* list of gray objects */
	GCObject *grayagain;  /* list of objects to be traversed atomically */
	GCObject *weak;  /* list of tables with weak values */
	GCObject *ephemeron;  /* list of ephemeron tables (weak keys) */
	GCObject *allweak;  /* list of all-weak tables */
	GCObject *tobefnz;  /* list of userdata to be GC */
	GCObject *fixedgc;  /* list of objects not to be collected */
	struct lua_State *twups;  /* list of threads with open upvalues */
	unsigned int gcfinnum;  /* number of finalizers to call in each GC step */
	int gcpause;  /* size of pause between successive GCs */
	int gcstepmul;  /* GC 'granularity' (间隔尺寸,粒度) */
	lua_CFunction panic;  /* to be called in unprotected errors */
	struct lua_State *mainthread;
	const lua_Number *version;  /* pointer to version number */
	TString *memerrmsg;  /* memory-error message */
	TString *tmname[TM_N];  /* array with tag-method names | 元方法的名称字符串 */
	struct Table *mt[LUA_NUMTAGS];  /* metatables for basic types */
	TString *strcache[STRCACHE_N][STRCACHE_M];  /* cache for strings in API */
} global_State;


/*
** 'per thread' state
** 与线程相关的脚本上下文lua_State
** reference: http://www.tuicool.com/articles/YRnyayM
*/
struct lua_State {
	CommonHeader;
	unsigned short nci;  /* number of items in 'ci' list */
	lu_byte status;	/* 线程脚本的状态, 可选状态 #include <lua.h> */
	StkId top;  /* first free slot in the stack | 指向当前线程栈的栈顶指针 */
	global_State *l_G;	/* 指向全局状态的指针 */
	CallInfo *ci;  /* call info for current function | 当前线程运行的函数调用信息 */
	const Instruction *oldpc;  /* last pc traced | 函数调用前，记录上一个函数的pc位置 */
	StkId stack_last;  /* last free slot in the stack | 栈的实际最后一个位置（栈的长度是动态增加的） */
	StkId stack;  /* stack base | 栈底 */
	UpVal *openupval;  /* list of open upvalues in this stack */
	GCObject *gclist;	/* 用于GC */
	struct lua_State *twups;  /* list of threads with open upvalues */
	struct lua_longjmp *errorJmp;  /* current error recover point | 发生错误的长跳转位置，用于记录当函数发生错误时跳转出去的位置。*/
	CallInfo base_ci;  /* CallInfo for first level (C calling Lua) | 指向函数调用栈的栈底 */
	volatile lua_Hook hook;	/* 挂钩/钩子 */
	ptrdiff_t errfunc;  /* current error handling function (stack index) */
	int stacksize;	/* 栈的大小 */
	int basehookcount;	/* 用户设置的执行时指令数（LUA_MASKCOUNT）下有效*/
	int hookcount;	/* 运行时，跑了多少条指令（LUA_MASKCOUNT）下有效 */
	unsigned short nny;  /* number of non-yieldable calls in stack */
	unsigned short nCcalls;  /* number of nested C calls | 当前C函数的调用深度 */
	l_signalT hookmask;	/* 支持的hook能力定义 #include <lua.h> */
	lu_byte allowhook;	/* 是否允许hook*/
};


#define G(L)	(L->l_G)


/*
** Union of all collectable objects (only for conversions)
*/
union GCUnion {
	GCObject gc;  /* common header */
	struct TString ts;
	struct Udata u;
	union Closure cl;
	struct Table h;
	struct Proto p;
	struct lua_State th;  /* thread */
};


#define cast_u(o)	cast(union GCUnion *, (o))

/* macros to convert a GCObject into a specific value */
#define gco2ts(o)  \
	check_exp(novariant((o)->tt) == LUA_TSTRING, &((cast_u(o))->ts))
#define gco2u(o)  check_exp((o)->tt == LUA_TUSERDATA, &((cast_u(o))->u))
#define gco2lcl(o)  check_exp((o)->tt == LUA_TLCL, &((cast_u(o))->cl.l))
#define gco2ccl(o)  check_exp((o)->tt == LUA_TCCL, &((cast_u(o))->cl.c))
#define gco2cl(o)  \
	check_exp(novariant((o)->tt) == LUA_TFUNCTION, &((cast_u(o))->cl))
#define gco2t(o)  check_exp((o)->tt == LUA_TTABLE, &((cast_u(o))->h))
#define gco2p(o)  check_exp((o)->tt == LUA_TPROTO, &((cast_u(o))->p))
#define gco2th(o)  check_exp((o)->tt == LUA_TTHREAD, &((cast_u(o))->th))


/* macro to convert a Lua object into a GCObject */
#define obj2gco(v) \
	check_exp(novariant((v)->tt) < LUA_TDEADKEY, (&(cast_u(v)->gc)))


/* actual number of total bytes allocated */
#define gettotalbytes(g)	cast(lu_mem, (g)->totalbytes + (g)->GCdebt)

LUAI_FUNC void luaE_setdebt(global_State *g, l_mem debt);
LUAI_FUNC void luaE_freethread(lua_State *L, lua_State *L1);
LUAI_FUNC CallInfo *luaE_extendCI(lua_State *L);
LUAI_FUNC void luaE_freeCI(lua_State *L);
LUAI_FUNC void luaE_shrinkCI(lua_State *L);


#endif

