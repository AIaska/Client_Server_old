#pragma once

#ifdef _WIN64

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

#endif

class SocketHelper
{
#ifdef _WIN64

public:
	SocketHelper() {};
	virtual ~SocketHelper() {};

protected:
	WSADATA wsaData; // info about the Windows Sockets implementation
	struct addrinfo* result = nullptr;
	struct addrinfo hints;

#endif
};

class ServerSocketHelper : private SocketHelper
{
#ifdef _WIN64

private:
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	int iSendResult;

public:
	ServerSocketHelper();
	~ServerSocketHelper();

	int Initialize();
	int Listen();
	bool Accept();
	int Receive();
	bool Send(char bufData[DEFAULT_BUFLEN], int iResult);
	int Shutdown();

#endif
};

class ClientSocketHelper : private SocketHelper
{
#ifdef _WIN64

private:
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* ptr = nullptr;
	const std::string sendbuf = "this is a test";

public:
	ClientSocketHelper();
	~ClientSocketHelper();

	int Initialize(const char* ip_adr);
	int Connect();
	int Send();
	void Receive();	
	int Shutdown();

#endif
};
