#include "cGameBrain.h"

#include "../util/tools.h"

cGameBrain* cGameBrain::theGameBrain = new cGameBrain();

float tanksSize = 10;

cGameBrain* cGameBrain::getTheGameBrain()
{
	return theGameBrain;
}

void cGameBrain::setPlayerObject(const std::string playerName)
{
	player = ::g_map_GameObjects[playerName];
}

void cGameBrain::addTank(std::string name)
{
	float randx = randInRange(-195, 195);
	float randy = randInRange(-118, 118);
	cGameObject* newTank = new cGameObject(::g_map_GameObjects["tank_template"]);
	// determine randomly??
	newTank->positionXYZ = glm::vec3(randx,randy, -2.0);
	newTank->friendlyName = name;
	newTank->isVisible = true;

	::g_map_GameObjects[name] = newTank;

	sTank* gameTank = new sTank();
	gameTank->name = name;
	gameTank->aiType = enemyType::ninguno;

	tanks.push_back(gameTank);
}

void cGameBrain::addTank(std::string name, enemyType aiType)
{
	float randx = randInRange(-195, 195);
	float randy = randInRange(-118, 118);
	cGameObject* newTank = new cGameObject(::g_map_GameObjects["tank_template"]);
	// determine randomly??
	newTank->positionXYZ = glm::vec3(randx, 15.0f, randy);
	newTank->friendlyName = name;
	newTank->isVisible = true;

	::g_map_GameObjects[name] = newTank;

	sTank* gameTank = new sTank();
	gameTank->name = name;
	gameTank->aiType = aiType;

	tanks.push_back(gameTank);
	colorTank(name);
}

void cGameBrain::detectCollisions()
{
	for (std::vector<sTank*>::iterator iterT = tanks.begin() ;
		iterT != tanks.end(); iterT++)
	{
		sTank* pTank = *iterT;
		for (std::vector<sBullet*>::iterator iterB = bullets.begin() ;
			iterB != bullets.end(); )
		{
			sBullet* pBullet = *iterB;
			cGameObject* pBulletObj = NULL, *pTankObj = NULL;
			auto tan = ::g_map_GameObjects.find(pTank->name);
			auto bull = ::g_map_GameObjects.find(pBullet->name);
			if(bull != ::g_map_GameObjects.end() && tan != ::g_map_GameObjects.end())
			{
				pTankObj = tan->second;
				pBulletObj = bull->second;
			}

			if (pBulletObj && pTankObj)
			{

				float distance = glm::distance(pTankObj->positionXYZ, pBulletObj->positionXYZ);

				if (pTank->isAlive && // what is dead may never die
					pTank->name != pBullet->shooter && // no suicide
					distance < tanksSize // it's close enough
					)
				{
					// I kill the tank
					pTank->isAlive = false;
					::g_map_GameObjects[pTank->name]->isVisible = false;
					// I reset the shooter's fireCooldown
					sTank *shooter = findShooter(pBullet->shooter);
					shooter->fireCooldown = 0;
					// I delete the bullet
					// from the scene
					::g_map_GameObjects.erase(pBullet->name);
					// and from the game state
					iterB = bullets.erase(iterB);

					if(pTank->name != "player")
					{
						iterT = tanks.erase(iterT);
						::g_map_GameObjects.erase(pTank->name);
						::g_map_GameObjects.erase(pTank->name + "_bullet");
						return;
					}
					continue;
				}
			}
			iterB++;
		}
	}
	
	for (int x = 0; x < (tanks.size()-1); x++)
	{
		for (int y = 1; y < tanks.size(); y++)
		{
			auto sTankA = tanks[x];
			auto sTankB = tanks[y];
			if (sTankA == sTankB) { continue; }
			auto tankA = ::g_map_GameObjects[sTankA->name];
			auto tankB = ::g_map_GameObjects[sTankB->name];

			float distance = glm::distance(tankA->positionXYZ,tankB->positionXYZ);
			if (distance < tanksSize && sTankA->name == "player")
			{
				//tanks.erase(tanks.begin() + y);
				//::g_map_GameObjects.erase(sTankB->name);
				tankA->isVisible = false;
				continue;
			}
		}
	}
}

sTank* cGameBrain::findShooter(std::string tankName)
{
	for (std::vector<sTank*>::iterator iterT = tanks.begin();
		iterT != tanks.end(); iterT++)
	{
		if ((*iterT)->name == tankName)
		{
			return *iterT;
		}
	}
	return NULL;
}

void cGameBrain::colorTank(const std::string& tank_name)
{
	cGameObject* tank = ::g_map_GameObjects.find(tank_name)->second;
	sTank* stTank = findShooter(tank_name);
	if (tank && stTank)
	{
		switch (stTank->aiType)
		{
		case enemyType::ninguno: break;
		case enemyType::A:
			tank->textures = { "redTank.bmp","","","" };
			break;
		case enemyType::B:
			tank->textures = { "blueTank.bmp","","","" };
			break;
		case enemyType::C:
			tank->textures = { "whiteTank.bmp","","","" };
			break;
		case enemyType::D:
			tank->textures = { "greenTank.bmp","","","" };
			break;
		default: break;
		}
	}
}

void cGameBrain::removeBullet(std::string name)
{
	for (std::vector<sBullet*>::iterator iterB = bullets.begin();
		iterB != bullets.end(); iterB++)
	{
		if ((*iterB)->name == name)
		{
			::g_map_GameObjects.erase(name);
			bullets.erase(iterB);
			return;
		}
	}
}

