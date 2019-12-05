#include "udp_client.h"

#include <bitset>
#include <iostream>

struct Player {
	float x, y;
};

std::vector<Player> mPlayers;

void _PrintWSAError(const char* file, int line)
{
	int WSAErrorCode = WSAGetLastError();
	wchar_t* s = NULL;
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&s, 0, NULL);
	fprintf(stderr, "[WSAError:%d] %S\n", WSAErrorCode, s);
	LocalFree(s);
}

void UDPClient::SetPosition(int id, float& x, float& y)
{
	x = mPlayers[id].x;
	y = mPlayers[id].y;
}

UDPClient::UDPClient(void)
	: mServerSocket(INVALID_SOCKET)
{
	mPlayers.resize(8);

	WSAData		WSAData;
	int			iResult;
	int			Port = 5150;
	SOCKADDR_IN ReceiverAddr;

	// Step #0 Initialize WinSock
	iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (iResult != 0) {
		PrintWSAError();
		return;
	}
}

UDPClient::~UDPClient(void)
{
	closesocket(mServerSocket);
	WSACleanup();
}

void UDPClient::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}
}

void UDPClient::CreateSocket(string ip, int port)
{
	mServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mServerSocket == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}

	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(port);
	si_other.sin_addr.s_addr = inet_addr(ip.c_str());

	SetNonBlocking(mServerSocket);
}

void UDPClient::Update(void)
{
	Recv();
}

void UDPClient::Recv(void)
{
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	char buffer[512];

	int result = recvfrom(mServerSocket, buffer, 512, 0, (struct sockaddr*) & si_other, &slen);
	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return;
		}
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our buffer, and just
		// ignore this.
		memset(buffer, '\0', 512);
		return;
	}

	std::cout << buffer << std::endl;
	// NumPlayers
	// Each player: { x, y }
	//unsigned int numPlayers;
	//memcpy(&numPlayers, &(buffer[0]), sizeof(unsigned int));
	//
	//float x, y;
	//for (int i = 0; i < numPlayers; i++) {
	//	memcpy(&x, &(buffer[i * 8 + 4]), sizeof(float));
	//	memcpy(&y, &(buffer[i * 8 + 8]), sizeof(float));
	//	mPlayers[i].x = x;
	//	mPlayers[i].y = y;
	//}
	//
	////unsigned short port = si_other.sin_port;
	////printf("%d : %hu received %d bytes\n", mServerSocket, port, result);
	//
	//printf("%d players: {", numPlayers);
	//for (int i = 0; i < numPlayers; i++) {
	//	printf(" {x: %.2f, y: %.2f}", mPlayers[i].x, mPlayers[i].y);
	//}
	//printf(" }\n");
}

void UDPClient::Send(char* data, int numBytes)
{
	int result = sendto(mServerSocket, data, numBytes, 0,
		(struct sockaddr*) & si_other, sizeof(si_other));

	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) return;
		PrintWSAError();
		return;
	}

	if (result == 0) {
		printf("Disconnected...\n");
		return;
	}

	// printf("Number of bytes sent: %d\n", result);
}