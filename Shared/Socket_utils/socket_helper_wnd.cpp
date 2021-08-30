#include "socket_helper_wnd.h"

CServerSocketHelper::CServerSocketHelper()
{

}

CServerSocketHelper::~CServerSocketHelper()
{
	closesocket(m_clientSocket);
	WSACleanup();
}

int CServerSocketHelper::Init()
{

	try
	{
		// Init Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::cout << "WSAStartup failed with error: %d\n" << iResult << "\n";
			return iResult;
		}

		SecureZeroMemory(&m_hints, sizeof(m_hints));
		m_hints.ai_family = AF_INET;
		m_hints.ai_socktype = SOCK_STREAM;
		m_hints.ai_protocol = IPPROTO_TCP;
		m_hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &m_hints, &m_pResult);
		if (iResult != 0) {
			std::cout << "getaddrinfo failed with error: " << iResult << "\n";
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Listen()
{
	try
	{
		// Create a SOCKET for connecting to server
		m_listenSocket = socket(m_pResult->ai_family, m_pResult->ai_socktype, m_pResult->ai_protocol);
		if (m_listenSocket == INVALID_SOCKET) {
			std::cout << "socket failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(m_pResult);
			WSACleanup();
			return false;
		}

		// Setup the TCP listening socket
		int iResult = bind(m_listenSocket, m_pResult->ai_addr, (int)m_pResult->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			std::cout << "bind failed with error: " << WSAGetLastError() << "\n";
			freeaddrinfo(m_pResult);
			closesocket(m_listenSocket);
			WSACleanup();
			return iResult;
		}

		freeaddrinfo(m_pResult);

		iResult = listen(m_listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			std::cout << "listen failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_listenSocket);
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

bool CServerSocketHelper::Accept()
{
	try
	{
		// Permit an incoming connection attempt on a socket
		m_clientSocket = accept(m_listenSocket, NULL, NULL);
		if (m_clientSocket == INVALID_SOCKET) {
			std::cout << "accept failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_listenSocket);
			WSACleanup();
			return false;
		}

		// No longer need server socket
		closesocket(m_listenSocket); // TO DO check

		return true;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Receive(std::string& received)
{
	try
	{
		// Receive until the peer shuts down the connection
		int iResult = 0;
		char recvbuf[DEFAULT_BUFLEN];
		//do {
			iResult = recv(m_clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
			if (iResult > 0) {
				std::cout << "Bytes received: " << iResult << "\n";
				std::cout << "iResult: " << iResult << '\n';

				received = recvbuf;
				received = received.substr(0, iResult);
			}
			else if (iResult == 0)
				std::cout << "No data received.\n";
			else {
				std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
				closesocket(m_clientSocket); // should we retry rather than close the socket?
				WSACleanup();
				return iResult;
			}

		//} while (iResult > 0);

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Send(const std::string& msg, const int numOfBytes)
{
	try
	{
		int iSendResult = send(m_clientSocket, msg.c_str(), numOfBytes, 0);
		if (iSendResult == SOCKET_ERROR) {
			std::cout << "send failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_clientSocket); // or retry?
			WSACleanup();
			return false;
		}

		printf("Bytes sent: %d\n", iSendResult);
		return iSendResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CServerSocketHelper::Shutdown()
{
	try
	{
		// shutdown the connection since we're done
		int iResult = shutdown(m_clientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "shuwdown failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_clientSocket);
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

//----------------------------------------------------------

CClientSocketHelper::CClientSocketHelper()
{

}

CClientSocketHelper::~CClientSocketHelper()
{
	closesocket(m_connectSocket);
	WSACleanup();
}

int CClientSocketHelper::Init(const char* ip_adr)
{
	try
	{
		// Init Winsock
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::cout << "WSAStartup failed with error: " << iResult << "\n";
			return iResult;
		}

		SecureZeroMemory(&m_hints, sizeof(m_hints));
		m_hints.ai_family = AF_UNSPEC;
		m_hints.ai_socktype = SOCK_STREAM;
		m_hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(ip_adr, DEFAULT_PORT, &m_hints, &m_pResult);
		if (iResult != 0) {
			std::cout << "getaddrinfo failed with error: " << iResult << "\n";
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CClientSocketHelper::Connect()
{
	try
	{
		// Attempt to connect to an address until one succeeds
		int iResult = 0;

		for (m_pAdrInfo = m_pResult; m_pAdrInfo != NULL; m_pAdrInfo = m_pAdrInfo->ai_next) {

			// Create a SOCKET for connecting to server
			m_connectSocket = socket(m_pAdrInfo->ai_family, m_pAdrInfo->ai_socktype,
				m_pAdrInfo->ai_protocol);
			if (m_connectSocket == INVALID_SOCKET) {
				std::cout << "socket failed with error: " << WSAGetLastError() << "\n";
				WSACleanup();
				return iResult;
			}

			// Connect to server.
			iResult = connect(m_connectSocket, m_pAdrInfo->ai_addr, (int)m_pAdrInfo->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(m_connectSocket);
				m_connectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}

		freeaddrinfo(m_pResult);

		if (m_connectSocket == INVALID_SOCKET) {
			std::cout << "Unable to connect to server!\n";
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CClientSocketHelper::Send(const std::string& msg) // to do pass data as parameter
{
	try
	{
		// Send an initial buffer
		int iResult = send(m_connectSocket, msg.c_str(), (int)strlen(msg.c_str()), 0);
		if (iResult == SOCKET_ERROR) {
			std::cout << "send failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_connectSocket); // close or retry?
			WSACleanup();
			return iResult;
		}

		printf("Bytes Sent: %ld\n", iResult);
		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

void CClientSocketHelper::Receive()
{
	try
	{
		// Receive until the peer closes the connection
		int iResult = 0;
		char recvbuf[DEFAULT_BUFLEN];
		//do {
			iResult = recv(m_connectSocket, recvbuf, DEFAULT_BUFLEN, 0);
			if (iResult > 0)
				std::cout << "Bytes received: " << iResult << "\n";
			else if (iResult == 0)
				std::cout << "Connection closed\n";
			else
				std::cout << "recv failed with error: " << WSAGetLastError() << "\n";
		//} while (iResult > 0);
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}

int CClientSocketHelper::Shutdown()
{
	try
	{
		// shutdown the connection since no more data will be sent
		int iResult = shutdown(m_connectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			std::cout << "shutdown failed with error: " << WSAGetLastError() << "\n";
			closesocket(m_connectSocket);
			WSACleanup();
			return iResult;
		}

		return iResult;
	}
	catch (const std::exception& ex)
	{
		std::cout << __FUNCTION__ << "threw exception: " << ex.what() << '\n';
	}
}
