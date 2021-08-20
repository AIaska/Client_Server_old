#include "SocketHelper.h"

#ifdef _WIN64

ServerSocketHelper::ServerSocketHelper()
{

}

ServerSocketHelper::~ServerSocketHelper()
{
	closesocket(ClientSocket);
	WSACleanup();
}

int ServerSocketHelper::Initialize()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error: %d\n" << iResult << "\n";
		return iResult;
	}

	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed with error: " << iResult << "\n";
		WSACleanup();
		return iResult;
	}

	return iResult;
}

int ServerSocketHelper::Listen()
{
	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cout << "socket failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Setup the TCP listening socket
	int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return iResult;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		std::cout << "listen failed with error: " << WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return iResult;
	}

	return iResult;
}

bool ServerSocketHelper::Accept()
{
	// Permit an incoming connection attempt on a socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "accept failed with error: " << WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}

	// No longer need server socket
	closesocket(ListenSocket); // TO DO check

	return true;
}

int ServerSocketHelper::Receive()
{
	// Receive until the peer shuts down the connection
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	do {
		iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0) {
			std::cout << "Bytes received: " << iResult << "\n";

			// Echo the buffer back to the sender
			Send(recvbuf, iResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
			closesocket(ClientSocket);
			WSACleanup();
			return iResult;
		}

	} while (iResult > 0);

	return iResult;
}

bool ServerSocketHelper::Send(char bufData[DEFAULT_BUFLEN], int iResult)
{
	iSendResult = send(ClientSocket, bufData, iResult, 0);
	if (iSendResult == SOCKET_ERROR) {
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}

	printf("Bytes sent: %d\n", iSendResult);
	return true;
}

int ServerSocketHelper::Shutdown()
{
	// shutdown the connection since we're done
	int iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cout << "shuwdown failed with error: " << WSAGetLastError() << "\n";
		closesocket(ClientSocket);
		WSACleanup();
		return iResult;
	}

	return iResult;
}

//----------------------------------------------------------

ClientSocketHelper::ClientSocketHelper()
{

}

ClientSocketHelper::~ClientSocketHelper()
{
	closesocket(ConnectSocket);
	WSACleanup();
}

int ClientSocketHelper::Initialize(const char* ip_adr)
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error: " << iResult << "\n";
		return iResult;
	}

	SecureZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(ip_adr, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed with error: " << iResult << "\n";
		WSACleanup();
		return iResult;
	}

	return iResult;
}

int ClientSocketHelper::Connect()
{
	// Attempt to connect to an address until one succeeds
	int iResult = 0;

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			std::cout << "socket failed with error: " << WSAGetLastError() << "\n";
			WSACleanup();
			return iResult;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		std::cout << "Unable to connect to server!\n";
		WSACleanup();
		return iResult;
	}

	return iResult;
}

int ClientSocketHelper::Send() // to do pass data as parameter
{
	// Send an initial buffer
	int iResult = send(ConnectSocket, sendbuf.c_str(), (int)strlen(sendbuf.c_str()), 0);
	if (iResult == SOCKET_ERROR) {
		std::cout << "send failed with error: " << WSAGetLastError() << "\n";
		closesocket(ConnectSocket);
		WSACleanup();
		return iResult;
	}

	printf("Bytes Sent: %ld\n", iResult);
	return iResult;
}

void ClientSocketHelper::Receive()
{
	// Receive until the peer closes the connection
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	do {
		iResult = recv(ConnectSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (iResult > 0)
			std::cout << "Bytes received: " << iResult << "\n";
		else if (iResult == 0)
			std::cout << "Connection closed\n";
		else
			std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
	} while (iResult > 0);
}

int ClientSocketHelper::Shutdown()
{
	// shutdown the connection since no more data will be sent
	int iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cout << "shutdown failed with error: " << WSAGetLastError() << "\n";
		closesocket(ConnectSocket);
		WSACleanup();
		return iResult;
	}

	return iResult;
}

#endif
