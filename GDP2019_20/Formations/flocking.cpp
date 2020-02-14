#include "flocking.h"
#include <iostream>
#include  <glm/gtx/string_cast.hpp>
#include "../steeringBehaviour/cSteeringBehaviour.hpp"

namespace formations
{
	void flocking::init(std::vector<cGameObject*>* vehicles)
	{
		separationWeight = alignmentWeight = cohesionWeight = 0.33f;
		for(auto vehicle : *vehicles)
		{
			boids.push_back(vehicle);
		}
	}
	
	glm::vec3 flocking::separation(cGameObject* self)
	{
		float maxVelocity = 10.f;
		float maxForce = 20.f;
		glm::vec3 totalFlee(0);
		float separationRadius = 40.0f;
		int neighbourCount = 0;
		for(int i = 0; i < boids.size(); i++)
		{
			float dist = glm::distance(self->positionXYZ, boids[i]->positionXYZ);
			//std::cout << "dist: " << dist << std::endl;
			if((dist > 0) && (dist < separationRadius)) 
			{
				//std::cout << "tooclose" << std::endl;
				glm::vec3 fleeVector = self->positionXYZ - boids[i]->positionXYZ;
				fleeVector= glm::normalize(fleeVector);
				totalFlee += fleeVector;
				neighbourCount++;
			}
		}

		glm::vec3 steerForce(0);
		if(neighbourCount > 0) 
		{
			glm::vec3 desiredVelocity = totalFlee / float(neighbourCount);
			desiredVelocity = glm::normalize(desiredVelocity);
			desiredVelocity *= maxVelocity;
			steerForce = desiredVelocity - self->velocity;
			//steerForce *= maxForce;
		}
		//std::cout << "steerforce: " << glm::to_string(steerForce) << std::endl;
		steerForce.y = 0.f;
		return steerForce;
	}

	glm::vec3 flocking::alignment(cGameObject* self)
	{
		float maxVelocity = 10.f;
		glm::vec3 totalVelocity(0);
		float alignmentRadius = 50.0f;
		int neighbourCount = 0;
		for(int i = 0; i < boids.size(); i++)
		{
			float dist = glm::distance(self->positionXYZ, boids[i]->positionXYZ);
			if((dist > 0) && (dist < alignmentRadius)) 
			{
				totalVelocity += boids[i]->velocity;
				neighbourCount++;
			}
		}
		
		glm::vec3 steerForce(0);
		if(neighbourCount > 0) 
		{
			//std::cout << "totalV: " << glm::to_string( totalVelocity) << std::endl;
			glm::vec3 desiredVelocity = totalVelocity / float(neighbourCount);
			if(glm::length(desiredVelocity) != 0.f)
			{
				desiredVelocity = glm::normalize(desiredVelocity);
			}
			desiredVelocity *= maxVelocity;
			steerForce = desiredVelocity - self->velocity;
			//steerForce *= maxForce;
		}
		steerForce.y = 0.f;
		return steerForce;
	}

	glm::vec3 flocking::cohesion(cGameObject* self)
	{
		glm::vec3 totalPosition(0);
		float cohesionRadius = 50.0f;
		int neighbourCount = 0;
		for(int i = 0; i < boids.size(); i++)
		{
			float dist = glm::distance(self->positionXYZ, boids[i]->positionXYZ);
			if((dist > 0) && (dist < cohesionRadius)) 
			{
				totalPosition += boids[i]->positionXYZ;
				neighbourCount++;
			}
		}
		glm::vec3 steerForce(0);
		if(neighbourCount > 0) 
		{
			glm::vec3 target = totalPosition / float(neighbourCount);
			//steerForce = Seek(target);
			steerForce = cSteeringBehaviour::Update(
					self->positionXYZ, self->velocity,
					target, glm::vec3(0),
					(1/60.f), steeringType::seek);
		}
		steerForce.y = 0.f;
		return steerForce;
	}

	void flocking::doTheFlocking()
	{
		for(auto boid : boids)
		{
			boid->velocity += (separation(boid) * separationWeight);
			boid->velocity += (alignment(boid) * alignmentWeight);
			boid->velocity += (cohesion(boid) * cohesionWeight);
		}
	}
	
}

