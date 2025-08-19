#pragma once
#include <afxwin.h>
#include <afxdialogex.h>
#include <string>
#include<vector>
#include<unordered_map>
#include <fstream>
#include <sstream>
#include"ini_manager.h"
#define LANGUAGE_SELECT_STATIC 300
#define LANGAUGE_BASE_BUTTON 400
#define ENGLISH_BUTTON (LANGAUGE_BASE_BUTTON + 0)
#define TRADITION_CHINESE_BUTTON (LANGAUGE_BASE_BUTTON + 1)
#define SIMPLIFIED_CHINESE_BUTTON (LANGAUGE_BASE_BUTTON + 2)
#define JAPANESE_BUTTON (LANGAUGE_BASE_BUTTON + 3)
#define KOREAN_BUTTON (LANGAUGE_BASE_BUTTON + 4)
#define LANGUAGE_CONFIRM_BUTTON 405
const int langauge_button_count = 5;
struct CStringHash
{
	std::size_t operator()(const CString& str) const {
		return std::hash<std::wstring>()(std::wstring(str));
	}
};
struct CStringEqual {
	bool operator()(const CString& lhs, const CString& rhs) const {
		return lhs.Compare(rhs) == 0;
	}
};
class Setting_Dialog : public CDialogEx {
private:
	std::unordered_map<CString, std::vector<CString>, CStringHash, CStringEqual>UI_language_text;
	std::unordered_map<int, CString>num_language_hash;
	std::unordered_map<CString, CString, CStringHash, CStringEqual>utf8_langauge_hash;
	CString user_langauge, ini_file_path;
	CFont* font;
	CStatic* language_select_static;
	CButton** language_buttons;
	CButton* language_confirm_button;
	DLGTEMPLATE* pTemplate;
	DLGTEMPLATE* CreateDialogTemplate();
protected:
	virtual BOOL OnInitDialog() override;
	afx_msg void Language_ButtonsClicked_Event(UINT);
	afx_msg void ButtonsClicked_Event();
	DECLARE_MESSAGE_MAP()
public:
	Setting_Dialog();
	Setting_Dialog(UINT, CWnd* pParent = nullptr);
	void SetIniFilePath(const CString& file_path);
	virtual ~Setting_Dialog();
};