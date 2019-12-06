#pragma once
#include <string>

#include "../globalStuff.h"
#include "Message.h"

class cTankControls
{
public:
	static void updateTank(std::string tankName, UserInputMessage input);
	static void fire();
	static std::string keysInfo();
	static bool hasFired();
};

