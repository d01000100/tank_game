#pragma once
#include <string>
#include <vector>

enum MessageType {
	OLI, NAME, USER_INPUT, GAME_STATE, UNOWN
};

std::string messageTypeString(MessageType t);

// It's a class because we need inheritance
// But it's basically a struct
class Message {
public:
	MessageType type;
	int theLength;
};

class OliMessage : public Message {
public:
	OliMessage();
};

class NameMessage : public Message {
public:
	std::string tank_name;
	NameMessage();
};

class UserInputMessage : public Message {
public:
	int A, S, D, W, Space;
	UserInputMessage();
};

struct sMessageTank
{
	std::string name;
	int fireCooldown = 0;
	int isAlive = 1;
	int xV,yV,zV;
	int xP,yP,zP;
	int degrees;
};

struct sMessageBullet
{
	std::string shooter, name;
	int xV,yV,zV;
	int xP,yP,zP;
};

class GameStateMessage : public Message {
public:
	std::vector<sMessageTank*> tanks;
	std::vector<sMessageBullet*> bullets;
	GameStateMessage();
};
