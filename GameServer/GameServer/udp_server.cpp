#include "udp_server.h"

//#include <winsock.h>
//#include <WS2tcpip.h>

#include <ctime>
#include <string>
#include <iostream>
#include "playerManager.h"
#include "ProtocolManager.h"
#include "../TankGameStuff/cGameBrain.h"

const float PING = 0.2; // 5Hz / 200ms per update / 5 updates per second


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

UDPServer::UDPServer(void)
	: mIsRunning(false)
	, mListenSocket(INVALID_SOCKET)
	, mAcceptSocket(INVALID_SOCKET)
{

	// WinSock vars
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

	// Step #1 Create a socket
	mListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mListenSocket == INVALID_SOCKET) {
		PrintWSAError();
		return;
	}

	// Step #2 Bind our socket
	ReceiverAddr.sin_family = AF_INET;
	ReceiverAddr.sin_port = htons(Port);
	ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	iResult = bind(mListenSocket, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
	if (iResult == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}

	// Set our socket to be nonblocking
	SetNonBlocking(mListenSocket);
	
	// Our server is ready 
	printf("[SERVER] Receiving IP: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
	printf("[SERVER] Receiving Port: %d\n", htons(ReceiverAddr.sin_port));
	printf("[SERVER] Ready to receive a datagram...\n");

	mIsRunning = true;
	//prev = std::clock();
} // end UDPServer

UDPServer::~UDPServer(void)
{
	closesocket(mListenSocket);
	WSACleanup();	// <-- Not necessary if quitting application, Windows will handle this.
}

void UDPServer::SetNonBlocking(SOCKET socket)
{
	ULONG NonBlock = 1;
	int result = ioctlsocket(socket, FIONBIO, &NonBlock);
	if (result == SOCKET_ERROR) {
		PrintWSAError();
		return;
	}
}

void UDPServer::Update(float deltaTime)
{
	if (!mIsRunning) return;

	// TODO: ReadData, SendData
	ReadData();

	timer += deltaTime;

	if (timer > PING)
	{
		BroadcastGameState();
		timer = 0;
	}
}

//void UDPServer::UpdatePlayers(void)
//{
//	for (int i = 0; i < numPlayersConnected; i++) {
//		if (vOnlinePlayers[i].up) vOnlinePlayers[i].y += 10.0f * elapsed_secs;
//		if (vOnlinePlayers[i].down) vOnlinePlayers[i].y -= 10.0f * elapsed_secs;
//		if (vOnlinePlayers[i].right) vOnlinePlayers[i].x += 10.0f * elapsed_secs;
//		if (vOnlinePlayers[i].left) vOnlinePlayers[i].x -= 10.0f * elapsed_secs;
//	}
//}
//
void UDPServer::BroadcastGameState()
{
	cGameBrain* theGameBrain = cGameBrain::getTheGameBrain();
	GameStateMessage* mState = theGameBrain->encodeGameState();
	SendBuffer encodedMessage = writeMessage(mState);

	for (int i = 0; i < vOnlinePlayers.size(); i++) {
		int result = sendto(mListenSocket, (char*)encodedMessage.buffer, encodedMessage.getDataLength(), 0,
			(struct sockaddr*) & (vOnlinePlayers[i]->si_other), sizeof(vOnlinePlayers[i]->si_other));
		printf("Sent to %s %i bytes\n",
			vOnlinePlayers[i]->tankName.c_str(),
			result);
	}
}
//
//
//
//Player* GetPlayerByPort(unsigned short port, struct sockaddr_in si_other)
//{
//	// If a player with this port is already connected, return it
//	for (int i = 0; i < vOnlinePlayers.size(); i++) {
//		if (vOnlinePlayers[i].port == port) return &(vOnlinePlayers[i]);
//	}
//
//	// Otherwise create a new player, and return that one!
//	vOnlinePlayers[numPlayersConnected].port = port;
//	vOnlinePlayers[numPlayersConnected].x = 0.0f;
//	vOnlinePlayers[numPlayersConnected].y = 0.0f;
//	vOnlinePlayers[numPlayersConnected].si_other = si_other;
//	return &(vOnlinePlayers[numPlayersConnected++]);
//}

void UDPServer::ReadData(void)
{
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	char buffer[512];

	int result = recvfrom(mListenSocket, buffer, 512, 0, (struct sockaddr*) & si_other, &slen);
	if (result == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			// printf(".");		// Quick test
			return;
		}
		PrintWSAError();

		// For a TCP connection you would close this socket, and remove it from 
		// your list of connections. For UDP we will clear our serializedMessage, and just
		// ignore this.
		memset(buffer, '\0', 512);
		return;
	}


	unsigned short port = si_other.sin_port;

	std::cout << buffer << std::endl;

	std::string theCoolBuffer = std::string(buffer);
	processMessage(theCoolBuffer, si_other);

	// Send the data back to the client
	// result = sendto(mListenSocket, serializedMessage, 1, 0, (struct sockaddr*) & si_other, sizeof(si_other));
}

void UDPServer::playerDM(char* message, int size, sockaddr_in si_other)
{
	int result = sendto(mListenSocket, message, size, 0,
		(struct sockaddr*) &(si_other), sizeof(si_other));
}

void UDPServer::processMessage(std::string buffer, sockaddr_in addr)
{
	networkPlayer* tempNPlayer = NULL;

	if (buffer == "holi:D")
	{
		tempNPlayer = addPlayer(addr);

		NameMessage* message = new NameMessage();
		message->tank_name = tempNPlayer->tankName;
		SendBuffer serializedMessage = writeMessage(message);

		playerDM((char*)serializedMessage.buffer, serializedMessage.getDataLength(), tempNPlayer->si_other);
	}
}
