#include "StudyLua.h"
#include <stdio.h>

/* my define */
template <class T>
static void binarycout(T n) {
	for (int i = 31; i >= 0; i--) {
		cout << ((n >> i) & 1);
	}
}


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

LUAI_FUNC unsigned int luaS_hash(const char *str, size_t l, unsigned int seed) {
	str = new char[100];
	str = "0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz0123456789abcdefghijklmnopqrstuvwxyz";
	l = 256;
	unsigned int h = seed ^ cast(unsigned int, l);
#if defined(TEST)
	cout << "seed = " << seed << endl;
	cout << "seed binary = " << endl; binarycout(seed); cout << endl;
	cout << "l = " << l << endl;
	cout << "l binary = " << endl; binarycout(l); cout << endl;
	cout << "h = " << h << endl;
	cout << "h binary = " << endl; binarycout(h); cout << endl << endl;
#endif
	size_t step = (l >> LUAI_HASHLIMIT) + 1;
	cout << "l >> LUAI_HASHLIMIT = " << endl; binarycout(l >> LUAI_HASHLIMIT); cout << endl;
	cout << "step = " << step << endl << endl;

	for (int i = 0; l >= step; l -= step, i++) {
#if defined(TEST)
		cout << "i = " << i << endl;
		cout << "l = " << endl; binarycout(l); cout << endl;
#endif
		h ^= ((h << 5) + (h >> 2) + cast_byte(str[l - 1]));
	}
	return h;
}

unsigned int makeseed(lua_State *L) {
#undef TEST
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
	printf("buff[1] value is %d\n\n", *(buff + p - sizeof(size_t)));

#endif

	addbuf(buff, p, &h);	/* local variable */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("h(local variable) value is %x\n", &h);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[2] value is %d\n\n", *(buff + p - sizeof(size_t)));
#endif

	addbuf(buff, p, luaO_nilobject); /* global variable */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("luaO_nilobject(global variable) value is %x\n", luaO_nilobject);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[3] value is %d\n\n", *(buff + p - sizeof(size_t)));
#endif

	addbuf(buff, p, &lua_newstate); /* public function */
#if defined(TEST)
	printf("p value is %d\n", p);
	printf("lua_newstate(public variable) value is %x\n", &lua_newstate);
	printf("buff+p address value is %x\n", buff + p - sizeof(size_t));
	printf("buff[4] value is %d\n\n", *(buff + p - sizeof(size_t)));
#endif

	lua_assert(p == sizeof(buff));
	return luaS_hash(buff, p, h);
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