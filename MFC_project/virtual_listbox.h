#pragma once
#include <afxwin.h>
#include <vector>
//虛擬virtual listbox，主要由CListBox和SCrollBar組成，主要用於顯示大量資料，並且可以透過滾動條來查看不同的項目
class  virtual_listbox : public CListBox {
private:
    int m_itemsPerPage; // 可見項目數，依畫面高度可調
    int m_scrollPos;//SCrollBar位置，從0開始
    int item_count;//總項目數
    CScrollBar* scrollbar;//為了可以像一般的Clistbox使用，設為pointer
protected:
public:
    //constructor
    virtual_listbox() {
        m_itemsPerPage = 0;
        m_scrollPos = 0;
        item_count = 0;
        scrollbar = nullptr;
    }
    //如果要使用一般的Clistbox要使用這個function
    BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
        return CListBox::Create(dwStyle, rect, pParentWnd, nID);
    }
    //如果要使用virtualbox要使用這個function，並且需設定scrollbar_id
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
    //獲得現在的ScrollBar位置
    int GetScrollPos() const { return m_scrollPos; }
    //獲得現在的可見項目數
    int GetItemsPerPage() const { return m_itemsPerPage; }

    int GetThumbTrackPos() {
        SCROLLINFO si = { sizeof(si), SIF_TRACKPOS };
        if (GetScrollBar()->GetScrollInfo(&si)) return si.nTrackPos;
        return 0;
    }
    //設定可見項目數，ScrollBar位置，總項目數
    void SetParameter(int itemsPerPage, int scrollPos, int total_count) {
        m_itemsPerPage = itemsPerPage;
        m_scrollPos = scrollPos;
        item_count = total_count;
    }
    //設定ScrollBar位置
    void SetScrollPos(int pos) { m_scrollPos = pos; }
    //更新ListBox內的資訊
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
    //更新ScrollBar位置，並且更新ScrollBar的顯示
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