#pragma once
#include <string>

#include "../globalStuff.h"

struct sTankInputState
{
	bool W, S, A, D, Space;
};

class cTankControls
{
public:
	static sTankInputState pressedKeys;
	static void setPlayer(std::string name);
	static void InputListen(GLFWwindow* window);
	static void key_callback(int key, int action, int mods);
	static void updateTank();
	static void fire();
	static std::string keysInfo();
	static bool hasFired();
};

