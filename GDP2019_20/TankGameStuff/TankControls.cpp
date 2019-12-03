#include "TankControls.h"
#include <string>
#include "../GLCommon.h"

sTankInputState cTankControls::pressedKeys;

std::string tank_name = "player",
	bullet_name = "tank_bullet";

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
	cGameObject* player = ::g_map_GameObjects[tank_name];
	if (player)
	{
		glm::vec3 velocity = glm::vec3(0);
		float rotationStep = 2.0f, speed = 10.0f;

		// check rotation
		if (pressedKeys.A)
		{
			player->updateOrientation(glm::vec3(0, -rotationStep, 0));
		}
		if (pressedKeys.D)
		{
			player->updateOrientation(glm::vec3(0, rotationStep, 0));
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
	return ::g_map_GameObjects[bullet_name] != NULL;
}

void cTankControls::fire()
{
	if (!hasFired())
	{
		cGameObject* bullet4MyBible = new cGameObject(*::g_map_GameObjects["sphere"]),
			* player = ::g_map_GameObjects[tank_name];
		float bulletSpeed = 30.0f, lifetime = 2.0f * 60.0f;

		if (player)
		{
			bullet4MyBible->positionXYZ = player->positionXYZ;
			bullet4MyBible->velocity = player->getCurrentAT() * bulletSpeed;
			bullet4MyBible->friendlyName = bullet_name;
			bullet4MyBible->lifetime = lifetime;
			bullet4MyBible->isVisible = true;
			bullet4MyBible->inverseMass = 1.0f;
			bullet4MyBible->tag = "lifetime";

			::g_map_GameObjects[bullet_name] = bullet4MyBible;
		}
	}
}