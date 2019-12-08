#include "cGameBrain.h"

#include "../util/tools.h"

cGameBrain* cGameBrain::theGameBrain = new cGameBrain();

float tanksSize = 10;

cGameBrain* cGameBrain::getTheGameBrain()
{
	return theGameBrain;
}

std::string cGameBrain::addTank()
{
	cGameObject* newTank = new cGameObject(::g_map_GameObjects["tank_template"]);
	// determine randomly??
	int xRand = randInRange(-100,100);
	int yRand = randInRange(-100,100);
	newTank->positionXYZ = glm::vec3(xRand,yRand, -2.0);
	newTank->isVisible = true;

	::g_map_GameObjects.insert({newTank->friendlyName,newTank});

	sTank* gameTank = new sTank();
	gameTank->name = newTank->friendlyName;

	this->tanks.push_back(gameTank);
	return gameTank->name;
}

void cGameBrain::detectCollisions()
{
	for (std::vector<sTank*>::iterator iterT = tanks.begin() ;
		iterT != tanks.end(); iterT++)
	{
		sTank* pTank = *iterT;
		for (std::map<std::string, sBullet*>::iterator iterB = mBullets.begin() ;
			iterB != mBullets.end(); )
		{
			sBullet* pBullet = iterB->second;
			std::string shooter_name = iterB->first;
			if (pFindObjectByFriendlyNameMap(pTank->name) && 
				pFindObjectByFriendlyNameMap(pBullet->name))
			{
				cGameObject* pTankObj = ::g_map_GameObjects[pTank->name];
				cGameObject* pBulletObj = ::g_map_GameObjects[pBullet->name];

				float distance = glm::distance(pTankObj->positionXYZ, pBulletObj->positionXYZ);

				if (pTank->isAlive && // what is dead may never die
					pTank->name != pBullet->shooter && // no suicide
					distance < tanksSize // it's close enough
					)
				{
					// I kill the tank
					pTank->isAlive = false;
					pTankObj->isVisible = false;

					// I reset the shooter's fireCooldown
					sTank *shooter = findShooter(pBullet->shooter);
					shooter->fireCooldown = 0;

					// I delete the bullet
					// from the scene
					::g_map_GameObjects.erase(pBullet->name);

					pBullet->lifetime = 0;
					continue;
				}
			}
			iterB++;
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

void cGameBrain::removeBullet(std::string name)
{
	// no es necesario
}

void cGameBrain::addBullet(std::string shooterName)
{
	// add in the game state
	sBullet* newBullet = new sBullet();
	newBullet->name = shooterName + "_bullet";
	newBullet->shooter = shooterName;
	mBullets[shooterName] = newBullet;

	// add in the scene
	cGameObject* bullet4MyBible = new cGameObject(*::g_map_GameObjects["sphere"]);
	std::map<std::string, cGameObject*>::iterator itPlayer = ::g_map_GameObjects.find(shooterName);
	float bulletSpeed = 60.0f;

	if (itPlayer != ::g_map_GameObjects.end())
	{
		cGameObject* player = itPlayer->second;
		bullet4MyBible->positionXYZ = player->positionXYZ;
		bullet4MyBible->velocity = player->getCurrentAT() * bulletSpeed;
		bullet4MyBible->friendlyName = newBullet->name;
		bullet4MyBible->lifetime = newBullet->lifetime;
		bullet4MyBible->isVisible = true;
		bullet4MyBible->inverseMass = 1.0f;
		bullet4MyBible->tag = "lifetime";

		::g_map_GameObjects[newBullet->name] = bullet4MyBible;
	}
}

GameStateMessage* cGameBrain::encodeGameState()
{
	GameStateMessage* state = new GameStateMessage();
	for (std::vector<sTank*>::iterator iterT = tanks.begin();
		iterT != tanks.end(); iterT++)
	{
		sTank* pTank = *iterT;
		if (pFindObjectByFriendlyNameMap(pTank->name))
		{
			cGameObject* gTank = ::g_map_GameObjects[pTank->name];
			sMessageTank *mTank = new sMessageTank();
			mTank->name = pTank->name;
			mTank->fireCooldown = pTank->fireCooldown;
			mTank->isAlive = pTank->isAlive;
			mTank->xP = gTank->positionXYZ.x;
			mTank->yP = gTank->positionXYZ.y;
			mTank->zP = gTank->positionXYZ.z;
			mTank->xV = gTank->velocity.x;
			mTank->yV = gTank->velocity.y;
			mTank->zV = gTank->velocity.z;
			mTank->degrees = gTank->getEulerAngle().z;
			state->tanks.push_back(mTank);
		}
	}

	for (std::map<std::string, sBullet*>::iterator iterB = mBullets.begin();
		iterB != mBullets.end(); iterB++)
	{
		sBullet* pBullet = iterB->second;
		sMessageBullet* mBullet = new sMessageBullet();
		mBullet->name = pBullet->name;
		mBullet->shooter = pBullet->shooter;
		if (pFindObjectByFriendlyNameMap(pBullet->name))
		{
			cGameObject* gBullet = ::g_map_GameObjects[pBullet->name];
			mBullet->xP = gBullet->positionXYZ.x;
			mBullet->yP = gBullet->positionXYZ.y;
			mBullet->zP = gBullet->positionXYZ.z;
			mBullet->xV = gBullet->velocity.x;
			mBullet->yV = gBullet->velocity.y;
			mBullet->zV = gBullet->velocity.z;
		}
		mBullet->lifetime = pBullet->lifetime;
		state->bullets.push_back(mBullet);
	}

	return state;
}

sTank* cGameBrain::get_sTank(std::string tankName)
{
	for (std::vector<sTank*>::iterator iterT = tanks.begin();
		iterT != tanks.end(); iterT++)
	{
		sTank* pTank = *iterT;
		if(pTank->name == tankName)
		{
			return pTank;
		}
	}
	return NULL;
}

void cGameBrain::Update(float deltatime)
{
	for (std::vector<sTank*>::iterator iterT = tanks.begin();
		iterT != tanks.end(); iterT++)
	{
		sTank* pTank = *iterT;
		if(pTank->fireCooldown >= 0)
		{
			pTank->fireCooldown -= deltatime;
		}
	}
	detectCollisions();
}