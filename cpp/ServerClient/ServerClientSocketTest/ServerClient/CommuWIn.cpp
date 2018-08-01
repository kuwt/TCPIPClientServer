#pragma comment(lib, "Ws2_32.lib")

#include "CommuWin.h"

#define OKAY (1)
#define ERROR (-1)
#define MAX_MSG_SIZE (500)

class NetworkServices
{
public:
	static int sendMessage(SOCKET curSocket, const char* message, int messageSize);
	static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);
};
int NetworkServices::sendMessage(SOCKET curSocket, const char* message, int messageSize)
{
	int status = send(curSocket, message, messageSize, 0);
	return status;
}
int NetworkServices::receiveMessage(SOCKET curSocket, char * buffer, int bufSize)
{
	int status = OKAY;
	int socketstatus = recv(curSocket, buffer, bufSize, 0);
	if (socketstatus == -1)
	{
		status = ERROR;
		goto FUNC_EXIT;
	}
FUNC_EXIT:
	return status;
}
CommuWin::CommuWin(int InPort, int OutPort)
{
	WSAData wsaData;
	WORD DLLVersion;
	DLLVersion = MAKEWORD(2, 1);
	int r = WSAStartup(DLLVersion, &wsaData);
	///////////////////////////////////////////////////////////////////////
	m_outSocket = socket(AF_INET, SOCK_STREAM, NULL);
	m_outAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_outAddr.sin_family = AF_INET;
	m_outAddr.sin_port = htons(OutPort);
	m_inSocket = socket(AF_INET, SOCK_STREAM, NULL);
	m_inAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_inAddr.sin_family = AF_INET;
	m_inAddr.sin_port = htons(InPort);
}
CommuWin::~CommuWin()
{	
}


int CommuWin::connectTo()
{
	int status = OKAY;
	printf("connect to");
	printf("\n");
	m_sendThread = std::thread(&CommuWin::SetupSendEnd,this);
	m_receiveThread = std::thread(&CommuWin::SetupRecvEnd,this);
	return status;
}
int CommuWin::SetupSendEnd()
{
	int status = OKAY;
	SOCKET sListen; 
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&m_outAddr, sizeof(m_outAddr));
	listen(sListen, SOMAXCONN);

	m_outSocket = accept(sListen, NULL, NULL);

	if (m_outSocket != INVALID_SOCKET)
	{
		SetSendStatus(true);
		printf("accepted\n");
	}

	return status;
}
int CommuWin::SetupRecvEnd()
{
	int status = OKAY;
	int connectSucceed = 0;
	do
	{
		Sleep(200);
		connectSucceed = connect(m_inSocket, (SOCKADDR*)&m_inAddr, sizeof(m_inAddr));
	} while (connectSucceed == SOCKET_ERROR);
	SetRecvStatus(true);
	printf("connected\n");

	if (GetRecvStatus())
	{
		ReceiveMsgFrom();
	}

	return status;
}
int CommuWin::sendMsg(std::string msg)
{
	int status = OKAY;
	if (GetSendStatus())
	{
		std::string tmpmsg = msg;
		tmpmsg.resize(MAX_MSG_SIZE);
		m_sendThread.detach();
		m_sendThread = std::thread(
			&CommuWin::sendMsgTo,
			this,
			tmpmsg);
	}
	return status;
}
int CommuWin::sendMsgTo(std::string msg)
{
	int status = OKAY;
	std::string s = msg;
	status = NetworkServices::sendMessage(m_outSocket, s.c_str(), (int)msg.size());
	return status;
}
/*
int CommuWin::StartReceiveMsg()
{
	int status = OKAY;
	if (GetRecvStatus())
	{
		m_receiveThread.detach();
		m_receiveThread = std::thread(&CommuWin::ReceiveMsgFrom,this);
	}
	return status;
}*/
int CommuWin::ReceiveMsgFrom()
{
	int status = OKAY;
	while (true)
	{
		char message[MAX_MSG_SIZE];
		ZeroMemory(message, MAX_MSG_SIZE);
		status = NetworkServices::receiveMessage(m_inSocket, message, sizeof(message));
		if (status == ERROR)
		{
			goto FUNC_EXIT;
		}
		printf(message);
		printf("\n");
	}
FUNC_EXIT:
	return status;
}

void CommuWin::SetRecvStatus(bool ready)
{
	std::lock_guard<std::mutex> lock(m_accessMutexReceive);
	m_IsRecvReady = ready;
}
void CommuWin::SetSendStatus(bool ready)
{
	std::lock_guard<std::mutex> lock(m_accessMutexSend);
	m_IsSendReady = ready;
}
bool CommuWin::GetRecvStatus()
{
	std::lock_guard<std::mutex> lock(m_accessMutexReceive);
	return m_IsRecvReady;
}
bool CommuWin::GetSendStatus()
{
	std::lock_guard<std::mutex> lock(m_accessMutexSend);
	return m_IsSendReady;
}


int CommuWin::registerListener(const CommuWinListener *pListener)
{

	return OKAY;

}
int CommuWin::removeListener(const CommuWinListener *pListener)
{

	return OKAY;
}
int CommuWin::notifyListener()
{
	return OKAY;

}