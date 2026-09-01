#include "common.h"


//放置一些公共的数学函数   50~90
int RandInRange(int min, int max)
{
	//0~32767   RAND_MAX  7fff
	int v=rand()%(max-min);

	return v+min;
}

//0~1


  //lib   静态链接库(开发环境，第三方api，文档）    dll  动态链接库（运行环境）  .so
float RandInRange(float min, float max)
{
	float v=static_cast<float>(rand())/RAND_MAX;

	return v*(max-min)+min;
}





// 内部工具函数：多字节转宽字符
std::wstring multi_byte_to_wide_char(const std::string& str, UINT code_page) {
	if (str.empty()) return L"";

	int wide_size = MultiByteToWideChar(
		code_page,
		MB_ERR_INVALID_CHARS,
		str.c_str(),
		static_cast<int>(str.size()),
		nullptr,
		0
	);

	if (wide_size <= 0) {
		return L"";
	}

	std::vector<wchar_t> wide_buffer(wide_size);
	int ret = MultiByteToWideChar(
		code_page,
		0,
		str.c_str(),
		static_cast<int>(str.size()),
		wide_buffer.data(),
		wide_size
	);

	if (ret <= 0) {
		return L"";
	}

	return std::wstring(wide_buffer.data(), wide_size);
}

// 内部工具函数：宽字符转多字节
std::string wide_char_to_multi_byte(const std::wstring& wstr, UINT code_page) {
	if (wstr.empty()) return "";
	BOOL usedDefault = FALSE;
	char defaultChar = '?';
	int mb_size = WideCharToMultiByte(
		CP_ACP,
		0,
		wstr.c_str(),
		-1,
		nullptr,
		0,
		&defaultChar,
		&usedDefault
	);

	if (mb_size <= 0) {
		return "";
	}

	std::vector<char> mb_buffer(mb_size);
	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		wstr.c_str(),
		-1,
		mb_buffer.data(),
		mb_size,
		&defaultChar,
		&usedDefault
	);

	if (ret <= 0) {
		return "";
	}

	return std::string(mb_buffer.data(), mb_size);
}

// UTF-8 转 GBK
std::string utf8_to_gbk(const std::string& utf8_str) {
	std::wstring wide_str = multi_byte_to_wide_char(utf8_str, CP_UTF8);
	if (wide_str.empty() && !utf8_str.empty()) return "";
	return wide_char_to_multi_byte(wide_str, 936); // GBK 代码页为 936
}

// GBK 转 UTF-8
std::string gbk_to_utf8(const std::string& gbk_str) {
	std::wstring wide_str = multi_byte_to_wide_char(gbk_str, 936);
	if (wide_str.empty() && !gbk_str.empty()) return "";
	return wide_char_to_multi_byte(wide_str, CP_UTF8);
}
