#pragma once


#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
//系统头文件最好是尖括号<>，写双引号也没有错
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <math.h>

#include <d3d9.h>		//DX微软提供的图形库
#include <d3dx9.h>


#include <conio.h>
#include <windows.h>
#include <tchar.h>
#include <vector>	//动态数组
#include <list>		//双向链表
#include <set>		//集合
#include <map>		//映射

#include <string>
using namespace std;




#define DECLARE_MEMBER(type, name,FunName) \
	private:type m_##name; \
	public:void Set##FunName(type t) \
	{ \
		m_##name = t; \
	} \
	public:type Get##FunName() \
	{ \
		return m_##name; \
	} 

#define DECLARE_MEMBER_READONLY(type, name,FunName) \
	private:type m_##name; \
	public:type Get##FunName() \
	{ \
		return m_##name; \
	} 

#define DECLARE_MEMBER_WRITEONLY(type, name,FunName) \
	private:type m_##name; \
	public:void Set##FunName(type t) \
	{ \
		m_##name = t; \
	} 

#define DELETE_OBJECT(obj) \
	if (obj) \
	{ \
		delete obj; \
		obj = NULL; \
	}


#define DELETE_ARRAY(obj) \
	if (obj) \
	{ \
		delete[] obj; \
		obj = NULL; \
	}

#ifdef _UNICODE
#define TSTRING wstring
#define TCOUT wcout
#else
#define TSTRING string
#define TCOUT cout
#endif



#include "common.h"
#include "Singleton.h"
#include "Graphic.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
