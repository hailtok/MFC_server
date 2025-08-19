#pragma once
#include<afxsock.h>
#include<functional>
//�ϥ�socket�ǿ�Shared_memory�̪��Ϥ��j�p
//�ѩ�o�������]�w�򥻤W�u�B�z�@��@�����p�A�ҥH���\�h�ҥ~�A���O�����_�u�����S���B�z
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


