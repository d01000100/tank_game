#include "ProtocolManager.h"

int getGameStateSize(GameStateMessage* message);
int getSMessageTank_size(sMessageTank smTank);
int getSMessageBullet_size(sMessageBullet smBullet);

SendBuffer writeMessage(OliMessage* message) {
	//	int       	int     
	// [theLength][message_type]	
	SendBuffer buffer;
	buffer.writeInt(8);
	buffer.writeInt(OLI);
	return buffer;
}

SendBuffer writeMessage(NameMessage* message) {
	//	int            int        int     string
	// [theLength][message_type][length][tank_name]
	unsigned int name_length = message->tank_name.length();

	SendBuffer buffer;
	buffer.writeInt(12+name_length);
	buffer.writeInt(NAME);
	buffer.writeInt(name_length);
	buffer.writeString(message->tank_name);
	return buffer;
}

SendBuffer writeMessage(UserInputMessage* message) {
	//	int             int     int int int int   int
	// [theLength][message_type][W] [A] [S] [D] [Space]

	SendBuffer buffer;
	buffer.writeInt(7*4);
	buffer.writeInt(USER_INPUT);
	buffer.writeInt(message->W);
	buffer.writeInt(message->A);
	buffer.writeInt(message->S);
	buffer.writeInt(message->D);
	buffer.writeInt(message->Space);
	return buffer;
}

SendBuffer writeMessage(GameStateMessage* message) {
	//	int             int       int    Object     int      Object
	// [theLength][message_type][nTanks] [tanks] [nBullets] [bullets]

	SendBuffer buffer;
	buffer.writeInt(16+getGameStateSize(message));
	buffer.writeInt(GAME_STATE);
	buffer.writeInt(message->tanks.size());

	for (int t = 0; t < message->tanks.size(); t++)
	{
		sMessageTank* tank = message->tanks[t];
		//    int      string     int          int
		//[name_length][name][fire_cooldown][is_alive]
		buffer.writeInt(tank->name.size());
		buffer.writeString(tank->name);
		buffer.writeInt(tank->fireCooldown);
		buffer.writeInt(tank->isAlive);
		buffer.writeInt(tank->xV);
		buffer.writeInt(tank->yV);
		buffer.writeInt(tank->zV);
		buffer.writeInt(tank->xP);
		buffer.writeInt(tank->yP);
		buffer.writeInt(tank->zP);
		buffer.writeInt(tank->degrees);
	}
	buffer.writeInt(message->bullets.size());

	for (int t = 0; t < message->bullets.size(); t++)
	{
		sMessageBullet* bullet = message->bullets[t];
		//    int      string     int          string
		//[name_length][name][shooter_lenght][shooter]
		buffer.writeInt(bullet->name.size());
		buffer.writeString(bullet->name);
		buffer.writeInt(bullet->shooter.size());
		buffer.writeString(bullet->shooter);
		buffer.writeInt(bullet->xV);
		buffer.writeInt(bullet->yV);
		buffer.writeInt(bullet->zV);
		buffer.writeInt(bullet->xP);
		buffer.writeInt(bullet->yP);
		buffer.writeInt(bullet->zP);
	}
	return buffer;
}

Message* readMessage(RecieveBuffer buffer) {
	// ====== Header ========
	//     int
	//[message_type]
	int theLength = buffer.readInt();
	MessageType message_type = (MessageType)buffer.readInt();

	switch (message_type)
	{
	case OLI:
	{
		OliMessage* message = new OliMessage();
		message->theLength = theLength;
		return message;
		break;
	}
	case NAME:
	{
		//  int     string
		//[length][tank_name]
		NameMessage* message = new NameMessage();
		message->theLength = theLength;
		int name_length = buffer.readInt();
		message->tank_name = buffer.readString(name_length);
		return message;
		break;
	}
	case USER_INPUT:
	{
		//int int int int   int
		//[W] [A] [S] [D] [Space]
		UserInputMessage* message = new UserInputMessage();
		message->theLength = theLength;
		message->W = buffer.readInt();
		message->A = buffer.readInt();
		message->S = buffer.readInt();
		message->D = buffer.readInt();
		message->Space = buffer.readInt();
		return message;
		break;
	}
	case GAME_STATE:
	{
		//   int    Object     int      Object
		//[nTanks] [tanks] [nBullets] [bullets]

		GameStateMessage* message = new GameStateMessage();
		int nTanks = buffer.readInt();
		message->theLength = theLength;

		for (int t = 0; t < nTanks; t++)
		{
			sMessageTank* tank = new sMessageTank();
			//    int      string     int          int
			//[name_length][name][fire_cooldown][is_alive]
			int name_length = buffer.readInt();
			tank->name = buffer.readString(name_length);
			tank->fireCooldown = buffer.readInt();
			tank->isAlive = buffer.readInt();
			tank->xV = buffer.readInt();
			tank->yV = buffer.readInt();
			tank->zV = buffer.readInt();
			tank->xP = buffer.readInt();
			tank->yP = buffer.readInt();
			tank->zP = buffer.readInt();
			tank->degrees = buffer.readInt();
			message->tanks.push_back(tank);

		}
		int nBullets = buffer.readInt();

		for (int t = 0; t < nBullets; t++)
		{
			sMessageBullet* Bullet = new sMessageBullet();
			//    int      string     int          string
			//[name_length][name][shooter_length][shooter]
			int name_length = buffer.readInt();
			Bullet->name = buffer.readString(name_length);
			int shooter_length = buffer.readInt();
			Bullet->shooter = buffer.readString(shooter_length);
			Bullet->xV = buffer.readInt();
			Bullet->yV = buffer.readInt();
			Bullet->zV = buffer.readInt();
			Bullet->xP = buffer.readInt();
			Bullet->yP = buffer.readInt();
			Bullet->zP = buffer.readInt();
			message->bullets.push_back(Bullet);
		}
		return message;
		break;
	}
	default:
		Message *m = new Message();
		m->type = UNOWN;
		return m;
		break;
	}
}

int getGameStateSize(GameStateMessage* message)
{
	int theSize = 0;
	for (int index = 0; index < message->tanks.size(); index++)
	{
		sMessageTank* smT = message->tanks[index];
		theSize += getSMessageTank_size(*smT);
	}
	for (int index = 0; index < message->bullets.size(); index++)
	{
		sMessageBullet* smB = message->bullets[index];
		theSize += getSMessageBullet_size(*smB);
	}
	return theSize;
}

int getSMessageTank_size(sMessageTank smTank)
{
	int theSize = 4*9;
	theSize += smTank.name.size();
	return theSize;
}

int getSMessageBullet_size(sMessageBullet smBullet)
{
	int theSize = 4*6;
	theSize += smBullet.name.size();
	theSize += smBullet.shooter.size();
	return theSize;
}
