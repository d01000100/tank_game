#include "coordinator.h"

namespace formations
{
	coordinator* coordinator::theOneAndOnlyCoordinator = new coordinator();
	
	coordinator* coordinator::getTheCoordinator()
	{
		return theOneAndOnlyCoordinator;
	}
	
	void coordinator::init(std::map<std::string,cGameObject*>* g_GO)
	{
		currentFormation = "Circle";
		
		auto* vehicle = new cGameObject(g_GO->at("tank_template"));
		vehicle->positionXYZ = glm::vec3(0, 15.f, 0);
		vehicle->friendlyName = "coordinador";
		vehicle->textures = { "blueTank.bmp","","","" };
		g_GO->insert({vehicle->friendlyName,vehicle});
		this->self = vehicle;
		this->self ->setAT(glm::vec3(0,0,-1));
		this->self->physicsShapeType = UNKNOWN;

		initCircle();
		initLine();
		initSquare();
		initTwoLine();
		initVee();

		for(auto i=0;i<12;i++)
		{
			addVehicle(g_GO);
		}
	}

	void coordinator::update()
	{
		std::vector<glm::vec3>* theFormationOffsets{};
		selectFormationVector(&theFormationOffsets);

		if(!theFormationOffsets) return;

		auto selfMatrix = calculateWorldMatrix(self);
		lookTowardsDirection(self);
		
		for(auto i=0;i<12;i++)
		{
			auto theOffset = (*theFormationOffsets)[i];
			glm::vec4 offVec4(theOffset,1);
			glm::vec4 targV4 = selfMatrix *offVec4;
			
			//glm::vec3 target = (*theFormationOffsets)[i] + self->positionXYZ;
			glm::vec3 target(targV4);
			float dist = glm::distance(vehicles[i]->positionXYZ,target);
			if(dist > 1.f)
			{
				vehicles[i]->velocity = cSteeringBehaviour::Update(
					vehicles[i]->positionXYZ, vehicles[i]->velocity,
					target, glm::vec3(0),
					(1/60.f), steeringType::seek);
				lookTowardsDirection(vehicles[i]);
			}
			else
			{
				vehicles[i]->velocity = glm::vec3(0);
				vehicles[i]->setAT(self->getCurrentAT());
			}
		}
	}

	void coordinator::addVehicle(std::map<std::string,cGameObject*>* g_GO)
	{
		const float randx = randInRange(-195, 195);
		const float randy = randInRange(-118, 118);
		cGameObject* vehicle = new cGameObject(g_GO->at("tank_template"));
		// determine randomly??
		vehicle->positionXYZ = glm::vec3(randx, 15.0f, randy);
		vehicle->friendlyName = "vehicle_" + std::to_string(vehicle->getUniqueID());
		vehicle->isVisible = true;
		vehicle->textures = { "blueTank.bmp","","","" };

		g_GO->insert({vehicle->friendlyName,vehicle});
		this->vehicles.push_back(vehicle);
	}

	void coordinator::lookTowardsDirection(cGameObject* vehicle)
	{
		if(glm::length(vehicle->velocity)>0.f)
		{
			glm::vec3 desiredAT = glm::normalize(vehicle->velocity);
			vehicle->setAT(desiredAT);
		}
	}

	void coordinator::selectFormationVector(std::vector<glm::vec3>** vecs)
	{
		if(currentFormation == "Circle") *vecs = &circleOffsets;
		if(currentFormation == "Vee") *vecs = &veeOffsets;
		if(currentFormation == "Square") *vecs = &squareOffsets;
		if(currentFormation == "Line") *vecs = &lineOffsets;
		if(currentFormation == "TwoLines") *vecs = &twoLinesOffsets;
	}

	void coordinator::thePaqman(std::map<std::string,cGameObject*>* g_GO)
	{
		auto vehicle = g_GO->begin();
		for(vehicle; vehicle != g_GO->end(); ++vehicle)
		{
			auto theGO = vehicle->second;
			auto pos = theGO->positionXYZ;

			if(theGO->inverseMass < 1.f) continue;
			
			if (pos.x > 200.f) theGO->positionXYZ.x = -200.f;
			if (pos.x < -200.f) theGO->positionXYZ.x = 200.f;
			
			if (pos.z > 120.f) theGO->positionXYZ.z = -120.f;
			if (pos.z < -120.f) theGO->positionXYZ.z = 120.f;
		}
	}

	void coordinator::initCircle()
	{
		// circle
		circleOffsets.push_back(glm::vec3(50,15,0));
		circleOffsets.push_back(glm::vec3(-50,15,0));
		circleOffsets.push_back(glm::vec3(0,15,50));
		circleOffsets.push_back(glm::vec3(0,15,-50));
		circleOffsets.push_back(glm::vec3(45,15,25));
		circleOffsets.push_back(glm::vec3(25,15,45));
		circleOffsets.push_back(glm::vec3(-45,15,25));
		circleOffsets.push_back(glm::vec3(-25,15,45));
		circleOffsets.push_back(glm::vec3(45,15,-25));
		circleOffsets.push_back(glm::vec3(25,15,-45));
		circleOffsets.push_back(glm::vec3(-45,15,-25));
		circleOffsets.push_back(glm::vec3(-25,15,-45));
	}
	
