#pragma once
#include <string>
#include <vector>

#include "../globalStuff.h"
#include "Message.h"

class cGameBrain
{
private:
	std::vector<sTank*> tanks;
	std::vector<sBullet*> bullets;
	static cGameBrain* theGameBrain;
public:
	static cGameBrain* getTheGameBrain();
	void addTank(std::string name);
	void removeBullet(std::string name);
	void addBullet(std::string shooterName);
	void detectCollisions();
	sTank* findShooter(std::string);
};

