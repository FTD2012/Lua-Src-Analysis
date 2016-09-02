#include "StudyLua.h"
#include <stdio.h>

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

	return 0;
}