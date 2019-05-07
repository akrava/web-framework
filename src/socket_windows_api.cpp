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

static void copyToString(std::string & data, const char * input, int len);
static size_t getContentLength(const std::string & data);
static size_t getCurrentLength(const std::string & data);
static bool endsWithTwoNewLines(const std::string & data);

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
	clientSocket = accept(serverSocket, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "accept failed: " << WSAGetLastError() << "\n";
	}
	char recvbuf[DEFAULT_BUFLEN];
	if (clientSocket == -1) {
		throw RuntimeException(string("A client was not accepted: ") + strerror(errno));
	}
	string data = "";
	size_t length = string::npos;
	size_t cur = 0;
	do {
		size_t numRead = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);
		if (numRead == 0) {
			break;
		} else if (numRead < 0) {
			perror("The client was not read from");
			closesocket(clientSocket);
			return string();
		}
		copyToString(data, recvbuf, numRead);
		if (length == string::npos) {
			length = getContentLength(data);
		}
		if (cur == 0) {
			cur = getCurrentLength(data);
		} else {
			cur += numRead;
		}
		if (cur == 0 && length == string::npos && endsWithTwoNewLines(data)) {
			break;
		}
	} while (cur < length);
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

void copyToString(std::string & data, const char * input, int len) {
	string temp;
	temp.assign(input, len);
	data += temp;
}

size_t getContentLength(const std::string & data) {
	size_t startKey = data.find("Content-Length: ");
	size_t endValue = data.find("\r\n", startKey + 16);
	if (startKey != string::npos && endValue != string::npos) {
		try {
			string length_str = data.substr(startKey + 16, endValue - startKey - 16);
			return stoul(length_str);
		}
		catch (out_of_range & err) {
		}
		catch (invalid_argument & err) {
		}
	}
	return string::npos;
}

size_t getCurrentLength(const std::string & data) {
	size_t startBody = data.find("\r\n\r\n");
	if (startBody != string::npos) {
		return data.length() - startBody - 4;
	}
	return 0;
}

bool endsWithTwoNewLines(const std::string & data) {
	size_t position = data.find("\r\n\r\n");
	if (position == string::npos) {
		return false;
	}
	return position + 4 == data.length();
}

#endif // defined(_WIN32) || defined(_Win64)