	void coordinator::initVee()
	{
		// vee
		veeOffsets.push_back(glm::vec3(20,15,0));
		veeOffsets.push_back(glm::vec3(-20,15,0));
		veeOffsets.push_back(glm::vec3(30,15,-10));
		veeOffsets.push_back(glm::vec3(-30,15,-10));
		veeOffsets.push_back(glm::vec3(40,15,-20));
		veeOffsets.push_back(glm::vec3(-40,15,-20));
		veeOffsets.push_back(glm::vec3(50,15,-30));
		veeOffsets.push_back(glm::vec3(-50,15,-30));
		veeOffsets.push_back(glm::vec3(60,15,-40));
		veeOffsets.push_back(glm::vec3(-60,15,-40));
		veeOffsets.push_back(glm::vec3(70,15,-50));
		veeOffsets.push_back(glm::vec3(-70,15,-50));
	}
	
	void coordinator::initSquare()
	{
		// square
		squareOffsets.push_back(glm::vec3(40,15,40));
		squareOffsets.push_back(glm::vec3(-40,15,-40));
		squareOffsets.push_back(glm::vec3(40,15,-40));
		squareOffsets.push_back(glm::vec3(-40,15,40));
		squareOffsets.push_back(glm::vec3(40,15,20));
		squareOffsets.push_back(glm::vec3(40,15,-20));
		squareOffsets.push_back(glm::vec3(20,15,40));
		squareOffsets.push_back(glm::vec3(-20,15,40));
		squareOffsets.push_back(glm::vec3(-40,15,20));
		squareOffsets.push_back(glm::vec3(-40,15,-20));
		squareOffsets.push_back(glm::vec3(20,15,-40));
		squareOffsets.push_back(glm::vec3(-20,15,-40));
	}
	
	void coordinator::initLine()
	{
		// line
		lineOffsets.push_back(glm::vec3(7.5,15,0));
		lineOffsets.push_back(glm::vec3(20,15,0));
		lineOffsets.push_back(glm::vec3(35,15,0));
		lineOffsets.push_back(glm::vec3(50,15,0));
		lineOffsets.push_back(glm::vec3(65,15,0));
		lineOffsets.push_back(glm::vec3(80,15,0));
		
		lineOffsets.push_back(glm::vec3(-7.5,15,0));
		lineOffsets.push_back(glm::vec3(-20,15,0));
		lineOffsets.push_back(glm::vec3(-35,15,0));
		lineOffsets.push_back(glm::vec3(-50,15,0));
		lineOffsets.push_back(glm::vec3(-65,15,0));
		lineOffsets.push_back(glm::vec3(-80,15,0));
		
	}

	void coordinator::initTwoLine()
	{
		twoLinesOffsets.push_back(glm::vec3(7.5,15,0));
		twoLinesOffsets.push_back(glm::vec3(-7.5,15,0));
		twoLinesOffsets.push_back(glm::vec3(20,15,0));
		twoLinesOffsets.push_back(glm::vec3(-20,15,0));
		twoLinesOffsets.push_back(glm::vec3(35,15,0));
		twoLinesOffsets.push_back(glm::vec3(-35,15,0));
		
		twoLinesOffsets.push_back(glm::vec3(7.5,15,20));
		twoLinesOffsets.push_back(glm::vec3(-7.5,15,20));
		twoLinesOffsets.push_back(glm::vec3(20,15,20));
		twoLinesOffsets.push_back(glm::vec3(-20,15,20));
		twoLinesOffsets.push_back(glm::vec3(35,15,20));
		twoLinesOffsets.push_back(glm::vec3(-35,15,20));
	}

	glm::mat4 coordinator::calculateWorldMatrix(cGameObject* pCurrentObject)
	{
		glm::mat4 matWorld = glm::mat4(1.0f);
		// ******* TRANSLATION TRANSFORM *********
		glm::mat4 matTrans
			= glm::translate(glm::mat4(1.0f),
				glm::vec3(pCurrentObject->positionXYZ.x,
					pCurrentObject->positionXYZ.y,
					pCurrentObject->positionXYZ.z));
		matWorld = matWorld * matTrans;
		// ******* TRANSLATION TRANSFORM *********

		// ******* ROTATION TRANSFORM *********
		glm::mat4 matRotation = glm::mat4(pCurrentObject->getQOrientation());
		matWorld = matWorld * matRotation;
		// ******* ROTATION TRANSFORM *********

		// ******* SCALE TRANSFORM *********
		glm::mat4 scale = glm::scale(glm::mat4(1.0f),
			glm::vec3(pCurrentObject->scale,
				pCurrentObject->scale,
				pCurrentObject->scale));
		matWorld = matWorld * scale;
		// ******* SCALE TRANSFORM *********
		return matWorld;
	}

	void coordinator::correctVerticalAlignment()
	{
		for(auto i=0;i<12;i++)
		{
			vehicles[i]->positionXYZ.y = 15.f;
		}
	}
	
}
