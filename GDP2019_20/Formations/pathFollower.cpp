#include "pathFollower.h"

namespace formations
{
	pathFollower* pathFollower::thePathFollower = new pathFollower();
	
	pathFollower* pathFollower::getThePathFollower()
	{
		return thePathFollower;
	}

	
	void pathFollower::init(std::map<std::string,cGameObject*>* g_GO)
	{		
		self = g_GO->at("coordinador");
		currentBeacon = 0;
		isReverse = false;
		isActive = false;
		// bounds
		/* X: 200 to -200 || Z: 120 to -120 */
		addBeacon(g_GO,glm::vec3(180,15,-100));
		addBeacon(g_GO,glm::vec3(180,15,100));
		addBeacon(g_GO,glm::vec3(0,15,0));
		addBeacon(g_GO,glm::vec3(-180,15,100));		
		addBeacon(g_GO,glm::vec3(-180,15,-100));
		colorBeacon(currentBeacon,1);
	}

	void pathFollower::addBeacon(std::map<std::string, cGameObject*>* g_GO,glm::vec3 posVec)
	{
		cGameObject* beacon = new cGameObject(g_GO->at("sphere"));
		
		beacon->positionXYZ = posVec;
		beacon->friendlyName = "beacon_" + std::to_string(beacon->getUniqueID());
		beacon->isVisible = true;
		beacon->textures = { "Red.bmp","","","" };

		g_GO->insert({beacon->friendlyName,beacon});
		this->beacons.push_back(beacon);
	}

	void pathFollower::update()
	{
		auto target = beacons[currentBeacon]->positionXYZ;
		glm::vec3 steerForce = cSteeringBehaviour::Update(
					self->positionXYZ, self->velocity,
					target, glm::vec3(0),
					(1/60.f), steeringType::seek);
		
		self->velocity = steerForce;

		checkBeaconProximity();
	}

	void pathFollower::checkBeaconProximity()
	{
		auto target = beacons[currentBeacon]->positionXYZ;
		auto dist = glm::distance(target,self->positionXYZ);
		if (dist < 5.f)
		{
			selectNextBeacon();
		}
	}

	void pathFollower::selectNextBeacon()
	{
		colorBeacon(currentBeacon,0); //color inactive..
		addToBeaconSelection(&currentBeacon);
		colorBeacon(currentBeacon,1); //color active..
	}

	void pathFollower::colorBeacon(int pos, int colour)
	{
		if(colour)
		{
			beacons[currentBeacon]->textures = {"HotPink.bmp","","",""};
		}
		else
		{
			beacons[currentBeacon]->textures = {"Red.bmp","","",""};
		}
	}

	void pathFollower::addToBeaconSelection(int* index)
	{
		if(!isReverse)
		{
			*index += 1;
			if(*index > 4) *index = 0;
		}
		else
		{
			*index -= 1;
			if(*index < 0) *index = 4;
		}
	}
}
