#include "ProtocolManager.h"

SendBuffer writeMessage(OliMessage* message) {
	//      int     
	//[message_type]	
	SendBuffer buffer;
	buffer.writeInt(OLI);
	return buffer;
}

SendBuffer writeMessage(NameMessage* message) {
	//     int        int     string
	//[message_type][length][tank_name]
	unsigned int name_length = message->tank_name.length();

	SendBuffer buffer;
	buffer.writeInt(NAME);
	buffer.writeInt(name_length);
	buffer.writeString(message->tank_name);
	return buffer;
}

SendBuffer writeMessage(UserInputMessage* message) {
	//      int     int int int int   int
	//[message_type][W] [A] [S] [D] [Space]

	SendBuffer buffer;
	buffer.writeInt(USER_INPUT);
	buffer.writeInt(message->W);
	buffer.writeInt(message->A);
	buffer.writeInt(message->S);
	buffer.writeInt(message->D);
	buffer.writeInt(message->Space);
	return buffer;
}

SendBuffer writeMessage(GameStateMessage* message) {
	//      int       int    Object     int      Object
	//[message_type][nTanks] [tanks] [nBullets] [bullets]

	SendBuffer buffer;
	buffer.writeInt(GAME_STATE);
	buffer.writeInt(message->tanks.size());

	for (int t = 0; t < message->tanks.size(); t++)
	{
		sTank* tank = message->tanks[t];
		//    int      string     int          int
		//[name_length][name][fire_cooldown][is_alive]
		buffer.writeInt(tank->name.size());
		buffer.writeString(tank->name);
		buffer.writeInt(tank->fireCooldown);
		buffer.writeInt(tank->isAlive);
	}
	buffer.writeInt(message->bullets.size());

	for (int t = 0; t < message->bullets.size(); t++)
	{
		sBullet* bullet = message->bullets[t];
		//    int      string     int          string
		//[name_length][name][shooter_lenght][shooter]
		buffer.writeInt(bullet->name.size());
		buffer.writeString(bullet->name);
		buffer.writeInt(bullet->shooter.size());
		buffer.writeString(bullet->shooter);
	}
	return buffer;
}

Message* readMessage(RecieveBuffer buffer) {
	// ====== Header ========
	//     int
	//[message_type]
	MessageType message_type = (MessageType)buffer.readInt();

	switch (message_type)
	{
	case OLI:
		return new OliMessage();
		break;
	case NAME:
	{
		//  int     string
		//[length][tank_name]
		NameMessage* message = new NameMessage();
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

		for (int t = 0; t < nTanks; t++)
		{
			sTank* tank = new sTank();
			//    int      string     int          int
			//[name_length][name][fire_cooldown][is_alive]
			int name_length = buffer.readInt();
			tank->name = buffer.readString(name_length);
			tank->fireCooldown = buffer.readInt();
			tank->isAlive = buffer.readInt();
			message->tanks.push_back(tank);
		}
		int nBullets = buffer.readInt();

		for (int t = 0; t < nBullets; t++)
		{
			sBullet* Bullet = new sBullet();
			//    int      string     int          string
			//[name_length][name][shooter_length][shooter]
			int name_length = buffer.readInt();
			Bullet->name = buffer.readString(name_length);
			int shooter_length = buffer.readInt();
			Bullet->shooter = buffer.readString(shooter_length);
			message->bullets.push_back(Bullet);
		}
		return message;
		break;
	}
	default:
		Message *m = new Message();
		m->type = ERROR;
		return m;
		break;
	}
}