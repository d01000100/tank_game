#pragma once

#include <WS2tcpip.h>

#include <ctime>
#include <string>
#include <iostream>

using namespace std;

struct networkPlayer {
	struct sockaddr_in si_other;
	string tankName;
};

void processMessage(string buffer, sockaddr_in addr);
networkPlayer* addPlayer(sockaddr_in addr);
