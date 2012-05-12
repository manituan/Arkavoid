#include "D3DCore_Impl.h"


void D3DCore_Impl::Ini_SetInt(const WCHAR *section, const WCHAR *name, int value)
{
	WCHAR buf[256];

	if(szIniFile[0]) {
		swprintf(buf,256,L"%d",value);
		WritePrivateProfileString(section, name, buf, szIniFile.c_str());
	}
}


int D3DCore_Impl::Ini_GetInt(const WCHAR *section, const WCHAR *name, int def_val)
{
	WCHAR buf[256];

	if(szIniFile[0]) {
		if(GetPrivateProfileString(section, name, L"", buf, sizeof(buf), szIniFile.c_str()))
		{ return _wtoi(buf); }
		else { return def_val; }
	}
	return def_val;
}


void D3DCore_Impl::Ini_SetFloat(const WCHAR *section, const WCHAR *name, float value)
{
	WCHAR buf[256];

	if(szIniFile[0]) {
		swprintf(buf,256,L"%f",value);
		WritePrivateProfileString(section, name, buf, szIniFile.c_str());
	}
}


float D3DCore_Impl::Ini_GetFloat(const WCHAR *section, const WCHAR *name, float def_val)
{
	WCHAR buf[256];

	if(szIniFile[0]) {
		if(GetPrivateProfileString(section, name, L"", buf, sizeof(buf), szIniFile.c_str()))
		{ return (float)_wtof(buf); }
		else { return def_val; }
	}
	return def_val;
}


void D3DCore_Impl::Ini_SetString(const WCHAR *section, const WCHAR *name, const WCHAR *value)
{
	if(szIniFile[0]) WritePrivateProfileString(section, name, value, szIniFile.c_str());
}


WCHAR* D3DCore_Impl::Ini_GetString(const WCHAR *section, const WCHAR *name, const WCHAR *def_val)
{
	if(szIniFile[0]) GetPrivateProfileString(section, name, def_val, szIniString, sizeof(szIniString), szIniFile.c_str());
	else wcscpy(szIniString, def_val);
	return szIniString;
}

