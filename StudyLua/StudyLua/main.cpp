// StudyLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;


extern "C" {
#include "./lua-5.3.3/src/lua.h"
#include "./lua-5.3.3/src/lauxlib.h"
#include "./lua-5.3.3/src/lualib.h"
#include "./lua-5.3.3/src/lobject.h"
}


void Binarycout(int n)
{
	int a = 1;
	cout << a << endl;
	for (int i = 31; i >= 0; i--)
	{
		cout << ((n >> i) & 1);
	}

	cout << endl;
}

void myBinarycout(int n) {
	cout << "This is my Binary!" << endl;
	for (int i = 31; i >= 0; i--) {
		cout << (n >> i);
	}
	cout << endl;
}

int myceil_16(int x) {
	return (x + 0xf) >> 4; /* x = ceil( x / 16 ) */
}
int _tmain(int argc, _TCHAR* argv[])
{
	Binarycout(5);
	myBinarycout(5);

	char a[10] = { "1213" }, b[10];
	int arr[] = { 1, 3, 8, 90, 100, 455 };
	std::string str = "123.4";
	cout << "This is master branch" << endl;
	char *ch = new char[100];
	//lua_strx2number(str.c_str(), &ch);

	system("pause");

	lua_State *L = luaL_newstate();

	lua_pushstring(L, "1213");
	lua_pushnumber(L, 20);

	if (lua_isstring(L, 1)) {
		cout << lua_isstring(L, 1) << endl;
	}

	if (lua_isnumber(L, 2)) {
		cout << lua_isnumber(L, 2) << endl;
	}

	lua_close(L);
	return 0;
}

