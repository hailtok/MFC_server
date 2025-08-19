#include"ini_manager.h"
std::string GetIniLanguage(const CString& file_path) {
	std::ifstream file(file_path);
	std::string line, language;
	while (std::getline(file, line)) {
		if (line == "[LANGUAGE]") {
			std::getline(file, line);
			size_t eqPos = line.find('=');
			language = line.substr(eqPos + 1);
			break;
		}
	}
	return language;
}
void WriteNewDefaultLanguage(const CString& language, const CString& file_path) {
	std::ifstream file(file_path);
	std::string line;
	std::vector<std::string> output;
	while (std::getline(file, line)) {
		if (line.empty()) continue;
		output.push_back(line);
		if (line == "[LANGUAGE]") {
			std::getline(file, line);
			output.push_back("category=" + CStringToUtf8String(language));
		}
	}
	std::ofstream out(file_path, std::ios::binary | std::ios::trunc);
	for (const std::string& l : output) {
		out << l << "\n";
	}
}
std::string CStringToUtf8String(const CString& cstr) {
	LPCWSTR wideStr = cstr.GetString();
	int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, nullptr, 0, nullptr, nullptr);
	if (utf8Len <= 0) return "";
	std::string utf8Str(utf8Len, 0);
	WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, &utf8Str[0], utf8Len, nullptr, nullptr);
	utf8Str.pop_back();
	return utf8Str;
}
std::wstring Utf8ToWideChar(const std::string& utf8Str) {
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	std::wstring wide(len, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wide[0], len);
	return wide;
}
CString WideCharToUtf8(const CString& wideStr) {
	LPCWSTR wstr = wideStr.GetString();
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
	CStringA utf8Str;
	LPSTR buffer = utf8Str.GetBuffer(len);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, len, nullptr, nullptr);
	utf8Str.ReleaseBuffer();
	return CString(utf8Str);
}
std::vector<CString> find_specfic_file(const CString& file_path, const CString& file_subtitle) {
	std::vector<CString>output;
	// search specific file in the directory
	CString searchPath = file_path + _T("*.")+ file_subtitle;
	CFileFind finder;
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking) {
		bWorking = finder.FindNextFile();
		if (!finder.IsDots() && !finder.IsDirectory()) {
			output.push_back(finder.GetFileName());
		}
	}
	finder.Close();
	return output;
}