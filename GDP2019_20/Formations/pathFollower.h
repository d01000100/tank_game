#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "../cGameObject.h"
#include "../steeringBehaviour/cSteeringBehaviour.hpp"

namespace formations
{
	class pathFollower
	{
		static pathFollower* thePathFollower;		
		std::vector<cGameObject*> beacons;
		
	public:

		cGameObject* self;
		int currentBeacon;
		bool isReverse, isActive;
		
		static pathFollower* getThePathFollower();
		void init(std::map<std::string,cGameObject*>* g_GO);
		void addBeacon(
			std::map<std::string, cGameObject*>* g_GO,
			glm::vec3 posVec);
		void update();
		void checkBeaconProximity();
		void selectNextBeacon();
		
		//utils
		void colorBeacon(int pos, int colour);
		inline void addToBeaconSelection(int* index);
	};
	
}

