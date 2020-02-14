#pragma once
#include <glm/glm.hpp>
#include "../cGameObject.h"

namespace formations
{
	class flocking
	{
	public:
		float separationWeight, alignmentWeight, cohesionWeight;
		std::vector<cGameObject*> boids;
		void init(std::vector<cGameObject*>* vehicles);
		glm::vec3 separation(cGameObject* self);
		glm::vec3 alignment(cGameObject* self);
		glm::vec3 cohesion(cGameObject* self);
		void doTheFlocking();
	};
}

