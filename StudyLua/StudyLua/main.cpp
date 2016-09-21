// StudyLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;

//#define LUA_TAG_TRUE

#if defined(LUA_TAG_TRUE)

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

	//Binarycout(5);
	//myBinarycout(5);

	//char a[10] = { "1213" };
	//int arr[] = { 1, 3, 8, 90, 100, 455 };
	//std::string str = "123.4";
	//cout << "This is master branch" << endl;
	//char *ch = new char[100];
	////lua_strx2number(str.c_str(), &ch);
	//lua_State *L = new lua_State;
	//makeseed(L);

	//system("pause");

	char buff[100];
	lua_integer2str(buff, sizeof(buff), 123);
	cout << "buff = " << buff << endl;

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

#else

#include "StudyLua.h"

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
	return (x + 0xF) >> 4; /* x = ceil( x / 16 ) */
}
int _tmain(int argc, _TCHAR* argv[])
{
	cout << sizeof((floor(1.1f))) << endl;

	char buff[100] = "-9012345678777777777777";
	cout << buff[strspn(buff, "-012345678")] << endl;

	lua_State *L = new lua_State;

	cout << "sizeof L_Umaxalign = " << sizeof(L_Umaxalign) << endl;
	cout << "sizeof TString = " << sizeof(TString) << endl;
	cout << "sizeof UString = " << sizeof(UTString) << endl;
	cout << "sizeof Udata = " << sizeof(Udata) << endl;
	cout << "sizeof UUdata = " << sizeof(UUdata) << endl;

	cout << check_exp(false, 0) << endl;
	cout << check_exp(true, 1) << endl;

	Binarycout(5);
	myBinarycout(5);

	char a[10] = { "1213" };
	int arr[] = { 1, 3, 8, 90, 100, 455 };
	std::string str = "123.4";
	cout << "This is master branch" << endl;
	char *ch = new char[100];
	//lua_strx2number(str.c_str(), &ch);
	api_check(L, 1, "This is one");
	api_check(L, 0, "This is zeor");
	makeseed(L);

	system("pause");
	return 0;
}


#endif