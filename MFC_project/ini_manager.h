#pragma once
#include <afxwin.h>
#include <fstream>
#include <sstream>
#include<vector>
#include<string>
//�Ω�B�zini�ɮת�Ū����class
std::string GetIniLanguage(const CString&);
//��ϥγ]�w�n�F�y���ﶵ�A�N���ܪ��y���g�Jini�ɮ�
void WriteNewDefaultLanguage(const CString&, const CString&);
//�H�U�T��function�D�n�B�z�r��s�X���ഫ
std::string CStringToUtf8String(const CString&);
std::wstring Utf8ToWideChar(const std::string&);
CString WideCharToUtf8(const CString&);
//�b���w����Ƨ����M��S�w�����ɦW
std::vector<CString> find_specfic_file(const CString&, const CString&);