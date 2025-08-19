#include"sockets.h"
CClientSocket::CClientSocket(CWnd* windows_parent) {
    parent = windows_parent;
}
void CClientSocket::OnReceive(int nErrorCode) {
    Receive(&file_size, sizeof(file_size));
    m_onReceive();
    CAsyncSocket::OnReceive(nErrorCode);
}
CClientSocket::~CClientSocket() {
    parent=nullptr;
}
CServerSocket::CServerSocket(CWnd* windows_parent) {
    m_pClientSocket = new CClientSocket(windows_parent);
}
void CServerSocket::OnAccept(int nErrorCode) {
    Accept(*m_pClientSocket);
    CAsyncSocket::OnAccept(nErrorCode);
}
CServerSocket::~CServerSocket() {
    delete m_pClientSocket;
}