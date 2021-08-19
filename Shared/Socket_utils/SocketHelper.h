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

#define DEFAULT_BUFLEN 512 // to do move to ctor-s
#define DEFAULT_PORT "27015"

#endif

// TO DO comment on usage (init -> listen -> connect etc)

class SocketHelper
{
#ifdef _WIN64



protected:
	WSADATA wsaData; // del structure contains information about the Windows Sockets implementation
	int iResult = 0;
	struct addrinfo* result = nullptr; // to do smart ptrs everywhere
	struct addrinfo hints;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN; // to do change type

public:



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

	bool Initialize();
	bool Listen();
	bool Accept();
	bool Receive();
	bool Shutdown();

#endif
};

class ClientSocketHelper : private SocketHelper
{
#ifdef _WIN64

private:
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo* ptr = nullptr;
	const char* sendbuf = "this is a test";

public:
	ClientSocketHelper();
	~ClientSocketHelper();

	bool Initialize(const char* ip_adr);
	bool Connect();
	bool Send();
	bool Shutdown();
	void Receive();

#endif
};