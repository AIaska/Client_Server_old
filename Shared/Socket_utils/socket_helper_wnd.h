#ifndef _SOCKET_HELPER_WND_H_INCLUDED_
#define _SOCKET_HELPER_WND_H_INCLUDED_

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <iostream>

#include "../Logger/Logger.h"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512 // will be read from config
#define DEFAULT_PORT "27015"

class CSocketHelper
{
public:
	CSocketHelper() {};
	virtual ~CSocketHelper() {};

protected:
	WSADATA wsaData; // info about the Windows Sockets implementation
	struct addrinfo* m_pResult = nullptr;
	struct addrinfo m_hints;
};

class CServerSocketHelper : private CSocketHelper
{
public:
	CServerSocketHelper();
	~CServerSocketHelper();

	int Init();
	int Listen();
	bool Accept();
	int Receive();
	bool Send(char bufData[DEFAULT_BUFLEN], int iResult);
	int Shutdown();

private:
	SOCKET m_listenSocket = INVALID_SOCKET;
	SOCKET m_clientSocket = INVALID_SOCKET;
	int m_iSendResult;
};

class CClientSocketHelper : private CSocketHelper
{
public:
	CClientSocketHelper();
	~CClientSocketHelper();

	int Init(const char* ip_adr);
	int Connect();
	int Send();
	void Receive();	
	int Shutdown();

private:
	SOCKET m_connectSocket = INVALID_SOCKET;
	struct addrinfo* m_pAdrInfo = nullptr;
	const std::string m_sSendBuf = "this is a test";
};

#endif // _SOCKET_HELPER_WND_H_INCLUDED_
