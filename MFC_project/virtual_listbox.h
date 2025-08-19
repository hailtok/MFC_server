#pragma once
#include <afxwin.h>
#include <vector>
//����virtual listbox�A�D�n��CListBox�MSCrollBar�զ��A�D�n�Ω���ܤj�q��ơA�åB�i�H�z�L�u�ʱ��Ӭd�ݤ��P������
class  virtual_listbox : public CListBox {
private:
    int m_itemsPerPage; // �i�����ؼơA�̵e�����ץi��
    int m_scrollPos;//SCrollBar��m�A�q0�}�l
    int item_count;//�`���ؼ�
    CScrollBar* scrollbar;//���F�i�H���@�몺Clistbox�ϥΡA�]��pointer
protected:
public:
    //constructor
    virtual_listbox() {
        m_itemsPerPage = 0;
        m_scrollPos = 0;
        item_count = 0;
        scrollbar = nullptr;
    }
    //�p�G�n�ϥΤ@�몺Clistbox�n�ϥγo��function
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
        return CListBox::Create(dwStyle, rect, pParentWnd, nID);
    }
    //�p�G�n�ϥ�virtualbox�n�ϥγo��function�A�åB�ݳ]�wscrollbar_id
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, UINT scrollbar_id) {
        CListBox::Create(dwStyle, rect, pParentWnd, nID);
        CRect rcListBox;
        GetWindowRect(&rcListBox);
        GetParent()->ScreenToClient(&rcListBox);
        int scrollbarWidth = 15;
        CRect rcScrollBar(
            rcListBox.right, rcListBox.top,
            rcListBox.right + scrollbarWidth, rcListBox.bottom
        );
        scrollbar = new CScrollBar();
        scrollbar->Create(WS_CHILD | WS_VISIBLE | SBS_VERT, rcScrollBar, pParentWnd, scrollbar_id);
        return TRUE;
    }
    //destructor
    virtual ~virtual_listbox() {
        delete scrollbar;
        CListBox::~CListBox();
    }
    CScrollBar* GetScrollBar() {
        return this->scrollbar;
    }
    //��o�{�b��ScrollBar��m
    int GetScrollPos() const { return m_scrollPos; }
    //��o�{�b���i�����ؼ�
    int GetItemsPerPage() const { return m_itemsPerPage; }

    int GetThumbTrackPos() {
        SCROLLINFO si = { sizeof(si), SIF_TRACKPOS };
        if (GetScrollBar()->GetScrollInfo(&si)) return si.nTrackPos;
        return 0;
    }
    //�]�w�i�����ؼơAScrollBar��m�A�`���ؼ�
    void SetParameter(int itemsPerPage, int scrollPos, int total_count) {
        m_itemsPerPage = itemsPerPage;
        m_scrollPos = scrollPos;
        item_count = total_count;
    }
    //�]�wScrollBar��m
    void SetScrollPos(int pos) { m_scrollPos = pos; }
    //��sListBox������T
    void UpdateListBoxItems(std::vector<CString>& items) {
        if (scrollbar != nullptr) {
            item_count = items.size();
            this->SetRedraw(FALSE);
            //clean listbox
            this->ResetContent();
            //setting the items that users can see
            int endIndex = min(m_scrollPos + m_itemsPerPage, item_count);
            for (int i = m_scrollPos; i < endIndex; ++i)
            {
                this->AddString(items[i]);
            }
            //make listbox show the first item
            this->SetTopIndex(0);
            this->SetRedraw(TRUE);
        }
    }
    //��sScrollBar��m�A�åB��sScrollBar�����
    void UpdateScrollBar() {
        if (scrollbar) {
            SCROLLINFO si = { 0 };
            si.cbSize = sizeof(si);
            si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
            si.nMin = 0;
            si.nMax = item_count - 1;
            si.nPage = m_itemsPerPage;
            si.nPos = m_scrollPos;
            scrollbar->SetScrollInfo(&si, TRUE);
        }
    }
};