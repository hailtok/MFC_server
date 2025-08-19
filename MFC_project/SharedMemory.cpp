#include"SharedMemory.h"
Shared_Memory::Shared_Memory(){
    session = _T("Local");
    share_memory_name = _T("ImageSharedMemory");
    max_shared_memory_size = 1024 * 1024 * 10;
}
void Shared_Memory::Create() {
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, max_shared_memory_size, share_memory_name);
}
void Shared_Memory::Create(CString session_range,CString shared_memory_name,size_t size) {
    session = session_range;
    share_memory_name = shared_memory_name;
    max_shared_memory_size = size;
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, max_shared_memory_size, share_memory_name);
}
std::vector<char> Shared_Memory::Read_Shared_Memory(const size_t& size) {
    // 開啟已存在的共享記憶體
    HANDLE hMapFile = OpenFileMapping(
        PAGE_READWRITE,
        FALSE,
        session + _T("\\") + share_memory_name);
    if (hMapFile == NULL) {
        return {};
    }
    // 對映到自己的虛擬記憶體空間
    LPVOID pBuf = MapViewOfFile(
        hMapFile,
        PAGE_READWRITE, // 權限
        0, 0,
        size);
    if (pBuf == NULL) {
        CloseHandle(hMapFile);
        return {};
    }
    std::vector<char> buffer(size);
    memcpy(buffer.data(), pBuf, size);
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);
    return buffer;
}
std::vector<char> ReadImageFile(const CString& file_path_from_disk) {
    std::ifstream file(file_path_from_disk, std::ios::binary | std::ios::ate);
    if (!file) {
        return {};
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        return {};
    }
    return buffer;
}
void DrawImageBuffer(CWnd* pWnd, const std::vector<char>& buffer) {
    if (buffer.empty()) return;
    CImage image;
    HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, buffer.size());
    memcpy(hMem, buffer.data(), buffer.size());
    IStream* pStream = nullptr;
    CreateStreamOnHGlobal(hMem, TRUE, &pStream);
    image.Load(pStream);
    // 取得目標控制項的大小
    CRect rc;
    pWnd->GetClientRect(&rc);
    CDC* pDC = pWnd->GetDC();
    // 繪製並自動縮放到控制項大小
    image.Draw(pDC->GetSafeHdc(), rc);
    pWnd->ReleaseDC(pDC);
    pStream->Release();
}
