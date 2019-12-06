#pragma once

#include <WS2tcpip.h>

#include <ctime>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

struct networkPlayer {
	struct sockaddr_in si_other;
	string tankName;
};

extern std::vector<networkPlayer*> vOnlinePlayers;

networkPlayer* addPlayer(sockaddr_in addr);
networkPlayer* findPlayer(sockaddr_in addr);