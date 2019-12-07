#include "TankControls.h"
#include "cGameBrain.h"
#include <string>

void cTankControls::updateTank(std::string tankName, UserInputMessage pressedKeys)
{
	std::map<std::string, cGameObject*>::iterator itGO;
	itGO = ::g_map_GameObjects.find(tankName);
	if (itGO != ::g_map_GameObjects.end())
	{
		cGameObject* player = itGO->second;
		glm::vec3 velocity = glm::vec3(0);
		float rotationStep = 2.0f, speed = 10.0f;

		// check rotation
		if (pressedKeys.A)
		{
			player->updateOrientation(glm::vec3(0, rotationStep, 0));
		}
		if (pressedKeys.D)
		{
			player->updateOrientation(glm::vec3(0, -rotationStep, 0));
		}

		// check velocity
		if (pressedKeys.W)
		{
			velocity += player->getCurrentAT() * speed;
		}
		if (pressedKeys.S)
		{
			velocity += player->getCurrentAT() * -speed;
		}

		if (pressedKeys.Space)
		{
			if(canFire(player->friendlyName))
			{
				fire(player->friendlyName);
			}
		}

		player->velocity = velocity;
	}
}

bool cTankControls::canFire(std::string shooterName)
{
	cGameBrain* theGameBrain = cGameBrain::getTheGameBrain();
	sTank* pTank = theGameBrain->get_sTank(shooterName);
	if(pTank->fireCooldown <= 0)
	{
		return true;
	}
	return false;
}

void cTankControls::fire(std::string shooterName)
{
	cGameBrain* theGameBrain = cGameBrain::getTheGameBrain();
	theGameBrain->addBullet(shooterName);
	sTank* pTank = theGameBrain->get_sTank(shooterName);
	pTank->fireCooldown = 2.0f;
}
