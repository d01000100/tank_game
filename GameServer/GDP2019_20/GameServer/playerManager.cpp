#include "playerManager.h"
#include "../TankGameStuff/cGameBrain.h"

unsigned int maxPlayers = 4;
std::vector<networkPlayer*> mPlayers;

void processMessage(string buffer, sockaddr_in addr)
{
	networkPlayer* tempNPlayer = NULL;
	if (buffer == "holi:D")
	{
		tempNPlayer = addPlayer(addr);
	}
}

networkPlayer* addPlayer(sockaddr_in addr)
{
	cGameBrain* pTheGameBrain = cGameBrain::getTheGameBrain();
	networkPlayer* nwPlayer = new networkPlayer();
	nwPlayer->si_other = addr;
	nwPlayer->tankName = pTheGameBrain->addTank();
	mPlayers.push_back(nwPlayer);
	return nwPlayer;
}

