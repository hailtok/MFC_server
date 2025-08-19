#include"image_process_dialog.h"
#include <atlimage.h>
BEGIN_MESSAGE_MAP(image_process_dialog, CDialogEx)
    ON_LBN_SELCHANGE(ID_LISTBOX, &image_process_dialog::OnChooseFile)
    ON_COMMAND(MENU_FILE_OPEN, &image_process_dialog::OnFileOpen)
    ON_COMMAND(MENU_FILE_SAVE, &image_process_dialog::OnFileSave)
    ON_COMMAND_RANGE(MENU_LANGUAGE_TRADITIONAL_CHINESE, MENU_LANGUAGE_KOREAN, &image_process_dialog::OnChangeLanguage)
    ON_WM_VSCROLL()
    ON_NOTIFY(LVN_GETDISPINFO, ID_LISTCTRL, &image_process_dialog::OnGetDispInfo)
    ON_MESSAGE(WM_SOCKET_NOTIFY, &image_process_dialog::OnSocketNotify)
	ON_BN_CLICKED(BT_FONT1, &image_process_dialog::OnBnClickedButtonFont1)
	ON_BN_CLICKED(BT_FONT2, &image_process_dialog::OnBnClickedButtonFont2)
END_MESSAGE_MAP()
image_process_dialog::image_process_dialog(){
	pTemplate = CreateDialogTemplate();
    this->InitModalIndirect(pTemplate);
    shared_file = NULL;
    accelerators = new HACCEL();
    combobox_static = new CStatic();
    bmp_text_static = new CStatic();
    file_name_static = new CStatic();
    feature_point_static = new CStatic();
    bmp_static = new CStatic();
    image_process_combo = new CComboBox();
    listbox = new virtual_listbox();
    listctrl = new CListCtrl();
    Server_Socket = new CServerSocket(this);
    shared_memory = new Shared_Memory();
    change_font1 = new CButton();
	change_font2 = new CButton();
}
image_process_dialog::image_process_dialog(UINT Template,CWnd* pParent) :CDialogEx(Template, pParent) {
    pTemplate = nullptr;
    shared_file = NULL;
    accelerators = new HACCEL();
    combobox_static = new CStatic();
    bmp_text_static = new CStatic();
    file_name_static = new CStatic();
    feature_point_static = new CStatic();
    bmp_static = new CStatic();
    image_process_combo = new CComboBox();
    listbox = new virtual_listbox();
	listctrl = new CListCtrl();
	Server_Socket = new CServerSocket(this);
    shared_memory = new Shared_Memory();
    change_font1 = new CButton();
    change_font2 = new CButton();
}
BOOL image_process_dialog::PreTranslateMessage(MSG* pMsg) {
    if (accelerators && TranslateAccelerator(this->GetSafeHwnd(), *accelerators, pMsg)) {
        return TRUE;
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}
DLGTEMPLATE* image_process_dialog::CreateDialogTemplate() {
    // 配置記憶體
    BYTE* pMem = new BYTE[1024];
    ZeroMemory(pMem, 1024);

    DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

    // 基本對話框樣式
    pDlg->style = DS_SETFONT | DS_CENTER | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
    pDlg->cx = 600;
    pDlg->cy = 360;

    BYTE* p = (BYTE*)(pDlg + 1);

    // 無 menu
    *(WORD*)p = 0; p += sizeof(WORD);

    // 使用預設 dialog class
    *(WORD*)p = 0; p += sizeof(WORD);

    // Caption 字串
    wcscpy_s((WCHAR*)p, 32, L"IMAGE PROCESSING WINDOW");
    p += (wcslen(L"IMAGE PROCESSING WINDOW") + 1) * sizeof(WCHAR);

    // Font 設定
    *(WORD*)p = 10;  p += sizeof(WORD); // 字體大小
    wcscpy_s((WCHAR*)p, 32, L"Microsoft YaHei UI");

    return pDlg;
}
void image_process_dialog::LoadDigMenu() {
    HMENU hMenuBar = CreateMenu();           // 主選單列
    HMENU hFileMenu = CreatePopupMenu();     // 檔案 子選單
    HMENU hLanguageMenu = CreatePopupMenu(); // 語言 子選單

    // 加入 檔案(F)
    AppendMenu(hFileMenu, MF_STRING, MENU_FILE_SAVE, _T("儲存(&S)\tCtrl+S"));
    AppendMenu(hFileMenu, MF_STRING, MENU_FILE_OPEN, _T("開啟(&O)\tCtrl+O"));

    // 加入 語言(L)
    AppendMenu(hLanguageMenu, MF_STRING, MENU_LANGUAGE_TRADITIONAL_CHINESE, _T("繁體中文(&Z)"));
    AppendMenu(hLanguageMenu, MF_STRING, MENU_LANGUAGE_SIMPLIFIED_CHINESE, _T("簡體中文(&X)"));
    AppendMenu(hLanguageMenu, MF_STRING, MENU_LANGUAGE_ENGLISH, _T("英文(&C)"));
    AppendMenu(hLanguageMenu, MF_STRING, MENU_LANGUAGE_JAPANESE, _T("日文(&V)"));
    AppendMenu(hLanguageMenu, MF_STRING, MENU_LANGUAGE_KOREAN, _T("韓文(&B)"));

    // 加入 POPUP 到 MenuBar
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFileMenu, _T("檔案(&F)"));
    AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hLanguageMenu, _T("語言(&L)"));

    // 設定視窗的 Menu
    ::SetMenu(m_hWnd, hMenuBar);
}
void image_process_dialog::LoadDigAccelerators() {
    ACCEL accelTable[] = {
    { FCONTROL | FVIRTKEY, 'S', MENU_FILE_SAVE },
    { FCONTROL | FVIRTKEY, 'O', MENU_FILE_OPEN }
    };
    *accelerators = CreateAcceleratorTable(accelTable, _countof(accelTable));
}
BOOL image_process_dialog::OnInitDialog() {
    CDialogEx::OnInitDialog();
    LoadDigMenu();
    LoadDigAccelerators();
    //setting UI in dialog
    combobox_static->Create(_T("演算法"), WS_CHILD | WS_VISIBLE, CRect(10, 0, 60, 15), this);
    image_process_combo->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, CRect(10, 25, 250, 60), this, ID_COMBOBOX);
    image_process_combo->AddString(_T("Algorithm 1"));
    image_process_combo->AddString(_T("Algorithm 2"));
    image_process_combo->AddString(_T("Algorithm 3"));
    image_process_combo->AddString(_T("Algorithm 4"));

	bmp_text_static->Create(_T("圖像"), WS_CHILD | WS_VISIBLE , CRect(300, 0, 360, 15), this);
    bmp_static->Create(NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, CRect(300, 20, 1100, 660), this);

    file_name_static->Create(_T("檔案名稱"), WS_CHILD | WS_VISIBLE, CRect(10, 65, 70, 80), this);
    
    listbox->Create(WS_CHILD | WS_VISIBLE |LBS_NOTIFY,
        CRect(10, 85, 250, 350), this, ID_LISTBOX, ID_SCROLLBAR);
    //setting listbox height and items per page
    listbox->GetWindowRect(&listbox_rc);
    ScreenToClient(&listbox_rc);
    int itemHeight = listbox->GetItemHeight(0);
    listbox->SetParameter(listbox_rc.Height() / itemHeight, 0, files.size());

    feature_point_static->Create(_T("特徵點"), WS_CHILD | WS_VISIBLE, CRect(10,360, 250,375 ), this);
    listctrl->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | LVS_SINGLESEL | LVS_OWNERDATA,
        CRect(10, 390, 250, 780), this, ID_LISTCTRL);
    listctrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    listctrl->InsertColumn(0, _T("X"), LVCFMT_LEFT, 100);
    listctrl->InsertColumn(1, _T("Y"), LVCFMT_LEFT, 100);
    Server_Socket->m_pClientSocket->SetOnReceive([this]() {
        ::PostMessage(Server_Socket->m_pClientSocket->GetParent()->GetSafeHwnd(), WM_SOCKET_NOTIFY, (WPARAM)Server_Socket->m_pClientSocket->GetFileSize(), 0);
        });
    Server_Socket->Create(port, SOCK_STREAM);
    Server_Socket->Listen();

    shared_memory->Create();

	change_font1->Create(_T("更改字型1"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, 800, 100, 830), this, BT_FONT1);
	change_font2->Create(_T("更改字型2"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(120, 800, 220, 830), this, BT_FONT2);
    return TRUE;
}
void image_process_dialog::OnChooseFile() {
    if (receive) {
        cs.Lock();
        receive = false;
        int sel = listbox->GetCurSel();
        imgBuffer = ReadImageFile(_T("./") + files[sel]);
        // 分析特徵點
        features = DetectFeaturesFromBitmap(LoadBitmapFromMemory(imgBuffer));
        listctrl->SetItemCount(features.size());
        AfxBeginThread([](LPVOID pParam) -> UINT {
            image_process_dialog* dlg = (image_process_dialog*)pParam;
            Sleep(3000);
            // 執行緒內部做耗時處理
            std::vector<char> buffer = ReadImageFile(_T("./") + dlg->files[dlg->listbox->GetCurSel()]);

            // 產生 feature
            HBITMAP hBmp = LoadBitmapFromMemory(buffer);
            std::vector<std::pair<int, int>> detectedFeatures = DetectFeaturesFromBitmap(hBmp);

            // 共享記憶體寫入
            if (dlg->shared_file) {
                CloseHandle(dlg->shared_file);
            }
            HANDLE handle = dlg->shared_memory->Write_Shared_Memory(buffer, buffer.size());

            // 發送資料長度給 Client
            size_t size = buffer.size();
            dlg->Server_Socket->m_pClientSocket->Send(&size, sizeof(size_t));
            return 0;
            }, this);
        cs.Unlock();
    }
    
    cs.Lock();
    //Sleep(5000);
    CPaintDC dc(this);
    int sel = listbox->GetCurSel();
    imgBuffer=ReadImageFile(_T("./") + files[sel]);
    DrawImageBuffer(bmp_static, imgBuffer);
    // 分析特徵點
    features = DetectFeaturesFromBitmap(LoadBitmapFromMemory(imgBuffer));
    listctrl->SetItemCount(features.size());
    if (shared_file) {
        CloseHandle(shared_file);
    }
    shared_file= shared_memory->Write_Shared_Memory(imgBuffer,imgBuffer.size());
    size_t size = imgBuffer.size();
    Server_Socket->m_pClientSocket->Send(&size,sizeof(size_t));
    
}
void image_process_dialog::OnFileSave() {
    
}
void image_process_dialog::OnFileOpen() {
    //read file from disk 
    CFileDialog dlg(TRUE, _T("bmp"), NULL,
        OFN_ALLOWMULTISELECT |OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
        _T("Bitmap Files (*.bmp)|*.bmp||"));
    //add items to listbox
    if (dlg.DoModal() == IDOK) {
        POSITION pos = dlg.GetStartPosition();
        while (pos)
        {
            CString filePath = dlg.GetNextPathName(pos);
            CFileFind finder;
            finder.FindFile(filePath);
            finder.FindNextFile();
            CString fileName = finder.GetFileName();
            files.push_back(fileName);
            listbox->AddString(fileName);
            //only for testing
            /*
            for (int i = 0;i<1000000;++i) {
                CString temp;
				temp.Format(_T("Item %d"), i);
                files.push_back(temp);
            }
            */
        }
    }
    //update listboxitem, only show the items that user can see
    listbox->UpdateListBoxItems(files);
    //update scrollbar, make the bar looks normal
    listbox->UpdateScrollBar();
}
void image_process_dialog::OnChangeLanguage(UINT UI_ID) {
    std::unordered_map<UINT, CString>umap = {
        {MENU_LANGUAGE_TRADITIONAL_CHINESE,_T("zh-tw")},
        {MENU_LANGUAGE_SIMPLIFIED_CHINESE,_T("zh-cn")},
        {MENU_LANGUAGE_ENGLISH,_T("en-us") },
        {MENU_LANGUAGE_JAPANESE,_T("ja-jp") },
        {MENU_LANGUAGE_KOREAN,_T("ko-kr") }
    };
    WriteNewDefaultLanguage(umap[UI_ID],ini_file_path);
}
void image_process_dialog::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult) {
    NMLVDISPINFO* pDispInfo = (NMLVDISPINFO*)pNMHDR;
    int row = pDispInfo->item.iItem;
    int col = pDispInfo->item.iSubItem;
    if (pDispInfo->item.mask & LVIF_TEXT) {
        CString str;
        if (row >= 0 && row < (int)features.size()) {
            if (col == 0) str.Format(_T("%d"), features[row].first);
            else if (col == 1) str.Format(_T("%d"), features[row].second);
        }
        _tcscpy_s(pDispInfo->item.pszText, pDispInfo->item.cchTextMax, str);
    }
    *pResult = 0;
}
void image_process_dialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    if (pScrollBar == listbox->GetScrollBar()) {
        SCROLLINFO si = { 0 };
        si.cbSize = sizeof(si);
        si.fMask = SIF_ALL;
        listbox->GetScrollBar()->GetScrollInfo(&si);
        int m_itemsPerPage=listbox->GetItemsPerPage(),m_scrollPos = listbox->GetScrollPos();
        switch (nSBCode)
        {
        case SB_LINEUP:
            m_scrollPos = max(m_scrollPos - 1, si.nMin);
            break;
        case SB_LINEDOWN:
            m_scrollPos = min(m_scrollPos + 1, si.nMax - m_itemsPerPage + 1);
            break;
        case SB_PAGEUP:
            m_scrollPos = max(m_scrollPos - m_itemsPerPage, si.nMin);
            break;
        case SB_PAGEDOWN:
            m_scrollPos = min(m_scrollPos + m_itemsPerPage, si.nMax - m_itemsPerPage + 1);
            break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            m_scrollPos = min(listbox->GetThumbTrackPos(), files.size() - m_itemsPerPage);
            break;
        }
        //scrollpos changed
        if (m_scrollPos != listbox->GetScrollPos()) {
            //update scrollpos parameter in listbox
            listbox->SetScrollPos(m_scrollPos);
            listbox->UpdateListBoxItems(files);
            listbox->UpdateScrollBar();
        }
    }
    else {
        CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
    }
}
LRESULT image_process_dialog::OnSocketNotify(WPARAM wParam, LPARAM lParam) {
    imgBuffer = shared_memory->Read_Shared_Memory(Server_Socket->m_pClientSocket->GetFileSize());
    DrawImageBuffer(bmp_static, imgBuffer);
    receive = true;
    return TRUE;
}
image_process_dialog::~image_process_dialog() {
    delete pTemplate;
    delete accelerators;
    delete combobox_static;
    delete bmp_text_static;
    delete file_name_static;
    delete feature_point_static;
    delete bmp_static;
    delete image_process_combo;
    delete listctrl;
    delete Server_Socket;
    delete shared_memory;
}
/*
void image_process_dialog::send_WM_COPYDATA() {
    HWND hReceiverWnd = ::FindWindow(NULL, _T("ReceiverWindowTitle")); // 目標視窗標題

    if (hReceiverWnd)
    {
        CString str = _T("Data via WM_COPYDATA");
        COPYDATASTRUCT cds;
        cds.dwData = 1;
        cds.cbData = (str.GetLength() + 1) * sizeof(TCHAR);
        cds.lpData = (void*)str.GetString();

        ::SendMessage(hReceiverWnd, WM_COPYDATA, (WPARAM)NULL, (LPARAM)&cds);
    }
}
*/
void image_process_dialog::OnBnClickedButtonFont1() {
    // 先刪掉舊字型（如果有）
    m_customFont.DeleteObject();

    // 設定新字型
    m_customFont.CreateFont(
        20,                // 高度（像素）
        0, 0, 0,           // 寬度/傾斜/角度
        FW_BOLD,           // 粗體
        FALSE, FALSE, FALSE,   // 非斜體、非底線、非刪除線
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        _T("Microsoft JhengHei")   // 字型名稱
    );

    // 套用到 ListBox
    listbox->SetFont(&m_customFont);
	// 更新 ListBox 的參數
    CRect rc;
    listbox->GetClientRect(&rc);
    int itemHeight = listbox->GetItemHeight(0);
    listbox->SetParameter(rc.Height() / itemHeight, 0, files.size());
    listbox->UpdateListBoxItems(files);
    // 可選：強制重繪
    listbox->MoveWindow(listbox_rc);
    listbox->Invalidate();
}
void image_process_dialog::OnBnClickedButtonFont2() {
    // 先刪掉舊字型（如果有）
    m_customFont.DeleteObject();

    // 設定新字型
    m_customFont.CreateFont(
        30,                // 高度（像素）
        0, 0, 0,           // 寬度/傾斜/角度
        FW_BOLD,           // 粗體
        FALSE, FALSE, FALSE,   // 非斜體、非底線、非刪除線
        ANSI_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        _T("Microsoft JhengHei")   // 字型名稱
    );

    // 套用到 ListBox
    listbox->SetFont(&m_customFont);
	// 更新 ListBox 的參數
    CRect rc;
    listbox->GetClientRect(&rc);
    int itemHeight = listbox->GetItemHeight(0);
    listbox->SetParameter(rc.Height() / itemHeight, 0, files.size());
    listbox->UpdateListBoxItems(files);
    listbox->MoveWindow(listbox_rc);
    // 可選：強制重繪
    listbox->Invalidate();
}
