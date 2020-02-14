#pragma once
#include <glm/glm.hpp>
#include "../cGameObject.h"

namespace formations
{
	class flocking
	{
	public:
		std::vector<cGameObject*> boids;		
		glm::vec3 separation(cGameObject* self);
		glm::vec3 alignment(cGameObject* self);
		glm::vec3 cohesion(cGameObject* self);
	};
}

