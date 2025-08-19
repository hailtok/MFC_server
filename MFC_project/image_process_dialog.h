#pragma once
#include <afxwin.h>
#include <afxext.h>
#include <afxdialogex.h>
#include<afxmt.h>
#include<iostream>
#include <vector>
#include<unordered_map>
#include <cmath>
#include "SharedMemory.h"
#include "ini_manager.h"
#include "virtual_listbox.h"
#include "sockets.h"
#include "imagedetection.h"
#define WM_SOCKET_NOTIFY (WM_USER + 1)

#define ID_LISTBOX 200
#define ID_SCROLLBAR 201
#define ID_COMBOBOX 202
#define ID_LISTCTRL 203

#define BT_FONT1 300
#define BT_FONT2 301

#define MENU_FILE 500
#define MENU_FILE_SAVE 501
#define MENU_FILE_OPEN 502
#define MENU_LANGUAGE_TRADITIONAL_CHINESE 503
#define MENU_LANGUAGE_SIMPLIFIED_CHINESE 504
#define MENU_LANGUAGE_ENGLISH 505
#define MENU_LANGUAGE_JAPANESE 506
#define MENU_LANGUAGE_KOREAN 507

static CCriticalSection cs;
static bool receive = true;
class  image_process_dialog : public CDialogEx {
private:
	DLGTEMPLATE* pTemplate;
	HACCEL* accelerators;
	CStatic* combobox_static, * bmp_text_static, * file_name_static, * feature_point_static, * bmp_static;
	CComboBox* image_process_combo;
	virtual_listbox *listbox;
	CListCtrl* listctrl;
	UINT port = 1100;
	CServerSocket* Server_Socket;
	Shared_Memory* shared_memory;
	CButton *change_font1, *change_font2;
public:
	image_process_dialog();
	image_process_dialog(UINT,CWnd* pParent = nullptr);
	virtual ~image_process_dialog();
protected:
	virtual BOOL OnInitDialog() override;
	virtual BOOL PreTranslateMessage(MSG*) override;
	afx_msg void OnChooseFile();
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnChangeLanguage(UINT);
	afx_msg void OnGetDispInfo(NMHDR*, LRESULT*);
	afx_msg void OnVScroll(UINT, UINT, CScrollBar*);
	afx_msg LRESULT OnSocketNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonFont1();
	afx_msg void OnBnClickedButtonFont2();
	DECLARE_MESSAGE_MAP()
private:
	DLGTEMPLATE* CreateDialogTemplate();
	void LoadDigMenu();
	void LoadDigAccelerators();
	HANDLE shared_file;
	CString ini_file_path=_T(".\\config_ui_language_utf8.ini");
	std::vector<CString>files;
	std::vector<std::pair<int, int>> features;
	std::vector<char> imgBuffer;
	CFont m_customFont;
	CRect listbox_rc;
};

