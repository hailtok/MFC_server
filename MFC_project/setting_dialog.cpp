#include"setting_dialog.h"
BEGIN_MESSAGE_MAP(Setting_Dialog, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, LANGAUGE_BASE_BUTTON,
		LANGAUGE_BASE_BUTTON + langauge_button_count - 1, &Setting_Dialog::Language_ButtonsClicked_Event)
	ON_BN_CLICKED(LANGUAGE_CONFIRM_BUTTON, ButtonsClicked_Event)
END_MESSAGE_MAP()
Setting_Dialog::Setting_Dialog() {
	//UI language for multiple languages
	pTemplate = this->CreateDialogTemplate();
	this->InitModalIndirect(pTemplate);
	user_langauge = _T("ENGLISH");
	UI_language_text = {
		{_T("ENGLISH"),{_T("Choose Language"),_T("English"),_T("Tradition Chinese"),
		_T("Simplified Chinese"),_T("Japanese"),_T("Korean"),_T("Confirm")}},

		{_T("繁體中文"),{_T("選擇語言"),_T("英文"),_T("繁體中文"),_T("簡體中文"),
		_T("日文"),_T("韓文"),_T("確認")}},

		{_T("简体中文"),{_T("选择语言"),_T("英文"),_T("繁体中文"),_T("简体中文"),
		_T("日文"),_T("朝鲜语"),_T("确认")}},

		{_T("日本語"),{_T("言語を選択してください"),_T("英語"),_T("繁体字中国語"),_T("簡体字中国語"),
		_T("日本語"),_T("韓国語"),_T("確認します")}},

		{_T("한국인"),{_T("언어 선택"),_T("영어"),_T("중국어 번체"),_T("중국어 간체"),
		_T("일본어"),_T("한국인"),_T("확인하다")}}
	};
	num_language_hash = { {0,_T("ENGLISH")},{1,_T("繁體中文")}, {2,_T("简体中文")},
		{3,_T("日本語")}, {4,_T("한국인")} };
	utf8_langauge_hash = { {_T("ENGLISH"),_T("en-us")},{_T("Tradition Chinese"),_T("zh-tw")},
		{_T("Simplified Chinese"),_T("zh-cn")},{_T("Japanese"),_T("ja-jp")},{_T("Korean"),_T("ko-kr")} };
	font = new CFont();
	language_select_static = new CStatic();
	language_buttons = new CButton * [langauge_button_count];
	for (int i = 0; i < langauge_button_count; ++i) {
		language_buttons[i] = new CButton();
	}
	language_confirm_button = new CButton();
}
Setting_Dialog::Setting_Dialog(UINT Template, CWnd* pParent) :CDialogEx(Template, pParent) {
	//UI language for multiple languages
	pTemplate = nullptr;
	user_langauge = _T("ENGLISH");
	UI_language_text = {
		{_T("ENGLISH"),{_T("Choose Language"),_T("English"),_T("Tradition Chinese"),
		_T("Simplified Chinese"),_T("Japanese"),_T("Korean"),_T("Confirm")}},

		{_T("繁體中文"),{_T("選擇語言"),_T("英文"),_T("繁體中文"),_T("簡體中文"),
		_T("日文"),_T("韓文"),_T("確認")}},

		{_T("简体中文"),{_T("选择语言"),_T("英文"),_T("繁体中文"),_T("简体中文"),
		_T("日文"),_T("朝鲜语"),_T("确认")}},

		{_T("日本語"),{_T("言語を選択してください"),_T("英語"),_T("繁体字中国語"),_T("簡体字中国語"),
		_T("日本語"),_T("韓国語"),_T("確認します")}},

		{_T("한국인"),{_T("언어 선택"),_T("영어"),_T("중국어 번체"),_T("중국어 간체"),
		_T("일본어"),_T("한국인"),_T("확인하다")}}
	};
	num_language_hash = { {0,_T("ENGLISH")},{1,_T("繁體中文")}, {2,_T("简体中文")},
		{3,_T("日本語")}, {4,_T("한국인")} };
	utf8_langauge_hash = { {_T("ENGLISH"),_T("en-us")},{_T("Tradition Chinese"),_T("zh-tw")},
		{_T("Simplified Chinese"),_T("zh-cn")},{_T("Japanese"),_T("ja-jp")},{_T("Korean"),_T("ko-kr")} };
	font = new CFont();
	language_select_static = new CStatic();
	language_buttons = new CButton * [langauge_button_count];
	for (int i = 0; i < langauge_button_count; ++i) {
		language_buttons[i] = new CButton();
	}
	language_confirm_button = new CButton();
}
BOOL Setting_Dialog::OnInitDialog() {
	CDialogEx::OnInitDialog();
	font->CreateFont(
		16,                        // 高度（像素）
		0, 0, 0, FW_NORMAL,        // 寬、傾斜、粗細等
		FALSE, FALSE, FALSE,       // Italic, Underline, StrikeOut
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		_T("MS Shell Dlg"));       // 字型名稱
	language_select_static->Create(UI_language_text[num_language_hash[0]][0], WS_CHILD | WS_VISIBLE | SS_CENTER,
		CRect(10, 10, 300, 40), this, LANGUAGE_SELECT_STATIC);
	language_select_static->SetFont(font);
	for (int i = 0; i < langauge_button_count; ++i) {
		language_buttons[i]->Create(UI_language_text[num_language_hash[0]][i + 1], WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			CRect(10, 50 + i * 30, 300, 80 + i * 30), this, LANGAUGE_BASE_BUTTON + i);
		language_buttons[i]->SetFont(font);
	}
	language_confirm_button->Create(UI_language_text[num_language_hash[0]][6], WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		CRect(10, 200, 300, 240), this, LANGUAGE_CONFIRM_BUTTON);
	language_confirm_button->SetFont(font);
	//default language selection
	((CButton*)GetDlgItem(ENGLISH_BUTTON))->SetCheck(BST_CHECKED);
	return TRUE;
}
//Change UI Language when user selects a language
void Setting_Dialog::Language_ButtonsClicked_Event(UINT ID) {
	int selected_language = ID - LANGAUGE_BASE_BUTTON;
	user_langauge = UI_language_text[num_language_hash[0]][selected_language + 1];
	if (selected_language >= 0) {
		//write static text
		GetDlgItem(LANGUAGE_SELECT_STATIC)->SetWindowText(UI_language_text[num_language_hash[selected_language]][0]);
		//write button text
		for (int i = 1; i < langauge_button_count + 1; ++i) {
			CButton* radio_button = (CButton*)GetDlgItem(LANGAUGE_BASE_BUTTON + i - 1);
			radio_button->SetWindowText(UI_language_text[num_language_hash[selected_language]][i]);
		}
		//write confirm button text
		GetDlgItem(LANGUAGE_CONFIRM_BUTTON)->SetWindowText(UI_language_text[num_language_hash[selected_language]][6]);
	}
}
DLGTEMPLATE* Setting_Dialog::CreateDialogTemplate() {
	// 配置記憶體
	BYTE* pMem = new BYTE[1024];
	ZeroMemory(pMem, 1024);

	DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

	// 基本對話框樣式
	pDlg->style = DS_SETFONT | DS_CENTER | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
	pDlg->cx = 205;
	pDlg->cy = 150;

	BYTE* p = (BYTE*)(pDlg + 1);

	// 無 menu
	*(WORD*)p = 0; p += sizeof(WORD);

	// 使用預設 dialog class
	*(WORD*)p = 0; p += sizeof(WORD);

	// Caption 字串
	wcscpy_s((WCHAR*)p, 32, L"LANGUAGE SETTING WINDOW");
	p += (wcslen(L"LANGUAGE SETTING WINDOW") + 1) * sizeof(WCHAR);

	// Font 設定
	*(WORD*)p = 9;  p += sizeof(WORD); // 字體大小
	wcscpy_s((WCHAR*)p, 32, L"MS Shell Dlg");

	return pDlg;
}
//Write the selected language to the ini file
void Setting_Dialog::ButtonsClicked_Event() {
	WriteNewDefaultLanguage(utf8_langauge_hash[user_langauge], ini_file_path);
	EndDialog(IDOK);
}
void Setting_Dialog::SetIniFilePath(const CString& file_path) {
	ini_file_path = file_path;
}
Setting_Dialog::~Setting_Dialog() {

	delete pTemplate;
	delete font;
	delete language_select_static;
	for (int i = 0; i < langauge_button_count; ++i) {
		delete language_buttons[i];
	}
	delete[] language_buttons;
	delete language_confirm_button;
}