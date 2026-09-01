#pragma once
#include "myhead.h"

//放置一些公共的数学函数

//随机一个有范围的整数
int RandInRange(int min, int max);

//随机一个有范围的小数
float RandInRange(float min, float max);


//字符集转化函数(ANSI->Unicode）




// 内部工具函数：多字节转宽字符
std::wstring multi_byte_to_wide_char(const std::string& str, UINT code_page) ;

// 内部工具函数：宽字符转多字节
std::string wide_char_to_multi_byte(const std::wstring& wstr, UINT code_page) ;

// UTF-8 转 GBK
std::string utf8_to_gbk(const std::string& utf8_str) ;

// GBK 转 UTF-8
std::string gbk_to_utf8(const std::string& gbk_str) ;





