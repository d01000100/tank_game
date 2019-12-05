#pragma once
#include <string>
#include <vector>

#include "../globalStuff.h"

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

class cGameBrain
{
private:
	std::vector<sTank*> tanks;
	std::vector<sBullet*> bullets;
	static cGameBrain* theGameBrain;
public:
	static cGameBrain* getTheGameBrain();
	std::string addTank();
	void removeBullet(std::string name);
	void addBullet(std::string shooterName);
	void detectCollisions();
	sTank* findShooter(std::string);
};

