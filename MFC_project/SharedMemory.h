#pragma once
#include <afxwin.h>
#include<windows.h>
#include<vector>
#include<fstream>
#include <atlimage.h>
//�D�n�ΨӳB�zShared_memory����Ū�g���
std::vector<char> ReadImageFile(const CString& file_path_from_disk);
void DrawImageBuffer(CWnd*, const std::vector<char>&);
class Shared_Memory {
private:
    CString session;
    CString share_memory_name;
    size_t max_shared_memory_size;
public:
    Shared_Memory();
    void Create();
    void Create(CString, CString, size_t);
    //�U�����function�򥻬O�@�˪��A�D�n�O�ϥ�template�ӳB�z���P������input�A
    // �\�γ��O�g�J�@�ɰO����
    template<typename T>
    HANDLE Write_Shared_Memory(const T& input, const size_t& input_size) {
        // �إߦ@�ɰO����
        HANDLE hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, input_size, session + _T("\\") + share_memory_name);
        if (!hMapFile) {
            return NULL;
        }
        LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, input_size);
        if (!pBuf) {
            CloseHandle(hMapFile);
            return NULL;
        }
        memcpy(pBuf, input, input_size);
        UnmapViewOfFile(pBuf);
        return hMapFile;
    }
    template<typename T>
    HANDLE Write_Shared_Memory(const std::vector<T>& input, const size_t& input_size) {
        // �إߦ@�ɰO����
        HANDLE hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, input_size, session + _T("\\") + share_memory_name);
        if (!hMapFile) return NULL;

        LPVOID pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, input_size);
        if (!pBuf) {
            CloseHandle(hMapFile);
            return NULL;
        }
        memcpy(pBuf, input.data(), input_size);
        UnmapViewOfFile(pBuf);
        return hMapFile;
    }
    //Ū���@�ɰO���骺��ơAsize�O�nŪ�����j�p
    std::vector<char> Read_Shared_Memory(const size_t& size);
    //destructor
    virtual ~Shared_Memory() {};
};