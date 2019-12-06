#include "ProtocolManager.h"

int main()
{
	SendBuffer sendBuf = writeMessage(new OliMessage());

	RecieveBuffer recvBuf;
	recvBuf.buffer = sendBuf.buffer;
	recvBuf.setDataRecieved(1024);

	OliMessage* readOli = (OliMessage*)readMessage(recvBuf);

	printf("read Oli's type: %s\n", messageTypeString(readOli->type));

	return 0;
}

//SendBuffer sendBuf = writeMessage(new OliMessage());
//RecieveBuffer recvBuf;
//recvBuf.buffer = sendBuf.buffer;
//recvBuf.setDataRecieved(1024);
//
//OliMessage* readOli = (OliMessage*)readMessage(recvBuf);
//
//printf("read Oli's type: %s\n", messageTypeString(readOli->type));
//
//NameMessage* nameM = new NameMessage();
//nameM->tank_name = "el tanque chido";
//sendBuf = writeMessage(nameM);
//recvBuf.buffer = sendBuf.buffer;
//recvBuf.setDataRecieved(1024);
//
//NameMessage* readName = (NameMessage*)readMessage(recvBuf);
//
//printf("read tank name: %s\n", readName->tank_name.c_str());
//
//UserInputMessage* UserInputM = new UserInputMessage();
//UserInputM->W = 1;
//UserInputM->A = 0;
//UserInputM->S = 0;
//UserInputM->D = 1;
//UserInputM->Space = 1;
//sendBuf = writeMessage(UserInputM);
//recvBuf.buffer = sendBuf.buffer;
//recvBuf.setDataRecieved(1024);
//
//UserInputMessage* readUserInput = (UserInputMessage*)readMessage(recvBuf);
//
//printf("read A: %i, Space: %i\n", readUserInput->A, readUserInput->Space);
//
//GameStateMessage* GameStateM = new GameStateMessage();
//sMessageTank tank1, tank2;
//tank1.name = "hey";
//tank2.name = "sup";
//tank1.fireCooldown = 2;
//tank1.xP = 1;
//tank1.yP = 1;
//tank1.zP = 1;
//tank1.xV = 1;
//tank1.yV = 1;
//tank1.zV = 1;
//tank1.degrees = 275;
//sMessageBullet bullet;
//bullet.shooter = "hey";
//std::vector<sMessageTank*> tanks;
//tanks.push_back(&tank1);
//tanks.push_back(&tank2);
//std::vector<sMessageBullet*> bullets;
//bullets.push_back(&bullet);
//GameStateM->bullets = bullets;
//GameStateM->tanks = tanks;
//sendBuf = writeMessage(GameStateM);
//recvBuf.buffer = sendBuf.buffer;
//recvBuf.setDataRecieved(1024);
//
//GameStateMessage* readGameState = (GameStateMessage*)readMessage(recvBuf);
//
//for (int t = 0; t < readGameState->tanks.size(); t++)
//{
//	printf("tank name: %s\n",
//		readGameState->tanks[t]->name.c_str());
//	std::cout << readGameState->tanks[t]->xP << std::endl;
//	std::cout << readGameState->tanks[t]->yP << std::endl;
//	std::cout << readGameState->tanks[t]->zP << std::endl;
//	std::cout << readGameState->tanks[t]->xV << std::endl;
//	std::cout << readGameState->tanks[t]->yV << std::endl;
//	std::cout << readGameState->tanks[t]->zV << std::endl;
//	std::cout << readGameState->tanks[t]->degrees << std::endl;
//
//}
//for (int t = 0; t < readGameState->bullets.size(); t++)
//{
//	printf("bullet shooter: %s\n",
//		readGameState->bullets[t]->shooter.c_str());
//}
//return 0;