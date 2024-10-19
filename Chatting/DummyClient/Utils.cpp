#include "Utils.h"
#include "pch.h"
#include <locale>
#include <codecvt>

wstring Utils::StringToWString(string str)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}

string Utils::WStringToString(wstring wstr)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstr);
}