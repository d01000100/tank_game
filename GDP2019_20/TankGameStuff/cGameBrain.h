#pragma once
#include <string>
#include <vector>

#include "../globalStuff.h"
#include "../steeringBehaviour/cSteeringBehaviour.hpp"

enum class enemyType
{
	ninguno,A,B,C,D
};

class sTank
{
public:
	std::string name;
	enemyType aiType;
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
	cGameObject* player = nullptr;
	float counterD_ = 0.f;
	float timerD_ = 0.f;
	int tankCounter = 1;
public:
	static cGameBrain* getTheGameBrain();
	void setPlayerObject(std::string playerName);
	void addTank(std::string name);
	void addTank(std::string name, enemyType aiType);
	void removeBullet(std::string name);
	void addBullet(std::string shooterName);
	void detectCollisions();
	sTank* findShooter(std::string);
	void colorTank(const std::string& tank_name);
// for the enemy AI things
	void enemyUpdate();
	void typeA(cGameObject* npc);
	void typeB(cGameObject* npc);
	void typeC(cGameObject* npc);
	void typeD(cGameObject* npc);
	bool canFire(cGameObject* npc);
	void fire(cGameObject* npc);
	void lookTowardsDirection(cGameObject* npc);
};

