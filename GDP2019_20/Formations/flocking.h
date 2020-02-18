#pragma once
#include <glm/glm.hpp>
#include "../cGameObject.h"

namespace formations
{
	class flocking
	{
		static flocking* theFlocking_;
	public:
		static flocking* getTheFlocking();
		float separationWeight, alignmentWeight, cohesionWeight;
		std::vector<cGameObject*> boids;

		// methods
		void init(std::vector<cGameObject*>* vehicles);
		glm::vec3 separation(cGameObject* self);
		glm::vec3 alignment(cGameObject* self);
		glm::vec3 cohesion(cGameObject* self);
		void doTheFlocking();

		// utils
		void moveSeparationWeight(float x);
		void moveAlignmentWeight(float x);
		void moveCohesionWeight(float x);
	};
}