void cGameBrain::addBullet(std::string shooterName)
{
	// add in the game state
	sBullet* newBullet = new sBullet();
	newBullet->name = shooterName + "_bullet";
	newBullet->shooter = shooterName;
	bullets.push_back(newBullet);

	// add in the scene
	cGameObject* bullet4MyBible = new cGameObject(*::g_map_GameObjects["sphere"]);
	std::map<std::string, cGameObject*>::iterator itPlayer = ::g_map_GameObjects.find(shooterName);
	float bulletSpeed = 60.0f, lifetime = 2.0f * 60.0f;

	if (itPlayer != ::g_map_GameObjects.end())
	{
		cGameObject* player = itPlayer->second;
		bullet4MyBible->positionXYZ = player->positionXYZ;
		bullet4MyBible->velocity = player->getCurrentAT() * bulletSpeed;
		bullet4MyBible->friendlyName = newBullet->name;
		bullet4MyBible->lifetime = lifetime;
		bullet4MyBible->isVisible = true;
		bullet4MyBible->inverseMass = 1.0f;
		bullet4MyBible->tag = "lifetime";

		::g_map_GameObjects[newBullet->name] = bullet4MyBible;
	}
}

void cGameBrain::enemyUpdate()
{
	for (auto tank : tanks)
	{
		//if (tank->name == "player") 
		//{ return; }
		cGameObject* theNPC = ::g_map_GameObjects[tank->name];
		switch (tank->aiType)
		{
		case enemyType::ninguno: //none in spanish
			break;
		case enemyType::A:
			typeA(theNPC);
			break;
		case enemyType::B: 
			typeB(theNPC);
			break;
		case enemyType::C:
			typeC(theNPC);
			break;
		case enemyType::D:
			typeD(theNPC);
			break;
		default: break;
		}
		lookTowardsDirection(theNPC);
	}
	
	if(tanks.size() < 7)
	{
		int randEnemyType = randInRange(1, 5);
		std::string name = "enemy_" + std::to_string(randEnemyType) + "_" + std::to_string(tankCounter);
		addTank(name, enemyType(randEnemyType));
		tankCounter++;
	}
}

void cGameBrain::typeA(cGameObject* npc)
{
	glm::vec3 playerEnemyLine = glm::normalize(npc->positionXYZ - player->positionXYZ);
	float dotProd = glm::dot(playerEnemyLine, player->getCurrentAT());
	if (dotProd>0.5f)
	{
		npc->velocity = cSteeringBehaviour::Update(
			npc->positionXYZ, npc->velocity, 
			player->positionXYZ, player->velocity,
			static_cast<float>(averageDeltaTime), steeringType::flee);
	}
	else
	{
		npc->velocity = cSteeringBehaviour::Update(
			npc->positionXYZ, npc->velocity,
			player->positionXYZ, player->velocity,
			static_cast<float>(averageDeltaTime), steeringType::seek);
	}
}

void cGameBrain::typeB(cGameObject* npc)
{
	// pursue player
	npc->velocity = cSteeringBehaviour::Update(
		npc->positionXYZ, npc->velocity,
		player->positionXYZ, player->velocity,
		static_cast<float>(averageDeltaTime), steeringType::pursue);
	
	auto itGO = ::g_map_GameObjects.find(player->friendlyName + "_bullet");
	if (itGO != ::g_map_GameObjects.end())
	{
		//std::cout << "evading bullet!" << std::endl;
		const auto theBullet = itGO->second;
		npc->velocity = cSteeringBehaviour::Update(
			npc->positionXYZ, npc->velocity,
			theBullet->positionXYZ, theBullet->velocity,
			static_cast<float>(averageDeltaTime), steeringType::evade);
	}
}

void cGameBrain::typeC(cGameObject* npc)
{
	// approach the player
	npc->velocity = cSteeringBehaviour::Update(
		npc->positionXYZ, npc->velocity,
		player->positionXYZ, player->velocity,
		static_cast<float>(averageDeltaTime), steeringType::arrive);
	
	// check if he should shoot
	const float distanceToPLayer = glm::distance(player->positionXYZ, npc->positionXYZ);
	if(distanceToPLayer < 80.f)
	{
		fire(npc);
	}
}

void cGameBrain::typeD(cGameObject* npc)
{
	counterD_ += float(averageDeltaTime);
	
	if (counterD_ > 1.f)
	{
		timerD_ += 1.f;
		counterD_ = 0.f;
	}
	timerD_ = 6.f;
	if(timerD_ < 7.f)
	{
		npc->velocity = cSteeringBehaviour::Update(
			npc->positionXYZ, npc->velocity,
			player->positionXYZ, player->velocity,
			static_cast<float>(averageDeltaTime), steeringType::wander);
	}
	else if (timerD_ < 10.f)
	{
		npc->velocity = cSteeringBehaviour::Update(
			npc->positionXYZ, npc->velocity,
			player->positionXYZ, player->velocity,
			static_cast<float>(averageDeltaTime), steeringType::none);
	}
	else if(timerD_ > 10.f)
	{
		timerD_ = 0.f;
	}
}

bool cGameBrain::canFire(cGameObject* npc)
{
	bool temp = false;
	auto theBullet = ::g_map_GameObjects.find(npc->friendlyName + "_bullet");
	
	temp = theBullet == ::g_map_GameObjects.end();
	temp = temp && npc->isVisible;
	
	return temp;
}

void cGameBrain::fire(cGameObject* npc)
{
	if (canFire(npc))
	{
		addBullet(npc->friendlyName);
	}
}

void cGameBrain::lookTowardsDirection(cGameObject* npc)
{
	if (npc->friendlyName != "player")
	{
		glm::vec3 desiredAT = glm::normalize(npc->velocity);
		npc->setAT(desiredAT);
	}
}
