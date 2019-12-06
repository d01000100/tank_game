#pragma once
#include <string>

#include "../globalStuff.h"
#include "Message.h"

class cTankControls
{
public:
	static UserInputMessage pressedKeys;
	static void setPlayer(std::string name);
	static void InputListen(GLFWwindow* window);
	static void key_callback(int key, int action, int mods);
	static void updateTank();
	static void fire();
	static std::string keysInfo();
	static bool hasFired();
};

