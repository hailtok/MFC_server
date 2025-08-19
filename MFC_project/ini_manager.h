#pragma once
#include <afxwin.h>
#include <fstream>
#include <sstream>
#include<vector>
#include<string>
//用於處理ini檔案的讀取的class
std::string GetIniLanguage(const CString&);
//當使用設定好了語言選項，將其選擇的語言寫入ini檔案
void WriteNewDefaultLanguage(const CString&, const CString&);
//以下三個function主要處理字串編碼的轉換
std::string CStringToUtf8String(const CString&);
std::wstring Utf8ToWideChar(const std::string&);
CString WideCharToUtf8(const CString&);
//在指定的資料夾內尋找特定的副檔名
std::vector<CString> find_specfic_file(const CString&, const CString&);