#include "StudyLua.h"
#include <stdio.h>

/* 为什么要在声明处定义为外部变量？ */
LUAI_DDEF const TValue luaO_nilobject_ = { NILCONSTANT };

static void *l_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	(void)ud;
	(void)osize;
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else
		return realloc(ptr, nsize);
}

static void *preinit_thread(lua_State *L, global_State *g) {
	G(L) = g;
	L->stack = NULL;
	L->ci = NULL;
	L->stacksize = 0;
	L->twups = L;	/* thread has no upvalues */
	L->errorJmp = NULL;
	L->nCcalls = 0;
	L->hook = NULL;
	L->hookmask = 0;
	L->basehookcount = 0;
	L->allowhook = 1;
	resethookcount(L);
	L->openupval = NULL;
	L->nny = 1;
	L->status = LUA_OK;
	L->errfunc = 0;
}

unsigned int makeseed(lua_State *L) {
	char buff[4 * sizeof(size_t)];	/* buff[16] */
#if defined(TEST)
	printf("buff address value is %x\n\n", buff);
#endif

	unsigned int h = luai_makeseed();
#if defined(TEST)
	printf("unsigned int h = %u\n", h);
	printf("buff address value is %x\n\n", buff);
#endif

	int p = 0;

	addbuf(buff, p, L);	/* heap variable */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("L(heap variable) value is %x\n", L);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[1] address value is %x\n\n", *(buff + p - sizeof(size_t)));

#endif

	addbuf(buff, p, &h);	/* local variable */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("h(local variable) value is %x\n", &h);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[2] address value is %x\n\n", *(buff + p - sizeof(size_t)));
#endif

	addbuf(buff, p, luaO_nilobject); /* global variable */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("luaO_nilobject(global variable) value is %x\n", luaO_nilobject);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[3] address value is %x\n\n", *(buff + p - sizeof(size_t)));
#endif

	addbuf(buff, p, &lua_newstate); /* public function */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("lua_newstate(public variable) value is %x\n", &lua_newstate);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[4] address value is %x\n\n", *(buff + p - sizeof(size_t)));
#endif

	lua_assert(p == sizeof(buff));
	return 1;
}


LUA_API lua_State *lua_newstate(lua_Alloc f, void * ud) {
	int i;
	lua_State *L;
	global_State *g;
	LG *l = cast(LG *, (*f)(ud, NULL, LUA_TTHREAD, sizeof(LG)));
	if (l == NULL) return NULL;
	L = &l->l.l;
	g = &l->g;
	L->next = NULL;
	L->tt = LUA_TTHREAD;
	g->currentwhite = bitmask(WHITE0BIT); /* 1 << 0 */
	L->marked = luaC_white(g);
	printf("L->marked is %c \n", L->marked);
	g->frealloc = f;
	g->ud = ud;
	g->mainthread = L;
	return 0;
}