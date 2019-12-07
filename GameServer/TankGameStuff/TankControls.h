#pragma once
#include <string>

#include "../globalStuff.h"
#include "Message.h"

class cTankControls
{
public:
	static void updateTank(std::string tankName, UserInputMessage input);
	static void fire(std::string shooterName);
	static std::string keysInfo();
	static bool canFire(std::string shooterName);
};

