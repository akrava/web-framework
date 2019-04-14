#if defined(_WIN32) || defined(_Win64)

#include <socket_windows_api.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <runtime_exception.h>
#define DEFAULT_BUFLEN 512
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

SocketWindowsAPI::~SocketWindowsAPI() {
	int result = shutdown(serverSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cerr << "shutdown failed: " << WSAGetLastError() << endl;
		closesocket(serverSocket);
		WSACleanup();
	}
	closesocket(serverSocket);
	WSACleanup();
}

void SocketWindowsAPI::init(string ip, int port, bool isIpv6) {
	serverSocket = INVALID_SOCKET;
	clientSocket = INVALID_SOCKET;
	WSADATA wsaData;
	addrinfo *addr = NULL;
	addrinfo hints;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cerr << "WSAStartup failed with error:" << result << endl;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	char port_text[DEFAULT_BUFLEN];
	_itoa_s(port, port_text, 10);
	result = getaddrinfo(ip.c_str(), port_text, &hints, &addr);

	if (result != 0) {
		cerr << "getaddrinfo failed: " << result << "\n";
		WSACleanup();
	}

	// Create a SOCKET for connecting to server
	serverSocket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(addr);
		WSACleanup();
		
	}

	// Setup the TCP listening socket
	result = bind(serverSocket, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(addr);
		closesocket(serverSocket);
		WSACleanup();
		
	}

	result = listen(serverSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(serverSocket);
		WSACleanup();
		
	}
}

std::string SocketWindowsAPI::getData() {
	clientSocket = INVALID_SOCKET;

	// Accept a client socket
	clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "accept failed: " << WSAGetLastError() << "\n";
	}

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	if (clientSocket == -1) {
		throw RuntimeException(string("A client was not accepted: ") + strerror(errno));
	}
	//  cout << "A client is connected from " << inet_ntoa(sender.sin_addr) << ":"
	//   	<< to_string(ntohs(sender.sin_port)) << endl;
	string data;
	auto * buffer = new char[DEFAULT_BUFLEN + 1];
	size_t numRead = recv(clientSocket, buffer, DEFAULT_BUFLEN, 0);
	if (numRead < 0) {
		perror("The client was not read from");
		closesocket(clientSocket);
		delete[] buffer;
		return string();
	}
	buffer[numRead] = 0;
	data += buffer;
	size_t length = 0;
	size_t cur = 0;
	size_t startKey = data.find("Content-Length: ");
	size_t endValue = data.find("\r\n", startKey + 16);
	size_t startBody = data.find("\r\n\r\n");
	if (startKey != string::npos && endValue != string::npos && startBody != string::npos) {
		try {
			string length_str = data.substr(startKey + 16, endValue - startKey - 16);
			length = stoul(length_str);
			cur = data.length() - startBody - 3;
		}
		catch (out_of_range & err) {
		}
		catch (invalid_argument & err) {
		}
	}
	while (cur < length) {
		size_t length_cur = recv(clientSocket, buffer, 1024, 0);
		if (numRead < 1) {
			perror("The client was not read from");
			closesocket(clientSocket);
			delete[] buffer;
			return std::string();
		}
		buffer[length_cur] = 0;
		cur += length_cur;
		data += buffer;
	}
	delete[] buffer;
	return data;
}

void SocketWindowsAPI::receiveData(const std::string & data) {
	int result = send(clientSocket, data.c_str(), data.length(), 0);
	if (result == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(clientSocket);
	}
	printf("Bytes sent: %d\n", result);
}

std::string SocketWindowsAPI::getIpFromDomain(std::string &domain, bool isHttps, bool *IPv6) {
	return "";
}

#endif // defined(_WIN32) || defined(_Win64)