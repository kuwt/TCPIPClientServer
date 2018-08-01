#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

class CommuWinListener
{
public:
	virtual void SignalConnectionCompleted() = 0;
	virtual void MessageUpdate(std::string msg) = 0;
};

class CommuWin
{
private:
	std::mutex m_accessMutexSend;
	std::mutex m_accessMutexReceive;
	std::thread m_sendThread;
	std::thread m_receiveThread;

	bool m_IsSendReady = false;
	bool m_IsRecvReady = false;

	SOCKET m_outSocket;
	SOCKADDR_IN m_outAddr;
	SOCKET m_inSocket;
	SOCKADDR_IN m_inAddr;

	std::vector< CommuWinListener * > vpListeners;
public:
	CommuWin(int InPort, int OutPort);
	~CommuWin();
	int connectTo();
	int sendMsg(std::string msg);
	//int StartReceiveMsg();

	int registerListener(const CommuWinListener * pListener);
	int removeListener(const CommuWinListener * pListener);
	int notifyListener();

private:
	bool GetRecvStatus();
	bool GetSendStatus();
	void SetRecvStatus(bool ready);
	void SetSendStatus(bool ready);
	int SetupRecvEnd();
	int SetupSendEnd();
	int sendMsgTo(std::string msg);
	int ReceiveMsgFrom();
};


