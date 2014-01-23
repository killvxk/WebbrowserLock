#include "Control.h"
#include<iostream>   
using namespace std;  

#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.LuaControl.lib")   
#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")  
#pragma comment(lib,DIRECTORY_LIB "Function\\Function.Internal.AdditionalControl.lib")   
lua_State* L;  

int luaadd(int x, int y)  
{  
	int sum = 0;  
	lua_getglobal(L, "add");  
	lua_pushnumber(L, x);  
	lua_pushnumber(L, y);  
	lua_call(L, 2, 1);  
	sum = (int)lua_tonumber(L, -1);  
	lua_pop(L, -1);  
	return sum;  
}  

BOOL WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd ) 
{  
	StartAdditional();
	Sleep(5000);
	int sum = 0;
	L = luaL_newstate();    
	luaL_dofile(L, "1.lua");  
	sum = luaadd(10, 5);  
	cout<<"Sum = "<<sum<<endl;          
	lua_close(L);  
}  