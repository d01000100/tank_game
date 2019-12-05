#include "TankControls.h"
#include "cGameBrain.h"
#include <string>

sTankInputState cTankControls::pressedKeys;

std::string tank_name = "player",
	bullet_name = "player_bullet";

std::string cTankControls::keysInfo()
{
	std::string res = "TankPressedKeys: ";
	if (pressedKeys.A) { res += "A"; }
	if (pressedKeys.S) { res += "S"; }
	if (pressedKeys.D) { res += "D"; }
	if (pressedKeys.W) { res += "W"; }
	if (pressedKeys.Space) { res += "_"; }
	return res;
}

void cTankControls::setPlayer(std::string name)
{
	tank_name = name;
	bullet_name = name + "_bullet";
}

void cTankControls::InputListen(GLFWwindow* window)
{
	pressedKeys.A = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	pressedKeys.S = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	pressedKeys.D = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	pressedKeys.W = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	pressedKeys.Space = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

	updateTank();
}

void cTankControls::updateTank()
{
	std::map<std::string, cGameObject*>::iterator itGO;
	itGO = ::g_map_GameObjects.find(tank_name);
	if (itGO != ::g_map_GameObjects.end())
	{
		cGameObject* player = ::g_map_GameObjects[tank_name];
		if (player)
		{
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
}

bool cTankControls::hasFired()
{
	return ::g_map_GameObjects[bullet_name] != NULL;
}

void cTankControls::fire()
{
	if (!hasFired())
	{
		cGameBrain* theGameBrain = cGameBrain::getTheGameBrain();
		theGameBrain->addBullet(tank_name);
	}
}