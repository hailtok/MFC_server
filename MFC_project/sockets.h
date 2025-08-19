#pragma once
#include<afxsock.h>
#include<functional>
//使用socket傳輸Shared_memory裡的圖片大小
//由於這部分的設定基本上只處理一對一的情況，所以有許多例外，像是中間斷線等都沒有處理
class CClientSocket : public CAsyncSocket {
public:
	CClientSocket(CWnd*);
	size_t GetFileSize() const { return file_size; }
	CWnd* GetParent() const { return parent; }
	using ReceiveCallback = std::function<void()>;
	void SetOnReceive(ReceiveCallback cb) { m_onReceive = cb; }
	virtual ~CClientSocket();
protected:
	virtual void OnReceive(int nErrorCode) override;
private:
	CWnd* parent = nullptr;
	size_t file_size = 0;
	ReceiveCallback m_onReceive;
};
class CServerSocket : public CAsyncSocket {
public:
	CServerSocket(CWnd*);
	CClientSocket* m_pClientSocket = nullptr;
	virtual void OnAccept(int nErrorCode)override;
	virtual ~CServerSocket();
};


