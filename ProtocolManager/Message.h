#pragma once
#include <string>
#include <vector>

enum MessageType {
	OLI, NAME, USER_INPUT, GAME_STATE, ERROR
};

std::string messageTypeString(MessageType t);

// It's a class because we need inheritance
// But it's basically a struct
class Message {
public:
	MessageType type;
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

struct sTank
{
	std::string name;
	float fireCooldown = 0;
	bool isAlive = true;
};

struct sBullet
{
	std::string shooter, name;
};

class GameStateMessage : public Message {
public:
	std::vector<sTank*> tanks;
	std::vector<sBullet*> bullets;
	GameStateMessage();
};