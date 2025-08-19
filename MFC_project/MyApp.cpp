#include "MyApp.h"
#include "setting_dialog.h"
#include "image_process_dialog.h"
#include"resource.h"
CMyApp main_window;
BOOL CMyApp::InitInstance()
{
    CWinApp::InitInstance();


	CString ini_file_path = _T(".\\config_ui_language_utf8.ini");
    Setting_Dialog *language_setting=new Setting_Dialog();
    language_setting->SetIniFilePath(ini_file_path);
    if (GetIniLanguage(ini_file_path).empty()) {
        language_setting->DoModal();
    }
    delete language_setting;
    // 建立主對話框
    image_process_dialog dlg;
    m_pMainWnd = &dlg;
    dlg.DoModal();
    return FALSE; // 結束應用程式
}

