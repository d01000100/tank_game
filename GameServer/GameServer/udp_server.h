#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <vector>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

using std::vector;

void _PrintWSAError(const char* file, int line);
#define PrintWSAError() _PrintWSAError(__FILE__, __LINE__)

class UDPServer
{
public:
	UDPServer(void);
	~UDPServer(void);

	void Update(float deltaTime);
private:
	void SetNonBlocking(SOCKET socket);
	void ReadData(void);
	void BroadcastGameState(void);
	void playerDM(char* theCoolBuffer, int size, sockaddr_in si_other);
	void processMessage(char* buffer, sockaddr_in addr);


	bool mIsRunning;

	FD_SET mReadSet;
	float timer;
	SOCKET mAcceptSocket;
	SOCKET mListenSocket;
};
