#pragma once
#include <string>
#include <vector>
#include <map>
#include "../globalStuff.h"
#include "Message.h"

struct sTank
{
	std::string name;
	float fireCooldown = 0;
	bool isAlive = true;
};

struct sBullet
{
	std::string shooter, name;
	float lifetime = 2.0f * 60.0f;
};

class cGameBrain
{
private:
	std::vector<sTank*> tanks;
	std::map<std::string, sBullet*> mBullets;
	static cGameBrain* theGameBrain;
public:
	static cGameBrain* getTheGameBrain();
	std::string addTank();
	void removeBullet(std::string name);
	void addBullet(std::string shooterName);
	void detectCollisions();
	sTank* findShooter(std::string);
	sTank* get_sTank(std::string tankName);
	GameStateMessage* encodeGameState();

	void Update(float deltatime);
};

