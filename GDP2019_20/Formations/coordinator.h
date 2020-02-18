#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "../cGameObject.h"
#include "../steeringBehaviour/cSteeringBehaviour.hpp"

namespace formations
{
	class coordinator
	{
		static coordinator* theOneAndOnlyCoordinator;
		
		std::vector<glm::vec3> circleOffsets;
		std::vector<glm::vec3> veeOffsets;
		std::vector<glm::vec3> squareOffsets;
		std::vector<glm::vec3> lineOffsets;
		std::vector<glm::vec3> twoLinesOffsets;
		

		template <class T>
		static T randInRange(T min, T max)
		{ double value = min + static_cast <double> (rand())/ (static_cast <double> (RAND_MAX / (static_cast<double>(max - min)))); return static_cast<T>(value); }
		
	public:

		cGameObject* self;
		std::string currentFormation;
		std::vector<cGameObject*> vehicles;
		
		static coordinator* getTheCoordinator();
		void init(std::map<std::string,cGameObject*>* g_GO);
		void update();
		void initCircle();
		void initVee();
		void initSquare();
		void initLine();
		void initTwoLine();

		//utils
		void selectFormationVector(std::vector<glm::vec3>** vecs);
		void addVehicle(std::map<std::string,cGameObject*>* g_GO);
		void lookTowardsDirection(cGameObject* vehicle);
		static void thePaqman(std::map<std::string,cGameObject*>* g_GO);
		glm::mat4 calculateWorldMatrix(cGameObject* pCurrentObject);
		void correctVerticalAlignment();
	};
	
}

