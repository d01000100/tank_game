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
			fire();
		}

		player->velocity = velocity;
	}
}

bool cTankControls::hasFired()
{
	return true;
}

void cTankControls::fire()
{
	if (!hasFired())
	{
		cGameBrain* theGameBrain = cGameBrain::getTheGameBrain();
		theGameBrain->addBullet("");
	}
}